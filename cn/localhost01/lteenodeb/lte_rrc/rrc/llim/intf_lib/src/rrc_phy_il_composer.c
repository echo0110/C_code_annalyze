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
 *  File Description : The file rrc_phy_il_composer.c contains the definitions 
 *                     of rrc-phy interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_phy_il_composer.h"
#include "rrc_phy_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_sounding_rs_ul_config_common_setup_len
(
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup
);

static
rrc_return_et
rrc_il_compose_sounding_rs_ul_config_common_setup
(
    U8  **pp_buffer,
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup
);

static
rrc_length_t
rrc_il_get_sounding_rs_ul_config_common_len
(
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common
);

static
rrc_return_et
rrc_il_compose_sounding_rs_ul_config_common
(
    U8  **pp_buffer,
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common
);

static
rrc_length_t
rrc_il_get_rrc_phy_tdd_config_len
(
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config
);

static
rrc_return_et
rrc_il_compose_rrc_phy_tdd_config
(
    U8  **pp_buffer,
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config
);

static
rrc_length_t
rrc_il_get_rrc_phy_prs_config_len
(
    rrc_phy_prs_config_t *p_rrc_phy_prs_config
);

static
rrc_return_et
rrc_il_compose_rrc_phy_prs_config
(
    U8  **pp_buffer,
    rrc_phy_prs_config_t *p_rrc_phy_prs_config
);

static
rrc_length_t
rrc_il_get_rrc_phys_dl_earfcn_len
(
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn
);

static
rrc_return_et
rrc_il_compose_rrc_phys_dl_earfcn
(
    U8  **pp_buffer,
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len
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
rrc_return_et
rrc_il_compose_rrc_dmtc_configuration_r13
(
    U8  **pp_buffer,
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13
);

static
rrc_length_t
rrc_il_get_rrc_config_phy_cell_parameters_len
(
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters
);

static
rrc_return_et
rrc_il_compose_rrc_config_phy_cell_parameters
(
    U8  **pp_buffer,
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters
);

static
rrc_length_t
rrc_il_get_rrc_phy_sync_signals_len
(
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals
);

static
rrc_return_et
rrc_il_compose_rrc_phy_sync_signals
(
    U8  **pp_buffer,
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals
);

static
rrc_length_t
rrc_il_get_rrc_phy_prach_configuration_len
(
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration
);

static
rrc_return_et
rrc_il_compose_rrc_phy_prach_configuration
(
    U8  **pp_buffer,
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration
);

static
rrc_length_t
rrc_il_get_ul_ref_signals_pusch_len
(
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch
);

static
rrc_return_et
rrc_il_compose_ul_ref_signals_pusch
(
    U8  **pp_buffer,
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch
);

static
rrc_length_t
rrc_il_get_rrc_phy_pusch_configuration_len
(
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration
);

static
rrc_return_et
rrc_il_compose_rrc_phy_pusch_configuration
(
    U8  **pp_buffer,
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration
);

static
rrc_length_t
rrc_il_get_rrc_phy_pucch_configuration_len
(
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration
);

static
rrc_return_et
rrc_il_compose_rrc_phy_pucch_configuration
(
    U8  **pp_buffer,
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration
);

static
rrc_length_t
rrc_il_get_rrc_phy_phich_configuration_len
(
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration
);

static
rrc_return_et
rrc_il_compose_rrc_phy_phich_configuration
(
    U8  **pp_buffer,
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration
);

static
rrc_length_t
rrc_il_get_pdsch_config_common_len
(
    pdsch_config_common_t *p_pdsch_config_common
);

static
rrc_return_et
rrc_il_compose_pdsch_config_common
(
    U8  **pp_buffer,
    pdsch_config_common_t *p_pdsch_config_common
);

static
rrc_length_t
rrc_il_get_RrcPhyPrachParametersCeR13_len
(
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13
);

static
rrc_return_et
rrc_il_compose_RrcPhyPrachParametersCeR13
(
    U8  **pp_buffer,
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13
);

static
rrc_length_t
rrc_il_get_RrcPhyPrachParametersListCeR13_len
(
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13
);

static
rrc_return_et
rrc_il_compose_RrcPhyPrachParametersListCeR13
(
    U8  **pp_buffer,
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13
);

static
rrc_length_t
rrc_il_get_RrcPhyPrachConfigV1310_len
(
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310
);

static
rrc_return_et
rrc_il_compose_RrcPhyPrachConfigV1310
(
    U8  **pp_buffer,
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310
);

static
rrc_length_t
rrc_il_get_RrcFrequancyHoppingParametersR13_len
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
rrc_il_get_rrc_phy_reconfig_cell_parameters_len
(
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters
);

static
rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_parameters
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters
);

static
rrc_length_t
rrc_il_get_rrc_phy_pdsch_configuration_dedicated_len
(
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_pdsch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_ack_nack_repetition_param_len
(
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_ack_nack_repetition_param
(
    U8  **pp_buffer,
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_pucch_configuration_dedicated_len
(
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_pucch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_pusch_configuration_dedicated_len
(
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_pusch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_uplink_power_control_dedicated_len
(
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_uplink_power_control_dedicated
(
    U8  **pp_buffer,
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_index_len
(
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index
);

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_index
(
    U8  **pp_buffer,
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index
);

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_pdcch_config_param_len
(
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_pdcch_config_param
(
    U8  **pp_buffer,
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_pdcch_configuration_len
(
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration
);

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_pdcch_configuration
(
    U8  **pp_buffer,
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration
);

static
rrc_length_t
rrc_il_get_rrc_phy_subband_cqi_param_param_len
(
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_subband_cqi_param_param
(
    U8  **pp_buffer,
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_indicator_periodic_len
(
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_indicator_periodic
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_periodic_param_len
(
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting_periodic_param
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_periodic_len
(
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting_periodic
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_len
(
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting
);

static
rrc_length_t
rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_param_len
(
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated_param
(
    U8  **pp_buffer,
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_len
(
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated
(
    U8  **pp_buffer,
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_codebook_subset_restriction_len
(
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction
);

static
rrc_return_et
rrc_il_compose_rrc_phy_codebook_subset_restriction
(
    U8  **pp_buffer,
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction
);

static
rrc_length_t
rrc_il_get_rrc_phy_ue_transmit_antenna_selection_len
(
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection
);

static
rrc_return_et
rrc_il_compose_rrc_phy_ue_transmit_antenna_selection
(
    U8  **pp_buffer,
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection
);

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_information_dedicated_len
(
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_information_dedicated
(
    U8  **pp_buffer,
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_information_len
(
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information
);

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_information
(
    U8  **pp_buffer,
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information
);

static
rrc_length_t
rrc_il_get_rrc_phy_scheduling_request_config_param_len
(
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param
);

static
rrc_return_et
rrc_il_compose_rrc_phy_scheduling_request_config_param
(
    U8  **pp_buffer,
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param
);

static
rrc_length_t
rrc_il_get_rrc_phy_scheduling_request_config_len
(
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config
);

static
rrc_return_et
rrc_il_compose_rrc_phy_scheduling_request_config
(
    U8  **pp_buffer,
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_config_v920_len
(
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_config_v920
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920
);

static
rrc_length_t
rrc_il_get_rrc_phy_codebook_subset_restriction_v920_len
(
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920
);

static
rrc_return_et
rrc_il_compose_rrc_phy_codebook_subset_restriction_v920
(
    U8  **pp_buffer,
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920
);

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_info_v920_len
(
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920
);

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_info_v920
(
    U8  **pp_buffer,
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920
);

static
rrc_length_t
rrc_il_get_rrc_codebook_subset_restriction_r10_len
(
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10
);

static
rrc_return_et
rrc_il_compose_rrc_codebook_subset_restriction_r10
(
    U8  **pp_buffer,
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10
);

static
rrc_length_t
rrc_il_get_rrc_antenna_info_dedicated_r10_len
(
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10
);

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_dedicated_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10
);

static
rrc_length_t
rrc_il_get_rrc_antenna_info_r10_len
(
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10
);

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10
);

static
rrc_length_t
rrc_il_get_rrc_antenna_info_ul_r10_len
(
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10
);

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_ul_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_len
(
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_len
(
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_len
(
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_wideband_r10_len
(
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_wideband_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_subband_r10_len
(
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_subband_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_periodic_r10_len
(
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_periodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_len
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_len
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_len
(
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_len
(
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_fdd_r10_len
(
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_fdd_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_tdd_r10_len
(
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_tdd_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_r10_len
(
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_len
(
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
);

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_csi_subframePattern_r10_len
(
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10
);

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_csi_subframePattern_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10
);

static
rrc_length_t
rrc_il_get_rrc_cqi_report_config_r10_len
(
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10
);

static
rrc_return_et
rrc_il_compose_rrc_cqi_report_config_r10
(
    U8  **pp_buffer,
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10
);

static
rrc_length_t
rrc_il_get_rrc_additional_spectrum_emission_ca_r10_len
(
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10
);

static
rrc_return_et
rrc_il_compose_rrc_additional_spectrum_emission_ca_r10
(
    U8  **pp_buffer,
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10
);

static
rrc_length_t
rrc_il_get_n1pucch_an_cs_r10_len
(
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10
);

static
rrc_return_et
rrc_il_compose_n1pucch_an_cs_r10
(
    U8  **pp_buffer,
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10
);

static
rrc_length_t
rrc_il_get_n1pucch_an_cs_list_r10_len
(
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10
);

static
rrc_return_et
rrc_il_compose_n1pucch_an_cs_list_r10
(
    U8  **pp_buffer,
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10
);

static
rrc_length_t
rrc_il_get_rrc_channel_selection_n1_pucch_an_cs_r10_len
(
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10
);

static
rrc_return_et
rrc_il_compose_rrc_channel_selection_n1_pucch_an_cs_r10
(
    U8  **pp_buffer,
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10
);

static
rrc_length_t
rrc_il_get_rrc_pucch_channel_selection_config_v1020_len
(
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020
);

static
rrc_return_et
rrc_il_compose_rrc_pucch_channel_selection_config_v1020
(
    U8  **pp_buffer,
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020
);

static
rrc_length_t
rrc_il_get_rrc_phy_physical_config_dedicated_extended_len
(
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended
);

static
rrc_return_et
rrc_il_compose_rrc_phy_physical_config_dedicated_extended
(
    U8  **pp_buffer,
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended
);

static
rrc_length_t
rrc_il_get_rrc_phy_physical_config_dedicated_len
(
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated
);

static
rrc_return_et
rrc_il_compose_rrc_phy_physical_config_dedicated
(
    U8  **pp_buffer,
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_scell_non_ul_config_len
(
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config
);

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_scell_non_ul_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config
);

static
rrc_length_t
rrc_il_get_rrc_cqi_report_config_scell_r10_len
(
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10
);

static
rrc_return_et
rrc_il_compose_rrc_cqi_report_config_scell_r10
(
    U8  **pp_buffer,
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_scell_ul_config_len
(
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config
);

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_scell_ul_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config
);

static
rrc_length_t
rrc_il_get_rrc_physical_config_dedicated_scell_len
(
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell
);

static
rrc_return_et
rrc_il_compose_rrc_physical_config_dedicated_scell
(
    U8  **pp_buffer,
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_config_dedicated_scell_len
(
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell
);

static
rrc_return_et
rrc_il_compose_rrc_radio_res_config_dedicated_scell
(
    U8  **pp_buffer,
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell
);

static
rrc_length_t
rrc_il_get_rrc_phy_scell_config_len
(
    rrc_phy_scell_config_t *p_rrc_phy_scell_config
);

static
rrc_return_et
rrc_il_compose_rrc_phy_scell_config
(
    U8  **pp_buffer,
    rrc_phy_scell_config_t *p_rrc_phy_scell_config
);

static
rrc_length_t
rrc_il_get_rrc_phy_scell_config_list_len
(
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list
);

static
rrc_return_et
rrc_il_compose_rrc_phy_scell_config_list
(
    U8  **pp_buffer,
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list
);

static
rrc_length_t
rrc_il_get_RrcMpdcchConfigSetupR13_len
(
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13
);

static
rrc_return_et
rrc_il_compose_RrcMpdcchConfigSetupR13
(
    U8  **pp_buffer,
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13
);

static
rrc_length_t
rrc_il_get_RrcMpdcchConfigR13_len
(
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13
);

static
rrc_return_et
rrc_il_compose_RrcMpdcchConfigR13
(
    U8  **pp_buffer,
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13
);

static
rrc_length_t
rrc_il_get_RrcEpdcchSetConfigR11_len
(
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11
);

static
rrc_return_et
rrc_il_compose_RrcEpdcchSetConfigR11
(
    U8  **pp_buffer,
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11
);

static
rrc_length_t
rrc_il_get_EpdcchConfigSetup_len
(
    EpdcchConfigSetup_t *p_EpdcchConfigSetup
);

static
rrc_return_et
rrc_il_compose_EpdcchConfigSetup
(
    U8  **pp_buffer,
    EpdcchConfigSetup_t *p_EpdcchConfigSetup
);

static
rrc_length_t
rrc_il_get_EpdcchConfig_len
(
    EpdcchConfig_t *p_EpdcchConfig
);

static
rrc_return_et
rrc_il_compose_EpdcchConfig
(
    U8  **pp_buffer,
    EpdcchConfig_t *p_EpdcchConfig
);

static
rrc_length_t
rrc_il_get_RrcAckNackRepetitionSetupR13_len
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
rrc_il_get_RrcPucchConfigDedicatedR13_len
(
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13
);

static
rrc_return_et
rrc_il_compose_RrcPucchConfigDedicatedR13
(
    U8  **pp_buffer,
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13
);

static
rrc_length_t
rrc_il_get_RrcPhysicalConfigDedicatedR13_len
(
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13
);

static
rrc_return_et
rrc_il_compose_RrcPhysicalConfigDedicatedR13
(
    U8  **pp_buffer,
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrc_phy_config_cell_req_len
(
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_config_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_config_cell_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_config_cell_req->cell_index);

    /* Get length of IE */
    length += rrc_il_get_rrc_config_phy_cell_parameters_len(&p_rrc_phy_config_cell_req->cell_parameters);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_sync_signals_len(&p_rrc_phy_config_cell_req->sync_signals);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_prach_configuration_len(&p_rrc_phy_config_cell_req->prach_configuration);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pusch_configuration_len(&p_rrc_phy_config_cell_req->pusch_configuration);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pucch_configuration_len(&p_rrc_phy_config_cell_req->pucch_configuration);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_phich_configuration_len(&p_rrc_phy_config_cell_req->phich_configuration);

    /* Get length of IE */
    length += rrc_il_get_pdsch_config_common_len(&p_rrc_phy_config_cell_req->pdsch_configuration);

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_config_cell_req->explicit_start_required);
    }

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcPhyPrachConfigV1310_len(&p_rrc_phy_config_cell_req->prachConfigurationV1310);
    }

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_FREQUENCY_HOPPING_PARAMS_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcFrequancyHoppingParametersR13_len(&p_rrc_phy_config_cell_req->frequancyHoppingParametersR13);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_config_cell_req
(
    U8  **pp_buffer,
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_config_cell_req != PNULL);

    /* This function composes rrc_phy_config_cell_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_config_cell_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_config_cell_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_config_cell_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_config_cell_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_config_cell_req->cell_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_config_phy_cell_parameters(pp_buffer, &p_rrc_phy_config_cell_req->cell_parameters))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_sync_signals(pp_buffer, &p_rrc_phy_config_cell_req->sync_signals))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_prach_configuration(pp_buffer, &p_rrc_phy_config_cell_req->prach_configuration))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pusch_configuration(pp_buffer, &p_rrc_phy_config_cell_req->pusch_configuration))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pucch_configuration(pp_buffer, &p_rrc_phy_config_cell_req->pucch_configuration))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_phich_configuration(pp_buffer, &p_rrc_phy_config_cell_req->phich_configuration))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_pdsch_config_common(pp_buffer, &p_rrc_phy_config_cell_req->pdsch_configuration))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_req->explicit_start_required > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_req->explicit_start_required] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_config_cell_req->explicit_start_required);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_config_cell_req->explicit_start_required, "explicit_start_required");
    *pp_buffer += sizeof(p_rrc_phy_config_cell_req->explicit_start_required);
    }

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPhyPrachConfigV1310(pp_buffer, &p_rrc_phy_config_cell_req->prachConfigurationV1310))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_config_cell_req->bitmask & RRC_PHY_FREQUENCY_HOPPING_PARAMS_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcFrequancyHoppingParametersR13(pp_buffer, &p_rrc_phy_config_cell_req->frequancyHoppingParametersR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_config_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CONFIG_CELL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_config_cell_req
(
    rrc_phy_config_cell_req_t  *p_rrc_phy_config_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_config_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_config_cell_req_len(p_rrc_phy_config_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CONFIG_CELL_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CONFIG_CELL_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CONFIG_CELL_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_CONFIG_CELL_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_config_cell_req(&p_phy_msg, p_rrc_phy_config_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
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
rrc_il_get_sounding_rs_ul_config_common_setup_len
(
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_sounding_rs_ul_config_common_setup != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common_setup->presence_bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common_setup->srs_bw_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common_setup->srs_subframe_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans);

    /* Optional element */
    if(p_sounding_rs_ul_config_common_setup->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common_setup->srs_max_up_pts);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_sounding_rs_ul_config_common_setup
(
    U8  **pp_buffer,
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_sounding_rs_ul_config_common_setup != PNULL);

    /* This function composes sounding_rs_ul_config_common_setup */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_sounding_rs_ul_config_common_setup->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common_setup->presence_bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_bw_config > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_bw_config] should be less than"
            " or equal to 7. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_bw_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_sounding_rs_ul_config_common_setup->srs_bw_config, "srs_bw_config");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common_setup->srs_bw_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_subframe_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_subframe_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_subframe_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_sounding_rs_ul_config_common_setup->srs_subframe_config, "srs_subframe_config");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common_setup->srs_subframe_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans] should be less than"
            " or equal to 1. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans, "ack_nack_srs_simul_trans");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans);

    /* Optional element */
    if(p_sounding_rs_ul_config_common_setup->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_max_up_pts > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_max_up_pts] should be less than"
            " or equal to 0. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_max_up_pts);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_sounding_rs_ul_config_common_setup->srs_max_up_pts, "srs_max_up_pts");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common_setup->srs_max_up_pts);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_sounding_rs_ul_config_common_len
(
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_sounding_rs_ul_config_common != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_sounding_rs_ul_config_common->presence_bitmask);

    /* Optional element */
    if(p_sounding_rs_ul_config_common->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
    {

    /* Get length of IE */
    length += rrc_il_get_sounding_rs_ul_config_common_setup_len(&p_sounding_rs_ul_config_common->setup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_sounding_rs_ul_config_common
(
    U8  **pp_buffer,
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_sounding_rs_ul_config_common != PNULL);

    /* This function composes sounding_rs_ul_config_common */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_sounding_rs_ul_config_common->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_sounding_rs_ul_config_common->presence_bitmask);

    /* Optional element */
    if(p_sounding_rs_ul_config_common->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_sounding_rs_ul_config_common_setup(pp_buffer, &p_sounding_rs_ul_config_common->setup))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_tdd_config_len
(
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_tdd_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tdd_config->sub_frame_assignment);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tdd_config->special_sub_frame_pattern);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_tdd_config
(
    U8  **pp_buffer,
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_tdd_config != PNULL);

    /* This function composes rrc_phy_tdd_config */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_tdd_config->sub_frame_assignment > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tdd_config->sub_frame_assignment] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_phy_tdd_config->sub_frame_assignment);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_tdd_config->sub_frame_assignment, "sub_frame_assignment");
    *pp_buffer += sizeof(p_rrc_phy_tdd_config->sub_frame_assignment);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_tdd_config->special_sub_frame_pattern > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tdd_config->special_sub_frame_pattern] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_phy_tdd_config->special_sub_frame_pattern);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_tdd_config->special_sub_frame_pattern, "special_sub_frame_pattern");
    *pp_buffer += sizeof(p_rrc_phy_tdd_config->special_sub_frame_pattern);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_prs_config_len
(
    rrc_phy_prs_config_t *p_rrc_phy_prs_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_prs_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prs_config->prs_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prs_config->prs_cyclic_prefix_type);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_prs_config
(
    U8  **pp_buffer,
    rrc_phy_prs_config_t *p_rrc_phy_prs_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_prs_config != PNULL);

    /* This function composes rrc_phy_prs_config */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_prs_config->prs_bandwidth < 6) || (p_rrc_phy_prs_config->prs_bandwidth > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prs_config->prs_bandwidth] should be in range "
            "6 to 100. Incorrect value %u received.", p_rrc_phy_prs_config->prs_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prs_config->prs_bandwidth, "prs_bandwidth");
    *pp_buffer += sizeof(p_rrc_phy_prs_config->prs_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prs_config->prs_cyclic_prefix_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prs_config->prs_cyclic_prefix_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_prs_config->prs_cyclic_prefix_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prs_config->prs_cyclic_prefix_type, "prs_cyclic_prefix_type");
    *pp_buffer += sizeof(p_rrc_phy_prs_config->prs_cyclic_prefix_type);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phys_dl_earfcn_len
(
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phys_dl_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phys_dl_earfcn->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_phys_dl_earfcn->count * sizeof(p_rrc_phys_dl_earfcn->earfcnDl[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phys_dl_earfcn
(
    U8  **pp_buffer,
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phys_dl_earfcn != PNULL);

    /* This function composes rrc_phys_dl_earfcn */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phys_dl_earfcn->count > RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phys_dl_earfcn->count] should be less than"
            " or equal to RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT. Incorrect value %u received.", p_rrc_phys_dl_earfcn->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phys_dl_earfcn->count, "count");
    *pp_buffer += sizeof(p_rrc_phys_dl_earfcn->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_phys_dl_earfcn->count; loop++)
        {
            rrc_cp_pack_U32(*pp_buffer, &p_rrc_phys_dl_earfcn->earfcnDl[loop], "earfcnDl[]");
            *pp_buffer += sizeof(U32);
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
rrc_il_get_rrc_config_phy_cell_parameters_len
(
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_config_phy_cell_parameters != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->duplexing_mode);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->freq_band_indicator);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->ul_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->dl_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->num_of_antennas);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->ul_tx_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->dl_tx_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->subcarrier_spacing);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->ul_cyclic_prefix);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->dl_cyclic_prefix);

    /* Get length of IE */
    length += rrc_il_get_sounding_rs_ul_config_common_len(&p_rrc_config_phy_cell_parameters->srs_bandwidth_configuration);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->phys_cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->pcfich_power_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->phich_power_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_config_phy_cell_parameters->max_rs_epre);

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_TDD_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_tdd_config_len(&p_rrc_config_phy_cell_parameters->tdd_config);
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_PRS_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_prs_config_len(&p_rrc_config_phy_cell_parameters->prs_config);
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DL_EARFCN_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phys_dl_earfcn_len(&p_rrc_config_phy_cell_parameters->dl_earfcn_list);
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_LAA_SCELL_CONFIGURATION_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len(&p_rrc_config_phy_cell_parameters->laa_scell_configuration_r13);
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DMTC_CONFIGURATION_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_dmtc_configuration_r13_len(&p_rrc_config_phy_cell_parameters->dmtc_configuration_r13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_config_phy_cell_parameters
(
    U8  **pp_buffer,
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_config_phy_cell_parameters != PNULL);

    /* This function composes rrc_config_phy_cell_parameters */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_config_phy_cell_parameters->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->duplexing_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->duplexing_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->duplexing_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->duplexing_mode, "duplexing_mode");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->duplexing_mode);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_config_phy_cell_parameters->freq_band_indicator < 1) || (p_rrc_config_phy_cell_parameters->freq_band_indicator > MAX_FREQ_BAND_INDICATOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->freq_band_indicator] should be in range "
            "1 to MAX_FREQ_BAND_INDICATOR. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->freq_band_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->freq_band_indicator, "freq_band_indicator");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->freq_band_indicator);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_config_phy_cell_parameters->ul_earfcn, "ul_earfcn");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->ul_earfcn);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_config_phy_cell_parameters->dl_earfcn, "dl_earfcn");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->dl_earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->num_of_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->num_of_antennas] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->num_of_antennas);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->num_of_antennas, "num_of_antennas");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->num_of_antennas);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->ul_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->ul_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->ul_tx_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->ul_tx_bandwidth, "ul_tx_bandwidth");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->ul_tx_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->dl_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->dl_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->dl_tx_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->dl_tx_bandwidth, "dl_tx_bandwidth");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->dl_tx_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->subcarrier_spacing > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->subcarrier_spacing] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->subcarrier_spacing);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->subcarrier_spacing, "subcarrier_spacing");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->subcarrier_spacing);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->ul_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->ul_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->ul_cyclic_prefix);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->ul_cyclic_prefix, "ul_cyclic_prefix");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->ul_cyclic_prefix);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->dl_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->dl_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->dl_cyclic_prefix);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->dl_cyclic_prefix, "dl_cyclic_prefix");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->dl_cyclic_prefix);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_sounding_rs_ul_config_common(pp_buffer, &p_rrc_config_phy_cell_parameters->srs_bandwidth_configuration))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->phys_cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_config_phy_cell_parameters->phys_cell_id, "phys_cell_id");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->phys_cell_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->pcfich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->pcfich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->pcfich_power_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_config_phy_cell_parameters->pcfich_power_offset, "pcfich_power_offset");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->pcfich_power_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->phich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->phich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->phich_power_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_config_phy_cell_parameters->phich_power_offset, "phich_power_offset");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->phich_power_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->max_rs_epre > 160))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->max_rs_epre] should be less than"
            " or equal to 160. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->max_rs_epre);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_config_phy_cell_parameters->max_rs_epre, "max_rs_epre");
    *pp_buffer += sizeof(p_rrc_config_phy_cell_parameters->max_rs_epre);

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_TDD_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_tdd_config(pp_buffer, &p_rrc_config_phy_cell_parameters->tdd_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_PRS_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_prs_config(pp_buffer, &p_rrc_config_phy_cell_parameters->prs_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DL_EARFCN_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phys_dl_earfcn(pp_buffer, &p_rrc_config_phy_cell_parameters->dl_earfcn_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_LAA_SCELL_CONFIGURATION_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_dedicated_laa_scell_config(pp_buffer, &p_rrc_config_phy_cell_parameters->laa_scell_configuration_r13))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DMTC_CONFIGURATION_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_dmtc_configuration_r13(pp_buffer, &p_rrc_config_phy_cell_parameters->dmtc_configuration_r13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_sync_signals_len
(
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_sync_signals != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sync_signals->prim_syn_signal_power);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sync_signals->sec_syn_signal_power);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sync_signals->sec_syn_signal_m_seq1);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sync_signals->sec_syn_signal_m_seq2);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_sync_signals
(
    U8  **pp_buffer,
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_sync_signals != PNULL);

    /* This function composes rrc_phy_sync_signals */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_sync_signals->prim_syn_signal_power < -6) || (p_rrc_phy_sync_signals->prim_syn_signal_power > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->prim_syn_signal_power] should be in range "
            "-6 to 4. Incorrect value %d received.", p_rrc_phy_sync_signals->prim_syn_signal_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_phy_sync_signals->prim_syn_signal_power, "prim_syn_signal_power");
    *pp_buffer += sizeof(p_rrc_phy_sync_signals->prim_syn_signal_power);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_power < -6) || (p_rrc_phy_sync_signals->sec_syn_signal_power > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_power] should be in range "
            "-6 to 4. Incorrect value %d received.", p_rrc_phy_sync_signals->sec_syn_signal_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_phy_sync_signals->sec_syn_signal_power, "sec_syn_signal_power");
    *pp_buffer += sizeof(p_rrc_phy_sync_signals->sec_syn_signal_power);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_m_seq1 > 30))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_m_seq1] should be less than"
            " or equal to 30. Incorrect value %u received.", p_rrc_phy_sync_signals->sec_syn_signal_m_seq1);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sync_signals->sec_syn_signal_m_seq1, "sec_syn_signal_m_seq1");
    *pp_buffer += sizeof(p_rrc_phy_sync_signals->sec_syn_signal_m_seq1);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_m_seq2 > 30))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_m_seq2] should be less than"
            " or equal to 30. Incorrect value %u received.", p_rrc_phy_sync_signals->sec_syn_signal_m_seq2);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sync_signals->sec_syn_signal_m_seq2, "sec_syn_signal_m_seq2");
    *pp_buffer += sizeof(p_rrc_phy_sync_signals->sec_syn_signal_m_seq2);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_prach_configuration_len
(
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_prach_configuration != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prach_configuration->prach_config_sequence);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prach_configuration->root_seq_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prach_configuration->prach_freq_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prach_configuration->zero_cor_zone_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_prach_configuration->high_speed_flag);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_prach_configuration
(
    U8  **pp_buffer,
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_prach_configuration != PNULL);

    /* This function composes rrc_phy_prach_configuration */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->prach_config_sequence > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->prach_config_sequence] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_phy_prach_configuration->prach_config_sequence);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prach_configuration->prach_config_sequence, "prach_config_sequence");
    *pp_buffer += sizeof(p_rrc_phy_prach_configuration->prach_config_sequence);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->root_seq_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->root_seq_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_rrc_phy_prach_configuration->root_seq_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_prach_configuration->root_seq_index, "root_seq_index");
    *pp_buffer += sizeof(p_rrc_phy_prach_configuration->root_seq_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->prach_freq_offset > 104))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->prach_freq_offset] should be less than"
            " or equal to 104. Incorrect value %u received.", p_rrc_phy_prach_configuration->prach_freq_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prach_configuration->prach_freq_offset, "prach_freq_offset");
    *pp_buffer += sizeof(p_rrc_phy_prach_configuration->prach_freq_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->zero_cor_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->zero_cor_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_prach_configuration->zero_cor_zone_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prach_configuration->zero_cor_zone_config, "zero_cor_zone_config");
    *pp_buffer += sizeof(p_rrc_phy_prach_configuration->zero_cor_zone_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_prach_configuration->high_speed_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_prach_configuration->high_speed_flag, "high_speed_flag");
    *pp_buffer += sizeof(p_rrc_phy_prach_configuration->high_speed_flag);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_ul_ref_signals_pusch_len
(
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_ul_ref_signals_pusch != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ul_ref_signals_pusch->group_hopping_enabled);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ul_ref_signals_pusch->group_assign_pusch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ul_ref_signals_pusch->seq_hopping_enabled);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ul_ref_signals_pusch->cyclic_shift);

    return length;
}

static
rrc_return_et
rrc_il_compose_ul_ref_signals_pusch
(
    U8  **pp_buffer,
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ul_ref_signals_pusch != PNULL);

    /* This function composes ul_ref_signals_pusch */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->group_hopping_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->group_hopping_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_ul_ref_signals_pusch->group_hopping_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ul_ref_signals_pusch->group_hopping_enabled, "group_hopping_enabled");
    *pp_buffer += sizeof(p_ul_ref_signals_pusch->group_hopping_enabled);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->group_assign_pusch > 29))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->group_assign_pusch] should be less than"
            " or equal to 29. Incorrect value %u received.", p_ul_ref_signals_pusch->group_assign_pusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ul_ref_signals_pusch->group_assign_pusch, "group_assign_pusch");
    *pp_buffer += sizeof(p_ul_ref_signals_pusch->group_assign_pusch);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->seq_hopping_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->seq_hopping_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_ul_ref_signals_pusch->seq_hopping_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ul_ref_signals_pusch->seq_hopping_enabled, "seq_hopping_enabled");
    *pp_buffer += sizeof(p_ul_ref_signals_pusch->seq_hopping_enabled);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_ul_ref_signals_pusch->cyclic_shift);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ul_ref_signals_pusch->cyclic_shift, "cyclic_shift");
    *pp_buffer += sizeof(p_ul_ref_signals_pusch->cyclic_shift);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_pusch_configuration_len
(
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_pusch_configuration != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration->pusch_hopping_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration->num_of_sub_bands);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration->pusch_hopping_mode);

    /* Get length of IE */
    length += rrc_il_get_ul_ref_signals_pusch_len(&p_rrc_phy_pusch_configuration->ul_ref_signal);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_pusch_configuration
(
    U8  **pp_buffer,
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_pusch_configuration != PNULL);

    /* This function composes rrc_phy_pusch_configuration */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration->pusch_hopping_offset > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->pusch_hopping_offset] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_phy_pusch_configuration->pusch_hopping_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration->pusch_hopping_offset, "pusch_hopping_offset");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration->pusch_hopping_offset);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_pusch_configuration->num_of_sub_bands < 1) || (p_rrc_phy_pusch_configuration->num_of_sub_bands > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->num_of_sub_bands] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_pusch_configuration->num_of_sub_bands);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration->num_of_sub_bands, "num_of_sub_bands");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration->num_of_sub_bands);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration->pusch_hopping_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->pusch_hopping_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_pusch_configuration->pusch_hopping_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration->pusch_hopping_mode, "pusch_hopping_mode");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration->pusch_hopping_mode);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_ul_ref_signals_pusch(pp_buffer, &p_rrc_phy_pusch_configuration->ul_ref_signal))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_pucch_configuration_len
(
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_pucch_configuration != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration->delta_pucch_shift);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration->nrb_cqi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration->ncs_an);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration->n1pucch_an);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_pucch_configuration
(
    U8  **pp_buffer,
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_pucch_configuration != PNULL);

    /* This function composes rrc_phy_pucch_configuration */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_pucch_configuration->delta_pucch_shift < 1) || (p_rrc_phy_pucch_configuration->delta_pucch_shift > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->delta_pucch_shift] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_phy_pucch_configuration->delta_pucch_shift);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pucch_configuration->delta_pucch_shift, "delta_pucch_shift");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration->delta_pucch_shift);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->nrb_cqi > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->nrb_cqi] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_phy_pucch_configuration->nrb_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pucch_configuration->nrb_cqi, "nrb_cqi");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration->nrb_cqi);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->ncs_an > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->ncs_an] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_pucch_configuration->ncs_an);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pucch_configuration->ncs_an, "ncs_an");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration->ncs_an);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->n1pucch_an > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->n1pucch_an] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_pucch_configuration->n1pucch_an);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_pucch_configuration->n1pucch_an, "n1pucch_an");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration->n1pucch_an);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_phich_configuration_len
(
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_phich_configuration != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_phich_configuration->phich_resource);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_phich_configuration->phich_duration);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_phich_configuration
(
    U8  **pp_buffer,
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_phich_configuration != PNULL);

    /* This function composes rrc_phy_phich_configuration */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_phich_configuration->phich_resource > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_phich_configuration->phich_resource] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_phich_configuration->phich_resource);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_phich_configuration->phich_resource, "phich_resource");
    *pp_buffer += sizeof(p_rrc_phy_phich_configuration->phich_resource);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_phich_configuration->phich_duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_phich_configuration->phich_duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_phich_configuration->phich_duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_phich_configuration->phich_duration, "phich_duration");
    *pp_buffer += sizeof(p_rrc_phy_phich_configuration->phich_duration);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_pdsch_config_common_len
(
    pdsch_config_common_t *p_pdsch_config_common
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_pdsch_config_common != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_pdsch_config_common->ref_signal_power);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_pdsch_config_common->pb);

    return length;
}

static
rrc_return_et
rrc_il_compose_pdsch_config_common
(
    U8  **pp_buffer,
    pdsch_config_common_t *p_pdsch_config_common
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_pdsch_config_common != PNULL);

    /* This function composes pdsch_config_common */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_pdsch_config_common->ref_signal_power < -60) || (p_pdsch_config_common->ref_signal_power > 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pdsch_config_common->ref_signal_power] should be in range "
            "-60 to 50. Incorrect value %d received.", p_pdsch_config_common->ref_signal_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_pdsch_config_common->ref_signal_power, "ref_signal_power");
    *pp_buffer += sizeof(p_pdsch_config_common->ref_signal_power);

    /* Check for correct range [H - higher boundary] */
    if ((p_pdsch_config_common->pb > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pdsch_config_common->pb] should be less than"
            " or equal to 3. Incorrect value %u received.", p_pdsch_config_common->pb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_pdsch_config_common->pb, "pb");
    *pp_buffer += sizeof(p_pdsch_config_common->pb);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPhyPrachParametersCeR13_len
(
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPhyPrachParametersCeR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->ceLevel);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->prachConfigIndexR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13);

    /* Optional element */
    if(p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_HOPPING_OFFSET_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13);
    }

    /* Optional element */
    if(p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_START_SF_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersCeR13->prachStartSfR13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPhyPrachParametersCeR13
(
    U8  **pp_buffer,
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPhyPrachParametersCeR13 != PNULL);

    /* This function composes RrcPhyPrachParametersCeR13 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->ceLevel > EMTC_CE_LEVEL_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->ceLevel] should be less than"
            " or equal to EMTC_CE_LEVEL_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->ceLevel);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->ceLevel, "ceLevel");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->ceLevel);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachConfigIndexR13 > EMTC_MAX_PRACH_CONFIG_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachConfigIndexR13] should be less than"
            " or equal to EMTC_MAX_PRACH_CONFIG_INDEX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachConfigIndexR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->prachConfigIndexR13, "prachConfigIndexR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->prachConfigIndexR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13 > EMTC_MAX_PRACH_FREQ_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_FREQ_OFFSET. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13, "prachFreqOffsetR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13 > EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13] should be less than"
            " or equal to EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13, "numRepetitionPerPreambleAttemptR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13 > EMTC_PRACH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13] should be less than"
            " or equal to EMTC_PRACH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13, "prachHoppingConfigR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13);

    /* Optional element */
    if(p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_HOPPING_OFFSET_R13_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13 > EMTC_MAX_PRACH_HOPPING_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_HOPPING_OFFSET. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13, "prachHoppingOffsetR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13);
    }

    /* Optional element */
    if(p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_START_SF_R13_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachStartSfR13 > EMTC_PRACH_STARTING_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachStartSfR13] should be less than"
            " or equal to EMTC_PRACH_STARTING_SF_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachStartSfR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersCeR13->prachStartSfR13, "prachStartSfR13");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersCeR13->prachStartSfR13);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPhyPrachParametersListCeR13_len
(
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPhyPrachParametersListCeR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhyPrachParametersListCeR13->numCeLevels);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcPhyPrachParametersListCeR13->numCeLevels < 1) || (p_RrcPhyPrachParametersListCeR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersListCeR13->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %d received.", p_RrcPhyPrachParametersListCeR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcPhyPrachParametersListCeR13->numCeLevels; loop++)
        {
            length += rrc_il_get_RrcPhyPrachParametersCeR13_len(&p_RrcPhyPrachParametersListCeR13->prachParametersCeR13[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPhyPrachParametersListCeR13
(
    U8  **pp_buffer,
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPhyPrachParametersListCeR13 != PNULL);

    /* This function composes RrcPhyPrachParametersListCeR13 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcPhyPrachParametersListCeR13->numCeLevels < 1) || (p_RrcPhyPrachParametersListCeR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersListCeR13->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcPhyPrachParametersListCeR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhyPrachParametersListCeR13->numCeLevels, "numCeLevels");
    *pp_buffer += sizeof(p_RrcPhyPrachParametersListCeR13->numCeLevels);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcPhyPrachParametersListCeR13->numCeLevels; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_RrcPhyPrachParametersCeR13(pp_buffer, &p_RrcPhyPrachParametersListCeR13->prachParametersCeR13[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPhyPrachConfigV1310_len
(
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPhyPrachConfigV1310 != PNULL);

    /* Get length of IE */
    length += rrc_il_get_RrcPhyPrachParametersListCeR13_len(&p_RrcPhyPrachConfigV1310->prachParametersListCe);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPhyPrachConfigV1310
(
    U8  **pp_buffer,
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPhyPrachConfigV1310 != PNULL);

    /* This function composes RrcPhyPrachConfigV1310 */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPhyPrachParametersListCeR13(pp_buffer, &p_RrcPhyPrachConfigV1310->prachParametersListCe))
    {
        return RRC_FAILURE;
    }

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
rrc_il_get_rrc_phy_config_cell_cnf_len
(
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_config_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_config_cell_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_config_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_config_cell_cnf != PNULL);

    /* This function composes rrc_phy_config_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_config_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_config_cell_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_config_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_config_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_config_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CONFIG_CELL_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_config_cell_cnf
(
    rrc_phy_config_cell_cnf_t  *p_rrc_phy_config_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_config_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_config_cell_cnf_len(p_rrc_phy_config_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CONFIG_CELL_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CONFIG_CELL_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CONFIG_CELL_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_CONFIG_CELL_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_config_cell_cnf(&p_phy_msg, p_rrc_phy_config_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_reconfig_cell_req_len
(
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_reconfig_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_req->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_req->sfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_req->sf);

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_reconfig_cell_parameters_len(&p_rrc_phy_reconfig_cell_req->phy_recfg_cell_parameters);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_sync_signals_len(&p_rrc_phy_reconfig_cell_req->sync_signals);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_prach_configuration_len(&p_rrc_phy_reconfig_cell_req->prach_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pusch_configuration_len(&p_rrc_phy_reconfig_cell_req->pusch_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pucch_configuration_len(&p_rrc_phy_reconfig_cell_req->pucch_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_phich_configuration_len(&p_rrc_phy_reconfig_cell_req->phich_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_pdsch_config_common_len(&p_rrc_phy_reconfig_cell_req->pdsch_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcPhyPrachConfigV1310_len(&p_rrc_phy_reconfig_cell_req->prachConfigurationV1310);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_FREQUANCY_HOPPING_PARAMS_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcFrequancyHoppingParametersR13_len(&p_rrc_phy_reconfig_cell_req->frequancyHoppingParametersR13);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_req
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_reconfig_cell_req != PNULL);

    /* This function composes rrc_phy_reconfig_cell_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_reconfig_cell_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_req->cell_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_req->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_req->sfn);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_req->sf, "sf");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_req->sf);

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_reconfig_cell_parameters(pp_buffer, &p_rrc_phy_reconfig_cell_req->phy_recfg_cell_parameters))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_sync_signals(pp_buffer, &p_rrc_phy_reconfig_cell_req->sync_signals))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_prach_configuration(pp_buffer, &p_rrc_phy_reconfig_cell_req->prach_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pusch_configuration(pp_buffer, &p_rrc_phy_reconfig_cell_req->pusch_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pucch_configuration(pp_buffer, &p_rrc_phy_reconfig_cell_req->pucch_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_phich_configuration(pp_buffer, &p_rrc_phy_reconfig_cell_req->phich_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_pdsch_config_common(pp_buffer, &p_rrc_phy_reconfig_cell_req->pdsch_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPhyPrachConfigV1310(pp_buffer, &p_rrc_phy_reconfig_cell_req->prachConfigurationV1310))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_FREQUANCY_HOPPING_PARAMS_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcFrequancyHoppingParametersR13(pp_buffer, &p_rrc_phy_reconfig_cell_req->frequancyHoppingParametersR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_reconfig_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_RECONFIG_CELL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_cell_req
(
    rrc_phy_reconfig_cell_req_t  *p_rrc_phy_reconfig_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_reconfig_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_reconfig_cell_req_len(p_rrc_phy_reconfig_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_RECONFIG_CELL_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_RECONFIG_CELL_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_RECONFIG_CELL_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_RECONFIG_CELL_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_reconfig_cell_req(&p_phy_msg, p_rrc_phy_reconfig_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_phy_reconfig_cell_parameters_len
(
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_reconfig_cell_parameters != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->bitmask);

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DUPLEX_MODE)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->duplexing_mode);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_EARFCN)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->ul_earfcn);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_EARFCN)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->dl_earfcn);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_NUM_OF_ANTENNAS)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->num_of_antennas);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_sounding_rs_ul_config_common_len(&p_rrc_phy_reconfig_cell_parameters->srs_bandwidth_configuration);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_CELL_ID)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->phys_cell_id);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PCFICH_POWER_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PHICH_POWER_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->phich_power_offset);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_parameters->max_rs_epre);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_PRS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_prs_config_len(&p_rrc_phy_reconfig_cell_parameters->prs_reconfig);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_parameters
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_reconfig_cell_parameters != PNULL);

    /* This function composes rrc_phy_reconfig_cell_parameters */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->bitmask);

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DUPLEX_MODE)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->duplexing_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->duplexing_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->duplexing_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->duplexing_mode, "duplexing_mode");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->duplexing_mode);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_EARFCN)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->ul_earfcn, "ul_earfcn");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->ul_earfcn);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_EARFCN)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->dl_earfcn, "dl_earfcn");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->dl_earfcn);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_NUM_OF_ANTENNAS)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_reconfig_cell_parameters->num_of_antennas < 1) || (p_rrc_phy_reconfig_cell_parameters->num_of_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->num_of_antennas] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->num_of_antennas);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->num_of_antennas, "num_of_antennas");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->num_of_antennas);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth, "ul_tx_bandwidth");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth, "dl_tx_bandwidth");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix, "ul_cyclic_prefix");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix, "dl_cyclic_prefix");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_sounding_rs_ul_config_common(pp_buffer, &p_rrc_phy_reconfig_cell_parameters->srs_bandwidth_configuration))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_CELL_ID)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->phys_cell_id, "phys_cell_id");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->phys_cell_id);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PCFICH_POWER_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset, "pcfich_power_offset");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PHICH_POWER_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->phich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->phich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->phich_power_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->phich_power_offset, "phich_power_offset");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->phich_power_offset);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers, "pri_sync_sig_epre_eprers");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers, "sec_sync_sig_epre_eprers");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->max_rs_epre > 160))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->max_rs_epre] should be less than"
            " or equal to 160. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->max_rs_epre);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_parameters->max_rs_epre, "max_rs_epre");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_parameters->max_rs_epre);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_PRS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_prs_config(pp_buffer, &p_rrc_phy_reconfig_cell_parameters->prs_reconfig))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_reconfig_cell_cnf_len
(
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_reconfig_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_reconfig_cell_cnf != PNULL);

    /* This function composes rrc_phy_reconfig_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_reconfig_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_reconfig_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_RECONFIG_CELL_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_cell_cnf
(
    rrc_phy_reconfig_cell_cnf_t  *p_rrc_phy_reconfig_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_reconfig_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_reconfig_cell_cnf_len(p_rrc_phy_reconfig_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_RECONFIG_CELL_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_RECONFIG_CELL_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_RECONFIG_CELL_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_RECONFIG_CELL_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_reconfig_cell_cnf(&p_phy_msg, p_rrc_phy_reconfig_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_delete_cell_req_len
(
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_delete_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_delete_cell_req->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_delete_cell_req
(
    U8  **pp_buffer,
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_delete_cell_req != PNULL);

    /* This function composes rrc_phy_delete_cell_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_delete_cell_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_delete_cell_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_delete_cell_req->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_delete_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_DELETE_CELL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_delete_cell_req
(
    rrc_phy_delete_cell_req_t  *p_rrc_phy_delete_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_delete_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_delete_cell_req_len(p_rrc_phy_delete_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_DELETE_CELL_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_DELETE_CELL_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_DELETE_CELL_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_DELETE_CELL_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_delete_cell_req(&p_phy_msg, p_rrc_phy_delete_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_delete_cell_cnf_len
(
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_delete_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_delete_cell_cnf->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_delete_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_delete_cell_cnf != PNULL);

    /* This function composes rrc_phy_delete_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_delete_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_delete_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_delete_cell_cnf->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_delete_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_DELETE_CELL_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_delete_cell_cnf
(
    rrc_phy_delete_cell_cnf_t  *p_rrc_phy_delete_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_delete_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_delete_cell_cnf_len(p_rrc_phy_delete_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_DELETE_CELL_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_DELETE_CELL_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_DELETE_CELL_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_DELETE_CELL_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_delete_cell_cnf(&p_phy_msg, p_rrc_phy_delete_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_create_ue_entity_req_len
(
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_create_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_create_ue_entity_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_create_ue_entity_req->rnti);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_physical_config_dedicated_len(&p_rrc_phy_create_ue_entity_req->physical_config_dedicated);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_create_ue_entity_req->bitmask);

    /* Optional element */
    if(p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_SCELL_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_scell_config_list_len(&p_rrc_phy_create_ue_entity_req->rrc_phy_scell_config_list);
    }

    /* Optional element */
    if(p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcPhysicalConfigDedicatedR13_len(&p_rrc_phy_create_ue_entity_req->physicalConfigDedicatedR13);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_create_ue_entity_req != PNULL);

    /* This function composes rrc_phy_create_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_create_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_create_ue_entity_req->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_create_ue_entity_req->rnti < 1) || (p_rrc_phy_create_ue_entity_req->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_create_ue_entity_req->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_phy_create_ue_entity_req->rnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_create_ue_entity_req->rnti, "rnti");
    *pp_buffer += sizeof(p_rrc_phy_create_ue_entity_req->rnti);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_physical_config_dedicated(pp_buffer, &p_rrc_phy_create_ue_entity_req->physical_config_dedicated))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_create_ue_entity_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_create_ue_entity_req->bitmask);

    /* Optional element */
    if(p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_SCELL_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_scell_config_list(pp_buffer, &p_rrc_phy_create_ue_entity_req->rrc_phy_scell_config_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPhysicalConfigDedicatedR13(pp_buffer, &p_rrc_phy_create_ue_entity_req->physicalConfigDedicatedR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_create_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CREATE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_create_ue_entity_req
(
    rrc_phy_create_ue_entity_req_t  *p_rrc_phy_create_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_create_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_create_ue_entity_req_len(p_rrc_phy_create_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CREATE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CREATE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CREATE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_CREATE_UE_ENTITY_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_create_ue_entity_req(&p_phy_msg, p_rrc_phy_create_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_phy_pdsch_configuration_dedicated_len
(
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_pdsch_configuration_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pdsch_configuration_dedicated->p_a);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_pdsch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_pdsch_configuration_dedicated != PNULL);

    /* This function composes rrc_phy_pdsch_configuration_dedicated */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pdsch_configuration_dedicated->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pdsch_configuration_dedicated->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_pdsch_configuration_dedicated->p_a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pdsch_configuration_dedicated->p_a, "p_a");
    *pp_buffer += sizeof(p_rrc_phy_pdsch_configuration_dedicated->p_a);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_ack_nack_repetition_param_len
(
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_ack_nack_repetition_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_ack_nack_repetition_param->factor);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_ack_nack_repetition_param->an_rep);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_ack_nack_repetition_param
(
    U8  **pp_buffer,
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_ack_nack_repetition_param != PNULL);

    /* This function composes rrc_phy_ack_nack_repetition_param */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ack_nack_repetition_param->factor > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ack_nack_repetition_param->factor] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_phy_ack_nack_repetition_param->factor);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_ack_nack_repetition_param->factor, "factor");
    *pp_buffer += sizeof(p_rrc_phy_ack_nack_repetition_param->factor);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ack_nack_repetition_param->an_rep > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ack_nack_repetition_param->an_rep] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_ack_nack_repetition_param->an_rep);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_ack_nack_repetition_param->an_rep, "an_rep");
    *pp_buffer += sizeof(p_rrc_phy_ack_nack_repetition_param->an_rep);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_pucch_configuration_dedicated_len
(
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_pucch_configuration_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_ack_nack_repetition_param_len(&p_rrc_phy_pucch_configuration_dedicated->ack_nack_repetition_param);
    }

    /* Optional element */
    if(p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_pucch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_pucch_configuration_dedicated != PNULL);

    /* This function composes rrc_phy_pucch_configuration_dedicated */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_pucch_configuration_dedicated->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_ack_nack_repetition_param(pp_buffer, &p_rrc_phy_pucch_configuration_dedicated->ack_nack_repetition_param))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode, "tdd_ack_nack_feedback_mode");
    *pp_buffer += sizeof(p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_pusch_configuration_dedicated_len
(
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_pusch_configuration_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_pusch_configuration_dedicated
(
    U8  **pp_buffer,
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_pusch_configuration_dedicated != PNULL);

    /* This function composes rrc_phy_pusch_configuration_dedicated */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index, "beta_offset_ack_index");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index, "beta_offset_ri_index");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index, "beta_offset_cqi_index");
    *pp_buffer += sizeof(p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_uplink_power_control_dedicated_len
(
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_uplink_power_control_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->p_srs_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_uplink_power_control_dedicated->filter_coefficient);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_uplink_power_control_dedicated
(
    U8  **pp_buffer,
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_uplink_power_control_dedicated != PNULL);

    /* This function composes rrc_phy_uplink_power_control_dedicated */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch < -8) || (p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch, "p0_ue_pusch");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled, "delta_mcs_enabled");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled, "accumulation_enabled");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch < -8) || (p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch, "p0_ue_pucch");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p_srs_offset > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p_srs_offset] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->p_srs_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->p_srs_offset, "p_srs_offset");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->p_srs_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->filter_coefficient > 14))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->filter_coefficient] should be less than"
            " or equal to 14. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->filter_coefficient);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_uplink_power_control_dedicated->filter_coefficient, "filter_coefficient");
    *pp_buffer += sizeof(p_rrc_phy_uplink_power_control_dedicated->filter_coefficient);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_index_len
(
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_tpc_index != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tpc_index->bitmask);

    /* Optional element */
    if(p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tpc_index->index_of_format3);
    }

    /* Optional element */
    if(p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3A_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tpc_index->index_of_format3a);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_index
(
    U8  **pp_buffer,
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_tpc_index != PNULL);

    /* This function composes rrc_phy_tpc_index */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_tpc_index->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_tpc_index->bitmask);

    /* Optional element */
    if(p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_tpc_index->index_of_format3 < 1) || (p_rrc_phy_tpc_index->index_of_format3 > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tpc_index->index_of_format3] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_phy_tpc_index->index_of_format3);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_tpc_index->index_of_format3, "index_of_format3");
    *pp_buffer += sizeof(p_rrc_phy_tpc_index->index_of_format3);
    }

    /* Optional element */
    if(p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3A_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_tpc_index->index_of_format3a < 1) || (p_rrc_phy_tpc_index->index_of_format3a > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tpc_index->index_of_format3a] should be in range "
            "1 to 31. Incorrect value %u received.", p_rrc_phy_tpc_index->index_of_format3a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_tpc_index->index_of_format3a, "index_of_format3a");
    *pp_buffer += sizeof(p_rrc_phy_tpc_index->index_of_format3a);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_pdcch_config_param_len
(
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_tpc_pdcch_config_param != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_tpc_pdcch_config_param->tpc_rnti);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_tpc_index_len(&p_rrc_phy_tpc_pdcch_config_param->tpc_index);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_pdcch_config_param
(
    U8  **pp_buffer,
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_tpc_pdcch_config_param != PNULL);

    /* This function composes rrc_phy_tpc_pdcch_config_param */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_tpc_pdcch_config_param->tpc_rnti); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_tpc_pdcch_config_param->tpc_rnti[loop], "tpc_rnti[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_tpc_index(pp_buffer, &p_rrc_phy_tpc_pdcch_config_param->tpc_index))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_tpc_pdcch_configuration_len
(
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_tpc_pdcch_configuration != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_tpc_pdcch_configuration->bitmask);

    /* Optional element */
    if(p_rrc_phy_tpc_pdcch_configuration->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_tpc_pdcch_config_param_len(&p_rrc_phy_tpc_pdcch_configuration->tpc_pdcch_config_param);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_tpc_pdcch_configuration
(
    U8  **pp_buffer,
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_tpc_pdcch_configuration != PNULL);

    /* This function composes rrc_phy_tpc_pdcch_configuration */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_tpc_pdcch_configuration->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_tpc_pdcch_configuration->bitmask);

    /* Optional element */
    if(p_rrc_phy_tpc_pdcch_configuration->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_tpc_pdcch_config_param(pp_buffer, &p_rrc_phy_tpc_pdcch_configuration->tpc_pdcch_config_param))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_subband_cqi_param_param_len
(
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_subband_cqi_param_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_subband_cqi_param_param->k);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_subband_cqi_param_param
(
    U8  **pp_buffer,
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_subband_cqi_param_param != PNULL);

    /* This function composes rrc_phy_subband_cqi_param_param */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_subband_cqi_param_param->k < 1) || (p_rrc_phy_subband_cqi_param_param->k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_subband_cqi_param_param->k] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_subband_cqi_param_param->k);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_subband_cqi_param_param->k, "k");
    *pp_buffer += sizeof(p_rrc_phy_subband_cqi_param_param->k);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_indicator_periodic_len
(
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_format_indicator_periodic != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_indicator_periodic->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_indicator_periodic->bitmask & RRC_PHY_SUBBAND_CQI_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_subband_cqi_param_param_len(&p_rrc_phy_cqi_format_indicator_periodic->subband_cqi_param);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_indicator_periodic
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_format_indicator_periodic != PNULL);

    /* This function composes rrc_phy_cqi_format_indicator_periodic */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_format_indicator_periodic->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_indicator_periodic->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_indicator_periodic->bitmask & RRC_PHY_SUBBAND_CQI_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_subband_cqi_param_param(pp_buffer, &p_rrc_phy_cqi_format_indicator_periodic->subband_cqi_param))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_periodic_param_len
(
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_reporting_periodic_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic_param->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_format_indicator_periodic_len(&p_rrc_phy_cqi_reporting_periodic_param->cqi_format_indicator_periodic);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting_periodic_param->bitmask & RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic_param->ri_config_index);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting_periodic_param
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_reporting_periodic_param != PNULL);

    /* This function composes rrc_phy_cqi_reporting_periodic_param */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic_param->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index, "cqi_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index, "cqi_pmi_config_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_format_indicator_periodic(pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->cqi_format_indicator_periodic))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rrc_phy_cqi_reporting_periodic_param->bitmask & RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->ri_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->ri_config_index, "ri_config_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic_param->ri_config_index);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi, "simultaneous_ack_nack_and_cqi");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_periodic_len
(
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_reporting_periodic != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting_periodic->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting_periodic->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_reporting_periodic_param_len(&p_rrc_phy_cqi_reporting_periodic->cqi_reporting_periodic_param);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting_periodic
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_reporting_periodic != PNULL);

    /* This function composes rrc_phy_cqi_reporting_periodic */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting_periodic->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting_periodic->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting_periodic->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_reporting_periodic_param(pp_buffer, &p_rrc_phy_cqi_reporting_periodic->cqi_reporting_periodic_param))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_reporting_len
(
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_reporting != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_reporting_periodic_len(&p_rrc_phy_cqi_reporting->cqi_reporting_periodic);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_reporting
(
    U8  **pp_buffer,
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_reporting != PNULL);

    /* This function composes rrc_phy_cqi_reporting */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_reporting->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic, "cqi_reporting_mode_aperiodic");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset < -1) || (p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset, "nom_pdsch_rs_epre_offset");
    *pp_buffer += sizeof(p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset);

    /* Optional element */
    if(p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_reporting_periodic(pp_buffer, &p_rrc_phy_cqi_reporting->cqi_reporting_periodic))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_param_len
(
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_sounding_rs_ul_config_dedicated_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated_param
(
    U8  **pp_buffer,
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_sounding_rs_ul_config_dedicated_param != PNULL);

    /* This function composes rrc_phy_sounding_rs_ul_config_dedicated_param */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth, "srs_bandwidth");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth, "srs_hopping_bandwidth");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position] should be less than"
            " or equal to 23. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position, "frequency_domain_position");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration, "duration");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index, "srs_configuration_index");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb, "transmission_comb");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift, "cyclic_shift");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_len
(
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_sounding_rs_ul_config_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_param_len(&p_rrc_phy_sounding_rs_ul_config_dedicated->sounding_rs_ul_config_dedicated_param);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated
(
    U8  **pp_buffer,
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_sounding_rs_ul_config_dedicated != PNULL);

    /* This function composes rrc_phy_sounding_rs_ul_config_dedicated */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated_param(pp_buffer, &p_rrc_phy_sounding_rs_ul_config_dedicated->sounding_rs_ul_config_dedicated_param))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_codebook_subset_restriction_len
(
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_codebook_subset_restriction != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_codebook_subset_restriction->type);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_codebook_subset_restriction->value);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_codebook_subset_restriction
(
    U8  **pp_buffer,
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_codebook_subset_restriction != PNULL);

    /* This function composes rrc_phy_codebook_subset_restriction */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_codebook_subset_restriction->type > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_codebook_subset_restriction->type] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_codebook_subset_restriction->type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_codebook_subset_restriction->type, "type");
    *pp_buffer += sizeof(p_rrc_phy_codebook_subset_restriction->type);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_codebook_subset_restriction->value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_codebook_subset_restriction->value[loop], "value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_ue_transmit_antenna_selection_len
(
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_ue_transmit_antenna_selection != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_ue_transmit_antenna_selection->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_ue_transmit_antenna_selection->request_type);

    /* Optional element */
    if(p_rrc_phy_ue_transmit_antenna_selection->bitmask & RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_ue_transmit_antenna_selection
(
    U8  **pp_buffer,
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_ue_transmit_antenna_selection != PNULL);

    /* This function composes rrc_phy_ue_transmit_antenna_selection */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_ue_transmit_antenna_selection->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_ue_transmit_antenna_selection->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ue_transmit_antenna_selection->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ue_transmit_antenna_selection->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_ue_transmit_antenna_selection->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_ue_transmit_antenna_selection->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_phy_ue_transmit_antenna_selection->request_type);

    /* Optional element */
    if(p_rrc_phy_ue_transmit_antenna_selection->bitmask & RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type, "ue_transmit_antenna_selection_type");
    *pp_buffer += sizeof(p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_information_dedicated_len
(
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_antenna_information_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_antenna_information_dedicated->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_antenna_information_dedicated->transmission_mode);

    /* Optional element */
    if(p_rrc_phy_antenna_information_dedicated->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_codebook_subset_restriction_len(&p_rrc_phy_antenna_information_dedicated->codebook_subset_restriction);
    }

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_ue_transmit_antenna_selection_len(&p_rrc_phy_antenna_information_dedicated->ue_transmit_antenna_selection);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_information_dedicated
(
    U8  **pp_buffer,
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_antenna_information_dedicated != PNULL);

    /* This function composes rrc_phy_antenna_information_dedicated */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_antenna_information_dedicated->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_antenna_information_dedicated->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_antenna_information_dedicated->transmission_mode > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_antenna_information_dedicated->transmission_mode] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_antenna_information_dedicated->transmission_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_antenna_information_dedicated->transmission_mode, "transmission_mode");
    *pp_buffer += sizeof(p_rrc_phy_antenna_information_dedicated->transmission_mode);

    /* Optional element */
    if(p_rrc_phy_antenna_information_dedicated->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_codebook_subset_restriction(pp_buffer, &p_rrc_phy_antenna_information_dedicated->codebook_subset_restriction))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_ue_transmit_antenna_selection(pp_buffer, &p_rrc_phy_antenna_information_dedicated->ue_transmit_antenna_selection))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_information_len
(
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_antenna_information != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_antenna_information->bitmask);

    /* Optional element */
    if(p_rrc_phy_antenna_information->bitmask & RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_antenna_information_dedicated_len(&p_rrc_phy_antenna_information->antenna_information_explicit_value);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_information
(
    U8  **pp_buffer,
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_antenna_information != PNULL);

    /* This function composes rrc_phy_antenna_information */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_antenna_information->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_antenna_information->bitmask);

    /* Optional element */
    if(p_rrc_phy_antenna_information->bitmask & RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_antenna_information_dedicated(pp_buffer, &p_rrc_phy_antenna_information->antenna_information_explicit_value))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_scheduling_request_config_param_len
(
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_scheduling_request_config_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scheduling_request_config_param->sr_configuration_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scheduling_request_config_param->dsr_trans_max);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_scheduling_request_config_param
(
    U8  **pp_buffer,
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_scheduling_request_config_param != PNULL);

    /* This function composes rrc_phy_scheduling_request_config_param */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index, "sr_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->sr_configuration_index > 157))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->sr_configuration_index] should be less than"
            " or equal to 157. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->sr_configuration_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_scheduling_request_config_param->sr_configuration_index, "sr_configuration_index");
    *pp_buffer += sizeof(p_rrc_phy_scheduling_request_config_param->sr_configuration_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->dsr_trans_max > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->dsr_trans_max] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->dsr_trans_max);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_scheduling_request_config_param->dsr_trans_max, "dsr_trans_max");
    *pp_buffer += sizeof(p_rrc_phy_scheduling_request_config_param->dsr_trans_max);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_scheduling_request_config_len
(
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_scheduling_request_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scheduling_request_config->bitmask);

    /* Optional element */
    if(p_rrc_phy_scheduling_request_config->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_scheduling_request_config_param_len(&p_rrc_phy_scheduling_request_config->scheduling_request_config_param);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_scheduling_request_config
(
    U8  **pp_buffer,
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_scheduling_request_config != PNULL);

    /* This function composes rrc_phy_scheduling_request_config */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_scheduling_request_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_scheduling_request_config->bitmask);

    /* Optional element */
    if(p_rrc_phy_scheduling_request_config->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_scheduling_request_config_param(pp_buffer, &p_rrc_phy_scheduling_request_config->scheduling_request_config_param))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_config_v920_len
(
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_config_v920 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_config_v920->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_CQI_MASK_R9_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_config_v920->cqi_mask_r9);
    }

    /* Optional element */
    if(p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_config_v920
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_config_v920 != PNULL);

    /* This function composes rrc_phy_cqi_report_config_v920 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_config_v920->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_config_v920->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_CQI_MASK_R9_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_config_v920->cqi_mask_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_config_v920->cqi_mask_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_config_v920->cqi_mask_r9);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_config_v920->cqi_mask_r9, "cqi_mask_r9");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_config_v920->cqi_mask_r9);
    }

    /* Optional element */
    if(p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9, "pmi_ri_report_r9");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_codebook_subset_restriction_v920_len
(
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_codebook_subset_restriction_v920 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_codebook_subset_restriction_v920->type);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_codebook_subset_restriction_v920->value);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_codebook_subset_restriction_v920
(
    U8  **pp_buffer,
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_codebook_subset_restriction_v920 != PNULL);

    /* This function composes rrc_phy_codebook_subset_restriction_v920 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_codebook_subset_restriction_v920->type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_codebook_subset_restriction_v920->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_codebook_subset_restriction_v920->type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_codebook_subset_restriction_v920->type, "type");
    *pp_buffer += sizeof(p_rrc_phy_codebook_subset_restriction_v920->type);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_codebook_subset_restriction_v920->value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_codebook_subset_restriction_v920->value[loop], "value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_antenna_info_v920_len
(
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_antenna_info_v920 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_antenna_info_v920->bitmask);

    /* Optional element */
    if(p_rrc_phy_antenna_info_v920->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_codebook_subset_restriction_v920_len(&p_rrc_phy_antenna_info_v920->codebook_subset_restriction_v920);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_antenna_info_v920
(
    U8  **pp_buffer,
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_antenna_info_v920 != PNULL);

    /* This function composes rrc_phy_antenna_info_v920 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_antenna_info_v920->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_antenna_info_v920->bitmask);

    /* Optional element */
    if(p_rrc_phy_antenna_info_v920->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_codebook_subset_restriction_v920(pp_buffer, &p_rrc_phy_antenna_info_v920->codebook_subset_restriction_v920))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_codebook_subset_restriction_r10_len
(
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_codebook_subset_restriction_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_codebook_subset_restriction_r10->num_bits);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_codebook_subset_restriction_r10->value);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_codebook_subset_restriction_r10
(
    U8  **pp_buffer,
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_codebook_subset_restriction_r10 != PNULL);

    /* This function composes rrc_codebook_subset_restriction_r10 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_codebook_subset_restriction_r10->num_bits < 2) || (p_rrc_codebook_subset_restriction_r10->num_bits > 109))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_codebook_subset_restriction_r10->num_bits] should be in range "
            "2 to 109. Incorrect value %u received.", p_rrc_codebook_subset_restriction_r10->num_bits);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_codebook_subset_restriction_r10->num_bits, "num_bits");
    *pp_buffer += sizeof(p_rrc_codebook_subset_restriction_r10->num_bits);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_codebook_subset_restriction_r10->value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_codebook_subset_restriction_r10->value[loop], "value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_antenna_info_dedicated_r10_len
(
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_antenna_info_dedicated_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_dedicated_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_dedicated_r10->transmission_mode);

    /* Optional element */
    if(p_rrc_antenna_info_dedicated_r10->bitmask & RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_codebook_subset_restriction_r10_len(&p_rrc_antenna_info_dedicated_r10->codebook_subset_restriction_r10);
    }

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_ue_transmit_antenna_selection_len(&p_rrc_antenna_info_dedicated_r10->ue_transmit_antenna_selection);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_dedicated_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_antenna_info_dedicated_r10 != PNULL);

    /* This function composes rrc_antenna_info_dedicated_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_antenna_info_dedicated_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_antenna_info_dedicated_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_dedicated_r10->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_dedicated_r10->transmission_mode] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_antenna_info_dedicated_r10->transmission_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_antenna_info_dedicated_r10->transmission_mode, "transmission_mode");
    *pp_buffer += sizeof(p_rrc_antenna_info_dedicated_r10->transmission_mode);

    /* Optional element */
    if(p_rrc_antenna_info_dedicated_r10->bitmask & RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_codebook_subset_restriction_r10(pp_buffer, &p_rrc_antenna_info_dedicated_r10->codebook_subset_restriction_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_ue_transmit_antenna_selection(pp_buffer, &p_rrc_antenna_info_dedicated_r10->ue_transmit_antenna_selection))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_antenna_info_r10_len
(
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_antenna_info_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_r10->bitmask);

    /* Optional element */
    if(p_rrc_antenna_info_r10->bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_antenna_info_dedicated_r10_len(&p_rrc_antenna_info_r10->explicit_value);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_antenna_info_r10 != PNULL);

    /* This function composes rrc_antenna_info_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_antenna_info_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_antenna_info_r10->bitmask);

    /* Optional element */
    if(p_rrc_antenna_info_r10->bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_antenna_info_dedicated_r10(pp_buffer, &p_rrc_antenna_info_r10->explicit_value))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_antenna_info_ul_r10_len
(
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_antenna_info_ul_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_ul_r10->bitmask);

    /* Optional element */
    if(p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_ul_r10->transmission_mode_ul);
    }

    /* Optional element */
    if(p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_antenna_info_ul_r10->four_antenna_port_activated);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_antenna_info_ul_r10
(
    U8  **pp_buffer,
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_antenna_info_ul_r10 != PNULL);

    /* This function composes rrc_antenna_info_ul_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_antenna_info_ul_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_antenna_info_ul_r10->bitmask);

    /* Optional element */
    if(p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_ul_r10->transmission_mode_ul > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_ul_r10->transmission_mode_ul] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_antenna_info_ul_r10->transmission_mode_ul);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_antenna_info_ul_r10->transmission_mode_ul, "transmission_mode_ul");
    *pp_buffer += sizeof(p_rrc_antenna_info_ul_r10->transmission_mode_ul);
    }

    /* Optional element */
    if(p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_ul_r10->four_antenna_port_activated > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_ul_r10->four_antenna_port_activated] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_antenna_info_ul_r10->four_antenna_port_activated);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_antenna_info_ul_r10->four_antenna_port_activated, "four_antenna_port_activated");
    *pp_buffer += sizeof(p_rrc_antenna_info_ul_r10->four_antenna_port_activated);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_len
(
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger1);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger2);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger != PNULL);

    /* This function composes rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger1, "trigger1");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger1);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger2, "trigger2");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger2);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_len
(
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10_setup != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic);

    /* Optional element */
    if(p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask & RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_len(&p_rrc_phy_cqi_report_aperiodic_r10_setup->aperiodic_csi_trigger);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10_setup != PNULL);

    /* This function composes rrc_phy_cqi_report_aperiodic_r10_setup */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic, "cqi_reporting_mode_aperiodic");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic);

    /* Optional element */
    if(p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask & RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger(pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10_setup->aperiodic_csi_trigger))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_len
(
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_aperiodic_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_aperiodic_r10->bitmask & RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_setup_len(&p_rrc_phy_cqi_report_aperiodic_r10->setup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_aperiodic_r10 != PNULL);

    /* This function composes rrc_phy_cqi_report_aperiodic_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_aperiodic_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_aperiodic_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_aperiodic_r10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_aperiodic_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_aperiodic_r10->bitmask & RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10_setup(pp_buffer, &p_rrc_phy_cqi_report_aperiodic_r10->setup))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_wideband_r10_len
(
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_format_ind_wideband_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_ind_wideband_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_wideband_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_wideband_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_format_ind_wideband_r10 != PNULL);

    /* This function composes rrc_phy_cqi_format_ind_wideband_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_format_ind_wideband_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_ind_wideband_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_wideband_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode, "csi_report_mode");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_subband_r10_len
(
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_format_ind_subband_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_ind_subband_r10->k);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_subband_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_format_ind_subband_r10 != PNULL);

    /* This function composes rrc_phy_cqi_format_ind_subband_r10 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_cqi_format_ind_subband_r10->k < 1) || (p_rrc_phy_cqi_format_ind_subband_r10->k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_subband_r10->k] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_subband_r10->k);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_format_ind_subband_r10->k, "k");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_ind_subband_r10->k);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor, "periodicy_factor");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_format_ind_periodic_r10_len
(
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_format_ind_periodic_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_format_ind_wideband_r10_len(&p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_wideband);
    }

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_format_ind_subband_r10_len(&p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_subband);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_format_ind_periodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_format_ind_periodic_r10 != PNULL);

    /* This function composes rrc_phy_cqi_format_ind_periodic_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_format_ind_periodic_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_format_ind_wideband_r10(pp_buffer, &p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_wideband))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_format_ind_subband_r10(pp_buffer, &p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_subband))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_len
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask & RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup != PNULL);

    /* This function composes rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2, "cqi_pmi_config_index2");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask & RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2, "ri_config_index2");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_len
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_len(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->setup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index != PNULL);

    /* This function composes rrc_phy_cqi_report_periodic_r10_setup_csi_config_index */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup(pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->setup))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_len
(
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_format_ind_periodic_r10_len(&p_rrc_phy_cqi_report_periodic_r10_setup->cqi_format_ind_periodic);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9);
    }

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_len(&p_rrc_phy_cqi_report_periodic_r10_setup->csi_config_index);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10_setup != PNULL);

    /* This function composes rrc_phy_cqi_report_periodic_r10_setup */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index > 1184))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index] should be less than"
            " or equal to 1184. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index, "cqi_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1 > 1184))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1] should be less than"
            " or equal to 1184. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1, "cqi_pucch_resource_index_p1");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index, "cqi_pmi_config_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_format_ind_periodic_r10(pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_format_ind_periodic))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index, "ri_config_index");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi, "simultaneous_ack_nack_cqi");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9, "cqi_mask_r9");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9);
    }

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index(pp_buffer, &p_rrc_phy_cqi_report_periodic_r10_setup->csi_config_index))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_periodic_r10_len
(
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_periodic_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_periodic_r10_setup_len(&p_rrc_phy_cqi_report_periodic_r10->setup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_periodic_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_periodic_r10 != PNULL);

    /* This function composes rrc_phy_cqi_report_periodic_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_periodic_r10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_periodic_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_periodic_r10->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_periodic_r10_setup(pp_buffer, &p_rrc_phy_cqi_report_periodic_r10->setup))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_fdd_r10_len
(
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_fdd_r10 != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_meas_subframe_pattern_fdd_r10->data);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_fdd_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_fdd_r10 != PNULL);

    /* This function composes rrc_phy_meas_subframe_pattern_fdd_r10 */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_fdd_r10->data); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_fdd_r10->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_tdd_r10_len
(
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_tdd_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10);
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10);
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_tdd_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_tdd_r10 != PNULL);

    /* This function composes rrc_phy_meas_subframe_pattern_tdd_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10[loop], "subframe_config1_5_r10[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10[loop], "subframe_config0_r10[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10[loop], "subframe_config6_r10[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_meas_subframe_pattern_r10_len
(
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_meas_subframe_pattern_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_meas_subframe_pattern_fdd_r10_len(&p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_fdd_r10);
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_meas_subframe_pattern_tdd_r10_len(&p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_tdd_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_meas_subframe_pattern_r10
(
    U8  **pp_buffer,
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_meas_subframe_pattern_r10 != PNULL);

    /* This function composes rrc_phy_meas_subframe_pattern_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_meas_subframe_pattern_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_meas_subframe_pattern_r10->bitmask);

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_meas_subframe_pattern_fdd_r10(pp_buffer, &p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_fdd_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_meas_subframe_pattern_tdd_r10(pp_buffer, &p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_tdd_r10))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_len
(
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup != PNULL);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_meas_subframe_pattern_r10_len(&p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set1_r10);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_meas_subframe_pattern_r10_len(&p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set2_r10);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup != PNULL);

    /* This function composes rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_meas_subframe_pattern_r10(pp_buffer, &p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set1_r10))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_meas_subframe_pattern_r10(pp_buffer, &p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set2_r10))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_cqi_report_csi_subframePattern_r10_len
(
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cqi_report_csi_subframePattern_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_len(&p_rrc_phy_cqi_report_csi_subframePattern_r10->csi_subframe_pattern_config_r10_setup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_cqi_report_csi_subframePattern_r10
(
    U8  **pp_buffer,
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cqi_report_csi_subframePattern_r10 != PNULL);

    /* This function composes rrc_phy_cqi_report_csi_subframePattern_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type);

    /* Optional element */
    if(p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup(pp_buffer, &p_rrc_phy_cqi_report_csi_subframePattern_r10->csi_subframe_pattern_config_r10_setup))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_cqi_report_config_r10_len
(
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_cqi_report_config_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_r10->bitmask);

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_aperiodic_r10_len(&p_rrc_cqi_report_config_r10->cqi_report_aperiodic_r10);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset);

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_periodic_r10_len(&p_rrc_cqi_report_config_r10->cqi_report_periodic_r10);
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10);
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_csi_subframePattern_r10_len(&p_rrc_cqi_report_config_r10->csi_subframePattern_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_cqi_report_config_r10
(
    U8  **pp_buffer,
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_cqi_report_config_r10 != PNULL);

    /* This function composes rrc_cqi_report_config_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_cqi_report_config_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_r10->bitmask);

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_aperiodic_r10(pp_buffer, &p_rrc_cqi_report_config_r10->cqi_report_aperiodic_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset < -1) || (p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset, "nomPDSCH_rs_epre_offset");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset);

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_periodic_r10(pp_buffer, &p_rrc_cqi_report_config_r10->cqi_report_periodic_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10, "cqi_report_pmi_ri_report_r10");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10);
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_csi_subframePattern_r10(pp_buffer, &p_rrc_cqi_report_config_r10->csi_subframePattern_r10))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_additional_spectrum_emission_ca_r10_len
(
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_additional_spectrum_emission_ca_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_additional_spectrum_emission_ca_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_additional_spectrum_emission_ca_r10->request_type);

    /* Optional element */
    if(p_rrc_additional_spectrum_emission_ca_r10->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_additional_spectrum_emission_ca_r10
(
    U8  **pp_buffer,
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_additional_spectrum_emission_ca_r10 != PNULL);

    /* This function composes rrc_additional_spectrum_emission_ca_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_additional_spectrum_emission_ca_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_additional_spectrum_emission_ca_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_additional_spectrum_emission_ca_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_additional_spectrum_emission_ca_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_additional_spectrum_emission_ca_r10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_additional_spectrum_emission_ca_r10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_additional_spectrum_emission_ca_r10->request_type);

    /* Optional element */
    if(p_rrc_additional_spectrum_emission_ca_r10->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell < 1) || (p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell, "additional_spectrum_emission_pcell");
    *pp_buffer += sizeof(p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_n1pucch_an_cs_r10_len
(
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_n1pucch_an_cs_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_n1pucch_an_cs_r10->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_n1pucch_an_cs_r10->count * sizeof(p_n1pucch_an_cs_r10->n1_pucch_an_cs[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_n1pucch_an_cs_r10
(
    U8  **pp_buffer,
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_n1pucch_an_cs_r10 != PNULL);

    /* This function composes n1pucch_an_cs_r10 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_n1pucch_an_cs_r10->count < 1) || (p_n1pucch_an_cs_r10->count > MAX_N1PUCCH_AN_CS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_n1pucch_an_cs_r10->count] should be in range "
            "1 to MAX_N1PUCCH_AN_CS. Incorrect value %u received.", p_n1pucch_an_cs_r10->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_n1pucch_an_cs_r10->count, "count");
    *pp_buffer += sizeof(p_n1pucch_an_cs_r10->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_n1pucch_an_cs_r10->count; loop++)
        {
            if (p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop] > 2047)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_n1pucch_an_cs_r10->n1_pucch_an_cs[%u] should be less"
                    " than or equal to 2047. "
                    "Incorrect value %d received.", loop, p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U16(*pp_buffer, &p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop], "n1_pucch_an_cs[]");
            *pp_buffer += sizeof(U16);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_n1pucch_an_cs_list_r10_len
(
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_n1pucch_an_cs_list_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_n1pucch_an_cs_list_r10->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_n1pucch_an_cs_list_r10->count < 1) || (p_n1pucch_an_cs_list_r10->count > MAX_N1PUCCH_AN_CS_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_n1pucch_an_cs_list_r10->count] should be in range "
            "1 to MAX_N1PUCCH_AN_CS_LIST. Incorrect value %d received.", p_n1pucch_an_cs_list_r10->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_n1pucch_an_cs_list_r10->count; loop++)
        {
            length += rrc_il_get_n1pucch_an_cs_r10_len(&p_n1pucch_an_cs_list_r10->n1_pucch_an_cs[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_n1pucch_an_cs_list_r10
(
    U8  **pp_buffer,
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_n1pucch_an_cs_list_r10 != PNULL);

    /* This function composes n1pucch_an_cs_list_r10 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_n1pucch_an_cs_list_r10->count < 1) || (p_n1pucch_an_cs_list_r10->count > MAX_N1PUCCH_AN_CS_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_n1pucch_an_cs_list_r10->count] should be in range "
            "1 to MAX_N1PUCCH_AN_CS_LIST. Incorrect value %u received.", p_n1pucch_an_cs_list_r10->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_n1pucch_an_cs_list_r10->count, "count");
    *pp_buffer += sizeof(p_n1pucch_an_cs_list_r10->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_n1pucch_an_cs_list_r10->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_n1pucch_an_cs_r10(pp_buffer, &p_n1pucch_an_cs_list_r10->n1_pucch_an_cs[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_channel_selection_n1_pucch_an_cs_r10_len
(
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_channel_selection_n1_pucch_an_cs_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type);

    /* Optional element */
    if(p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask & RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_n1pucch_an_cs_list_r10_len(&p_rrc_channel_selection_n1_pucch_an_cs_r10->n1pucch_an_cs_list_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_channel_selection_n1_pucch_an_cs_r10
(
    U8  **pp_buffer,
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_channel_selection_n1_pucch_an_cs_r10 != PNULL);

    /* This function composes rrc_channel_selection_n1_pucch_an_cs_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type);

    /* Optional element */
    if(p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask & RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_n1pucch_an_cs_list_r10(pp_buffer, &p_rrc_channel_selection_n1_pucch_an_cs_r10->n1pucch_an_cs_list_r10))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pucch_channel_selection_config_v1020_len
(
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pucch_channel_selection_config_v1020 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pucch_channel_selection_config_v1020->bitmask);

    /* Optional element */
    if(p_rrc_pucch_channel_selection_config_v1020->bitmask & RRC_N1PUCCH_AN_CS_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_channel_selection_n1_pucch_an_cs_r10_len(&p_rrc_pucch_channel_selection_config_v1020->channel_selection_n1_pucch_an_cs_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pucch_channel_selection_config_v1020
(
    U8  **pp_buffer,
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pucch_channel_selection_config_v1020 != PNULL);

    /* This function composes rrc_pucch_channel_selection_config_v1020 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pucch_channel_selection_config_v1020->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_pucch_channel_selection_config_v1020->bitmask);

    /* Optional element */
    if(p_rrc_pucch_channel_selection_config_v1020->bitmask & RRC_N1PUCCH_AN_CS_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_channel_selection_n1_pucch_an_cs_r10(pp_buffer, &p_rrc_pucch_channel_selection_config_v1020->channel_selection_n1_pucch_an_cs_r10))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_physical_config_dedicated_extended_len
(
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_physical_config_dedicated_extended != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_physical_config_dedicated_extended->bitmask);

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_additional_spectrum_emission_ca_r10_len(&p_rrc_phy_physical_config_dedicated_extended->additional_spectrum_emission_ca);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_pucch_channel_selection_config_v1020_len(&p_rrc_phy_physical_config_dedicated_extended->pucch_channel_selection_config_v1020);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_physical_config_dedicated_extended
(
    U8  **pp_buffer,
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_physical_config_dedicated_extended != PNULL);

    /* This function composes rrc_phy_physical_config_dedicated_extended */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_physical_config_dedicated_extended->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_physical_config_dedicated_extended->bitmask);

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_additional_spectrum_emission_ca_r10(pp_buffer, &p_rrc_phy_physical_config_dedicated_extended->additional_spectrum_emission_ca))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_pucch_channel_selection_config_v1020(pp_buffer, &p_rrc_phy_physical_config_dedicated_extended->pucch_channel_selection_config_v1020))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_physical_config_dedicated_len
(
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_physical_config_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_physical_config_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pdsch_configuration_dedicated_len(&p_rrc_phy_physical_config_dedicated->pdsch_configuration_dedicated);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pucch_configuration_dedicated_len(&p_rrc_phy_physical_config_dedicated->pucch_configuration_dedicated);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pusch_configuration_dedicated_len(&p_rrc_phy_physical_config_dedicated->pusch_configuration_dedicated);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_uplink_power_control_dedicated_len(&p_rrc_phy_physical_config_dedicated->uplink_power_control_dedicated);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_tpc_pdcch_configuration_len(&p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pucch);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_tpc_pdcch_configuration_len(&p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pusch);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_reporting_len(&p_rrc_phy_physical_config_dedicated->cqi_reporting);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_sounding_rs_ul_config_dedicated_len(&p_rrc_phy_physical_config_dedicated->sounding_rs_ul_config_dedicated);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_antenna_information_len(&p_rrc_phy_physical_config_dedicated->antenna_information);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_scheduling_request_config_len(&p_rrc_phy_physical_config_dedicated->scheduling_request_config);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_config_v920_len(&p_rrc_phy_physical_config_dedicated->cqi_report_config_v920);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_antenna_info_v920_len(&p_rrc_phy_physical_config_dedicated->antenna_info_v920);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_antenna_info_r10_len(&p_rrc_phy_physical_config_dedicated->antenna_info_r10);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_antenna_info_ul_r10_len(&p_rrc_phy_physical_config_dedicated->antenna_info_ul_r10);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_cqi_report_config_r10_len(&p_rrc_phy_physical_config_dedicated->cqi_report_config_r10);
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_physical_config_dedicated_extended_len(&p_rrc_phy_physical_config_dedicated->phy_physical_config_dedicated_extended);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_physical_config_dedicated
(
    U8  **pp_buffer,
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_physical_config_dedicated != PNULL);

    /* This function composes rrc_phy_physical_config_dedicated */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_physical_config_dedicated->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_physical_config_dedicated->bitmask);

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pdsch_configuration_dedicated(pp_buffer, &p_rrc_phy_physical_config_dedicated->pdsch_configuration_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pucch_configuration_dedicated(pp_buffer, &p_rrc_phy_physical_config_dedicated->pucch_configuration_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pusch_configuration_dedicated(pp_buffer, &p_rrc_phy_physical_config_dedicated->pusch_configuration_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_uplink_power_control_dedicated(pp_buffer, &p_rrc_phy_physical_config_dedicated->uplink_power_control_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_tpc_pdcch_configuration(pp_buffer, &p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pucch))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_tpc_pdcch_configuration(pp_buffer, &p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pusch))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_reporting(pp_buffer, &p_rrc_phy_physical_config_dedicated->cqi_reporting))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_sounding_rs_ul_config_dedicated(pp_buffer, &p_rrc_phy_physical_config_dedicated->sounding_rs_ul_config_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_antenna_information(pp_buffer, &p_rrc_phy_physical_config_dedicated->antenna_information))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_scheduling_request_config(pp_buffer, &p_rrc_phy_physical_config_dedicated->scheduling_request_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_config_v920(pp_buffer, &p_rrc_phy_physical_config_dedicated->cqi_report_config_v920))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_antenna_info_v920(pp_buffer, &p_rrc_phy_physical_config_dedicated->antenna_info_v920))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_antenna_info_r10(pp_buffer, &p_rrc_phy_physical_config_dedicated->antenna_info_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_antenna_info_ul_r10(pp_buffer, &p_rrc_phy_physical_config_dedicated->antenna_info_ul_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_cqi_report_config_r10(pp_buffer, &p_rrc_phy_physical_config_dedicated->cqi_report_config_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_physical_config_dedicated_extended(pp_buffer, &p_rrc_phy_physical_config_dedicated->phy_physical_config_dedicated_extended))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_scell_non_ul_config_len
(
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_radio_res_dedicated_scell_non_ul_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_antenna_info_dedicated_r10_len(&p_rrc_radio_res_dedicated_scell_non_ul_config->antenna_info_dedicated_r10);
    }

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_pdsch_configuration_dedicated_len(&p_rrc_radio_res_dedicated_scell_non_ul_config->pdsch_configuration_dedicated);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_scell_non_ul_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_radio_res_dedicated_scell_non_ul_config != PNULL);

    /* This function composes rrc_radio_res_dedicated_scell_non_ul_config */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_antenna_info_dedicated_r10(pp_buffer, &p_rrc_radio_res_dedicated_scell_non_ul_config->antenna_info_dedicated_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_pdsch_configuration_dedicated(pp_buffer, &p_rrc_radio_res_dedicated_scell_non_ul_config->pdsch_configuration_dedicated))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_cqi_report_config_scell_r10_len
(
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_cqi_report_config_scell_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_scell_r10->bitmask);

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset);

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_cqi_report_periodic_r10_len(&p_rrc_cqi_report_config_scell_r10->cqi_report_periodic_r10);
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_cqi_report_config_scell_r10
(
    U8  **pp_buffer,
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_cqi_report_config_scell_r10 != PNULL);

    /* This function composes rrc_cqi_report_config_scell_r10 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_cqi_report_config_scell_r10->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_scell_r10->bitmask);

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic, "cqi_reporting_mode_aperiodic");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset < -1) || (p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset, "nomPDSCH_RS_EPRE_Offset");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset);

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cqi_report_periodic_r10(pp_buffer, &p_rrc_cqi_report_config_scell_r10->cqi_report_periodic_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10, "cqi_report_pmi_ri_report_r10");
    *pp_buffer += sizeof(p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_scell_ul_config_len
(
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_radio_res_dedicated_scell_ul_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_radio_res_dedicated_scell_ul_config->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_ul_config->bitmask & RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_cqi_report_config_scell_r10_len(&p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_scell_ul_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_radio_res_dedicated_scell_ul_config != PNULL);

    /* This function composes rrc_radio_res_dedicated_scell_ul_config */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_radio_res_dedicated_scell_ul_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_radio_res_dedicated_scell_ul_config->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_dedicated_scell_ul_config->bitmask & RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_cqi_report_config_scell_r10(pp_buffer, &p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_physical_config_dedicated_scell_len
(
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_physical_config_dedicated_scell != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_physical_config_dedicated_scell->bitmask);

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_radio_res_dedicated_scell_non_ul_config_len(&p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_non_ul_config);
    }

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_radio_res_dedicated_scell_ul_config_len(&p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_ul_config);
    }

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_LAA_SCELL_CONFIGURATION_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len(&p_rrc_physical_config_dedicated_scell->laa_scell_configuration_r13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_physical_config_dedicated_scell
(
    U8  **pp_buffer,
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_physical_config_dedicated_scell != PNULL);

    /* This function composes rrc_physical_config_dedicated_scell */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_physical_config_dedicated_scell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_physical_config_dedicated_scell->bitmask);

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_dedicated_scell_non_ul_config(pp_buffer, &p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_non_ul_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_dedicated_scell_ul_config(pp_buffer, &p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_ul_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_physical_config_dedicated_scell->bitmask & RRM_LAA_SCELL_CONFIGURATION_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_dedicated_laa_scell_config(pp_buffer, &p_rrc_physical_config_dedicated_scell->laa_scell_configuration_r13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_radio_res_config_dedicated_scell_len
(
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_radio_res_config_dedicated_scell != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_radio_res_config_dedicated_scell->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_config_dedicated_scell->bitmask & RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_physical_config_dedicated_scell_len(&p_rrc_radio_res_config_dedicated_scell->physical_config_dedicated_scell);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_radio_res_config_dedicated_scell
(
    U8  **pp_buffer,
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_radio_res_config_dedicated_scell != PNULL);

    /* This function composes rrc_radio_res_config_dedicated_scell */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_radio_res_config_dedicated_scell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_radio_res_config_dedicated_scell->bitmask);

    /* Optional element */
    if(p_rrc_radio_res_config_dedicated_scell->bitmask & RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_physical_config_dedicated_scell(pp_buffer, &p_rrc_radio_res_config_dedicated_scell->physical_config_dedicated_scell))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_scell_config_len
(
    rrc_phy_scell_config_t *p_rrc_phy_scell_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_scell_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scell_config->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scell_config->cell_index);

    /* Optional element */
    if(p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_OPERATION_LIST_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scell_config->operation_type);
    }

    /* Optional element */
    if(p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_DEDICATED_SCELL_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_radio_res_config_dedicated_scell_len(&p_rrc_phy_scell_config->radio_res_config_dedicated_scell);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_scell_config
(
    U8  **pp_buffer,
    rrc_phy_scell_config_t *p_rrc_phy_scell_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_scell_config != PNULL);

    /* This function composes rrc_phy_scell_config */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_scell_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_scell_config->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scell_config->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_scell_config->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_scell_config->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_scell_config->cell_index);

    /* Optional element */
    if(p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_OPERATION_LIST_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scell_config->operation_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config->operation_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_phy_scell_config->operation_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_scell_config->operation_type, "operation_type");
    *pp_buffer += sizeof(p_rrc_phy_scell_config->operation_type);
    }

    /* Optional element */
    if(p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_DEDICATED_SCELL_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_config_dedicated_scell(pp_buffer, &p_rrc_phy_scell_config->radio_res_config_dedicated_scell))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_phy_scell_config_list_len
(
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_scell_config_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_scell_config_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_scell_config_list->count < 1) || (p_rrc_phy_scell_config_list->count > RRC_MAX_SCELL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config_list->count] should be in range "
            "1 to RRC_MAX_SCELL. Incorrect value %d received.", p_rrc_phy_scell_config_list->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_phy_scell_config_list->count; loop++)
        {
            length += rrc_il_get_rrc_phy_scell_config_len(&p_rrc_phy_scell_config_list->rrc_phy_scell_config[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_phy_scell_config_list
(
    U8  **pp_buffer,
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_scell_config_list != PNULL);

    /* This function composes rrc_phy_scell_config_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_scell_config_list->count < 1) || (p_rrc_phy_scell_config_list->count > RRC_MAX_SCELL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config_list->count] should be in range "
            "1 to RRC_MAX_SCELL. Incorrect value %u received.", p_rrc_phy_scell_config_list->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_scell_config_list->count, "count");
    *pp_buffer += sizeof(p_rrc_phy_scell_config_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_phy_scell_config_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_phy_scell_config(pp_buffer, &p_rrc_phy_scell_config_list->rrc_phy_scell_config[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMpdcchConfigSetupR13_len
(
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMpdcchConfigSetupR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMpdcchConfigSetupR13
(
    U8  **pp_buffer,
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMpdcchConfigSetupR13 != PNULL);

    /* This function composes RrcMpdcchConfigSetupR13 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13 > EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13] should be less than"
            " or equal to EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13, "mpdcchPdschHoppingConfigR13");
    *pp_buffer += sizeof(p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13 > EMTC_MPDCCH_START_SF_UESS_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_UESS_FDD_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13, "mpdcchStartSfUessFddR13");
    *pp_buffer += sizeof(p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13 > EMTC_MPDCCH_NUM_REPETITION_UESS_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_UESS_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13, "mpdcchNumRepetitionR13");
    *pp_buffer += sizeof(p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMpdcchConfigR13_len
(
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMpdcchConfigR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMpdcchConfigR13->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMpdcchConfigR13->mpdcchConfigType);

    /* Optional element */
    if(p_RrcMpdcchConfigR13->bitmask & RRC_MPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcMpdcchConfigSetupR13_len(&p_RrcMpdcchConfigR13->mpdcchConfigSetupR13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMpdcchConfigR13
(
    U8  **pp_buffer,
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMpdcchConfigR13 != PNULL);

    /* This function composes RrcMpdcchConfigR13 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMpdcchConfigR13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_RrcMpdcchConfigR13->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigR13->mpdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigR13->mpdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMpdcchConfigR13->mpdcchConfigType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMpdcchConfigR13->mpdcchConfigType, "mpdcchConfigType");
    *pp_buffer += sizeof(p_RrcMpdcchConfigR13->mpdcchConfigType);

    /* Optional element */
    if(p_RrcMpdcchConfigR13->bitmask & RRC_MPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcMpdcchConfigSetupR13(pp_buffer, &p_RrcMpdcchConfigR13->mpdcchConfigSetupR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcEpdcchSetConfigR11_len
(
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcEpdcchSetConfigR11 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEpdcchSetConfigR11->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEpdcchSetConfigR11->setConfigId);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEpdcchSetConfigR11->transmissionType);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEpdcchSetConfigR11->numPrbPairs);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEpdcchSetConfigR11->prbBitmap);

    /* Optional element */
    if(p_RrcEpdcchSetConfigR11->bitmask & RRC_MPDCCH_CONFIG_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcMpdcchConfigR13_len(&p_RrcEpdcchSetConfigR11->mpdcchConfigR13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcEpdcchSetConfigR11
(
    U8  **pp_buffer,
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcEpdcchSetConfigR11 != PNULL);

    /* This function composes RrcEpdcchSetConfigR11 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEpdcchSetConfigR11->bitmask, "bitmask");
    *pp_buffer += sizeof(p_RrcEpdcchSetConfigR11->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->setConfigId > EMTC_MAX_EPDCCH_CONFIG_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->setConfigId] should be less than"
            " or equal to EMTC_MAX_EPDCCH_CONFIG_ID. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->setConfigId);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEpdcchSetConfigR11->setConfigId, "setConfigId");
    *pp_buffer += sizeof(p_RrcEpdcchSetConfigR11->setConfigId);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->transmissionType > EMTC_TRANSMISSION_TYPE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->transmissionType] should be less than"
            " or equal to EMTC_TRANSMISSION_TYPE_MAX. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->transmissionType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEpdcchSetConfigR11->transmissionType, "transmissionType");
    *pp_buffer += sizeof(p_RrcEpdcchSetConfigR11->transmissionType);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->numPrbPairs > EMTC_NUM_PRB_PAIR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->numPrbPairs] should be less than"
            " or equal to EMTC_NUM_PRB_PAIR_MAX. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->numPrbPairs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEpdcchSetConfigR11->numPrbPairs, "numPrbPairs");
    *pp_buffer += sizeof(p_RrcEpdcchSetConfigR11->numPrbPairs);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->prbBitmap > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->prbBitmap] should be less than"
            " or equal to 63. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->prbBitmap);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEpdcchSetConfigR11->prbBitmap, "prbBitmap");
    *pp_buffer += sizeof(p_RrcEpdcchSetConfigR11->prbBitmap);

    /* Optional element */
    if(p_RrcEpdcchSetConfigR11->bitmask & RRC_MPDCCH_CONFIG_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcMpdcchConfigR13(pp_buffer, &p_RrcEpdcchSetConfigR11->mpdcchConfigR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_EpdcchConfigSetup_len
(
    EpdcchConfigSetup_t *p_EpdcchConfigSetup
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_EpdcchConfigSetup != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchConfigSetup->bitmask);

    /* Optional element */
    if(p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_START_SYMBOL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchConfigSetup->startSymbolR11);
    }

    /* Optional element */
    if(p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_ADD_MOD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcEpdcchSetConfigR11_len(&p_EpdcchConfigSetup->epdcchSetConfigR11);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_EpdcchConfigSetup
(
    U8  **pp_buffer,
    EpdcchConfigSetup_t *p_EpdcchConfigSetup
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_EpdcchConfigSetup != PNULL);

    /* This function composes EpdcchConfigSetup */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchConfigSetup->bitmask, "bitmask");
    *pp_buffer += sizeof(p_EpdcchConfigSetup->bitmask);

    /* Optional element */
    if(p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_START_SYMBOL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_EpdcchConfigSetup->startSymbolR11 < 1) || (p_EpdcchConfigSetup->startSymbolR11 > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchConfigSetup->startSymbolR11] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_EpdcchConfigSetup->startSymbolR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchConfigSetup->startSymbolR11, "startSymbolR11");
    *pp_buffer += sizeof(p_EpdcchConfigSetup->startSymbolR11);
    }

    /* Optional element */
    if(p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_ADD_MOD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcEpdcchSetConfigR11(pp_buffer, &p_EpdcchConfigSetup->epdcchSetConfigR11))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_EpdcchConfig_len
(
    EpdcchConfig_t *p_EpdcchConfig
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_EpdcchConfig != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchConfig->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchConfig->epdcchConfigType);

    /* Optional element */
    if(p_EpdcchConfig->bitmask & RRC_EPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_EpdcchConfigSetup_len(&p_EpdcchConfig->epdcchConfigSetup);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_EpdcchConfig
(
    U8  **pp_buffer,
    EpdcchConfig_t *p_EpdcchConfig
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_EpdcchConfig != PNULL);

    /* This function composes EpdcchConfig */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchConfig->bitmask, "bitmask");
    *pp_buffer += sizeof(p_EpdcchConfig->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchConfig->epdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchConfig->epdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_EpdcchConfig->epdcchConfigType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchConfig->epdcchConfigType, "epdcchConfigType");
    *pp_buffer += sizeof(p_EpdcchConfig->epdcchConfigType);

    /* Optional element */
    if(p_EpdcchConfig->bitmask & RRC_EPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_EpdcchConfigSetup(pp_buffer, &p_EpdcchConfig->epdcchConfigSetup))
    {
        return RRC_FAILURE;
    }
    }

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
rrc_il_get_RrcPucchConfigDedicatedR13_len
(
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPucchConfigDedicatedR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPucchConfigDedicatedR13->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPucchConfigDedicatedR13->type);

    /* Optional element */
    if(p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcAckNackRepetitionSetupR13_len(&p_RrcPucchConfigDedicatedR13->ackNackRepetitionSetup);
    }

    /* Optional element */
    if(p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_NUM_REPETITION_CE_FORMAT1_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPucchConfigDedicatedR13
(
    U8  **pp_buffer,
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPucchConfigDedicatedR13 != PNULL);

    /* This function composes RrcPucchConfigDedicatedR13 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPucchConfigDedicatedR13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_RrcPucchConfigDedicatedR13->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchConfigDedicatedR13->type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchConfigDedicatedR13->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcPucchConfigDedicatedR13->type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPucchConfigDedicatedR13->type, "type");
    *pp_buffer += sizeof(p_RrcPucchConfigDedicatedR13->type);

    /* Optional element */
    if(p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcAckNackRepetitionSetupR13(pp_buffer, &p_RrcPucchConfigDedicatedR13->ackNackRepetitionSetup))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_NUM_REPETITION_CE_FORMAT1_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX. Incorrect value %u received.", p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13, "pucchNumRepetitionCeFormat1r13");
    *pp_buffer += sizeof(p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPhysicalConfigDedicatedR13_len
(
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPhysicalConfigDedicatedR13 != PNULL);

    /* Get length of IE */
    length += rrc_il_get_EpdcchConfig_len(&p_RrcPhysicalConfigDedicatedR13->epdcchConfig);

    /* Get length of IE */
    length += rrc_il_get_RrcPucchConfigDedicatedR13_len(&p_RrcPhysicalConfigDedicatedR13->pucchConfigDedicatedR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPhysicalConfigDedicatedR13->ceMode);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPhysicalConfigDedicatedR13
(
    U8  **pp_buffer,
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPhysicalConfigDedicatedR13 != PNULL);

    /* This function composes RrcPhysicalConfigDedicatedR13 */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_EpdcchConfig(pp_buffer, &p_RrcPhysicalConfigDedicatedR13->epdcchConfig))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPucchConfigDedicatedR13(pp_buffer, &p_RrcPhysicalConfigDedicatedR13->pucchConfigDedicatedR13))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhysicalConfigDedicatedR13->ceMode > EMTC_CE_MODE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhysicalConfigDedicatedR13->ceMode] should be less than"
            " or equal to EMTC_CE_MODE_MAX. Incorrect value %u received.", p_RrcPhysicalConfigDedicatedR13->ceMode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPhysicalConfigDedicatedR13->ceMode, "ceMode");
    *pp_buffer += sizeof(p_RrcPhysicalConfigDedicatedR13->ceMode);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_create_ue_entity_cnf_len
(
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_create_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_create_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_create_ue_entity_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_create_ue_entity_cnf != PNULL);

    /* This function composes rrc_phy_create_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_create_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_create_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_create_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_create_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_create_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_create_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_create_ue_entity_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_create_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CREATE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_create_ue_entity_cnf
(
    rrc_phy_create_ue_entity_cnf_t  *p_rrc_phy_create_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_create_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_create_ue_entity_cnf_len(p_rrc_phy_create_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CREATE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CREATE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CREATE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_CREATE_UE_ENTITY_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_create_ue_entity_cnf(&p_phy_msg, p_rrc_phy_create_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_delete_ue_entity_req_len
(
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_delete_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_delete_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_delete_ue_entity_req != PNULL);

    /* This function composes rrc_phy_delete_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_delete_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_delete_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_delete_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_DELETE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_delete_ue_entity_req
(
    rrc_phy_delete_ue_entity_req_t  *p_rrc_phy_delete_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_delete_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_delete_ue_entity_req_len(p_rrc_phy_delete_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_DELETE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_DELETE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_DELETE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_DELETE_UE_ENTITY_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_delete_ue_entity_req(&p_phy_msg, p_rrc_phy_delete_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_delete_ue_entity_cnf_len
(
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_delete_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_delete_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_delete_ue_entity_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_delete_ue_entity_cnf != PNULL);

    /* This function composes rrc_phy_delete_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_delete_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_delete_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_delete_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_delete_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_delete_ue_entity_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_delete_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_DELETE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_delete_ue_entity_cnf
(
    rrc_phy_delete_ue_entity_cnf_t  *p_rrc_phy_delete_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_delete_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_delete_ue_entity_cnf_len(p_rrc_phy_delete_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_DELETE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_DELETE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_DELETE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_DELETE_UE_ENTITY_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_delete_ue_entity_cnf(&p_phy_msg, p_rrc_phy_delete_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_reconfig_ue_entity_req_len
(
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_req->ue_index);

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_physical_config_dedicated_len(&p_rrc_phy_reconfig_ue_entity_req->physical_config_dedicated);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_req->bitmask);

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_SCELL_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_phy_scell_config_list_len(&p_rrc_phy_reconfig_ue_entity_req->rrc_phy_scell_config_list);
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_RECONFIG_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_RrcPhysicalConfigDedicatedR13_len(&p_rrc_phy_reconfig_ue_entity_req->physicalConfigDedicatedR13);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_req != PNULL);

    /* This function composes rrc_phy_reconfig_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_req->ue_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_physical_config_dedicated(pp_buffer, &p_rrc_phy_reconfig_ue_entity_req->physical_config_dedicated))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_req->bitmask);

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_SCELL_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_scell_config_list(pp_buffer, &p_rrc_phy_reconfig_ue_entity_req->rrc_phy_scell_config_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_RECONFIG_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_RrcPhysicalConfigDedicatedR13(pp_buffer, &p_rrc_phy_reconfig_ue_entity_req->physicalConfigDedicatedR13))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_reconfig_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_RECONFIG_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_ue_entity_req
(
    rrc_phy_reconfig_ue_entity_req_t  *p_rrc_phy_reconfig_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_reconfig_ue_entity_req_len(p_rrc_phy_reconfig_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_RECONFIG_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_RECONFIG_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_RECONFIG_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_RECONFIG_UE_ENTITY_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_reconfig_ue_entity_req(&p_phy_msg, p_rrc_phy_reconfig_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_reconfig_ue_entity_cnf_len
(
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_cnf->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_cnf->response);

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_cnf->bitmask & RRC_PHY_FAIL_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_reconfig_ue_entity_cnf->fail_cause);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_cnf != PNULL);

    /* This function composes rrc_phy_reconfig_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_cnf->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_cnf->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_cnf->response);

    /* Optional element */
    if(p_rrc_phy_reconfig_ue_entity_cnf->bitmask & RRC_PHY_FAIL_CAUSE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_reconfig_ue_entity_cnf->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_rrc_phy_reconfig_ue_entity_cnf->fail_cause);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_reconfig_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_RECONFIG_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_ue_entity_cnf
(
    rrc_phy_reconfig_ue_entity_cnf_t  *p_rrc_phy_reconfig_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_reconfig_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_reconfig_ue_entity_cnf_len(p_rrc_phy_reconfig_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_RECONFIG_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_RECONFIG_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_RECONFIG_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_RECONFIG_UE_ENTITY_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_reconfig_ue_entity_cnf(&p_phy_msg, p_rrc_phy_reconfig_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_change_crnti_req_len
(
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_change_crnti_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_change_crnti_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_change_crnti_req->old_crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_change_crnti_req->new_crnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_change_crnti_req
(
    U8  **pp_buffer,
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_change_crnti_req != PNULL);

    /* This function composes rrc_phy_change_crnti_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_change_crnti_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_change_crnti_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_change_crnti_req->old_crnti, "old_crnti");
    *pp_buffer += sizeof(p_rrc_phy_change_crnti_req->old_crnti);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_change_crnti_req->new_crnti, "new_crnti");
    *pp_buffer += sizeof(p_rrc_phy_change_crnti_req->new_crnti);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_change_crnti_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CHANGE_CRNTI_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_change_crnti_req
(
    rrc_phy_change_crnti_req_t  *p_rrc_phy_change_crnti_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_change_crnti_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_change_crnti_req_len(p_rrc_phy_change_crnti_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CHANGE_CRNTI_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CHANGE_CRNTI_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CHANGE_CRNTI_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_CHANGE_CRNTI_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_change_crnti_req(&p_phy_msg, p_rrc_phy_change_crnti_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_change_crnti_cnf_len
(
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_change_crnti_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_change_crnti_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_change_crnti_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_change_crnti_cnf != PNULL);

    /* This function composes rrc_phy_change_crnti_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_change_crnti_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_phy_change_crnti_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_change_crnti_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_change_crnti_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_change_crnti_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_change_crnti_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_change_crnti_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_change_crnti_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CHANGE_CRNTI_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_change_crnti_cnf
(
    rrc_phy_change_crnti_cnf_t  *p_rrc_phy_change_crnti_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_change_crnti_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_change_crnti_cnf_len(p_rrc_phy_change_crnti_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CHANGE_CRNTI_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CHANGE_CRNTI_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CHANGE_CRNTI_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_CHANGE_CRNTI_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_change_crnti_cnf(&p_phy_msg, p_rrc_phy_change_crnti_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_cell_start_req_len
(
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cell_start_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cell_start_req->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_cell_start_req
(
    U8  **pp_buffer,
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cell_start_req != PNULL);

    /* This function composes rrc_phy_cell_start_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_start_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cell_start_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_cell_start_req->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_cell_start_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CELL_START_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_cell_start_req
(
    rrc_phy_cell_start_req_t  *p_rrc_phy_cell_start_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_cell_start_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_cell_start_req_len(p_rrc_phy_cell_start_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CELL_START_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CELL_START_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CELL_START_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_CELL_START_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cell_start_req(&p_phy_msg, p_rrc_phy_cell_start_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_cell_start_cnf_len
(
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cell_start_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cell_start_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cell_start_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_cell_start_cnf
(
    U8  **pp_buffer,
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cell_start_cnf != PNULL);

    /* This function composes rrc_phy_cell_start_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_start_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cell_start_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_cell_start_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cell_start_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_phy_cell_start_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_phy_cell_start_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_cell_start_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CELL_START_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_cell_start_cnf
(
    rrc_phy_cell_start_cnf_t  *p_rrc_phy_cell_start_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_cell_start_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_cell_start_cnf_len(p_rrc_phy_cell_start_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CELL_START_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CELL_START_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CELL_START_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_CELL_START_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cell_start_cnf(&p_phy_msg, p_rrc_phy_cell_start_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_cell_stop_req_len
(
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cell_stop_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cell_stop_req->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_cell_stop_req
(
    U8  **pp_buffer,
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cell_stop_req != PNULL);

    /* This function composes rrc_phy_cell_stop_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_stop_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_stop_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_stop_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cell_stop_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_cell_stop_req->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_cell_stop_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CELL_STOP_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_cell_stop_req
(
    rrc_phy_cell_stop_req_t  *p_rrc_phy_cell_stop_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_cell_stop_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_cell_stop_req_len(p_rrc_phy_cell_stop_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CELL_STOP_REQ", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CELL_STOP_REQ, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CELL_STOP_REQ, msg_length,cell_index);

    /* Fill RRC_PHY_CELL_STOP_REQ message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cell_stop_req(&p_phy_msg, p_rrc_phy_cell_stop_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_phy_cell_stop_cnf_len
(
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_phy_cell_stop_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_phy_cell_stop_cnf->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_phy_cell_stop_cnf
(
    U8  **pp_buffer,
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_phy_cell_stop_cnf != PNULL);

    /* This function composes rrc_phy_cell_stop_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_stop_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_stop_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_stop_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_phy_cell_stop_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_phy_cell_stop_cnf->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_phy_il_send_rrc_phy_cell_stop_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PHY_CELL_STOP_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_phy_il_send_rrc_phy_cell_stop_cnf
(
    rrc_phy_cell_stop_cnf_t  *p_rrc_phy_cell_stop_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_phy_msg;

    RRC_ASSERT(p_rrc_phy_cell_stop_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_phy_cell_stop_cnf_len(p_rrc_phy_cell_stop_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PHY_CELL_STOP_CNF", src_module_id, dst_module_id);

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
    p_phy_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_phy_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PHY_CELL_STOP_CNF, msg_api_length);

    /* Fill interface header */
    p_phy_msg = p_phy_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_phy_msg, transaction_id, src_module_id,
        RRC_PHY_MODULE_ID, RRC_PHY_CELL_STOP_CNF, msg_length,cell_index);

    /* Fill RRC_PHY_CELL_STOP_CNF message */
    p_phy_msg = p_phy_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_phy_cell_stop_cnf(&p_phy_msg, p_rrc_phy_cell_stop_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_phy_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

