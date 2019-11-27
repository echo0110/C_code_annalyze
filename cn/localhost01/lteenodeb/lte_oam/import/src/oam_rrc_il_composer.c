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
 *  File Description : The file rrc_oam_il_composer.c contains the definitions 
 *                     of rrc-oam interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "oam_rrc_il_composer.h"
#include "rrc_oam_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
static oam_error_code_et oam_error = NO_ERROR;


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ext_comm_info_len
(
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ext_comm_info
(
    UInt8  **pp_buffer,
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ext_connection_status_len
(
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ext_connection_status
(
    UInt8  **pp_buffer,
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_integrity_algorithms_len
(
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_integrity_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ciphering_algorithms_len
(
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ciphering_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_supported_security_algorithms_len
(
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_supported_security_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_sector_info_len
(
    rrc_oam_sector_info_t *p_rrc_oam_sector_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_sector_info
(
    UInt8  **pp_buffer,
    rrc_oam_sector_info_t *p_rrc_oam_sector_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_sector_info_list_len
(
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_sector_info_list
(
    UInt8  **pp_buffer,
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_timer_info_len
(
    rrc_oam_timer_info_t *p_rrc_oam_timer_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_timer_info_t *p_rrc_oam_timer_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_llim_timer_info_len
(
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_llim_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_csc_timer_info_len
(
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_csc_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_uecc_timer_info_len
(
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_uecc_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_module_timers_info_len
(
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_module_timers_info
(
    UInt8  **pp_buffer,
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_shared_mem_config_len
(
    rrc_shared_mem_config_t *p_rrc_shared_mem_config
);

static
oam_return_et
oam_rrc_il_compose_rrc_shared_mem_config
(
    UInt8  **pp_buffer,
    rrc_shared_mem_config_t *p_rrc_shared_mem_config
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_connection_establishment_len
(
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_connection_establishment
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_connection_reestablishment_len
(
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_connection_reestablishment
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment
);

static
rrc_length_t
oam_rrc_il_get_rrc_connEstabTime_len
(
    rrc_connEstabTime_t *p_rrc_connEstabTime
);

static
oam_return_et
oam_rrc_il_compose_rrc_connEstabTime
(
    UInt8  **pp_buffer,
    rrc_connEstabTime_t *p_rrc_connEstabTime
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_len
(
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_ue_context_release_len
(
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_ue_context_release
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_connection_related_stats_len
(
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_connection_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_estab_init_fail_len
(
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_estab_init_fail
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_estab_add_fail_len
(
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_estab_add_fail
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail
);

static
rrc_length_t
oam_rrc_il_get_rrc_erab_estabTime_len
(
    rrc_erab_estabTime_t *p_rrc_erab_estabTime
);

static
oam_return_et
oam_rrc_il_compose_rrc_erab_estabTime
(
    UInt8  **pp_buffer,
    rrc_erab_estabTime_t *p_rrc_erab_estabTime
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_setup_len
(
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_setup
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_rel_enb_cause_len
(
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_rel_enb_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_rel_fail_cause_len
(
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_rel_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_release_len
(
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_release
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_mod_fail_cause_len
(
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_mod_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_modification_len
(
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_modification
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_number_len
(
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_number
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_related_stats_len
(
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IntraEnbOutAtt_cause_len
(
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IntraEnbOutSucc_cause_len
(
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_intra_enb_ho_stats_len
(
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_intra_enb_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_InterEnbOutAtt_cause_len
(
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_InterEnbOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_InterEnbOutSucc_cause_len
(
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_InterEnbOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_enb_ho_stats_len
(
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_enb_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_mnc_len
(
    mnc_t *p_mnc
);

static
oam_return_et
oam_rrc_il_compose_mnc
(
    UInt8  **pp_buffer,
    mnc_t *p_mnc
);

static
rrc_length_t
oam_rrc_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
);

static
oam_return_et
oam_rrc_il_compose_plmn_identity
(
    UInt8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
);

static
rrc_length_t
oam_rrc_il_get_cell_global_id_eutra_len
(
    cell_global_id_eutra_t *p_cell_global_id_eutra
);

static
oam_return_et
oam_rrc_il_compose_cell_global_id_eutra
(
    UInt8  **pp_buffer,
    cell_global_id_eutra_t *p_cell_global_id_eutra
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_OutAttTarget_cause_len
(
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_OutAttTarget_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_OutSuccTarget_cause_len
(
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_OutSuccTarget_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_len
(
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_list_len
(
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats_list
(
    UInt8  **pp_buffer,
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_intra_freq_ho_stats_len
(
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_intra_freq_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_freq_ho_stats_len
(
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_freq_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_drx_non_drx_ho_stats_len
(
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_drx_non_drx_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_utran_geran_cgi_len
(
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi
);

static
oam_return_et
oam_rrc_il_compose_rrc_utran_geran_cgi
(
    UInt8  **pp_buffer,
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi
);

static
rrc_length_t
oam_rrc_il_get_cdma2000_1xrtt_cell_identifier_len
(
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier
);

static
oam_return_et
oam_rrc_il_compose_cdma2000_1xrtt_cell_identifier
(
    UInt8  **pp_buffer,
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier
);

static
rrc_length_t
oam_rrc_il_get_cdma2000_hrpd_sector_identifier_len
(
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier
);

static
oam_return_et
oam_rrc_il_compose_cdma2000_hrpd_sector_identifier
(
    UInt8  **pp_buffer,
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier
);

static
rrc_length_t
oam_rrc_il_get_rrc_inter_rat_global_cgi_len
(
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi
);

static
oam_return_et
oam_rrc_il_compose_rrc_inter_rat_global_cgi
(
    UInt8  **pp_buffer,
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IratOutAtt_cause_len
(
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IratOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IratOutSucc_cause_len
(
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IratOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_len
(
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_list_len
(
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats_list
(
    UInt8  **pp_buffer,
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_related_stats_len
(
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_ue_assoc_s1_connection_len
(
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection
);

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_ue_assoc_s1_connection
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection
);

static
rrc_length_t
oam_rrc_il_get_rrc_csc_oam_stat_paging_len
(
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging
);

static
oam_return_et
oam_rrc_il_compose_rrc_csc_oam_stat_paging
(
    UInt8  **pp_buffer,
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_connection_stats_len
(
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_connection_stats
(
    UInt8  **pp_buffer,
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_stats_len
(
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_stats
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_stats_len
(
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_stats
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_rrc_connection_stats_info_len
(
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_rrc_connection_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_stats_info_len
(
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_stats_info_len
(
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_sampling_period_info_len
(
    rrc_sampling_period_info_t *p_rrc_sampling_period_info
);

static
oam_return_et
oam_rrc_il_compose_rrc_sampling_period_info
(
    UInt8  **pp_buffer,
    rrc_sampling_period_info_t *p_rrc_sampling_period_info
);

static
rrc_length_t
oam_rrc_il_get_rrc_connection_stats_len
(
    rrc_connection_stats_t *p_rrc_connection_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_connection_stats
(
    UInt8  **pp_buffer,
    rrc_connection_stats_t *p_rrc_connection_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_handover_stats_len
(
    rrc_handover_stats_t *p_rrc_handover_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_handover_stats
(
    UInt8  **pp_buffer,
    rrc_handover_stats_t *p_rrc_handover_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_erab_stats_len
(
    rrc_erab_stats_t *p_rrc_erab_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_erab_stats
(
    UInt8  **pp_buffer,
    rrc_erab_stats_t *p_rrc_erab_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_ue_assoc_s1_conn_stats_len
(
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_ue_assoc_s1_conn_stats
(
    UInt8  **pp_buffer,
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_paging_stats_len
(
    rrc_paging_stats_t *p_rrc_paging_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_paging_stats
(
    UInt8  **pp_buffer,
    rrc_paging_stats_t *p_rrc_paging_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_irat_mobility_stats_len
(
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats
);

static
oam_return_et
oam_rrc_il_compose_rrc_irat_mobility_stats
(
    UInt8  **pp_buffer,
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_cell_status_len
(
    rrc_oam_cell_status_t *p_rrc_oam_cell_status
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_cell_status
(
    UInt8  **pp_buffer,
    rrc_oam_cell_status_t *p_rrc_oam_cell_status
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ue_status_len
(
    rrc_oam_ue_status_t *p_rrc_oam_ue_status
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ue_status
(
    UInt8  **pp_buffer,
    rrc_oam_ue_status_t *p_rrc_oam_ue_status
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ue_status_list_len
(
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ue_status_list
(
    UInt8  **pp_buffer,
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list
);

static
rrc_length_t
oam_rrc_il_get_ueRrcInfoStatus_len
(
    ueRrcInfoStatus_t *p_ueRrcInfoStatus
);

static
oam_return_et
oam_rrc_il_compose_ueRrcInfoStatus
(
    UInt8  **pp_buffer,
    ueRrcInfoStatus_t *p_ueRrcInfoStatus
);

static
rrc_length_t
oam_rrc_il_get_oam_rrc_ps_stats_resp_len
(
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp
);

static
oam_return_et
oam_rrc_il_compose_oam_rrc_ps_stats_resp
(
    UInt8  **pp_buffer,
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_trace_collection_entity_len
(
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_trace_collection_entity
(
    UInt8  **pp_buffer,
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_e_utran_trace_id_len
(
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_e_utran_trace_id
(
    UInt8  **pp_buffer,
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id
);

static
rrc_length_t
oam_rrc_il_get_rrc_oam_trace_file_len
(
    rrc_oam_trace_file_t *p_rrc_oam_trace_file
);

static
oam_return_et
oam_rrc_il_compose_rrc_oam_trace_file
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_t *p_rrc_oam_trace_file
);

static
rrc_length_t
oam_rrc_il_get_rrc_trace_failure_list_item_len
(
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item
);

static
oam_return_et
oam_rrc_il_compose_rrc_trace_failure_list_item
(
    UInt8  **pp_buffer,
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item
);

static
rrc_length_t
oam_rrc_il_get_rrc_trace_failure_list_len
(
    rrc_trace_failure_list_t *p_rrc_trace_failure_list
);

static
oam_return_et
oam_rrc_il_compose_rrc_trace_failure_list
(
    UInt8  **pp_buffer,
    rrc_trace_failure_list_t *p_rrc_trace_failure_list
);

static
rrc_length_t
oam_rrc_il_get_rrc_PoolStats_len
(
    rrc_PoolStats *p_rrc_PoolStats
);

static
oam_return_et
oam_rrc_il_compose_rrc_PoolStats
(
    UInt8  **pp_buffer,
    rrc_PoolStats *p_rrc_PoolStats
);

static
rrc_length_t
oam_rrc_il_get_rrc_MemPoolStats_len
(
    rrc_MemPoolStats *p_rrc_MemPoolStats
);

static
oam_return_et
oam_rrc_il_compose_rrc_MemPoolStats
(
    UInt8  **pp_buffer,
    rrc_MemPoolStats *p_rrc_MemPoolStats
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
oam_rrc_il_get_rrc_oam_communication_info_req_len
(
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_communication_info_req != PNULL);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_req->ext_comm_info_upper_layer); loop++)
        {
            length += oam_rrc_il_get_rrc_oam_ext_comm_info_len(&p_rrc_oam_communication_info_req->ext_comm_info_upper_layer[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_communication_info_req
(
    UInt8  **pp_buffer,
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_communication_info_req != PNULL);

    /* This function composes rrc_oam_communication_info_req */
    

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_req->ext_comm_info_upper_layer); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ext_comm_info(pp_buffer, &p_rrc_oam_communication_info_req->ext_comm_info_upper_layer[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_communication_info_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_COMMUNICATION_INFO_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_communication_info_req
(
    rrc_oam_communication_info_req_t  *p_rrc_oam_communication_info_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_communication_info_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_communication_info_req_len(p_rrc_oam_communication_info_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_COMMUNICATION_INFO_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_COMMUNICATION_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_COMMUNICATION_INFO_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_COMMUNICATION_INFO_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_communication_info_req(&p_oam_msg, p_rrc_oam_communication_info_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ext_comm_info_len
(
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ext_comm_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_comm_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_comm_info->ext_module_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_comm_info->transport_mode);

    /* Optional element */
    if(p_rrc_oam_ext_comm_info->bitmask & RRC_OAM_IP_ADDR_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ext_comm_info->ip_addr);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_comm_info->port);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ext_comm_info
(
    UInt8  **pp_buffer,
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ext_comm_info != PNULL);

    /* This function composes rrc_oam_ext_comm_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ext_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_ext_comm_info->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ext_comm_info->ext_module_id < 1) || (p_rrc_oam_ext_comm_info->ext_module_id > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_comm_info->ext_module_id] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_ext_comm_info->ext_module_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ext_comm_info->ext_module_id, "ext_module_id");
    *pp_buffer += sizeof(p_rrc_oam_ext_comm_info->ext_module_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ext_comm_info->transport_mode > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_comm_info->transport_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_ext_comm_info->transport_mode);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ext_comm_info->transport_mode, "transport_mode");
    *pp_buffer += sizeof(p_rrc_oam_ext_comm_info->transport_mode);

    /* Optional element */
    if(p_rrc_oam_ext_comm_info->bitmask & RRC_OAM_IP_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ext_comm_info->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ext_comm_info->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ext_comm_info->port, "port");
    *pp_buffer += sizeof(p_rrc_oam_ext_comm_info->port);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_communication_info_resp_len
(
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_communication_info_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_communication_info_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_communication_info_resp->response);

    /* Optional element */
    if(p_rrc_oam_communication_info_resp->bitmask & RRC_OAM_COMM_INFO_FAIL_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_communication_info_resp->cause);
    }

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer); loop++)
        {
            length += oam_rrc_il_get_rrc_oam_ext_connection_status_len(&p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_communication_info_resp
(
    UInt8  **pp_buffer,
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_communication_info_resp != PNULL);

    /* This function composes rrc_oam_communication_info_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_communication_info_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_communication_info_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_communication_info_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_communication_info_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_communication_info_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_communication_info_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_communication_info_resp->response);

    /* Optional element */
    if(p_rrc_oam_communication_info_resp->bitmask & RRC_OAM_COMM_INFO_FAIL_CAUSE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_communication_info_resp->cause > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_communication_info_resp->cause] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_communication_info_resp->cause);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_communication_info_resp->cause, "cause");
    *pp_buffer += sizeof(p_rrc_oam_communication_info_resp->cause);
    }

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ext_connection_status(pp_buffer, &p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_communication_info_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_COMMUNICATION_INFO_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_communication_info_resp
(
    rrc_oam_communication_info_resp_t  *p_rrc_oam_communication_info_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_communication_info_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_communication_info_resp_len(p_rrc_oam_communication_info_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_COMMUNICATION_INFO_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_COMMUNICATION_INFO_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_COMMUNICATION_INFO_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_COMMUNICATION_INFO_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_communication_info_resp(&p_oam_msg, p_rrc_oam_communication_info_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ext_connection_status_len
(
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ext_connection_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_connection_status->ext_module_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ext_connection_status->connection_status);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ext_connection_status
(
    UInt8  **pp_buffer,
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ext_connection_status != PNULL);

    /* This function composes rrc_oam_ext_connection_status */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ext_connection_status->ext_module_id < 1) || (p_rrc_oam_ext_connection_status->ext_module_id > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_connection_status->ext_module_id] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_ext_connection_status->ext_module_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ext_connection_status->ext_module_id, "ext_module_id");
    *pp_buffer += sizeof(p_rrc_oam_ext_connection_status->ext_module_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ext_connection_status->connection_status > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_connection_status->connection_status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_ext_connection_status->connection_status);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ext_connection_status->connection_status, "connection_status");
    *pp_buffer += sizeof(p_rrc_oam_ext_connection_status->connection_status);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_provision_req_len
(
    rrc_oam_provision_req_t *p_rrc_oam_provision_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_provision_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->log_level);

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_LOG_FILE_NAME_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_provision_req->log_file_name);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->phy_sync_mode);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_sector_info_list_len(&p_rrc_oam_provision_req->sector_info_list);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_llim_timer_info_len(&p_rrc_oam_provision_req->llim_timer_info);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_csc_timer_info_len(&p_rrc_oam_provision_req->csc_timer_info);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_uecc_timer_info_len(&p_rrc_oam_provision_req->uecc_timer_info);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->non_broadcast_sfn_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->max_cell_reconfig_retry_count);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->rest_rrc_core_no);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->no_of_uecc_instances);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_provision_req->no_of_uecc_instances * sizeof(p_rrc_oam_provision_req->core_no[0]));
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->len_vendor_name);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_provision_req->len_vendor_name * sizeof(p_rrc_oam_provision_req->vendor_name[0]));

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->pdcp_integrity_threshold_counter);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_MODULE_TIMERS_INFO)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_module_timers_info_len(&p_rrc_oam_provision_req->module_timers_info);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_CONFIG_OPTION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->advance_logging_config_option);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_NETWORK_IP_PORT_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_provision_req->network_ip_port);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_req->ue_context_list_size);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_SHARED_MEM_NAME_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_shared_mem_config_len(&p_rrc_oam_provision_req->shared_mem_logging_config);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_provision_req
(
    UInt8  **pp_buffer,
    rrc_oam_provision_req_t *p_rrc_oam_provision_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_provision_req != PNULL);

    /* This function composes rrc_oam_provision_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_provision_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_provision_req->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->log_level, "log_level");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->log_level);

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_LOG_FILE_NAME_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_provision_req->log_file_name); loop++)
        {
            OAM_CP_PACK_SINT8(*pp_buffer, &p_rrc_oam_provision_req->log_file_name[loop], "log_file_name[]");
            *pp_buffer += sizeof(SInt8);
        }
    }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->phy_sync_mode, "phy_sync_mode");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->phy_sync_mode);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_sector_info_list(pp_buffer, &p_rrc_oam_provision_req->sector_info_list))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_llim_timer_info(pp_buffer, &p_rrc_oam_provision_req->llim_timer_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_csc_timer_info(pp_buffer, &p_rrc_oam_provision_req->csc_timer_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_uecc_timer_info(pp_buffer, &p_rrc_oam_provision_req->uecc_timer_info))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->non_broadcast_sfn_offset > 1023))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->non_broadcast_sfn_offset] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_oam_provision_req->non_broadcast_sfn_offset);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_provision_req->non_broadcast_sfn_offset, "non_broadcast_sfn_offset");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->non_broadcast_sfn_offset);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->max_cell_reconfig_retry_count, "max_cell_reconfig_retry_count");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->max_cell_reconfig_retry_count);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->rest_rrc_core_no, "rest_rrc_core_no");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->rest_rrc_core_no);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_provision_req->no_of_uecc_instances < 1) || (p_rrc_oam_provision_req->no_of_uecc_instances > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->no_of_uecc_instances] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_provision_req->no_of_uecc_instances);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->no_of_uecc_instances, "no_of_uecc_instances");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->no_of_uecc_instances);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_provision_req->no_of_uecc_instances; loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->core_no[loop], "core_no[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_provision_req->len_vendor_name < 1) || (p_rrc_oam_provision_req->len_vendor_name > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->len_vendor_name] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_provision_req->len_vendor_name);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->len_vendor_name, "len_vendor_name");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->len_vendor_name);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_provision_req->len_vendor_name; loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->vendor_name[loop], "vendor_name[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_provision_req->pdcp_integrity_threshold_counter, "pdcp_integrity_threshold_counter");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->pdcp_integrity_threshold_counter);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_MODULE_TIMERS_INFO)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_module_timers_info(pp_buffer, &p_rrc_oam_provision_req->module_timers_info))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_CONFIG_OPTION_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->advance_logging_config_option > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->advance_logging_config_option] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_oam_provision_req->advance_logging_config_option);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->advance_logging_config_option, "advance_logging_config_option");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->advance_logging_config_option);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_NETWORK_IP_PORT_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_provision_req->network_ip_port); loop++)
        {
            OAM_CP_PACK_SINT8(*pp_buffer, &p_rrc_oam_provision_req->network_ip_port[loop], "network_ip_port[]");
            *pp_buffer += sizeof(SInt8);
        }
    }
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_req->ue_context_list_size, "ue_context_list_size");
    *pp_buffer += sizeof(p_rrc_oam_provision_req->ue_context_list_size);
    }

    /* Optional element */
    if(p_rrc_oam_provision_req->bitmask & RRC_OAM_SHARED_MEM_NAME_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_shared_mem_config(pp_buffer, &p_rrc_oam_provision_req->shared_mem_logging_config))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_provision_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_PROVISION_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_provision_req
(
    rrc_oam_provision_req_t  *p_rrc_oam_provision_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_provision_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_provision_req_len(p_rrc_oam_provision_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_PROVISION_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_PROVISION_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_PROVISION_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_PROVISION_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_provision_req(&p_oam_msg, p_rrc_oam_provision_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_integrity_algorithms_len
(
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_integrity_algorithms != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_integrity_algorithms->num_algorithms);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_integrity_algorithms->num_algorithms * sizeof(p_rrc_oam_integrity_algorithms->algorithms[0]));

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_integrity_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_integrity_algorithms != PNULL);

    /* This function composes rrc_oam_integrity_algorithms */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_integrity_algorithms->num_algorithms < 1) || (p_rrc_oam_integrity_algorithms->num_algorithms > RRC_MAX_INTEGRITY_ALGORITHMS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_integrity_algorithms->num_algorithms] should be in range "
            "1 to RRC_MAX_INTEGRITY_ALGORITHMS. Incorrect value %u received.", p_rrc_oam_integrity_algorithms->num_algorithms);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_integrity_algorithms->num_algorithms, "num_algorithms");
    *pp_buffer += sizeof(p_rrc_oam_integrity_algorithms->num_algorithms);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_integrity_algorithms->num_algorithms; loop++)
        {
            if (p_rrc_oam_integrity_algorithms->algorithms[loop] > 2)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_rrc_oam_integrity_algorithms->algorithms[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_rrc_oam_integrity_algorithms->algorithms[loop]);
                return OAM_FAILURE;
            }
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_integrity_algorithms->algorithms[loop], "algorithms[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ciphering_algorithms_len
(
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ciphering_algorithms != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ciphering_algorithms->num_algorithms);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_ciphering_algorithms->num_algorithms * sizeof(p_rrc_oam_ciphering_algorithms->algorithms[0]));

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ciphering_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ciphering_algorithms != PNULL);

    /* This function composes rrc_oam_ciphering_algorithms */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ciphering_algorithms->num_algorithms < 1) || (p_rrc_oam_ciphering_algorithms->num_algorithms > RRC_MAX_CIPHERING_ALGORITHMS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ciphering_algorithms->num_algorithms] should be in range "
            "1 to RRC_MAX_CIPHERING_ALGORITHMS. Incorrect value %u received.", p_rrc_oam_ciphering_algorithms->num_algorithms);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ciphering_algorithms->num_algorithms, "num_algorithms");
    *pp_buffer += sizeof(p_rrc_oam_ciphering_algorithms->num_algorithms);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ciphering_algorithms->num_algorithms; loop++)
        {
            if (p_rrc_oam_ciphering_algorithms->algorithms[loop] > 2)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_rrc_oam_ciphering_algorithms->algorithms[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_rrc_oam_ciphering_algorithms->algorithms[loop]);
                return OAM_FAILURE;
            }
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ciphering_algorithms->algorithms[loop], "algorithms[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_supported_security_algorithms_len
(
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_supported_security_algorithms != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_integrity_algorithms_len(&p_rrc_oam_supported_security_algorithms->integrity_algorithms);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ciphering_algorithms_len(&p_rrc_oam_supported_security_algorithms->ciphering_algorithms);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_supported_security_algorithms
(
    UInt8  **pp_buffer,
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_supported_security_algorithms != PNULL);

    /* This function composes rrc_oam_supported_security_algorithms */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_integrity_algorithms(pp_buffer, &p_rrc_oam_supported_security_algorithms->integrity_algorithms))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ciphering_algorithms(pp_buffer, &p_rrc_oam_supported_security_algorithms->ciphering_algorithms))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_sector_info_len
(
    rrc_oam_sector_info_t *p_rrc_oam_sector_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_sector_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_sector_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_sector_info->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_sector_info->max_num_supported_ue);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_supported_security_algorithms_len(&p_rrc_oam_sector_info->supported_security_algorithms);

    /* Optional element */
    if(p_rrc_oam_sector_info->bitmask & RRC_OAM_MAX_NUM_SUPPORTED_EMTC_UE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_sector_info->maxNumSupportedEmtcUe);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_sector_info
(
    UInt8  **pp_buffer,
    rrc_oam_sector_info_t *p_rrc_oam_sector_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_sector_info != PNULL);

    /* This function composes rrc_oam_sector_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_sector_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_sector_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_sector_info->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_sector_info->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_sector_info->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_sector_info->cell_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info->max_num_supported_ue < 1) || (p_rrc_oam_sector_info->max_num_supported_ue > RRC_MAX_NUM_SUPPORTED_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->max_num_supported_ue] should be in range "
            "1 to RRC_MAX_NUM_SUPPORTED_UE. Incorrect value %u received.", p_rrc_oam_sector_info->max_num_supported_ue);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_sector_info->max_num_supported_ue, "max_num_supported_ue");
    *pp_buffer += sizeof(p_rrc_oam_sector_info->max_num_supported_ue);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_supported_security_algorithms(pp_buffer, &p_rrc_oam_sector_info->supported_security_algorithms))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrc_oam_sector_info->bitmask & RRC_OAM_MAX_NUM_SUPPORTED_EMTC_UE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info->maxNumSupportedEmtcUe < 1) || (p_rrc_oam_sector_info->maxNumSupportedEmtcUe > MAX_NUM_SUPPORTED_EMTC_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->maxNumSupportedEmtcUe] should be in range "
            "1 to MAX_NUM_SUPPORTED_EMTC_UE. Incorrect value %u received.", p_rrc_oam_sector_info->maxNumSupportedEmtcUe);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_sector_info->maxNumSupportedEmtcUe, "maxNumSupportedEmtcUe");
    *pp_buffer += sizeof(p_rrc_oam_sector_info->maxNumSupportedEmtcUe);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_sector_info_list_len
(
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_sector_info_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_sector_info_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info_list->count < 1) || (p_rrc_oam_sector_info_list->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_sector_info_list->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_sector_info_list->count; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_sector_info_len(&p_rrc_oam_sector_info_list->sector_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_sector_info_list
(
    UInt8  **pp_buffer,
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_sector_info_list != PNULL);

    /* This function composes rrc_oam_sector_info_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info_list->count < 1) || (p_rrc_oam_sector_info_list->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_sector_info_list->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_sector_info_list->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_sector_info_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_sector_info_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_sector_info(pp_buffer, &p_rrc_oam_sector_info_list->sector_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_timer_info_len
(
    rrc_oam_timer_info_t *p_rrc_oam_timer_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_timer_info->timer_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_timer_info->timer_val);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_timer_info_t *p_rrc_oam_timer_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_timer_info != PNULL);

    /* This function composes rrc_oam_timer_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_timer_info->timer_id > 41))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_timer_info->timer_id] should be less than"
            " or equal to 41. Incorrect value %u received.", p_rrc_oam_timer_info->timer_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_timer_info->timer_id, "timer_id");
    *pp_buffer += sizeof(p_rrc_oam_timer_info->timer_id);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_timer_info->timer_val < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_timer_info->timer_val] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_timer_info->timer_val);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_timer_info->timer_val, "timer_val");
    *pp_buffer += sizeof(p_rrc_oam_timer_info->timer_val);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_llim_timer_info_len
(
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_llim_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_llim_timer_info->num_of_timers);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_llim_timer_info->num_of_timers < 14) || (p_rrc_oam_llim_timer_info->num_of_timers > MAX_LLIM_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_llim_timer_info->num_of_timers] should be in range "
            "14 to MAX_LLIM_TIMER_INFO. Incorrect value %d received.", p_rrc_oam_llim_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_llim_timer_info->num_of_timers; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_timer_info_len(&p_rrc_oam_llim_timer_info->timer_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_llim_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_llim_timer_info != PNULL);

    /* This function composes rrc_oam_llim_timer_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_llim_timer_info->num_of_timers < 14) || (p_rrc_oam_llim_timer_info->num_of_timers > MAX_LLIM_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_llim_timer_info->num_of_timers] should be in range "
            "14 to MAX_LLIM_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_llim_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_llim_timer_info->num_of_timers, "num_of_timers");
    *pp_buffer += sizeof(p_rrc_oam_llim_timer_info->num_of_timers);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_llim_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_timer_info(pp_buffer, &p_rrc_oam_llim_timer_info->timer_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_csc_timer_info_len
(
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_csc_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_csc_timer_info->num_of_timers);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_csc_timer_info->num_of_timers < 7) || (p_rrc_oam_csc_timer_info->num_of_timers > MAX_CSC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_csc_timer_info->num_of_timers] should be in range "
            "7 to MAX_CSC_TIMER_INFO. Incorrect value %d received.", p_rrc_oam_csc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_csc_timer_info->num_of_timers; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_timer_info_len(&p_rrc_oam_csc_timer_info->timer_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_csc_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_csc_timer_info != PNULL);

    /* This function composes rrc_oam_csc_timer_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_csc_timer_info->num_of_timers < 7) || (p_rrc_oam_csc_timer_info->num_of_timers > MAX_CSC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_csc_timer_info->num_of_timers] should be in range "
            "7 to MAX_CSC_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_csc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_csc_timer_info->num_of_timers, "num_of_timers");
    *pp_buffer += sizeof(p_rrc_oam_csc_timer_info->num_of_timers);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_csc_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_timer_info(pp_buffer, &p_rrc_oam_csc_timer_info->timer_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_uecc_timer_info_len
(
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_uecc_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_uecc_timer_info->num_of_timers);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_uecc_timer_info->num_of_timers < 21) || (p_rrc_oam_uecc_timer_info->num_of_timers > MAX_UECC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_uecc_timer_info->num_of_timers] should be in range "
            "21 to MAX_UECC_TIMER_INFO. Incorrect value %d received.", p_rrc_oam_uecc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_uecc_timer_info->num_of_timers; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_timer_info_len(&p_rrc_oam_uecc_timer_info->timer_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_uecc_timer_info
(
    UInt8  **pp_buffer,
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_uecc_timer_info != PNULL);

    /* This function composes rrc_oam_uecc_timer_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_uecc_timer_info->num_of_timers < 21) || (p_rrc_oam_uecc_timer_info->num_of_timers > MAX_UECC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_uecc_timer_info->num_of_timers] should be in range "
            "21 to MAX_UECC_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_uecc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_uecc_timer_info->num_of_timers, "num_of_timers");
    *pp_buffer += sizeof(p_rrc_oam_uecc_timer_info->num_of_timers);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_uecc_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_timer_info(pp_buffer, &p_rrc_oam_uecc_timer_info->timer_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_module_timers_info_len
(
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_module_timers_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->bitmask);

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_integrity_threshold_timer_value);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_store_ue_context_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_ue_information_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->rrc_health_mon_time_int);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_PREP_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_dc_prep_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_module_timers_info
(
    UInt8  **pp_buffer,
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_module_timers_info != PNULL);

    /* This function composes rrc_oam_module_timers_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->bitmask);

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer, "uecc_intra_cell_ho_guard_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer, "uecc_ue_reconfig_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_integrity_threshold_timer_value, "uecc_integrity_threshold_timer_value");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_integrity_threshold_timer_value);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value, "uecc_cco_guard_timer_value");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_store_ue_context_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_store_ue_context_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_store_ue_context_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_store_ue_context_timer, "uecc_store_ue_context_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_store_ue_context_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer, "uecc_ue_counter_check_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_information_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_information_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_information_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_ue_information_timer, "uecc_ue_information_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_ue_information_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->rrc_health_mon_time_int, "rrc_health_mon_time_int");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->rrc_health_mon_time_int);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_PREP_TIMER_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_module_timers_info->uecc_dc_prep_timer < 50) || (p_rrc_oam_module_timers_info->uecc_dc_prep_timer > 65535))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_dc_prep_timer] should be in range "
            "50 to 65535. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_dc_prep_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_dc_prep_timer, "uecc_dc_prep_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_dc_prep_timer);
    }

    /* Optional element */
    if(p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer < 50) || (p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer > 65535))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer] should be in range "
            "50 to 65535. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer, "uecc_dc_bearer_change_guard_timer");
    *pp_buffer += sizeof(p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_shared_mem_config_len
(
    rrc_shared_mem_config_t *p_rrc_shared_mem_config
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_shared_mem_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_shared_mem_config->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_shared_mem_config->SharedMemSize);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_shared_mem_config->MaxLogFileSize);

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_shared_mem_config->LogFilePath);
    }

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_shared_mem_config->core_no);
    }

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_shared_mem_config->MaxLogFileCount);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_shared_mem_config
(
    UInt8  **pp_buffer,
    rrc_shared_mem_config_t *p_rrc_shared_mem_config
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_shared_mem_config != PNULL);

    /* This function composes rrc_shared_mem_config */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_shared_mem_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_shared_mem_config->bitmask);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->SharedMemSize < 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->SharedMemSize] should be greater than"
            " or equal to 3. Incorrect value %u received.", p_rrc_shared_mem_config->SharedMemSize);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_shared_mem_config->SharedMemSize, "SharedMemSize");
    *pp_buffer += sizeof(p_rrc_shared_mem_config->SharedMemSize);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->MaxLogFileSize < 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->MaxLogFileSize] should be greater than"
            " or equal to 3. Incorrect value %u received.", p_rrc_shared_mem_config->MaxLogFileSize);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_shared_mem_config->MaxLogFileSize, "MaxLogFileSize");
    *pp_buffer += sizeof(p_rrc_shared_mem_config->MaxLogFileSize);

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_shared_mem_config->LogFilePath); loop++)
        {
            OAM_CP_PACK_SINT8(*pp_buffer, &p_rrc_shared_mem_config->LogFilePath[loop], "LogFilePath[]");
            *pp_buffer += sizeof(SInt8);
        }
    }
    }

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_shared_mem_config->core_no, "core_no");
    *pp_buffer += sizeof(p_rrc_shared_mem_config->core_no);
    }

    /* Optional element */
    if(p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->MaxLogFileCount < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->MaxLogFileCount] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_shared_mem_config->MaxLogFileCount);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_shared_mem_config->MaxLogFileCount, "MaxLogFileCount");
    *pp_buffer += sizeof(p_rrc_shared_mem_config->MaxLogFileCount);
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_provision_resp_len
(
    rrc_oam_provision_resp_t *p_rrc_oam_provision_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_provision_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_provision_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_provision_resp
(
    UInt8  **pp_buffer,
    rrc_oam_provision_resp_t *p_rrc_oam_provision_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_provision_resp != PNULL);

    /* This function composes rrc_oam_provision_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_provision_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_provision_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_provision_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_provision_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_PROVISION_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_provision_resp
(
    rrc_oam_provision_resp_t  *p_rrc_oam_provision_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_provision_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_provision_resp_len(p_rrc_oam_provision_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_PROVISION_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_PROVISION_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_PROVISION_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_PROVISION_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_provision_resp(&p_oam_msg, p_rrc_oam_provision_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_log_level_resp_len
(
    rrc_oam_get_log_level_resp_t *p_rrc_oam_get_log_level_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_log_level_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_rrc_oam_get_log_level_resp->bitmask & RRC_OAM_LOG_LEVEL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_log_level_resp->log_level);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_log_level_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_log_level_resp_t *p_rrc_oam_get_log_level_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_log_level_resp != PNULL);

    /* This function composes rrc_oam_get_log_level_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_log_level_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_log_level_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_log_level_resp->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_log_level_resp->log_on_off);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_log_level_resp->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_rrc_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_rrc_oam_get_log_level_resp->bitmask & RRC_OAM_LOG_LEVEL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_log_level_resp->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_get_log_level_resp->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_log_level_resp->log_level, "log_level");
    *pp_buffer += sizeof(p_rrc_oam_get_log_level_resp->log_level);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_log_level_resp
(
    rrc_oam_get_log_level_resp_t  *p_rrc_oam_get_log_level_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_log_level_resp_len(p_rrc_oam_get_log_level_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_LOG_LEVEL_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_log_level_resp(&p_oam_msg, p_rrc_oam_get_log_level_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_set_log_level_req_len
(
    rrc_oam_set_log_level_req_t *p_rrc_oam_set_log_level_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_set_log_level_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_set_log_level_req->log_level);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_set_log_level_req
(
    UInt8  **pp_buffer,
    rrc_oam_set_log_level_req_t *p_rrc_oam_set_log_level_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_set_log_level_req != PNULL);

    /* This function composes rrc_oam_set_log_level_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_set_log_level_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_set_log_level_req->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_set_log_level_req->log_level, "log_level");
    *pp_buffer += sizeof(p_rrc_oam_set_log_level_req->log_level);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_set_log_level_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_SET_LOG_LEVEL_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_set_log_level_req
(
    rrc_oam_set_log_level_req_t  *p_rrc_oam_set_log_level_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_set_log_level_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_set_log_level_req_len(p_rrc_oam_set_log_level_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_SET_LOG_LEVEL_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_SET_LOG_LEVEL_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_SET_LOG_LEVEL_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_SET_LOG_LEVEL_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_set_log_level_req(&p_oam_msg, p_rrc_oam_set_log_level_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_set_log_level_resp_len
(
    rrc_oam_set_log_level_resp_t *p_rrc_oam_set_log_level_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_set_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_set_log_level_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_set_log_level_resp
(
    UInt8  **pp_buffer,
    rrc_oam_set_log_level_resp_t *p_rrc_oam_set_log_level_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_set_log_level_resp != PNULL);

    /* This function composes rrc_oam_set_log_level_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_set_log_level_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_set_log_level_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_set_log_level_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_set_log_level_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_set_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_SET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_set_log_level_resp
(
    rrc_oam_set_log_level_resp_t  *p_rrc_oam_set_log_level_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_set_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_set_log_level_resp_len(p_rrc_oam_set_log_level_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_SET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_SET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_SET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_SET_LOG_LEVEL_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_set_log_level_resp(&p_oam_msg, p_rrc_oam_set_log_level_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_log_enable_req_len
(
    rrc_oam_log_enable_req_t *p_rrc_oam_log_enable_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_log_enable_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_log_enable_req->log_on_off);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_log_enable_req
(
    UInt8  **pp_buffer,
    rrc_oam_log_enable_req_t *p_rrc_oam_log_enable_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_log_enable_req != PNULL);

    /* This function composes rrc_oam_log_enable_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_log_enable_req->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_log_enable_req->log_on_off);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_log_enable_req->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_rrc_oam_log_enable_req->log_on_off);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_log_enable_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_LOG_ENABLE_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_log_enable_req
(
    rrc_oam_log_enable_req_t  *p_rrc_oam_log_enable_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_log_enable_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_log_enable_req_len(p_rrc_oam_log_enable_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_LOG_ENABLE_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_LOG_ENABLE_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_LOG_ENABLE_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_LOG_ENABLE_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_log_enable_req(&p_oam_msg, p_rrc_oam_log_enable_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_log_enable_resp_len
(
    rrc_oam_log_enable_resp_t *p_rrc_oam_log_enable_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_log_enable_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_log_enable_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_log_enable_resp
(
    UInt8  **pp_buffer,
    rrc_oam_log_enable_resp_t *p_rrc_oam_log_enable_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_log_enable_resp != PNULL);

    /* This function composes rrc_oam_log_enable_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_log_enable_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_log_enable_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_log_enable_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_log_enable_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_log_enable_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_LOG_ENABLE_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_log_enable_resp
(
    rrc_oam_log_enable_resp_t  *p_rrc_oam_log_enable_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_log_enable_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_log_enable_resp_len(p_rrc_oam_log_enable_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_LOG_ENABLE_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_LOG_ENABLE_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_LOG_ENABLE_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_LOG_ENABLE_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_log_enable_resp(&p_oam_msg, p_rrc_oam_log_enable_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_cell_stats_resp_len
(
    rrc_oam_get_cell_stats_resp_t *p_rrc_oam_get_cell_stats_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_connection_related_stats_len(&p_rrc_oam_get_cell_stats_resp->rrc_connection_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_erab_related_stats_len(&p_rrc_oam_get_cell_stats_resp->rrc_erab_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_handover_related_stats_len(&p_rrc_oam_get_cell_stats_resp->rrc_handover_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_ue_assoc_s1_connection_len(&p_rrc_oam_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_csc_oam_stat_paging_len(&p_rrc_oam_get_cell_stats_resp->rrc_paging_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_resp->rrc_IratIncMobility_LAI);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_cell_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_stats_resp_t *p_rrc_oam_get_cell_stats_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_resp != PNULL);

    /* This function composes rrc_oam_get_cell_stats_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_cell_stats_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_resp->response > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_oam_get_cell_stats_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_connection_related_stats(pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_connection_related_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_erab_related_stats(pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_erab_related_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_handover_related_stats(pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_handover_related_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_ue_assoc_s1_connection(pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_csc_oam_stat_paging(pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_paging_related_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_get_cell_stats_resp->rrc_IratIncMobility_LAI, "rrc_IratIncMobility_LAI");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_resp->rrc_IratIncMobility_LAI);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_cell_stats_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_CELL_STATS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_stats_resp
(
    rrc_oam_get_cell_stats_resp_t  *p_rrc_oam_get_cell_stats_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_cell_stats_resp_len(p_rrc_oam_get_cell_stats_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_CELL_STATS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_CELL_STATS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_CELL_STATS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_CELL_STATS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_cell_stats_resp(&p_oam_msg, p_rrc_oam_get_cell_stats_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_connection_establishment_len
(
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_connection_establishment != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_sum);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFaileNBCause_Unspecified);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_Congestion);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_EnergySaving);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_connection_establishment
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_connection_establishment != PNULL);

    /* This function composes rrc_uecc_oam_stat_connection_establishment */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause[loop], "rrc_connEstabAtt_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_sum, "rrc_connEstabAtt_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause[loop], "rrc_connEstabSucc_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_sum, "rrc_connEstabSucc_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause[loop], "rrc_connEstabFail_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_sum, "rrc_connEstabFail_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_sum);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFaileNBCause_Unspecified, "rrc_connEstabFaileNBCause_Unspecified");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFaileNBCause_Unspecified);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_Congestion, "rrc_ConnEstabFaileNBCause_Congestion");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_Congestion);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_EnergySaving, "rrc_ConnEstabFaileNBCause_EnergySaving");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_EnergySaving);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_connection_reestablishment_len
(
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_connection_reestablishment != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_connection_reestablishment
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_connection_reestablishment != PNULL);

    /* This function composes rrc_uecc_oam_stat_connection_reestablishment */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause[loop], "rrc_connReEstabAtt_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_sum, "rrc_connReEstabAtt_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause[loop], "rrc_connReEstabSucc_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_sum, "rrc_connReEstabSucc_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause[loop], "rrc_connReEstabFail_cause[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_sum, "rrc_connReEstabFail_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_connEstabTime_len
(
    rrc_connEstabTime_t *p_rrc_connEstabTime
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_connEstabTime != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connEstabTime->count);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connEstabTime->rrc_connEstabTime_Sum);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connEstabTime->rrc_connEstabTime_Max);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_connEstabTime
(
    UInt8  **pp_buffer,
    rrc_connEstabTime_t *p_rrc_connEstabTime
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_connEstabTime != PNULL);

    /* This function composes rrc_connEstabTime */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_connEstabTime->count, "count");
    *pp_buffer += sizeof(p_rrc_connEstabTime->count);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_connEstabTime->rrc_connEstabTime_Sum, "rrc_connEstabTime_Sum");
    *pp_buffer += sizeof(p_rrc_connEstabTime->rrc_connEstabTime_Sum);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_connEstabTime->rrc_connEstabTime_Max, "rrc_connEstabTime_Max");
    *pp_buffer += sizeof(p_rrc_connEstabTime->rrc_connEstabTime_Max);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_len
(
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause != PNULL);

    /* This function composes rrc_uecc_oam_ue_ctxt_rel_req_fail_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork[loop], "ueCtxt_relReq_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport[loop], "ueCtxt_relReq_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas[loop], "ueCtxt_relReq_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol[loop], "ueCtxt_relReq_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc[loop], "ueCtxt_relReq_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_ue_context_release_len
(
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_ue_context_release != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_len(&p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_sum);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_ue_context_release->ueContext_RelSuccNbr);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_ue_context_release
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_ue_context_release != PNULL);

    /* This function composes rrc_uecc_oam_stat_ue_context_release */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause(pp_buffer, &p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_sum, "ueCtxt_relReq_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_sum);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_ue_context_release->ueContext_RelSuccNbr, "ueContext_RelSuccNbr");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_ue_context_release->ueContext_RelSuccNbr);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_connection_related_stats_len
(
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_connection_related_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_connection_establishment_len(&p_rrc_oam_connection_related_stats->rrc_connection_establishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_connection_reestablishment_len(&p_rrc_oam_connection_related_stats->rrc_connection_reestablishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_related_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause); loop++)
        {
            length += oam_rrc_il_get_rrc_connEstabTime_len(&p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause[loop]);
        }
    }
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_ue_context_release_len(&p_rrc_oam_connection_related_stats->ue_contxt_rel_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_connection_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_connection_related_stats != PNULL);

    /* This function composes rrc_oam_connection_related_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_connection_related_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_connection_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_connection_establishment(pp_buffer, &p_rrc_oam_connection_related_stats->rrc_connection_establishment))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_connection_reestablishment(pp_buffer, &p_rrc_oam_connection_related_stats->rrc_connection_reestablishment))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_connection_related_stats->rrcConnectionNumber, "rrcConnectionNumber");
    *pp_buffer += sizeof(p_rrc_oam_connection_related_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_connEstabTime(pp_buffer, &p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_ue_context_release(pp_buffer, &p_rrc_oam_connection_related_stats->ue_contxt_rel_stats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_estab_init_fail_len
(
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_estab_init_fail != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_estab_init_fail
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_estab_init_fail != PNULL);

    /* This function composes rrc_uecc_oam_erab_estab_init_fail */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork[loop], "erab_estabInitFailNbr_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport[loop], "erab_estabInitFailNbr_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas[loop], "erab_estabInitFailNbr_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol[loop], "erab_estabInitFailNbr_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc[loop], "erab_estabInitFailNbr_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_estab_add_fail_len
(
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_estab_add_fail != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_estab_add_fail
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_estab_add_fail != PNULL);

    /* This function composes rrc_uecc_oam_erab_estab_add_fail */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork[loop], "erab_estabAddFailNbr_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport[loop], "erab_estabAddFailNbr_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas[loop], "erab_estabAddFailNbr_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol[loop], "erab_estabAddFailNbr_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc[loop], "erab_estabAddFailNbr_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_erab_estabTime_len
(
    rrc_erab_estabTime_t *p_rrc_erab_estabTime
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_erab_estabTime != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_estabTime->count);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_estabTime->erab_estabTime_Sum);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_estabTime->erab_estabTime_Max);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_erab_estabTime
(
    UInt8  **pp_buffer,
    rrc_erab_estabTime_t *p_rrc_erab_estabTime
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_erab_estabTime != PNULL);

    /* This function composes rrc_erab_estabTime */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_erab_estabTime->count, "count");
    *pp_buffer += sizeof(p_rrc_erab_estabTime->count);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_erab_estabTime->erab_estabTime_Sum, "erab_estabTime_Sum");
    *pp_buffer += sizeof(p_rrc_erab_estabTime->erab_estabTime_Sum);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_erab_estabTime->erab_estabTime_Max, "erab_estabTime_Max");
    *pp_buffer += sizeof(p_rrc_erab_estabTime->erab_estabTime_Max);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_setup_len
(
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_setup != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_erab_estab_init_fail_len(&p_rrc_uecc_oam_stat_erab_setup->erab_estab_init_fail_stats);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitFailNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_erab_estab_add_fail_len(&p_rrc_uecc_oam_stat_erab_setup->erab_estab_add_fail_stats);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddFailNbr_sum);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci); loop++)
        {
            length += oam_rrc_il_get_rrc_erab_estabTime_len(&p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_setup
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_setup != PNULL);

    /* This function composes rrc_uecc_oam_stat_erab_setup */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci[loop], "erab_estabInitAttNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_sum, "erab_estabInitAttNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci[loop], "erab_estabInitSuccNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_sum, "erab_estabInitSuccNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_erab_estab_init_fail(pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estab_init_fail_stats))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabInitFailNbr_sum, "erab_estabInitFailNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitFailNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci[loop], "erab_estabAddAttNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_sum, "erab_estabAddAttNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci[loop], "erab_estabAddSuccNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_sum, "erab_estabAddSuccNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_erab_estab_add_fail(pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estab_add_fail_stats))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabAddFailNbr_sum, "erab_estabAddFailNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddFailNbr_sum);

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_erab_estabTime(pp_buffer, &p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_erab_rel_enb_cause_len
(
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_rel_enb_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_erab_rel_enb_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_erab_rel_enb_cause != PNULL);

    /* This function composes rrc_uecc_oam_erab_rel_enb_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork[loop], "erab_relEnbNbr_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport[loop], "erab_relEnbNbr_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas[loop], "erab_relEnbNbr_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol[loop], "erab_relEnbNbr_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc[loop], "erab_relEnbNbr_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_rel_fail_cause_len
(
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_rel_fail_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_rel_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_rel_fail_cause != PNULL);

    /* This function composes rrc_uecc_oam_rel_fail_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork[loop], "erab_relFailNbr_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport[loop], "erab_relFailNbr_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas[loop], "erab_relFailNbr_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol[loop], "erab_relFailNbr_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc[loop], "erab_relFailNbr_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_release_len
(
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_release != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_erab_rel_enb_cause_len(&p_rrc_uecc_oam_stat_erab_release->erab_rel_enb_cause_stats);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_rel_fail_cause_len(&p_rrc_uecc_oam_stat_erab_release->rel_fail_cause_stats);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relFailNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_release
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_release != PNULL);

    /* This function composes rrc_uecc_oam_stat_erab_release */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci[loop], "erab_relEnbNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_sum, "erab_relEnbNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_erab_rel_enb_cause(pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_rel_enb_cause_stats))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci[loop], "erab_relAttNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_sum, "erab_relAttNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci[loop], "erab_relSuccNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_sum, "erab_relSuccNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_rel_fail_cause(pp_buffer, &p_rrc_uecc_oam_stat_erab_release->rel_fail_cause_stats))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relFailNbr_sum, "erab_relFailNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relFailNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci[loop], "erab_relActNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_sum, "erab_relActNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_mod_fail_cause_len
(
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_mod_fail_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_mod_fail_cause
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_mod_fail_cause != PNULL);

    /* This function composes rrc_uecc_oam_stat_erab_mod_fail_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork[loop], "erab_modQosFailNbr_cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport[loop], "erab_modQosFailNbr_cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas[loop], "erab_modQosFailNbr_cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol[loop], "erab_modQosFailNbr_cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc[loop], "erab_modQosFailNbr_cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_modification_len
(
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_modification != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_sum);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_mod_fail_cause_len(&p_rrc_uecc_oam_stat_erab_modification->erab_mod_fail_cause_stats);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosFailNbr_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_modification
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_modification != PNULL);

    /* This function composes rrc_uecc_oam_stat_erab_modification */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci[loop], "erab_modQosAttNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_sum, "erab_modQosAttNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_sum);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci[loop], "erab_modQosSuccNbr_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_sum, "erab_modQosSuccNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_mod_fail_cause(pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_mod_fail_cause_stats))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_modification->erab_modQosFailNbr_sum, "erab_modQosFailNbr_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosFailNbr_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_erab_number_len
(
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_number != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_erab_number
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_erab_number != PNULL);

    /* This function composes rrc_uecc_oam_stat_erab_number */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci[loop], "erabSimNumber_qci[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_erab_number->erabSimNumber_sum, "erabSimNumber_sum");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_related_stats_len
(
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_erab_related_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_setup_len(&p_rrc_oam_erab_related_stats->erab_setup_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_release_len(&p_rrc_oam_erab_related_stats->erab_release_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_modification_len(&p_rrc_oam_erab_related_stats->erab_mod_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_number_len(&p_rrc_oam_erab_related_stats->erab_sim_num_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_erab_related_stats != PNULL);

    /* This function composes rrc_oam_erab_related_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_erab_related_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_erab_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_setup(pp_buffer, &p_rrc_oam_erab_related_stats->erab_setup_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_release(pp_buffer, &p_rrc_oam_erab_related_stats->erab_release_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_modification(pp_buffer, &p_rrc_oam_erab_related_stats->erab_mod_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_number(pp_buffer, &p_rrc_oam_erab_related_stats->erab_sim_num_stats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IntraEnbOutAtt_cause_len
(
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_IntraEnbOutAtt_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_IntraEnbOutAtt_cause != PNULL);

    /* This function composes rrc_oam_ho_IntraEnbOutAtt_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork[loop], "ho_IntraEnbOutAtt_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport[loop], "ho_IntraEnbOutAtt_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas[loop], "ho_IntraEnbOutAtt_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol[loop], "ho_IntraEnbOutAtt_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc[loop], "ho_IntraEnbOutAtt_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IntraEnbOutSucc_cause_len
(
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_IntraEnbOutSucc_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_IntraEnbOutSucc_cause != PNULL);

    /* This function composes rrc_oam_ho_IntraEnbOutSucc_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork[loop], "ho_IntraEnbOutSucc_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport[loop], "ho_IntraEnbOutSucc_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas[loop], "ho_IntraEnbOutSucc_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol[loop], "ho_IntraEnbOutSucc_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc[loop], "ho_IntraEnbOutSucc_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_intra_enb_ho_stats_len
(
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_intra_enb_ho_stats != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_IntraEnbOutAtt_cause_len(&p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_IntraEnbOutSucc_cause_len(&p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_intra_enb_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_intra_enb_ho_stats != PNULL);

    /* This function composes rrc_oam_intra_enb_ho_stats */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutAtt_cause(pp_buffer, &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_sum, "ho_IntraEnbOutAtt_sum");
    *pp_buffer += sizeof(p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_IntraEnbOutSucc_cause(pp_buffer, &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_sum, "ho_IntraEnbOutSucc_sum");
    *pp_buffer += sizeof(p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_InterEnbOutAtt_cause_len
(
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_InterEnbOutAtt_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_InterEnbOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_InterEnbOutAtt_cause != PNULL);

    /* This function composes rrc_oam_ho_InterEnbOutAtt_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork[loop], "ho_InterEnbOutAtt_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport[loop], "ho_InterEnbOutAtt_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas[loop], "ho_InterEnbOutAtt_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol[loop], "ho_InterEnbOutAtt_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc[loop], "ho_InterEnbOutAtt_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_InterEnbOutSucc_cause_len
(
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_InterEnbOutSucc_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_InterEnbOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_InterEnbOutSucc_cause != PNULL);

    /* This function composes rrc_oam_ho_InterEnbOutSucc_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork[loop], "ho_InterEnbOutSucc_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport[loop], "ho_InterEnbOutSucc_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas[loop], "ho_InterEnbOutSucc_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol[loop], "ho_InterEnbOutSucc_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc[loop], "ho_InterEnbOutSucc_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_enb_ho_stats_len
(
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_inter_enb_ho_stats != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_InterEnbOutAtt_cause_len(&p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_InterEnbOutSucc_cause_len(&p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_sum);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutPrepAtt);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_enb_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_inter_enb_ho_stats != PNULL);

    /* This function composes rrc_oam_inter_enb_ho_stats */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_InterEnbOutAtt_cause(pp_buffer, &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_sum, "ho_InterEnbOutAtt_sum");
    *pp_buffer += sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_InterEnbOutSucc_cause(pp_buffer, &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_sum, "ho_InterEnbOutSucc_sum");
    *pp_buffer += sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_sum);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutPrepAtt, "ho_InterEnbOutPrepAtt");
    *pp_buffer += sizeof(p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutPrepAtt);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_mnc_len
(
    mnc_t *p_mnc
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_mnc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mnc->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_mnc->count * sizeof(p_mnc->mnc[0]));

    return length;
}

static
oam_return_et
oam_rrc_il_compose_mnc
(
    UInt8  **pp_buffer,
    mnc_t *p_mnc
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_mnc != PNULL);

    /* This function composes mnc */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mnc->count < 2) || (p_mnc->count > MAX_MNC_OCTET_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mnc->count] should be in range "
            "2 to MAX_MNC_OCTET_SIZE. Incorrect value %u received.", p_mnc->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_mnc->count, "count");
    *pp_buffer += sizeof(p_mnc->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mnc->count; loop++)
        {
            if (p_mnc->mnc[loop] > 9)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_mnc->mnc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_mnc->mnc[loop]);
                return OAM_FAILURE;
            }
            OAM_CP_PACK_UINT8(*pp_buffer, &p_mnc->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_plmn_identity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_plmn_identity->presence_bitmask);

    /* Optional element */
    if(p_plmn_identity->presence_bitmask & 1)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_plmn_identity->mcc);
    }

    /* Get length of IE */
    length += oam_rrc_il_get_mnc_len(&p_plmn_identity->mnc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_plmn_identity
(
    UInt8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_plmn_identity != PNULL);

    /* This function composes plmn_identity */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_plmn_identity->presence_bitmask, "presence_bitmask");
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
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_plmn_identity->mcc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_plmn_identity->mcc[loop]);
                return OAM_FAILURE;
            }
            OAM_CP_PACK_UINT8(*pp_buffer, &p_plmn_identity->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_mnc(pp_buffer, &p_plmn_identity->mnc))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_cell_global_id_eutra_len
(
    cell_global_id_eutra_t *p_cell_global_id_eutra
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_cell_global_id_eutra != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_plmn_identity_len(&p_cell_global_id_eutra->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_eutra->cell_identity);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_cell_global_id_eutra
(
    UInt8  **pp_buffer,
    cell_global_id_eutra_t *p_cell_global_id_eutra
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_cell_global_id_eutra != PNULL);

    /* This function composes cell_global_id_eutra */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_plmn_identity(pp_buffer, &p_cell_global_id_eutra->plmn_identity))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_eutra->cell_identity); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_cell_global_id_eutra->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_OutAttTarget_cause_len
(
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_OutAttTarget_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_OutAttTarget_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_OutAttTarget_cause != PNULL);

    /* This function composes rrc_oam_ho_OutAttTarget_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork[loop], "ho_OutAttTarget_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport[loop], "ho_OutAttTarget_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas[loop], "ho_OutAttTarget_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol[loop], "ho_OutAttTarget_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc[loop], "ho_OutAttTarget_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_OutSuccTarget_cause_len
(
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_OutSuccTarget_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_OutSuccTarget_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_OutSuccTarget_cause != PNULL);

    /* This function composes rrc_oam_ho_OutSuccTarget_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork[loop], "ho_OutSuccTarget_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport[loop], "ho_OutSuccTarget_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas[loop], "ho_OutSuccTarget_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol[loop], "ho_OutSuccTarget_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc[loop], "ho_OutSuccTarget_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_len
(
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_meas_on_neigh_cell_ho_stats != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_cell_global_id_eutra_len(&p_rrc_oam_meas_on_neigh_cell_ho_stats->ecgi);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_OutAttTarget_cause_len(&p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_OutSuccTarget_cause_len(&p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_meas_on_neigh_cell_ho_stats != PNULL);

    /* This function composes rrc_oam_meas_on_neigh_cell_ho_stats */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_cell_global_id_eutra(pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats->ecgi))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_OutAttTarget_cause(pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_sum, "ho_OutAttTarget_sum");
    *pp_buffer += sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_OutSuccTarget_cause(pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_sum, "ho_OutSuccTarget_sum");
    *pp_buffer += sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_list_len
(
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_meas_on_neigh_cell_ho_stats_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %d received.", p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_len(&p_rrc_oam_meas_on_neigh_cell_ho_stats_list->meas_on_neigh_cell_ho_stats[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats_list
(
    UInt8  **pp_buffer,
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_meas_on_neigh_cell_ho_stats_list != PNULL);

    /* This function composes rrc_oam_meas_on_neigh_cell_ho_stats_list */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %u received.", p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats(pp_buffer, &p_rrc_oam_meas_on_neigh_cell_ho_stats_list->meas_on_neigh_cell_ho_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_intra_freq_ho_stats_len
(
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_intra_freq_ho_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutSucc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_intra_freq_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_intra_freq_ho_stats != PNULL);

    /* This function composes rrc_oam_intra_freq_ho_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutAtt, "ho_IntraFreqOutAtt");
    *pp_buffer += sizeof(p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutSucc, "ho_IntraFreqOutSucc");
    *pp_buffer += sizeof(p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutSucc);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_freq_ho_stats_len
(
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_inter_freq_ho_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutSucc);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutSucc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_freq_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_inter_freq_ho_stats != PNULL);

    /* This function composes rrc_oam_inter_freq_ho_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutAtt, "ho_InterFreqMeasGapOutAtt");
    *pp_buffer += sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutSucc, "ho_InterFreqMeasGapOutSucc");
    *pp_buffer += sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutSucc);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutAtt, "ho_InterFreqNoMeasGapOutAtt");
    *pp_buffer += sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutSucc, "ho_InterFreqNoMeasGapOutSucc");
    *pp_buffer += sizeof(p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutSucc);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_drx_non_drx_ho_stats_len
(
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_drx_non_drx_ho_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutSucc);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutSucc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_drx_non_drx_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_drx_non_drx_ho_stats != PNULL);

    /* This function composes rrc_oam_drx_non_drx_ho_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutAtt, "ho_DrxOutAtt");
    *pp_buffer += sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutSucc, "ho_DrxOutSucc");
    *pp_buffer += sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutSucc);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutAtt, "ho_NoDrxOutAtt");
    *pp_buffer += sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutSucc, "ho_NoDrxOutSucc");
    *pp_buffer += sizeof(p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutSucc);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_utran_geran_cgi_len
(
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_utran_geran_cgi != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_utran_geran_cgi->rat_type);

    /* Get length of IE */
    length += oam_rrc_il_get_plmn_identity_len(&p_rrc_utran_geran_cgi->plmn_identity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_utran_geran_cgi->cell_identity);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_utran_geran_cgi
(
    UInt8  **pp_buffer,
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_utran_geran_cgi != PNULL);

    /* This function composes rrc_utran_geran_cgi */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_utran_geran_cgi->rat_type, "rat_type");
    *pp_buffer += sizeof(p_rrc_utran_geran_cgi->rat_type);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_plmn_identity(pp_buffer, &p_rrc_utran_geran_cgi->plmn_identity))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_utran_geran_cgi->cell_identity, "cell_identity");
    *pp_buffer += sizeof(p_rrc_utran_geran_cgi->cell_identity);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_cdma2000_1xrtt_cell_identifier_len
(
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_cdma2000_1xrtt_cell_identifier != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_cdma2000_1xrtt_cell_identifier
(
    UInt8  **pp_buffer,
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_cdma2000_1xrtt_cell_identifier != PNULL);

    /* This function composes cdma2000_1xrtt_cell_identifier */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id[loop], "cdma2000_1xrtt_cell_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_cdma2000_hrpd_sector_identifier_len
(
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_cdma2000_hrpd_sector_identifier != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_cdma2000_hrpd_sector_identifier->sector_id_length);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cdma2000_hrpd_sector_identifier->sector_id);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_cdma2000_hrpd_sector_identifier
(
    UInt8  **pp_buffer,
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_cdma2000_hrpd_sector_identifier != PNULL);

    /* This function composes cdma2000_hrpd_sector_identifier */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cdma2000_hrpd_sector_identifier->sector_id_length < 16) || (p_cdma2000_hrpd_sector_identifier->sector_id_length > MAX_SECTOR_ID_LENGTH))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_cdma2000_hrpd_sector_identifier->sector_id_length] should be in range "
            "16 to MAX_SECTOR_ID_LENGTH. Incorrect value %u received.", p_cdma2000_hrpd_sector_identifier->sector_id_length);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_cdma2000_hrpd_sector_identifier->sector_id_length, "sector_id_length");
    *pp_buffer += sizeof(p_cdma2000_hrpd_sector_identifier->sector_id_length);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cdma2000_hrpd_sector_identifier->sector_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_cdma2000_hrpd_sector_identifier->sector_id[loop], "sector_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_inter_rat_global_cgi_len
(
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_inter_rat_global_cgi != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_inter_rat_global_cgi->bitmask);

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_utran_geran_cgi_len(&p_rrc_inter_rat_global_cgi->cgi_info);
    }

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_cdma2000_1xrtt_cell_identifier_len(&p_rrc_inter_rat_global_cgi->cell_id_1xrtt);
    }

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_cdma2000_hrpd_sector_identifier_len(&p_rrc_inter_rat_global_cgi->sector_id_hrpd);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_inter_rat_global_cgi
(
    UInt8  **pp_buffer,
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_inter_rat_global_cgi != PNULL);

    /* This function composes rrc_inter_rat_global_cgi */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_inter_rat_global_cgi->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_inter_rat_global_cgi->bitmask);

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_utran_geran_cgi(pp_buffer, &p_rrc_inter_rat_global_cgi->cgi_info))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_cdma2000_1xrtt_cell_identifier(pp_buffer, &p_rrc_inter_rat_global_cgi->cell_id_1xrtt))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_cdma2000_hrpd_sector_identifier(pp_buffer, &p_rrc_inter_rat_global_cgi->sector_id_hrpd))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IratOutAtt_cause_len
(
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_IratOutAtt_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IratOutAtt_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_IratOutAtt_cause != PNULL);

    /* This function composes rrc_oam_ho_IratOutAtt_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork[loop], "ho_IratOutAtt_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport[loop], "ho_IratOutAtt_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas[loop], "ho_IratOutAtt_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol[loop], "ho_IratOutAtt_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc[loop], "ho_IratOutAtt_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ho_IratOutSucc_cause_len
(
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ho_IratOutSucc_cause != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ho_IratOutSucc_cause
(
    UInt8  **pp_buffer,
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ho_IratOutSucc_cause != PNULL);

    /* This function composes rrc_oam_ho_IratOutSucc_cause */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork[loop], "ho_IratOutSucc_Cause_radioNetwork[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport[loop], "ho_IratOutSucc_Cause_transport[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas[loop], "ho_IratOutSucc_Cause_nas[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol[loop], "ho_IratOutSucc_Cause_protocol[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc); loop++)
        {
            OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc[loop], "ho_IratOutSucc_Cause_misc[]");
            *pp_buffer += sizeof(UInt32);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_len
(
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_inter_rat_ho_stats != PNULL);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_inter_rat_global_cgi_len(&p_rrc_oam_inter_rat_ho_stats->target_cgi);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_IratOutAtt_cause_len(&p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_sum);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ho_IratOutSucc_cause_len(&p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_sum);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats
(
    UInt8  **pp_buffer,
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_inter_rat_ho_stats != PNULL);

    /* This function composes rrc_oam_inter_rat_ho_stats */
    

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_inter_rat_global_cgi(pp_buffer, &p_rrc_oam_inter_rat_ho_stats->target_cgi))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_IratOutAtt_cause(pp_buffer, &p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_sum, "ho_IratOutAtt_sum");
    *pp_buffer += sizeof(p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_sum);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ho_IratOutSucc_cause(pp_buffer, &p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_cause))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_sum, "ho_IratOutSucc_sum");
    *pp_buffer += sizeof(p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_sum);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_list_len
(
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_inter_rat_ho_stats_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_inter_rat_ho_stats_list->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_inter_rat_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_inter_rat_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %d received.", p_rrc_oam_inter_rat_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_inter_rat_ho_stats_list->count; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_len(&p_rrc_oam_inter_rat_ho_stats_list->inter_rat_ho_stats[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats_list
(
    UInt8  **pp_buffer,
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_inter_rat_ho_stats_list != PNULL);

    /* This function composes rrc_oam_inter_rat_ho_stats_list */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_inter_rat_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_inter_rat_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %u received.", p_rrc_oam_inter_rat_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_inter_rat_ho_stats_list->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_inter_rat_ho_stats_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_inter_rat_ho_stats_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats(pp_buffer, &p_rrc_oam_inter_rat_ho_stats_list->inter_rat_ho_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_related_stats_len
(
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_handover_related_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_ENB_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_intra_enb_ho_stats_len(&p_rrc_oam_handover_related_stats->intra_enb_ho_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_ENB_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_enb_ho_stats_len(&p_rrc_oam_handover_related_stats->inter_enb_ho_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_list_len(&p_rrc_oam_handover_related_stats->meas_on_neigh_cell_ho_stats_list);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_intra_freq_ho_stats_len(&p_rrc_oam_handover_related_stats->intra_freq_ho_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_FREQ_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_freq_ho_stats_len(&p_rrc_oam_handover_related_stats->inter_freq_ho_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_drx_non_drx_ho_stats_len(&p_rrc_oam_handover_related_stats->drx_non_drx_ho_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_RAT_HO_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_list_len(&p_rrc_oam_handover_related_stats->inter_rat_ho_stats_list);
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_related_stats->ho_failure_num);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_related_stats
(
    UInt8  **pp_buffer,
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_handover_related_stats != PNULL);

    /* This function composes rrc_oam_handover_related_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_handover_related_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_handover_related_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_ENB_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_intra_enb_ho_stats(pp_buffer, &p_rrc_oam_handover_related_stats->intra_enb_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_ENB_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_enb_ho_stats(pp_buffer, &p_rrc_oam_handover_related_stats->inter_enb_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats_list(pp_buffer, &p_rrc_oam_handover_related_stats->meas_on_neigh_cell_ho_stats_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_intra_freq_ho_stats(pp_buffer, &p_rrc_oam_handover_related_stats->intra_freq_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_FREQ_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_freq_ho_stats(pp_buffer, &p_rrc_oam_handover_related_stats->inter_freq_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_drx_non_drx_ho_stats(pp_buffer, &p_rrc_oam_handover_related_stats->drx_non_drx_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_RAT_HO_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats_list(pp_buffer, &p_rrc_oam_handover_related_stats->inter_rat_ho_stats_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_handover_related_stats->ho_failure_num, "ho_failure_num");
    *pp_buffer += sizeof(p_rrc_oam_handover_related_stats->ho_failure_num);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_uecc_oam_stat_ue_assoc_s1_connection_len
(
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_ue_assoc_s1_connection != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabSucc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_uecc_oam_stat_ue_assoc_s1_connection
(
    UInt8  **pp_buffer,
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_uecc_oam_stat_ue_assoc_s1_connection != PNULL);

    /* This function composes rrc_uecc_oam_stat_ue_assoc_s1_connection */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabAtt, "s1sig_connEstabAtt");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabSucc, "s1sig_connEstabSucc");
    *pp_buffer += sizeof(p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabSucc);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_csc_oam_stat_paging_len
(
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_csc_oam_stat_paging != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_csc_oam_stat_paging->pag_discardedNbr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_csc_oam_stat_paging->pag_succNbr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_csc_oam_stat_paging->pag_receivedNbr);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_csc_oam_stat_paging
(
    UInt8  **pp_buffer,
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_csc_oam_stat_paging != PNULL);

    /* This function composes rrc_csc_oam_stat_paging */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_csc_oam_stat_paging->pag_discardedNbr, "pag_discardedNbr");
    *pp_buffer += sizeof(p_rrc_csc_oam_stat_paging->pag_discardedNbr);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_csc_oam_stat_paging->pag_succNbr, "pag_succNbr");
    *pp_buffer += sizeof(p_rrc_csc_oam_stat_paging->pag_succNbr);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_csc_oam_stat_paging->pag_receivedNbr, "pag_receivedNbr");
    *pp_buffer += sizeof(p_rrc_csc_oam_stat_paging->pag_receivedNbr);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_cell_stats_req_len
(
    rrc_oam_get_cell_stats_req_t *p_rrc_oam_get_cell_stats_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->cell_index);

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_connection_stats_len(&p_rrc_oam_get_cell_stats_req->rrc_connection_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_erab_stats_len(&p_rrc_oam_get_cell_stats_req->rrc_erab_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_handover_stats_len(&p_rrc_oam_get_cell_stats_req->rrc_handover_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->rrc_ue_assoc_s1_conn_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->rrc_paging_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_RESET_CELL_STATISTICS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->reset_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_stats_req->rrc_IratIncMobility_LAI);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_cell_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_stats_req_t *p_rrc_oam_get_cell_stats_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_req != PNULL);

    /* This function composes rrc_oam_get_cell_stats_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_cell_stats_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_stats_req->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->cell_index);

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_connection_stats(pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_connection_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_erab_stats(pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_erab_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_handover_stats(pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_handover_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_ue_assoc_s1_conn_stats, "rrc_ue_assoc_s1_conn_stats");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->rrc_ue_assoc_s1_conn_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_paging_stats, "rrc_paging_stats");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->rrc_paging_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_RESET_CELL_STATISTICS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_req->reset_stats, "reset_stats");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->reset_stats);
    }

    /* Optional element */
    if(p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_stats_req->rrc_IratIncMobility_LAI, "rrc_IratIncMobility_LAI");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_stats_req->rrc_IratIncMobility_LAI);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_cell_stats_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_CELL_STATS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_stats_req
(
    rrc_oam_get_cell_stats_req_t  *p_rrc_oam_get_cell_stats_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_stats_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_cell_stats_req_len(p_rrc_oam_get_cell_stats_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_CELL_STATS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_CELL_STATS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_CELL_STATS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_GET_CELL_STATS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_cell_stats_req(&p_oam_msg, p_rrc_oam_get_cell_stats_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_connection_stats_len
(
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_connection_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->rrc_connection_establishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->rrc_connection_reestablishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->rrcConnectionSetupTime);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_connection_stats->ue_contxt_rel_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_connection_stats
(
    UInt8  **pp_buffer,
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_connection_stats != PNULL);

    /* This function composes rrc_oam_connection_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_connection_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_connection_stats->rrc_connection_establishment, "rrc_connection_establishment");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->rrc_connection_establishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_connection_stats->rrc_connection_reestablishment, "rrc_connection_reestablishment");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->rrc_connection_reestablishment);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_connection_stats->rrcConnectionNumber, "rrcConnectionNumber");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_connection_stats->rrcConnectionSetupTime, "rrcConnectionSetupTime");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->rrcConnectionSetupTime);
    }

    /* Optional element */
    if(p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_connection_stats->ue_contxt_rel_stats, "ue_contxt_rel_stats");
    *pp_buffer += sizeof(p_rrc_oam_connection_stats->ue_contxt_rel_stats);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_stats_len
(
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_erab_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats->erab_setup_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats->erab_release_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats->erab_mod_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats->erab_num_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_stats
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_erab_stats != PNULL);

    /* This function composes rrc_oam_erab_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_erab_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_erab_stats->erab_setup_stats, "erab_setup_stats");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats->erab_setup_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_erab_stats->erab_release_stats, "erab_release_stats");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats->erab_release_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_erab_stats->erab_mod_stats, "erab_mod_stats");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats->erab_mod_stats);
    }

    /* Optional element */
    if(p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_erab_stats->erab_num_stats, "erab_num_stats");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats->erab_num_stats);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_stats_len
(
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_handover_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->intra_enb_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->inter_enb_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->meas_on_neigh_cell_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->intra_freq_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->inter_freq_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->drx_non_drx_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->inter_rat_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats->ho_failure_num);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_stats
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_handover_stats != PNULL);

    /* This function composes rrc_oam_handover_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_handover_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->bitmask);

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->intra_enb_ho_related_stats, "intra_enb_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->intra_enb_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->inter_enb_ho_related_stats, "inter_enb_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->inter_enb_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->meas_on_neigh_cell_ho_related_stats, "meas_on_neigh_cell_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->meas_on_neigh_cell_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->intra_freq_ho_related_stats, "intra_freq_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->intra_freq_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->inter_freq_ho_related_stats, "inter_freq_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->inter_freq_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->drx_non_drx_ho_related_stats, "drx_non_drx_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->drx_non_drx_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->inter_rat_ho_related_stats, "inter_rat_ho_related_stats");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->inter_rat_ho_related_stats);
    }

    /* Optional element */
    if(p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_handover_stats->ho_failure_num, "ho_failure_num");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats->ho_failure_num);
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_config_stats_req_len
(
    rrc_oam_config_stats_req_t *p_rrc_oam_config_stats_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_config_stats_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_config_stats_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_config_stats_req->rrc_periodic_reporting);

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_rrc_connection_stats_info_len(&p_rrc_oam_config_stats_req->rrc_connection_stats);
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_erab_stats_info_len(&p_rrc_oam_config_stats_req->rrc_erab_stats);
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_handover_stats_info_len(&p_rrc_oam_config_stats_req->rrc_handover_stats);
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_sampling_period_info_len(&p_rrc_oam_config_stats_req->rrc_ue_assoc_s1_conn_stats);
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_PAGING_RELATED_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_sampling_period_info_len(&p_rrc_oam_config_stats_req->rrc_paging_stats);
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_sampling_period_info_len(&p_rrc_oam_config_stats_req->rrc_IratIncMobility_LAI_stats);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_config_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_config_stats_req_t *p_rrc_oam_config_stats_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_config_stats_req != PNULL);

    /* This function composes rrc_oam_config_stats_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_config_stats_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_config_stats_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_config_stats_req->rrc_periodic_reporting > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_config_stats_req->rrc_periodic_reporting] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_config_stats_req->rrc_periodic_reporting);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_config_stats_req->rrc_periodic_reporting, "rrc_periodic_reporting");
    *pp_buffer += sizeof(p_rrc_oam_config_stats_req->rrc_periodic_reporting);

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_rrc_connection_stats_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_connection_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_erab_stats_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_erab_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_handover_stats_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_handover_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_sampling_period_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_ue_assoc_s1_conn_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_PAGING_RELATED_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_sampling_period_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_paging_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_oam_config_stats_req->bitmask & RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_sampling_period_info(pp_buffer, &p_rrc_oam_config_stats_req->rrc_IratIncMobility_LAI_stats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_config_stats_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CONFIG_STATS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_config_stats_req
(
    rrc_oam_config_stats_req_t  *p_rrc_oam_config_stats_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_config_stats_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_config_stats_req_len(p_rrc_oam_config_stats_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CONFIG_STATS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CONFIG_STATS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CONFIG_STATS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_CONFIG_STATS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_config_stats_req(&p_oam_msg, p_rrc_oam_config_stats_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_rrc_connection_stats_info_len
(
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_rrc_connection_stats_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_rrc_connection_stats_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_rrc_connection_stats_info->sampling_period);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_rrc_connection_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_rrc_connection_stats_info != PNULL);

    /* This function composes rrc_oam_rrc_connection_stats_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_rrc_connection_stats_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_rrc_connection_stats_info->bitmask);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_rrc_connection_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_rrc_connection_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_rrc_connection_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_rrc_connection_stats_info->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_oam_rrc_connection_stats_info->sampling_period);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_erab_stats_info_len
(
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_erab_stats_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats_info->sampling_period);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_erab_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_erab_stats_info != PNULL);

    /* This function composes rrc_oam_erab_stats_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_erab_stats_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats_info->bitmask);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_erab_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_erab_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_erab_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_erab_stats_info->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats_info->sampling_period);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_handover_stats_info_len
(
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_handover_stats_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats_info->sampling_period);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_handover_stats_info
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_handover_stats_info != PNULL);

    /* This function composes rrc_oam_handover_stats_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_handover_stats_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats_info->bitmask);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_handover_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_handover_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_handover_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_handover_stats_info->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats_info->sampling_period);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_sampling_period_info_len
(
    rrc_sampling_period_info_t *p_rrc_sampling_period_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_sampling_period_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_sampling_period_info->sampling_period);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_sampling_period_info
(
    UInt8  **pp_buffer,
    rrc_sampling_period_info_t *p_rrc_sampling_period_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_sampling_period_info != PNULL);

    /* This function composes rrc_sampling_period_info */
    

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_sampling_period_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_sampling_period_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_sampling_period_info->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_sampling_period_info->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_sampling_period_info->sampling_period);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_config_stats_resp_len
(
    rrc_oam_config_stats_resp_t *p_rrc_oam_config_stats_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_config_stats_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_config_stats_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_config_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_config_stats_resp_t *p_rrc_oam_config_stats_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_config_stats_resp != PNULL);

    /* This function composes rrc_oam_config_stats_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_config_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_config_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_config_stats_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_config_stats_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_config_stats_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_config_stats_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CONFIG_STATS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_config_stats_resp
(
    rrc_oam_config_stats_resp_t  *p_rrc_oam_config_stats_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_config_stats_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_config_stats_resp_len(p_rrc_oam_config_stats_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CONFIG_STATS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CONFIG_STATS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CONFIG_STATS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_CONFIG_STATS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_config_stats_resp(&p_oam_msg, p_rrc_oam_config_stats_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_rrc_connection_stats_ind_len
(
    rrc_oam_rrc_connection_stats_ind_t *p_rrc_oam_rrc_connection_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_rrc_connection_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_rrc_connection_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_rrc_connection_stats_ind->count < 1) || (p_rrc_oam_rrc_connection_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_rrc_connection_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_rrc_connection_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_rrc_connection_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_connection_stats_len(&p_rrc_oam_rrc_connection_stats_ind->rrc_connection[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_rrc_connection_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_rrc_connection_stats_ind_t *p_rrc_oam_rrc_connection_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_rrc_connection_stats_ind != PNULL);

    /* This function composes rrc_oam_rrc_connection_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_rrc_connection_stats_ind->count < 1) || (p_rrc_oam_rrc_connection_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_rrc_connection_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_rrc_connection_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_rrc_connection_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_rrc_connection_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_rrc_connection_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_connection_stats(pp_buffer, &p_rrc_oam_rrc_connection_stats_ind->rrc_connection[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_rrc_connection_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_RRC_CONNECTION_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_rrc_connection_stats_ind
(
    rrc_oam_rrc_connection_stats_ind_t  *p_rrc_oam_rrc_connection_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_rrc_connection_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_rrc_connection_stats_ind_len(p_rrc_oam_rrc_connection_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_RRC_CONNECTION_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_RRC_CONNECTION_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_RRC_CONNECTION_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_RRC_CONNECTION_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_rrc_connection_stats_ind(&p_oam_msg, p_rrc_oam_rrc_connection_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_connection_stats_len
(
    rrc_connection_stats_t *p_rrc_connection_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_connection_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connection_stats->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connection_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connection_stats->sampling_period);

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_connection_establishment_len(&p_rrc_connection_stats->rrc_connection_establishment);
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_connection_reestablishment_len(&p_rrc_connection_stats->rrc_connection_reestablishment);
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_connection_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT)
    {

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_connection_stats->rrc_connEstabTime_cause); loop++)
        {
            length += oam_rrc_il_get_rrc_connEstabTime_len(&p_rrc_connection_stats->rrc_connEstabTime_cause[loop]);
        }
    }
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_ue_context_release_len(&p_rrc_connection_stats->ue_contxt_rel_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_connection_stats
(
    UInt8  **pp_buffer,
    rrc_connection_stats_t *p_rrc_connection_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_connection_stats != PNULL);

    /* This function composes rrc_connection_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_connection_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_connection_stats->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_connection_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_connection_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_connection_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_connection_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_connection_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_connection_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_connection_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_connection_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_connection_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_connection_stats->sampling_period);

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_connection_establishment(pp_buffer, &p_rrc_connection_stats->rrc_connection_establishment))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_connection_reestablishment(pp_buffer, &p_rrc_connection_stats->rrc_connection_reestablishment))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_connection_stats->rrcConnectionNumber, "rrcConnectionNumber");
    *pp_buffer += sizeof(p_rrc_connection_stats->rrcConnectionNumber);
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_connection_stats->rrc_connEstabTime_cause); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_connEstabTime(pp_buffer, &p_rrc_connection_stats->rrc_connEstabTime_cause[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_ue_context_release(pp_buffer, &p_rrc_connection_stats->ue_contxt_rel_stats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_handover_stats_ind_len
(
    rrc_oam_handover_stats_ind_t *p_rrc_oam_handover_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_handover_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_handover_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_handover_stats_ind->count < 1) || (p_rrc_oam_handover_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_handover_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_handover_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_handover_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_handover_stats_len(&p_rrc_oam_handover_stats_ind->handover_stats[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_handover_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_ind_t *p_rrc_oam_handover_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_handover_stats_ind != PNULL);

    /* This function composes rrc_oam_handover_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_handover_stats_ind->count < 1) || (p_rrc_oam_handover_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_handover_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_handover_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_handover_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_handover_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_handover_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_handover_stats(pp_buffer, &p_rrc_oam_handover_stats_ind->handover_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_handover_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_HANDOVER_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_handover_stats_ind
(
    rrc_oam_handover_stats_ind_t  *p_rrc_oam_handover_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_handover_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_handover_stats_ind_len(p_rrc_oam_handover_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_HANDOVER_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_HANDOVER_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_HANDOVER_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_HANDOVER_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_handover_stats_ind(&p_oam_msg, p_rrc_oam_handover_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_handover_stats_len
(
    rrc_handover_stats_t *p_rrc_handover_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_handover_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_handover_stats->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_handover_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_handover_stats->sampling_period);

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_intra_enb_ho_stats_len(&p_rrc_handover_stats->intra_enb_ho_stats);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_enb_ho_stats_len(&p_rrc_handover_stats->inter_enb_ho_stats);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_meas_on_neigh_cell_ho_stats_list_len(&p_rrc_handover_stats->meas_on_neigh_cell_ho_stats_list);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_intra_freq_ho_stats_len(&p_rrc_handover_stats->intra_freq_ho_stats);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_freq_ho_stats_len(&p_rrc_handover_stats->inter_freq_ho_stats);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_drx_non_drx_ho_stats_len(&p_rrc_handover_stats->drx_non_drx_ho_stats);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_inter_rat_ho_stats_list_len(&p_rrc_handover_stats->inter_rat_ho_stats_list);
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_handover_stats->ho_failure_num);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_handover_stats
(
    UInt8  **pp_buffer,
    rrc_handover_stats_t *p_rrc_handover_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_handover_stats != PNULL);

    /* This function composes rrc_handover_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_handover_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_handover_stats->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_handover_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_handover_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_handover_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_handover_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_handover_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_handover_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_handover_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_handover_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_handover_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_handover_stats->sampling_period);

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_intra_enb_ho_stats(pp_buffer, &p_rrc_handover_stats->intra_enb_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_enb_ho_stats(pp_buffer, &p_rrc_handover_stats->inter_enb_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_meas_on_neigh_cell_ho_stats_list(pp_buffer, &p_rrc_handover_stats->meas_on_neigh_cell_ho_stats_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_intra_freq_ho_stats(pp_buffer, &p_rrc_handover_stats->intra_freq_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_freq_ho_stats(pp_buffer, &p_rrc_handover_stats->inter_freq_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_drx_non_drx_ho_stats(pp_buffer, &p_rrc_handover_stats->drx_non_drx_ho_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_inter_rat_ho_stats_list(pp_buffer, &p_rrc_handover_stats->inter_rat_ho_stats_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_handover_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_handover_stats->ho_failure_num, "ho_failure_num");
    *pp_buffer += sizeof(p_rrc_handover_stats->ho_failure_num);
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_erab_stats_ind_len
(
    rrc_oam_erab_stats_ind_t *p_rrc_oam_erab_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_erab_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_erab_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_erab_stats_ind->count < 1) || (p_rrc_oam_erab_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_erab_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_erab_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_erab_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_erab_stats_len(&p_rrc_oam_erab_stats_ind->erab_stats[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_erab_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_ind_t *p_rrc_oam_erab_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_erab_stats_ind != PNULL);

    /* This function composes rrc_oam_erab_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_erab_stats_ind->count < 1) || (p_rrc_oam_erab_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_erab_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_erab_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_erab_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_erab_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_erab_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_erab_stats(pp_buffer, &p_rrc_oam_erab_stats_ind->erab_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_erab_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_ERAB_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_erab_stats_ind
(
    rrc_oam_erab_stats_ind_t  *p_rrc_oam_erab_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_erab_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_erab_stats_ind_len(p_rrc_oam_erab_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_ERAB_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_ERAB_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_ERAB_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_ERAB_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_erab_stats_ind(&p_oam_msg, p_rrc_oam_erab_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_erab_stats_len
(
    rrc_erab_stats_t *p_rrc_erab_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_erab_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_stats->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_erab_stats->sampling_period);

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_setup_len(&p_rrc_erab_stats->erab_setup_stats);
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_release_len(&p_rrc_erab_stats->erab_release_stats);
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_modification_len(&p_rrc_erab_stats->erab_mod_stats);
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_uecc_oam_stat_erab_number_len(&p_rrc_erab_stats->erab_sim_num_stats);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_erab_stats
(
    UInt8  **pp_buffer,
    rrc_erab_stats_t *p_rrc_erab_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_erab_stats != PNULL);

    /* This function composes rrc_erab_stats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_erab_stats->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_erab_stats->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_erab_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_erab_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_erab_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_erab_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_erab_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_erab_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_erab_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_erab_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_erab_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_erab_stats->sampling_period);

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_setup(pp_buffer, &p_rrc_erab_stats->erab_setup_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_release(pp_buffer, &p_rrc_erab_stats->erab_release_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_modification(pp_buffer, &p_rrc_erab_stats->erab_mod_stats))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_uecc_oam_stat_erab_number(pp_buffer, &p_rrc_erab_stats->erab_sim_num_stats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_ue_assoc_s1_conn_stats_ind_len
(
    rrc_oam_ue_assoc_s1_conn_stats_ind_t *p_rrc_oam_ue_assoc_s1_conn_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ue_assoc_s1_conn_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_assoc_s1_conn_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ue_assoc_s1_conn_stats_ind->count < 1) || (p_rrc_oam_ue_assoc_s1_conn_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_assoc_s1_conn_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_ue_assoc_s1_conn_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_assoc_s1_conn_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_ue_assoc_s1_conn_stats_len(&p_rrc_oam_ue_assoc_s1_conn_stats_ind->ue_assoc_s1_conn_stats[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_ue_assoc_s1_conn_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_ue_assoc_s1_conn_stats_ind_t *p_rrc_oam_ue_assoc_s1_conn_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ue_assoc_s1_conn_stats_ind != PNULL);

    /* This function composes rrc_oam_ue_assoc_s1_conn_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ue_assoc_s1_conn_stats_ind->count < 1) || (p_rrc_oam_ue_assoc_s1_conn_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_assoc_s1_conn_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_ue_assoc_s1_conn_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ue_assoc_s1_conn_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_ue_assoc_s1_conn_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_assoc_s1_conn_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_ue_assoc_s1_conn_stats(pp_buffer, &p_rrc_oam_ue_assoc_s1_conn_stats_ind->ue_assoc_s1_conn_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_ue_assoc_s1_conn_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_ue_assoc_s1_conn_stats_ind
(
    rrc_oam_ue_assoc_s1_conn_stats_ind_t  *p_rrc_oam_ue_assoc_s1_conn_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_ue_assoc_s1_conn_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_ue_assoc_s1_conn_stats_ind_len(p_rrc_oam_ue_assoc_s1_conn_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ue_assoc_s1_conn_stats_ind(&p_oam_msg, p_rrc_oam_ue_assoc_s1_conn_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_ue_assoc_s1_conn_stats_len
(
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_ue_assoc_s1_conn_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_assoc_s1_conn_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_assoc_s1_conn_stats->sampling_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabAtt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabSucc);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_ue_assoc_s1_conn_stats
(
    UInt8  **pp_buffer,
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_ue_assoc_s1_conn_stats != PNULL);

    /* This function composes rrc_ue_assoc_s1_conn_stats */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_assoc_s1_conn_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_ue_assoc_s1_conn_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_ue_assoc_s1_conn_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_ue_assoc_s1_conn_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_ue_assoc_s1_conn_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_ue_assoc_s1_conn_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_ue_assoc_s1_conn_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_ue_assoc_s1_conn_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_ue_assoc_s1_conn_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_ue_assoc_s1_conn_stats->sampling_period);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabAtt, "s1sig_connEstabAtt");
    *pp_buffer += sizeof(p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabAtt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabSucc, "s1sig_connEstabSucc");
    *pp_buffer += sizeof(p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabSucc);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_paging_stats_ind_len
(
    rrc_oam_paging_stats_ind_t *p_rrc_oam_paging_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_paging_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_paging_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_paging_stats_ind->count < 1) || (p_rrc_oam_paging_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_paging_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_paging_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_paging_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_paging_stats_len(&p_rrc_oam_paging_stats_ind->rrc_paging_stats[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_paging_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_paging_stats_ind_t *p_rrc_oam_paging_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_paging_stats_ind != PNULL);

    /* This function composes rrc_oam_paging_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_paging_stats_ind->count < 1) || (p_rrc_oam_paging_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_paging_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_paging_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_paging_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_paging_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_paging_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_paging_stats(pp_buffer, &p_rrc_oam_paging_stats_ind->rrc_paging_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_paging_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_PAGING_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_paging_stats_ind
(
    rrc_oam_paging_stats_ind_t  *p_rrc_oam_paging_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_paging_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_paging_stats_ind_len(p_rrc_oam_paging_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_PAGING_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_PAGING_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_PAGING_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_PAGING_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_paging_stats_ind(&p_oam_msg, p_rrc_oam_paging_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_paging_stats_len
(
    rrc_paging_stats_t *p_rrc_paging_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_paging_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_paging_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_paging_stats->sampling_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_paging_stats->pag_discardedNbr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_paging_stats->pag_succNbr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_paging_stats->pag_receivedNbr);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_paging_stats
(
    UInt8  **pp_buffer,
    rrc_paging_stats_t *p_rrc_paging_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_paging_stats != PNULL);

    /* This function composes rrc_paging_stats */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_paging_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_paging_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_paging_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_paging_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_paging_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_paging_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_paging_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_paging_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_paging_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_paging_stats->sampling_period);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_paging_stats->pag_discardedNbr, "pag_discardedNbr");
    *pp_buffer += sizeof(p_rrc_paging_stats->pag_discardedNbr);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_paging_stats->pag_succNbr, "pag_succNbr");
    *pp_buffer += sizeof(p_rrc_paging_stats->pag_succNbr);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_paging_stats->pag_receivedNbr, "pag_receivedNbr");
    *pp_buffer += sizeof(p_rrc_paging_stats->pag_receivedNbr);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_irat_mobility_stats_ind_len
(
    rrc_oam_irat_mobility_stats_ind_t *p_rrc_oam_irat_mobility_stats_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_irat_mobility_stats_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_irat_mobility_stats_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_irat_mobility_stats_ind->count < 1) || (p_rrc_oam_irat_mobility_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_irat_mobility_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_oam_irat_mobility_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_irat_mobility_stats_ind->count; loop++)
        {
            length += oam_rrc_il_get_rrc_irat_mobility_stats_len(&p_rrc_oam_irat_mobility_stats_ind->rrc_irat_mobility_stats[loop]);
        }
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_irat_mobility_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_irat_mobility_stats_ind_t *p_rrc_oam_irat_mobility_stats_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_irat_mobility_stats_ind != PNULL);

    /* This function composes rrc_oam_irat_mobility_stats_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_irat_mobility_stats_ind->count < 1) || (p_rrc_oam_irat_mobility_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_irat_mobility_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_irat_mobility_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_irat_mobility_stats_ind->count, "count");
    *pp_buffer += sizeof(p_rrc_oam_irat_mobility_stats_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_irat_mobility_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_irat_mobility_stats(pp_buffer, &p_rrc_oam_irat_mobility_stats_ind->rrc_irat_mobility_stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_irat_mobility_stats_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_IRAT_MOBILITY_STATS_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_irat_mobility_stats_ind
(
    rrc_oam_irat_mobility_stats_ind_t  *p_rrc_oam_irat_mobility_stats_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_irat_mobility_stats_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_irat_mobility_stats_ind_len(p_rrc_oam_irat_mobility_stats_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_IRAT_MOBILITY_STATS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_IRAT_MOBILITY_STATS_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_IRAT_MOBILITY_STATS_IND, msg_length,cell_index);

    /* Fill RRC_OAM_IRAT_MOBILITY_STATS_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_irat_mobility_stats_ind(&p_oam_msg, p_rrc_oam_irat_mobility_stats_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_irat_mobility_stats_len
(
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_irat_mobility_stats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_irat_mobility_stats->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_irat_mobility_stats->sampling_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_irat_mobility_stats->rrc_IratIncMobility_LAI);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_irat_mobility_stats
(
    UInt8  **pp_buffer,
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_irat_mobility_stats != PNULL);

    /* This function composes rrc_irat_mobility_stats */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_irat_mobility_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_irat_mobility_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_irat_mobility_stats->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_irat_mobility_stats->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_irat_mobility_stats->cell_index);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_irat_mobility_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_irat_mobility_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_irat_mobility_stats->sampling_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_irat_mobility_stats->sampling_period, "sampling_period");
    *pp_buffer += sizeof(p_rrc_irat_mobility_stats->sampling_period);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_irat_mobility_stats->rrc_IratIncMobility_LAI, "rrc_IratIncMobility_LAI");
    *pp_buffer += sizeof(p_rrc_irat_mobility_stats->rrc_IratIncMobility_LAI);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_cell_status_req_len
(
    rrc_oam_get_cell_status_req_t *p_rrc_oam_get_cell_status_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_status_req->cell_index);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_cell_status_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_status_req_t *p_rrc_oam_get_cell_status_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_req != PNULL);

    /* This function composes rrc_oam_get_cell_status_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_status_req->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_status_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_status_req->cell_index);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_cell_status_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_CELL_STATUS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_status_req
(
    rrc_oam_get_cell_status_req_t  *p_rrc_oam_get_cell_status_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_cell_status_req_len(p_rrc_oam_get_cell_status_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_CELL_STATUS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_CELL_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_CELL_STATUS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_GET_CELL_STATUS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_cell_status_req(&p_oam_msg, p_rrc_oam_get_cell_status_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_cell_status_resp_len
(
    rrc_oam_get_cell_status_resp_t *p_rrc_oam_get_cell_status_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_status_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_status_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_cell_status_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_cell_status_resp->bitmask & RRC_OAM_CELL_STATUS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_cell_status_len(&p_rrc_oam_get_cell_status_resp->cell_status);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_cell_status_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_status_resp_t *p_rrc_oam_get_cell_status_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_resp != PNULL);

    /* This function composes rrc_oam_get_cell_status_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_cell_status_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_status_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_status_resp->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_status_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_status_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_cell_status_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_cell_status_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_get_cell_status_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_cell_status_resp->bitmask & RRC_OAM_CELL_STATUS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_cell_status(pp_buffer, &p_rrc_oam_get_cell_status_resp->cell_status))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_cell_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_CELL_STATUS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_status_resp
(
    rrc_oam_get_cell_status_resp_t  *p_rrc_oam_get_cell_status_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_cell_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_cell_status_resp_len(p_rrc_oam_get_cell_status_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_CELL_STATUS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_CELL_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_CELL_STATUS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_CELL_STATUS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_cell_status_resp(&p_oam_msg, p_rrc_oam_get_cell_status_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_cell_status_len
(
    rrc_oam_cell_status_t *p_rrc_oam_cell_status
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_cell_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_status->num_ue);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_status->num_srb1);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_status->num_srb2);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_status->num_drb);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_cell_status
(
    UInt8  **pp_buffer,
    rrc_oam_cell_status_t *p_rrc_oam_cell_status
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_cell_status != PNULL);

    /* This function composes rrc_oam_cell_status */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_cell_status->num_ue, "num_ue");
    *pp_buffer += sizeof(p_rrc_oam_cell_status->num_ue);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_cell_status->num_srb1, "num_srb1");
    *pp_buffer += sizeof(p_rrc_oam_cell_status->num_srb1);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_cell_status->num_srb2, "num_srb2");
    *pp_buffer += sizeof(p_rrc_oam_cell_status->num_srb2);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_cell_status->num_drb, "num_drb");
    *pp_buffer += sizeof(p_rrc_oam_cell_status->num_drb);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_ue_status_req_len
(
    rrc_oam_get_ue_status_req_t *p_rrc_oam_get_ue_status_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_req->cell_index);

    /* Optional element */
    if(p_rrc_oam_get_ue_status_req->bitmask & RRC_OAM_UE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_req->ue_index);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_ue_status_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_ue_status_req_t *p_rrc_oam_get_ue_status_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_req != PNULL);

    /* This function composes rrc_oam_get_ue_status_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_ue_status_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ue_status_req->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ue_status_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_req->cell_index);

    /* Optional element */
    if(p_rrc_oam_get_ue_status_req->bitmask & RRC_OAM_UE_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_ue_status_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_req->ue_index);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_ue_status_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_UE_STATUS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_ue_status_req
(
    rrc_oam_get_ue_status_req_t  *p_rrc_oam_get_ue_status_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_ue_status_req_len(p_rrc_oam_get_ue_status_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_UE_STATUS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_UE_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_UE_STATUS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_GET_UE_STATUS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_ue_status_req(&p_oam_msg, p_rrc_oam_get_ue_status_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_ue_status_resp_len
(
    rrc_oam_get_ue_status_resp_t *p_rrc_oam_get_ue_status_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ue_status_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_ue_status_resp->bitmask & RRC_OAM_UE_STATUS_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_ue_status_list_len(&p_rrc_oam_get_ue_status_resp->ue_status_list);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_ue_status_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_ue_status_resp_t *p_rrc_oam_get_ue_status_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_resp != PNULL);

    /* This function composes rrc_oam_get_ue_status_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_ue_status_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ue_status_resp->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ue_status_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_ue_status_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ue_status_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_get_ue_status_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_ue_status_resp->bitmask & RRC_OAM_UE_STATUS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ue_status_list(pp_buffer, &p_rrc_oam_get_ue_status_resp->ue_status_list))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_ue_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_UE_STATUS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_ue_status_resp
(
    rrc_oam_get_ue_status_resp_t  *p_rrc_oam_get_ue_status_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_ue_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_ue_status_resp_len(p_rrc_oam_get_ue_status_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_UE_STATUS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_UE_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_UE_STATUS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_UE_STATUS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_ue_status_resp(&p_oam_msg, p_rrc_oam_get_ue_status_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ue_status_len
(
    rrc_oam_ue_status_t *p_rrc_oam_ue_status
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ue_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->ue_state);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->ue_category);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->num_srb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->num_drb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->enb_ue_s1ap_id);

    /* Optional element */
    if(p_rrc_oam_ue_status->bitmask & RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status->mme_ue_s1ap_id);
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ue_status
(
    UInt8  **pp_buffer,
    rrc_oam_ue_status_t *p_rrc_oam_ue_status
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ue_status != PNULL);

    /* This function composes rrc_oam_ue_status */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ue_status->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->bitmask);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ue_status->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->ue_index);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ue_status->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->crnti);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ue_status->ue_state, "ue_state");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->ue_state);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ue_status->ue_category, "ue_category");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->ue_category);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ue_status->num_srb < 1) || (p_rrc_oam_ue_status->num_srb > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->num_srb] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_oam_ue_status->num_srb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ue_status->num_srb, "num_srb");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->num_srb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status->num_drb > 11))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->num_drb] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_oam_ue_status->num_drb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_ue_status->num_drb, "num_drb");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->num_drb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status->enb_ue_s1ap_id > 16777215))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->enb_ue_s1ap_id] should be less than"
            " or equal to 16777215. Incorrect value %u received.", p_rrc_oam_ue_status->enb_ue_s1ap_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ue_status->enb_ue_s1ap_id, "enb_ue_s1ap_id");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->enb_ue_s1ap_id);

    /* Optional element */
    if(p_rrc_oam_ue_status->bitmask & RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_oam_ue_status->mme_ue_s1ap_id, "mme_ue_s1ap_id");
    *pp_buffer += sizeof(p_rrc_oam_ue_status->mme_ue_s1ap_id);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_ue_status_list_len
(
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_ue_status_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_ue_status_list->num_ue_status);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status_list->num_ue_status > RRC_MAX_NUM_UE_STATUS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status_list->num_ue_status] should be less than"
            " or equal to RRC_MAX_NUM_UE_STATUS. Incorrect value %d received.", p_rrc_oam_ue_status_list->num_ue_status);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_status_list->num_ue_status; loop++)
        {
            length += oam_rrc_il_get_rrc_oam_ue_status_len(&p_rrc_oam_ue_status_list->ue_status[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_ue_status_list
(
    UInt8  **pp_buffer,
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_ue_status_list != PNULL);

    /* This function composes rrc_oam_ue_status_list */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status_list->num_ue_status > RRC_MAX_NUM_UE_STATUS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status_list->num_ue_status] should be less than"
            " or equal to RRC_MAX_NUM_UE_STATUS. Incorrect value %u received.", p_rrc_oam_ue_status_list->num_ue_status);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_ue_status_list->num_ue_status, "num_ue_status");
    *pp_buffer += sizeof(p_rrc_oam_ue_status_list->num_ue_status);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_status_list->num_ue_status; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_ue_status(pp_buffer, &p_rrc_oam_ue_status_list->ue_status[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_ps_stats_req_len
(
    rrc_oam_get_ps_stats_req_t *p_rrc_oam_get_ps_stats_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ps_stats_req->cell_index);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_ps_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_ps_stats_req_t *p_rrc_oam_get_ps_stats_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_req != PNULL);

    /* This function composes rrc_oam_get_ps_stats_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ps_stats_req->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ps_stats_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_ps_stats_req->cell_index);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_ps_stats_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_PS_STATS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_ps_stats_req
(
    rrc_oam_get_ps_stats_req_t  *p_rrc_oam_get_ps_stats_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_ps_stats_req_len(p_rrc_oam_get_ps_stats_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_PS_STATS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_PS_STATS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_PS_STATS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_GET_PS_STATS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_ps_stats_req(&p_oam_msg, p_rrc_oam_get_ps_stats_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_ps_stats_resp_len
(
    rrc_oam_get_ps_stats_resp_t *p_rrc_oam_get_ps_stats_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ps_stats_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_ps_stats_resp->response);

    /* Get length of IE */
    length += oam_rrc_il_get_oam_rrc_ps_stats_resp_len(&p_rrc_oam_get_ps_stats_resp->ps_stats_resp);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_ps_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_ps_stats_resp_t *p_rrc_oam_get_ps_stats_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_resp != PNULL);

    /* This function composes rrc_oam_get_ps_stats_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ps_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ps_stats_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_get_ps_stats_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_ps_stats_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_get_ps_stats_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_get_ps_stats_resp->response);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_oam_rrc_ps_stats_resp(pp_buffer, &p_rrc_oam_get_ps_stats_resp->ps_stats_resp))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_ps_stats_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_PS_STATS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_ps_stats_resp
(
    rrc_oam_get_ps_stats_resp_t  *p_rrc_oam_get_ps_stats_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_ps_stats_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_ps_stats_resp_len(p_rrc_oam_get_ps_stats_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_PS_STATS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_PS_STATS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_PS_STATS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_PS_STATS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_ps_stats_resp(&p_oam_msg, p_rrc_oam_get_ps_stats_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_ueRrcInfoStatus_len
(
    ueRrcInfoStatus_t *p_ueRrcInfoStatus
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_ueRrcInfoStatus != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ueRrcInfoStatus->cRNTI);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ueRrcInfoStatus->ueIndex);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ueRrcInfoStatus->enbS1APId);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ueRrcInfoStatus->mmeS1APId);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_ueRrcInfoStatus->status);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_ueRrcInfoStatus
(
    UInt8  **pp_buffer,
    ueRrcInfoStatus_t *p_ueRrcInfoStatus
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_ueRrcInfoStatus != PNULL);

    /* This function composes ueRrcInfoStatus */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_ueRrcInfoStatus->cRNTI, "cRNTI");
    *pp_buffer += sizeof(p_ueRrcInfoStatus->cRNTI);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_ueRrcInfoStatus->ueIndex, "ueIndex");
    *pp_buffer += sizeof(p_ueRrcInfoStatus->ueIndex);

    /* Check for correct range [H - higher boundary] */
    if ((p_ueRrcInfoStatus->enbS1APId > 16777215))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ueRrcInfoStatus->enbS1APId] should be less than"
            " or equal to 16777215. Incorrect value %u received.", p_ueRrcInfoStatus->enbS1APId);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_ueRrcInfoStatus->enbS1APId, "enbS1APId");
    *pp_buffer += sizeof(p_ueRrcInfoStatus->enbS1APId);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_ueRrcInfoStatus->mmeS1APId, "mmeS1APId");
    *pp_buffer += sizeof(p_ueRrcInfoStatus->mmeS1APId);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_ueRrcInfoStatus->status, "status");
    *pp_buffer += sizeof(p_ueRrcInfoStatus->status);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_oam_rrc_ps_stats_resp_len
(
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_oam_rrc_ps_stats_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_oam_rrc_ps_stats_resp->noOfUe);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_oam_rrc_ps_stats_resp->noOfUe; loop++)
        {
            length += oam_rrc_il_get_ueRrcInfoStatus_len(&p_oam_rrc_ps_stats_resp->ueInfoStatus[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_oam_rrc_ps_stats_resp
(
    UInt8  **pp_buffer,
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_oam_rrc_ps_stats_resp != PNULL);

    /* This function composes oam_rrc_ps_stats_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_oam_rrc_ps_stats_resp->noOfUe, "noOfUe");
    *pp_buffer += sizeof(p_oam_rrc_ps_stats_resp->noOfUe);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_oam_rrc_ps_stats_resp->noOfUe; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_ueRrcInfoStatus(pp_buffer, &p_oam_rrc_ps_stats_resp->ueInfoStatus[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_reset_cell_stats_req_len
(
    rrc_oam_reset_cell_stats_req_t *p_rrc_oam_reset_cell_stats_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_reset_cell_stats_req->cell_index);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_reset_cell_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_reset_cell_stats_req_t *p_rrc_oam_reset_cell_stats_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_req != PNULL);

    /* This function composes rrc_oam_reset_cell_stats_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_req->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_reset_cell_stats_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_reset_cell_stats_req->cell_index);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_reset_cell_stats_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_RESET_CELL_STATS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_reset_cell_stats_req
(
    rrc_oam_reset_cell_stats_req_t  *p_rrc_oam_reset_cell_stats_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_reset_cell_stats_req_len(p_rrc_oam_reset_cell_stats_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_RESET_CELL_STATS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_RESET_CELL_STATS_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_RESET_CELL_STATS_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_RESET_CELL_STATS_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_reset_cell_stats_req(&p_oam_msg, p_rrc_oam_reset_cell_stats_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_reset_cell_stats_resp_len
(
    rrc_oam_reset_cell_stats_resp_t *p_rrc_oam_reset_cell_stats_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_reset_cell_stats_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_reset_cell_stats_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_reset_cell_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_reset_cell_stats_resp_t *p_rrc_oam_reset_cell_stats_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_resp != PNULL);

    /* This function composes rrc_oam_reset_cell_stats_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_reset_cell_stats_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_reset_cell_stats_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_reset_cell_stats_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_reset_cell_stats_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_reset_cell_stats_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_RESET_CELL_STATS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_reset_cell_stats_resp
(
    rrc_oam_reset_cell_stats_resp_t  *p_rrc_oam_reset_cell_stats_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_reset_cell_stats_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_reset_cell_stats_resp_len(p_rrc_oam_reset_cell_stats_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_RESET_CELL_STATS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_RESET_CELL_STATS_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_RESET_CELL_STATS_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_RESET_CELL_STATS_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_reset_cell_stats_resp(&p_oam_msg, p_rrc_oam_reset_cell_stats_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_cleanup_resp_len
(
    rrc_oam_cleanup_resp_t *p_rrc_oam_cleanup_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_cleanup_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cleanup_resp->response);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_cleanup_resp
(
    UInt8  **pp_buffer,
    rrc_oam_cleanup_resp_t *p_rrc_oam_cleanup_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_cleanup_resp != PNULL);

    /* This function composes rrc_oam_cleanup_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cleanup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cleanup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_cleanup_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cleanup_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_cleanup_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_cleanup_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CLEANUP_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_cleanup_resp
(
    rrc_oam_cleanup_resp_t  *p_rrc_oam_cleanup_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_cleanup_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_cleanup_resp_len(p_rrc_oam_cleanup_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CLEANUP_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CLEANUP_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CLEANUP_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_CLEANUP_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_cleanup_resp(&p_oam_msg, p_rrc_oam_cleanup_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_init_ind_len
(
    rrc_oam_init_ind_t *p_rrc_oam_init_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_init_ind != PNULL);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_init_ind
(
    UInt8  **pp_buffer,
    rrc_oam_init_ind_t *p_rrc_oam_init_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_init_ind != PNULL);

    /* This function composes rrc_oam_init_ind */
    

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_init_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_INIT_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_init_ind
(
    rrc_oam_init_ind_t  *p_rrc_oam_init_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_init_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_init_ind_len(p_rrc_oam_init_ind);
    
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_INIT_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_INIT_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_INIT_IND, msg_length,cell_index);

    /* Fill RRC_OAM_INIT_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_init_ind(&p_oam_msg, p_rrc_oam_init_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
	{
		oam_mem_free(p_msg,&oam_error);
		return OAM_FAILURE;
	}

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}
rrc_length_t
oam_rrc_il_get_rrc_oam_cell_traffic_trace_start_len
(
    rrc_oam_cell_traffic_trace_start_t *p_rrc_oam_cell_traffic_trace_start
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_start->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_start->interface_to_trace);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_start->trace_depth);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_trace_collection_entity_len(&p_rrc_oam_cell_traffic_trace_start->trace_collection_entity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_cell_traffic_trace_start->traceRef);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_cell_traffic_trace_start
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_start_t *p_rrc_oam_cell_traffic_trace_start
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start != PNULL);

    /* This function composes rrc_oam_cell_traffic_trace_start */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_start->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->interface_to_trace > 7))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->interface_to_trace] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->interface_to_trace);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start->interface_to_trace, "interface_to_trace");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_start->interface_to_trace);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->trace_depth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->trace_depth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->trace_depth);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start->trace_depth, "trace_depth");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_start->trace_depth);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_collection_entity(pp_buffer, &p_rrc_oam_cell_traffic_trace_start->trace_collection_entity))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_cell_traffic_trace_start->traceRef); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start->traceRef[loop], "traceRef[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_cell_traffic_trace_start
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CELL_TRAFFIC_TRACE_START message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_start
(
    rrc_oam_cell_traffic_trace_start_t  *p_rrc_oam_cell_traffic_trace_start,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_cell_traffic_trace_start_len(p_rrc_oam_cell_traffic_trace_start);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CELL_TRAFFIC_TRACE_START", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CELL_TRAFFIC_TRACE_START, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CELL_TRAFFIC_TRACE_START, msg_length,cell_index);

    /* Fill RRC_OAM_CELL_TRAFFIC_TRACE_START message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_cell_traffic_trace_start(&p_oam_msg, p_rrc_oam_cell_traffic_trace_start))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_trace_collection_entity_len
(
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_trace_collection_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_collection_entity->length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_trace_collection_entity->length * sizeof(p_rrc_oam_trace_collection_entity->data[0]));

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_trace_collection_entity
(
    UInt8  **pp_buffer,
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_trace_collection_entity != PNULL);

    /* This function composes rrc_oam_trace_collection_entity */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_collection_entity->length > MAX_TRANSPORT_LAYER_ADDR_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_collection_entity->length] should be less than"
            " or equal to MAX_TRANSPORT_LAYER_ADDR_SIZE. Incorrect value %u received.", p_rrc_oam_trace_collection_entity->length);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_collection_entity->length, "length");
    *pp_buffer += sizeof(p_rrc_oam_trace_collection_entity->length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_trace_collection_entity->length; loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_collection_entity->data[loop], "data[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_cell_traffic_trace_stop_len
(
    rrc_oam_cell_traffic_trace_stop_t *p_rrc_oam_cell_traffic_trace_stop
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_stop != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_stop->cell_index);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_cell_traffic_trace_stop
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_stop_t *p_rrc_oam_cell_traffic_trace_stop
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_stop != PNULL);

    /* This function composes rrc_oam_cell_traffic_trace_stop */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_stop->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_stop->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_stop->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_stop->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_stop->cell_index);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_cell_traffic_trace_stop
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CELL_TRAFFIC_TRACE_STOP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_stop
(
    rrc_oam_cell_traffic_trace_stop_t  *p_rrc_oam_cell_traffic_trace_stop,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_stop != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_cell_traffic_trace_stop_len(p_rrc_oam_cell_traffic_trace_stop);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CELL_TRAFFIC_TRACE_STOP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CELL_TRAFFIC_TRACE_STOP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CELL_TRAFFIC_TRACE_STOP, msg_length,cell_index);

    /* Fill RRC_OAM_CELL_TRAFFIC_TRACE_STOP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_cell_traffic_trace_stop(&p_oam_msg, p_rrc_oam_cell_traffic_trace_stop))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_cell_traffic_trace_start_error_ind_len
(
    rrc_oam_cell_traffic_trace_start_error_ind_t *p_rrc_oam_cell_traffic_trace_start_error_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start_error_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_cell_traffic_trace_start_error_ind->cause);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_cell_traffic_trace_start_error_ind
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_start_error_ind_t *p_rrc_oam_cell_traffic_trace_start_error_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start_error_ind != PNULL);

    /* This function composes rrc_oam_cell_traffic_trace_start_error_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_cell_traffic_trace_start_error_ind->cause, "cause");
    *pp_buffer += sizeof(p_rrc_oam_cell_traffic_trace_start_error_ind->cause);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_cell_traffic_trace_start_error_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_start_error_ind
(
    rrc_oam_cell_traffic_trace_start_error_ind_t  *p_rrc_oam_cell_traffic_trace_start_error_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_cell_traffic_trace_start_error_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_cell_traffic_trace_start_error_ind_len(p_rrc_oam_cell_traffic_trace_start_error_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND, msg_length,cell_index);

    /* Fill RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_cell_traffic_trace_start_error_ind(&p_oam_msg, p_rrc_oam_cell_traffic_trace_start_error_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_trace_file_transfer_start_ind_len
(
    rrc_oam_trace_file_transfer_start_ind_t *p_rrc_oam_trace_file_transfer_start_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_start_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_file_transfer_start_ind->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_file_transfer_start_ind->ue_index);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_e_utran_trace_id_len(&p_rrc_oam_trace_file_transfer_start_ind->e_utran_trace_id);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_trace_file_len(&p_rrc_oam_trace_file_transfer_start_ind->trace_file);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_trace_collection_entity_len(&p_rrc_oam_trace_file_transfer_start_ind->trace_collection_entity);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_trace_file_transfer_start_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_transfer_start_ind_t *p_rrc_oam_trace_file_transfer_start_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_start_ind != PNULL);

    /* This function composes rrc_oam_trace_file_transfer_start_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_file_transfer_start_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file_transfer_start_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_file_transfer_start_ind->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_file_transfer_start_ind->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_trace_file_transfer_start_ind->cell_index);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_trace_file_transfer_start_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_oam_trace_file_transfer_start_ind->ue_index);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_e_utran_trace_id(pp_buffer, &p_rrc_oam_trace_file_transfer_start_ind->e_utran_trace_id))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_file(pp_buffer, &p_rrc_oam_trace_file_transfer_start_ind->trace_file))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_collection_entity(pp_buffer, &p_rrc_oam_trace_file_transfer_start_ind->trace_collection_entity))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_trace_file_transfer_start_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_TRACE_FILE_TRANSFER_START_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_trace_file_transfer_start_ind
(
    rrc_oam_trace_file_transfer_start_ind_t  *p_rrc_oam_trace_file_transfer_start_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_start_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_trace_file_transfer_start_ind_len(p_rrc_oam_trace_file_transfer_start_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_TRACE_FILE_TRANSFER_START_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_TRACE_FILE_TRANSFER_START_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_TRACE_FILE_TRANSFER_START_IND, msg_length,cell_index);

    /* Fill RRC_OAM_TRACE_FILE_TRANSFER_START_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_file_transfer_start_ind(&p_oam_msg, p_rrc_oam_trace_file_transfer_start_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_e_utran_trace_id_len
(
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_e_utran_trace_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_e_utran_trace_id->trace_reference);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_e_utran_trace_id->trace_recording_session_reference);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_e_utran_trace_id
(
    UInt8  **pp_buffer,
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_e_utran_trace_id != PNULL);

    /* This function composes rrc_oam_e_utran_trace_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_e_utran_trace_id->trace_reference); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_e_utran_trace_id->trace_reference[loop], "trace_reference[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_e_utran_trace_id->trace_recording_session_reference, "trace_recording_session_reference");
    *pp_buffer += sizeof(p_rrc_oam_e_utran_trace_id->trace_recording_session_reference);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_oam_trace_file_len
(
    rrc_oam_trace_file_t *p_rrc_oam_trace_file
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_trace_file != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_file->len_file_name);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_oam_trace_file->len_file_name * sizeof(p_rrc_oam_trace_file->file_name[0]));

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_oam_trace_file
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_t *p_rrc_oam_trace_file
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_trace_file != PNULL);

    /* This function composes rrc_oam_trace_file */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_trace_file->len_file_name < 1) || (p_rrc_oam_trace_file->len_file_name > MAX_FILE_NAME_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file->len_file_name] should be in range "
            "1 to MAX_FILE_NAME_SIZE. Incorrect value %u received.", p_rrc_oam_trace_file->len_file_name);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_trace_file->len_file_name, "len_file_name");
    *pp_buffer += sizeof(p_rrc_oam_trace_file->len_file_name);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_trace_file->len_file_name; loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_file->file_name[loop], "file_name[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_trace_file_transfer_complete_ind_len
(
    rrc_oam_trace_file_transfer_complete_ind_t *p_rrc_oam_trace_file_transfer_complete_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_complete_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_file_transfer_complete_ind->cell_index);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_oam_trace_file_len(&p_rrc_oam_trace_file_transfer_complete_ind->trace_file);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_trace_file_transfer_complete_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_transfer_complete_ind_t *p_rrc_oam_trace_file_transfer_complete_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_complete_ind != PNULL);

    /* This function composes rrc_oam_trace_file_transfer_complete_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_file_transfer_complete_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file_transfer_complete_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_file_transfer_complete_ind->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_file_transfer_complete_ind->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_trace_file_transfer_complete_ind->cell_index);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_file(pp_buffer, &p_rrc_oam_trace_file_transfer_complete_ind->trace_file))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_trace_file_transfer_complete_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_trace_file_transfer_complete_ind
(
    rrc_oam_trace_file_transfer_complete_ind_t  *p_rrc_oam_trace_file_transfer_complete_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_trace_file_transfer_complete_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_trace_file_transfer_complete_ind_len(p_rrc_oam_trace_file_transfer_complete_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND, msg_length,cell_index);

    /* Fill RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_file_transfer_complete_ind(&p_oam_msg, p_rrc_oam_trace_file_transfer_complete_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_trace_start_error_ind_len
(
    rrc_oam_trace_start_error_ind_t *p_rrc_oam_trace_start_error_ind
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_trace_start_error_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_trace_start_error_ind->cell_index);

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_trace_failure_list_len(&p_rrc_oam_trace_start_error_ind->trace_failure_list);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_trace_start_error_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_start_error_ind_t *p_rrc_oam_trace_start_error_ind
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_trace_start_error_ind != PNULL);

    /* This function composes rrc_oam_trace_start_error_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_start_error_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_start_error_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_start_error_ind->cell_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_trace_start_error_ind->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_oam_trace_start_error_ind->cell_index);

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_trace_failure_list(pp_buffer, &p_rrc_oam_trace_start_error_ind->trace_failure_list))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_trace_start_error_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_TRACE_START_ERROR_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_trace_start_error_ind
(
    rrc_oam_trace_start_error_ind_t  *p_rrc_oam_trace_start_error_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_trace_start_error_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_trace_start_error_ind_len(p_rrc_oam_trace_start_error_ind);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_TRACE_START_ERROR_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_TRACE_START_ERROR_IND, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_TRACE_START_ERROR_IND, msg_length,cell_index);

    /* Fill RRC_OAM_TRACE_START_ERROR_IND message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_trace_start_error_ind(&p_oam_msg, p_rrc_oam_trace_start_error_ind))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_trace_failure_list_item_len
(
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_trace_failure_list_item != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_trace_failure_list_item->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_trace_failure_list_item->cause);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_trace_failure_list_item
(
    UInt8  **pp_buffer,
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_trace_failure_list_item != PNULL);

    /* This function composes rrc_trace_failure_list_item */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_trace_failure_list_item->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_trace_failure_list_item->ue_index);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_trace_failure_list_item->cause, "cause");
    *pp_buffer += sizeof(p_rrc_trace_failure_list_item->cause);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_trace_failure_list_len
(
    rrc_trace_failure_list_t *p_rrc_trace_failure_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_trace_failure_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_trace_failure_list->counter);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_trace_failure_list->counter < 1) || (p_rrc_trace_failure_list->counter > RRC_MAX_NUM_SUPPORTED_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_trace_failure_list->counter] should be in range "
            "1 to RRC_MAX_NUM_SUPPORTED_UE. Incorrect value %d received.", p_rrc_trace_failure_list->counter);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_trace_failure_list->counter; loop++)
        {
            length += oam_rrc_il_get_rrc_trace_failure_list_item_len(&p_rrc_trace_failure_list->item[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_trace_failure_list
(
    UInt8  **pp_buffer,
    rrc_trace_failure_list_t *p_rrc_trace_failure_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_trace_failure_list != PNULL);

    /* This function composes rrc_trace_failure_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_trace_failure_list->counter < 1) || (p_rrc_trace_failure_list->counter > RRC_MAX_NUM_SUPPORTED_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_trace_failure_list->counter] should be in range "
            "1 to RRC_MAX_NUM_SUPPORTED_UE. Incorrect value %u received.", p_rrc_trace_failure_list->counter);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_trace_failure_list->counter, "counter");
    *pp_buffer += sizeof(p_rrc_trace_failure_list->counter);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_trace_failure_list->counter; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_trace_failure_list_item(pp_buffer, &p_rrc_trace_failure_list->item[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_add_lgw_req_len
(
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_add_lgw_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_add_lgw_req->bitmask);

    /* Optional element */
    if(p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_add_lgw_req->ip_addr);
    }

    /* Optional element */
    if(p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_oam_add_lgw_req->ipv6_addr);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_add_lgw_req
(
    UInt8  **pp_buffer,
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_add_lgw_req != PNULL);

    /* This function composes rrc_oam_add_lgw_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_add_lgw_req->bitmask);

    /* Optional element */
    if(p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_add_lgw_req->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_req->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Optional element */
    if(p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_add_lgw_req->ipv6_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_req->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_add_lgw_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_ADD_LGW_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_add_lgw_req
(
    rrc_oam_add_lgw_req_t  *p_rrc_oam_add_lgw_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_add_lgw_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_add_lgw_req_len(p_rrc_oam_add_lgw_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_ADD_LGW_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_ADD_LGW_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_ADD_LGW_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_ADD_LGW_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_add_lgw_req(&p_oam_msg, p_rrc_oam_add_lgw_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_add_lgw_resp_len
(
    rrc_oam_add_lgw_resp_t *p_rrc_oam_add_lgw_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_add_lgw_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_add_lgw_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_add_lgw_resp->response);

    /* Optional element */
    if(p_rrc_oam_add_lgw_resp->bitmask & RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_add_lgw_resp->cause);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_add_lgw_resp
(
    UInt8  **pp_buffer,
    rrc_oam_add_lgw_resp_t *p_rrc_oam_add_lgw_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_add_lgw_resp != PNULL);

    /* This function composes rrc_oam_add_lgw_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_add_lgw_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_add_lgw_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_add_lgw_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_add_lgw_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_add_lgw_resp->response);

    /* Optional element */
    if(p_rrc_oam_add_lgw_resp->bitmask & RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_add_lgw_resp->cause < 1) || (p_rrc_oam_add_lgw_resp->cause > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_add_lgw_resp->cause] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_oam_add_lgw_resp->cause);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_add_lgw_resp->cause, "cause");
    *pp_buffer += sizeof(p_rrc_oam_add_lgw_resp->cause);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_add_lgw_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_ADD_LGW_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_add_lgw_resp
(
    rrc_oam_add_lgw_resp_t  *p_rrc_oam_add_lgw_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_add_lgw_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_add_lgw_resp_len(p_rrc_oam_add_lgw_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_ADD_LGW_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_ADD_LGW_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_ADD_LGW_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_ADD_LGW_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_add_lgw_resp(&p_oam_msg, p_rrc_oam_add_lgw_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_delete_lgw_req_len
(
    rrc_oam_delete_lgw_req_t *p_rrc_oam_delete_lgw_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_delete_lgw_req->bitmask);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_delete_lgw_req
(
    UInt8  **pp_buffer,
    rrc_oam_delete_lgw_req_t *p_rrc_oam_delete_lgw_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_req != PNULL);

    /* This function composes rrc_oam_delete_lgw_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_delete_lgw_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_delete_lgw_req->bitmask);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_delete_lgw_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_DELETE_LGW_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_delete_lgw_req
(
    rrc_oam_delete_lgw_req_t  *p_rrc_oam_delete_lgw_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_delete_lgw_req_len(p_rrc_oam_delete_lgw_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_DELETE_LGW_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_DELETE_LGW_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_DELETE_LGW_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_DELETE_LGW_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_delete_lgw_req(&p_oam_msg, p_rrc_oam_delete_lgw_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_delete_lgw_resp_len
(
    rrc_oam_delete_lgw_resp_t *p_rrc_oam_delete_lgw_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_delete_lgw_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_delete_lgw_resp->response);

    /* Optional element */
    if(p_rrc_oam_delete_lgw_resp->bitmask & RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_delete_lgw_resp->cause);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_delete_lgw_resp
(
    UInt8  **pp_buffer,
    rrc_oam_delete_lgw_resp_t *p_rrc_oam_delete_lgw_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_resp != PNULL);

    /* This function composes rrc_oam_delete_lgw_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_delete_lgw_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_delete_lgw_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_delete_lgw_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_delete_lgw_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_delete_lgw_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_delete_lgw_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_delete_lgw_resp->response);

    /* Optional element */
    if(p_rrc_oam_delete_lgw_resp->bitmask & RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_delete_lgw_resp->cause < 1) || (p_rrc_oam_delete_lgw_resp->cause > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_delete_lgw_resp->cause] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_oam_delete_lgw_resp->cause);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrc_oam_delete_lgw_resp->cause, "cause");
    *pp_buffer += sizeof(p_rrc_oam_delete_lgw_resp->cause);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_delete_lgw_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_DELETE_LGW_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_delete_lgw_resp
(
    rrc_oam_delete_lgw_resp_t  *p_rrc_oam_delete_lgw_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_delete_lgw_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_delete_lgw_resp_len(p_rrc_oam_delete_lgw_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_DELETE_LGW_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_DELETE_LGW_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_DELETE_LGW_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_DELETE_LGW_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_delete_lgw_resp(&p_oam_msg, p_rrc_oam_delete_lgw_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_debug_info_req_len
(
    rrc_oam_get_debug_info_req_t *p_rrc_oam_get_debug_info_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_debug_info_req->debug_type);

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_debug_info_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_debug_info_req_t *p_rrc_oam_get_debug_info_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_req != PNULL);

    /* This function composes rrc_oam_get_debug_info_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_debug_info_req->debug_type, "debug_type");
    *pp_buffer += sizeof(p_rrc_oam_get_debug_info_req->debug_type);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_debug_info_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_DEBUG_INFO_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_debug_info_req
(
    rrc_oam_get_debug_info_req_t  *p_rrc_oam_get_debug_info_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_debug_info_req_len(p_rrc_oam_get_debug_info_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_DEBUG_INFO_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_DEBUG_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_DEBUG_INFO_REQ, msg_length,cell_index);

    /* Fill RRC_OAM_GET_DEBUG_INFO_REQ message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_debug_info_req(&p_oam_msg, p_rrc_oam_get_debug_info_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
oam_rrc_il_get_rrc_oam_get_debug_info_resp_len
(
    rrc_oam_get_debug_info_resp_t *p_rrc_oam_get_debug_info_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_debug_info_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_debug_info_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_oam_get_debug_info_resp->fail_cause);
    }

    /* Optional element */
    if(p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT)
    {

    /* Get length of IE */
    length += oam_rrc_il_get_rrc_MemPoolStats_len(&p_rrc_oam_get_debug_info_resp->csplMemPoolStats);
    }

    return length;
}

oam_return_et
oam_rrc_il_compose_rrc_oam_get_debug_info_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_debug_info_resp_t *p_rrc_oam_get_debug_info_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_resp != PNULL);

    /* This function composes rrc_oam_get_debug_info_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrc_oam_get_debug_info_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_oam_get_debug_info_resp->bitmask);

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT32(*pp_buffer, &p_rrc_oam_get_debug_info_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_oam_get_debug_info_resp->response);

    /* Optional element */
    if(p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT32(*pp_buffer, &p_rrc_oam_get_debug_info_resp->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_rrc_oam_get_debug_info_resp->fail_cause);
    }

    /* Optional element */
    if(p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_MemPoolStats(pp_buffer, &p_rrc_oam_get_debug_info_resp->csplMemPoolStats))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrc_il_send_rrc_oam_get_debug_info_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_OAM_GET_DEBUG_INFO_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrc_il_send_rrc_oam_get_debug_info_resp
(
    rrc_oam_get_debug_info_resp_t  *p_rrc_oam_get_debug_info_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_oam_msg;

    OAM_NULL_CHECK(p_rrc_oam_get_debug_info_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = oam_rrc_il_get_rrc_oam_get_debug_info_resp_len(p_rrc_oam_get_debug_info_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRC_OAM_GET_DEBUG_INFO_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_OAM_GET_DEBUG_INFO_RESP, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, RRC_OAM_GET_DEBUG_INFO_RESP, msg_length,cell_index);

    /* Fill RRC_OAM_GET_DEBUG_INFO_RESP message */
    p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_rrc_il_compose_rrc_oam_get_debug_info_resp(&p_oam_msg, p_rrc_oam_get_debug_info_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_PoolStats_len
(
    rrc_PoolStats *p_rrc_PoolStats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_PoolStats != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_PoolStats->bufSize);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_PoolStats->numOfBuf);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_PoolStats->numOfAllocBuf);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_PoolStats->numOfPeakBuf);

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_PoolStats
(
    UInt8  **pp_buffer,
    rrc_PoolStats *p_rrc_PoolStats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_PoolStats != PNULL);

    /* This function composes rrc_PoolStats */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_PoolStats->bufSize, "bufSize");
    *pp_buffer += sizeof(p_rrc_PoolStats->bufSize);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_PoolStats->numOfBuf, "numOfBuf");
    *pp_buffer += sizeof(p_rrc_PoolStats->numOfBuf);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_PoolStats->numOfAllocBuf, "numOfAllocBuf");
    *pp_buffer += sizeof(p_rrc_PoolStats->numOfAllocBuf);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_PoolStats->numOfPeakBuf, "numOfPeakBuf");
    *pp_buffer += sizeof(p_rrc_PoolStats->numOfPeakBuf);

    return OAM_SUCCESS;
}

static
rrc_length_t
oam_rrc_il_get_rrc_MemPoolStats_len
(
    rrc_MemPoolStats *p_rrc_MemPoolStats
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_MemPoolStats != PNULL);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_MemPoolStats->stats); loop++)
        {
            length += oam_rrc_il_get_rrc_PoolStats_len(&p_rrc_MemPoolStats->stats[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_rrc_il_compose_rrc_MemPoolStats
(
    UInt8  **pp_buffer,
    rrc_MemPoolStats *p_rrc_MemPoolStats
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_MemPoolStats != PNULL);

    /* This function composes rrc_MemPoolStats */
    

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_MemPoolStats->stats); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_compose_rrc_PoolStats(pp_buffer, &p_rrc_MemPoolStats->stats[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

