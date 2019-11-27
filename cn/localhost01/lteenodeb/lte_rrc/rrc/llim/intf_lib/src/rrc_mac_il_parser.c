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
 *  File Description : The file rrc_mac_il_parser.c contains the definitions 
 *                     of rrc-mac interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_mac_il_parser.h"
#include "rrc_mac_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_rrc_mac_tdd_config
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_preamble_group_a_config
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacRachCeLevelInfo
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacRachConfigurationR13
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_rach_config_info
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_phich_config_info
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pucch_config_info
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_rbs_for_dci_1a
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_rbs_for_dci_1c
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_mib_msg_req
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sibtype1_msg_req
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_num_si_repetitions
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_num_si_transmissions
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_si_start_offset_info
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_si_segment_data
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_si_msg_param
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sched_reqd_for_sib8_info
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_si_msg_info
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_si_msg_req
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_srs_max_uppts
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_srs_common_setup_info
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_srs_common_config
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pdsch_config_common_info
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_downlink_power_control_common
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_uplink_power_control_common
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pusch_config_common_info
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_freq_selective_scheduling
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_contn_free_rach_timer_info
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tpc_rnti_range
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_power_control_enable
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_explicit_cell_start_required
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ul_sync_loss_timer_info
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pucch_cqi_sinr_threshold_info
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_n_gap
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_crnti_range
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dynamic_pdcch_info
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_bandwidth_info
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_subframes_info
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_config_index_info
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_transmission_power_info
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_muting_config_info
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_prs_config_info
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_m2ap_mac_csa_pattern_list
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_m2ap_embms_sib2_info
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_m2ap_mac_embms_sib13_info
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_m2ap_embms_notification_config
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_m2ap_mac_mcch_config
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_m2ap_mbms_create_area_info
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_m2ap_mac_create_embms_area_entity_info
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_m2ap_mac_delete_embms_area_entity_info
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_m2ap_mac_embms_transmission_power_info
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_m2ap_mac_embms_params_info
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_radio_res_dedicated_laa_scell_config
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_dmtc_configuration_r13
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcEmtcDataConfig
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPrachStartSfR13
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMaxNumPreambleAttemptCeR13
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPrachParametersCeR13
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPrachHoppingOffsetR13
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPrachConfigV1310
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacSibtype1BrMsgInfo
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacSiMsgsBr
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacFddDownlinkOrTddSfBitmapBr
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacFddUplinkSfBitmapBr
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacSiMsgBrInfo
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcN1PucchAnInfoListR13
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPucchConfigCommonV1310
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacModificationPeriodV1310
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPcchConfigV1310
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPdschConfigCommonV1310
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPuschConfigCommonV1310
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcFrequancyHoppingParametersR13
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_config_cell_error_code
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_dl_earfcn
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_timestamp_reqd
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sfn
(
    rrc_mac_sfn_t *p_rrc_mac_sfn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sf
(
    rrc_mac_sf_t *p_rrc_mac_sf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_timestamp
(
    rrc_timestamp_t *p_rrc_timestamp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dci_format_for_si
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_mod_period_info
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sfn_gap_info
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacEmtcSfn
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacSfnGapInfoV1310
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sr_setup_info
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sr_config_info
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sr_prohbit_timer_value_v920
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_aperiodic_csi_trigger_r10
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_aperiodic_config_info
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ri_config_index_info
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_pmi_config_index_info
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cqi_pmi_config_index2_r10
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_csi_config_index_v10
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_pmi_config_index_info_v1020
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_periodic_config_info
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_mask_r9
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pmi_ri_report_r9
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_report_config_v920
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_csi_meas_subframe_set1_r10
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_csi_meas_subframe_set2_r10
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_csi_subframe_pattern_config_v10
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cqi_info
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tdd_ack_nack_feedback_mode
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pusch_config_info
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_srs_dedicated_setup_info
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_srs_dedicated_config
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_codebook_subset_restriction
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_short_drx_config
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_cmd_l2_trigger_info
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_drx_setup_config_info
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_drx_config_info
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_meas_gap_setup_config_info
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_meas_gap_config_info
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_codebook_subset_restriction_v920
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_uplink_power_control_dedicated
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tpc_rnti_info
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tpc_pdcch_config_pucch
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tpc_pdcch_config_pusch
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ambr_qos_info
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_user_location_type
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ue_capability_additional_params
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_n1_pucch_an_persist_list
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_max_sps_harq_retx
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_explicit_release_after
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_dl_setup_info
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_dl_info
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_p_zero_persistent
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_two_intervals_config
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_ul_setup_info
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_ul_info
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_config
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_rf_params
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ue_cat
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ul_mu_mimo_info
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_phr_timer_info
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_phr_timer_config
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_min_ue_power
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ue_compliance_info
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pdsch_epre_to_ue_rs_ratio
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tti_bundling_info
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_operator_info
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_codebook_subset_restriction_v1020
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_interference_info
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcAckNackRepetitionSetupR13
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPucchAckNackRepetitionR13
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPucchNumRepetitionCeR13SetupModeA
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPucchNumRepetitionCeR13
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_PucchConfigDedicatedR13
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_MpdcchConfigSetupR13
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_MpdcchConfigR13
(
    MpdcchConfigR13_t *p_MpdcchConfigR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_EpdcchSetConfigR11
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacCeLevel
(
    RrcMacCeLevel_t *p_RrcMacCeLevel,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacCeMode
(
    RrcMacCeMode_t *p_RrcMacCeMode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ue_category_v1020
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_n1pucch_an_cs_r10
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_n1pucch_an_cs_list_r10
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pucch_channel_selection_config_v1020
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pucch_format_r10
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_scell_deactivation_timer
(
    scell_deactivation_timer_t *p_scell_deactivation_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_supported_mimo_capability_dl_r10
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_tx_mode_info
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pdsch_config_info
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_num_harq_process
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_init_mcs
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_num_of_layer_info
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_code_book_index_info
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_scell_activate_status
(
    scell_activate_status_t *p_scell_activate_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_earfcn
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_scell_info
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ca_lc_bandwidth_dist_info
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_carrier_aggr_config
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_add_ue_info
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_configure_status_report_required_ue
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ul_lc_create_req
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_configure_status_report_required_enb
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_lc_create_req
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_gbr_qos_info
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_qos_info
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_rohc_profile
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_create_lc_req
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_uplink_rat1_info
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ul_lc_config_resp
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_lc_config_resp
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_create_lc_error
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_sps_crnti_info
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ca_config_error
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_delete_lc_status
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_max_rb_info
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_time_alignment_timer_info
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_p0_ue_pusch
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_max_ul_harq_tx_info
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_pc_max_config_info
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacUeCategoryV1310
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_reconfig_ue_info
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ul_lc_reconfig_req
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_lc_reconfig_req
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_reconfigure_lc_req
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_delete_lc_req
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_reconfigure_lc_error
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_delete_lc_error
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_ca_reconfig_error
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_delete_ca_config_error
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPagingNarrowband
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacMpdcchNumRepetitionPaging
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacPdschNumRepetition
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMacDirectIndication
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_intra_enb_ho_req
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_crnti_info
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_preamble_info
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_scell_rel_flag
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrc_mac_config_cell_req
(
    rrc_mac_config_cell_req_t *p_rrc_mac_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_config_cell_req, 0, sizeof(rrc_mac_config_cell_req_t));

    /* This function parses rrc_mac_config_cell_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->duplexing_mode, p_src + *p_length_read, "duplexing_mode");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->dl_res_blocks, p_src + *p_length_read, "dl_res_blocks");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->dl_res_blocks < 1) || (p_rrc_mac_config_cell_req->dl_res_blocks > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->dl_res_blocks] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_config_cell_req->dl_res_blocks);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->ul_res_blocks, p_src + *p_length_read, "ul_res_blocks");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->ul_res_blocks < 1) || (p_rrc_mac_config_cell_req->ul_res_blocks > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->ul_res_blocks] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_config_cell_req->ul_res_blocks);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->max_harq_retrans, p_src + *p_length_read, "max_harq_retrans");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->max_harq_retrans < 1) || (p_rrc_mac_config_cell_req->max_harq_retrans > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->max_harq_retrans] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_config_cell_req->max_harq_retrans);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->num_of_tx_antennas, p_src + *p_length_read, "num_of_tx_antennas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->num_of_tx_antennas < 1) || (p_rrc_mac_config_cell_req->num_of_tx_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->num_of_tx_antennas] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_config_cell_req->num_of_tx_antennas);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix, p_src + *p_length_read, "ul_cyclic_lenth_prefix");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix < 1) || (p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->start_ra_rnti_range, p_src + *p_length_read, "start_ra_rnti_range");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->start_ra_rnti_range < 1) || (p_rrc_mac_config_cell_req->start_ra_rnti_range > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->start_ra_rnti_range] should be in range "
            "1 to 60. Incorrect value %u received.", p_rrc_mac_config_cell_req->start_ra_rnti_range);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->end_ra_rnti_range, p_src + *p_length_read, "end_ra_rnti_range");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->end_ra_rnti_range < 1) || (p_rrc_mac_config_cell_req->end_ra_rnti_range > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->end_ra_rnti_range] should be in range "
            "1 to 60. Incorrect value %u received.", p_rrc_mac_config_cell_req->end_ra_rnti_range);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_config_cell_req->mod_period, p_src + *p_length_read, "mod_period");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->mod_period < 64) || (p_rrc_mac_config_cell_req->mod_period > 4096))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->mod_period] should be in range "
            "64 to 4096. Incorrect value %u received.", p_rrc_mac_config_cell_req->mod_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->sfn_gap, p_src + *p_length_read, "sfn_gap");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->sfn_gap < 1) || (p_rrc_mac_config_cell_req->sfn_gap > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->sfn_gap] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_mac_config_cell_req->sfn_gap);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->nb, p_src + *p_length_read, "nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_req->nb > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->nb] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_config_cell_req->nb);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->cfi_value, p_src + *p_length_read, "cfi_value");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->cfi_value < 1) || (p_rrc_mac_config_cell_req->cfi_value > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->cfi_value] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_config_cell_req->cfi_value);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_config_cell_req->num_of_eul, p_src + *p_length_read, "num_of_eul");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_config_cell_req->size_of_eul, p_src + *p_length_read, "size_of_eul");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_req->dci_format_for_si_msgs, p_src + *p_length_read, "dci_format_for_si_msgs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_req->dci_format_for_si_msgs > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->dci_format_for_si_msgs] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_config_cell_req->dci_format_for_si_msgs);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_TDD_CONFIG:
                /* rrc_mac_tdd_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tdd_config(
                    &p_rrc_mac_config_cell_req->tdd_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_TDD_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RACH_CONFIG_INFO:
                /* rrc_mac_rach_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rach_config_info(
                    &p_rrc_mac_config_cell_req->rach_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_RACH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PHICH_CONFIG_INFO:
                /* rrc_mac_phich_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_phich_config_info(
                    &p_rrc_mac_config_cell_req->phich_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PHICH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CONFIG_INFO:
                /* rrc_mac_pucch_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_config_info(
                    &p_rrc_mac_config_cell_req->pucch_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PUCCH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RBS_FOR_DCI_1A:
                /* rrc_mac_rbs_for_dci_1a TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rbs_for_dci_1a(
                    &p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1a,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RBS_FOR_DCI_1C:
                /* rrc_mac_rbs_for_dci_1c TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rbs_for_dci_1c(
                    &p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1c,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MIB_MSG_REQ:
                /* rrc_mac_mib_msg_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_config_cell_req->mib_msg_req_counter >= (S32)ARRSIZE(p_rrc_mac_config_cell_req->mib_msg_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_mib_msg_req(
                    &p_rrc_mac_config_cell_req->mib_msg_req[p_rrc_mac_config_cell_req->mib_msg_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->mib_msg_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SIBTYPE1_MSG_REQ:
                /* rrc_mac_sibtype1_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sibtype1_msg_req(
                    &p_rrc_mac_config_cell_req->sib1_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_INFO:
                /* rrc_mac_si_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_msg_req(
                    &p_rrc_mac_config_cell_req->si_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SI_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SRS_COMMON_CONFIG:
                /* rrc_mac_srs_common_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_common_config(
                    &p_rrc_mac_config_cell_req->srs_common_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SRS_COMMON_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_COMMON_INFO:
                /* rrc_mac_pdsch_config_common_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_config_common_info(
                    &p_rrc_mac_config_cell_req->pdsch_config_common_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON:
                /* rrc_mac_downlink_power_control_common TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_downlink_power_control_common(
                    &p_rrc_mac_config_cell_req->downlink_power_control_common,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UPLINK_POWER_CONTROL_COMMON:
                /* rrc_mac_uplink_power_control_common TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_uplink_power_control_common(
                    &p_rrc_mac_config_cell_req->uplink_power_control_common,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_COMMON:
                /* rrc_mac_pusch_config_common_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pusch_config_common_info(
                    &p_rrc_mac_config_cell_req->pusch_config_common_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_FREQ_SELECTIVE_ENABLE:
                /* rrc_mac_freq_selective_scheduling TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_freq_selective_scheduling(
                    &p_rrc_mac_config_cell_req->freq_selective_scheduling,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CONTN_FREE_RACH_TIMER_INFO:
                /* rrc_mac_contn_free_rach_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_contn_free_rach_timer_info(
                    &p_rrc_mac_config_cell_req->contn_free_rach_timer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TPC_RNTI_RANGE:
                /* rrc_mac_tpc_rnti_range TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_rnti_range(
                    &p_rrc_mac_config_cell_req->tpc_rnti_range,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_TPC_RNTI_RANGE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_POWER_CONTROL_ENABLE:
                /* rrc_mac_power_control_enable TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_power_control_enable(
                    &p_rrc_mac_config_cell_req->power_control_enable,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_POWER_CONTROL_ENABLE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_EXPLICIT_CELL_START_REQUIRED:
                /* rrc_mac_explicit_cell_start_required TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_explicit_cell_start_required(
                    &p_rrc_mac_config_cell_req->explicit_cell_start_required,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UL_SYNC_LOSS_TIMER_INFO:
                /* rrc_mac_ul_sync_loss_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_sync_loss_timer_info(
                    &p_rrc_mac_config_cell_req->ul_sync_loss_timer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO:
                /* rrc_mac_pucch_cqi_sinr_threshold_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_cqi_sinr_threshold_info(
                    &p_rrc_mac_config_cell_req->pucch_cqi_sinr_threshold_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_N_GAP:
                /* rrc_mac_n_gap TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_n_gap(
                    &p_rrc_mac_config_cell_req->n_gap,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_N_GAP_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SPS_CRNTI_RANGE:
                /* rrc_mac_sps_crnti_range TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_crnti_range(
                    &p_rrc_mac_config_cell_req->sps_crnti_range,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SPS_CRNTI_RANGE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DYNAMIC_PDCCH_INFO:
                /* rrc_mac_dynamic_pdcch_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dynamic_pdcch_info(
                    &p_rrc_mac_config_cell_req->dynamic_pdcch_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_CONFIG_INFO:
                /* rrc_mac_prs_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_config_info(
                    &p_rrc_mac_config_cell_req->prs_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PRS_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_EMBMS_SIB2_INFO:
                /* rrc_m2ap_embms_sib2_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_embms_sib2_info(
                    &p_rrc_mac_config_cell_req->rrc_m2ap_embms_sib2_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_SIB13_INFO:
                /* rrc_m2ap_mac_embms_sib13_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_embms_sib13_info(
                    &p_rrc_mac_config_cell_req->rrc_m2ap_mac_sib13_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_CONFIG_SIB13_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO:
                /* rrc_m2ap_mac_create_embms_area_entity_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_create_embms_area_entity_info(
                    &p_rrc_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO:
                /* rrc_m2ap_mac_delete_embms_area_entity_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_delete_embms_area_entity_info(
                    &p_rrc_mac_config_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_M2AP_CONFIG_MBMS_AREA_DELETE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_EMBMS_CONFIG:
                /* rrc_m2ap_mac_embms_params_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_embms_params_info(
                    &p_rrc_mac_config_cell_req->rrc_m2ap_mac_embms_params_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_LAA_SCELL_CONFIG_INFO:
                /* rrc_radio_res_dedicated_laa_scell_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_dedicated_laa_scell_config(
                    &p_rrc_mac_config_cell_req->laa_scell_configuration_r13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_LAA_SCELL_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DMTC_CONFIG_INFO:
                /* rrc_dmtc_configuration_r13 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_dmtc_configuration_r13(
                    &p_rrc_mac_config_cell_req->dmtc_configuration_r13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_DMTC_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_EMTC_DATA_CONFIG:
                /* RrcEmtcDataConfig TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcEmtcDataConfig(
                    &p_rrc_mac_config_cell_req->emtcDataConfig,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_EMTC_DATA_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRACH_CONFIG_V1310:
                /* RrcMacPrachConfigV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPrachConfigV1310(
                    &p_rrc_mac_config_cell_req->prachConfigV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PRACH_CONFIG_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SIBTYPE1_BR_MSG_INFO:
                /* RrcMacSibtype1BrMsgInfo TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacSibtype1BrMsgInfo(
                    &p_rrc_mac_config_cell_req->sibType1BrMsgInfo,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_BR_INFO:
                /* RrcMacSiMsgBrInfo TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacSiMsgBrInfo(
                    &p_rrc_mac_config_cell_req->siMsgBrInfo,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_SI_MSG_BR_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CONFIG_COMMON_V1310:
                /* RrcMacPucchConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPucchConfigCommonV1310(
                    &p_rrc_mac_config_cell_req->pucchConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MODIFICATION_PERIOD_V1310:
                /* RrcMacModificationPeriodV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacModificationPeriodV1310(
                    &p_rrc_mac_config_cell_req->modificationPeriodV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_MODIFICATION_PERIOD_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PCCH_CONFIG_V1310:
                /* RrcMacPcchConfigV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPcchConfigV1310(
                    &p_rrc_mac_config_cell_req->pcchConfigV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PCCH_CONFIG_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_COMMON_V1310:
                /* RrcMacPdschConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPdschConfigCommonV1310(
                    &p_rrc_mac_config_cell_req->pdschConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_COMMON_V1310:
                /* RrcMacPuschConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPuschConfigCommonV1310(
                    &p_rrc_mac_config_cell_req->puschConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13:
                /* RrcFrequancyHoppingParametersR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcFrequancyHoppingParametersR13(
                    &p_rrc_mac_config_cell_req->frequancyHoppingParametersR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_req->optional_elems_present |= RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_tdd_config
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tdd_config, 0, sizeof(rrc_mac_tdd_config_t));

    /* This function parses rrc_mac_tdd_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tdd_config->sub_frame_assignment, p_src + *p_length_read, "sub_frame_assignment");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_config->sub_frame_assignment > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_config->sub_frame_assignment] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_mac_tdd_config->sub_frame_assignment);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tdd_config->special_sub_frame_pattern, p_src + *p_length_read, "special_sub_frame_pattern");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_config->special_sub_frame_pattern > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_config->special_sub_frame_pattern] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_tdd_config->special_sub_frame_pattern);
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
rrc_il_parse_rrc_mac_preamble_group_a_config
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_preamble_group_a_config, 0, sizeof(rrc_mac_preamble_group_a_config_t));

    /* This function parses rrc_mac_preamble_group_a_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a, p_src + *p_length_read, "num_of_preambles_in_group_a");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a < 4) || (p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a] should be in range "
            "4 to 60. Incorrect value %u received.", p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_preamble_group_a_config->message_size_group_a, p_src + *p_length_read, "message_size_group_a");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_preamble_group_a_config->message_size_group_a < 56) || (p_rrc_mac_preamble_group_a_config->message_size_group_a > 256))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_group_a_config->message_size_group_a] should be in range "
            "56 to 256. Incorrect value %u received.", p_rrc_mac_preamble_group_a_config->message_size_group_a);
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
rrc_il_parse_RrcMacRachCeLevelInfo
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacRachCeLevelInfo, 0, sizeof(RrcMacRachCeLevelInfo_t));

    /* This function parses RrcMacRachCeLevelInfo */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13, p_src + *p_length_read, "raResponseWindowSizeR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13 > EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13] should be less than"
            " or equal to EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13, p_src + *p_length_read, "macContentionResolutionTimerR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13 > EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13] should be less than"
            " or equal to EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachCeLevelInfo->rarHoppingConfigR13, p_src + *p_length_read, "rarHoppingConfigR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->rarHoppingConfigR13 > EMTC_RAR_HOPPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->rarHoppingConfigR13] should be less than"
            " or equal to EMTC_RAR_HOPPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->rarHoppingConfigR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachCeLevelInfo->firstPreambleR13, p_src + *p_length_read, "firstPreambleR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->firstPreambleR13 > EMTC_MAX_PREAMBLE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->firstPreambleR13] should be less than"
            " or equal to EMTC_MAX_PREAMBLE. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->firstPreambleR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachCeLevelInfo->lastPreambleR13, p_src + *p_length_read, "lastPreambleR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->lastPreambleR13 > EMTC_MAX_PREAMBLE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->lastPreambleR13] should be less than"
            " or equal to EMTC_MAX_PREAMBLE. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->lastPreambleR13);
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
rrc_il_parse_RrcMacRachConfigurationR13
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacRachConfigurationR13, 0, sizeof(RrcMacRachConfigurationR13_t));

    /* This function parses RrcMacRachConfigurationR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacRachConfigurationR13->numCeLevels, p_src + *p_length_read, "numCeLevels");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacRachConfigurationR13->numCeLevels < 1) || (p_RrcMacRachConfigurationR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachConfigurationR13->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacRachConfigurationR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_RACH_CE_LEVEL_INFO_LIST:
                /* RrcMacRachCeLevelInfo TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_RrcMacRachConfigurationR13->numCeLevelsCounter >= (S32)ARRSIZE(p_RrcMacRachConfigurationR13->rachCeLevelInfo))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_RrcMacRachCeLevelInfo(
                    &p_RrcMacRachConfigurationR13->rachCeLevelInfo[p_RrcMacRachConfigurationR13->numCeLevelsCounter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacRachConfigurationR13->numCeLevelsCounter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_rach_config_info
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_rach_config_info, 0, sizeof(rrc_mac_rach_config_info_t));

    /* This function parses rrc_mac_rach_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->ra_response_window_size, p_src + *p_length_read, "ra_response_window_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->ra_response_window_size < 2) || (p_rrc_mac_rach_config_info->ra_response_window_size > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->ra_response_window_size] should be in range "
            "2 to 10. Incorrect value %u received.", p_rrc_mac_rach_config_info->ra_response_window_size);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->mac_contention_resolution_timer, p_src + *p_length_read, "mac_contention_resolution_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->mac_contention_resolution_timer < 8) || (p_rrc_mac_rach_config_info->mac_contention_resolution_timer > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->mac_contention_resolution_timer] should be in range "
            "8 to 64. Incorrect value %u received.", p_rrc_mac_rach_config_info->mac_contention_resolution_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->max_harq_msg3tx, p_src + *p_length_read, "max_harq_msg3tx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->max_harq_msg3tx < 1) || (p_rrc_mac_rach_config_info->max_harq_msg3tx > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->max_harq_msg3tx] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_rach_config_info->max_harq_msg3tx);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->prach_configuration_index, p_src + *p_length_read, "prach_configuration_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->prach_configuration_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->prach_configuration_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_rach_config_info->prach_configuration_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_rrc_mac_rach_config_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->num_of_contention_based_preambles, p_src + *p_length_read, "num_of_contention_based_preambles");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->num_of_contention_based_preambles < 4) || (p_rrc_mac_rach_config_info->num_of_contention_based_preambles > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->num_of_contention_based_preambles] should be in range "
            "4 to 64. Incorrect value %u received.", p_rrc_mac_rach_config_info->num_of_contention_based_preambles);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rach_config_info->preamble_initial_received_target_power, p_src + *p_length_read, "preamble_initial_received_target_power");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->preamble_initial_received_target_power > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->preamble_initial_received_target_power] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_rach_config_info->preamble_initial_received_target_power);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PREAMBLE_GROUP_A_CONFIG:
                /* rrc_mac_preamble_group_a_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_preamble_group_a_config(
                    &p_rrc_mac_rach_config_info->preamble_group_a_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_rach_config_info->optional_elems_present |= RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RACH_CONFIGURATION_R13:
                /* RrcMacRachConfigurationR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacRachConfigurationR13(
                    &p_rrc_mac_rach_config_info->rachConfigurationR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_rach_config_info->optional_elems_present |= RRC_MAC_RACH_CONFIGURATION_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_phich_config_info
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_phich_config_info, 0, sizeof(rrc_mac_phich_config_info_t));

    /* This function parses rrc_mac_phich_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_phich_config_info->phich_duration, p_src + *p_length_read, "phich_duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phich_config_info->phich_duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phich_config_info->phich_duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_phich_config_info->phich_duration);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_phich_config_info->phich_resource, p_src + *p_length_read, "phich_resource");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phich_config_info->phich_resource > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phich_config_info->phich_resource] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_phich_config_info->phich_resource);
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
rrc_il_parse_rrc_mac_pucch_config_info
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pucch_config_info, 0, sizeof(rrc_mac_pucch_config_info_t));

    /* This function parses rrc_mac_pucch_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pucch_config_info->delta_pucch_shift, p_src + *p_length_read, "delta_pucch_shift");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pucch_config_info->delta_pucch_shift < 1) || (p_rrc_mac_pucch_config_info->delta_pucch_shift > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->delta_pucch_shift] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_mac_pucch_config_info->delta_pucch_shift);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pucch_config_info->nrb_cqi, p_src + *p_length_read, "nrb_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->nrb_cqi > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->nrb_cqi] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_mac_pucch_config_info->nrb_cqi);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pucch_config_info->ncs_an, p_src + *p_length_read, "ncs_an");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->ncs_an > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->ncs_an] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_pucch_config_info->ncs_an);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_pucch_config_info->n1pucch_an, p_src + *p_length_read, "n1pucch_an");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->n1pucch_an > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->n1pucch_an] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_pucch_config_info->n1pucch_an);
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
rrc_il_parse_rrc_mac_rbs_for_dci_1a
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_rbs_for_dci_1a, 0, sizeof(rrc_mac_rbs_for_dci_1a_t));

    /* This function parses rrc_mac_rbs_for_dci_1a */

    if (*p_length_read + (S32)sizeof(p_rrc_mac_rbs_for_dci_1a->num_rb_buffer) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_rbs_for_dci_1a->num_rb_buffer); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop], (void*)(p_src + *p_length_read), "num_rb_buffer[]");
            if (p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop]);
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
rrc_il_parse_rrc_mac_rbs_for_dci_1c
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_rbs_for_dci_1c, 0, sizeof(rrc_mac_rbs_for_dci_1c_t));

    /* This function parses rrc_mac_rbs_for_dci_1c */

    if (*p_length_read + (S32)sizeof(p_rrc_mac_rbs_for_dci_1c->num_rb_buffer) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_rbs_for_dci_1c->num_rb_buffer); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop], (void*)(p_src + *p_length_read), "num_rb_buffer[]");
            if (p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop]);
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
rrc_il_parse_rrc_mac_mib_msg_req
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_mib_msg_req, 0, sizeof(rrc_mac_mib_msg_req_t));

    /* This function parses rrc_mac_mib_msg_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_mib_msg_req->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_mib_msg_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_mib_msg_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_mib_msg_req->sfn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_mac_mib_msg_req->mib_msg_buf) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_mib_msg_req->mib_msg_buf_counter %u is exceeding p_rrc_mac_mib_msg_req->mib_msg_buf array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_mac_mib_msg_req->mib_msg_buf[loop], p_src + *p_length_read + loop, "mib_msg_buf[]");
        }

        p_rrc_mac_mib_msg_req->mib_msg_buf_counter = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_rrc_mac_sibtype1_msg_req
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sibtype1_msg_req, 0, sizeof(rrc_mac_sibtype1_msg_req_t));

    /* This function parses rrc_mac_sibtype1_msg_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sibtype1_msg_req->si_mapping_changed, p_src + *p_length_read, "si_mapping_changed");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sibtype1_msg_req->si_mapping_changed > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sibtype1_msg_req->si_mapping_changed] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sibtype1_msg_req->si_mapping_changed);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sibtype1_msg_req->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sibtype1_msg_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sibtype1_msg_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sibtype1_msg_req->sfn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter %u is exceeding p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf[loop], p_src + *p_length_read + loop, "sibtype1_msg_buf[]");
        }

        p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_rrc_num_si_repetitions
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_num_si_repetitions, 0, sizeof(rrc_num_si_repetitions));

    /* This function parses rrc_num_si_repetitions */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_num_si_repetitions->num_of_repetitions, p_src + *p_length_read, "num_of_repetitions");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_num_si_repetitions->num_of_repetitions > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_num_si_repetitions->num_of_repetitions] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_num_si_repetitions->num_of_repetitions);
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
rrc_il_parse_rrc_num_si_transmissions
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_num_si_transmissions, 0, sizeof(rrc_num_si_transmissions_t));

    /* This function parses rrc_num_si_transmissions */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_num_si_transmissions->num_of_transmissions, p_src + *p_length_read, "num_of_transmissions");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_num_si_transmissions->num_of_transmissions < 1) || (p_rrc_num_si_transmissions->num_of_transmissions > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_num_si_transmissions->num_of_transmissions] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_num_si_transmissions->num_of_transmissions);
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
rrc_il_parse_rrc_mac_si_start_offset_info
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_si_start_offset_info, 0, sizeof(rrc_mac_si_start_offset_info_t));

    /* This function parses rrc_mac_si_start_offset_info */

    if (*p_length_read + (S32)sizeof(p_rrc_mac_si_start_offset_info->si_start_offset) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_si_start_offset_info->si_start_offset); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_si_start_offset_info->si_start_offset[loop], (void*)(p_src + *p_length_read), "si_start_offset[]");
            if (p_rrc_mac_si_start_offset_info->si_start_offset[loop] > 39)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_si_start_offset_info->si_start_offset[%u] should be less"
                    " than or equal to 39. "
                    "Incorrect value %d received.", loop, p_rrc_mac_si_start_offset_info->si_start_offset[loop]);
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
rrc_il_parse_rrc_mac_si_segment_data
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_si_segment_data, 0, sizeof(rrc_mac_si_segment_data_t));

    /* This function parses rrc_mac_si_segment_data */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_si_segment_data->si_msg_buf_length, p_src + *p_length_read, "si_msg_buf_length");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)(p_rrc_mac_si_segment_data->si_msg_buf_length * sizeof(p_rrc_mac_si_segment_data->si_msg_buf[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_si_segment_data->si_msg_buf_length; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_si_segment_data->si_msg_buf[loop], (void*)(p_src + *p_length_read), "si_msg_buf[]");
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
rrc_il_parse_rrc_mac_si_msg_param
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_si_msg_param, 0, sizeof(rrc_mac_si_msg_param_t));

    /* This function parses rrc_mac_si_msg_param */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_param->id, p_src + *p_length_read, "id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_param->id > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_param->id] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_si_msg_param->id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_si_msg_param->offset, p_src + *p_length_read, "offset");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_si_msg_param->length, p_src + *p_length_read, "length");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_sched_reqd_for_sib8_info
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sched_reqd_for_sib8_info, 0, sizeof(rrc_mac_sched_reqd_for_sib8_info_t));

    /* This function parses rrc_mac_sched_reqd_for_sib8_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8, p_src + *p_length_read, "sched_reqd_for_sib8");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8 > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8);
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
rrc_il_parse_rrc_mac_si_msg_info
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_si_msg_info, 0, sizeof(rrc_mac_si_msg_info_t));

    /* This function parses rrc_mac_si_msg_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_info->si_index, p_src + *p_length_read, "si_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->si_index > 11))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->si_index] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_mac_si_msg_info->si_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_info->periodicity, p_src + *p_length_read, "periodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_info->periodicity < 1) || (p_rrc_mac_si_msg_info->periodicity > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->periodicity] should be in range "
            "1 to 7. Incorrect value %u received.", p_rrc_mac_si_msg_info->periodicity);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_si_msg_info->starting_sfn, p_src + *p_length_read, "starting_sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->starting_sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->starting_sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_si_msg_info->starting_sfn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_info->starting_sf, p_src + *p_length_read, "starting_sf");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->starting_sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->starting_sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_si_msg_info->starting_sf);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_info->num_of_segment, p_src + *p_length_read, "num_of_segment");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_info->num_of_segment < 1) || (p_rrc_mac_si_msg_info->num_of_segment > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->num_of_segment] should be in range "
            "1 to 64. Incorrect value %u received.", p_rrc_mac_si_msg_info->num_of_segment);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_NUM_SI_REPETITIONS:
                /* rrc_num_si_repetitions TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_num_si_repetitions(
                    &p_rrc_mac_si_msg_info->num_si_repetitions,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->optional_elems_present |= RRC_NUM_SI_REPETITIONS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_NUM_SI_TRANSMISSIONS:
                /* rrc_num_si_transmissions TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_num_si_transmissions(
                    &p_rrc_mac_si_msg_info->num_si_transmissions,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->optional_elems_present |= RRC_NUM_SI_TRANSMISSIONS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_START_OFFSET:
                /* rrc_mac_si_start_offset_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_start_offset_info(
                    &p_rrc_mac_si_msg_info->si_start_offset_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->optional_elems_present |= RRC_NUM_SI_START_OFFSET_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_SEGMENT_REQ:
                /* rrc_mac_si_segment_data TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_si_msg_info->segment_counter >= (S32)ARRSIZE(p_rrc_mac_si_msg_info->si_segment_data))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_segment_data(
                    &p_rrc_mac_si_msg_info->si_segment_data[p_rrc_mac_si_msg_info->segment_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->segment_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SI_MSG_INFO_PARAM:
                /* rrc_mac_si_msg_param TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_si_msg_info->si_msg_param_counter >= (S32)ARRSIZE(p_rrc_mac_si_msg_info->si_msg_param))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_msg_param(
                    &p_rrc_mac_si_msg_info->si_msg_param[p_rrc_mac_si_msg_info->si_msg_param_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->si_msg_param_counter++;

                *p_length_read += length_read;

                break;
            case RRC_SCHED_REQD_FOR_SIB8:
                /* rrc_mac_sched_reqd_for_sib8_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sched_reqd_for_sib8_info(
                    &p_rrc_mac_si_msg_info->rrc_mac_sched_reqd_for_sib8,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_info->optional_elems_present |= RRC_SCHED_REQD_FOR_SIB8_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_si_msg_req
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_si_msg_req, 0, sizeof(rrc_mac_si_msg_req_t));

    /* This function parses rrc_mac_si_msg_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_req->si_window_size, p_src + *p_length_read, "si_window_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_req->si_window_size < 1) || (p_rrc_mac_si_msg_req->si_window_size > 40))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_req->si_window_size] should be in range "
            "1 to 40. Incorrect value %u received.", p_rrc_mac_si_msg_req->si_window_size);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_si_msg_req->num_si_message, p_src + *p_length_read, "num_si_message");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_req->num_si_message > 74))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_req->num_si_message] should be less than"
            " or equal to 74. Incorrect value %u received.", p_rrc_mac_si_msg_req->num_si_message);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SI_MSG_REQ:
                /* rrc_mac_si_msg_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_si_msg_req->si_msg_info_counter >= (S32)ARRSIZE(p_rrc_mac_si_msg_req->si_msg_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_msg_info(
                    &p_rrc_mac_si_msg_req->si_msg_info[p_rrc_mac_si_msg_req->si_msg_info_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_si_msg_req->si_msg_info_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_srs_max_uppts
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_srs_max_uppts, 0, sizeof(rrc_mac_srs_max_uppts_t));

    /* This function parses rrc_mac_srs_max_uppts */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_max_uppts->max_uppts, p_src + *p_length_read, "max_uppts");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_max_uppts->max_uppts > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_max_uppts->max_uppts] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_srs_max_uppts->max_uppts);
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
rrc_il_parse_rrc_mac_srs_common_setup_info
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_srs_common_setup_info, 0, sizeof(rrc_mac_srs_common_setup_info_t));

    /* This function parses rrc_mac_srs_common_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_common_setup_info->srs_bandwidth_config, p_src + *p_length_read, "srs_bandwidth_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->srs_bandwidth_config > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->srs_bandwidth_config] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->srs_bandwidth_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_common_setup_info->srs_subframe_config, p_src + *p_length_read, "srs_subframe_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->srs_subframe_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->srs_subframe_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->srs_subframe_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans, p_src + *p_length_read, "ack_nack_srs_simultaneous_trans");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SRS_MAX_UPPTS:
                /* rrc_mac_srs_max_uppts TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_max_uppts(
                    &p_rrc_mac_srs_common_setup_info->srs_max_uppts,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_srs_common_setup_info->optional_elems_present |= RRC_MAC_SRS_MAX_UPPTS_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_srs_common_config
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_srs_common_config, 0, sizeof(rrc_mac_srs_common_config_t));

    /* This function parses rrc_mac_srs_common_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_common_config->config_type, p_src + *p_length_read, "config_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_config->config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_config->config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_common_config->config_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SRS_COMMON_SETUP_INFO:
                /* rrc_mac_srs_common_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_common_setup_info(
                    &p_rrc_mac_srs_common_config->srs_common_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_srs_common_config->optional_elems_present |= RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_pdsch_config_common_info
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pdsch_config_common_info, 0, sizeof(rrc_mac_pdsch_config_common_info_t));

    /* This function parses rrc_mac_pdsch_config_common_info */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_pdsch_config_common_info->reference_signal_power, p_src + *p_length_read, "reference_signal_power");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pdsch_config_common_info->reference_signal_power < -60) || (p_rrc_mac_pdsch_config_common_info->reference_signal_power > 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_common_info->reference_signal_power] should be in range "
            "-60 to 50. Incorrect value %d received.", p_rrc_mac_pdsch_config_common_info->reference_signal_power);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pdsch_config_common_info->p_b, p_src + *p_length_read, "p_b");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pdsch_config_common_info->p_b > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_common_info->p_b] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_pdsch_config_common_info->p_b);
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
rrc_il_parse_rrc_mac_downlink_power_control_common
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_downlink_power_control_common, 0, sizeof(rrc_mac_downlink_power_control_common_t));

    /* This function parses rrc_mac_downlink_power_control_common */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_downlink_power_control_common->pcfichPowerOffset, p_src + *p_length_read, "pcfichPowerOffset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pcfichPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pcfichPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pcfichPowerOffset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_downlink_power_control_common->phichPowerOffset, p_src + *p_length_read, "phichPowerOffset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->phichPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->phichPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->phichPowerOffset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_downlink_power_control_common->pdcchPowerOffset, p_src + *p_length_read, "pdcchPowerOffset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pdcchPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pdcchPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pdcchPowerOffset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_downlink_power_control_common->pbchTransmissionPower, p_src + *p_length_read, "pbchTransmissionPower");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pbchTransmissionPower > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pbchTransmissionPower] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pbchTransmissionPower);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_downlink_power_control_common->pchTransmissionpower, p_src + *p_length_read, "pchTransmissionpower");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pchTransmissionpower > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pchTransmissionpower] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pchTransmissionpower);
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
rrc_il_parse_rrc_mac_uplink_power_control_common
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_uplink_power_control_common, 0, sizeof(rrc_mac_uplink_power_control_common_t));

    /* This function parses rrc_mac_uplink_power_control_common */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_uplink_power_control_common->p0_nominal_pusch, p_src + *p_length_read, "p0_nominal_pusch");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_uplink_power_control_common->p0_nominal_pusch < -126) || (p_rrc_mac_uplink_power_control_common->p0_nominal_pusch > 24))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_common->p0_nominal_pusch] should be in range "
            "-126 to 24. Incorrect value %d received.", p_rrc_mac_uplink_power_control_common->p0_nominal_pusch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_uplink_power_control_common->alpha, p_src + *p_length_read, "alpha");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_common->alpha > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_common->alpha] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_uplink_power_control_common->alpha);
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
rrc_il_parse_rrc_mac_pusch_config_common_info
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pusch_config_common_info, 0, sizeof(rrc_mac_pusch_config_common_info_t));

    /* This function parses rrc_mac_pusch_config_common_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pusch_config_common_info->enable_64_qam, p_src + *p_length_read, "enable_64_qam");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_common_info->enable_64_qam > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_common_info->enable_64_qam] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pusch_config_common_info->enable_64_qam);
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
rrc_il_parse_rrc_mac_freq_selective_scheduling
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_freq_selective_scheduling, 0, sizeof(rrc_mac_freq_selective_scheduling_t));

    /* This function parses rrc_mac_freq_selective_scheduling */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable, p_src + *p_length_read, "ul_freq_selective_enable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable, p_src + *p_length_read, "dl_freq_selective_enable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable);
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
rrc_il_parse_rrc_mac_contn_free_rach_timer_info
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_contn_free_rach_timer_info, 0, sizeof(rrc_mac_contn_free_rach_timer_info_t));

    /* This function parses rrc_mac_contn_free_rach_timer_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer, p_src + *p_length_read, "contn_free_rach_timer");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer < 25) || (p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer > 200))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer] should be in range "
            "25 to 200. Incorrect value %u received.", p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer);
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
rrc_il_parse_rrc_mac_tpc_rnti_range
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tpc_rnti_range, 0, sizeof(rrc_mac_tpc_rnti_range_t));

    /* This function parses rrc_mac_tpc_rnti_range */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tpc_rnti_range->startTpcRntiPucch, p_src + *p_length_read, "startTpcRntiPucch");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->startTpcRntiPucch < 61) || (p_rrc_mac_tpc_rnti_range->startTpcRntiPucch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->startTpcRntiPucch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->startTpcRntiPucch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tpc_rnti_range->endTpcRntiPucch, p_src + *p_length_read, "endTpcRntiPucch");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->endTpcRntiPucch < 61) || (p_rrc_mac_tpc_rnti_range->endTpcRntiPucch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->endTpcRntiPucch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->endTpcRntiPucch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tpc_rnti_range->startTpcRntiPusch, p_src + *p_length_read, "startTpcRntiPusch");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->startTpcRntiPusch < 61) || (p_rrc_mac_tpc_rnti_range->startTpcRntiPusch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->startTpcRntiPusch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->startTpcRntiPusch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tpc_rnti_range->endTpcRntiPusch, p_src + *p_length_read, "endTpcRntiPusch");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->endTpcRntiPusch < 61) || (p_rrc_mac_tpc_rnti_range->endTpcRntiPusch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->endTpcRntiPusch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->endTpcRntiPusch);
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
rrc_il_parse_rrc_mac_power_control_enable
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_power_control_enable, 0, sizeof(rrc_mac_power_control_enable_t));

    /* This function parses rrc_mac_power_control_enable */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable, p_src + *p_length_read, "harqBlerClpcPucchEnable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable, p_src + *p_length_read, "cqiSinrClpcPucchEnable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_power_control_enable->clpcPuschEnable, p_src + *p_length_read, "clpcPuschEnable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->clpcPuschEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->clpcPuschEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->clpcPuschEnable);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable, p_src + *p_length_read, "pdcchPowOrAggregationEnable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable, p_src + *p_length_read, "clpcPuschfreqSelectiveEnable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable);
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
rrc_il_parse_rrc_mac_explicit_cell_start_required
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_explicit_cell_start_required, 0, sizeof(rrc_mac_explicit_cell_start_required_t));

    /* This function parses rrc_mac_explicit_cell_start_required */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_explicit_cell_start_required->explicit_start_required, p_src + *p_length_read, "explicit_start_required");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_explicit_cell_start_required->explicit_start_required > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_explicit_cell_start_required->explicit_start_required] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_explicit_cell_start_required->explicit_start_required);
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
rrc_il_parse_rrc_mac_ul_sync_loss_timer_info
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ul_sync_loss_timer_info, 0, sizeof(rrc_mac_ul_sync_loss_timer_info_t));

    /* This function parses rrc_mac_ul_sync_loss_timer_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ul_sync_loss_timer_info->ul_sync_loss_timer_value, p_src + *p_length_read, "ul_sync_loss_timer_value");
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
rrc_il_parse_rrc_mac_pucch_cqi_sinr_threshold_info
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pucch_cqi_sinr_threshold_info, 0, sizeof(rrc_mac_pucch_cqi_sinr_threshold_info_t));

    /* This function parses rrc_mac_pucch_cqi_sinr_threshold_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pucch_cqi_sinr_threshold_info->sinr_value, p_src + *p_length_read, "sinr_value");
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
rrc_il_parse_rrc_mac_n_gap
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_n_gap, 0, sizeof(rrc_mac_n_gap_t));

    /* This function parses rrc_mac_n_gap */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_n_gap->n_gap, p_src + *p_length_read, "n_gap");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n_gap->n_gap > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n_gap->n_gap] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_n_gap->n_gap);
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
rrc_il_parse_rrc_mac_sps_crnti_range
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_crnti_range, 0, sizeof(rrc_mac_sps_crnti_range_t));

    /* This function parses rrc_mac_sps_crnti_range */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sps_crnti_range->start_sps_crnti, p_src + *p_length_read, "start_sps_crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_range->start_sps_crnti < 61) || (p_rrc_mac_sps_crnti_range->start_sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_range->start_sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_range->start_sps_crnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sps_crnti_range->end_sps_crnti, p_src + *p_length_read, "end_sps_crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_range->end_sps_crnti < 61) || (p_rrc_mac_sps_crnti_range->end_sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_range->end_sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_range->end_sps_crnti);
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
rrc_il_parse_rrc_mac_dynamic_pdcch_info
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dynamic_pdcch_info, 0, sizeof(rrc_mac_dynamic_pdcch_info_t));

    /* This function parses rrc_mac_dynamic_pdcch_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled, p_src + *p_length_read, "isDynamicPdcchDisabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled);
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
rrc_il_parse_rrc_mac_prs_bandwidth_info
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_bandwidth_info, 0, sizeof(rrc_mac_prs_bandwidth_info_t));

    /* This function parses rrc_mac_prs_bandwidth_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_prs_bandwidth_info->prs_bandwidth, p_src + *p_length_read, "prs_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_bandwidth_info->prs_bandwidth < 6) || (p_rrc_mac_prs_bandwidth_info->prs_bandwidth > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_bandwidth_info->prs_bandwidth] should be in range "
            "6 to 100. Incorrect value %u received.", p_rrc_mac_prs_bandwidth_info->prs_bandwidth);
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
rrc_il_parse_rrc_mac_prs_subframes_info
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_subframes_info, 0, sizeof(rrc_mac_prs_subframes_info_t));

    /* This function parses rrc_mac_prs_subframes_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_prs_subframes_info->prs_subframes, p_src + *p_length_read, "prs_subframes");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_subframes_info->prs_subframes < 1) || (p_rrc_mac_prs_subframes_info->prs_subframes > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_subframes_info->prs_subframes] should be in range "
            "1 to 6. Incorrect value %u received.", p_rrc_mac_prs_subframes_info->prs_subframes);
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
rrc_il_parse_rrc_mac_prs_config_index_info
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_config_index_info, 0, sizeof(rrc_mac_prs_config_index_info_t));

    /* This function parses rrc_mac_prs_config_index_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_prs_config_index_info->prs_config_index, p_src + *p_length_read, "prs_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_config_index_info->prs_config_index > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_config_index_info->prs_config_index] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_mac_prs_config_index_info->prs_config_index);
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
rrc_il_parse_rrc_mac_prs_transmission_power_info
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_transmission_power_info, 0, sizeof(rrc_mac_prs_transmission_power_info_t));

    /* This function parses rrc_mac_prs_transmission_power_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_prs_transmission_power_info->prs_transmission_power, p_src + *p_length_read, "prs_transmission_power");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_transmission_power_info->prs_transmission_power > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_transmission_power_info->prs_transmission_power] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_prs_transmission_power_info->prs_transmission_power);
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
rrc_il_parse_rrc_mac_prs_muting_config_info
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_muting_config_info, 0, sizeof(rrc_mac_prs_muting_config_info_t));

    /* This function parses rrc_mac_prs_muting_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_prs_muting_config_info->bits_to_be_read, p_src + *p_length_read, "bits_to_be_read");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_muting_config_info->bits_to_be_read < 2) || (p_rrc_mac_prs_muting_config_info->bits_to_be_read > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_muting_config_info->bits_to_be_read] should be in range "
            "2 to 16. Incorrect value %u received.", p_rrc_mac_prs_muting_config_info->bits_to_be_read);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_prs_muting_config_info->prs_muting_config_index, p_src + *p_length_read, "prs_muting_config_index");
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
rrc_il_parse_rrc_mac_prs_config_info
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_prs_config_info, 0, sizeof(rrc_mac_prs_config_info_t));

    /* This function parses rrc_mac_prs_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_prs_config_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_prs_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PRS_BANDWIDTH_INFO:
                /* rrc_mac_prs_bandwidth_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_bandwidth_info(
                    &p_rrc_mac_prs_config_info->prs_bandwidth_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_prs_config_info->optional_elems_present |= RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_SUBFRAMES_INFO:
                /* rrc_mac_prs_subframes_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_subframes_info(
                    &p_rrc_mac_prs_config_info->prs_subframes_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_prs_config_info->optional_elems_present |= RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_CONFIG_INDEX_INFO:
                /* rrc_mac_prs_config_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_config_index_info(
                    &p_rrc_mac_prs_config_info->prs_config_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_prs_config_info->optional_elems_present |= RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_TRANSMISSION_POWER_INFO:
                /* rrc_mac_prs_transmission_power_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_transmission_power_info(
                    &p_rrc_mac_prs_config_info->prs_transmission_power_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_prs_config_info->optional_elems_present |= RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_MUTING_CONFIG_INFO:
                /* rrc_mac_prs_muting_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_muting_config_info(
                    &p_rrc_mac_prs_config_info->prs_muting_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_prs_config_info->optional_elems_present |= RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_m2ap_mac_csa_pattern_list
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_m2ap_mac_csa_pattern_list, 0, sizeof(m2ap_mac_csa_pattern_list_t));

    /* This function parses m2ap_mac_csa_pattern_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period, p_src + *p_length_read, "radio_frame_allocation_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset, p_src + *p_length_read, "radio_frame_allocation_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_csa_pattern_list->choice, p_src + *p_length_read, "choice");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->choice > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->choice] should be less than"
            " or equal to 1. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->choice);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_m2ap_mac_csa_pattern_list->subframe_allocation, p_src + *p_length_read, "subframe_allocation");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_m2ap_embms_sib2_info
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_m2ap_embms_sib2_info, 0, sizeof(rrc_m2ap_embms_sib2_info_t));

    /* This function parses rrc_m2ap_embms_sib2_info */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_M2AP_MAC_CSA_PATTERN_LIST:
                /* m2ap_mac_csa_pattern_list TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter >= (S32)ARRSIZE(p_rrc_m2ap_embms_sib2_info->csa_pattern_list))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_m2ap_mac_csa_pattern_list(
                    &p_rrc_m2ap_embms_sib2_info->csa_pattern_list[p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_m2ap_mac_embms_sib13_info
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_m2ap_mac_embms_sib13_info, 0, sizeof(rrc_m2ap_mac_embms_sib13_info_t));

    /* This function parses rrc_m2ap_mac_embms_sib13_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_m2ap_mac_embms_sib13_info->si_index, p_src + *p_length_read, "si_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_m2ap_mac_embms_sib13_info->si_index < 1) || (p_rrc_m2ap_mac_embms_sib13_info->si_index > 74))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_m2ap_mac_embms_sib13_info->si_index] should be in range "
            "1 to 74. Incorrect value %u received.", p_rrc_m2ap_mac_embms_sib13_info->si_index);
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
rrc_il_parse_m2ap_embms_notification_config
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_m2ap_embms_notification_config, 0, sizeof(m2ap_embms_notification_config_t));

    /* This function parses m2ap_embms_notification_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_embms_notification_config->notification_rept_coeff, p_src + *p_length_read, "notification_rept_coeff");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_embms_notification_config->notification_rept_coeff < 2) || (p_m2ap_embms_notification_config->notification_rept_coeff > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_rept_coeff] should be in range "
            "2 to 4. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_rept_coeff);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_embms_notification_config->notification_offset, p_src + *p_length_read, "notification_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_embms_notification_config->notification_offset > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_offset] should be less than"
            " or equal to 10. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_embms_notification_config->notification_sf_index, p_src + *p_length_read, "notification_sf_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_embms_notification_config->notification_sf_index > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_sf_index] should be less than"
            " or equal to 6. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_sf_index);
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
rrc_il_parse_m2ap_mac_mcch_config
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_m2ap_mac_mcch_config, 0, sizeof(m2ap_mac_mcch_config_t));

    /* This function parses m2ap_mac_mcch_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_mcch_config->mcch_rep_period, p_src + *p_length_read, "mcch_rep_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_rep_period > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_rep_period] should be less than"
            " or equal to 3. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_rep_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_mcch_config->mcch_offset, p_src + *p_length_read, "mcch_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_offset > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_offset] should be less than"
            " or equal to 10. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_mcch_config->mcch_mod_period, p_src + *p_length_read, "mcch_mod_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_mod_period > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_mod_period] should be less than"
            " or equal to 1. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_mod_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_mcch_config->sf_alloc_info, p_src + *p_length_read, "sf_alloc_info");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mac_mcch_config->signalling_mcs, p_src + *p_length_read, "signalling_mcs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->signalling_mcs > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->signalling_mcs] should be less than"
            " or equal to 3. Incorrect value %u received.", p_m2ap_mac_mcch_config->signalling_mcs);
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
rrc_il_parse_m2ap_mbms_create_area_info
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_m2ap_mbms_create_area_info, 0, sizeof(m2ap_mbms_create_area_info_t));

    /* This function parses m2ap_mbms_create_area_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->area_index, p_src + *p_length_read, "area_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->area_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->area_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_m2ap_mbms_create_area_info->area_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->area_id, p_src + *p_length_read, "area_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->area_type, p_src + *p_length_read, "area_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_mbms_create_area_info->area_type < 1) || (p_m2ap_mbms_create_area_info->area_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->area_type] should be in range "
            "1 to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->area_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->non_mbsfn_region_len, p_src + *p_length_read, "non_mbsfn_region_len");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_mbms_create_area_info->non_mbsfn_region_len < 1) || (p_m2ap_mbms_create_area_info->non_mbsfn_region_len > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->non_mbsfn_region_len] should be in range "
            "1 to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->non_mbsfn_region_len);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->request_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->request_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->request_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_m2ap_mbms_create_area_info->notification_ind, p_src + *p_length_read, "notification_ind");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->notification_ind > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->notification_ind] should be less than"
            " or equal to 7. Incorrect value %u received.", p_m2ap_mbms_create_area_info->notification_ind);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_M2AP_MAC_MCCH_CONFIG:
                /* m2ap_mac_mcch_config TLV */
                if (RRC_FAILURE == rrc_il_parse_m2ap_mac_mcch_config(
                    &p_m2ap_mbms_create_area_info->mcch_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_m2ap_mbms_create_area_info->optional_elems_present |= RRC_M2AP_MAC_MCCH_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_m2ap_mac_create_embms_area_entity_info
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_m2ap_mac_create_embms_area_entity_info, 0, sizeof(rrc_m2ap_mac_create_embms_area_entity_info_t));

    /* This function parses rrc_m2ap_mac_create_embms_area_entity_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area, p_src + *p_length_read, "num_of_area");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_M2AP_MAC_NOTIFICATION_CONFIG:
                /* m2ap_embms_notification_config TLV */
                if (RRC_FAILURE == rrc_il_parse_m2ap_embms_notification_config(
                    &p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_embms_notification_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_m2ap_mac_create_embms_area_entity_info->optional_elems_present |= RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MBMS_CREATE_AREA_INFO:
                /* m2ap_mbms_create_area_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter >= (S32)ARRSIZE(p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_m2ap_mbms_create_area_info(
                    &p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info[p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_m2ap_mac_delete_embms_area_entity_info
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_m2ap_mac_delete_embms_area_entity_info, 0, sizeof(rrc_m2ap_mac_delete_embms_area_entity_info_t));

    /* This function parses rrc_m2ap_mac_delete_embms_area_entity_info */

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count %u is exceeding p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index[loop], p_src + *p_length_read + loop, "area_index[]");
        }

        p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_m2ap_mac_embms_transmission_power_info
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_m2ap_mac_embms_transmission_power_info, 0, sizeof(m2ap_mac_embms_transmission_power_info_t));

    /* This function parses m2ap_mac_embms_transmission_power_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_m2ap_mac_embms_transmission_power_info->embms_transmission_power, p_src + *p_length_read, "embms_transmission_power");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_embms_transmission_power_info->embms_transmission_power > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_embms_transmission_power_info->embms_transmission_power] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_m2ap_mac_embms_transmission_power_info->embms_transmission_power);
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
rrc_il_parse_rrc_m2ap_mac_embms_params_info
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_m2ap_mac_embms_params_info, 0, sizeof(rrc_m2ap_mac_embms_params_info_t));

    /* This function parses rrc_m2ap_mac_embms_params_info */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_M2AP_MAC_EMBMS_TRANSMISSION_POWER_INFO:
                /* m2ap_mac_embms_transmission_power_info TLV */
                if (RRC_FAILURE == rrc_il_parse_m2ap_mac_embms_transmission_power_info(
                    &p_rrc_m2ap_mac_embms_params_info->m2ap_mac_embms_transmission_power_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_m2ap_mac_embms_params_info->optional_elems_present |= RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_radio_res_dedicated_laa_scell_config
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_radio_res_dedicated_laa_scell_config, 0, sizeof(rrc_radio_res_dedicated_laa_scell_config_t));

    /* This function parses rrc_radio_res_dedicated_laa_scell_config */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position, p_src + *p_length_read, "subframe_start_position");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config, p_src + *p_length_read, "laa_scell_subframe_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config > 255))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config] should be less than"
            " or equal to 255. Incorrect value %u received.", p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config);
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
rrc_il_parse_rrc_dmtc_configuration_r13
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_dmtc_configuration_r13, 0, sizeof(rrc_dmtc_configuration_r13_t));

    /* This function parses rrc_dmtc_configuration_r13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_dmtc_configuration_r13->dmtc_periodicity, p_src + *p_length_read, "dmtc_periodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->dmtc_periodicity > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->dmtc_periodicity] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->dmtc_periodicity);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_dmtc_configuration_r13->dmtc_offset, p_src + *p_length_read, "dmtc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->dmtc_offset > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->dmtc_offset] should be less than"
            " or equal to 159. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->dmtc_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_dmtc_configuration_r13->ds_occassion_duration, p_src + *p_length_read, "ds_occassion_duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->ds_occassion_duration > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->ds_occassion_duration] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->ds_occassion_duration);
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
rrc_il_parse_RrcEmtcDataConfig
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcEmtcDataConfig, 0, sizeof(RrcEmtcDataConfig_t));

    /* This function parses RrcEmtcDataConfig */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEmtcDataConfig->sib1BrIndex, p_src + *p_length_read, "sib1BrIndex");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEmtcDataConfig->sib1BrIndex > EMTC_MAX_SIB1_BR_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEmtcDataConfig->sib1BrIndex] should be less than"
            " or equal to EMTC_MAX_SIB1_BR_INDEX. Incorrect value %u received.", p_RrcEmtcDataConfig->sib1BrIndex);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEmtcDataConfig->numUeDedicatedNarrowbands, p_src + *p_length_read, "numUeDedicatedNarrowbands");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcEmtcDataConfig->numUeDedicatedNarrowbands < 1) || (p_RrcEmtcDataConfig->numUeDedicatedNarrowbands > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEmtcDataConfig->numUeDedicatedNarrowbands] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcEmtcDataConfig->numUeDedicatedNarrowbands);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_RrcEmtcDataConfig->numUeDedicatedNarrowbands * sizeof(p_RrcEmtcDataConfig->ueDedicatedNarrowbands[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with BOTH boundaries
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcEmtcDataConfig->numUeDedicatedNarrowbands; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop], (void*)(p_src + *p_length_read), "ueDedicatedNarrowbands[]");
            if ((p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop] < 1) || (p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop] > EMTC_MAX_AVAILABLE_NARROWBAND))
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcEmtcDataConfig->ueDedicatedNarrowbands[%u] should be in "
                    "range 1 to EMTC_MAX_AVAILABLE_NARROWBAND. "
                    "Incorrect value %d received.", loop, p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop]);
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
rrc_il_parse_RrcPrachStartSfR13
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcPrachStartSfR13, 0, sizeof(RrcPrachStartSfR13_t));

    /* This function parses RrcPrachStartSfR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachStartSfR13->prachStartingSubframeR13, p_src + *p_length_read, "prachStartingSubframeR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachStartSfR13->prachStartingSubframeR13 > EMTC_PRACH_STARTING_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachStartSfR13->prachStartingSubframeR13] should be less than"
            " or equal to EMTC_PRACH_STARTING_SF_MAX. Incorrect value %u received.", p_RrcPrachStartSfR13->prachStartingSubframeR13);
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
rrc_il_parse_RrcMaxNumPreambleAttemptCeR13
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMaxNumPreambleAttemptCeR13, 0, sizeof(RrcMaxNumPreambleAttemptCeR13_t));

    /* This function parses RrcMaxNumPreambleAttemptCeR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13, p_src + *p_length_read, "maxNumPreambleAttemptCeR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13 > EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13] should be less than"
            " or equal to EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX. Incorrect value %u received.", p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13);
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
rrc_il_parse_RrcPrachParametersCeR13
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcPrachParametersCeR13, 0, sizeof(RrcPrachParametersCeR13_t));

    /* This function parses RrcPrachParametersCeR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->ceLevel, p_src + *p_length_read, "ceLevel");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcPrachParametersCeR13->ceLevel);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->prachConfigIndexR13, p_src + *p_length_read, "prachConfigIndexR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachConfigIndexR13 > EMTC_MAX_PRACH_CONFIG_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachConfigIndexR13] should be less than"
            " or equal to EMTC_MAX_PRACH_CONFIG_INDEX. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachConfigIndexR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->prachFreqOffsetR13, p_src + *p_length_read, "prachFreqOffsetR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachFreqOffsetR13 > EMTC_MAX_PRACH_FREQ_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachFreqOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_FREQ_OFFSET. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachFreqOffsetR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13, p_src + *p_length_read, "numRepetitionPerPreambleAttemptR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13 > EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13] should be less than"
            " or equal to EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13, p_src + *p_length_read, "numMpdcchNarrowbandsToMonitorR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 < 1) || (p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 > EMTC_MAX_NARROWBAND_TO_MONITOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13] should be in range "
            "1 to EMTC_MAX_NARROWBAND_TO_MONITOR. Incorrect value %u received.", p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 * sizeof(p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop], (void*)(p_src + *p_length_read), "mpdcchNarrowBandIndex[]");
            if (p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop] > EMTC_MAX_AVAILABLE_NARROWBAND)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[%u] should be less"
                    " than or equal to EMTC_MAX_AVAILABLE_NARROWBAND. "
                    "Incorrect value %d received.", loop, p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13, p_src + *p_length_read, "mpdcchNumRepetitionsRaR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13 > EMTC_MPDCCH_NUM_REPETITION_RA_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_RA_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachParametersCeR13->prachHoppingConfigR13, p_src + *p_length_read, "prachHoppingConfigR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachHoppingConfigR13 > EMTC_PRACH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachHoppingConfigR13] should be less than"
            " or equal to EMTC_PRACH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PRACH_STARTING_SUBFRAME_R13:
                /* RrcPrachStartSfR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcPrachStartSfR13(
                    &p_RrcPrachParametersCeR13->prachStartingSubframeR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcPrachParametersCeR13->bitmask |= RRC_MAC_PRACH_START_SF_R13_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13:
                /* RrcMaxNumPreambleAttemptCeR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMaxNumPreambleAttemptCeR13(
                    &p_RrcPrachParametersCeR13->maxNumPreambleAttemptCeR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcPrachParametersCeR13->bitmask |= RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcPrachHoppingOffsetR13
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcPrachHoppingOffsetR13, 0, sizeof(RrcPrachHoppingOffsetR13_t));

    /* This function parses RrcPrachHoppingOffsetR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13, p_src + *p_length_read, "prachHoppingOffsetR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13 > EMTC_MAX_PRACH_HOPPING_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_HOPPING_OFFSET. Incorrect value %u received.", p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13);
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
rrc_il_parse_RrcMacPrachConfigV1310
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacPrachConfigV1310, 0, sizeof(RrcMacPrachConfigV1310_t));

    /* This function parses RrcMacPrachConfigV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType, p_src + *p_length_read, "mpdcchStartSfCssRaType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa, p_src + *p_length_read, "mpdcchStartSfCssRa");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa > EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX. Incorrect value %u received.", p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPrachConfigV1310->numCeLevels, p_src + *p_length_read, "numCeLevels");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPrachConfigV1310->numCeLevels < 1) || (p_RrcMacPrachConfigV1310->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacPrachConfigV1310->numCeLevels);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PRACH_PARAMETERS_CE_R13_LIST:
                /* RrcPrachParametersCeR13 TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_RrcMacPrachConfigV1310->numCeLevelsCounter >= (S32)ARRSIZE(p_RrcMacPrachConfigV1310->prachParametersCeR13))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_RrcPrachParametersCeR13(
                    &p_RrcMacPrachConfigV1310->prachParametersCeR13[p_RrcMacPrachConfigV1310->numCeLevelsCounter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacPrachConfigV1310->numCeLevelsCounter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_PRACH_HOPPING_OFFSET_R13:
                /* RrcPrachHoppingOffsetR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcPrachHoppingOffsetR13(
                    &p_RrcMacPrachConfigV1310->prachHoppingOffsetR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacPrachConfigV1310->bitmask |= RRC_PRACH_HOPPING_OFFSET_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcMacSibtype1BrMsgInfo
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacSibtype1BrMsgInfo, 0, sizeof(RrcMacSibtype1BrMsgInfo_t));

    /* This function parses RrcMacSibtype1BrMsgInfo */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacSibtype1BrMsgInfo->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSibtype1BrMsgInfo->sfn > EMTC_MAX_STARTING_SFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSibtype1BrMsgInfo->sfn] should be less than"
            " or equal to EMTC_MAX_STARTING_SFN. Incorrect value %u received.", p_RrcMacSibtype1BrMsgInfo->sfn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13, p_src + *p_length_read, "startSymbolBrR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13 < 1) || (p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13 > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter %u is exceeding p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf[loop], p_src + *p_length_read + loop, "sibtype1BrMsgBuf[]");
        }

        p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_RrcMacSiMsgsBr
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacSiMsgsBr, 0, sizeof(RrcMacSiMsgsBr_t));

    /* This function parses RrcMacSiMsgsBr */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siIndexBr, p_src + *p_length_read, "siIndexBr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgsBr->siIndexBr < 1) || (p_RrcMacSiMsgsBr->siIndexBr > EMTC_MAX_SI_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siIndexBr] should be in range "
            "1 to EMTC_MAX_SI_COUNT. Incorrect value %u received.", p_RrcMacSiMsgsBr->siIndexBr);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siPeriodicity, p_src + *p_length_read, "siPeriodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siPeriodicity > EMTC_SI_PERIODICITY_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siPeriodicity] should be less than"
            " or equal to EMTC_SI_PERIODICITY_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siPeriodicity);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacSiMsgsBr->startingSfn, p_src + *p_length_read, "startingSfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->startingSfn > EMTC_MAX_STARTING_SFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->startingSfn] should be less than"
            " or equal to EMTC_MAX_STARTING_SFN. Incorrect value %u received.", p_RrcMacSiMsgsBr->startingSfn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siRepetitionPatternR13, p_src + *p_length_read, "siRepetitionPatternR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siRepetitionPatternR13 > EMTC_SI_REPETITION_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siRepetitionPatternR13] should be less than"
            " or equal to EMTC_SI_REPETITION_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siRepetitionPatternR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siNarrowbandR13, p_src + *p_length_read, "siNarrowbandR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgsBr->siNarrowbandR13 < 1) || (p_RrcMacSiMsgsBr->siNarrowbandR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siNarrowbandR13] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacSiMsgsBr->siNarrowbandR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siTbsR13, p_src + *p_length_read, "siTbsR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siTbsR13 > EMTC_SI_TB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siTbsR13] should be less than"
            " or equal to EMTC_SI_TB_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siTbsR13);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_RrcMacSiMsgsBr->siMsgBufBr) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_RrcMacSiMsgsBr->siMsgBufLengthBr %u is exceeding p_RrcMacSiMsgsBr->siMsgBufBr array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_RrcMacSiMsgsBr->siMsgBufBr[loop], p_src + *p_length_read + loop, "siMsgBufBr[]");
        }

        p_RrcMacSiMsgsBr->siMsgBufLengthBr = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_RrcMacFddDownlinkOrTddSfBitmapBr
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacFddDownlinkOrTddSfBitmapBr, 0, sizeof(RrcMacFddDownlinkOrTddSfBitmapBr_t));

    /* This function parses RrcMacFddDownlinkOrTddSfBitmapBr */

    if (*p_length_read + (S32)sizeof(p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr[loop], (void*)(p_src + *p_length_read), "fddDownlinksubframeBitmapBr[]");
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
rrc_il_parse_RrcMacFddUplinkSfBitmapBr
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacFddUplinkSfBitmapBr, 0, sizeof(RrcMacFddUplinkSfBitmapBr_t));

    /* This function parses RrcMacFddUplinkSfBitmapBr */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr, p_src + *p_length_read, "fddUplinkSubframeBitmapBr");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr > EMTC_MAX_FDD_UPLINK_SF_BITMAP))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr] should be less than"
            " or equal to EMTC_MAX_FDD_UPLINK_SF_BITMAP. Incorrect value %u received.", p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr);
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
rrc_il_parse_RrcMacSiMsgBrInfo
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacSiMsgBrInfo, 0, sizeof(RrcMacSiMsgBrInfo_t));

    /* This function parses RrcMacSiMsgBrInfo */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgBrInfo->siWindowLengthBr, p_src + *p_length_read, "siWindowLengthBr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgBrInfo->siWindowLengthBr > EMTC_MAX_SI_WINDOW_MS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->siWindowLengthBr] should be less than"
            " or equal to EMTC_MAX_SI_WINDOW_MS. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->siWindowLengthBr);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13, p_src + *p_length_read, "siHoppingConfigCommonR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13 > EMTC_SI_HOPPING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13] should be less than"
            " or equal to EMTC_SI_HOPPING_MAX. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacSiMsgBrInfo->numSiMessageBr, p_src + *p_length_read, "numSiMessageBr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgBrInfo->numSiMessageBr < 1) || (p_RrcMacSiMsgBrInfo->numSiMessageBr > EMTC_MAX_SI_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->numSiMessageBr] should be in range "
            "1 to EMTC_MAX_SI_COUNT. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->numSiMessageBr);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SI_BR_MSG_REQ:
                /* RrcMacSiMsgsBr TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter >= (S32)ARRSIZE(p_RrcMacSiMsgBrInfo->siMsgsBr))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_RrcMacSiMsgsBr(
                    &p_RrcMacSiMsgBrInfo->siMsgsBr[p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR:
                /* RrcMacFddDownlinkOrTddSfBitmapBr TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacFddDownlinkOrTddSfBitmapBr(
                    &p_RrcMacSiMsgBrInfo->fddDownlinkOrTddSfBitmapBr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacSiMsgBrInfo->bitmask |= RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_FDD_UPNLINK_SF_BITMAP_BR:
                /* RrcMacFddUplinkSfBitmapBr TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacFddUplinkSfBitmapBr(
                    &p_RrcMacSiMsgBrInfo->fddUplinkSfBitmapBr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacSiMsgBrInfo->bitmask |= RRC_MAC_FDD_UPLINK_SF_BITMAP_BR_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcN1PucchAnInfoListR13
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcN1PucchAnInfoListR13, 0, sizeof(RrcN1PucchAnInfoListR13_t));

    /* This function parses RrcN1PucchAnInfoListR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcN1PucchAnInfoListR13->numCeLevels, p_src + *p_length_read, "numCeLevels");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcN1PucchAnInfoListR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcN1PucchAnInfoListR13->numCeLevels] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcN1PucchAnInfoListR13->numCeLevels);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_RrcN1PucchAnInfoListR13->numCeLevels * sizeof(p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcN1PucchAnInfoListR13->numCeLevels; loop++)
        {
            rrc_cp_unpack_U16((void*)&p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop], (void*)(p_src + *p_length_read), "n1PucchAnInfo[]");
            if (p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop] > EMTC_MAX_N1_PUCCH_AN)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[%u] should be less"
                    " than or equal to EMTC_MAX_N1_PUCCH_AN. "
                    "Incorrect value %d received.", loop, p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop]);
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
rrc_il_parse_RrcMacPucchConfigCommonV1310
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacPucchConfigCommonV1310, 0, sizeof(RrcMacPucchConfigCommonV1310_t));

    /* This function parses RrcMacPucchConfigCommonV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPucchConfigCommonV1310->numCeLevels, p_src + *p_length_read, "numCeLevels");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchConfigCommonV1310->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchConfigCommonV1310->numCeLevels] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacPucchConfigCommonV1310->numCeLevels);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_RrcMacPucchConfigCommonV1310->numCeLevels * sizeof(p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcMacPucchConfigCommonV1310->numCeLevels; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop], (void*)(p_src + *p_length_read), "pucchNumRepetitionCeMsg4PerLevel[]");
            if (p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop] > EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[%u] should be less"
                    " than or equal to EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX. "
                    "Incorrect value %d received.", loop, p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_N1_PUCCH_AN_INFO_LIST:
                /* RrcN1PucchAnInfoListR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcN1PucchAnInfoListR13(
                    &p_RrcMacPucchConfigCommonV1310->n1PucchAnInfoList,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacPucchConfigCommonV1310->bitmask |= RRC_MAC_N1_PUCCH_AN_INFO_LIST_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcMacModificationPeriodV1310
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacModificationPeriodV1310, 0, sizeof(RrcMacModificationPeriodV1310_t));

    /* This function parses RrcMacModificationPeriodV1310 */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_RrcMacModificationPeriodV1310->modificationPeriodV1310, p_src + *p_length_read, "modificationPeriodV1310");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacModificationPeriodV1310->modificationPeriodV1310 < 64) || (p_RrcMacModificationPeriodV1310->modificationPeriodV1310 > EMTC_MAX_MOD_PERIOD))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacModificationPeriodV1310->modificationPeriodV1310] should be in range "
            "64 to EMTC_MAX_MOD_PERIOD. Incorrect value %u received.", p_RrcMacModificationPeriodV1310->modificationPeriodV1310);
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
rrc_il_parse_RrcMacPcchConfigV1310
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacPcchConfigV1310, 0, sizeof(RrcMacPcchConfigV1310_t));

    /* This function parses RrcMacPcchConfigV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPcchConfigV1310->pagingNarrowBandsR13, p_src + *p_length_read, "pagingNarrowBandsR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPcchConfigV1310->pagingNarrowBandsR13 < 1) || (p_RrcMacPcchConfigV1310->pagingNarrowBandsR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->pagingNarrowBandsR13] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacPcchConfigV1310->pagingNarrowBandsR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13, p_src + *p_length_read, "mpdcchMaxNumRepetitionPagingR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13 > EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13] should be less than"
            " or equal to EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPcchConfigV1310->nBV1310, p_src + *p_length_read, "nBV1310");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPcchConfigV1310->nBV1310 > EMTC_NB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->nBV1310] should be less than"
            " or equal to EMTC_NB_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->nBV1310);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacPcchConfigV1310->sfnGapV1310, p_src + *p_length_read, "sfnGapV1310");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPcchConfigV1310->sfnGapV1310 < 1) || (p_RrcMacPcchConfigV1310->sfnGapV1310 > EMTC_SFN_GAP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->sfnGapV1310] should be in range "
            "1 to EMTC_SFN_GAP_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->sfnGapV1310);
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
rrc_il_parse_RrcMacPdschConfigCommonV1310
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacPdschConfigCommonV1310, 0, sizeof(RrcMacPdschConfigCommonV1310_t));

    /* This function parses RrcMacPdschConfigCommonV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA, p_src + *p_length_read, "pdschMaxNumRepetitionCeModeA");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA > EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA] should be less than"
            " or equal to EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX. Incorrect value %u received.", p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA);
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
rrc_il_parse_RrcMacPuschConfigCommonV1310
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacPuschConfigCommonV1310, 0, sizeof(RrcMacPuschConfigCommonV1310_t));

    /* This function parses RrcMacPuschConfigCommonV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA, p_src + *p_length_read, "puschMaxNumRepetitionCeModeA");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA > EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA] should be less than"
            " or equal to EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX. Incorrect value %u received.", p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA);
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
rrc_il_parse_RrcFrequancyHoppingParametersR13
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcFrequancyHoppingParametersR13, 0, sizeof(RrcFrequancyHoppingParametersR13_t));

    /* This function parses RrcFrequancyHoppingParametersR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcFrequancyHoppingParametersR13->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcFrequancyHoppingParametersR13->type > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcFrequancyHoppingParametersR13->type] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcFrequancyHoppingParametersR13->type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13, p_src + *p_length_read, "intervalUlHoppingConfigCommonModeAFddR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13 > EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13] should be less than"
            " or equal to EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX. Incorrect value %u received.", p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_config_cell_cnf
(
    rrc_mac_config_cell_cnf_t *p_rrc_mac_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_config_cell_cnf, 0, sizeof(rrc_mac_config_cell_cnf_t));

    /* This function parses rrc_mac_config_cell_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_config_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CONFIG_CELL_ERROR_CODE:
                /* rrc_mac_config_cell_error_code TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_config_cell_error_code(
                    &p_rrc_mac_config_cell_cnf->config_cell_error_code,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_cnf->optional_elems_present |= RRC_MAC_CONFIG_CELL_ERROR_CODE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_LAA_CELL_CONFIG_EARFCN:
                /* rrc_dl_earfcn TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_dl_earfcn(
                    &p_rrc_mac_config_cell_cnf->dl_earfcn_list,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_config_cell_cnf->optional_elems_present |= RRC_MAC_CONFIG_CELL_EARFCN_LIST_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_config_cell_error_code
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_config_cell_error_code, 0, sizeof(rrc_mac_config_cell_error_code_t));

    /* This function parses rrc_mac_config_cell_error_code */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_config_cell_error_code->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_error_code->error_code > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_error_code->error_code] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_config_cell_error_code->error_code);
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
rrc_il_parse_rrc_dl_earfcn
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_dl_earfcn, 0, sizeof(rrc_dl_earfcn_t));

    /* This function parses rrc_dl_earfcn */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_dl_earfcn->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dl_earfcn->count > RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dl_earfcn->count] should be less than"
            " or equal to RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT. Incorrect value %u received.", p_rrc_dl_earfcn->count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrc_dl_earfcn->count * sizeof(p_rrc_dl_earfcn->earfcnDl[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_dl_earfcn->count; loop++)
        {
            rrc_cp_unpack_U32((void*)&p_rrc_dl_earfcn->earfcnDl[loop], (void*)(p_src + *p_length_read), "earfcnDl[]");
            *p_length_read += sizeof(U32);
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
rrc_il_parse_rrc_mac_sfn_req
(
    rrc_mac_sfn_req_t *p_rrc_mac_sfn_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sfn_req, 0, sizeof(rrc_mac_sfn_req_t));

    /* This function parses rrc_mac_sfn_req */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_TIMESTAMP_REQD_INFO:
                /* rrc_mac_timestamp_reqd TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_timestamp_reqd(
                    &p_rrc_mac_sfn_req->timestamp_reqd,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sfn_req->optional_elems_present |= RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_timestamp_reqd
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_timestamp_reqd, 0, sizeof(rrc_mac_timestamp_reqd_t));

    /* This function parses rrc_mac_timestamp_reqd */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_timestamp_reqd->timestamp_reqd, p_src + *p_length_read, "timestamp_reqd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_timestamp_reqd->timestamp_reqd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_timestamp_reqd->timestamp_reqd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_timestamp_reqd->timestamp_reqd);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_sfn_cnf
(
    rrc_mac_sfn_cnf_t *p_rrc_mac_sfn_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sfn_cnf, 0, sizeof(rrc_mac_sfn_cnf_t));

    /* This function parses rrc_mac_sfn_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sfn_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sfn_cnf->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SFN_INFO:
                /* rrc_mac_sfn TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sfn(
                    &p_rrc_mac_sfn_cnf->sfn,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sfn_cnf->optional_elems_present |= RRC_MAC_SFN_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SF_INFO:
                /* rrc_mac_sf TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sf(
                    &p_rrc_mac_sfn_cnf->sf,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sfn_cnf->optional_elems_present |= RRC_MAC_SF_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TIMESTAMP_INFO:
                /* rrc_timestamp TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_timestamp(
                    &p_rrc_mac_sfn_cnf->timestamp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sfn_cnf->optional_elems_present |= RRC_MAC_TIMESTAMP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sfn
(
    rrc_mac_sfn_t *p_rrc_mac_sfn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sfn, 0, sizeof(rrc_mac_sfn_t));

    /* This function parses rrc_mac_sfn */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sfn->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sfn->sfn);
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
rrc_il_parse_rrc_mac_sf
(
    rrc_mac_sf_t *p_rrc_mac_sf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sf, 0, sizeof(rrc_mac_sf_t));

    /* This function parses rrc_mac_sf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sf->sf, p_src + *p_length_read, "sf");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sf->sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sf->sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sf->sf);
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
rrc_il_parse_rrc_timestamp
(
    rrc_timestamp_t *p_rrc_timestamp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_timestamp, 0, sizeof(rrc_timestamp_t));

    /* This function parses rrc_timestamp */

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U64(&p_rrc_timestamp->ts_sec, p_src + *p_length_read, "ts_sec");
    *p_length_read += sizeof(U64);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_timestamp->ts_sec > 2147483647))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_timestamp->ts_sec] should be less than"
            " or equal to 2147483647. Incorrect value %llu received.", p_rrc_timestamp->ts_sec);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_timestamp->ts_nsec, p_src + *p_length_read, "ts_nsec");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_timestamp->ts_nsec > 2147483647))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_timestamp->ts_nsec] should be less than"
            " or equal to 2147483647. Incorrect value %u received.", p_rrc_timestamp->ts_nsec);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_sfn_ind
(
    rrc_mac_sfn_ind_t *p_rrc_mac_sfn_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sfn_ind, 0, sizeof(rrc_mac_sfn_ind_t));

    /* This function parses rrc_mac_sfn_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sfn_ind->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn_ind->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_ind->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sfn_ind->sfn);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_reconfig_cell_req
(
    rrc_mac_reconfig_cell_req_t *p_rrc_mac_reconfig_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfig_cell_req, 0, sizeof(rrc_mac_reconfig_cell_req_t));

    /* This function parses rrc_mac_reconfig_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfig_cell_req->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_reconfig_cell_req->sfn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_reconfig_cell_req->sf, p_src + *p_length_read, "sf");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_req->sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_req->sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_reconfig_cell_req->sf);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_DCI_FORMAT_FOR_SI:
                /* rrc_mac_dci_format_for_si TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dci_format_for_si(
                    &p_rrc_mac_reconfig_cell_req->dci_format_for_si,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RBS_FOR_DCI_1A:
                /* rrc_mac_rbs_for_dci_1a TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rbs_for_dci_1a(
                    &p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1a,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RBS_FOR_DCI_1C:
                /* rrc_mac_rbs_for_dci_1c TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rbs_for_dci_1c(
                    &p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1c,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MIB_MSG_REQ:
                /* rrc_mac_mib_msg_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfig_cell_req->mib_msg_req_counter >= (S32)ARRSIZE(p_rrc_mac_reconfig_cell_req->mib_msg_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_mib_msg_req(
                    &p_rrc_mac_reconfig_cell_req->mib_msg_req[p_rrc_mac_reconfig_cell_req->mib_msg_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->mib_msg_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SIBTYPE1_MSG_REQ:
                /* rrc_mac_sibtype1_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sibtype1_msg_req(
                    &p_rrc_mac_reconfig_cell_req->sib1_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_INFO:
                /* rrc_mac_si_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_msg_req(
                    &p_rrc_mac_reconfig_cell_req->si_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MOD_PERIOD_INFO:
                /* rrc_mac_mod_period_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_mod_period_info(
                    &p_rrc_mac_reconfig_cell_req->mod_period_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SFN_GAP_INFO:
                /* rrc_mac_sfn_gap_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sfn_gap_info(
                    &p_rrc_mac_reconfig_cell_req->sfn_gap_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SFN_GAP_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CONTN_FREE_RACH_TIMER_INFO:
                /* rrc_mac_contn_free_rach_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_contn_free_rach_timer_info(
                    &p_rrc_mac_reconfig_cell_req->contn_free_rach_timer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_COMMON_INFO:
                /* rrc_mac_pdsch_config_common_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_config_common_info(
                    &p_rrc_mac_reconfig_cell_req->pdsch_config_common_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON:
                /* rrc_mac_downlink_power_control_common TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_downlink_power_control_common(
                    &p_rrc_mac_reconfig_cell_req->downlink_power_control_common,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UPLINK_POWER_CONTROL_COMMON:
                /* rrc_mac_uplink_power_control_common TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_uplink_power_control_common(
                    &p_rrc_mac_reconfig_cell_req->uplink_power_control_common_t,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UL_SYNC_LOSS_TIMER_INFO:
                /* rrc_mac_ul_sync_loss_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_sync_loss_timer_info(
                    &p_rrc_mac_reconfig_cell_req->ul_sync_loss_timer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO:
                /* rrc_mac_pucch_cqi_sinr_threshold_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_cqi_sinr_threshold_info(
                    &p_rrc_mac_reconfig_cell_req->pucch_cqi_sinr_threshold_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_COMMON:
                /* rrc_mac_pusch_config_common_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pusch_config_common_info(
                    &p_rrc_mac_reconfig_cell_req->pusch_config_common_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_N_GAP:
                /* rrc_mac_n_gap TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_n_gap(
                    &p_rrc_mac_reconfig_cell_req->n_gap,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_N_GAP_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SPS_CRNTI_RANGE:
                /* rrc_mac_sps_crnti_range TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_crnti_range(
                    &p_rrc_mac_reconfig_cell_req->sps_crnti_range,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PRS_CONFIG_INFO:
                /* rrc_mac_prs_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_prs_config_info(
                    &p_rrc_mac_reconfig_cell_req->prs_reconfig_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PRS_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_EMBMS_SIB2_INFO:
                /* rrc_m2ap_embms_sib2_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_embms_sib2_info(
                    &p_rrc_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_SIB13_INFO:
                /* rrc_m2ap_mac_embms_sib13_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_embms_sib13_info(
                    &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_sib13_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_SIB13_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO:
                /* rrc_m2ap_mac_create_embms_area_entity_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_create_embms_area_entity_info(
                    &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_M2AP_MBMS_AREA_CREATE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO:
                /* rrc_m2ap_mac_delete_embms_area_entity_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_delete_embms_area_entity_info(
                    &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_M2AP_MBMS_AREA_DELETE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_M2AP_MAC_EMBMS_CONFIG:
                /* rrc_m2ap_mac_embms_params_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_m2ap_mac_embms_params_info(
                    &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_embms_params_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SFN_EMTC:
                /* RrcMacEmtcSfn TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacEmtcSfn(
                    &p_rrc_mac_reconfig_cell_req->emtcSfn,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SFN_EMTC_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SIBTYPE1_BR_MSG_INFO:
                /* RrcMacSibtype1BrMsgInfo TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacSibtype1BrMsgInfo(
                    &p_rrc_mac_reconfig_cell_req->sibType1BrMsgInfo,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_BR_INFO:
                /* RrcMacSiMsgBrInfo TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacSiMsgBrInfo(
                    &p_rrc_mac_reconfig_cell_req->siMsgBrInfo,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SI_MSG_BR_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CONFIG_COMMON_V1310:
                /* RrcMacPucchConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPucchConfigCommonV1310(
                    &p_rrc_mac_reconfig_cell_req->pucchConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MODIFICATION_PERIOD_V1310:
                /* RrcMacModificationPeriodV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacModificationPeriodV1310(
                    &p_rrc_mac_reconfig_cell_req->modificationPeriodV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_MODIFICATION_PERIOD_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_COMMON_V1310:
                /* RrcMacPdschConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPdschConfigCommonV1310(
                    &p_rrc_mac_reconfig_cell_req->pdschConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_COMMON_V1310:
                /* RrcMacPuschConfigCommonV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPuschConfigCommonV1310(
                    &p_rrc_mac_reconfig_cell_req->puschConfigCommonV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13:
                /* RrcFrequancyHoppingParametersR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcFrequancyHoppingParametersR13(
                    &p_rrc_mac_reconfig_cell_req->frequancyHoppingParametersR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SFN_GAP_INFO_V1310:
                /* RrcMacSfnGapInfoV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacSfnGapInfoV1310(
                    &p_rrc_mac_reconfig_cell_req->sfnGapInfoV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_SFN_GAP_V1310_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_dci_format_for_si
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dci_format_for_si, 0, sizeof(rrc_mac_dci_format_for_si_t));

    /* This function parses rrc_mac_dci_format_for_si */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs, p_src + *p_length_read, "dci_format_for_si_msgs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs);
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
rrc_il_parse_rrc_mac_mod_period_info
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_mod_period_info, 0, sizeof(rrc_mac_mod_period_info_t));

    /* This function parses rrc_mac_mod_period_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_mod_period_info->mod_period, p_src + *p_length_read, "mod_period");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_mod_period_info->mod_period < 64) || (p_rrc_mac_mod_period_info->mod_period > 4096))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_mod_period_info->mod_period] should be in range "
            "64 to 4096. Incorrect value %u received.", p_rrc_mac_mod_period_info->mod_period);
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
rrc_il_parse_rrc_mac_sfn_gap_info
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sfn_gap_info, 0, sizeof(rrc_mac_sfn_gap_info_t));

    /* This function parses rrc_mac_sfn_gap_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sfn_gap_info->sfn_gap, p_src + *p_length_read, "sfn_gap");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sfn_gap_info->sfn_gap < 1) || (p_rrc_mac_sfn_gap_info->sfn_gap > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_gap_info->sfn_gap] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_mac_sfn_gap_info->sfn_gap);
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
rrc_il_parse_RrcMacEmtcSfn
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacEmtcSfn, 0, sizeof(RrcMacEmtcSfn_t));

    /* This function parses RrcMacEmtcSfn */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacEmtcSfn->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcSfn->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcSfn->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_RrcMacEmtcSfn->sfn);
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
rrc_il_parse_RrcMacSfnGapInfoV1310
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacSfnGapInfoV1310, 0, sizeof(RrcMacSfnGapInfoV1310_t));

    /* This function parses RrcMacSfnGapInfoV1310 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacSfnGapInfoV1310->sfnGapV1310, p_src + *p_length_read, "sfnGapV1310");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSfnGapInfoV1310->sfnGapV1310 < 1) || (p_RrcMacSfnGapInfoV1310->sfnGapV1310 > EMTC_SFN_GAP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSfnGapInfoV1310->sfnGapV1310] should be in range "
            "1 to EMTC_SFN_GAP_MAX. Incorrect value %u received.", p_RrcMacSfnGapInfoV1310->sfnGapV1310);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_reconfig_cell_cnf
(
    rrc_mac_reconfig_cell_cnf_t *p_rrc_mac_reconfig_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfig_cell_cnf, 0, sizeof(rrc_mac_reconfig_cell_cnf_t));

    /* This function parses rrc_mac_reconfig_cell_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfig_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfig_cell_cnf->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_create_ue_entity_req
(
    rrc_mac_create_ue_entity_req_t *p_rrc_mac_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_create_ue_entity_req, 0, sizeof(rrc_mac_create_ue_entity_req_t));

    /* This function parses rrc_mac_create_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_create_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_ADD_UE_INFO:
                /* rrc_mac_add_ue_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_add_ue_info(
                    &p_rrc_mac_create_ue_entity_req->add_ue_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_req->optional_elems_present |= RRC_MAC_ADD_UE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CREATE_LC_REQ:
                /* rrc_mac_create_lc_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_create_ue_entity_req->create_lc_req_counter >= (S32)ARRSIZE(p_rrc_mac_create_ue_entity_req->create_lc_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_create_lc_req(
                    &p_rrc_mac_create_ue_entity_req->create_lc_req[p_rrc_mac_create_ue_entity_req->create_lc_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_req->create_lc_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_RAT1_INFO:
                /* rrc_mac_uplink_rat1_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_uplink_rat1_info(
                    &p_rrc_mac_create_ue_entity_req->uplink_rat1_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_req->optional_elems_present |= RRC_MAC_ADD_RAT1_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sr_setup_info
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sr_setup_info, 0, sizeof(rrc_mac_sr_setup_info_t));

    /* This function parses rrc_mac_sr_setup_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sr_setup_info->sr_pucch_resource_index, p_src + *p_length_read, "sr_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_setup_info->sr_pucch_resource_index > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_setup_info->sr_pucch_resource_index] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_sr_setup_info->sr_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sr_setup_info->sr_configuration_index, p_src + *p_length_read, "sr_configuration_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_setup_info->sr_configuration_index > 157))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_setup_info->sr_configuration_index] should be less than"
            " or equal to 157. Incorrect value %u received.", p_rrc_mac_sr_setup_info->sr_configuration_index);
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
rrc_il_parse_rrc_mac_sr_config_info
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sr_config_info, 0, sizeof(rrc_mac_sr_config_info_t));

    /* This function parses rrc_mac_sr_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sr_config_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sr_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SR_SETUP_INFO:
                /* rrc_mac_sr_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sr_setup_info(
                    &p_rrc_mac_sr_config_info->sr_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sr_config_info->optional_elems_present |= RRC_MAC_SR_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sr_prohbit_timer_value_v920
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sr_prohbit_timer_value_v920, 0, sizeof(rrc_mac_sr_prohbit_timer_value_v920_t));

    /* This function parses rrc_mac_sr_prohbit_timer_value_v920 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer, p_src + *p_length_read, "sr_prohibit_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer);
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
rrc_il_parse_aperiodic_csi_trigger_r10
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_aperiodic_csi_trigger_r10, 0, sizeof(aperiodic_csi_trigger_r10_t));

    /* This function parses aperiodic_csi_trigger_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_aperiodic_csi_trigger_r10->trigger1r10, p_src + *p_length_read, "trigger1r10");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_aperiodic_csi_trigger_r10->trigger1r10 > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_aperiodic_csi_trigger_r10->trigger1r10] should be less than"
            " or equal to 64. Incorrect value %u received.", p_aperiodic_csi_trigger_r10->trigger1r10);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_aperiodic_csi_trigger_r10->trigger2r10, p_src + *p_length_read, "trigger2r10");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_aperiodic_csi_trigger_r10->trigger2r10 > 192))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_aperiodic_csi_trigger_r10->trigger2r10] should be less than"
            " or equal to 192. Incorrect value %u received.", p_aperiodic_csi_trigger_r10->trigger2r10);
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
rrc_il_parse_rrc_mac_cqi_aperiodic_config_info
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_aperiodic_config_info, 0, sizeof(rrc_mac_cqi_aperiodic_config_info_t));

    /* This function parses rrc_mac_cqi_aperiodic_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode, p_src + *p_length_read, "cqi_aperiodic_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode < 12) || (p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode] should be in range "
            "12 to 31. Incorrect value %u received.", p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_APERIODIC_CSI_TRIGGER_R10:
                /* aperiodic_csi_trigger_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_aperiodic_csi_trigger_r10(
                    &p_rrc_mac_cqi_aperiodic_config_info->aperiodic_csi_trigger_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_aperiodic_config_info->optional_elems_present |= RRC_MAC_APERIODIC_CSI_TRIGGER_R10_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k, 0, sizeof(rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t));

    /* This function parses rrc_mac_cqi_fmt_ind_perdiodic_subband_k */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k, p_src + *p_length_read, "cqi_format_indicator_periodic_subband_k");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k);
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
rrc_il_parse_rrc_mac_ri_config_index_info
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ri_config_index_info, 0, sizeof(rrc_mac_ri_config_index_info_t));

    /* This function parses rrc_mac_ri_config_index_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ri_config_index_info->ri_config_index, p_src + *p_length_read, "ri_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ri_config_index_info->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ri_config_index_info->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_ri_config_index_info->ri_config_index);
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
rrc_il_parse_rrc_mac_cqi_pmi_config_index_info
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_pmi_config_index_info, 0, sizeof(rrc_mac_cqi_pmi_config_index_info_t));

    /* This function parses rrc_mac_cqi_pmi_config_index_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index, p_src + *p_length_read, "cqi_pmi_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index, p_src + *p_length_read, "cqi_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi, p_src + *p_length_read, "simultaneous_ack_nack_and_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic, p_src + *p_length_read, "cqi_fmt_ind_periodic");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K:
                /* rrc_mac_cqi_fmt_ind_perdiodic_subband_k TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_fmt_ind_perdiodic_subband_k(
                    &p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic_subband_k,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present |= RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RI_CONFIG_INDEX_INFO:
                /* rrc_mac_ri_config_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ri_config_index_info(
                    &p_rrc_mac_cqi_pmi_config_index_info->ri_config_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present |= RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_cqi_pmi_config_index2_r10
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_cqi_pmi_config_index2_r10, 0, sizeof(cqi_pmi_config_index2_r10_t));

    /* This function parses cqi_pmi_config_index2_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2, p_src + *p_length_read, "cqi_pmi_config_index2");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2);
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
rrc_il_parse_rrc_mac_csi_config_index_v10
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_csi_config_index_v10, 0, sizeof(rrc_mac_csi_config_index_v10_t));

    /* This function parses rrc_mac_csi_config_index_v10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_csi_config_index_v10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_config_index_v10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_config_index_v10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_csi_config_index_v10->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10:
                /* cqi_pmi_config_index2_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_cqi_pmi_config_index2_r10(
                    &p_rrc_mac_csi_config_index_v10->cqi_pmi_config_index2_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_csi_config_index_v10->optional_elems_present |= RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RI_CONFIG_INDEX_INFO:
                /* rrc_mac_ri_config_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ri_config_index_info(
                    &p_rrc_mac_csi_config_index_v10->ri_config_index2_r_10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_csi_config_index_v10->optional_elems_present |= RRC_MAC_RI_CONFIG_INDEX2_R10_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_cqi_pmi_config_index_info_v1020
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_pmi_config_index_info_v1020, 0, sizeof(rrc_mac_cqi_pmi_config_index_info_v1020_t));

    /* This function parses rrc_mac_cqi_pmi_config_index_info_v1020 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index, p_src + *p_length_read, "cqi_pmi_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index, p_src + *p_length_read, "cqi_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi, p_src + *p_length_read, "simultaneous_ack_nack_and_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic, p_src + *p_length_read, "cqi_fmt_ind_periodic");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K:
                /* rrc_mac_cqi_fmt_ind_perdiodic_subband_k TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_fmt_ind_perdiodic_subband_k(
                    &p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic_subband_k,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present |= RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RI_CONFIG_INDEX_INFO:
                /* rrc_mac_ri_config_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ri_config_index_info(
                    &p_rrc_mac_cqi_pmi_config_index_info_v1020->ri_config_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present |= RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CSI_CONFIG_INDEX_V10:
                /* rrc_mac_csi_config_index_v10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_csi_config_index_v10(
                    &p_rrc_mac_cqi_pmi_config_index_info_v1020->csi_config_index_v10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present |= RRC_MAC_CSI_CONFIG_INDEX_V10_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_cqi_periodic_config_info
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_periodic_config_info, 0, sizeof(rrc_mac_cqi_periodic_config_info_t));

    /* This function parses rrc_mac_cqi_periodic_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_periodic_config_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_periodic_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_periodic_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_periodic_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO:
                /* rrc_mac_cqi_pmi_config_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_pmi_config_index_info(
                    &p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_periodic_config_info->optional_elems_present |= RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020:
                /* rrc_mac_cqi_pmi_config_index_info_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_pmi_config_index_info_v1020(
                    &p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_periodic_config_info->optional_elems_present |= RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_cqi_mask_r9
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_mask_r9, 0, sizeof(rrc_mac_cqi_mask_r9_t));

    /* This function parses rrc_mac_cqi_mask_r9 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_cqi_mask_r9->cqi_mask_r9, p_src + *p_length_read, "cqi_mask_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_mask_r9->cqi_mask_r9 > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_mask_r9->cqi_mask_r9] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_mask_r9->cqi_mask_r9);
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
rrc_il_parse_rrc_mac_pmi_ri_report_r9
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pmi_ri_report_r9, 0, sizeof(rrc_mac_pmi_ri_report_r9_t));

    /* This function parses rrc_mac_pmi_ri_report_r9 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9, p_src + *p_length_read, "pmi_ri_report_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9 > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9);
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
rrc_il_parse_rrc_mac_cqi_report_config_v920
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_report_config_v920, 0, sizeof(rrc_mac_cqi_report_config_v920_t));

    /* This function parses rrc_mac_cqi_report_config_v920 */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9:
                /* rrc_mac_cqi_mask_r9 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_mask_r9(
                    &p_rrc_mac_cqi_report_config_v920->rrc_mac_cqi_mask_r9,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_report_config_v920->optional_elems_present |= RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9:
                /* rrc_mac_pmi_ri_report_r9 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pmi_ri_report_r9(
                    &p_rrc_mac_cqi_report_config_v920->rrc_mac_pmi_ri_report_r9,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_report_config_v920->optional_elems_present |= RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_csi_meas_subframe_set1_r10
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_csi_meas_subframe_set1_r10, 0, sizeof(rrc_mac_csi_meas_subframe_set1_r10_t));

    /* This function parses rrc_mac_csi_meas_subframe_set1_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_csi_meas_subframe_set1_r10->choice, p_src + *p_length_read, "choice");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_meas_subframe_set1_r10->choice > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_meas_subframe_set1_r10->choice] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_csi_meas_subframe_set1_r10->choice);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_csi_meas_subframe_set1_r10->count %u is exceeding p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset[loop], p_src + *p_length_read + loop, "csi_measurement_subset[]");
        }

        p_rrc_mac_csi_meas_subframe_set1_r10->count = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_rrc_mac_csi_meas_subframe_set2_r10
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_csi_meas_subframe_set2_r10, 0, sizeof(rrc_mac_csi_meas_subframe_set2_r10_t));

    /* This function parses rrc_mac_csi_meas_subframe_set2_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_csi_meas_subframe_set2_r10->choice, p_src + *p_length_read, "choice");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_meas_subframe_set2_r10->choice > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_meas_subframe_set2_r10->choice] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_csi_meas_subframe_set2_r10->choice);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_csi_meas_subframe_set2_r10->count %u is exceeding p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset[loop], p_src + *p_length_read + loop, "csi_measurement_subset[]");
        }

        p_rrc_mac_csi_meas_subframe_set2_r10->count = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_rrc_mac_csi_subframe_pattern_config_v10
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_csi_subframe_pattern_config_v10, 0, sizeof(rrc_mac_csi_subframe_pattern_config_v10_t));

    /* This function parses rrc_mac_csi_subframe_pattern_config_v10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_csi_subframe_pattern_config_v10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_subframe_pattern_config_v10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_subframe_pattern_config_v10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_csi_subframe_pattern_config_v10->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10:
                /* rrc_mac_csi_meas_subframe_set1_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_csi_meas_subframe_set1_r10(
                    &p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set1_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present |= RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10:
                /* rrc_mac_csi_meas_subframe_set2_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_csi_meas_subframe_set2_r10(
                    &p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set2_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present |= RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_cqi_info
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cqi_info, 0, sizeof(rrc_mac_cqi_info_t));

    /* This function parses rrc_mac_cqi_info */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_APERIODIC_CONFIG_INFO:
                /* rrc_mac_cqi_aperiodic_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_aperiodic_config_info(
                    &p_rrc_mac_cqi_info->cqi_aperiodic_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_info->optional_elems_present |= RRC_MAC_CQI_APERIODIC_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_PERIODIC_CONFIG_INFO:
                /* rrc_mac_cqi_periodic_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_periodic_config_info(
                    &p_rrc_mac_cqi_info->cqi_periodic_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_info->optional_elems_present |= RRC_MAC_CQI_PERIODIC_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_REPORT_CONFIG_V920:
                /* rrc_mac_cqi_report_config_v920 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_report_config_v920(
                    &p_rrc_mac_cqi_info->cqi_report_config_v920,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_info->optional_elems_present |= RRC_MAC_CQI_REPORT_CONFIG_V920_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10:
                /* rrc_mac_csi_subframe_pattern_config_v10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_csi_subframe_pattern_config_v10(
                    &p_rrc_mac_cqi_info->rrc_mac_csi_subframe_pattern_config_v10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cqi_info->optional_elems_present |= RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_tdd_ack_nack_feedback_mode
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tdd_ack_nack_feedback_mode, 0, sizeof(rrc_mac_tdd_ack_nack_feedback_mode_t));

    /* This function parses rrc_mac_tdd_ack_nack_feedback_mode */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode, p_src + *p_length_read, "tdd_ack_nack_feedback_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode);
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
rrc_il_parse_rrc_mac_pusch_config_info
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pusch_config_info, 0, sizeof(rrc_mac_pusch_config_info_t));

    /* This function parses rrc_mac_pusch_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pusch_config_info->beta_offset_ack_index, p_src + *p_length_read, "beta_offset_ack_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_ack_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_ack_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_ack_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pusch_config_info->beta_offset_ri_index, p_src + *p_length_read, "beta_offset_ri_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_ri_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_ri_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_ri_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pusch_config_info->beta_offset_cqi_index, p_src + *p_length_read, "beta_offset_cqi_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_cqi_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_cqi_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_cqi_index);
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
rrc_il_parse_rrc_mac_srs_dedicated_setup_info
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_srs_dedicated_setup_info, 0, sizeof(rrc_mac_srs_dedicated_setup_info_t));

    /* This function parses rrc_mac_srs_dedicated_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth, p_src + *p_length_read, "srs_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth, p_src + *p_length_read, "srs_hopping_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->freq_domain_position, p_src + *p_length_read, "freq_domain_position");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->freq_domain_position > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->freq_domain_position] should be less than"
            " or equal to 23. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->freq_domain_position);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->duration, p_src + *p_length_read, "duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->duration);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_srs_dedicated_setup_info->srs_config_index, p_src + *p_length_read, "srs_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_config_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->transmission_comb, p_src + *p_length_read, "transmission_comb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->transmission_comb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->transmission_comb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->transmission_comb);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_setup_info->cyclic_shift, p_src + *p_length_read, "cyclic_shift");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->cyclic_shift);
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
rrc_il_parse_rrc_mac_srs_dedicated_config
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_srs_dedicated_config, 0, sizeof(rrc_mac_srs_dedicated_config_t));

    /* This function parses rrc_mac_srs_dedicated_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_srs_dedicated_config->config_type, p_src + *p_length_read, "config_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_config->config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_config->config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_config->config_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SRS_DEDICATED_SETUP_INFO:
                /* rrc_mac_srs_dedicated_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_dedicated_setup_info(
                    &p_rrc_mac_srs_dedicated_config->srs_dedicated_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_srs_dedicated_config->optional_elems_present |= RRC_MAC_SRS_DEDICATED_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_codebook_subset_restriction
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_codebook_subset_restriction, 0, sizeof(rrc_mac_codebook_subset_restriction_t));

    /* This function parses rrc_mac_codebook_subset_restriction */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_codebook_subset_restriction->cbsr_type, p_src + *p_length_read, "cbsr_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction->cbsr_type > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction->cbsr_type] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction->cbsr_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_mac_codebook_subset_restriction->cbsr_value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction->cbsr_value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_codebook_subset_restriction->cbsr_value[loop], (void*)(p_src + *p_length_read), "cbsr_value[]");
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
rrc_il_parse_rrc_mac_short_drx_config
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_short_drx_config, 0, sizeof(rrc_mac_short_drx_config_t));

    /* This function parses rrc_mac_short_drx_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_short_drx_config->short_drx_cycle, p_src + *p_length_read, "short_drx_cycle");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_short_drx_config->short_drx_cycle > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_short_drx_config->short_drx_cycle] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_short_drx_config->short_drx_cycle);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_short_drx_config->short_drx_cycle_timer, p_src + *p_length_read, "short_drx_cycle_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_short_drx_config->short_drx_cycle_timer < 1) || (p_rrc_mac_short_drx_config->short_drx_cycle_timer > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_short_drx_config->short_drx_cycle_timer] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_mac_short_drx_config->short_drx_cycle_timer);
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
rrc_il_parse_rrc_mac_cmd_l2_trigger_info
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cmd_l2_trigger_info, 0, sizeof(rrc_mac_cmd_l2_trigger_info_t));

    /* This function parses rrc_mac_cmd_l2_trigger_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer, p_src + *p_length_read, "drx_mac_ce_timer");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer < 3) || (p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer > 2559))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer] should be in range "
            "3 to 2559. Incorrect value %u received.", p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold, p_src + *p_length_read, "drx_mac_ce_cmd_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold < 1) || (p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold > 2559))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold] should be in range "
            "1 to 2559. Incorrect value %u received.", p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold);
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
rrc_il_parse_rrc_mac_drx_setup_config_info
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_drx_setup_config_info, 0, sizeof(rrc_mac_drx_setup_config_info_t));

    /* This function parses rrc_mac_drx_setup_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_drx_setup_config_info->on_duration_timer, p_src + *p_length_read, "on_duration_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->on_duration_timer > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->on_duration_timer] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->on_duration_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_drx_setup_config_info->drx_inactivity_timer, p_src + *p_length_read, "drx_inactivity_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->drx_inactivity_timer > 22))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->drx_inactivity_timer] should be less than"
            " or equal to 22. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->drx_inactivity_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_drx_setup_config_info->drx_retransmission_timer, p_src + *p_length_read, "drx_retransmission_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->drx_retransmission_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->drx_retransmission_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->drx_retransmission_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_drx_setup_config_info->long_drx_cycle, p_src + *p_length_read, "long_drx_cycle");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->long_drx_cycle > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->long_drx_cycle] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->long_drx_cycle);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_drx_setup_config_info->drx_start_offset, p_src + *p_length_read, "drx_start_offset");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SHORT_DRX_CONFIG:
                /* rrc_mac_short_drx_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_short_drx_config(
                    &p_rrc_mac_drx_setup_config_info->short_drx,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_drx_setup_config_info->bitmask |= RRC_MAC_DRX_SETUP_CONFIG_SHORT_DRX_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DRX_MAC_CE_CMD_TRIGGER_INFO:
                /* rrc_mac_cmd_l2_trigger_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cmd_l2_trigger_info(
                    &p_rrc_mac_drx_setup_config_info->mac_ce_cmd_l2_trigger_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_drx_setup_config_info->bitmask |= RRC_MAC_DRX_SETUP_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_drx_config_info
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_drx_config_info, 0, sizeof(rrc_mac_drx_config_info_t));

    /* This function parses rrc_mac_drx_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_drx_config_info->drx_config_type, p_src + *p_length_read, "drx_config_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_config_info->drx_config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_config_info->drx_config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_drx_config_info->drx_config_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_DRX_SETUP_CONFIG_INFO:
                /* rrc_mac_drx_setup_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_drx_setup_config_info(
                    &p_rrc_mac_drx_config_info->drx_setup_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_drx_config_info->bitmask |= RRC_MAC_DRX_CONFIG_DRX_SETUP_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_meas_gap_setup_config_info
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_meas_gap_setup_config_info, 0, sizeof(rrc_mac_meas_gap_setup_config_info_t));

    /* This function parses rrc_mac_meas_gap_setup_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id, p_src + *p_length_read, "gap_pattern_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id > 11))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_meas_gap_setup_config_info->gap_offset, p_src + *p_length_read, "gap_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_setup_config_info->gap_offset > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_setup_config_info->gap_offset] should be less than"
            " or equal to 159. Incorrect value %u received.", p_rrc_mac_meas_gap_setup_config_info->gap_offset);
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
rrc_il_parse_rrc_mac_meas_gap_config_info
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_meas_gap_config_info, 0, sizeof(rrc_mac_meas_gap_config_info_t));

    /* This function parses rrc_mac_meas_gap_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_meas_gap_config_info->meas_gap_config_type, p_src + *p_length_read, "meas_gap_config_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_config_info->meas_gap_config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_config_info->meas_gap_config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_meas_gap_config_info->meas_gap_config_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_MEAS_GAP_SETUP_CONFIG_INFO:
                /* rrc_mac_meas_gap_setup_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_meas_gap_setup_config_info(
                    &p_rrc_mac_meas_gap_config_info->meas_gap_setup_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_meas_gap_config_info->bitmask |= RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_codebook_subset_restriction_v920
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_codebook_subset_restriction_v920, 0, sizeof(rrc_mac_codebook_subset_restriction_v920_t));

    /* This function parses rrc_mac_codebook_subset_restriction_v920 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_codebook_subset_restriction_v920->cbsr_type, p_src + *p_length_read, "cbsr_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction_v920->cbsr_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction_v920->cbsr_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction_v920->cbsr_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_mac_codebook_subset_restriction_v920->cbsr_value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction_v920->cbsr_value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_codebook_subset_restriction_v920->cbsr_value[loop], (void*)(p_src + *p_length_read), "cbsr_value[]");
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
rrc_il_parse_rrc_mac_uplink_power_control_dedicated
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_uplink_power_control_dedicated, 0, sizeof(rrc_mac_uplink_power_control_dedicated_t));

    /* This function parses rrc_mac_uplink_power_control_dedicated */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch, p_src + *p_length_read, "p0_ue_pusch");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch < -8) || (p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled, p_src + *p_length_read, "delta_mcs_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled, p_src + *p_length_read, "accumulation_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled);
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
rrc_il_parse_rrc_mac_tpc_rnti_info
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tpc_rnti_info, 0, sizeof(rrc_mac_tpc_rnti_info_t));

    /* This function parses rrc_mac_tpc_rnti_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_tpc_rnti_info->tpcRnti, p_src + *p_length_read, "tpcRnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_info->tpcRnti < 1) || (p_rrc_mac_tpc_rnti_info->tpcRnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_info->tpcRnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_info->tpcRnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tpc_rnti_info->tpcIndex, p_src + *p_length_read, "tpcIndex");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_info->tpcIndex < 1) || (p_rrc_mac_tpc_rnti_info->tpcIndex > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_info->tpcIndex] should be in range "
            "1 to 31. Incorrect value %u received.", p_rrc_mac_tpc_rnti_info->tpcIndex);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tpc_rnti_info->isConfiguredForDci3Or3A, p_src + *p_length_read, "isConfiguredForDci3Or3A");
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
rrc_il_parse_rrc_mac_tpc_pdcch_config_pucch
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tpc_pdcch_config_pucch, 0, sizeof(rrc_mac_tpc_pdcch_config_pucch_t));

    /* This function parses rrc_mac_tpc_pdcch_config_pucch */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tpc_pdcch_config_pucch->release, p_src + *p_length_read, "release");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tpc_pdcch_config_pucch->release > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_pdcch_config_pucch->release] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tpc_pdcch_config_pucch->release);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_TPC_RNTI_INFO:
                /* rrc_mac_tpc_rnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_rnti_info(
                    &p_rrc_mac_tpc_pdcch_config_pucch->tpc_rnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_tpc_pdcch_config_pucch->bitmask |= RRC_MAC_TPC_RNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_tpc_pdcch_config_pusch
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tpc_pdcch_config_pusch, 0, sizeof(rrc_mac_tpc_pdcch_config_pusch_t));

    /* This function parses rrc_mac_tpc_pdcch_config_pusch */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tpc_pdcch_config_pusch->release, p_src + *p_length_read, "release");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tpc_pdcch_config_pusch->release > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_pdcch_config_pusch->release] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tpc_pdcch_config_pusch->release);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_TPC_RNTI_INFO:
                /* rrc_mac_tpc_rnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_rnti_info(
                    &p_rrc_mac_tpc_pdcch_config_pusch->tpc_rnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_tpc_pdcch_config_pusch->bitmask |= RRC_MAC_TPC_RNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_ambr_qos_info
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ambr_qos_info, 0, sizeof(rrc_mac_ambr_qos_info_t));

    /* This function parses rrc_mac_ambr_qos_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_ambr_qos_info->dl_ambr, p_src + *p_length_read, "dl_ambr");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_ambr_qos_info->ul_ambr, p_src + *p_length_read, "ul_ambr");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_user_location_type
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_user_location_type, 0, sizeof(rrc_mac_user_location_type_t));

    /* This function parses rrc_mac_user_location_type */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_user_location_type->user_location_type, p_src + *p_length_read, "user_location_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_user_location_type->user_location_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_user_location_type->user_location_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_user_location_type->user_location_type);
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
rrc_il_parse_rrc_mac_ue_capability_additional_params
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_capability_additional_params, 0, sizeof(rrc_mac_ue_capability_additional_params_t));

    /* This function parses rrc_mac_ue_capability_additional_params */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ue_capability_additional_params->rat_1_support, p_src + *p_length_read, "rat_1_support");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_capability_additional_params->rat_1_support > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_capability_additional_params->rat_1_support] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ue_capability_additional_params->rat_1_support);
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
rrc_il_parse_rrc_mac_n1_pucch_an_persist_list
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_n1_pucch_an_persist_list, 0, sizeof(rrc_mac_n1_pucch_an_persist_list_t));

    /* This function parses rrc_mac_n1_pucch_an_persist_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist, p_src + *p_length_read, "n1_pucch_an_persist");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist);
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
rrc_il_parse_rrc_mac_max_sps_harq_retx
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_max_sps_harq_retx, 0, sizeof(rrc_mac_max_sps_harq_retx_t));

    /* This function parses rrc_mac_max_sps_harq_retx */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx, p_src + *p_length_read, "max_sps_harq_retx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx);
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
rrc_il_parse_rrc_mac_explicit_release_after
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_explicit_release_after, 0, sizeof(rrc_mac_explicit_release_after_t));

    /* This function parses rrc_mac_explicit_release_after */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_explicit_release_after->explicit_release_afte, p_src + *p_length_read, "explicit_release_afte");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_explicit_release_after->explicit_release_afte > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_explicit_release_after->explicit_release_afte] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_explicit_release_after->explicit_release_afte);
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
rrc_il_parse_rrc_mac_sps_dl_setup_info
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_dl_setup_info, 0, sizeof(rrc_mac_sps_dl_setup_info_t));

    /* This function parses rrc_mac_sps_dl_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl, p_src + *p_length_read, "semi_persist_sched_interval_dl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes, p_src + *p_length_read, "number_of_conf_sps_processes");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes < 1) || (p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SPS_N1_PUCCH_AN_PERSIST_LIST:
                /* rrc_mac_n1_pucch_an_persist_list TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter >= (S32)ARRSIZE(p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_n1_pucch_an_persist_list(
                    &p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist[p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_MAX_SPS_HARQ_RETX:
                /* rrc_mac_max_sps_harq_retx TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_max_sps_harq_retx(
                    &p_rrc_mac_sps_dl_setup_info->max_sps_harq_retx,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_dl_setup_info->bitmask |= RRC_MAC_MAX_SPS_HARQ_RETX_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_EXPLICIT_RELEASE_AFTER:
                /* rrc_mac_explicit_release_after TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_explicit_release_after(
                    &p_rrc_mac_sps_dl_setup_info->explicit_release_after,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_dl_setup_info->bitmask |= RRC_MAC_RRC_MAC_EXPLICIT_RELEASE_AFTER_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sps_dl_info
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_dl_info, 0, sizeof(rrc_mac_sps_dl_info_t));

    /* This function parses rrc_mac_sps_dl_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_dl_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sps_dl_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SPS_DL_SETUP_INFO:
                /* rrc_mac_sps_dl_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_dl_setup_info(
                    &p_rrc_mac_sps_dl_info->sps_dl_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_dl_info->bitmask |= RRC_MAC_SPS_DL_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_p_zero_persistent
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_p_zero_persistent, 0, sizeof(rrc_mac_p_zero_persistent_t));

    /* This function parses rrc_mac_p_zero_persistent */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent, p_src + *p_length_read, "p_zero_nominal_pusch_persistent");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent < -126) || (p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent > 24))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent] should be in range "
            "-126 to 24. Incorrect value %d received.", p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent, p_src + *p_length_read, "p_zero_ue_pusch_persistent");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent < -8) || (p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent);
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
rrc_il_parse_rrc_mac_two_intervals_config
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_two_intervals_config, 0, sizeof(rrc_mac_two_intervals_config_t));

    /* This function parses rrc_mac_two_intervals_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_two_intervals_config->two_intervals_config, p_src + *p_length_read, "two_intervals_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_two_intervals_config->two_intervals_config < 1) || (p_rrc_mac_two_intervals_config->two_intervals_config > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_two_intervals_config->two_intervals_config] should be in range "
            "1 to 1. Incorrect value %u received.", p_rrc_mac_two_intervals_config->two_intervals_config);
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
rrc_il_parse_rrc_mac_sps_ul_setup_info
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_ul_setup_info, 0, sizeof(rrc_mac_sps_ul_setup_info_t));

    /* This function parses rrc_mac_sps_ul_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul, p_src + *p_length_read, "semi_persist_sched_interval_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_ul_setup_info->implicit_release_after, p_src + *p_length_read, "implicit_release_after");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_setup_info->implicit_release_after > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_setup_info->implicit_release_after] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_sps_ul_setup_info->implicit_release_after);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_P_ZERO_PERSISTENT:
                /* rrc_mac_p_zero_persistent TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_p_zero_persistent(
                    &p_rrc_mac_sps_ul_setup_info->p_zero_persistent,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_ul_setup_info->bitmask |= RRC_MAC_P_ZERO_PERSISTENT_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TWO_INTERVALS_CONFIG:
                /* rrc_mac_two_intervals_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_two_intervals_config(
                    &p_rrc_mac_sps_ul_setup_info->two_intervals_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_ul_setup_info->bitmask |= RRC_MAC_TWO_INTERVALS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sps_ul_info
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_ul_info, 0, sizeof(rrc_mac_sps_ul_info_t));

    /* This function parses rrc_mac_sps_ul_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_sps_ul_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sps_ul_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SPS_UL_SETUP_INFO:
                /* rrc_mac_sps_ul_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_ul_setup_info(
                    &p_rrc_mac_sps_ul_info->sps_ul_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_ul_info->bitmask |= RRC_MAC_SPS_UL_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sps_config
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_config, 0, sizeof(rrc_mac_sps_config_t));

    /* This function parses rrc_mac_sps_config */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SPS_DL_INFO:
                /* rrc_mac_sps_dl_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_dl_info(
                    &p_rrc_mac_sps_config->sps_dl_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_config->bitmask |= RRC_MAC_SPS_DL_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SPS_UL_INFO:
                /* rrc_mac_sps_ul_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_ul_info(
                    &p_rrc_mac_sps_config->sps_ul_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_sps_config->bitmask |= RRC_MAC_SPS_UL_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_rf_params
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_rf_params, 0, sizeof(rrc_mac_rf_params_t));

    /* This function parses rrc_mac_rf_params */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_rf_params->halfDuplexFdd, p_src + *p_length_read, "halfDuplexFdd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rf_params->halfDuplexFdd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rf_params->halfDuplexFdd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_rf_params->halfDuplexFdd);
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
rrc_il_parse_rrc_mac_ue_cat
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_cat, 0, sizeof(rrc_mac_ue_cat_t));

    /* This function parses rrc_mac_ue_cat */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ue_cat->ue_category, p_src + *p_length_read, "ue_category");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_cat->ue_category < 1) || (p_rrc_mac_ue_cat->ue_category > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_cat->ue_category] should be in range "
            "1 to 5. Incorrect value %u received.", p_rrc_mac_ue_cat->ue_category);
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
rrc_il_parse_rrc_mac_ul_mu_mimo_info
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ul_mu_mimo_info, 0, sizeof(rrc_mac_ul_mu_mimo_info_t));

    /* This function parses rrc_mac_ul_mu_mimo_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status, p_src + *p_length_read, "ul_mu_mimo_status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status);
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
rrc_il_parse_rrc_mac_phr_timer_info
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_phr_timer_info, 0, sizeof(rrc_mac_phr_timer_info_t));

    /* This function parses rrc_mac_phr_timer_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_phr_timer_info->periodicPHRTimer, p_src + *p_length_read, "periodicPHRTimer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_info->periodicPHRTimer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_info->periodicPHRTimer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_phr_timer_info->periodicPHRTimer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_phr_timer_info->prohibitPHRTimer, p_src + *p_length_read, "prohibitPHRTimer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_info->prohibitPHRTimer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_info->prohibitPHRTimer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_phr_timer_info->prohibitPHRTimer);
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
rrc_il_parse_rrc_mac_phr_timer_config
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_phr_timer_config, 0, sizeof(rrc_mac_phr_timer_config_t));

    /* This function parses rrc_mac_phr_timer_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_phr_timer_config->phrTimerConfigType, p_src + *p_length_read, "phrTimerConfigType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_config->phrTimerConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_config->phrTimerConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_phr_timer_config->phrTimerConfigType);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PHR_TIMER_INFO:
                /* rrc_mac_phr_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_phr_timer_info(
                    &p_rrc_mac_phr_timer_config->phrTimerInfo,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_phr_timer_config->optional_elems_present |= RRC_MAC_PHR_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_min_ue_power
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_min_ue_power, 0, sizeof(rrc_mac_min_ue_power_t));

    /* This function parses rrc_mac_min_ue_power */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_min_ue_power->minUePower, p_src + *p_length_read, "minUePower");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_min_ue_power->minUePower < -40) || (p_rrc_mac_min_ue_power->minUePower > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_min_ue_power->minUePower] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_min_ue_power->minUePower);
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
rrc_il_parse_rrc_mac_ue_compliance_info
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_compliance_info, 0, sizeof(rrc_mac_ue_compliance_info_t));

    /* This function parses rrc_mac_ue_compliance_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ue_compliance_info->ue_compliance_release, p_src + *p_length_read, "ue_compliance_release");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_compliance_info->ue_compliance_release > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_compliance_info->ue_compliance_release] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_mac_ue_compliance_info->ue_compliance_release);
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
rrc_il_parse_rrc_mac_pdsch_epre_to_ue_rs_ratio
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pdsch_epre_to_ue_rs_ratio, 0, sizeof(rrc_mac_pdsch_epre_to_ue_rs_ratio_t));

    /* This function parses rrc_mac_pdsch_epre_to_ue_rs_ratio */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio, p_src + *p_length_read, "pdsch_epre_to_ue_rs_ratio");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio < -6) || (p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio] should be in range "
            "-6 to 3. Incorrect value %d received.", p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio);
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
rrc_il_parse_rrc_mac_tti_bundling_info
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tti_bundling_info, 0, sizeof(rrc_mac_tti_bundling_info_t));

    /* This function parses rrc_mac_tti_bundling_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tti_bundling_info->tti_bundling, p_src + *p_length_read, "tti_bundling");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tti_bundling_info->tti_bundling > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tti_bundling_info->tti_bundling] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tti_bundling_info->tti_bundling);
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
rrc_il_parse_rrc_mac_operator_info
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_operator_info, 0, sizeof(rrc_mac_operator_info_t));

    /* This function parses rrc_mac_operator_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_operator_info->operator_id, p_src + *p_length_read, "operator_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_operator_info->operator_id > MAX_MOCN_OPERATOR_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_operator_info->operator_id] should be less than"
            " or equal to MAX_MOCN_OPERATOR_ID. Incorrect value %u received.", p_rrc_mac_operator_info->operator_id);
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
rrc_il_parse_rrc_mac_codebook_subset_restriction_v1020
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_codebook_subset_restriction_v1020, 0, sizeof(rrc_mac_codebook_subset_restriction_v1020_t));

    /* This function parses rrc_mac_codebook_subset_restriction_v1020 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type, p_src + *p_length_read, "cbsr_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value[loop], (void*)(p_src + *p_length_read), "cbsr_value[]");
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
rrc_il_parse_rrc_mac_dl_interference_info
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_interference_info, 0, sizeof(rrc_mac_dl_interference_info_t));

    /* This function parses rrc_mac_dl_interference_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference, p_src + *p_length_read, "severe_dl_control_channel_interference");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference);
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
rrc_il_parse_RrcAckNackRepetitionSetupR13
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcAckNackRepetitionSetupR13, 0, sizeof(RrcAckNackRepetitionSetupR13_t));

    /* This function parses RrcAckNackRepetitionSetupR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcAckNackRepetitionSetupR13->repetitionFactorR13, p_src + *p_length_read, "repetitionFactorR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcAckNackRepetitionSetupR13->repetitionFactorR13 > EMTC_REPETITION_FACTOR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcAckNackRepetitionSetupR13->repetitionFactorR13] should be less than"
            " or equal to EMTC_REPETITION_FACTOR_MAX. Incorrect value %u received.", p_RrcAckNackRepetitionSetupR13->repetitionFactorR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13, p_src + *p_length_read, "n1PucchAnRepR13");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13 > EMTC_MAX_N1_PUCCH_AN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13] should be less than"
            " or equal to EMTC_MAX_N1_PUCCH_AN. Incorrect value %u received.", p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13);
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
rrc_il_parse_RrcMacPucchAckNackRepetitionR13
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacPucchAckNackRepetitionR13, 0, sizeof(RrcMacPucchAckNackRepetitionR13_t));

    /* This function parses RrcMacPucchAckNackRepetitionR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype, p_src + *p_length_read, "ackNackRepetitiontype");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_ACK_NACK_REPETITION_SETUP_R13:
                /* RrcAckNackRepetitionSetupR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcAckNackRepetitionSetupR13(
                    &p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitionSetup,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacPucchAckNackRepetitionR13->bitmask |= RRC_MAC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcPucchNumRepetitionCeR13SetupModeA
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcPucchNumRepetitionCeR13SetupModeA, 0, sizeof(RrcPucchNumRepetitionCeR13SetupModeA_t));

    /* This function parses RrcPucchNumRepetitionCeR13SetupModeA */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1, p_src + *p_length_read, "numRepetitionCeModeAFormat1");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX. Incorrect value %u received.", p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2, p_src + *p_length_read, "numRepetitionCeModeAFormat2");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX. Incorrect value %u received.", p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2);
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
rrc_il_parse_RrcMacPucchNumRepetitionCeR13
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMacPucchNumRepetitionCeR13, 0, sizeof(RrcMacPucchNumRepetitionCeR13_t));

    /* This function parses RrcMacPucchNumRepetitionCeR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPucchNumRepetitionCeR13->numRepetitionType, p_src + *p_length_read, "numRepetitionType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchNumRepetitionCeR13->numRepetitionType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchNumRepetitionCeR13->numRepetitionType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMacPucchNumRepetitionCeR13->numRepetitionType);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13:
                /* RrcPucchNumRepetitionCeR13SetupModeA TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcPucchNumRepetitionCeR13SetupModeA(
                    &p_RrcMacPucchNumRepetitionCeR13->setup,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_RrcMacPucchNumRepetitionCeR13->bitmask |= RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_PucchConfigDedicatedR13
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_PucchConfigDedicatedR13, 0, sizeof(PucchConfigDedicatedR13_t));

    /* This function parses PucchConfigDedicatedR13 */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_ACK_NACK_REPETITION_R13:
                /* RrcMacPucchAckNackRepetitionR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPucchAckNackRepetitionR13(
                    &p_PucchConfigDedicatedR13->ackNackRepetitionR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_PucchConfigDedicatedR13->bitmask |= RRC_MAC_ACK_NACK_REPETITION_R13_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_NUM_REPETITION_CE_R13:
                /* RrcMacPucchNumRepetitionCeR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPucchNumRepetitionCeR13(
                    &p_PucchConfigDedicatedR13->pucchNumRepetitionCeR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_PucchConfigDedicatedR13->bitmask |= RRC_MAC_PUCCH_NUM_REPETITION_CE_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_MpdcchConfigSetupR13
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_MpdcchConfigSetupR13, 0, sizeof(MpdcchConfigSetupR13_t));

    /* This function parses MpdcchConfigSetupR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->csiNumRepetitionCeR13, p_src + *p_length_read, "csiNumRepetitionCeR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->csiNumRepetitionCeR13 > EMTC_CSI_NUM_REPETITION_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->csiNumRepetitionCeR13] should be less than"
            " or equal to EMTC_CSI_NUM_REPETITION_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->csiNumRepetitionCeR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13, p_src + *p_length_read, "mpdcchPdschHoppingConfigR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13 > EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13] should be less than"
            " or equal to EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13, p_src + *p_length_read, "mpdcchNumRepetitionR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13 > EMTC_MPDCCH_NUM_REPETITION_UESS_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_UESS_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->mpdcchNarrowbandR13, p_src + *p_length_read, "mpdcchNarrowbandR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchNarrowbandR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchNarrowbandR13] should be less than"
            " or equal to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchNarrowbandR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType, p_src + *p_length_read, "mpdcchStartSfUessFddType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13, p_src + *p_length_read, "mpdcchStartSfUessFddR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13 > EMTC_MPDCCH_START_SF_UESS_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_UESS_FDD_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13);
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
rrc_il_parse_MpdcchConfigR13
(
    MpdcchConfigR13_t *p_MpdcchConfigR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_MpdcchConfigR13, 0, sizeof(MpdcchConfigR13_t));

    /* This function parses MpdcchConfigR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_MpdcchConfigR13->mpdcchConfigType, p_src + *p_length_read, "mpdcchConfigType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigR13->mpdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigR13->mpdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_MpdcchConfigR13->mpdcchConfigType);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_MPDCCH_CONFIG_SETUP_R13:
                /* MpdcchConfigSetupR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_MpdcchConfigSetupR13(
                    &p_MpdcchConfigR13->mpdcchConfigSetupR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_MpdcchConfigR13->bitmask |= RRC_MAC_MPDCCH_CONFIG_SETUP_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_EpdcchSetConfigR11
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_EpdcchSetConfigR11, 0, sizeof(EpdcchSetConfigR11_t));

    /* This function parses EpdcchSetConfigR11 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->setConfigIdR11, p_src + *p_length_read, "setConfigIdR11");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->setConfigIdR11 > EMTC_MAX_EPDCCH_CONFIG_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->setConfigIdR11] should be less than"
            " or equal to EMTC_MAX_EPDCCH_CONFIG_ID. Incorrect value %u received.", p_EpdcchSetConfigR11->setConfigIdR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->transmissionTypeR11, p_src + *p_length_read, "transmissionTypeR11");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->transmissionTypeR11 > EMTC_TRANSMISSION_TYPE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->transmissionTypeR11] should be less than"
            " or equal to EMTC_TRANSMISSION_TYPE_MAX. Incorrect value %u received.", p_EpdcchSetConfigR11->transmissionTypeR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->numberPrbPairsR11, p_src + *p_length_read, "numberPrbPairsR11");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->numberPrbPairsR11 > EMTC_NUM_PRB_PAIR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->numberPrbPairsR11] should be less than"
            " or equal to EMTC_NUM_PRB_PAIR_MAX. Incorrect value %u received.", p_EpdcchSetConfigR11->numberPrbPairsR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->resourceBlockAssignmentR11, p_src + *p_length_read, "resourceBlockAssignmentR11");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->resourceBlockAssignmentR11 > 14))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->resourceBlockAssignmentR11] should be less than"
            " or equal to 14. Incorrect value %u received.", p_EpdcchSetConfigR11->resourceBlockAssignmentR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->resourceBlockAssignBitmap, p_src + *p_length_read, "resourceBlockAssignBitmap");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->resourceBlockAssignBitmap > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->resourceBlockAssignBitmap] should be less than"
            " or equal to 63. Incorrect value %u received.", p_EpdcchSetConfigR11->resourceBlockAssignBitmap);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11, p_src + *p_length_read, "dmrsScramblingSequenceIntR11");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11 > EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11] should be less than"
            " or equal to EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT. Incorrect value %u received.", p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_EpdcchSetConfigR11->pucchResourceStartOffsetR11, p_src + *p_length_read, "pucchResourceStartOffsetR11");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->pucchResourceStartOffsetR11 > EMTC_MAX_PUCCH_RESOURCE_START_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->pucchResourceStartOffsetR11] should be less than"
            " or equal to EMTC_MAX_PUCCH_RESOURCE_START_OFFSET. Incorrect value %u received.", p_EpdcchSetConfigR11->pucchResourceStartOffsetR11);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchSetConfigR11->startSymbol, p_src + *p_length_read, "startSymbol");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_EpdcchSetConfigR11->startSymbol < 1) || (p_EpdcchSetConfigR11->startSymbol > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->startSymbol] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_EpdcchSetConfigR11->startSymbol);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_MPDCCH_CONFIG_R13:
                /* MpdcchConfigR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_MpdcchConfigR13(
                    &p_EpdcchSetConfigR11->mpdcchConfigR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_EpdcchSetConfigR11->bitmask |= RRC_MAC_MPDCCH_CONFIG_R13_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcMacCeLevel
(
    RrcMacCeLevel_t *p_RrcMacCeLevel,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacCeLevel, 0, sizeof(RrcMacCeLevel_t));

    /* This function parses RrcMacCeLevel */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacCeLevel->ceLevel, p_src + *p_length_read, "ceLevel");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacCeLevel->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacCeLevel->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacCeLevel->ceLevel);
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
rrc_il_parse_RrcMacCeMode
(
    RrcMacCeMode_t *p_RrcMacCeMode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacCeMode, 0, sizeof(RrcMacCeMode_t));

    /* This function parses RrcMacCeMode */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacCeMode->ceMode, p_src + *p_length_read, "ceMode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacCeMode->ceMode > EMTC_CE_MODE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacCeMode->ceMode] should be less than"
            " or equal to EMTC_CE_MODE_MAX. Incorrect value %u received.", p_RrcMacCeMode->ceMode);
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
rrc_il_parse_rrc_mac_ue_category_v1020
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_category_v1020, 0, sizeof(rrc_mac_ue_category_v1020_t));

    /* This function parses rrc_mac_ue_category_v1020 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ue_category_v1020->ue_category_v1020, p_src + *p_length_read, "ue_category_v1020");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_category_v1020->ue_category_v1020 < 6) || (p_rrc_mac_ue_category_v1020->ue_category_v1020 > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_category_v1020->ue_category_v1020] should be in range "
            "6 to 8. Incorrect value %u received.", p_rrc_mac_ue_category_v1020->ue_category_v1020);
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
rrc_il_parse_rrc_mac_n1pucch_an_cs_r10
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_n1pucch_an_cs_r10, 0, sizeof(rrc_mac_n1pucch_an_cs_r10_t));

    /* This function parses rrc_mac_n1pucch_an_cs_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs, p_src + *p_length_read, "rrc_mac_n1pucch_an_cs");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs);
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
rrc_il_parse_rrc_mac_n1pucch_an_cs_list_r10
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_n1pucch_an_cs_list_r10, 0, sizeof(rrc_mac_n1pucch_an_cs_list_r10_t));

    /* This function parses rrc_mac_n1pucch_an_cs_list_r10 */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PUCCH_AN_CS_V1020:
                /* rrc_mac_n1pucch_an_cs_r10 TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_n1pucch_an_cs_list_r10->count >= (S32)ARRSIZE(p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_n1pucch_an_cs_r10(
                    &p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10[p_rrc_mac_n1pucch_an_cs_list_r10->count],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_n1pucch_an_cs_list_r10->count++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_pucch_channel_selection_config_v1020
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pucch_channel_selection_config_v1020, 0, sizeof(rrc_mac_pucch_channel_selection_config_v1020_t));

    /* This function parses rrc_mac_pucch_channel_selection_config_v1020 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pucch_channel_selection_config_v1020->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_channel_selection_config_v1020->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_channel_selection_config_v1020->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pucch_channel_selection_config_v1020->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_N1PUCCH_AN_CS_LIST_R10:
                /* rrc_mac_n1pucch_an_cs_list_r10 TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_pucch_channel_selection_config_v1020->count >= (S32)ARRSIZE(p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_n1pucch_an_cs_list_r10(
                    &p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10[p_rrc_mac_pucch_channel_selection_config_v1020->count],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pucch_channel_selection_config_v1020->count++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_pucch_format_r10
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pucch_format_r10, 0, sizeof(rrc_mac_pucch_format_r10_t));

    /* This function parses rrc_mac_pucch_format_r10 */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020:
                /* rrc_mac_pucch_channel_selection_config_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_channel_selection_config_v1020(
                    &p_rrc_mac_pucch_format_r10->rrc_mac_pucch_channel_selection_config_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pucch_format_r10->optional_elems_present |= RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_scell_deactivation_timer
(
    scell_deactivation_timer_t *p_scell_deactivation_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_scell_deactivation_timer, 0, sizeof(scell_deactivation_timer_t));

    /* This function parses scell_deactivation_timer */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_scell_deactivation_timer->scell_deactivation_timer, p_src + *p_length_read, "scell_deactivation_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_scell_deactivation_timer->scell_deactivation_timer > 128))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scell_deactivation_timer->scell_deactivation_timer] should be less than"
            " or equal to 128. Incorrect value %u received.", p_scell_deactivation_timer->scell_deactivation_timer);
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
rrc_il_parse_rrc_mac_supported_mimo_capability_dl_r10
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_supported_mimo_capability_dl_r10, 0, sizeof(rrc_mac_supported_mimo_capability_dl_r10_t));

    /* This function parses rrc_mac_supported_mimo_capability_dl_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer, p_src + *p_length_read, "num_Of_layer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer < 1) || (p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer);
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
rrc_il_parse_rrc_mac_tx_mode_info
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_tx_mode_info, 0, sizeof(rrc_mac_tx_mode_info_t));

    /* This function parses rrc_mac_tx_mode_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_tx_mode_info->transmission_mode, p_src + *p_length_read, "transmission_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tx_mode_info->transmission_mode < 1) || (p_rrc_mac_tx_mode_info->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tx_mode_info->transmission_mode] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_tx_mode_info->transmission_mode);
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
rrc_il_parse_rrc_mac_pdsch_config_info
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pdsch_config_info, 0, sizeof(rrc_mac_pdsch_config_info_t));

    /* This function parses rrc_mac_pdsch_config_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pdsch_config_info->p_a, p_src + *p_length_read, "p_a");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pdsch_config_info->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_info->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_pdsch_config_info->p_a);
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
rrc_il_parse_rrc_mac_dl_num_harq_process
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_num_harq_process, 0, sizeof(rrc_mac_dl_num_harq_process_t));

    /* This function parses rrc_mac_dl_num_harq_process */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_num_harq_process->dl_num_harq_process, p_src + *p_length_read, "dl_num_harq_process");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_num_harq_process->dl_num_harq_process < 1) || (p_rrc_mac_dl_num_harq_process->dl_num_harq_process > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_num_harq_process->dl_num_harq_process] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_mac_dl_num_harq_process->dl_num_harq_process);
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
rrc_il_parse_rrc_mac_dl_init_mcs
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_init_mcs, 0, sizeof(rrc_mac_dl_init_mcs_t));

    /* This function parses rrc_mac_dl_init_mcs */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_init_mcs->dl_init_mcs, p_src + *p_length_read, "dl_init_mcs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_init_mcs->dl_init_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_init_mcs->dl_init_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_dl_init_mcs->dl_init_mcs);
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
rrc_il_parse_rrc_mac_num_of_layer_info
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_num_of_layer_info, 0, sizeof(rrc_mac_num_of_layer_info_t));

    /* This function parses rrc_mac_num_of_layer_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_num_of_layer_info->num_of_layer, p_src + *p_length_read, "num_of_layer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_num_of_layer_info->num_of_layer < 1) || (p_rrc_mac_num_of_layer_info->num_of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_num_of_layer_info->num_of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_num_of_layer_info->num_of_layer);
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
rrc_il_parse_rrc_mac_code_book_index_info
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_code_book_index_info, 0, sizeof(rrc_mac_code_book_index_info_t));

    /* This function parses rrc_mac_code_book_index_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_code_book_index_info->code_book_index, p_src + *p_length_read, "code_book_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_code_book_index_info->code_book_index > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_code_book_index_info->code_book_index] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_code_book_index_info->code_book_index);
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
rrc_il_parse_scell_activate_status
(
    scell_activate_status_t *p_scell_activate_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_scell_activate_status, 0, sizeof(scell_activate_status_t));

    /* This function parses scell_activate_status */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_scell_activate_status->scellActivateStatus, p_src + *p_length_read, "scellActivateStatus");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_scell_activate_status->scellActivateStatus > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scell_activate_status->scellActivateStatus] should be less than"
            " or equal to 1. Incorrect value %u received.", p_scell_activate_status->scellActivateStatus);
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
rrc_il_parse_rrc_mac_dl_earfcn
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_earfcn, 0, sizeof(rrc_mac_dl_earfcn_t));

    /* This function parses rrc_mac_dl_earfcn */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_dl_earfcn->dlEarfcn, p_src + *p_length_read, "dlEarfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_earfcn->dlEarfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_earfcn->dlEarfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_rrc_mac_dl_earfcn->dlEarfcn);
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
rrc_il_parse_rrc_mac_scell_info
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_scell_info, 0, sizeof(rrc_mac_scell_info_t));

    /* This function parses rrc_mac_scell_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_scell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_info->cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_mac_scell_info->cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_scell_info->scell_index, p_src + *p_length_read, "scell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_scell_info->scell_index < 1) || (p_rrc_mac_scell_info->scell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->scell_index] should be in range "
            "1 to 7. Incorrect value %u received.", p_rrc_mac_scell_info->scell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_scell_info->operation_type, p_src + *p_length_read, "operation_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_info->operation_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->operation_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_scell_info->operation_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_MIMO_CAPABILITY_DL_V1020:
                /* rrc_mac_supported_mimo_capability_dl_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_supported_mimo_capability_dl_r10(
                    &p_rrc_mac_scell_info->rrc_mac_supported_mimo_capability_dl_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_MIMO_CAPABILITY_DL_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TX_MODE_INFO:
                /* rrc_mac_tx_mode_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tx_mode_info(
                    &p_rrc_mac_scell_info->tx_mode_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_TX_MODE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020:
                /* rrc_mac_codebook_subset_restriction_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction_v1020(
                    &p_rrc_mac_scell_info->rrc_mac_codebook_subset_restriction_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_INFO:
                /* rrc_mac_pdsch_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_config_info(
                    &p_rrc_mac_scell_info->pdsch_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_PDSCH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_INFO:
                /* rrc_mac_cqi_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_info(
                    &p_rrc_mac_scell_info->cqi_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_CQI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_NUM_HARQ_PROC:
                /* rrc_mac_dl_num_harq_process TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_num_harq_process(
                    &p_rrc_mac_scell_info->rrc_mac_dl_num_harq_process,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_DL_NUM_HARQ_PROC_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_INIT_MCS:
                /* rrc_mac_dl_init_mcs TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_init_mcs(
                    &p_rrc_mac_scell_info->rrc_mac_dl_init_mcs,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_DL_INIT_MCS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_NUM_OF_LAYER_INFO:
                /* rrc_mac_num_of_layer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_num_of_layer_info(
                    &p_rrc_mac_scell_info->num_of_layer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_NUM_OF_LAYER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODE_BOOK_INDEX_INFO:
                /* rrc_mac_code_book_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_code_book_index_info(
                    &p_rrc_mac_scell_info->code_book_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_CODE_BOOK_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_USER_LOCATION_TYPE:
                /* rrc_mac_user_location_type TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_user_location_type(
                    &p_rrc_mac_scell_info->user_location_type,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_USER_LOCATION_TYPE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RF_PARAMS:
                /* rrc_mac_rf_params TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rf_params(
                    &p_rrc_mac_scell_info->rf_params,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_RF_PARAMS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SCELL_ACTIVATE_STATUS:
                /* scell_activate_status TLV */
                if (RRC_FAILURE == rrc_il_parse_scell_activate_status(
                    &p_rrc_mac_scell_info->scell_activate_status,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_SCELL_ACTIVATE_STATUS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_EARFCN:
                /* rrc_mac_dl_earfcn TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_earfcn(
                    &p_rrc_mac_scell_info->dl_earfcn,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_scell_info->optional_elems_present |= RRC_MAC_DL_EARFCN_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_ca_lc_bandwidth_dist_info
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ca_lc_bandwidth_dist_info, 0, sizeof(rrc_mac_ca_lc_bandwidth_dist_info_t));

    /* This function parses rrc_mac_ca_lc_bandwidth_dist_info */

    if (*p_length_read + (S32)sizeof(p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop], (void*)(p_src + *p_length_read), "band_width_distribution[]");
            if (p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop]);
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
rrc_il_parse_rrc_mac_carrier_aggr_config
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_carrier_aggr_config, 0, sizeof(rrc_mac_carrier_aggr_config_t));

    /* This function parses rrc_mac_carrier_aggr_config */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO:
                /* scell_deactivation_timer TLV */
                if (RRC_FAILURE == rrc_il_parse_scell_deactivation_timer(
                    &p_rrc_mac_carrier_aggr_config->scell_deactivation_timer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_carrier_aggr_config->presence_bitmask |= RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SCELL_INFO:
                /* rrc_mac_scell_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_carrier_aggr_config->count >= (S32)ARRSIZE(p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_scell_info(
                    &p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info[p_rrc_mac_carrier_aggr_config->count],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_carrier_aggr_config->count++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO:
                /* rrc_mac_ca_lc_bandwidth_dist_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_lc_bandwidth_dist_info(
                    &p_rrc_mac_carrier_aggr_config->ca_lc_bandwidth_dist_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_carrier_aggr_config->presence_bitmask |= RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_add_ue_info
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_add_ue_info, 0, sizeof(rrc_mac_add_ue_info_t));

    /* This function parses rrc_mac_add_ue_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_add_ue_info->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->crnti < 1) || (p_rrc_mac_add_ue_info->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_add_ue_info->crnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->ue_priority, p_src + *p_length_read, "ue_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->ue_priority > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->ue_priority] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_add_ue_info->ue_priority);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->dl_num_harq_process, p_src + *p_length_read, "dl_num_harq_process");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->dl_num_harq_process < 1) || (p_rrc_mac_add_ue_info->dl_num_harq_process > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_num_harq_process] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_num_harq_process);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->dl_i_mcs, p_src + *p_length_read, "dl_i_mcs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->dl_i_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_i_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_i_mcs);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->dl_max_rb, p_src + *p_length_read, "dl_max_rb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->dl_max_rb < 1) || (p_rrc_mac_add_ue_info->dl_max_rb > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_max_rb] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_max_rb);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->ul_i_mcs, p_src + *p_length_read, "ul_i_mcs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->ul_i_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->ul_i_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_add_ue_info->ul_i_mcs);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->transmission_mode, p_src + *p_length_read, "transmission_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->transmission_mode < 1) || (p_rrc_mac_add_ue_info->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->transmission_mode] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_add_ue_info->transmission_mode);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->num_of_layer, p_src + *p_length_read, "num_of_layer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->num_of_layer < 1) || (p_rrc_mac_add_ue_info->num_of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->num_of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_add_ue_info->num_of_layer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->code_book_index, p_src + *p_length_read, "code_book_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->code_book_index > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->code_book_index] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_add_ue_info->code_book_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->max_ul_harq_tx, p_src + *p_length_read, "max_ul_harq_tx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->max_ul_harq_tx > 13))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->max_ul_harq_tx] should be less than"
            " or equal to 13. Incorrect value %u received.", p_rrc_mac_add_ue_info->max_ul_harq_tx);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_add_ue_info->time_allignment_timer, p_src + *p_length_read, "time_allignment_timer");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->time_allignment_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->time_allignment_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_add_ue_info->time_allignment_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_add_ue_info->pc_max, p_src + *p_length_read, "pc_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->pc_max < -40) || (p_rrc_mac_add_ue_info->pc_max > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->pc_max] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_add_ue_info->pc_max);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_add_ue_info->p_a, p_src + *p_length_read, "p_a");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_add_ue_info->p_a);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SR_CONFIG_INFO:
                /* rrc_mac_sr_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sr_config_info(
                    &p_rrc_mac_add_ue_info->sr_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_SR_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920:
                /* rrc_mac_sr_prohbit_timer_value_v920 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sr_prohbit_timer_value_v920(
                    &p_rrc_mac_add_ue_info->sr_prohbit_timer_value_v920,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CQI_INFO:
                /* rrc_mac_cqi_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_info(
                    &p_rrc_mac_add_ue_info->cqi_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_CQI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE:
                /* rrc_mac_tdd_ack_nack_feedback_mode TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tdd_ack_nack_feedback_mode(
                    &p_rrc_mac_add_ue_info->mac_tdd_ack_nack_feedback_mode,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_INFO:
                /* rrc_mac_pusch_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pusch_config_info(
                    &p_rrc_mac_add_ue_info->pusch_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_PUSCH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SRS_DEDICATED_CONFIG:
                /* rrc_mac_srs_dedicated_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_dedicated_config(
                    &p_rrc_mac_add_ue_info->srs_dedicated_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_SRS_DEDICATED_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION:
                /* rrc_mac_codebook_subset_restriction TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction(
                    &p_rrc_mac_add_ue_info->codebook_subset_restriction,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DRX_CONFIG_INFO:
                /* rrc_mac_drx_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_drx_config_info(
                    &p_rrc_mac_add_ue_info->drx_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_DRX_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MEAS_GAP_CONFIG_INFO:
                /* rrc_mac_meas_gap_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_meas_gap_config_info(
                    &p_rrc_mac_add_ue_info->meas_gap_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_MEAS_GAP_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920:
                /* rrc_mac_codebook_subset_restriction_v920 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction_v920(
                    &p_rrc_mac_add_ue_info->codebook_subset_restriction_v920,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED:
                /* rrc_mac_uplink_power_control_dedicated TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_uplink_power_control_dedicated(
                    &p_rrc_mac_add_ue_info->uplink_power_control_dedicated,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TPC_PDCCH_CONFIG_PUCCH:
                /* rrc_mac_tpc_pdcch_config_pucch TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_pdcch_config_pucch(
                    &p_rrc_mac_add_ue_info->tpc_pdcch_config_pucch,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_TPC_PDCCH_CONFIG_PUCCH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TPC_PDCCH_CONFIG_PUSCH:
                /* rrc_mac_tpc_pdcch_config_pusch TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_pdcch_config_pusch(
                    &p_rrc_mac_add_ue_info->tpc_pdcch_config_pusch,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_TPC_PDCCH_CONFIG_PUSCH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_AMBR_QOS_INFO:
                /* rrc_mac_ambr_qos_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ambr_qos_info(
                    &p_rrc_mac_add_ue_info->ambr_qos_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_AMBR_QOS_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_USER_LOCATION_TYPE:
                /* rrc_mac_user_location_type TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_user_location_type(
                    &p_rrc_mac_add_ue_info->user_location_type,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_USER_LOCATION_TYPE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS:
                /* rrc_mac_ue_capability_additional_params TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_capability_additional_params(
                    &p_rrc_mac_add_ue_info->ue_capability_additional_params,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SPS_CONFIG:
                /* rrc_mac_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_config(
                    &p_rrc_mac_add_ue_info->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RF_PARAMS:
                /* rrc_mac_rf_params TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rf_params(
                    &p_rrc_mac_add_ue_info->rf_params,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_CONFIG_RF_PARAMS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CATEGORY_INFO:
                /* rrc_mac_ue_cat TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_cat(
                    &p_rrc_mac_add_ue_info->ue_category_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_ADD_UE_CATEGORY_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UL_MU_MIMO_INFO:
                /* rrc_mac_ul_mu_mimo_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_mu_mimo_info(
                    &p_rrc_mac_add_ue_info->ul_mu_mimo_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_UL_MU_MIMO_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PHR_TIMER_CONFIG:
                /* rrc_mac_phr_timer_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_phr_timer_config(
                    &p_rrc_mac_add_ue_info->phr_timer_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_ADD_UE_PHR_TIMER_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MIN_UE_POWER:
                /* rrc_mac_min_ue_power TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_min_ue_power(
                    &p_rrc_mac_add_ue_info->min_ue_power,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_MIN_UE_POWER_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_COMPLIANCE_INFO:
                /* rrc_mac_ue_compliance_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_compliance_info(
                    &p_rrc_mac_add_ue_info->ue_compliance_release,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_UE_COMPLIANCE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO:
                /* rrc_mac_pdsch_epre_to_ue_rs_ratio TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_epre_to_ue_rs_ratio(
                    &p_rrc_mac_add_ue_info->pdsch_epre_to_ue_rs_ratio,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TTI_BUNDLING_INFO:
                /* rrc_mac_tti_bundling_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tti_bundling_info(
                    &p_rrc_mac_add_ue_info->tti_bundling,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_TTI_BUNDLING_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_OPERATOR_INFO:
                /* rrc_mac_operator_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_operator_info(
                    &p_rrc_mac_add_ue_info->operator_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_MOCN_OPERATOR_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020:
                /* rrc_mac_codebook_subset_restriction_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction_v1020(
                    &p_rrc_mac_add_ue_info->rrc_mac_codebook_subset_restriction_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_ADD_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_INTERFERENCE_INFO:
                /* rrc_mac_dl_interference_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_interference_info(
                    &p_rrc_mac_add_ue_info->rrc_mac_dl_interference_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->optional_elems_present |= RRC_MAC_DL_INTERFERENCE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CONFIG_DEDICATED_R13:
                /* PucchConfigDedicatedR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_PucchConfigDedicatedR13(
                    &p_rrc_mac_add_ue_info->pucchConfigDedicatedR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->emtc_optional_elems_present |= RRC_MAC_PUCCH_CONFIG_DEDICATED_R13_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_EPDCCH_SET_CONFIG_R11:
                /* EpdcchSetConfigR11 TLV */
                if (RRC_FAILURE == rrc_il_parse_EpdcchSetConfigR11(
                    &p_rrc_mac_add_ue_info->epdcchSetConfigR11,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->emtc_optional_elems_present |= RRC_MAC_EPDCCH_SET_CONFIG_R11_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CE_LEVEL:
                /* RrcMacCeLevel TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacCeLevel(
                    &p_rrc_mac_add_ue_info->ceLevel,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->emtc_optional_elems_present |= RRC_MAC_CE_LEVEL_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CE_MODE:
                /* RrcMacCeMode TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacCeMode(
                    &p_rrc_mac_add_ue_info->ceMode,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->emtc_optional_elems_present |= RRC_MAC_CE_MODE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CATEGORY_V1020:
                /* rrc_mac_ue_category_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_category_v1020(
                    &p_rrc_mac_add_ue_info->rrc_mac_ue_category_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->ca_optional_elems_present |= RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_FORMAT_V1020:
                /* rrc_mac_pucch_format_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_format_r10(
                    &p_rrc_mac_add_ue_info->rrc_mac_pucch_format_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->ca_optional_elems_present |= RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CA_CONFIG:
                /* rrc_mac_carrier_aggr_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_carrier_aggr_config(
                    &p_rrc_mac_add_ue_info->rrc_mac_carrier_aggr_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_add_ue_info->ca_optional_elems_present |= RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_configure_status_report_required_ue
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_configure_status_report_required_ue, 0, sizeof(rrc_mac_configure_status_report_required_ue_t));

    /* This function parses rrc_mac_configure_status_report_required_ue */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag, p_src + *p_length_read, "ue_psr_req_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag);
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
rrc_il_parse_rrc_mac_ul_lc_create_req
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ul_lc_create_req, 0, sizeof(rrc_mac_ul_lc_create_req_t));

    /* This function parses rrc_mac_ul_lc_create_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ul_lc_create_req->lc_g_id, p_src + *p_length_read, "lc_g_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ul_lc_create_req->lc_g_id > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ul_lc_create_req->lc_g_id] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_ul_lc_create_req->lc_g_id);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_UE_PSR_REQ:
                /* rrc_mac_configure_status_report_required_ue TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_configure_status_report_required_ue(
                    &p_rrc_mac_ul_lc_create_req->configure_status_report_required_ue,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_ul_lc_create_req->optional_elems_present |= RRC_MAC_UE_PSR_REQ_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_configure_status_report_required_enb
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_configure_status_report_required_enb, 0, sizeof(rrc_mac_configure_status_report_required_enb_t));

    /* This function parses rrc_mac_configure_status_report_required_enb */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag, p_src + *p_length_read, "enb_psr_req_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag);
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
rrc_il_parse_rrc_mac_dl_lc_create_req
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_lc_create_req, 0, sizeof(rrc_mac_dl_lc_create_req_t));

    /* This function parses rrc_mac_dl_lc_create_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_lc_create_req->lch_priority, p_src + *p_length_read, "lch_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_lc_create_req->lch_priority < 1) || (p_rrc_mac_dl_lc_create_req->lch_priority > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_lc_create_req->lch_priority] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_mac_dl_lc_create_req->lch_priority);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_lc_create_req->rlc_sn_field_length, p_src + *p_length_read, "rlc_sn_field_length");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_lc_create_req->rlc_sn_field_length < 5) || (p_rrc_mac_dl_lc_create_req->rlc_sn_field_length > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_lc_create_req->rlc_sn_field_length] should be in range "
            "5 to 10. Incorrect value %u received.", p_rrc_mac_dl_lc_create_req->rlc_sn_field_length);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_ENB_PSR_REQ:
                /* rrc_mac_configure_status_report_required_enb TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_configure_status_report_required_enb(
                    &p_rrc_mac_dl_lc_create_req->configure_status_report_required_enb,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_dl_lc_create_req->optional_elems_present |= RRC_MAC_ENB_PSR_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO:
                /* rrc_mac_ca_lc_bandwidth_dist_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_lc_bandwidth_dist_info(
                    &p_rrc_mac_dl_lc_create_req->ca_lc_bandwidth_dist_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_dl_lc_create_req->optional_elems_present |= RRC_MAC_DL_CREATE_CA_LC_BANDWIDTH_DIST_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_gbr_qos_info
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_gbr_qos_info, 0, sizeof(rrc_mac_gbr_qos_info_t));

    /* This function parses rrc_mac_gbr_qos_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_gbr_qos_info->e_rab_mbr_dl, p_src + *p_length_read, "e_rab_mbr_dl");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_gbr_qos_info->e_rab_mbr_ul, p_src + *p_length_read, "e_rab_mbr_ul");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_gbr_qos_info->e_rab_gbr_dl, p_src + *p_length_read, "e_rab_gbr_dl");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_gbr_qos_info->e_rab_gbr_ul, p_src + *p_length_read, "e_rab_gbr_ul");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_qos_info
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_qos_info, 0, sizeof(rrc_mac_qos_info_t));

    /* This function parses rrc_mac_qos_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_qos_info->bearer_type, p_src + *p_length_read, "bearer_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_qos_info->bearer_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_qos_info->bearer_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_qos_info->bearer_type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_qos_info->qci, p_src + *p_length_read, "qci");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_qos_info->qci < 1) || (p_rrc_mac_qos_info->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_qos_info->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_mac_qos_info->qci);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_GBR_QOS_INFO:
                /* rrc_mac_gbr_qos_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_gbr_qos_info(
                    &p_rrc_mac_qos_info->gbr_qos_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_qos_info->optional_elems_present |= RRC_MAC_GBR_QOS_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_rohc_profile
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_rohc_profile, 0, sizeof(rrc_mac_rohc_profile_t));

    /* This function parses rrc_mac_rohc_profile */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_rohc_profile->rohc_profile, p_src + *p_length_read, "rohc_profile");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rohc_profile->rohc_profile > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rohc_profile->rohc_profile] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_mac_rohc_profile->rohc_profile);
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
rrc_il_parse_rrc_mac_create_lc_req
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_create_lc_req, 0, sizeof(rrc_mac_create_lc_req_t));

    /* This function parses rrc_mac_create_lc_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_create_lc_req->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_create_lc_req->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_create_lc_req->rlc_mode, p_src + *p_length_read, "rlc_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_req->rlc_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_req->rlc_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_create_lc_req->rlc_mode);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_UL_LC_CREATE_REQ:
                /* rrc_mac_ul_lc_create_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_lc_create_req(
                    &p_rrc_mac_create_lc_req->ul_lc_create_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_req->optional_elems_present |= RRC_MAC_UL_LC_CREATE_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_LC_CREATE_REQ:
                /* rrc_mac_dl_lc_create_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_lc_create_req(
                    &p_rrc_mac_create_lc_req->dl_lc_create_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_req->optional_elems_present |= RRC_MAC_DL_LC_CREATE_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_QOS_INFO:
                /* rrc_mac_qos_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_qos_info(
                    &p_rrc_mac_create_lc_req->qos_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_req->optional_elems_present |= RRC_MAC_QOS_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_ROHC_PROFILE:
                /* rrc_mac_rohc_profile TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rohc_profile(
                    &p_rrc_mac_create_lc_req->rohc_profile,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_req->optional_elems_present |= RRC_MAC_ROHC_PROFILE_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_uplink_rat1_info
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_uplink_rat1_info, 0, sizeof(rrc_mac_uplink_rat1_info_t));

    /* This function parses rrc_mac_uplink_rat1_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported, p_src + *p_length_read, "is_uplink_rat1_supported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_create_ue_entity_cnf
(
    rrc_mac_create_ue_entity_cnf_t *p_rrc_mac_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_create_ue_entity_cnf, 0, sizeof(rrc_mac_create_ue_entity_cnf_t));

    /* This function parses rrc_mac_create_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_create_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_create_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_create_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CREATE_LC_ERROR:
                /* rrc_mac_create_lc_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter >= (S32)ARRSIZE(p_rrc_mac_create_ue_entity_cnf->create_lc_error))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_create_lc_error(
                    &p_rrc_mac_create_ue_entity_cnf->create_lc_error[p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SPS_CRNTI_INFO:
                /* rrc_mac_sps_crnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_crnti_info(
                    &p_rrc_mac_create_ue_entity_cnf->sps_crnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_cnf->bitmask |= RRC_MAC_SPS_CRNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CREATE_CA_CONFIG_ERROR:
                /* rrc_mac_ca_config_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter >= (S32)ARRSIZE(p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_config_error(
                    &p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err[p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_ul_lc_config_resp
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ul_lc_config_resp, 0, sizeof(rrc_mac_ul_lc_config_resp_t));

    /* This function parses rrc_mac_ul_lc_config_resp */

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_lc_config_resp
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_lc_config_resp, 0, sizeof(rrc_mac_dl_lc_config_resp_t));

    /* This function parses rrc_mac_dl_lc_config_resp */

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_create_lc_error
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_create_lc_error, 0, sizeof(rrc_mac_create_lc_error_t));

    /* This function parses rrc_mac_create_lc_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_create_lc_error->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_create_lc_error->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_create_lc_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_create_lc_error->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_UL_LC_CONFIG_RESP:
                /* rrc_mac_ul_lc_config_resp TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_lc_config_resp(
                    &p_rrc_mac_create_lc_error->ul_lc_config_resp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_error->optional_elems_present |= RRC_MAC_UL_LC_CONFIG_RESP_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_LC_CONFIG_RESP:
                /* rrc_mac_dl_lc_config_resp TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_lc_config_resp(
                    &p_rrc_mac_create_lc_error->dl_lc_config_resp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_create_lc_error->optional_elems_present |= RRC_MAC_DL_LC_CONFIG_RESP_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_sps_crnti_info
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_sps_crnti_info, 0, sizeof(rrc_mac_sps_crnti_info_t));

    /* This function parses rrc_mac_sps_crnti_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_sps_crnti_info->sps_crnti, p_src + *p_length_read, "sps_crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_info->sps_crnti < 61) || (p_rrc_mac_sps_crnti_info->sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_info->sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_info->sps_crnti);
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
rrc_il_parse_rrc_mac_ca_config_error
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ca_config_error, 0, sizeof(rrc_mac_ca_config_error_t));

    /* This function parses rrc_mac_ca_config_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ca_config_error->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ca_config_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_config_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_ca_config_error->cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ca_config_error->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ca_config_error->error_code < 7) || (p_rrc_mac_ca_config_error->error_code > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_config_error->error_code] should be in range "
            "7 to 7. Incorrect value %u received.", p_rrc_mac_ca_config_error->error_code);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_delete_ue_entity_req
(
    rrc_mac_delete_ue_entity_req_t *p_rrc_mac_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_ue_entity_req, 0, sizeof(rrc_mac_delete_ue_entity_req_t));

    /* This function parses rrc_mac_delete_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_delete_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_delete_ue_entity_cnf
(
    rrc_mac_delete_ue_entity_cnf_t *p_rrc_mac_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_ue_entity_cnf, 0, sizeof(rrc_mac_delete_ue_entity_cnf_t));

    /* This function parses rrc_mac_delete_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_delete_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_delete_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_delete_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_DELETE_LC_STATUS:
                /* rrc_mac_delete_lc_status TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter >= (S32)ARRSIZE(p_rrc_mac_delete_ue_entity_cnf->delete_lc_status))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_delete_lc_status(
                    &p_rrc_mac_delete_ue_entity_cnf->delete_lc_status[p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_delete_lc_status
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_lc_status, 0, sizeof(rrc_mac_delete_lc_status_t));

    /* This function parses rrc_mac_delete_lc_status */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_status->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_status->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_status->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_status->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_status->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_lc_status->status < 1) || (p_rrc_mac_delete_lc_status->status > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_status->status] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_mac_delete_lc_status->status);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_reconfigure_ue_entity_req
(
    rrc_mac_reconfigure_ue_entity_req_t *p_rrc_mac_reconfigure_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfigure_ue_entity_req, 0, sizeof(rrc_mac_reconfigure_ue_entity_req_t));

    /* This function parses rrc_mac_reconfigure_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfigure_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_RECONFIG_UE_INFO:
                /* rrc_mac_reconfig_ue_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_reconfig_ue_info(
                    &p_rrc_mac_reconfigure_ue_entity_req->reconfig_ue_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present |= RRC_MAC_RECONFIG_UE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CREATE_LC_REQ:
                /* rrc_mac_create_lc_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->create_lc_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_create_lc_req(
                    &p_rrc_mac_reconfigure_ue_entity_req->create_lc_req[p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_RECONFIGURE_LC_REQ:
                /* rrc_mac_reconfigure_lc_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_reconfigure_lc_req(
                    &p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req[p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_DELETE_LC_REQ:
                /* rrc_mac_delete_lc_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_delete_lc_req(
                    &p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req[p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_RAT1_INFO:
                /* rrc_mac_uplink_rat1_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_uplink_rat1_info(
                    &p_rrc_mac_reconfigure_ue_entity_req->uplink_rat1_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present |= RRC_MAC_RECONFIG_RAT1_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_dl_max_rb_info
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_max_rb_info, 0, sizeof(rrc_mac_dl_max_rb_info_t));

    /* This function parses rrc_mac_dl_max_rb_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_dl_max_rb_info->dl_max_rb, p_src + *p_length_read, "dl_max_rb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_max_rb_info->dl_max_rb < 1) || (p_rrc_mac_dl_max_rb_info->dl_max_rb > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_max_rb_info->dl_max_rb] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_dl_max_rb_info->dl_max_rb);
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
rrc_il_parse_rrc_mac_time_alignment_timer_info
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_time_alignment_timer_info, 0, sizeof(rrc_mac_time_alignment_timer_info_t));

    /* This function parses rrc_mac_time_alignment_timer_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_time_alignment_timer_info->time_alignment_timer, p_src + *p_length_read, "time_alignment_timer");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_time_alignment_timer_info->time_alignment_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_time_alignment_timer_info->time_alignment_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_time_alignment_timer_info->time_alignment_timer);
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
rrc_il_parse_rrc_mac_p0_ue_pusch
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_p0_ue_pusch, 0, sizeof(rrc_mac_p0_ue_pusch_t));

    /* This function parses rrc_mac_p0_ue_pusch */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_p0_ue_pusch->p0_ue_pusch, p_src + *p_length_read, "p0_ue_pusch");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p0_ue_pusch->p0_ue_pusch < -8) || (p_rrc_mac_p0_ue_pusch->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p0_ue_pusch->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_p0_ue_pusch->p0_ue_pusch);
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
rrc_il_parse_rrc_mac_max_ul_harq_tx_info
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_max_ul_harq_tx_info, 0, sizeof(rrc_mac_max_ul_harq_tx_info_t));

    /* This function parses rrc_mac_max_ul_harq_tx_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx, p_src + *p_length_read, "max_ul_harq_tx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx > 13))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx] should be less than"
            " or equal to 13. Incorrect value %u received.", p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx);
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
rrc_il_parse_rrc_mac_pc_max_config_info
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pc_max_config_info, 0, sizeof(rrc_mac_pc_max_config_info_t));

    /* This function parses rrc_mac_pc_max_config_info */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_mac_pc_max_config_info->pc_max, p_src + *p_length_read, "pc_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pc_max_config_info->pc_max < -40) || (p_rrc_mac_pc_max_config_info->pc_max > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pc_max_config_info->pc_max] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_pc_max_config_info->pc_max);
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
rrc_il_parse_RrcMacUeCategoryV1310
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacUeCategoryV1310, 0, sizeof(RrcMacUeCategoryV1310_t));

    /* This function parses RrcMacUeCategoryV1310 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacUeCategoryV1310->ueCategoryV1310, p_src + *p_length_read, "ueCategoryV1310");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacUeCategoryV1310->ueCategoryV1310 > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacUeCategoryV1310->ueCategoryV1310] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcMacUeCategoryV1310->ueCategoryV1310);
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
rrc_il_parse_rrc_mac_reconfig_ue_info
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfig_ue_info, 0, sizeof(rrc_mac_reconfig_ue_info_t));

    /* This function parses rrc_mac_reconfig_ue_info */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CQI_INFO:
                /* rrc_mac_cqi_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_cqi_info(
                    &p_rrc_mac_reconfig_ue_info->cqi_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONF_CQI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_MAX_RB_INFO:
                /* rrc_mac_dl_max_rb_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_max_rb_info(
                    &p_rrc_mac_reconfig_ue_info->dl_max_rb_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_DL_MAX_RB_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TX_MODE_INFO:
                /* rrc_mac_tx_mode_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tx_mode_info(
                    &p_rrc_mac_reconfig_ue_info->tx_mode_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_TX_MODE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_NUM_OF_LAYER_INFO:
                /* rrc_mac_num_of_layer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_num_of_layer_info(
                    &p_rrc_mac_reconfig_ue_info->num_of_layer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_NUM_OF_LAYER_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODE_BOOK_INDEX_INFO:
                /* rrc_mac_code_book_index_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_code_book_index_info(
                    &p_rrc_mac_reconfig_ue_info->code_book_index_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_CODE_BOOK_INDEX_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SR_CONFIG_INFO:
                /* rrc_mac_sr_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sr_config_info(
                    &p_rrc_mac_reconfig_ue_info->sr_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONF_SR_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920:
                /* rrc_mac_sr_prohbit_timer_value_v920 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sr_prohbit_timer_value_v920(
                    &p_rrc_mac_reconfig_ue_info->sr_prohbit_timer_value_v920,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_SR_PROHBIT_TIMER_VALUE_V920_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CATEGORY_INFO:
                /* rrc_mac_ue_cat TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_cat(
                    &p_rrc_mac_reconfig_ue_info->ue_category_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_UE_CATEGORY_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SRS_DEDICATED_CONFIG:
                /* rrc_mac_srs_dedicated_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_srs_dedicated_config(
                    &p_rrc_mac_reconfig_ue_info->srs_dedicated_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_SRS_DEDICATED_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION:
                /* rrc_mac_codebook_subset_restriction TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction(
                    &p_rrc_mac_reconfig_ue_info->codebook_subset_restriction,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DRX_CONFIG_INFO:
                /* rrc_mac_drx_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_drx_config_info(
                    &p_rrc_mac_reconfig_ue_info->drx_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_DRX_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MEAS_GAP_CONFIG_INFO:
                /* rrc_mac_meas_gap_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_meas_gap_config_info(
                    &p_rrc_mac_reconfig_ue_info->meas_gap_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TIME_ALIGNMENT_TIME_INFO:
                /* rrc_mac_time_alignment_timer_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_time_alignment_timer_info(
                    &p_rrc_mac_reconfig_ue_info->time_alignment_timer_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_TIME_ALIGNMENT_TIME_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920:
                /* rrc_mac_codebook_subset_restriction_v920 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction_v920(
                    &p_rrc_mac_reconfig_ue_info->codebook_subset_restriction_v920,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_CONFIG_INFO:
                /* rrc_mac_pdsch_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_config_info(
                    &p_rrc_mac_reconfig_ue_info->pdsch_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_PDSCH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_P0_UE_PUSCH:
                /* rrc_mac_p0_ue_pusch TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_p0_ue_pusch(
                    &p_rrc_mac_reconfig_ue_info->p0_ue_pusch,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_P0_UE_PUSCH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TPC_PDCCH_CONFIG_PUCCH:
                /* rrc_mac_tpc_pdcch_config_pucch TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_pdcch_config_pucch(
                    &p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pucch,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUCCH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TPC_PDCCH_CONFIG_PUSCH:
                /* rrc_mac_tpc_pdcch_config_pusch TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tpc_pdcch_config_pusch(
                    &p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pusch,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUSCH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_AMBR_QOS_INFO:
                /* rrc_mac_ambr_qos_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ambr_qos_info(
                    &p_rrc_mac_reconfig_ue_info->ambr_qos_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_AMBR_QOS_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS:
                /* rrc_mac_ue_capability_additional_params TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_capability_additional_params(
                    &p_rrc_mac_reconfig_ue_info->ue_capability_additional_params,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SPS_CONFIG:
                /* rrc_mac_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_config(
                    &p_rrc_mac_reconfig_ue_info->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_RF_PARAMS:
                /* rrc_mac_rf_params TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_rf_params(
                    &p_rrc_mac_reconfig_ue_info->rf_params,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_RF_PARAMS_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PHR_TIMER_CONFIG:
                /* rrc_mac_phr_timer_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_phr_timer_config(
                    &p_rrc_mac_reconfig_ue_info->phr_timer_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_PHR_TIMER_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_COMPLIANCE_INFO:
                /* rrc_mac_ue_compliance_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_compliance_info(
                    &p_rrc_mac_reconfig_ue_info->ue_compliance_release,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_UE_COMPLIANCE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO:
                /* rrc_mac_pdsch_epre_to_ue_rs_ratio TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pdsch_epre_to_ue_rs_ratio(
                    &p_rrc_mac_reconfig_ue_info->pdsch_epre_to_ue_rs_ratio,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_TTI_BUNDLING_INFO:
                /* rrc_mac_tti_bundling_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_tti_bundling_info(
                    &p_rrc_mac_reconfig_ue_info->tti_bundling,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_TTI_BUNDLING_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MAX_UL_HARQ_TX_INFO:
                /* rrc_mac_max_ul_harq_tx_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_max_ul_harq_tx_info(
                    &p_rrc_mac_reconfig_ue_info->max_ul_harq_tx_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_MAX_UL_HARQ_TX_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PC_MAX_INFO:
                /* rrc_mac_pc_max_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pc_max_config_info(
                    &p_rrc_mac_reconfig_ue_info->pc_max_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_PC_MAX_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUSCH_CONFIG_INFO:
                /* rrc_mac_pusch_config_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pusch_config_info(
                    &p_rrc_mac_reconfig_ue_info->pusch_config_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_PUSCH_CONFIG_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020:
                /* rrc_mac_codebook_subset_restriction_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_codebook_subset_restriction_v1020(
                    &p_rrc_mac_reconfig_ue_info->rrc_mac_codebook_subset_restriction_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_INTERFERENCE_INFO:
                /* rrc_mac_dl_interference_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_interference_info(
                    &p_rrc_mac_reconfig_ue_info->rrc_mac_dl_interference_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_ADD_DL_INTERFERENCE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_OPERATOR_INFO:
                /* rrc_mac_operator_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_operator_info(
                    &p_rrc_mac_reconfig_ue_info->operator_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->optional_elems_present |= RRC_MAC_RECONFIG_OPERATOR_ID_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_CONFIG_DEDICATED_R13:
                /* PucchConfigDedicatedR13 TLV */
                if (RRC_FAILURE == rrc_il_parse_PucchConfigDedicatedR13(
                    &p_rrc_mac_reconfig_ue_info->pucchConfigDedicatedR13,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present |= RRC_MAC_RECONFIG_PUCCH_CONFIG_DEDICATED_R13_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_EPDCCH_SET_CONFIG_R11:
                /* EpdcchSetConfigR11 TLV */
                if (RRC_FAILURE == rrc_il_parse_EpdcchSetConfigR11(
                    &p_rrc_mac_reconfig_ue_info->epdcchSetConfigR11,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present |= RRC_MAC_RECONFIG_EPDCCH_SET_CONFIG_R11_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CE_LEVEL:
                /* RrcMacCeLevel TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacCeLevel(
                    &p_rrc_mac_reconfig_ue_info->ceLevel,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present |= RRC_MAC_RECONFIG_CE_LEVEL_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CE_MODE:
                /* RrcMacCeMode TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacCeMode(
                    &p_rrc_mac_reconfig_ue_info->ceMode,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present |= RRC_MAC_RECONFIG_CE_MODE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CATEGORY_V1310:
                /* RrcMacUeCategoryV1310 TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacUeCategoryV1310(
                    &p_rrc_mac_reconfig_ue_info->ueCategoryV1310,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present |= RRC_MAC_RECONFIG_UE_CATEGORY_V1310_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_UE_CATEGORY_V1020:
                /* rrc_mac_ue_category_v1020 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ue_category_v1020(
                    &p_rrc_mac_reconfig_ue_info->rrc_mac_ue_category_v1020,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->ca_optional_elems_present |= RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PUCCH_FORMAT_V1020:
                /* rrc_mac_pucch_format_r10 TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_pucch_format_r10(
                    &p_rrc_mac_reconfig_ue_info->rrc_mac_pucch_format_r10,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->ca_optional_elems_present |= RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CA_CONFIG:
                /* rrc_mac_carrier_aggr_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_carrier_aggr_config(
                    &p_rrc_mac_reconfig_ue_info->rrc_mac_carrier_aggr_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfig_ue_info->ca_optional_elems_present |= RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_ul_lc_reconfig_req
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ul_lc_reconfig_req, 0, sizeof(rrc_mac_ul_lc_reconfig_req_t));

    /* This function parses rrc_mac_ul_lc_reconfig_req */

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_mac_dl_lc_reconfig_req
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_dl_lc_reconfig_req, 0, sizeof(rrc_mac_dl_lc_reconfig_req_t));

    /* This function parses rrc_mac_dl_lc_reconfig_req */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO:
                /* rrc_mac_ca_lc_bandwidth_dist_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_lc_bandwidth_dist_info(
                    &p_rrc_mac_dl_lc_reconfig_req->ca_lc_bandwidth_dist_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_dl_lc_reconfig_req->optional_elems_present |= RRC_MAC_DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_reconfigure_lc_req
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfigure_lc_req, 0, sizeof(rrc_mac_reconfigure_lc_req_t));

    /* This function parses rrc_mac_reconfigure_lc_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_reconfigure_lc_req->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_req->lch_id);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_UL_LC_RECONFIGURE_REQ:
                /* rrc_mac_ul_lc_reconfig_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_lc_reconfig_req(
                    &p_rrc_mac_reconfigure_lc_req->ul_lc_reconfig_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_lc_req->optional_elems_present |= RRC_MAC_UL_LC_RECONFIG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_LC_RECONFIGURE_REQ:
                /* rrc_mac_dl_lc_reconfig_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_lc_reconfig_req(
                    &p_rrc_mac_reconfigure_lc_req->dl_lc_reconfig_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_lc_req->optional_elems_present |= RRC_MAC_DL_LC_RECONFIG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_GBR_QOS_INFO:
                /* rrc_mac_gbr_qos_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_gbr_qos_info(
                    &p_rrc_mac_reconfigure_lc_req->gbr_qos_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_lc_req->optional_elems_present |= RRC_MAC_GBR_QOS_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_delete_lc_req
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_lc_req, 0, sizeof(rrc_mac_delete_lc_req_t));

    /* This function parses rrc_mac_delete_lc_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_req->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_req->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_req->lc_type, p_src + *p_length_read, "lc_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_lc_req->lc_type < 1) || (p_rrc_mac_delete_lc_req->lc_type > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_req->lc_type] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_mac_delete_lc_req->lc_type);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_reconfigure_ue_entity_cnf
(
    rrc_mac_reconfigure_ue_entity_cnf_t *p_rrc_mac_reconfigure_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfigure_ue_entity_cnf, 0, sizeof(rrc_mac_reconfigure_ue_entity_cnf_t));

    /* This function parses rrc_mac_reconfigure_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfigure_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfigure_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_reconfigure_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CREATE_LC_ERROR:
                /* rrc_mac_create_lc_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_create_lc_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error[p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_RECONFIGURE_LC_ERROR:
                /* rrc_mac_reconfigure_lc_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_reconfigure_lc_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error[p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_DELETE_LC_ERROR:
                /* rrc_mac_delete_lc_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_delete_lc_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error[p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_DELETE_LC_STATUS:
                /* rrc_mac_delete_lc_status TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_delete_lc_status(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status[p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SPS_CRNTI_INFO:
                /* rrc_mac_sps_crnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sps_crnti_info(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->sps_crnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->optional_elems_present |= RRC_MAC_RECONF_SPS_CRNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_CREATE_CA_CONFIG_ERROR:
                /* rrc_mac_ca_config_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_config_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err[p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_RECONFIGURE_CA_CONFIG_ERROR:
                /* rrc_mac_ca_reconfig_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ca_reconfig_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err[p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_DELETE_CA_CONFIG_ERROR:
                /* rrc_mac_delete_ca_config_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter >= (S32)ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_delete_ca_config_error(
                    &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err[p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_reconfigure_lc_error
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfigure_lc_error, 0, sizeof(rrc_mac_reconfigure_lc_error_t));

    /* This function parses rrc_mac_reconfigure_lc_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_reconfigure_lc_error->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_error->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfigure_lc_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_error->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_UL_LC_CONFIG_RESP:
                /* rrc_mac_ul_lc_config_resp TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_ul_lc_config_resp(
                    &p_rrc_mac_reconfigure_lc_error->ul_lc_config_resp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_lc_error->optional_elems_present |= RRC_MAC_RECONF_UL_LC_CONFIG_RESP_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DL_LC_CONFIG_RESP:
                /* rrc_mac_dl_lc_config_resp TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_dl_lc_config_resp(
                    &p_rrc_mac_reconfigure_lc_error->dl_lc_config_resp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reconfigure_lc_error->optional_elems_present |= RRC_MAC_RECONF_DL_LC_CONFIG_RESP_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_delete_lc_error
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_lc_error, 0, sizeof(rrc_mac_delete_lc_error_t));

    /* This function parses rrc_mac_delete_lc_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_error->lch_id, p_src + *p_length_read, "lch_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_error->lch_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_delete_lc_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_delete_lc_error->response);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_lc_error->lc_type, p_src + *p_length_read, "lc_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->lc_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->lc_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_delete_lc_error->lc_type);
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
rrc_il_parse_rrc_mac_ca_reconfig_error
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ca_reconfig_error, 0, sizeof(rrc_mac_ca_reconfig_error_t));

    /* This function parses rrc_mac_ca_reconfig_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ca_reconfig_error->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ca_reconfig_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_reconfig_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_ca_reconfig_error->cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ca_reconfig_error->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ca_reconfig_error->error_code < 6) || (p_rrc_mac_ca_reconfig_error->error_code > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_reconfig_error->error_code] should be in range "
            "6 to 6. Incorrect value %u received.", p_rrc_mac_ca_reconfig_error->error_code);
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
rrc_il_parse_rrc_mac_delete_ca_config_error
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_delete_ca_config_error, 0, sizeof(rrc_mac_delete_ca_config_error_t));

    /* This function parses rrc_mac_delete_ca_config_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_delete_ca_config_error->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_ca_config_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ca_config_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_delete_ca_config_error->cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_delete_ca_config_error->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_ca_config_error->error_code < 6) || (p_rrc_mac_delete_ca_config_error->error_code > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ca_config_error->error_code] should be in range "
            "6 to 6. Incorrect value %u received.", p_rrc_mac_delete_ca_config_error->error_code);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ue_entity_power_headroom_ind
(
    rrc_mac_ue_entity_power_headroom_ind_t *p_rrc_mac_ue_entity_power_headroom_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_entity_power_headroom_ind, 0, sizeof(rrc_mac_ue_entity_power_headroom_ind_t));

    /* This function parses rrc_mac_ue_entity_power_headroom_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_entity_power_headroom_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_entity_power_headroom_ind->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_entity_power_headroom_ind->crnti < 1) || (p_rrc_mac_ue_entity_power_headroom_ind->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_entity_power_headroom_ind->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_entity_power_headroom_ind->crnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_entity_power_headroom_ind->power_headroom, p_src + *p_length_read, "power_headroom");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_entity_power_headroom_ind->power_headroom > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_entity_power_headroom_ind->power_headroom] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_ue_entity_power_headroom_ind->power_headroom);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ue_drx_cmd_req
(
    rrc_mac_ue_drx_cmd_req_t *p_rrc_mac_ue_drx_cmd_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_drx_cmd_req, 0, sizeof(rrc_mac_ue_drx_cmd_req_t));

    /* This function parses rrc_mac_ue_drx_cmd_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_drx_cmd_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ue_con_rej_req
(
    rrc_mac_ue_con_rej_req_t *p_rrc_mac_ue_con_rej_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_con_rej_req, 0, sizeof(rrc_mac_ue_con_rej_req_t));

    /* This function parses rrc_mac_ue_con_rej_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_con_rej_req->tcrnti, p_src + *p_length_read, "tcrnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_con_rej_req->tcrnti < 1) || (p_rrc_mac_ue_con_rej_req->tcrnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_con_rej_req->tcrnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_con_rej_req->tcrnti);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_bcch_config_req
(
    rrc_mac_bcch_config_req_t *p_rrc_mac_bcch_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_bcch_config_req, 0, sizeof(rrc_mac_bcch_config_req_t));

    /* This function parses rrc_mac_bcch_config_req */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_MIB_MSG_REQ:
                /* rrc_mac_mib_msg_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_bcch_config_req->mib_msg_req_counter >= (S32)ARRSIZE(p_rrc_mac_bcch_config_req->mib_msg_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_mac_mib_msg_req(
                    &p_rrc_mac_bcch_config_req->mib_msg_req[p_rrc_mac_bcch_config_req->mib_msg_req_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_bcch_config_req->mib_msg_req_counter++;

                *p_length_read += length_read;

                break;
            case RRC_MAC_SIBTYPE1_MSG_REQ:
                /* rrc_mac_sibtype1_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sibtype1_msg_req(
                    &p_rrc_mac_bcch_config_req->sib1_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_bcch_config_req->optional_elems_present |= RRC_MAC_BCCH_SIBTYPE1_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_SI_MSG_INFO:
                /* rrc_mac_si_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_si_msg_req(
                    &p_rrc_mac_bcch_config_req->si_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_bcch_config_req->optional_elems_present |= RRC_MAC_BCCH_SI_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_pcch_msg_req
(
    rrc_mac_pcch_msg_req_t *p_rrc_mac_pcch_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_pcch_msg_req, 0, sizeof(rrc_mac_pcch_msg_req_t));

    /* This function parses rrc_mac_pcch_msg_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_pcch_msg_req->paging_frame, p_src + *p_length_read, "paging_frame");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->paging_frame > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->paging_frame] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->paging_frame);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pcch_msg_req->paging_sub_frame, p_src + *p_length_read, "paging_sub_frame");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->paging_sub_frame > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->paging_sub_frame] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->paging_sub_frame);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_pcch_msg_req->num_of_sfn, p_src + *p_length_read, "num_of_sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pcch_msg_req->num_of_sfn < 1) || (p_rrc_mac_pcch_msg_req->num_of_sfn > 32768))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->num_of_sfn] should be in range "
            "1 to 32768. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->num_of_sfn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_pcch_msg_req->isPagingForEmtc, p_src + *p_length_read, "isPagingForEmtc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->isPagingForEmtc > EMTC_PAGING))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->isPagingForEmtc] should be less than"
            " or equal to EMTC_PAGING. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->isPagingForEmtc);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_PAGING_NARROWBAND:
                /* RrcMacPagingNarrowband TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPagingNarrowband(
                    &p_rrc_mac_pcch_msg_req->pagingNarrowBand,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pcch_msg_req->bitmask |= RRC_MAC_PAGING_NARROWBAND_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_MPDCCH_NUM_REPETITION_PAGING:
                /* RrcMacMpdcchNumRepetitionPaging TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacMpdcchNumRepetitionPaging(
                    &p_rrc_mac_pcch_msg_req->mpdcchNumRepetitionPaging,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pcch_msg_req->bitmask |= RRC_MAC_MPDCCH_NUM_REPETITION_PAGING_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PDSCH_NUM_REPETITION_PAGING:
                /* RrcMacPdschNumRepetition TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacPdschNumRepetition(
                    &p_rrc_mac_pcch_msg_req->pdschNumRepetition,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pcch_msg_req->bitmask |= RRC_MAC_PDSCH_NUM_REPETITION_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_DIRECT_INDICATION:
                /* RrcMacDirectIndication TLV */
                if (RRC_FAILURE == rrc_il_parse_RrcMacDirectIndication(
                    &p_rrc_mac_pcch_msg_req->directIndication,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_pcch_msg_req->bitmask |= RRC_MAC_DIRECT_INDICATION_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_RrcMacPagingNarrowband
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacPagingNarrowband, 0, sizeof(RrcMacPagingNarrowband_t));

    /* This function parses RrcMacPagingNarrowband */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPagingNarrowband->pagingNarrowBand, p_src + *p_length_read, "pagingNarrowBand");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPagingNarrowband->pagingNarrowBand < 1) || (p_RrcMacPagingNarrowband->pagingNarrowBand > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPagingNarrowband->pagingNarrowBand] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacPagingNarrowband->pagingNarrowBand);
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
rrc_il_parse_RrcMacMpdcchNumRepetitionPaging
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacMpdcchNumRepetitionPaging, 0, sizeof(RrcMacMpdcchNumRepetitionPaging_t));

    /* This function parses RrcMacMpdcchNumRepetitionPaging */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging, p_src + *p_length_read, "mpdcchNumRepetitionPaging");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging < 1) || (p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging > EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging] should be in range "
            "1 to EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX. Incorrect value %u received.", p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging);
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
rrc_il_parse_RrcMacPdschNumRepetition
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacPdschNumRepetition, 0, sizeof(RrcMacPdschNumRepetition_t));

    /* This function parses RrcMacPdschNumRepetition */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacPdschNumRepetition->pdschNumRepetition, p_src + *p_length_read, "pdschNumRepetition");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPdschNumRepetition->pdschNumRepetition < 1) || (p_RrcMacPdschNumRepetition->pdschNumRepetition > EMTC_PDSCH_PAGING_REP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPdschNumRepetition->pdschNumRepetition] should be in range "
            "1 to EMTC_PDSCH_PAGING_REP_MAX. Incorrect value %u received.", p_RrcMacPdschNumRepetition->pdschNumRepetition);
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
rrc_il_parse_RrcMacDirectIndication
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacDirectIndication, 0, sizeof(RrcMacDirectIndication_t));

    /* This function parses RrcMacDirectIndication */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacDirectIndication->directIndication, p_src + *p_length_read, "directIndication");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacDirectIndication->directIndication > EMTC_DIRECT_INDICATION_SI_MODIF))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacDirectIndication->directIndication] should be less than"
            " or equal to EMTC_DIRECT_INDICATION_SI_MODIF. Incorrect value %u received.", p_RrcMacDirectIndication->directIndication);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ccch_msg_req
(
    rrc_mac_ccch_msg_req_t *p_rrc_mac_ccch_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ccch_msg_req, 0, sizeof(rrc_mac_ccch_msg_req_t));

    /* This function parses rrc_mac_ccch_msg_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ccch_msg_req->rnti, p_src + *p_length_read, "rnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ccch_msg_req->rnti < 1) || (p_rrc_mac_ccch_msg_req->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_req->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ccch_msg_req->rnti);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ccch_msg_ind
(
    rrc_mac_ccch_msg_ind_t *p_rrc_mac_ccch_msg_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ccch_msg_ind, 0, sizeof(rrc_mac_ccch_msg_ind_t));

    /* This function parses rrc_mac_ccch_msg_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ccch_msg_ind->rnti, p_src + *p_length_read, "rnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ccch_msg_ind->rnti < 1) || (p_rrc_mac_ccch_msg_ind->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_ind->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ccch_msg_ind->rnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ccch_msg_ind->timing_advance, p_src + *p_length_read, "timing_advance");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ccch_msg_ind->timing_advance > 1282))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_ind->timing_advance] should be less than"
            " or equal to 1282. Incorrect value %u received.", p_rrc_mac_ccch_msg_ind->timing_advance);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ccch_msg_ind->sinr, p_src + *p_length_read, "sinr");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_RrcMacEmtcCcchMsgInd
(
    RrcMacEmtcCcchMsgInd_t *p_RrcMacEmtcCcchMsgInd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMacEmtcCcchMsgInd, 0, sizeof(RrcMacEmtcCcchMsgInd_t));

    /* This function parses RrcMacEmtcCcchMsgInd */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacEmtcCcchMsgInd->rnti, p_src + *p_length_read, "rnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacEmtcCcchMsgInd->rnti < 1) || (p_RrcMacEmtcCcchMsgInd->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->rnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_RrcMacEmtcCcchMsgInd->timing_advance, p_src + *p_length_read, "timing_advance");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcCcchMsgInd->timing_advance > 1282))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->timing_advance] should be less than"
            " or equal to 1282. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->timing_advance);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacEmtcCcchMsgInd->sinr, p_src + *p_length_read, "sinr");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMacEmtcCcchMsgInd->ceLevel, p_src + *p_length_read, "ceLevel");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcCcchMsgInd->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->ceLevel);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ho_rach_resource_req
(
    rrc_mac_ho_rach_resource_req_t *p_rrc_mac_ho_rach_resource_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ho_rach_resource_req, 0, sizeof(rrc_mac_ho_rach_resource_req_t));

    /* This function parses rrc_mac_ho_rach_resource_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ho_rach_resource_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_INTRA_ENB_HO_REQ:
                /* rrc_mac_intra_enb_ho_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_intra_enb_ho_req(
                    &p_rrc_mac_ho_rach_resource_req->intra_enb_ho_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_ho_rach_resource_req->optional_elems_present |= RRC_MAC_INTRA_ENB_HO_REQ_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_intra_enb_ho_req
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_intra_enb_ho_req, 0, sizeof(rrc_mac_intra_enb_ho_req_t));

    /* This function parses rrc_mac_intra_enb_ho_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho, p_src + *p_length_read, "enable_intra_enb_ho");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ho_rach_resource_resp
(
    rrc_mac_ho_rach_resource_resp_t *p_rrc_mac_ho_rach_resource_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ho_rach_resource_resp, 0, sizeof(rrc_mac_ho_rach_resource_resp_t));

    /* This function parses rrc_mac_ho_rach_resource_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ho_rach_resource_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ho_rach_resource_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ho_rach_resource_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ho_rach_resource_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ho_rach_resource_resp->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CRNTI_INFO:
                /* rrc_mac_crnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_crnti_info(
                    &p_rrc_mac_ho_rach_resource_resp->crnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_ho_rach_resource_resp->optional_elems_present |= RRC_MAC_CRNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_MAC_PREAMBLE_INFO:
                /* rrc_mac_preamble_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_preamble_info(
                    &p_rrc_mac_ho_rach_resource_resp->preamble_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_ho_rach_resource_resp->optional_elems_present |= RRC_MAC_PREAMBLE_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_crnti_info
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_crnti_info, 0, sizeof(rrc_mac_crnti_info_t));

    /* This function parses rrc_mac_crnti_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_crnti_info->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_crnti_info->crnti < 1) || (p_rrc_mac_crnti_info->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_crnti_info->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_crnti_info->crnti);
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
rrc_il_parse_rrc_mac_preamble_info
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_preamble_info, 0, sizeof(rrc_mac_preamble_info_t));

    /* This function parses rrc_mac_preamble_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_preamble_info->preamble_Idx, p_src + *p_length_read, "preamble_Idx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_preamble_info->preamble_Idx > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_info->preamble_Idx] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_preamble_info->preamble_Idx);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_preamble_info->prach_mask_Idx, p_src + *p_length_read, "prach_mask_Idx");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_preamble_info->prach_mask_Idx > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_info->prach_mask_Idx] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_preamble_info->prach_mask_Idx);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ho_rel_rach_resource_ind
(
    rrc_mac_ho_rel_rach_resource_ind_t *p_rrc_mac_ho_rel_rach_resource_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ho_rel_rach_resource_ind, 0, sizeof(rrc_mac_ho_rel_rach_resource_ind_t));

    /* This function parses rrc_mac_ho_rel_rach_resource_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ho_rel_rach_resource_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_CRNTI_INFO:
                /* rrc_mac_crnti_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_crnti_info(
                    &p_rrc_mac_ho_rel_rach_resource_ind->crnti_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_ho_rel_rach_resource_ind->optional_elems_present |= RRC_MAC_CRNTI_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_reset_ue_entity_req
(
    rrc_mac_reset_ue_entity_req_t *p_rrc_mac_reset_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reset_ue_entity_req, 0, sizeof(rrc_mac_reset_ue_entity_req_t));

    /* This function parses rrc_mac_reset_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reset_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG:
                /* rrc_mac_scell_rel_flag TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_scell_rel_flag(
                    &p_rrc_mac_reset_ue_entity_req->rrc_mac_scell_rel_flag,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_reset_ue_entity_req->optional_elems_present |= RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_scell_rel_flag
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_scell_rel_flag, 0, sizeof(rrc_mac_scell_rel_flag_t));

    /* This function parses rrc_mac_scell_rel_flag */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_scell_rel_flag->scell_rel_flag, p_src + *p_length_read, "scell_rel_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_rel_flag->scell_rel_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_rel_flag->scell_rel_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_scell_rel_flag->scell_rel_flag);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_reset_ue_entity_cnf
(
    rrc_mac_reset_ue_entity_cnf_t *p_rrc_mac_reset_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reset_ue_entity_cnf, 0, sizeof(rrc_mac_reset_ue_entity_cnf_t));

    /* This function parses rrc_mac_reset_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reset_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reset_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_radio_link_failure_ind
(
    rrc_mac_radio_link_failure_ind_t *p_rrc_mac_radio_link_failure_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_radio_link_failure_ind, 0, sizeof(rrc_mac_radio_link_failure_ind_t));

    /* This function parses rrc_mac_radio_link_failure_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_radio_link_failure_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_radio_link_failure_ind->rlf_cause, p_src + *p_length_read, "rlf_cause");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_ue_sync_status_ind
(
    rrc_mac_ue_sync_status_ind_t *p_rrc_mac_ue_sync_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_ue_sync_status_ind, 0, sizeof(rrc_mac_ue_sync_status_ind_t));

    /* This function parses rrc_mac_ue_sync_status_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_sync_status_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_ue_sync_status_ind->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_sync_status_ind->crnti < 1) || (p_rrc_mac_ue_sync_status_ind->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_sync_status_ind->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_sync_status_ind->crnti);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_ue_sync_status_ind->ue_sync_status, p_src + *p_length_read, "ue_sync_status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_sync_status_ind->ue_sync_status > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_sync_status_ind->ue_sync_status] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_ue_sync_status_ind->ue_sync_status);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_change_crnti_req
(
    rrc_mac_change_crnti_req_t *p_rrc_mac_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_change_crnti_req, 0, sizeof(rrc_mac_change_crnti_req_t));

    /* This function parses rrc_mac_change_crnti_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_change_crnti_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_change_crnti_req->old_crnti, p_src + *p_length_read, "old_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_change_crnti_req->new_crnti, p_src + *p_length_read, "new_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_change_crnti_cnf
(
    rrc_mac_change_crnti_cnf_t *p_rrc_mac_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_change_crnti_cnf, 0, sizeof(rrc_mac_change_crnti_cnf_t));

    /* This function parses rrc_mac_change_crnti_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_change_crnti_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_change_crnti_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_change_crnti_cnf->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_mac_cell_start_cnf
(
    rrc_mac_cell_start_cnf_t *p_rrc_mac_cell_start_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_cell_start_cnf, 0, sizeof(rrc_mac_cell_start_cnf_t));

    /* This function parses rrc_mac_cell_start_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_cell_start_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cell_start_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cell_start_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cell_start_cnf->response);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_MAC_SFN_INFO:
                /* rrc_mac_sfn TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_sfn(
                    &p_rrc_mac_cell_start_cnf->sfn,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_cell_start_cnf->optional_elems_present |= RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_mac_reconfig_complete_ind
(
    rrc_mac_reconfig_complete_ind_t *p_rrc_mac_reconfig_complete_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_reconfig_complete_ind, 0, sizeof(rrc_mac_reconfig_complete_ind_t));

    /* This function parses rrc_mac_reconfig_complete_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_reconfig_complete_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_reconfig_complete_ind->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_complete_ind->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_complete_ind->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfig_complete_ind->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

