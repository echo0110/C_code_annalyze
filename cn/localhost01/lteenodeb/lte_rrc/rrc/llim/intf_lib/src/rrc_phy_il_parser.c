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
 *  File Description : The file rrc_phy_il_parser.c contains the definitions 
 *                     of rrc-phy interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_phy_il_parser.h"
#include "rrc_phy_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_sounding_rs_ul_config_common_setup
(
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_sounding_rs_ul_config_common
(
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_tdd_config
(
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_prs_config
(
    rrc_phy_prs_config_t *p_rrc_phy_prs_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phys_dl_earfcn
(
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn,
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
rrc_il_parse_rrc_config_phy_cell_parameters
(
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_sync_signals
(
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_prach_configuration
(
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_ul_ref_signals_pusch
(
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_pusch_configuration
(
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_pucch_configuration
(
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_phich_configuration
(
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_pdsch_config_common
(
    pdsch_config_common_t *p_pdsch_config_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPhyPrachParametersCeR13
(
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPhyPrachParametersListCeR13
(
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPhyPrachConfigV1310
(
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310,
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
rrc_il_parse_rrc_phy_reconfig_cell_parameters
(
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_pdsch_configuration_dedicated
(
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_ack_nack_repetition_param
(
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_pucch_configuration_dedicated
(
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_pusch_configuration_dedicated
(
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_uplink_power_control_dedicated
(
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_tpc_index
(
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_tpc_pdcch_config_param
(
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_tpc_pdcch_configuration
(
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_subband_cqi_param_param
(
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_format_indicator_periodic
(
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_reporting_periodic_param
(
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_reporting_periodic
(
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_reporting
(
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated_param
(
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated
(
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_codebook_subset_restriction
(
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_ue_transmit_antenna_selection
(
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_antenna_information_dedicated
(
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_antenna_information
(
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_scheduling_request_config_param
(
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_scheduling_request_config
(
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_config_v920
(
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_codebook_subset_restriction_v920
(
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_antenna_info_v920
(
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_codebook_subset_restriction_r10
(
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_antenna_info_dedicated_r10
(
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_antenna_info_r10
(
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_antenna_info_ul_r10
(
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
(
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup
(
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10
(
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_format_ind_wideband_r10
(
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_format_ind_subband_r10
(
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_format_ind_periodic_r10
(
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup
(
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_periodic_r10
(
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_meas_subframe_pattern_fdd_r10
(
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_meas_subframe_pattern_tdd_r10
(
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_meas_subframe_pattern_r10
(
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
(
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_cqi_report_csi_subframePattern_r10
(
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_cqi_report_config_r10
(
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_additional_spectrum_emission_ca_r10
(
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_n1pucch_an_cs_r10
(
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_n1pucch_an_cs_list_r10
(
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_channel_selection_n1_pucch_an_cs_r10
(
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pucch_channel_selection_config_v1020
(
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_physical_config_dedicated_extended
(
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_physical_config_dedicated
(
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_radio_res_dedicated_scell_non_ul_config
(
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_cqi_report_config_scell_r10
(
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_radio_res_dedicated_scell_ul_config
(
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_physical_config_dedicated_scell
(
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_radio_res_config_dedicated_scell
(
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_scell_config
(
    rrc_phy_scell_config_t *p_rrc_phy_scell_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_phy_scell_config_list
(
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMpdcchConfigSetupR13
(
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcMpdcchConfigR13
(
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcEpdcchSetConfigR11
(
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_EpdcchConfigSetup
(
    EpdcchConfigSetup_t *p_EpdcchConfigSetup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_EpdcchConfig
(
    EpdcchConfig_t *p_EpdcchConfig,
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
rrc_il_parse_RrcPucchConfigDedicatedR13
(
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_RrcPhysicalConfigDedicatedR13
(
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrc_phy_config_cell_req
(
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_config_cell_req, 0, sizeof(rrc_phy_config_cell_req_t));

    /* This function parses rrc_phy_config_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_config_cell_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_config_cell_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_config_cell_req->cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_config_phy_cell_parameters(
        &p_rrc_phy_config_cell_req->cell_parameters,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_sync_signals(
        &p_rrc_phy_config_cell_req->sync_signals,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_prach_configuration(
        &p_rrc_phy_config_cell_req->prach_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pusch_configuration(
        &p_rrc_phy_config_cell_req->pusch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pucch_configuration(
        &p_rrc_phy_config_cell_req->pucch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_phich_configuration(
        &p_rrc_phy_config_cell_req->phich_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_pdsch_config_common(
        &p_rrc_phy_config_cell_req->pdsch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_phy_config_cell_req->bitmask & RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_config_cell_req->explicit_start_required, p_src + *p_length_read, "explicit_start_required");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_req->explicit_start_required > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_req->explicit_start_required] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_config_cell_req->explicit_start_required);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_config_cell_req->bitmask & RRC_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcPhyPrachConfigV1310(
        &p_rrc_phy_config_cell_req->prachConfigurationV1310,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_config_cell_req->bitmask & RRC_PHY_FREQUENCY_HOPPING_PARAMS_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcFrequancyHoppingParametersR13(
        &p_rrc_phy_config_cell_req->frequancyHoppingParametersR13,
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
rrc_il_parse_sounding_rs_ul_config_common_setup
(
    sounding_rs_ul_config_common_setup_t *p_sounding_rs_ul_config_common_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_sounding_rs_ul_config_common_setup, 0, sizeof(sounding_rs_ul_config_common_setup_t));

    /* This function parses sounding_rs_ul_config_common_setup */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_sounding_rs_ul_config_common_setup->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_sounding_rs_ul_config_common_setup->srs_bw_config, p_src + *p_length_read, "srs_bw_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_bw_config > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_bw_config] should be less than"
            " or equal to 7. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_bw_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_sounding_rs_ul_config_common_setup->srs_subframe_config, p_src + *p_length_read, "srs_subframe_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_subframe_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_subframe_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_subframe_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans, p_src + *p_length_read, "ack_nack_srs_simul_trans");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans] should be less than"
            " or equal to 1. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->ack_nack_srs_simul_trans);
        return RRC_FAILURE;
    }

    if (p_sounding_rs_ul_config_common_setup->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_sounding_rs_ul_config_common_setup->srs_max_up_pts, p_src + *p_length_read, "srs_max_up_pts");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_sounding_rs_ul_config_common_setup->srs_max_up_pts > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_sounding_rs_ul_config_common_setup->srs_max_up_pts] should be less than"
            " or equal to 0. Incorrect value %u received.", p_sounding_rs_ul_config_common_setup->srs_max_up_pts);
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
rrc_il_parse_sounding_rs_ul_config_common
(
    sounding_rs_ul_config_common_t *p_sounding_rs_ul_config_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_sounding_rs_ul_config_common, 0, sizeof(sounding_rs_ul_config_common_t));

    /* This function parses sounding_rs_ul_config_common */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_sounding_rs_ul_config_common->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U16);

    if (p_sounding_rs_ul_config_common->presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
    {

    if (RRC_FAILURE == rrc_il_parse_sounding_rs_ul_config_common_setup(
        &p_sounding_rs_ul_config_common->setup,
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
rrc_il_parse_rrc_phy_tdd_config
(
    rrc_phy_tdd_config_t *p_rrc_phy_tdd_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_tdd_config, 0, sizeof(rrc_phy_tdd_config_t));

    /* This function parses rrc_phy_tdd_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_tdd_config->sub_frame_assignment, p_src + *p_length_read, "sub_frame_assignment");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_tdd_config->sub_frame_assignment > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tdd_config->sub_frame_assignment] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_phy_tdd_config->sub_frame_assignment);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_tdd_config->special_sub_frame_pattern, p_src + *p_length_read, "special_sub_frame_pattern");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_tdd_config->special_sub_frame_pattern > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tdd_config->special_sub_frame_pattern] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_phy_tdd_config->special_sub_frame_pattern);
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
rrc_il_parse_rrc_phy_prs_config
(
    rrc_phy_prs_config_t *p_rrc_phy_prs_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_prs_config, 0, sizeof(rrc_phy_prs_config_t));

    /* This function parses rrc_phy_prs_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prs_config->prs_bandwidth, p_src + *p_length_read, "prs_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_prs_config->prs_bandwidth < 6) || (p_rrc_phy_prs_config->prs_bandwidth > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prs_config->prs_bandwidth] should be in range "
            "6 to 100. Incorrect value %u received.", p_rrc_phy_prs_config->prs_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prs_config->prs_cyclic_prefix_type, p_src + *p_length_read, "prs_cyclic_prefix_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prs_config->prs_cyclic_prefix_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prs_config->prs_cyclic_prefix_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_prs_config->prs_cyclic_prefix_type);
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
rrc_il_parse_rrc_phys_dl_earfcn
(
    rrc_phys_dl_earfcn_t *p_rrc_phys_dl_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phys_dl_earfcn, 0, sizeof(rrc_phys_dl_earfcn_t));

    /* This function parses rrc_phys_dl_earfcn */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phys_dl_earfcn->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phys_dl_earfcn->count > RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phys_dl_earfcn->count] should be less than"
            " or equal to RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT. Incorrect value %u received.", p_rrc_phys_dl_earfcn->count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrc_phys_dl_earfcn->count * sizeof(p_rrc_phys_dl_earfcn->earfcnDl[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_phys_dl_earfcn->count; loop++)
        {
            rrc_cp_unpack_U32((void*)&p_rrc_phys_dl_earfcn->earfcnDl[loop], (void*)(p_src + *p_length_read), "earfcnDl[]");
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
rrc_il_parse_rrc_config_phy_cell_parameters
(
    rrc_config_phy_cell_parameters_t *p_rrc_config_phy_cell_parameters,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_config_phy_cell_parameters, 0, sizeof(rrc_config_phy_cell_parameters_t));

    /* This function parses rrc_config_phy_cell_parameters */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_config_phy_cell_parameters->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->duplexing_mode, p_src + *p_length_read, "duplexing_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->duplexing_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->duplexing_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->duplexing_mode);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->freq_band_indicator, p_src + *p_length_read, "freq_band_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_config_phy_cell_parameters->freq_band_indicator < 1) || (p_rrc_config_phy_cell_parameters->freq_band_indicator > MAX_FREQ_BAND_INDICATOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->freq_band_indicator] should be in range "
            "1 to MAX_FREQ_BAND_INDICATOR. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->freq_band_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_config_phy_cell_parameters->ul_earfcn, p_src + *p_length_read, "ul_earfcn");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_config_phy_cell_parameters->dl_earfcn, p_src + *p_length_read, "dl_earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->num_of_antennas, p_src + *p_length_read, "num_of_antennas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->num_of_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->num_of_antennas] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->num_of_antennas);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->ul_tx_bandwidth, p_src + *p_length_read, "ul_tx_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->ul_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->ul_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->ul_tx_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->dl_tx_bandwidth, p_src + *p_length_read, "dl_tx_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->dl_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->dl_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->dl_tx_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->subcarrier_spacing, p_src + *p_length_read, "subcarrier_spacing");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->subcarrier_spacing > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->subcarrier_spacing] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->subcarrier_spacing);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->ul_cyclic_prefix, p_src + *p_length_read, "ul_cyclic_prefix");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->ul_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->ul_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->ul_cyclic_prefix);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->dl_cyclic_prefix, p_src + *p_length_read, "dl_cyclic_prefix");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->dl_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->dl_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->dl_cyclic_prefix);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_sounding_rs_ul_config_common(
        &p_rrc_config_phy_cell_parameters->srs_bandwidth_configuration,
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
    rrc_cp_unpack_U16(&p_rrc_config_phy_cell_parameters->phys_cell_id, p_src + *p_length_read, "phys_cell_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->phys_cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_config_phy_cell_parameters->pcfich_power_offset, p_src + *p_length_read, "pcfich_power_offset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->pcfich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->pcfich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->pcfich_power_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_config_phy_cell_parameters->phich_power_offset, p_src + *p_length_read, "phich_power_offset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->phich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->phich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->phich_power_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_config_phy_cell_parameters->max_rs_epre, p_src + *p_length_read, "max_rs_epre");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_config_phy_cell_parameters->max_rs_epre > 160))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_config_phy_cell_parameters->max_rs_epre] should be less than"
            " or equal to 160. Incorrect value %u received.", p_rrc_config_phy_cell_parameters->max_rs_epre);
        return RRC_FAILURE;
    }

    if (p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_TDD_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_tdd_config(
        &p_rrc_config_phy_cell_parameters->tdd_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_PRS_CONFIG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_prs_config(
        &p_rrc_config_phy_cell_parameters->prs_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DL_EARFCN_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phys_dl_earfcn(
        &p_rrc_config_phy_cell_parameters->dl_earfcn_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_LAA_SCELL_CONFIGURATION_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_dedicated_laa_scell_config(
        &p_rrc_config_phy_cell_parameters->laa_scell_configuration_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_config_phy_cell_parameters->bitmask & RRC_PHY_DMTC_CONFIGURATION_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_dmtc_configuration_r13(
        &p_rrc_config_phy_cell_parameters->dmtc_configuration_r13,
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
rrc_il_parse_rrc_phy_sync_signals
(
    rrc_phy_sync_signals_t *p_rrc_phy_sync_signals,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_sync_signals, 0, sizeof(rrc_phy_sync_signals_t));

    /* This function parses rrc_phy_sync_signals */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_phy_sync_signals->prim_syn_signal_power, p_src + *p_length_read, "prim_syn_signal_power");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_sync_signals->prim_syn_signal_power < -6) || (p_rrc_phy_sync_signals->prim_syn_signal_power > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->prim_syn_signal_power] should be in range "
            "-6 to 4. Incorrect value %d received.", p_rrc_phy_sync_signals->prim_syn_signal_power);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_phy_sync_signals->sec_syn_signal_power, p_src + *p_length_read, "sec_syn_signal_power");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_power < -6) || (p_rrc_phy_sync_signals->sec_syn_signal_power > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_power] should be in range "
            "-6 to 4. Incorrect value %d received.", p_rrc_phy_sync_signals->sec_syn_signal_power);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sync_signals->sec_syn_signal_m_seq1, p_src + *p_length_read, "sec_syn_signal_m_seq1");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_m_seq1 > 30))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_m_seq1] should be less than"
            " or equal to 30. Incorrect value %u received.", p_rrc_phy_sync_signals->sec_syn_signal_m_seq1);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sync_signals->sec_syn_signal_m_seq2, p_src + *p_length_read, "sec_syn_signal_m_seq2");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sync_signals->sec_syn_signal_m_seq2 > 30))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sync_signals->sec_syn_signal_m_seq2] should be less than"
            " or equal to 30. Incorrect value %u received.", p_rrc_phy_sync_signals->sec_syn_signal_m_seq2);
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
rrc_il_parse_rrc_phy_prach_configuration
(
    rrc_phy_prach_configuration_t *p_rrc_phy_prach_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_prach_configuration, 0, sizeof(rrc_phy_prach_configuration_t));

    /* This function parses rrc_phy_prach_configuration */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prach_configuration->prach_config_sequence, p_src + *p_length_read, "prach_config_sequence");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->prach_config_sequence > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->prach_config_sequence] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_phy_prach_configuration->prach_config_sequence);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_prach_configuration->root_seq_index, p_src + *p_length_read, "root_seq_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->root_seq_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->root_seq_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_rrc_phy_prach_configuration->root_seq_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prach_configuration->prach_freq_offset, p_src + *p_length_read, "prach_freq_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->prach_freq_offset > 104))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->prach_freq_offset] should be less than"
            " or equal to 104. Incorrect value %u received.", p_rrc_phy_prach_configuration->prach_freq_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prach_configuration->zero_cor_zone_config, p_src + *p_length_read, "zero_cor_zone_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->zero_cor_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->zero_cor_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_prach_configuration->zero_cor_zone_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_prach_configuration->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_prach_configuration->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_prach_configuration->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_prach_configuration->high_speed_flag);
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
rrc_il_parse_ul_ref_signals_pusch
(
    ul_ref_signals_pusch_t *p_ul_ref_signals_pusch,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_ul_ref_signals_pusch, 0, sizeof(ul_ref_signals_pusch_t));

    /* This function parses ul_ref_signals_pusch */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_ul_ref_signals_pusch->group_hopping_enabled, p_src + *p_length_read, "group_hopping_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->group_hopping_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->group_hopping_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_ul_ref_signals_pusch->group_hopping_enabled);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_ul_ref_signals_pusch->group_assign_pusch, p_src + *p_length_read, "group_assign_pusch");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->group_assign_pusch > 29))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->group_assign_pusch] should be less than"
            " or equal to 29. Incorrect value %u received.", p_ul_ref_signals_pusch->group_assign_pusch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_ul_ref_signals_pusch->seq_hopping_enabled, p_src + *p_length_read, "seq_hopping_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->seq_hopping_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->seq_hopping_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_ul_ref_signals_pusch->seq_hopping_enabled);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_ul_ref_signals_pusch->cyclic_shift, p_src + *p_length_read, "cyclic_shift");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_ref_signals_pusch->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_ul_ref_signals_pusch->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_ul_ref_signals_pusch->cyclic_shift);
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
rrc_il_parse_rrc_phy_pusch_configuration
(
    rrc_phy_pusch_configuration_t *p_rrc_phy_pusch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_pusch_configuration, 0, sizeof(rrc_phy_pusch_configuration_t));

    /* This function parses rrc_phy_pusch_configuration */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration->pusch_hopping_offset, p_src + *p_length_read, "pusch_hopping_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration->pusch_hopping_offset > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->pusch_hopping_offset] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_phy_pusch_configuration->pusch_hopping_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration->num_of_sub_bands, p_src + *p_length_read, "num_of_sub_bands");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_pusch_configuration->num_of_sub_bands < 1) || (p_rrc_phy_pusch_configuration->num_of_sub_bands > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->num_of_sub_bands] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_pusch_configuration->num_of_sub_bands);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration->pusch_hopping_mode, p_src + *p_length_read, "pusch_hopping_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration->pusch_hopping_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration->pusch_hopping_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_pusch_configuration->pusch_hopping_mode);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_ul_ref_signals_pusch(
        &p_rrc_phy_pusch_configuration->ul_ref_signal,
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
rrc_il_parse_rrc_phy_pucch_configuration
(
    rrc_phy_pucch_configuration_t *p_rrc_phy_pucch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_pucch_configuration, 0, sizeof(rrc_phy_pucch_configuration_t));

    /* This function parses rrc_phy_pucch_configuration */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pucch_configuration->delta_pucch_shift, p_src + *p_length_read, "delta_pucch_shift");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_pucch_configuration->delta_pucch_shift < 1) || (p_rrc_phy_pucch_configuration->delta_pucch_shift > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->delta_pucch_shift] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_phy_pucch_configuration->delta_pucch_shift);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pucch_configuration->nrb_cqi, p_src + *p_length_read, "nrb_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->nrb_cqi > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->nrb_cqi] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_phy_pucch_configuration->nrb_cqi);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pucch_configuration->ncs_an, p_src + *p_length_read, "ncs_an");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->ncs_an > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->ncs_an] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_pucch_configuration->ncs_an);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_pucch_configuration->n1pucch_an, p_src + *p_length_read, "n1pucch_an");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration->n1pucch_an > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration->n1pucch_an] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_pucch_configuration->n1pucch_an);
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
rrc_il_parse_rrc_phy_phich_configuration
(
    rrc_phy_phich_configuration_t *p_rrc_phy_phich_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_phich_configuration, 0, sizeof(rrc_phy_phich_configuration_t));

    /* This function parses rrc_phy_phich_configuration */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_phich_configuration->phich_resource, p_src + *p_length_read, "phich_resource");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_phich_configuration->phich_resource > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_phich_configuration->phich_resource] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_phich_configuration->phich_resource);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_phich_configuration->phich_duration, p_src + *p_length_read, "phich_duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_phich_configuration->phich_duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_phich_configuration->phich_duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_phich_configuration->phich_duration);
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
rrc_il_parse_pdsch_config_common
(
    pdsch_config_common_t *p_pdsch_config_common,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_pdsch_config_common, 0, sizeof(pdsch_config_common_t));

    /* This function parses pdsch_config_common */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_pdsch_config_common->ref_signal_power, p_src + *p_length_read, "ref_signal_power");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_pdsch_config_common->ref_signal_power < -60) || (p_pdsch_config_common->ref_signal_power > 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pdsch_config_common->ref_signal_power] should be in range "
            "-60 to 50. Incorrect value %d received.", p_pdsch_config_common->ref_signal_power);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pdsch_config_common->pb, p_src + *p_length_read, "pb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_pdsch_config_common->pb > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pdsch_config_common->pb] should be less than"
            " or equal to 3. Incorrect value %u received.", p_pdsch_config_common->pb);
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
rrc_il_parse_RrcPhyPrachParametersCeR13
(
    RrcPhyPrachParametersCeR13_t *p_RrcPhyPrachParametersCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcPhyPrachParametersCeR13, 0, sizeof(RrcPhyPrachParametersCeR13_t));

    /* This function parses RrcPhyPrachParametersCeR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->ceLevel, p_src + *p_length_read, "ceLevel");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->ceLevel > EMTC_CE_LEVEL_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->ceLevel] should be less than"
            " or equal to EMTC_CE_LEVEL_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->ceLevel);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->prachConfigIndexR13, p_src + *p_length_read, "prachConfigIndexR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachConfigIndexR13 > EMTC_MAX_PRACH_CONFIG_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachConfigIndexR13] should be less than"
            " or equal to EMTC_MAX_PRACH_CONFIG_INDEX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachConfigIndexR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13, p_src + *p_length_read, "prachFreqOffsetR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13 > EMTC_MAX_PRACH_FREQ_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_FREQ_OFFSET. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachFreqOffsetR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13, p_src + *p_length_read, "numRepetitionPerPreambleAttemptR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13 > EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13] should be less than"
            " or equal to EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13, p_src + *p_length_read, "prachHoppingConfigR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13 > EMTC_PRACH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13] should be less than"
            " or equal to EMTC_PRACH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachHoppingConfigR13);
        return RRC_FAILURE;
    }

    if (p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_HOPPING_OFFSET_R13_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13, p_src + *p_length_read, "prachHoppingOffsetR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13 > EMTC_MAX_PRACH_HOPPING_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_HOPPING_OFFSET. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachHoppingOffsetR13);
        return RRC_FAILURE;
    }
    }

    if (p_RrcPhyPrachParametersCeR13->bitmask & RRC_PHY_PRACH_START_SF_R13_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersCeR13->prachStartSfR13, p_src + *p_length_read, "prachStartSfR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhyPrachParametersCeR13->prachStartSfR13 > EMTC_PRACH_STARTING_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersCeR13->prachStartSfR13] should be less than"
            " or equal to EMTC_PRACH_STARTING_SF_MAX. Incorrect value %u received.", p_RrcPhyPrachParametersCeR13->prachStartSfR13);
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
rrc_il_parse_RrcPhyPrachParametersListCeR13
(
    RrcPhyPrachParametersListCeR13_t *p_RrcPhyPrachParametersListCeR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcPhyPrachParametersListCeR13, 0, sizeof(RrcPhyPrachParametersListCeR13_t));

    /* This function parses RrcPhyPrachParametersListCeR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPhyPrachParametersListCeR13->numCeLevels, p_src + *p_length_read, "numCeLevels");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcPhyPrachParametersListCeR13->numCeLevels < 1) || (p_RrcPhyPrachParametersListCeR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhyPrachParametersListCeR13->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcPhyPrachParametersListCeR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcPhyPrachParametersListCeR13->numCeLevels; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_RrcPhyPrachParametersCeR13(
                &p_RrcPhyPrachParametersListCeR13->prachParametersCeR13[loop],
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
rrc_il_parse_RrcPhyPrachConfigV1310
(
    RrcPhyPrachConfigV1310_t *p_RrcPhyPrachConfigV1310,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcPhyPrachConfigV1310, 0, sizeof(RrcPhyPrachConfigV1310_t));

    /* This function parses RrcPhyPrachConfigV1310 */

    if (RRC_FAILURE == rrc_il_parse_RrcPhyPrachParametersListCeR13(
        &p_RrcPhyPrachConfigV1310->prachParametersListCe,
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
rrc_il_parse_rrc_phy_config_cell_cnf
(
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_config_cell_cnf, 0, sizeof(rrc_phy_config_cell_cnf_t));

    /* This function parses rrc_phy_config_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_config_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_config_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_config_cell_cnf->response);
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
rrc_il_parse_rrc_phy_reconfig_cell_req
(
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_reconfig_cell_req, 0, sizeof(rrc_phy_reconfig_cell_req_t));

    /* This function parses rrc_phy_reconfig_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_reconfig_cell_req->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_req->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_req->sf, p_src + *p_length_read, "sf");
    *p_length_read += sizeof(U8);

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_reconfig_cell_parameters(
        &p_rrc_phy_reconfig_cell_req->phy_recfg_cell_parameters,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_sync_signals(
        &p_rrc_phy_reconfig_cell_req->sync_signals,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_prach_configuration(
        &p_rrc_phy_reconfig_cell_req->prach_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pusch_configuration(
        &p_rrc_phy_reconfig_cell_req->pusch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pucch_configuration(
        &p_rrc_phy_reconfig_cell_req->pucch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_phich_configuration(
        &p_rrc_phy_reconfig_cell_req->phich_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_pdsch_config_common(
        &p_rrc_phy_reconfig_cell_req->pdsch_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_PRACH_CONFIG_V1310_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcPhyPrachConfigV1310(
        &p_rrc_phy_reconfig_cell_req->prachConfigurationV1310,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_req->bitmask & RRC_RECONFIG_PHY_FREQUANCY_HOPPING_PARAMS_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcFrequancyHoppingParametersR13(
        &p_rrc_phy_reconfig_cell_req->frequancyHoppingParametersR13,
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
rrc_il_parse_rrc_phy_reconfig_cell_parameters
(
    rrc_phy_reconfig_cell_parameters_t *p_rrc_phy_reconfig_cell_parameters,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_reconfig_cell_parameters, 0, sizeof(rrc_phy_reconfig_cell_parameters_t));

    /* This function parses rrc_phy_reconfig_cell_parameters */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DUPLEX_MODE)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->duplexing_mode, p_src + *p_length_read, "duplexing_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->duplexing_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->duplexing_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->duplexing_mode);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_EARFCN)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->ul_earfcn, p_src + *p_length_read, "ul_earfcn");
    *p_length_read += sizeof(U16);
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_EARFCN)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_phy_reconfig_cell_parameters->dl_earfcn, p_src + *p_length_read, "dl_earfcn");
    *p_length_read += sizeof(U32);
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_NUM_OF_ANTENNAS)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->num_of_antennas, p_src + *p_length_read, "num_of_antennas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_reconfig_cell_parameters->num_of_antennas < 1) || (p_rrc_phy_reconfig_cell_parameters->num_of_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->num_of_antennas] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->num_of_antennas);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth, p_src + *p_length_read, "ul_tx_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->ul_tx_bandwidth);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth, p_src + *p_length_read, "dl_tx_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->dl_tx_bandwidth);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix, p_src + *p_length_read, "ul_cyclic_prefix");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->ul_cyclic_prefix);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix, p_src + *p_length_read, "dl_cyclic_prefix");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->dl_cyclic_prefix);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_sounding_rs_ul_config_common(
        &p_rrc_phy_reconfig_cell_parameters->srs_bandwidth_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_CELL_ID)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->phys_cell_id, p_src + *p_length_read, "phys_cell_id");
    *p_length_read += sizeof(U16);
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PCFICH_POWER_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset, p_src + *p_length_read, "pcfich_power_offset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->pcfich_power_offset);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PHICH_POWER_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->phich_power_offset, p_src + *p_length_read, "phich_power_offset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->phich_power_offset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->phich_power_offset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->phich_power_offset);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers, p_src + *p_length_read, "pri_sync_sig_epre_eprers");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->pri_sync_sig_epre_eprers);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers, p_src + *p_length_read, "sec_sync_sig_epre_eprers");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->sec_sync_sig_epre_eprers);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_parameters->max_rs_epre, p_src + *p_length_read, "max_rs_epre");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_parameters->max_rs_epre > 160))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_parameters->max_rs_epre] should be less than"
            " or equal to 160. Incorrect value %u received.", p_rrc_phy_reconfig_cell_parameters->max_rs_epre);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_reconfig_cell_parameters->bitmask & RRC_RECONFIG_PHY_PRS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_prs_config(
        &p_rrc_phy_reconfig_cell_parameters->prs_reconfig,
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
rrc_il_parse_rrc_phy_reconfig_cell_cnf
(
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_reconfig_cell_cnf, 0, sizeof(rrc_phy_reconfig_cell_cnf_t));

    /* This function parses rrc_phy_reconfig_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_reconfig_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_cell_cnf->response);
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
rrc_il_parse_rrc_phy_delete_cell_req
(
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_delete_cell_req, 0, sizeof(rrc_phy_delete_cell_req_t));

    /* This function parses rrc_phy_delete_cell_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_delete_cell_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_cell_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_cell_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_delete_cell_req->cell_index);
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
rrc_il_parse_rrc_phy_delete_cell_cnf
(
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_delete_cell_cnf, 0, sizeof(rrc_phy_delete_cell_cnf_t));

    /* This function parses rrc_phy_delete_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_delete_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_delete_cell_cnf->cell_index);
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
rrc_il_parse_rrc_phy_create_ue_entity_req
(
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_create_ue_entity_req, 0, sizeof(rrc_phy_create_ue_entity_req_t));

    /* This function parses rrc_phy_create_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_create_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_create_ue_entity_req->rnti, p_src + *p_length_read, "rnti");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_create_ue_entity_req->rnti < 1) || (p_rrc_phy_create_ue_entity_req->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_create_ue_entity_req->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_phy_create_ue_entity_req->rnti);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_physical_config_dedicated(
        &p_rrc_phy_create_ue_entity_req->physical_config_dedicated,
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
    rrc_cp_unpack_U16(&p_rrc_phy_create_ue_entity_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_SCELL_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_scell_config_list(
        &p_rrc_phy_create_ue_entity_req->rrc_phy_scell_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_create_ue_entity_req->bitmask & RRC_PHY_CREATE_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcPhysicalConfigDedicatedR13(
        &p_rrc_phy_create_ue_entity_req->physicalConfigDedicatedR13,
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
rrc_il_parse_rrc_phy_pdsch_configuration_dedicated
(
    rrc_phy_pdsch_configuration_dedicated_t *p_rrc_phy_pdsch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_pdsch_configuration_dedicated, 0, sizeof(rrc_phy_pdsch_configuration_dedicated_t));

    /* This function parses rrc_phy_pdsch_configuration_dedicated */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pdsch_configuration_dedicated->p_a, p_src + *p_length_read, "p_a");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pdsch_configuration_dedicated->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pdsch_configuration_dedicated->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_pdsch_configuration_dedicated->p_a);
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
rrc_il_parse_rrc_phy_ack_nack_repetition_param
(
    rrc_phy_ack_nack_repetition_param_t *p_rrc_phy_ack_nack_repetition_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_ack_nack_repetition_param, 0, sizeof(rrc_phy_ack_nack_repetition_param_t));

    /* This function parses rrc_phy_ack_nack_repetition_param */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_ack_nack_repetition_param->factor, p_src + *p_length_read, "factor");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ack_nack_repetition_param->factor > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ack_nack_repetition_param->factor] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_phy_ack_nack_repetition_param->factor);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_ack_nack_repetition_param->an_rep, p_src + *p_length_read, "an_rep");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ack_nack_repetition_param->an_rep > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ack_nack_repetition_param->an_rep] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_ack_nack_repetition_param->an_rep);
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
rrc_il_parse_rrc_phy_pucch_configuration_dedicated
(
    rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_pucch_configuration_dedicated, 0, sizeof(rrc_phy_pucch_configuration_dedicated_t));

    /* This function parses rrc_phy_pucch_configuration_dedicated */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_pucch_configuration_dedicated->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_ack_nack_repetition_param(
        &p_rrc_phy_pucch_configuration_dedicated->ack_nack_repetition_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_pucch_configuration_dedicated->bitmask & RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode, p_src + *p_length_read, "tdd_ack_nack_feedback_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_pucch_configuration_dedicated->tdd_ack_nack_feedback_mode);
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
rrc_il_parse_rrc_phy_pusch_configuration_dedicated
(
    rrc_phy_pusch_configuration_dedicated_t *p_rrc_phy_pusch_configuration_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_pusch_configuration_dedicated, 0, sizeof(rrc_phy_pusch_configuration_dedicated_t));

    /* This function parses rrc_phy_pusch_configuration_dedicated */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index, p_src + *p_length_read, "beta_offset_ack_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_ack_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index, p_src + *p_length_read, "beta_offset_ri_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_ri_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index, p_src + *p_length_read, "beta_offset_cqi_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_pusch_configuration_dedicated->beta_offset_cqi_index);
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
rrc_il_parse_rrc_phy_uplink_power_control_dedicated
(
    rrc_phy_uplink_power_control_dedicated_t *p_rrc_phy_uplink_power_control_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_uplink_power_control_dedicated, 0, sizeof(rrc_phy_uplink_power_control_dedicated_t));

    /* This function parses rrc_phy_uplink_power_control_dedicated */

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch, p_src + *p_length_read, "p0_ue_pusch");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch < -8) || (p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_phy_uplink_power_control_dedicated->p0_ue_pusch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled, p_src + *p_length_read, "delta_mcs_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->delta_mcs_enabled);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled, p_src + *p_length_read, "accumulation_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->accumulation_enabled);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch, p_src + *p_length_read, "p0_ue_pucch");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch < -8) || (p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_phy_uplink_power_control_dedicated->p0_ue_pucch);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_uplink_power_control_dedicated->p_srs_offset, p_src + *p_length_read, "p_srs_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->p_srs_offset > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->p_srs_offset] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->p_srs_offset);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_uplink_power_control_dedicated->filter_coefficient, p_src + *p_length_read, "filter_coefficient");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_uplink_power_control_dedicated->filter_coefficient > 14))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_uplink_power_control_dedicated->filter_coefficient] should be less than"
            " or equal to 14. Incorrect value %u received.", p_rrc_phy_uplink_power_control_dedicated->filter_coefficient);
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
rrc_il_parse_rrc_phy_tpc_index
(
    rrc_phy_tpc_index_t *p_rrc_phy_tpc_index,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_tpc_index, 0, sizeof(rrc_phy_tpc_index_t));

    /* This function parses rrc_phy_tpc_index */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_tpc_index->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_tpc_index->index_of_format3, p_src + *p_length_read, "index_of_format3");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_tpc_index->index_of_format3 < 1) || (p_rrc_phy_tpc_index->index_of_format3 > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tpc_index->index_of_format3] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_phy_tpc_index->index_of_format3);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_tpc_index->bitmask & TPC_INDEX_FORMAT_3A_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_tpc_index->index_of_format3a, p_src + *p_length_read, "index_of_format3a");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_tpc_index->index_of_format3a < 1) || (p_rrc_phy_tpc_index->index_of_format3a > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_tpc_index->index_of_format3a] should be in range "
            "1 to 31. Incorrect value %u received.", p_rrc_phy_tpc_index->index_of_format3a);
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
rrc_il_parse_rrc_phy_tpc_pdcch_config_param
(
    rrc_phy_tpc_pdcch_config_param_t *p_rrc_phy_tpc_pdcch_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_tpc_pdcch_config_param, 0, sizeof(rrc_phy_tpc_pdcch_config_param_t));

    /* This function parses rrc_phy_tpc_pdcch_config_param */

    if (*p_length_read + (S32)sizeof(p_rrc_phy_tpc_pdcch_config_param->tpc_rnti) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_tpc_pdcch_config_param->tpc_rnti); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_tpc_pdcch_config_param->tpc_rnti[loop], (void*)(p_src + *p_length_read), "tpc_rnti[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_tpc_index(
        &p_rrc_phy_tpc_pdcch_config_param->tpc_index,
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
rrc_il_parse_rrc_phy_tpc_pdcch_configuration
(
    rrc_phy_tpc_pdcch_configuration_t *p_rrc_phy_tpc_pdcch_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_tpc_pdcch_configuration, 0, sizeof(rrc_phy_tpc_pdcch_configuration_t));

    /* This function parses rrc_phy_tpc_pdcch_configuration */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_tpc_pdcch_configuration->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_tpc_pdcch_configuration->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_tpc_pdcch_config_param(
        &p_rrc_phy_tpc_pdcch_configuration->tpc_pdcch_config_param,
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
rrc_il_parse_rrc_phy_subband_cqi_param_param
(
    rrc_phy_subband_cqi_param_param_t *p_rrc_phy_subband_cqi_param_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_subband_cqi_param_param, 0, sizeof(rrc_phy_subband_cqi_param_param_t));

    /* This function parses rrc_phy_subband_cqi_param_param */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_subband_cqi_param_param->k, p_src + *p_length_read, "k");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_subband_cqi_param_param->k < 1) || (p_rrc_phy_subband_cqi_param_param->k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_subband_cqi_param_param->k] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_subband_cqi_param_param->k);
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
rrc_il_parse_rrc_phy_cqi_format_indicator_periodic
(
    rrc_phy_cqi_format_indicator_periodic_t *p_rrc_phy_cqi_format_indicator_periodic,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_format_indicator_periodic, 0, sizeof(rrc_phy_cqi_format_indicator_periodic_t));

    /* This function parses rrc_phy_cqi_format_indicator_periodic */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_format_indicator_periodic->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_format_indicator_periodic->bitmask & RRC_PHY_SUBBAND_CQI_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_subband_cqi_param_param(
        &p_rrc_phy_cqi_format_indicator_periodic->subband_cqi_param,
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
rrc_il_parse_rrc_phy_cqi_reporting_periodic_param
(
    rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_reporting_periodic_param, 0, sizeof(rrc_phy_cqi_reporting_periodic_param_t));

    /* This function parses rrc_phy_cqi_reporting_periodic_param */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting_periodic_param->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index, p_src + *p_length_read, "cqi_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index, p_src + *p_length_read, "cqi_pmi_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_format_indicator_periodic(
        &p_rrc_phy_cqi_reporting_periodic_param->cqi_format_indicator_periodic,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_phy_cqi_reporting_periodic_param->bitmask & RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting_periodic_param->ri_config_index, p_src + *p_length_read, "ri_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->ri_config_index);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi, p_src + *p_length_read, "simultaneous_ack_nack_and_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_reporting_periodic_param->simultaneous_ack_nack_and_cqi);
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
rrc_il_parse_rrc_phy_cqi_reporting_periodic
(
    rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_reporting_periodic, 0, sizeof(rrc_phy_cqi_reporting_periodic_t));

    /* This function parses rrc_phy_cqi_reporting_periodic */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting_periodic->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_reporting_periodic->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_reporting_periodic_param(
        &p_rrc_phy_cqi_reporting_periodic->cqi_reporting_periodic_param,
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
rrc_il_parse_rrc_phy_cqi_reporting
(
    rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_reporting, 0, sizeof(rrc_phy_cqi_reporting_t));

    /* This function parses rrc_phy_cqi_reporting */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_reporting->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic, p_src + *p_length_read, "cqi_reporting_mode_aperiodic");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_cqi_reporting->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset, p_src + *p_length_read, "nom_pdsch_rs_epre_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset < -1) || (p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_phy_cqi_reporting->nom_pdsch_rs_epre_offset);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_reporting->bitmask & RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_reporting_periodic(
        &p_rrc_phy_cqi_reporting->cqi_reporting_periodic,
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
rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated_param
(
    rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_rrc_phy_sounding_rs_ul_config_dedicated_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_sounding_rs_ul_config_dedicated_param, 0, sizeof(rrc_phy_sounding_rs_ul_config_dedicated_param_t));

    /* This function parses rrc_phy_sounding_rs_ul_config_dedicated_param */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth, p_src + *p_length_read, "srs_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth, p_src + *p_length_read, "srs_hopping_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_hopping_bandwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position, p_src + *p_length_read, "frequency_domain_position");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position] should be less than"
            " or equal to 23. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->frequency_domain_position);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration, p_src + *p_length_read, "duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->duration);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index, p_src + *p_length_read, "srs_configuration_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->srs_configuration_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb, p_src + *p_length_read, "transmission_comb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->transmission_comb);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift, p_src + *p_length_read, "cyclic_shift");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_sounding_rs_ul_config_dedicated_param->cyclic_shift);
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
rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated
(
    rrc_phy_sounding_rs_ul_config_dedicated_t *p_rrc_phy_sounding_rs_ul_config_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_sounding_rs_ul_config_dedicated, 0, sizeof(rrc_phy_sounding_rs_ul_config_dedicated_t));

    /* This function parses rrc_phy_sounding_rs_ul_config_dedicated */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_sounding_rs_ul_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated_param(
        &p_rrc_phy_sounding_rs_ul_config_dedicated->sounding_rs_ul_config_dedicated_param,
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
rrc_il_parse_rrc_phy_codebook_subset_restriction
(
    rrc_phy_codebook_subset_restriction_t *p_rrc_phy_codebook_subset_restriction,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_codebook_subset_restriction, 0, sizeof(rrc_phy_codebook_subset_restriction_t));

    /* This function parses rrc_phy_codebook_subset_restriction */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_codebook_subset_restriction->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_codebook_subset_restriction->type > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_codebook_subset_restriction->type] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_codebook_subset_restriction->type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_phy_codebook_subset_restriction->value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_codebook_subset_restriction->value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_codebook_subset_restriction->value[loop], (void*)(p_src + *p_length_read), "value[]");
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
rrc_il_parse_rrc_phy_ue_transmit_antenna_selection
(
    rrc_phy_ue_transmit_antenna_selection_t *p_rrc_phy_ue_transmit_antenna_selection,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_ue_transmit_antenna_selection, 0, sizeof(rrc_phy_ue_transmit_antenna_selection_t));

    /* This function parses rrc_phy_ue_transmit_antenna_selection */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_ue_transmit_antenna_selection->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_ue_transmit_antenna_selection->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ue_transmit_antenna_selection->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ue_transmit_antenna_selection->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_ue_transmit_antenna_selection->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_ue_transmit_antenna_selection->bitmask & RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type, p_src + *p_length_read, "ue_transmit_antenna_selection_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_ue_transmit_antenna_selection->ue_transmit_antenna_selection_type);
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
rrc_il_parse_rrc_phy_antenna_information_dedicated
(
    rrc_phy_antenna_information_dedicated_t *p_rrc_phy_antenna_information_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_antenna_information_dedicated, 0, sizeof(rrc_phy_antenna_information_dedicated_t));

    /* This function parses rrc_phy_antenna_information_dedicated */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_antenna_information_dedicated->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_antenna_information_dedicated->transmission_mode, p_src + *p_length_read, "transmission_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_antenna_information_dedicated->transmission_mode > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_antenna_information_dedicated->transmission_mode] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_phy_antenna_information_dedicated->transmission_mode);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_antenna_information_dedicated->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_codebook_subset_restriction(
        &p_rrc_phy_antenna_information_dedicated->codebook_subset_restriction,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_ue_transmit_antenna_selection(
        &p_rrc_phy_antenna_information_dedicated->ue_transmit_antenna_selection,
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
rrc_il_parse_rrc_phy_antenna_information
(
    rrc_phy_antenna_information_t *p_rrc_phy_antenna_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_antenna_information, 0, sizeof(rrc_phy_antenna_information_t));

    /* This function parses rrc_phy_antenna_information */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_antenna_information->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_antenna_information->bitmask & RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_antenna_information_dedicated(
        &p_rrc_phy_antenna_information->antenna_information_explicit_value,
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
rrc_il_parse_rrc_phy_scheduling_request_config_param
(
    rrc_phy_scheduling_request_config_param_t *p_rrc_phy_scheduling_request_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_scheduling_request_config_param, 0, sizeof(rrc_phy_scheduling_request_config_param_t));

    /* This function parses rrc_phy_scheduling_request_config_param */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index, p_src + *p_length_read, "sr_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->sr_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_scheduling_request_config_param->sr_configuration_index, p_src + *p_length_read, "sr_configuration_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->sr_configuration_index > 157))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->sr_configuration_index] should be less than"
            " or equal to 157. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->sr_configuration_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_scheduling_request_config_param->dsr_trans_max, p_src + *p_length_read, "dsr_trans_max");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scheduling_request_config_param->dsr_trans_max > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scheduling_request_config_param->dsr_trans_max] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_scheduling_request_config_param->dsr_trans_max);
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
rrc_il_parse_rrc_phy_scheduling_request_config
(
    rrc_phy_scheduling_request_config_t *p_rrc_phy_scheduling_request_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_scheduling_request_config, 0, sizeof(rrc_phy_scheduling_request_config_t));

    /* This function parses rrc_phy_scheduling_request_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_scheduling_request_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_scheduling_request_config->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_scheduling_request_config_param(
        &p_rrc_phy_scheduling_request_config->scheduling_request_config_param,
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
rrc_il_parse_rrc_phy_cqi_report_config_v920
(
    rrc_phy_cqi_report_config_v920_t *p_rrc_phy_cqi_report_config_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_config_v920, 0, sizeof(rrc_phy_cqi_report_config_v920_t));

    /* This function parses rrc_phy_cqi_report_config_v920 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_config_v920->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_CQI_MASK_R9_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_config_v920->cqi_mask_r9, p_src + *p_length_read, "cqi_mask_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_config_v920->cqi_mask_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_config_v920->cqi_mask_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_config_v920->cqi_mask_r9);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_cqi_report_config_v920->bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9, p_src + *p_length_read, "pmi_ri_report_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_config_v920->pmi_ri_report_r9);
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
rrc_il_parse_rrc_phy_codebook_subset_restriction_v920
(
    rrc_phy_codebook_subset_restriction_v920_t *p_rrc_phy_codebook_subset_restriction_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_codebook_subset_restriction_v920, 0, sizeof(rrc_phy_codebook_subset_restriction_v920_t));

    /* This function parses rrc_phy_codebook_subset_restriction_v920 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_codebook_subset_restriction_v920->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_codebook_subset_restriction_v920->type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_codebook_subset_restriction_v920->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_codebook_subset_restriction_v920->type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_phy_codebook_subset_restriction_v920->value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_codebook_subset_restriction_v920->value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_codebook_subset_restriction_v920->value[loop], (void*)(p_src + *p_length_read), "value[]");
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
rrc_il_parse_rrc_phy_antenna_info_v920
(
    rrc_phy_antenna_info_v920_t *p_rrc_phy_antenna_info_v920,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_antenna_info_v920, 0, sizeof(rrc_phy_antenna_info_v920_t));

    /* This function parses rrc_phy_antenna_info_v920 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_antenna_info_v920->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_antenna_info_v920->bitmask & RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_codebook_subset_restriction_v920(
        &p_rrc_phy_antenna_info_v920->codebook_subset_restriction_v920,
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
rrc_il_parse_rrc_codebook_subset_restriction_r10
(
    rrc_codebook_subset_restriction_r10_t *p_rrc_codebook_subset_restriction_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_codebook_subset_restriction_r10, 0, sizeof(rrc_codebook_subset_restriction_r10_t));

    /* This function parses rrc_codebook_subset_restriction_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_codebook_subset_restriction_r10->num_bits, p_src + *p_length_read, "num_bits");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_codebook_subset_restriction_r10->num_bits < 2) || (p_rrc_codebook_subset_restriction_r10->num_bits > 109))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_codebook_subset_restriction_r10->num_bits] should be in range "
            "2 to 109. Incorrect value %u received.", p_rrc_codebook_subset_restriction_r10->num_bits);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrc_codebook_subset_restriction_r10->value) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_codebook_subset_restriction_r10->value); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_codebook_subset_restriction_r10->value[loop], (void*)(p_src + *p_length_read), "value[]");
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
rrc_il_parse_rrc_antenna_info_dedicated_r10
(
    rrc_antenna_info_dedicated_r10_t *p_rrc_antenna_info_dedicated_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_antenna_info_dedicated_r10, 0, sizeof(rrc_antenna_info_dedicated_r10_t));

    /* This function parses rrc_antenna_info_dedicated_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_antenna_info_dedicated_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_antenna_info_dedicated_r10->transmission_mode, p_src + *p_length_read, "transmission_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_dedicated_r10->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_dedicated_r10->transmission_mode] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_antenna_info_dedicated_r10->transmission_mode);
        return RRC_FAILURE;
    }

    if (p_rrc_antenna_info_dedicated_r10->bitmask & RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_codebook_subset_restriction_r10(
        &p_rrc_antenna_info_dedicated_r10->codebook_subset_restriction_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_ue_transmit_antenna_selection(
        &p_rrc_antenna_info_dedicated_r10->ue_transmit_antenna_selection,
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
rrc_il_parse_rrc_antenna_info_r10
(
    rrc_antenna_info_r10_t *p_rrc_antenna_info_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_antenna_info_r10, 0, sizeof(rrc_antenna_info_r10_t));

    /* This function parses rrc_antenna_info_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_antenna_info_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_antenna_info_r10->bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_antenna_info_dedicated_r10(
        &p_rrc_antenna_info_r10->explicit_value,
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
rrc_il_parse_rrc_antenna_info_ul_r10
(
    rrc_antenna_info_ul_r10_t *p_rrc_antenna_info_ul_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_antenna_info_ul_r10, 0, sizeof(rrc_antenna_info_ul_r10_t));

    /* This function parses rrc_antenna_info_ul_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_antenna_info_ul_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_antenna_info_ul_r10->transmission_mode_ul, p_src + *p_length_read, "transmission_mode_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_ul_r10->transmission_mode_ul > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_ul_r10->transmission_mode_ul] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_antenna_info_ul_r10->transmission_mode_ul);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_antenna_info_ul_r10->bitmask & RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_antenna_info_ul_r10->four_antenna_port_activated, p_src + *p_length_read, "four_antenna_port_activated");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_antenna_info_ul_r10->four_antenna_port_activated > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_antenna_info_ul_r10->four_antenna_port_activated] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_antenna_info_ul_r10->four_antenna_port_activated);
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
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger
(
    rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger, 0, sizeof(rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t));

    /* This function parses rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger1, p_src + *p_length_read, "trigger1");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger->trigger2, p_src + *p_length_read, "trigger2");
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
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup
(
    rrc_phy_cqi_report_aperiodic_r10_setup_t *p_rrc_phy_cqi_report_aperiodic_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_aperiodic_r10_setup, 0, sizeof(rrc_phy_cqi_report_aperiodic_r10_setup_t));

    /* This function parses rrc_phy_cqi_report_aperiodic_r10_setup */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic, p_src + *p_length_read, "cqi_reporting_mode_aperiodic");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_phy_cqi_report_aperiodic_r10_setup->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_aperiodic_r10_setup->bitmask & RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger(
        &p_rrc_phy_cqi_report_aperiodic_r10_setup->aperiodic_csi_trigger,
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
rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10
(
    rrc_phy_cqi_report_aperiodic_r10_t *p_rrc_phy_cqi_report_aperiodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_aperiodic_r10, 0, sizeof(rrc_phy_cqi_report_aperiodic_r10_t));

    /* This function parses rrc_phy_cqi_report_aperiodic_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_aperiodic_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_aperiodic_r10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_aperiodic_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_aperiodic_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_aperiodic_r10->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_aperiodic_r10->bitmask & RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10_setup(
        &p_rrc_phy_cqi_report_aperiodic_r10->setup,
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
rrc_il_parse_rrc_phy_cqi_format_ind_wideband_r10
(
    rrc_phy_cqi_format_ind_wideband_r10_t *p_rrc_phy_cqi_format_ind_wideband_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_format_ind_wideband_r10, 0, sizeof(rrc_phy_cqi_format_ind_wideband_r10_t));

    /* This function parses rrc_phy_cqi_format_ind_wideband_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_format_ind_wideband_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_format_ind_wideband_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode, p_src + *p_length_read, "csi_report_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_wideband_r10->csi_report_mode);
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
rrc_il_parse_rrc_phy_cqi_format_ind_subband_r10
(
    rrc_phy_cqi_format_ind_subband_r10_t *p_rrc_phy_cqi_format_ind_subband_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_format_ind_subband_r10, 0, sizeof(rrc_phy_cqi_format_ind_subband_r10_t));

    /* This function parses rrc_phy_cqi_format_ind_subband_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_format_ind_subband_r10->k, p_src + *p_length_read, "k");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_cqi_format_ind_subband_r10->k < 1) || (p_rrc_phy_cqi_format_ind_subband_r10->k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_subband_r10->k] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_subband_r10->k);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor, p_src + *p_length_read, "periodicy_factor");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_format_ind_subband_r10->periodicy_factor);
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
rrc_il_parse_rrc_phy_cqi_format_ind_periodic_r10
(
    rrc_phy_cqi_format_ind_periodic_r10_t *p_rrc_phy_cqi_format_ind_periodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_format_ind_periodic_r10, 0, sizeof(rrc_phy_cqi_format_ind_periodic_r10_t));

    /* This function parses rrc_phy_cqi_format_ind_periodic_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_format_ind_periodic_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_format_ind_wideband_r10(
        &p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_wideband,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_cqi_format_ind_periodic_r10->bitmask & RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_format_ind_subband_r10(
        &p_rrc_phy_cqi_format_ind_periodic_r10->cqi_format_ind_subband,
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
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup, 0, sizeof(rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t));

    /* This function parses rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2, p_src + *p_length_read, "cqi_pmi_config_index2");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->cqi_pmi_config_index2);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->bitmask & RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2, p_src + *p_length_read, "ri_config_index2");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup->ri_config_index2);
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
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
(
    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index *p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index, 0, sizeof(rrc_phy_cqi_report_periodic_r10_setup_csi_config_index));

    /* This function parses rrc_phy_cqi_report_periodic_r10_setup_csi_config_index */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup(
        &p_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index->setup,
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
rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup
(
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_report_periodic_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_periodic_r10_setup, 0, sizeof(rrc_phy_cqi_report_periodic_r10_setup_t));

    /* This function parses rrc_phy_cqi_report_periodic_r10_setup */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index, p_src + *p_length_read, "cqi_pucch_resource_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index > 1184))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index] should be less than"
            " or equal to 1184. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1, p_src + *p_length_read, "cqi_pucch_resource_index_p1");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1 > 1184))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1] should be less than"
            " or equal to 1184. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pucch_resource_index_p1);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index, p_src + *p_length_read, "cqi_pmi_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_format_ind_periodic_r10(
        &p_rrc_phy_cqi_report_periodic_r10_setup->cqi_format_ind_periodic,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index, p_src + *p_length_read, "ri_config_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->ri_config_index);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi, p_src + *p_length_read, "simultaneous_ack_nack_cqi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->simultaneous_ack_nack_cqi);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9, p_src + *p_length_read, "cqi_mask_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10_setup->cqi_mask_r9);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_cqi_report_periodic_r10_setup->bitmask & RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup_csi_config_index(
        &p_rrc_phy_cqi_report_periodic_r10_setup->csi_config_index,
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
rrc_il_parse_rrc_phy_cqi_report_periodic_r10
(
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_report_periodic_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_periodic_r10, 0, sizeof(rrc_phy_cqi_report_periodic_r10_t));

    /* This function parses rrc_phy_cqi_report_periodic_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_periodic_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_periodic_r10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_periodic_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_periodic_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_periodic_r10->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_periodic_r10->bitmask & RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_periodic_r10_setup(
        &p_rrc_phy_cqi_report_periodic_r10->setup,
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
rrc_il_parse_rrc_phy_meas_subframe_pattern_fdd_r10
(
    rrc_phy_meas_subframe_pattern_fdd_r10_t *p_rrc_phy_meas_subframe_pattern_fdd_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_meas_subframe_pattern_fdd_r10, 0, sizeof(rrc_phy_meas_subframe_pattern_fdd_r10_t));

    /* This function parses rrc_phy_meas_subframe_pattern_fdd_r10 */

    if (*p_length_read + (S32)sizeof(p_rrc_phy_meas_subframe_pattern_fdd_r10->data) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_fdd_r10->data); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_meas_subframe_pattern_fdd_r10->data[loop], (void*)(p_src + *p_length_read), "data[]");
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
rrc_il_parse_rrc_phy_meas_subframe_pattern_tdd_r10
(
    rrc_phy_meas_subframe_pattern_tdd_r10_t *p_rrc_phy_meas_subframe_pattern_tdd_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_meas_subframe_pattern_tdd_r10, 0, sizeof(rrc_phy_meas_subframe_pattern_tdd_r10_t));

    /* This function parses rrc_phy_meas_subframe_pattern_tdd_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
    {

    if (*p_length_read + (S32)sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config1_5_r10[loop], (void*)(p_src + *p_length_read), "subframe_config1_5_r10[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
    {

    if (*p_length_read + (S32)sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config0_r10[loop], (void*)(p_src + *p_length_read), "subframe_config0_r10[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrc_phy_meas_subframe_pattern_tdd_r10->bitmask & RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
    {

    if (*p_length_read + (S32)sizeof(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_phy_meas_subframe_pattern_tdd_r10->subframe_config6_r10[loop], (void*)(p_src + *p_length_read), "subframe_config6_r10[]");
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
rrc_il_parse_rrc_phy_meas_subframe_pattern_r10
(
    rrc_phy_meas_subframe_pattern_r10_t *p_rrc_phy_meas_subframe_pattern_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_meas_subframe_pattern_r10, 0, sizeof(rrc_phy_meas_subframe_pattern_r10_t));

    /* This function parses rrc_phy_meas_subframe_pattern_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_meas_subframe_pattern_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_meas_subframe_pattern_fdd_r10(
        &p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_fdd_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_meas_subframe_pattern_r10->bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_meas_subframe_pattern_tdd_r10(
        &p_rrc_phy_meas_subframe_pattern_r10->subframe_pattern_tdd_r10,
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
rrc_il_parse_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup
(
    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t *p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup, 0, sizeof(rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t));

    /* This function parses rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup */

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_meas_subframe_pattern_r10(
        &p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set1_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_meas_subframe_pattern_r10(
        &p_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup->csi_meas_subframe_set2_r10,
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
rrc_il_parse_rrc_phy_cqi_report_csi_subframePattern_r10
(
    rrc_phy_cqi_report_csi_subframePattern_r10_t *p_rrc_phy_cqi_report_csi_subframePattern_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cqi_report_csi_subframePattern_r10, 0, sizeof(rrc_phy_cqi_report_csi_subframePattern_r10_t));

    /* This function parses rrc_phy_cqi_report_csi_subframePattern_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cqi_report_csi_subframePattern_r10->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_cqi_report_csi_subframePattern_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup(
        &p_rrc_phy_cqi_report_csi_subframePattern_r10->csi_subframe_pattern_config_r10_setup,
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
rrc_il_parse_rrc_cqi_report_config_r10
(
    rrc_cqi_report_config_r10_t *p_rrc_cqi_report_config_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_cqi_report_config_r10, 0, sizeof(rrc_cqi_report_config_r10_t));

    /* This function parses rrc_cqi_report_config_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_cqi_report_config_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_aperiodic_r10(
        &p_rrc_cqi_report_config_r10->cqi_report_aperiodic_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset, p_src + *p_length_read, "nomPDSCH_rs_epre_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset < -1) || (p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset);
        return RRC_FAILURE;
    }

    if (p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_periodic_r10(
        &p_rrc_cqi_report_config_r10->cqi_report_periodic_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10, p_src + *p_length_read, "cqi_report_pmi_ri_report_r10");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_cqi_report_config_r10->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_csi_subframePattern_r10(
        &p_rrc_cqi_report_config_r10->csi_subframePattern_r10,
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
rrc_il_parse_rrc_additional_spectrum_emission_ca_r10
(
    rrc_additional_spectrum_emission_ca_r10_t *p_rrc_additional_spectrum_emission_ca_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_additional_spectrum_emission_ca_r10, 0, sizeof(rrc_additional_spectrum_emission_ca_r10_t));

    /* This function parses rrc_additional_spectrum_emission_ca_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_additional_spectrum_emission_ca_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_additional_spectrum_emission_ca_r10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_additional_spectrum_emission_ca_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_additional_spectrum_emission_ca_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_additional_spectrum_emission_ca_r10->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_additional_spectrum_emission_ca_r10->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell, p_src + *p_length_read, "additional_spectrum_emission_pcell");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell < 1) || (p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_additional_spectrum_emission_ca_r10->additional_spectrum_emission_pcell);
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
rrc_il_parse_n1pucch_an_cs_r10
(
    n1pucch_an_cs_r10_t *p_n1pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_n1pucch_an_cs_r10, 0, sizeof(n1pucch_an_cs_r10_t));

    /* This function parses n1pucch_an_cs_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_n1pucch_an_cs_r10->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_n1pucch_an_cs_r10->count < 1) || (p_n1pucch_an_cs_r10->count > MAX_N1PUCCH_AN_CS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_n1pucch_an_cs_r10->count] should be in range "
            "1 to MAX_N1PUCCH_AN_CS. Incorrect value %u received.", p_n1pucch_an_cs_r10->count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_n1pucch_an_cs_r10->count * sizeof(p_n1pucch_an_cs_r10->n1_pucch_an_cs[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_n1pucch_an_cs_r10->count; loop++)
        {
            rrc_cp_unpack_U16((void*)&p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop], (void*)(p_src + *p_length_read), "n1_pucch_an_cs[]");
            if (p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop] > 2047)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_n1pucch_an_cs_r10->n1_pucch_an_cs[%u] should be less"
                    " than or equal to 2047. "
                    "Incorrect value %d received.", loop, p_n1pucch_an_cs_r10->n1_pucch_an_cs[loop]);
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
rrc_il_parse_n1pucch_an_cs_list_r10
(
    n1pucch_an_cs_list_r10_t *p_n1pucch_an_cs_list_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_n1pucch_an_cs_list_r10, 0, sizeof(n1pucch_an_cs_list_r10_t));

    /* This function parses n1pucch_an_cs_list_r10 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_n1pucch_an_cs_list_r10->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_n1pucch_an_cs_list_r10->count < 1) || (p_n1pucch_an_cs_list_r10->count > MAX_N1PUCCH_AN_CS_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_n1pucch_an_cs_list_r10->count] should be in range "
            "1 to MAX_N1PUCCH_AN_CS_LIST. Incorrect value %u received.", p_n1pucch_an_cs_list_r10->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_n1pucch_an_cs_list_r10->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_n1pucch_an_cs_r10(
                &p_n1pucch_an_cs_list_r10->n1_pucch_an_cs[loop],
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
rrc_il_parse_rrc_channel_selection_n1_pucch_an_cs_r10
(
    rrc_channel_selection_n1_pucch_an_cs_r10_t *p_rrc_channel_selection_n1_pucch_an_cs_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_channel_selection_n1_pucch_an_cs_r10, 0, sizeof(rrc_channel_selection_n1_pucch_an_cs_r10_t));

    /* This function parses rrc_channel_selection_n1_pucch_an_cs_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_channel_selection_n1_pucch_an_cs_r10->request_type);
        return RRC_FAILURE;
    }

    if (p_rrc_channel_selection_n1_pucch_an_cs_r10->bitmask & RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_n1pucch_an_cs_list_r10(
        &p_rrc_channel_selection_n1_pucch_an_cs_r10->n1pucch_an_cs_list_r10,
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
rrc_il_parse_rrc_pucch_channel_selection_config_v1020
(
    rrc_pucch_channel_selection_config_v1020_t *p_rrc_pucch_channel_selection_config_v1020,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pucch_channel_selection_config_v1020, 0, sizeof(rrc_pucch_channel_selection_config_v1020_t));

    /* This function parses rrc_pucch_channel_selection_config_v1020 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pucch_channel_selection_config_v1020->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_pucch_channel_selection_config_v1020->bitmask & RRC_N1PUCCH_AN_CS_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_channel_selection_n1_pucch_an_cs_r10(
        &p_rrc_pucch_channel_selection_config_v1020->channel_selection_n1_pucch_an_cs_r10,
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
rrc_il_parse_rrc_phy_physical_config_dedicated_extended
(
    rrc_phy_physical_config_dedicated_extended_t *p_rrc_phy_physical_config_dedicated_extended,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_physical_config_dedicated_extended, 0, sizeof(rrc_phy_physical_config_dedicated_extended_t));

    /* This function parses rrc_phy_physical_config_dedicated_extended */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_physical_config_dedicated_extended->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_additional_spectrum_emission_ca_r10(
        &p_rrc_phy_physical_config_dedicated_extended->additional_spectrum_emission_ca,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated_extended->bitmask & RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_pucch_channel_selection_config_v1020(
        &p_rrc_phy_physical_config_dedicated_extended->pucch_channel_selection_config_v1020,
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
rrc_il_parse_rrc_phy_physical_config_dedicated
(
    rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_physical_config_dedicated, 0, sizeof(rrc_phy_physical_config_dedicated_t));

    /* This function parses rrc_phy_physical_config_dedicated */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_physical_config_dedicated->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pdsch_configuration_dedicated(
        &p_rrc_phy_physical_config_dedicated->pdsch_configuration_dedicated,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pucch_configuration_dedicated(
        &p_rrc_phy_physical_config_dedicated->pucch_configuration_dedicated,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pusch_configuration_dedicated(
        &p_rrc_phy_physical_config_dedicated->pusch_configuration_dedicated,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_uplink_power_control_dedicated(
        &p_rrc_phy_physical_config_dedicated->uplink_power_control_dedicated,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_tpc_pdcch_configuration(
        &p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pucch,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_tpc_pdcch_configuration(
        &p_rrc_phy_physical_config_dedicated->tpc_pdcch_config_pusch,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_reporting(
        &p_rrc_phy_physical_config_dedicated->cqi_reporting,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_sounding_rs_ul_config_dedicated(
        &p_rrc_phy_physical_config_dedicated->sounding_rs_ul_config_dedicated,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_antenna_information(
        &p_rrc_phy_physical_config_dedicated->antenna_information,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_scheduling_request_config(
        &p_rrc_phy_physical_config_dedicated->scheduling_request_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_config_v920(
        &p_rrc_phy_physical_config_dedicated->cqi_report_config_v920,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_antenna_info_v920(
        &p_rrc_phy_physical_config_dedicated->antenna_info_v920,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_antenna_info_r10(
        &p_rrc_phy_physical_config_dedicated->antenna_info_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_antenna_info_ul_r10(
        &p_rrc_phy_physical_config_dedicated->antenna_info_ul_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_cqi_report_config_r10(
        &p_rrc_phy_physical_config_dedicated->cqi_report_config_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_physical_config_dedicated->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_physical_config_dedicated_extended(
        &p_rrc_phy_physical_config_dedicated->phy_physical_config_dedicated_extended,
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
rrc_il_parse_rrc_radio_res_dedicated_scell_non_ul_config
(
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_radio_res_dedicated_scell_non_ul_config, 0, sizeof(rrc_radio_res_dedicated_scell_non_ul_config_t));

    /* This function parses rrc_radio_res_dedicated_scell_non_ul_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_antenna_info_dedicated_r10(
        &p_rrc_radio_res_dedicated_scell_non_ul_config->antenna_info_dedicated_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask & RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_pdsch_configuration_dedicated(
        &p_rrc_radio_res_dedicated_scell_non_ul_config->pdsch_configuration_dedicated,
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
rrc_il_parse_rrc_cqi_report_config_scell_r10
(
    rrc_cqi_report_config_scell_r10_t *p_rrc_cqi_report_config_scell_r10,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_cqi_report_config_scell_r10, 0, sizeof(rrc_cqi_report_config_scell_r10_t));

    /* This function parses rrc_cqi_report_config_scell_r10 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_cqi_report_config_scell_r10->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic, p_src + *p_length_read, "cqi_reporting_mode_aperiodic");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_cqi_report_config_scell_r10->cqi_reporting_mode_aperiodic);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset, p_src + *p_length_read, "nomPDSCH_RS_EPRE_Offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset < -1) || (p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset] should be in range "
            "-1 to 6. Incorrect value %d received.", p_rrc_cqi_report_config_scell_r10->nomPDSCH_RS_EPRE_Offset);
        return RRC_FAILURE;
    }

    if (p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_cqi_report_periodic_r10(
        &p_rrc_cqi_report_config_scell_r10->cqi_report_periodic_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_cqi_report_config_scell_r10->bitmask & RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10, p_src + *p_length_read, "cqi_report_pmi_ri_report_r10");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10 > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_cqi_report_config_scell_r10->cqi_report_pmi_ri_report_r10);
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
rrc_il_parse_rrc_radio_res_dedicated_scell_ul_config
(
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_radio_res_dedicated_scell_ul_config, 0, sizeof(rrc_radio_res_dedicated_scell_ul_config_t));

    /* This function parses rrc_radio_res_dedicated_scell_ul_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_radio_res_dedicated_scell_ul_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_radio_res_dedicated_scell_ul_config->bitmask & RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_cqi_report_config_scell_r10(
        &p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell,
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
rrc_il_parse_rrc_physical_config_dedicated_scell
(
    rrc_physical_config_dedicated_scell_t *p_rrc_physical_config_dedicated_scell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_physical_config_dedicated_scell, 0, sizeof(rrc_physical_config_dedicated_scell_t));

    /* This function parses rrc_physical_config_dedicated_scell */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_physical_config_dedicated_scell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_dedicated_scell_non_ul_config(
        &p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_non_ul_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_physical_config_dedicated_scell->bitmask & RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_dedicated_scell_ul_config(
        &p_rrc_physical_config_dedicated_scell->radio_res_dedicated_scell_ul_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_physical_config_dedicated_scell->bitmask & RRM_LAA_SCELL_CONFIGURATION_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_dedicated_laa_scell_config(
        &p_rrc_physical_config_dedicated_scell->laa_scell_configuration_r13,
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
rrc_il_parse_rrc_radio_res_config_dedicated_scell
(
    rrc_radio_res_config_dedicated_scell_t *p_rrc_radio_res_config_dedicated_scell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_radio_res_config_dedicated_scell, 0, sizeof(rrc_radio_res_config_dedicated_scell_t));

    /* This function parses rrc_radio_res_config_dedicated_scell */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_radio_res_config_dedicated_scell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_radio_res_config_dedicated_scell->bitmask & RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_physical_config_dedicated_scell(
        &p_rrc_radio_res_config_dedicated_scell->physical_config_dedicated_scell,
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
rrc_il_parse_rrc_phy_scell_config
(
    rrc_phy_scell_config_t *p_rrc_phy_scell_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_scell_config, 0, sizeof(rrc_phy_scell_config_t));

    /* This function parses rrc_phy_scell_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_scell_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_scell_config->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scell_config->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_scell_config->cell_index);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_OPERATION_LIST_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_scell_config->operation_type, p_src + *p_length_read, "operation_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_scell_config->operation_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config->operation_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_phy_scell_config->operation_type);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_phy_scell_config->bitmask & RRC_PHY_CONFIG_DEDICATED_SCELL_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_radio_res_config_dedicated_scell(
        &p_rrc_phy_scell_config->radio_res_config_dedicated_scell,
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
rrc_il_parse_rrc_phy_scell_config_list
(
    rrc_phy_scell_config_list_t *p_rrc_phy_scell_config_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_scell_config_list, 0, sizeof(rrc_phy_scell_config_list_t));

    /* This function parses rrc_phy_scell_config_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_scell_config_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_phy_scell_config_list->count < 1) || (p_rrc_phy_scell_config_list->count > RRC_MAX_SCELL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_scell_config_list->count] should be in range "
            "1 to RRC_MAX_SCELL. Incorrect value %u received.", p_rrc_phy_scell_config_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_phy_scell_config_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_phy_scell_config(
                &p_rrc_phy_scell_config_list->rrc_phy_scell_config[loop],
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
rrc_il_parse_RrcMpdcchConfigSetupR13
(
    RrcMpdcchConfigSetupR13_t *p_RrcMpdcchConfigSetupR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_RrcMpdcchConfigSetupR13, 0, sizeof(RrcMpdcchConfigSetupR13_t));

    /* This function parses RrcMpdcchConfigSetupR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13, p_src + *p_length_read, "mpdcchPdschHoppingConfigR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13 > EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13] should be less than"
            " or equal to EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13, p_src + *p_length_read, "mpdcchStartSfUessFddR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13 > EMTC_MPDCCH_START_SF_UESS_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_UESS_FDD_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchStartSfUessFddR13);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13, p_src + *p_length_read, "mpdcchNumRepetitionR13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13 > EMTC_MPDCCH_NUM_REPETITION_UESS_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_UESS_MAX. Incorrect value %u received.", p_RrcMpdcchConfigSetupR13->mpdcchNumRepetitionR13);
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
rrc_il_parse_RrcMpdcchConfigR13
(
    RrcMpdcchConfigR13_t *p_RrcMpdcchConfigR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcMpdcchConfigR13, 0, sizeof(RrcMpdcchConfigR13_t));

    /* This function parses RrcMpdcchConfigR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMpdcchConfigR13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcMpdcchConfigR13->mpdcchConfigType, p_src + *p_length_read, "mpdcchConfigType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMpdcchConfigR13->mpdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMpdcchConfigR13->mpdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMpdcchConfigR13->mpdcchConfigType);
        return RRC_FAILURE;
    }

    if (p_RrcMpdcchConfigR13->bitmask & RRC_MPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcMpdcchConfigSetupR13(
        &p_RrcMpdcchConfigR13->mpdcchConfigSetupR13,
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
rrc_il_parse_RrcEpdcchSetConfigR11
(
    RrcEpdcchSetConfigR11_t *p_RrcEpdcchSetConfigR11,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcEpdcchSetConfigR11, 0, sizeof(RrcEpdcchSetConfigR11_t));

    /* This function parses RrcEpdcchSetConfigR11 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEpdcchSetConfigR11->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEpdcchSetConfigR11->setConfigId, p_src + *p_length_read, "setConfigId");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->setConfigId > EMTC_MAX_EPDCCH_CONFIG_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->setConfigId] should be less than"
            " or equal to EMTC_MAX_EPDCCH_CONFIG_ID. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->setConfigId);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEpdcchSetConfigR11->transmissionType, p_src + *p_length_read, "transmissionType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->transmissionType > EMTC_TRANSMISSION_TYPE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->transmissionType] should be less than"
            " or equal to EMTC_TRANSMISSION_TYPE_MAX. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->transmissionType);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEpdcchSetConfigR11->numPrbPairs, p_src + *p_length_read, "numPrbPairs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->numPrbPairs > EMTC_NUM_PRB_PAIR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->numPrbPairs] should be less than"
            " or equal to EMTC_NUM_PRB_PAIR_MAX. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->numPrbPairs);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcEpdcchSetConfigR11->prbBitmap, p_src + *p_length_read, "prbBitmap");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEpdcchSetConfigR11->prbBitmap > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEpdcchSetConfigR11->prbBitmap] should be less than"
            " or equal to 63. Incorrect value %u received.", p_RrcEpdcchSetConfigR11->prbBitmap);
        return RRC_FAILURE;
    }

    if (p_RrcEpdcchSetConfigR11->bitmask & RRC_MPDCCH_CONFIG_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcMpdcchConfigR13(
        &p_RrcEpdcchSetConfigR11->mpdcchConfigR13,
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
rrc_il_parse_EpdcchConfigSetup
(
    EpdcchConfigSetup_t *p_EpdcchConfigSetup,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_EpdcchConfigSetup, 0, sizeof(EpdcchConfigSetup_t));

    /* This function parses EpdcchConfigSetup */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchConfigSetup->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_START_SYMBOL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchConfigSetup->startSymbolR11, p_src + *p_length_read, "startSymbolR11");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_EpdcchConfigSetup->startSymbolR11 < 1) || (p_EpdcchConfigSetup->startSymbolR11 > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchConfigSetup->startSymbolR11] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_EpdcchConfigSetup->startSymbolR11);
        return RRC_FAILURE;
    }
    }

    if (p_EpdcchConfigSetup->bitmask & RRC_EPDCCH_CONFIG_ADD_MOD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcEpdcchSetConfigR11(
        &p_EpdcchConfigSetup->epdcchSetConfigR11,
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
rrc_il_parse_EpdcchConfig
(
    EpdcchConfig_t *p_EpdcchConfig,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_EpdcchConfig, 0, sizeof(EpdcchConfig_t));

    /* This function parses EpdcchConfig */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchConfig->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_EpdcchConfig->epdcchConfigType, p_src + *p_length_read, "epdcchConfigType");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchConfig->epdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchConfig->epdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_EpdcchConfig->epdcchConfigType);
        return RRC_FAILURE;
    }

    if (p_EpdcchConfig->bitmask & RRC_EPDCCH_CONFIG_SETUP_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_EpdcchConfigSetup(
        &p_EpdcchConfig->epdcchConfigSetup,
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
rrc_il_parse_RrcPucchConfigDedicatedR13
(
    RrcPucchConfigDedicatedR13_t *p_RrcPucchConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcPucchConfigDedicatedR13, 0, sizeof(RrcPucchConfigDedicatedR13_t));

    /* This function parses RrcPucchConfigDedicatedR13 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPucchConfigDedicatedR13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPucchConfigDedicatedR13->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchConfigDedicatedR13->type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchConfigDedicatedR13->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcPucchConfigDedicatedR13->type);
        return RRC_FAILURE;
    }

    if (p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcAckNackRepetitionSetupR13(
        &p_RrcPucchConfigDedicatedR13->ackNackRepetitionSetup,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_RrcPucchConfigDedicatedR13->bitmask & RRC_PUCCH_NUM_REPETITION_CE_FORMAT1_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13, p_src + *p_length_read, "pucchNumRepetitionCeFormat1r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX. Incorrect value %u received.", p_RrcPucchConfigDedicatedR13->pucchNumRepetitionCeFormat1r13);
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
rrc_il_parse_RrcPhysicalConfigDedicatedR13
(
    RrcPhysicalConfigDedicatedR13_t *p_RrcPhysicalConfigDedicatedR13,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_RrcPhysicalConfigDedicatedR13, 0, sizeof(RrcPhysicalConfigDedicatedR13_t));

    /* This function parses RrcPhysicalConfigDedicatedR13 */

    if (RRC_FAILURE == rrc_il_parse_EpdcchConfig(
        &p_RrcPhysicalConfigDedicatedR13->epdcchConfig,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_RrcPucchConfigDedicatedR13(
        &p_RrcPhysicalConfigDedicatedR13->pucchConfigDedicatedR13,
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
    rrc_cp_unpack_U8(&p_RrcPhysicalConfigDedicatedR13->ceMode, p_src + *p_length_read, "ceMode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPhysicalConfigDedicatedR13->ceMode > EMTC_CE_MODE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPhysicalConfigDedicatedR13->ceMode] should be less than"
            " or equal to EMTC_CE_MODE_MAX. Incorrect value %u received.", p_RrcPhysicalConfigDedicatedR13->ceMode);
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
rrc_il_parse_rrc_phy_create_ue_entity_cnf
(
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_create_ue_entity_cnf, 0, sizeof(rrc_phy_create_ue_entity_cnf_t));

    /* This function parses rrc_phy_create_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_create_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_create_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_create_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_create_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_create_ue_entity_cnf->response);
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
rrc_il_parse_rrc_phy_delete_ue_entity_req
(
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_delete_ue_entity_req, 0, sizeof(rrc_phy_delete_ue_entity_req_t));

    /* This function parses rrc_phy_delete_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_delete_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_phy_delete_ue_entity_cnf
(
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_delete_ue_entity_cnf, 0, sizeof(rrc_phy_delete_ue_entity_cnf_t));

    /* This function parses rrc_phy_delete_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_delete_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_delete_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_delete_ue_entity_cnf->response);
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
rrc_il_parse_rrc_phy_reconfig_ue_entity_req
(
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_reconfig_ue_entity_req, 0, sizeof(rrc_phy_reconfig_ue_entity_req_t));

    /* This function parses rrc_phy_reconfig_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_physical_config_dedicated(
        &p_rrc_phy_reconfig_ue_entity_req->physical_config_dedicated,
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
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_ue_entity_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_SCELL_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_phy_scell_config_list(
        &p_rrc_phy_reconfig_ue_entity_req->rrc_phy_scell_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_phy_reconfig_ue_entity_req->bitmask & RRC_PHY_RECONFIG_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_RrcPhysicalConfigDedicatedR13(
        &p_rrc_phy_reconfig_ue_entity_req->physicalConfigDedicatedR13,
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
rrc_il_parse_rrc_phy_reconfig_ue_entity_cnf
(
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_reconfig_ue_entity_cnf, 0, sizeof(rrc_phy_reconfig_ue_entity_cnf_t));

    /* This function parses rrc_phy_reconfig_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_ue_entity_cnf->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_reconfig_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_reconfig_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_reconfig_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_reconfig_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    if (p_rrc_phy_reconfig_ue_entity_cnf->bitmask & RRC_PHY_FAIL_CAUSE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_reconfig_ue_entity_cnf->fail_cause, p_src + *p_length_read, "fail_cause");
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
rrc_il_parse_rrc_phy_change_crnti_req
(
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_change_crnti_req, 0, sizeof(rrc_phy_change_crnti_req_t));

    /* This function parses rrc_phy_change_crnti_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_change_crnti_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_change_crnti_req->old_crnti, p_src + *p_length_read, "old_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_change_crnti_req->new_crnti, p_src + *p_length_read, "new_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_phy_change_crnti_cnf
(
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_change_crnti_cnf, 0, sizeof(rrc_phy_change_crnti_cnf_t));

    /* This function parses rrc_phy_change_crnti_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_change_crnti_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_change_crnti_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_change_crnti_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_change_crnti_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_change_crnti_cnf->response);
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
rrc_il_parse_rrc_phy_cell_start_req
(
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cell_start_req, 0, sizeof(rrc_phy_cell_start_req_t));

    /* This function parses rrc_phy_cell_start_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cell_start_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_start_req->cell_index);
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
rrc_il_parse_rrc_phy_cell_start_cnf
(
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cell_start_cnf, 0, sizeof(rrc_phy_cell_start_cnf_t));

    /* This function parses rrc_phy_cell_start_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cell_start_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_start_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_phy_cell_start_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_start_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_start_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_phy_cell_start_cnf->response);
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
rrc_il_parse_rrc_phy_cell_stop_req
(
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cell_stop_req, 0, sizeof(rrc_phy_cell_stop_req_t));

    /* This function parses rrc_phy_cell_stop_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cell_stop_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_stop_req->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_stop_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_stop_req->cell_index);
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
rrc_il_parse_rrc_phy_cell_stop_cnf
(
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_phy_cell_stop_cnf, 0, sizeof(rrc_phy_cell_stop_cnf_t));

    /* This function parses rrc_phy_cell_stop_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_phy_cell_stop_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_phy_cell_stop_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_phy_cell_stop_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_phy_cell_stop_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

