/******************************************************************************
*
*   FILE NAME:
*       uecc_llim_utils.h
*
*   DESCRIPTION:
*       This is utilities functions definitions for LLIM interface.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   20 Aug 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_LLIM_UTILS_H
#define UECC_LLIM_UTILS_H

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "uecc_ue_ctx.h"
#include "rrc_asn_enc_dec.h"

/****************************************************************************
* Defines
***************************************************************************/

#define ARRSIZE(array_name)     (sizeof(array_name) / sizeof(array_name[0]))

/****************************************************************************
 * Global functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_radio_resource_cfg_dedicated
*
*   DESCRIPTION:
*       This function builds RadioResourceConfigDedicated
*       structure for ASN1 encoding from given ue_adm_radio_res_config_t
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_resource_cfg_dedicated(
    uecc_ue_context_t               *p_uecc_ue_context,
    RadioResourceConfigDedicated    *p_asn1_rrc,
    rrm_srb_info_t                  *p_srb_info,
    OSCTXT                          *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_physical_config_dedicated
*
*   DESCRIPTION:
*                Builds ASN1 rrc_phy_physical_config_dedicated_t IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_physical_config_dedicated(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_sps_config
*
*   DESCRIPTION:
*                Builds ASN1 SPS_Config IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_sps_config(
    uecc_ue_context_t                       *p_uecc_ue_context,
    rrc_sps_config_t const                  *p_sps,
    SPS_Config                              *p_asn1_sps,
    OSCTXT                                  *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_mac_main_config
*
*   DESCRIPTION:
*                Builds ASN1 MAC-MainConfig IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_mac_main_config(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrm_mac_config_t                                *p_mac_config,
    RadioResourceConfigDedicated_mac_MainConfig     *p_asn1_mac,
    OSCTXT                                          *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_srb_to_add_mod
*
*   DESCRIPTION:
*                Builds ASN1 SRB-ToAddMod IE based on
*                configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_srb_to_add_mod(
/* CSR 00070192 Fix Start */
    uecc_ue_context_t               *p_uecc_ue_context,
/* CSR 00070192 Fix Stop */
    rrm_srb_config_t const          *p_srb_config,
    U8                              srb_identity,
    SRB_ToAddMod                    *p_asn1_srb_elem,
    OSCTXT                          *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_srb_to_add_mod_list
*
*   DESCRIPTION:
*                Builds ASN1 SRB_ToAddModList IE based on
*                configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_srb_to_add_mod_list(
/* CSR 00070192 Fix Start */
    uecc_ue_context_t           *p_uecc_ue_context,
/* CSR 00070192 Fix Stop */
    SRB_ToAddModList            *p_asn1_list,
    rrm_srb_info_t const        *p_srb_info,
    OSCTXT                      *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_drb_to_add_mod
*
*   DESCRIPTION:    This function fills 'DRB_ToAddMod' from information
*                   of 'rrm_drb_config_t'
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_drb_to_add_mod(
    uecc_ue_context_t       *p_uecc_ue_context,
    rrm_drb_config_t const  *p_drb,
    DRB_ToAddMod            *p_asn1_drb_elem,
    OSCTXT                  *p_asn1_ctx
);

/* ERAB RELEASE COMMAND START */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_drb_to_release_list
*
*   DESCRIPTION:    This function fills 'DRB_ToReleaseList' from information
*                   of 'erab_curr_proc_data'
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_drb_to_release_list(
    uecc_ue_context_t   *p_uecc_ue_context,
    DRB_ToReleaseList                            *p_asn1_list,
    OSCTXT                                      *p_asn1_ctx
);
/* ERAB RELEASE COMMAND END */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasConfig
*
*   DESCRIPTION:
*       This function builds MeasConfig field in RRCConnectionReconfiguration
*       message
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasConfig(
    uecc_ue_context_t   *p_uecc_ue_context,
    rrm_meas_config_t*  p_rrm_meas_config,
    OSCTXT*             p_asn1_ctx,
    MeasConfig*         p_meas_config
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrMeasConfig
*
*   DESCRIPTION:
*       This function builds CurrMeasConfig field in RRCConnectionReconfiguration
*       message
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasConfig(
    uecc_ue_context_t   *p_uecc_ue_context,
    uecc_ue_curr_meas_config_t*  p_ue_curr_meas_config,
    OSCTXT*             p_asn1_ctx,
    MeasConfig*         p_meas_config
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_get_nas_pdu_from_srb_data_status_ind_msg
*
*   DESCRIPTION:
*       Get NAS PDU from UECC_LLIM_SRB_DATA_STATUS_IND message if it exists.
*
*   RETURNS:
*       Pointer on NAS PDU
*       PNULL - if NAS PDU has not been found
******************************************************************************/
DedicatedInfoNAS* uecc_llim_get_nas_pdu_from_srb_data_status_ind_msg(
    uecc_ue_context_t  *p_uecc_ue_context,
    OSCTXT             *p_asn1_ctx,
    rrc_uecc_llim_srb_data_status_ind_t *p_llim_srb_data_status_ind
);
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_redirected_carrier_info
*
*   DESCRIPTION:
*                Builds RedirectedCarrierInfo based on
*                given rrm_redirected_carrier_info_t
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_redirected_carrier_info(
    uecc_ue_context_t               *p_uecc_ue_context,
    rrm_redirected_carrier_info_t   *p_rci,
    RedirectedCarrierInfo           *p_asn1_rci,
    OSCTXT                          *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_idle_mode_mobility_control
*
*   DESCRIPTION:
*                Builds IdleModeMobilityControlInfo based on
*                given rrm_idle_mode_mobility_control_info_t
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_idle_mode_mobility_control(
    uecc_ue_context_t                       *p_uecc_ue_context,
    rrm_idle_mode_mobility_control_info_t   *p_immc,
    IdleModeMobilityControlInfo             *p_asn1_immc,
    OSCTXT                                  *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_erb_modify_asn1_drb_to_add_mod_list
*
*   DESCRIPTION:
*                Builds drb_to_add_mod_list
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/

rrc_return_et uecc_llim_build_erb_modify_asn1_drb_to_add_mod_list(
    uecc_ue_context_t   *p_ue_context,
    DRB_ToAddModList                            *p_asn1_list,
    OSCTXT                                      *p_asn1_ctx
);

/* ERAB SETUP START */


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list
*
*   DESCRIPTION:    This function fills 'DRB_ToAddModList' 
*                 
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list(
    uecc_ue_context_t   *p_ue_context,
    DRB_ToAddModList                            *p_asn1_list,
    OSCTXT                                      *p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_erb_setup_asn1_radio_resource_cfg_dedicated
*
*   DESCRIPTION:
*       This function builds RadioResourceConfigDedicated
*       structure for ASN1 encoding from given ue_adm_radio_res_config_t
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_erb_setup_asn1_radio_resource_cfg_dedicated(
    uecc_ue_context_t               *p_uecc_ue_context,
    RadioResourceConfigDedicated    *p_asn1_rrc,
    OSCTXT                          *p_asn1_ctx
);

/******************************************************************************
*    FUNCTION NAME: uecc_llim_build_ics_asn1_radio_resource_cfg_dedicated
* 
*    DESCRIPTION:
*        This function builds RadioResourceConfigDedicated
*        structure for ASN1 encoding from given ue_adm_radio_res_config_t
* 
*    RETURNS:
*        RRC_FAILURE/RRC_SUCCESS
* 
* ******************************************************************************/
rrc_return_et uecc_llim_build_ics_asn1_radio_resource_cfg_dedicated(
        uecc_ue_context_t               *p_uecc_ue_context,
        RadioResourceConfigDedicated    *p_asn1_rrc,
        rrm_srb_info_t                  *p_srb_info,
        OSCTXT                          *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_radio_resource_cfg_dedicated_during_src_ho(
        uecc_ue_context_t               *p_uecc_ue_context,
        RadioResourceConfigDedicated    *p_asn1_rrc,
        rrm_srb_info_t                  *p_srb_info,
        OSCTXT                          *p_asn1_ctx
);

 

rrc_return_et uecc_llim_build_asn1_rrc_connection_release_v890_ies(
    uecc_ue_context_t*  p_uecc_ue_context,        /* UE context */
    rrm_rrc_connection_release_v890_ies_t   *p_rrm_rcr_v890,
    RRCConnectionRelease_v890_IEs           *p_asn1_rcr_v890,
    OSCTXT                                  *p_asn1_ctx
);

/* CR_220 */
/******************************************************************************
*   FUNCTION NAME : uecc_llim_build_asn1_rlf_timers_and_constants_r9
*
*   DESCRIPTION   : This function builds rlf_timers_and_constants_r9 field for
*                   radio_resource_config_dedicated.
*
*   RETURNS       : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_rlf_timers_and_constants_r9(
    uecc_ue_context_t*             p_uecc_ue_context,
    rlf_timers_and_constants_r9_t *p_rlf,
    RLF_TimersAndConstants_r9     *p_asn_rlf,
    OSCTXT                        *p_asn1_ctx
);

/* ERAB SETUP STOP */


void uecc_llim_fill_sps_config
(
        rrc_sps_config_t        *p_uecc_sps_config,
        rrc_sps_config_t         *p_llim_sps_config
);

/*CA_START*/
rrc_return_et uecc_llim_build_asn1_add_modify_scell_config
(
    uecc_ue_context_t               *p_uecc_ue_context,
    rrc_scell_to_add_mod_list_t     *p_scell_add_modify,
    SCellToAddModList_r10           *p_asn1_scell_add_modify_list,
    OSCTXT                          *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_rel_scell_config
(
    uecc_ue_context_t               *p_uecc_ue_context,
    rrc_scell_to_release_list_t     *p_scell_rel,
    SCellToReleaseList_r10          *p_asn1_scell_rel
    /*SPR 17777 +-*/
);

rrc_return_et uecc_llim_build_asn1_mac_main_config_v1020
(
    uecc_ue_context_t                   *p_uecc_ue_context,
    rrc_mac_main_config_v1020_t const   *p_mac_main_config_v1020,
    MAC_MainConfig_mac_MainConfig_v1020 *p_mac_MainConfig_v1020
    /*SPR 17777 +-*/
);

rrc_return_et   uecc_llim_build_asn1_radio_res_config_dedicated_scell
(
   uecc_ue_context_t               *p_uecc_ue_context,
   rrc_radio_res_config_dedicated_scell_t *p_radio_res_config_dedicated_scell,
   RadioResourceConfigDedicatedSCell_r10 *p_asn1_radio_res_config_dedicated_scell,
   OSCTXT                                *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_radio_res_config_common_scell
(
   uecc_ue_context_t               *p_uecc_ue_context,
   rrc_radio_res_config_common_scell_t *p_radio_res_config_common_scell,
   RadioResourceConfigCommonSCell_r10 *p_asn1_radio_res_config_common_scell
   /*SPR 17777 +-*/
);

rrc_return_et uecc_llim_build_asn1_radio_res_config_dedicated_scell_non_ul
(
    uecc_ue_context_t               *p_uecc_ue_context,
    rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config,
    PhysicalConfigDedicatedSCell_r10_nonUL_Configuration_r10 *p_asn1_physical_dedicated_scell_non_ul_config,
    OSCTXT                                *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_radio_res_config_dedicated_scell_ul
(
    uecc_ue_context_t               *p_uecc_ue_context,
    rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config,
    PhysicalConfigDedicatedSCell_r10_ul_Configuration_r10 *p_ul_Configuration_r10,
    OSCTXT                                *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_antenna_info_ul_r10
(
    uecc_ue_context_t                 *p_uecc_ue_context,
    rrc_antenna_info_ul_r10_t const   *p_rrc_antenna_info_ul_r10,
    AntennaInfoUL_r10                 *p_asn1_antenna_info_ul_r10
    /*SPR 17777 +-*/
);

rrc_return_et uecc_llim_build_asn1_cqi_report_config_r10
(
    uecc_ue_context_t                  *p_uecc_ue_context,
    rrc_cqi_report_config_r10_t const  *p_rrc_cqi_report_config_r10,
    CQI_ReportConfig_r10               *p_asn1_cqi_report_config_r10,
    OSCTXT                             *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_cqi_report_config_aperiodic_r10
(
    uecc_ue_context_t                         *p_uecc_ue_context,
    rrc_phy_cqi_report_aperiodic_r10_t const  *p_rrc_cqi_report_config_r10,
    CQI_ReportAperiodic_r10                   *p_asn1_cqi_report_config_r10,
    OSCTXT                                    *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_cqi_report_config_csi_subframe_pattern_r10
(
    uecc_ue_context_t                                   *p_uecc_ue_context,
    rrc_phy_cqi_report_csi_subframePattern_r10_t const  *p_csi_pattern_r10,
    CQI_ReportConfig_r10_csi_SubframePatternConfig_r10  *p_asn1_csi_pattern_r10,
    OSCTXT                                              *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_phy_meas_subframe_pattern
(
    uecc_ue_context_t                          *p_uecc_ue_context,
    rrc_phy_meas_subframe_pattern_r10_t const  *p_phy_meas_subframe_pattern,
    MeasSubframePattern_r10                    *p_asn1_meas_subframe_pattern,
    OSCTXT                                     *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_additional_spectrum_ca_r10
(
    uecc_ue_context_t                                        *p_uecc_ue_context,
    rrc_additional_spectrum_emission_ca_r10_t  const         *p_rrc_spctrm_emison_ca,
    PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10 *p_asn_spctrm_emison_ca,
    OSCTXT                                                   *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_pucch_format_channel_selection_r10
(
    uecc_ue_context_t                                  *p_uecc_ue_context,
    rrc_pucch_channel_selection_config_v1020_t const   *p_rrc_channel_selection,
    PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10
                                                       *p_asn_channel_selection,
    OSCTXT                                             *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_antenna_info_dedicated_r10
(
    uecc_ue_context_t                         *p_uecc_ue_context,
    rrc_antenna_info_dedicated_r10_t const    *p_rrc_antenna_info_dedicate_r10,
    AntennaInfoDedicated_r10                  *p_asn1_antenna_info_dedicated_r10,
    OSCTXT                                    *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_cqi_report_config_periodic_r10
(
    uecc_ue_context_t                         *p_uecc_ue_context,
    rrc_phy_cqi_report_periodic_r10_t  const  *p_rrc_phy_cqi_report_periodic, 
    CQI_ReportPeriodic_r10                    *p_asn_phy_cqi_report_periodic,
    OSCTXT                                    *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_cqi_format_ind_periodic
(
    uecc_ue_context_t                            *p_uecc_ue_context,
    rrc_phy_cqi_format_ind_periodic_r10_t const  *p_rrc_cqi_format_ind_periodic,
    CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
                                                 *p_asn_cqi_format_ind_periodic,
    OSCTXT                                       *p_asn1_ctx
);

rrc_return_et uecc_llim_build_asn1_phy_ca_config_dedicated(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
);

/*CA_STOP*/

/*SPR 22036 Fix Start*/
rrc_return_et uecc_llim_build_asn1_modify_drb_to_release_list(
    uecc_ue_context_t*   p_uecc_ue_context,
    DRB_ToReleaseList*   p_asn1_list,
    OSCTXT*              p_asn1_ctx
);

/*SPR 22036 Fix Stop*/


#ifdef ENDC_ENABLED
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasConfig_endc
*   DESCRIPTION  : This function builds MeasConfig field in
                   RRCConnectionReconfiguration message
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
******************************************************************************/
rrc_return_et uecc_llim_build_MeasConfig_endc
(
    uecc_ue_context_t            *p_uecc_ue_context,
    rrc_rrm_endc_meas_config_t*  p_rrm_meas_config,
    OSCTXT*                      p_asn1_ctx,
    MeasConfig*                  p_meas_config
);
#endif
#if  defined(ENDC_ENABLED)
/******************************************************************************
 * FUNCTION NAME : uecc_llim_build_and_send_pdcp_lwa_traffic_state_update_ind 
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 lwa_traffic_ind_et  flag
 *
 * OUTPUT        : None
 *
 * DESCRIPTION   : This function build and send indication to PDCP to 
 *                 START/STOP LWA Traffic.
 *
 * RETURNS       : None
 ******************************************************************************/
void uecc_llim_build_and_send_pdcp_lwa_traffic_state_update_ind
(
    uecc_ue_context_t*  p_ue_context,
    lwa_traffic_ind_et  flag
);
#endif
#endif /* UECC_LLIM_UTILS_H */
