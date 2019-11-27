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
 *  File Description : The file rrc_oam_il_parser.c contains the definitions 
 *                     of rrc-oam interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "oam_rrc_il_parser.h"
#include "rrc_oam_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(UInt16) + sizeof(UInt16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ext_comm_info
(
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ext_connection_status
(
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_integrity_algorithms
(
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ciphering_algorithms
(
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_supported_security_algorithms
(
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_sector_info
(
    rrc_oam_sector_info_t *p_rrc_oam_sector_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_sector_info_list
(
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_timer_info
(
    rrc_oam_timer_info_t *p_rrc_oam_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_llim_timer_info
(
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_csc_timer_info
(
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_uecc_timer_info
(
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_module_timers_info
(
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_shared_mem_config
(
    rrc_shared_mem_config_t *p_rrc_shared_mem_config,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_connection_establishment
(
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_connection_reestablishment
(
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_connEstabTime
(
    rrc_connEstabTime_t *p_rrc_connEstabTime,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
(
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_ue_context_release
(
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_connection_related_stats
(
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_estab_init_fail
(
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_estab_add_fail
(
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_erab_estabTime
(
    rrc_erab_estabTime_t *p_rrc_erab_estabTime,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_setup
(
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_rel_enb_cause
(
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_rel_fail_cause
(
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_release
(
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_mod_fail_cause
(
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_modification
(
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_number
(
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_related_stats
(
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutAtt_cause
(
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutSucc_cause
(
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_intra_enb_ho_stats
(
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_InterEnbOutAtt_cause
(
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_InterEnbOutSucc_cause
(
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_enb_ho_stats
(
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_mnc
(
    mnc_t *p_mnc,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_plmn_identity
(
    plmn_identity_t *p_plmn_identity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_cell_global_id_eutra
(
    cell_global_id_eutra_t *p_cell_global_id_eutra,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_OutAttTarget_cause
(
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_OutSuccTarget_cause
(
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats
(
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats_list
(
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_intra_freq_ho_stats
(
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_freq_ho_stats
(
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_drx_non_drx_ho_stats
(
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_utran_geran_cgi
(
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_cdma2000_1xrtt_cell_identifier
(
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_cdma2000_hrpd_sector_identifier
(
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_inter_rat_global_cgi
(
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IratOutAtt_cause
(
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IratOutSucc_cause
(
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats
(
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats_list
(
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_related_stats
(
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_ue_assoc_s1_connection
(
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_csc_oam_stat_paging
(
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_connection_stats
(
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_stats
(
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_stats
(
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_rrc_connection_stats_info
(
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_stats_info
(
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_stats_info
(
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_sampling_period_info
(
    rrc_sampling_period_info_t *p_rrc_sampling_period_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_connection_stats
(
    rrc_connection_stats_t *p_rrc_connection_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_handover_stats
(
    rrc_handover_stats_t *p_rrc_handover_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_erab_stats
(
    rrc_erab_stats_t *p_rrc_erab_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_ue_assoc_s1_conn_stats
(
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_paging_stats
(
    rrc_paging_stats_t *p_rrc_paging_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_irat_mobility_stats
(
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_cell_status
(
    rrc_oam_cell_status_t *p_rrc_oam_cell_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ue_status
(
    rrc_oam_ue_status_t *p_rrc_oam_ue_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ue_status_list
(
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_ueRrcInfoStatus
(
    ueRrcInfoStatus_t *p_ueRrcInfoStatus,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_oam_rrc_ps_stats_resp
(
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_trace_collection_entity
(
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_e_utran_trace_id
(
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_oam_trace_file
(
    rrc_oam_trace_file_t *p_rrc_oam_trace_file,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_trace_failure_list_item
(
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_trace_failure_list
(
    rrc_trace_failure_list_t *p_rrc_trace_failure_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_PoolStats
(
    rrc_PoolStats *p_rrc_PoolStats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_MemPoolStats
(
    rrc_MemPoolStats *p_rrc_MemPoolStats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_return_et
oam_rrc_il_parse_rrc_oam_communication_info_req
(
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_communication_info_req, 0, sizeof(rrc_oam_communication_info_req_t));

    /* This function parses rrc_oam_communication_info_req */

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_req->ext_comm_info_upper_layer); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ext_comm_info(
                &p_rrc_oam_communication_info_req->ext_comm_info_upper_layer[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ext_comm_info
(
    rrc_oam_ext_comm_info_t *p_rrc_oam_ext_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ext_comm_info, 0, sizeof(rrc_oam_ext_comm_info_t));

    /* This function parses rrc_oam_ext_comm_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ext_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ext_comm_info->ext_module_id, p_src + *p_length_read, "ext_module_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ext_comm_info->ext_module_id < 1) || (p_rrc_oam_ext_comm_info->ext_module_id > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_comm_info->ext_module_id] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_ext_comm_info->ext_module_id);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ext_comm_info->transport_mode, p_src + *p_length_read, "transport_mode");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ext_comm_info->transport_mode > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_comm_info->transport_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_ext_comm_info->transport_mode);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_ext_comm_info->bitmask & RRC_OAM_IP_ADDR_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ext_comm_info->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ext_comm_info->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_ext_comm_info->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ext_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_communication_info_resp
(
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_communication_info_resp, 0, sizeof(rrc_oam_communication_info_resp_t));

    /* This function parses rrc_oam_communication_info_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_communication_info_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_communication_info_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_communication_info_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_communication_info_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_communication_info_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_communication_info_resp->bitmask & RRC_OAM_COMM_INFO_FAIL_CAUSE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_communication_info_resp->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_communication_info_resp->cause > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_communication_info_resp->cause] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_communication_info_resp->cause);
        return OAM_FAILURE;
    }
    }

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ext_connection_status(
                &p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ext_connection_status
(
    rrc_oam_ext_connection_status_t *p_rrc_oam_ext_connection_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ext_connection_status, 0, sizeof(rrc_oam_ext_connection_status_t));

    /* This function parses rrc_oam_ext_connection_status */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ext_connection_status->ext_module_id, p_src + *p_length_read, "ext_module_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ext_connection_status->ext_module_id < 1) || (p_rrc_oam_ext_connection_status->ext_module_id > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_connection_status->ext_module_id] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_ext_connection_status->ext_module_id);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ext_connection_status->connection_status, p_src + *p_length_read, "connection_status");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ext_connection_status->connection_status > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ext_connection_status->connection_status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_ext_connection_status->connection_status);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_provision_req
(
    rrc_oam_provision_req_t *p_rrc_oam_provision_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_provision_req, 0, sizeof(rrc_oam_provision_req_t));

    /* This function parses rrc_oam_provision_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_provision_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_provision_req->log_level);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_LOG_FILE_NAME_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_provision_req->log_file_name) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_provision_req->log_file_name); loop++)
        {
            OAM_CP_UNPACK_SINT8((void*)&p_rrc_oam_provision_req->log_file_name[loop], (void*)(p_src + *p_length_read), "log_file_name[]");
            *p_length_read += sizeof(SInt8);
        }
    }
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->phy_sync_mode, p_src + *p_length_read, "phy_sync_mode");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_sector_info_list(
        &p_rrc_oam_provision_req->sector_info_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_llim_timer_info(
        &p_rrc_oam_provision_req->llim_timer_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_csc_timer_info(
        &p_rrc_oam_provision_req->csc_timer_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_uecc_timer_info(
        &p_rrc_oam_provision_req->uecc_timer_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_provision_req->non_broadcast_sfn_offset, p_src + *p_length_read, "non_broadcast_sfn_offset");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->non_broadcast_sfn_offset > 1023))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->non_broadcast_sfn_offset] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_oam_provision_req->non_broadcast_sfn_offset);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->max_cell_reconfig_retry_count, p_src + *p_length_read, "max_cell_reconfig_retry_count");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->rest_rrc_core_no, p_src + *p_length_read, "rest_rrc_core_no");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->no_of_uecc_instances, p_src + *p_length_read, "no_of_uecc_instances");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_provision_req->no_of_uecc_instances < 1) || (p_rrc_oam_provision_req->no_of_uecc_instances > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->no_of_uecc_instances] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_provision_req->no_of_uecc_instances);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_provision_req->no_of_uecc_instances * sizeof(p_rrc_oam_provision_req->core_no[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_provision_req->no_of_uecc_instances; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_provision_req->core_no[loop], (void*)(p_src + *p_length_read), "core_no[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->len_vendor_name, p_src + *p_length_read, "len_vendor_name");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_provision_req->len_vendor_name < 1) || (p_rrc_oam_provision_req->len_vendor_name > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->len_vendor_name] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_oam_provision_req->len_vendor_name);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_provision_req->len_vendor_name * sizeof(p_rrc_oam_provision_req->vendor_name[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_provision_req->len_vendor_name; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_provision_req->vendor_name[loop], (void*)(p_src + *p_length_read), "vendor_name[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_provision_req->pdcp_integrity_threshold_counter, p_src + *p_length_read, "pdcp_integrity_threshold_counter");
    *p_length_read += sizeof(UInt16);
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_MODULE_TIMERS_INFO)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_module_timers_info(
        &p_rrc_oam_provision_req->module_timers_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_CONFIG_OPTION_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->advance_logging_config_option, p_src + *p_length_read, "advance_logging_config_option");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_req->advance_logging_config_option > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_req->advance_logging_config_option] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_oam_provision_req->advance_logging_config_option);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_NETWORK_IP_PORT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_provision_req->network_ip_port) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_provision_req->network_ip_port); loop++)
        {
            OAM_CP_UNPACK_SINT8((void*)&p_rrc_oam_provision_req->network_ip_port[loop], (void*)(p_src + *p_length_read), "network_ip_port[]");
            *p_length_read += sizeof(SInt8);
        }
    }
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_req->ue_context_list_size, p_src + *p_length_read, "ue_context_list_size");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_provision_req->bitmask & RRC_OAM_SHARED_MEM_NAME_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_shared_mem_config(
        &p_rrc_oam_provision_req->shared_mem_logging_config,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_integrity_algorithms
(
    rrc_oam_integrity_algorithms_t *p_rrc_oam_integrity_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_integrity_algorithms, 0, sizeof(rrc_oam_integrity_algorithms_t));

    /* This function parses rrc_oam_integrity_algorithms */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_integrity_algorithms->num_algorithms, p_src + *p_length_read, "num_algorithms");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_integrity_algorithms->num_algorithms < 1) || (p_rrc_oam_integrity_algorithms->num_algorithms > RRC_MAX_INTEGRITY_ALGORITHMS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_integrity_algorithms->num_algorithms] should be in range "
            "1 to RRC_MAX_INTEGRITY_ALGORITHMS. Incorrect value %u received.", p_rrc_oam_integrity_algorithms->num_algorithms);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_integrity_algorithms->num_algorithms * sizeof(p_rrc_oam_integrity_algorithms->algorithms[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_integrity_algorithms->num_algorithms; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_integrity_algorithms->algorithms[loop], (void*)(p_src + *p_length_read), "algorithms[]");
            if (p_rrc_oam_integrity_algorithms->algorithms[loop] > 2)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_rrc_oam_integrity_algorithms->algorithms[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_rrc_oam_integrity_algorithms->algorithms[loop]);
                return OAM_FAILURE;
            }
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ciphering_algorithms
(
    rrc_oam_ciphering_algorithms_t *p_rrc_oam_ciphering_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ciphering_algorithms, 0, sizeof(rrc_oam_ciphering_algorithms_t));

    /* This function parses rrc_oam_ciphering_algorithms */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ciphering_algorithms->num_algorithms, p_src + *p_length_read, "num_algorithms");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ciphering_algorithms->num_algorithms < 1) || (p_rrc_oam_ciphering_algorithms->num_algorithms > RRC_MAX_CIPHERING_ALGORITHMS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ciphering_algorithms->num_algorithms] should be in range "
            "1 to RRC_MAX_CIPHERING_ALGORITHMS. Incorrect value %u received.", p_rrc_oam_ciphering_algorithms->num_algorithms);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_ciphering_algorithms->num_algorithms * sizeof(p_rrc_oam_ciphering_algorithms->algorithms[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ciphering_algorithms->num_algorithms; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_ciphering_algorithms->algorithms[loop], (void*)(p_src + *p_length_read), "algorithms[]");
            if (p_rrc_oam_ciphering_algorithms->algorithms[loop] > 2)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_rrc_oam_ciphering_algorithms->algorithms[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_rrc_oam_ciphering_algorithms->algorithms[loop]);
                return OAM_FAILURE;
            }
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_supported_security_algorithms
(
    rrc_oam_supported_security_algorithms_t *p_rrc_oam_supported_security_algorithms,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_supported_security_algorithms, 0, sizeof(rrc_oam_supported_security_algorithms_t));

    /* This function parses rrc_oam_supported_security_algorithms */

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_integrity_algorithms(
        &p_rrc_oam_supported_security_algorithms->integrity_algorithms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ciphering_algorithms(
        &p_rrc_oam_supported_security_algorithms->ciphering_algorithms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_sector_info
(
    rrc_oam_sector_info_t *p_rrc_oam_sector_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_sector_info, 0, sizeof(rrc_oam_sector_info_t));

    /* This function parses rrc_oam_sector_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_sector_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_sector_info->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_sector_info->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_sector_info->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_sector_info->max_num_supported_ue, p_src + *p_length_read, "max_num_supported_ue");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info->max_num_supported_ue < 1) || (p_rrc_oam_sector_info->max_num_supported_ue > RRC_MAX_NUM_SUPPORTED_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->max_num_supported_ue] should be in range "
            "1 to RRC_MAX_NUM_SUPPORTED_UE. Incorrect value %u received.", p_rrc_oam_sector_info->max_num_supported_ue);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_supported_security_algorithms(
        &p_rrc_oam_sector_info->supported_security_algorithms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_oam_sector_info->bitmask & RRC_OAM_MAX_NUM_SUPPORTED_EMTC_UE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_sector_info->maxNumSupportedEmtcUe, p_src + *p_length_read, "maxNumSupportedEmtcUe");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info->maxNumSupportedEmtcUe < 1) || (p_rrc_oam_sector_info->maxNumSupportedEmtcUe > MAX_NUM_SUPPORTED_EMTC_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info->maxNumSupportedEmtcUe] should be in range "
            "1 to MAX_NUM_SUPPORTED_EMTC_UE. Incorrect value %u received.", p_rrc_oam_sector_info->maxNumSupportedEmtcUe);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_sector_info_list
(
    rrc_oam_sector_info_list_t *p_rrc_oam_sector_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_sector_info_list, 0, sizeof(rrc_oam_sector_info_list_t));

    /* This function parses rrc_oam_sector_info_list */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_sector_info_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_sector_info_list->count < 1) || (p_rrc_oam_sector_info_list->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_sector_info_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_sector_info_list->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_sector_info_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_sector_info(
                &p_rrc_oam_sector_info_list->sector_info[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_timer_info
(
    rrc_oam_timer_info_t *p_rrc_oam_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_timer_info, 0, sizeof(rrc_oam_timer_info_t));

    /* This function parses rrc_oam_timer_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_timer_info->timer_id, p_src + *p_length_read, "timer_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_timer_info->timer_id > 41))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_timer_info->timer_id] should be less than"
            " or equal to 41. Incorrect value %u received.", p_rrc_oam_timer_info->timer_id);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_timer_info->timer_val, p_src + *p_length_read, "timer_val");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_timer_info->timer_val < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_timer_info->timer_val] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_timer_info->timer_val);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_llim_timer_info
(
    rrc_oam_llim_timer_info_t *p_rrc_oam_llim_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_llim_timer_info, 0, sizeof(rrc_oam_llim_timer_info_t));

    /* This function parses rrc_oam_llim_timer_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_llim_timer_info->num_of_timers, p_src + *p_length_read, "num_of_timers");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_llim_timer_info->num_of_timers < 14) || (p_rrc_oam_llim_timer_info->num_of_timers > MAX_LLIM_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_llim_timer_info->num_of_timers] should be in range "
            "14 to MAX_LLIM_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_llim_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_llim_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_timer_info(
                &p_rrc_oam_llim_timer_info->timer_info[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_csc_timer_info
(
    rrc_oam_csc_timer_info_t *p_rrc_oam_csc_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_csc_timer_info, 0, sizeof(rrc_oam_csc_timer_info_t));

    /* This function parses rrc_oam_csc_timer_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_csc_timer_info->num_of_timers, p_src + *p_length_read, "num_of_timers");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_csc_timer_info->num_of_timers < 7) || (p_rrc_oam_csc_timer_info->num_of_timers > MAX_CSC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_csc_timer_info->num_of_timers] should be in range "
            "7 to MAX_CSC_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_csc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_csc_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_timer_info(
                &p_rrc_oam_csc_timer_info->timer_info[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_uecc_timer_info
(
    rrc_oam_uecc_timer_info_t *p_rrc_oam_uecc_timer_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_uecc_timer_info, 0, sizeof(rrc_oam_uecc_timer_info_t));

    /* This function parses rrc_oam_uecc_timer_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_uecc_timer_info->num_of_timers, p_src + *p_length_read, "num_of_timers");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_uecc_timer_info->num_of_timers < 21) || (p_rrc_oam_uecc_timer_info->num_of_timers > MAX_UECC_TIMER_INFO))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_uecc_timer_info->num_of_timers] should be in range "
            "21 to MAX_UECC_TIMER_INFO. Incorrect value %u received.", p_rrc_oam_uecc_timer_info->num_of_timers);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_uecc_timer_info->num_of_timers; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_timer_info(
                &p_rrc_oam_uecc_timer_info->timer_info[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_module_timers_info
(
    rrc_oam_module_timers_info_t *p_rrc_oam_module_timers_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_module_timers_info, 0, sizeof(rrc_oam_module_timers_info_t));

    /* This function parses rrc_oam_module_timers_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer, p_src + *p_length_read, "uecc_intra_cell_ho_guard_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_intra_cell_ho_guard_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer, p_src + *p_length_read, "uecc_ue_reconfig_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_reconfig_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_integrity_threshold_timer_value, p_src + *p_length_read, "uecc_integrity_threshold_timer_value");
    *p_length_read += sizeof(UInt16);
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value, p_src + *p_length_read, "uecc_cco_guard_timer_value");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_cco_guard_timer_value);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_store_ue_context_timer, p_src + *p_length_read, "uecc_store_ue_context_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_store_ue_context_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_store_ue_context_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_store_ue_context_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer, p_src + *p_length_read, "uecc_ue_counter_check_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_counter_check_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_ue_information_timer, p_src + *p_length_read, "uecc_ue_information_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_module_timers_info->uecc_ue_information_timer < 50))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_ue_information_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_ue_information_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->rrc_health_mon_time_int, p_src + *p_length_read, "rrc_health_mon_time_int");
    *p_length_read += sizeof(UInt16);
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_PREP_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_dc_prep_timer, p_src + *p_length_read, "uecc_dc_prep_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_module_timers_info->uecc_dc_prep_timer < 50) || (p_rrc_oam_module_timers_info->uecc_dc_prep_timer > 65535))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_dc_prep_timer] should be in range "
            "50 to 65535. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_dc_prep_timer);
        return OAM_FAILURE;
    }
    }

    if (p_rrc_oam_module_timers_info->bitmask & RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer, p_src + *p_length_read, "uecc_dc_bearer_change_guard_timer");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer < 50) || (p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer > 65535))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer] should be in range "
            "50 to 65535. Incorrect value %u received.", p_rrc_oam_module_timers_info->uecc_dc_bearer_change_guard_timer);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_shared_mem_config
(
    rrc_shared_mem_config_t *p_rrc_shared_mem_config,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_shared_mem_config, 0, sizeof(rrc_shared_mem_config_t));

    /* This function parses rrc_shared_mem_config */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_shared_mem_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_shared_mem_config->SharedMemSize, p_src + *p_length_read, "SharedMemSize");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->SharedMemSize < 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->SharedMemSize] should be greater than"
            " or equal to 3. Incorrect value %u received.", p_rrc_shared_mem_config->SharedMemSize);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_shared_mem_config->MaxLogFileSize, p_src + *p_length_read, "MaxLogFileSize");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->MaxLogFileSize < 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->MaxLogFileSize] should be greater than"
            " or equal to 3. Incorrect value %u received.", p_rrc_shared_mem_config->MaxLogFileSize);
        return OAM_FAILURE;
    }

    if (p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_shared_mem_config->LogFilePath) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_shared_mem_config->LogFilePath); loop++)
        {
            OAM_CP_UNPACK_SINT8((void*)&p_rrc_shared_mem_config->LogFilePath[loop], (void*)(p_src + *p_length_read), "LogFilePath[]");
            *p_length_read += sizeof(SInt8);
        }
    }
    }

    if (p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_shared_mem_config->core_no, p_src + *p_length_read, "core_no");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_shared_mem_config->bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_shared_mem_config->MaxLogFileCount, p_src + *p_length_read, "MaxLogFileCount");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_shared_mem_config->MaxLogFileCount < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_shared_mem_config->MaxLogFileCount] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_shared_mem_config->MaxLogFileCount);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_provision_resp
(
    rrc_oam_provision_resp_t *p_rrc_oam_provision_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_provision_resp, 0, sizeof(rrc_oam_provision_resp_t));

    /* This function parses rrc_oam_provision_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_provision_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_provision_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_provision_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_log_level_resp
(
    rrc_oam_get_log_level_resp_t *p_rrc_oam_get_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_log_level_resp, 0, sizeof(rrc_oam_get_log_level_resp_t));

    /* This function parses rrc_oam_get_log_level_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_log_level_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_log_level_resp->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_log_level_resp->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_log_level_resp->log_on_off);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_log_level_resp->bitmask & RRC_OAM_LOG_LEVEL_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_log_level_resp->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_log_level_resp->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_get_log_level_resp->log_level);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_set_log_level_req
(
    rrc_oam_set_log_level_req_t *p_rrc_oam_set_log_level_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_set_log_level_req, 0, sizeof(rrc_oam_set_log_level_req_t));

    /* This function parses rrc_oam_set_log_level_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_set_log_level_req->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_set_log_level_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_oam_set_log_level_req->log_level);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_set_log_level_resp
(
    rrc_oam_set_log_level_resp_t *p_rrc_oam_set_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_set_log_level_resp, 0, sizeof(rrc_oam_set_log_level_resp_t));

    /* This function parses rrc_oam_set_log_level_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_set_log_level_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_set_log_level_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_set_log_level_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_log_enable_req
(
    rrc_oam_log_enable_req_t *p_rrc_oam_log_enable_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_log_enable_req, 0, sizeof(rrc_oam_log_enable_req_t));

    /* This function parses rrc_oam_log_enable_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_log_enable_req->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_log_enable_req->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_log_enable_req->log_on_off);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_log_enable_resp
(
    rrc_oam_log_enable_resp_t *p_rrc_oam_log_enable_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_log_enable_resp, 0, sizeof(rrc_oam_log_enable_resp_t));

    /* This function parses rrc_oam_log_enable_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_log_enable_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_log_enable_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_log_enable_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_cell_stats_resp
(
    rrc_oam_get_cell_stats_resp_t *p_rrc_oam_get_cell_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_cell_stats_resp, 0, sizeof(rrc_oam_get_cell_stats_resp_t));

    /* This function parses rrc_oam_get_cell_stats_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_cell_stats_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_resp->response > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_oam_get_cell_stats_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_connection_related_stats(
        &p_rrc_oam_get_cell_stats_resp->rrc_connection_related_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_erab_related_stats(
        &p_rrc_oam_get_cell_stats_resp->rrc_erab_related_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_handover_related_stats(
        &p_rrc_oam_get_cell_stats_resp->rrc_handover_related_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_ue_assoc_s1_connection(
        &p_rrc_oam_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_csc_oam_stat_paging(
        &p_rrc_oam_get_cell_stats_resp->rrc_paging_related_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_get_cell_stats_resp->rrc_IratIncMobility_LAI, p_src + *p_length_read, "rrc_IratIncMobility_LAI");
    *p_length_read += sizeof(UInt32);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_connection_establishment
(
    rrc_uecc_oam_stat_connection_establishment_t *p_rrc_uecc_oam_stat_connection_establishment,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_connection_establishment, 0, sizeof(rrc_uecc_oam_stat_connection_establishment_t));

    /* This function parses rrc_uecc_oam_stat_connection_establishment */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_cause[loop], (void*)(p_src + *p_length_read), "rrc_connEstabAtt_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabAtt_sum, p_src + *p_length_read, "rrc_connEstabAtt_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_cause[loop], (void*)(p_src + *p_length_read), "rrc_connEstabSucc_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabSucc_sum, p_src + *p_length_read, "rrc_connEstabSucc_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_cause[loop], (void*)(p_src + *p_length_read), "rrc_connEstabFail_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFail_sum, p_src + *p_length_read, "rrc_connEstabFail_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_connEstabFaileNBCause_Unspecified, p_src + *p_length_read, "rrc_connEstabFaileNBCause_Unspecified");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_Congestion, p_src + *p_length_read, "rrc_ConnEstabFaileNBCause_Congestion");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_establishment->rrc_ConnEstabFaileNBCause_EnergySaving, p_src + *p_length_read, "rrc_ConnEstabFaileNBCause_EnergySaving");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_connection_reestablishment
(
    rrc_uecc_oam_stat_connection_reestablishment_t *p_rrc_uecc_oam_stat_connection_reestablishment,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_connection_reestablishment, 0, sizeof(rrc_uecc_oam_stat_connection_reestablishment_t));

    /* This function parses rrc_uecc_oam_stat_connection_reestablishment */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_cause[loop], (void*)(p_src + *p_length_read), "rrc_connReEstabAtt_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabAtt_sum, p_src + *p_length_read, "rrc_connReEstabAtt_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_cause[loop], (void*)(p_src + *p_length_read), "rrc_connReEstabSucc_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabSucc_sum, p_src + *p_length_read, "rrc_connReEstabSucc_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_cause[loop], (void*)(p_src + *p_length_read), "rrc_connReEstabFail_cause[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_connection_reestablishment->rrc_connReEstabFail_sum, p_src + *p_length_read, "rrc_connReEstabFail_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_connEstabTime
(
    rrc_connEstabTime_t *p_rrc_connEstabTime,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_connEstabTime, 0, sizeof(rrc_connEstabTime_t));

    /* This function parses rrc_connEstabTime */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_connEstabTime->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_connEstabTime->rrc_connEstabTime_Sum, p_src + *p_length_read, "rrc_connEstabTime_Sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_connEstabTime->rrc_connEstabTime_Max, p_src + *p_length_read, "rrc_connEstabTime_Max");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause
(
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t *p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause, 0, sizeof(rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t));

    /* This function parses rrc_uecc_oam_ue_ctxt_rel_req_fail_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ueCtxt_relReq_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_transport[loop], (void*)(p_src + *p_length_read), "ueCtxt_relReq_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_nas[loop], (void*)(p_src + *p_length_read), "ueCtxt_relReq_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_protocol[loop], (void*)(p_src + *p_length_read), "ueCtxt_relReq_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause->ueCtxt_relReq_cause_misc[loop], (void*)(p_src + *p_length_read), "ueCtxt_relReq_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_ue_context_release
(
    rrc_uecc_oam_stat_ue_context_release_t *p_rrc_uecc_oam_stat_ue_context_release,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_ue_context_release, 0, sizeof(rrc_uecc_oam_stat_ue_context_release_t));

    /* This function parses rrc_uecc_oam_stat_ue_context_release */

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_ue_ctxt_rel_req_fail_cause(
        &p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_ue_context_release->ueCtxt_relReq_sum, p_src + *p_length_read, "ueCtxt_relReq_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_ue_context_release->ueContext_RelSuccNbr, p_src + *p_length_read, "ueContext_RelSuccNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_connection_related_stats
(
    rrc_oam_connection_related_stats_t *p_rrc_oam_connection_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_connection_related_stats, 0, sizeof(rrc_oam_connection_related_stats_t));

    /* This function parses rrc_oam_connection_related_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_connection_related_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_connection_establishment(
        &p_rrc_oam_connection_related_stats->rrc_connection_establishment,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_connection_reestablishment(
        &p_rrc_oam_connection_related_stats->rrc_connection_reestablishment,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_connection_related_stats->rrcConnectionNumber, p_src + *p_length_read, "rrcConnectionNumber");
    *p_length_read += sizeof(UInt32);
    }

    if (p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_connEstabTime(
                &p_rrc_oam_connection_related_stats->rrc_connEstabTime_cause[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_rrc_oam_connection_related_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_ue_context_release(
        &p_rrc_oam_connection_related_stats->ue_contxt_rel_stats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_estab_init_fail
(
    rrc_uecc_oam_erab_estab_init_fail_t *p_rrc_uecc_oam_erab_estab_init_fail,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_erab_estab_init_fail, 0, sizeof(rrc_uecc_oam_erab_estab_init_fail_t));

    /* This function parses rrc_uecc_oam_erab_estab_init_fail */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "erab_estabInitFailNbr_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_transport[loop], (void*)(p_src + *p_length_read), "erab_estabInitFailNbr_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_nas[loop], (void*)(p_src + *p_length_read), "erab_estabInitFailNbr_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_protocol[loop], (void*)(p_src + *p_length_read), "erab_estabInitFailNbr_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_init_fail->erab_estabInitFailNbr_cause_misc[loop], (void*)(p_src + *p_length_read), "erab_estabInitFailNbr_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_estab_add_fail
(
    rrc_uecc_oam_erab_estab_add_fail_t *p_rrc_uecc_oam_erab_estab_add_fail,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_erab_estab_add_fail, 0, sizeof(rrc_uecc_oam_erab_estab_add_fail_t));

    /* This function parses rrc_uecc_oam_erab_estab_add_fail */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "erab_estabAddFailNbr_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_transport[loop], (void*)(p_src + *p_length_read), "erab_estabAddFailNbr_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_nas[loop], (void*)(p_src + *p_length_read), "erab_estabAddFailNbr_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_protocol[loop], (void*)(p_src + *p_length_read), "erab_estabAddFailNbr_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_estab_add_fail->erab_estabAddFailNbr_cause_misc[loop], (void*)(p_src + *p_length_read), "erab_estabAddFailNbr_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_erab_estabTime
(
    rrc_erab_estabTime_t *p_rrc_erab_estabTime,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_erab_estabTime, 0, sizeof(rrc_erab_estabTime_t));

    /* This function parses rrc_erab_estabTime */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_erab_estabTime->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_erab_estabTime->erab_estabTime_Sum, p_src + *p_length_read, "erab_estabTime_Sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_erab_estabTime->erab_estabTime_Max, p_src + *p_length_read, "erab_estabTime_Max");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_setup
(
    rrc_uecc_oam_stat_erab_setup_t *p_rrc_uecc_oam_stat_erab_setup,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_erab_setup, 0, sizeof(rrc_uecc_oam_stat_erab_setup_t));

    /* This function parses rrc_uecc_oam_stat_erab_setup */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_estabInitAttNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabInitAttNbr_sum, p_src + *p_length_read, "erab_estabInitAttNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_estabInitSuccNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabInitSuccNbr_sum, p_src + *p_length_read, "erab_estabInitSuccNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_erab_estab_init_fail(
        &p_rrc_uecc_oam_stat_erab_setup->erab_estab_init_fail_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabInitFailNbr_sum, p_src + *p_length_read, "erab_estabInitFailNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_estabAddAttNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabAddAttNbr_sum, p_src + *p_length_read, "erab_estabAddAttNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_estabAddSuccNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabAddSuccNbr_sum, p_src + *p_length_read, "erab_estabAddSuccNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_erab_estab_add_fail(
        &p_rrc_uecc_oam_stat_erab_setup->erab_estab_add_fail_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_setup->erab_estabAddFailNbr_sum, p_src + *p_length_read, "erab_estabAddFailNbr_sum");
    *p_length_read += sizeof(UInt32);

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_erab_estabTime(
                &p_rrc_uecc_oam_stat_erab_setup->erab_estabTime_qci[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_erab_rel_enb_cause
(
    rrc_uecc_oam_erab_rel_enb_cause_t *p_rrc_uecc_oam_erab_rel_enb_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_erab_rel_enb_cause, 0, sizeof(rrc_uecc_oam_erab_rel_enb_cause_t));

    /* This function parses rrc_uecc_oam_erab_rel_enb_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_transport[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_nas[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_protocol[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_erab_rel_enb_cause->erab_relEnbNbr_cause_misc[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_rel_fail_cause
(
    rrc_uecc_oam_rel_fail_cause_t *p_rrc_uecc_oam_rel_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_rel_fail_cause, 0, sizeof(rrc_uecc_oam_rel_fail_cause_t));

    /* This function parses rrc_uecc_oam_rel_fail_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "erab_relFailNbr_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_transport[loop], (void*)(p_src + *p_length_read), "erab_relFailNbr_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_nas[loop], (void*)(p_src + *p_length_read), "erab_relFailNbr_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_protocol[loop], (void*)(p_src + *p_length_read), "erab_relFailNbr_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_rel_fail_cause->erab_relFailNbr_cause_misc[loop], (void*)(p_src + *p_length_read), "erab_relFailNbr_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_release
(
    rrc_uecc_oam_stat_erab_release_t *p_rrc_uecc_oam_stat_erab_release,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_erab_release, 0, sizeof(rrc_uecc_oam_stat_erab_release_t));

    /* This function parses rrc_uecc_oam_stat_erab_release */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_relEnbNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_release->erab_relEnbNbr_sum, p_src + *p_length_read, "erab_relEnbNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_erab_rel_enb_cause(
        &p_rrc_uecc_oam_stat_erab_release->erab_rel_enb_cause_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_relAttNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_release->erab_relAttNbr_sum, p_src + *p_length_read, "erab_relAttNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_relSuccNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_release->erab_relSuccNbr_sum, p_src + *p_length_read, "erab_relSuccNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_rel_fail_cause(
        &p_rrc_uecc_oam_stat_erab_release->rel_fail_cause_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_release->erab_relFailNbr_sum, p_src + *p_length_read, "erab_relFailNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_relActNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_release->erab_relActNbr_sum, p_src + *p_length_read, "erab_relActNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_mod_fail_cause
(
    rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_rrc_uecc_oam_stat_erab_mod_fail_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_erab_mod_fail_cause, 0, sizeof(rrc_uecc_oam_stat_erab_mod_fail_cause_t));

    /* This function parses rrc_uecc_oam_stat_erab_mod_fail_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "erab_modQosFailNbr_cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_transport[loop], (void*)(p_src + *p_length_read), "erab_modQosFailNbr_cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_nas[loop], (void*)(p_src + *p_length_read), "erab_modQosFailNbr_cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_protocol[loop], (void*)(p_src + *p_length_read), "erab_modQosFailNbr_cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_mod_fail_cause->erab_modQosFailNbr_cause_misc[loop], (void*)(p_src + *p_length_read), "erab_modQosFailNbr_cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_modification
(
    rrc_uecc_oam_stat_erab_modification_t *p_rrc_uecc_oam_stat_erab_modification,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_erab_modification, 0, sizeof(rrc_uecc_oam_stat_erab_modification_t));

    /* This function parses rrc_uecc_oam_stat_erab_modification */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_modQosAttNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_modification->erab_modQosAttNbr_sum, p_src + *p_length_read, "erab_modQosAttNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_qci[loop], (void*)(p_src + *p_length_read), "erab_modQosSuccNbr_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_modification->erab_modQosSuccNbr_sum, p_src + *p_length_read, "erab_modQosSuccNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_mod_fail_cause(
        &p_rrc_uecc_oam_stat_erab_modification->erab_mod_fail_cause_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_modification->erab_modQosFailNbr_sum, p_src + *p_length_read, "erab_modQosFailNbr_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_erab_number
(
    rrc_uecc_oam_stat_erab_number_t *p_rrc_uecc_oam_stat_erab_number,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_erab_number, 0, sizeof(rrc_uecc_oam_stat_erab_number_t));

    /* This function parses rrc_uecc_oam_stat_erab_number */

    if (*p_length_read + (SInt32)sizeof(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_uecc_oam_stat_erab_number->erabSimNumber_qci[loop], (void*)(p_src + *p_length_read), "erabSimNumber_qci[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_erab_number->erabSimNumber_sum, p_src + *p_length_read, "erabSimNumber_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_related_stats
(
    rrc_oam_erab_related_stats_t *p_rrc_oam_erab_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_erab_related_stats, 0, sizeof(rrc_oam_erab_related_stats_t));

    /* This function parses rrc_oam_erab_related_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_erab_related_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_setup(
        &p_rrc_oam_erab_related_stats->erab_setup_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_release(
        &p_rrc_oam_erab_related_stats->erab_release_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_modification(
        &p_rrc_oam_erab_related_stats->erab_mod_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_number(
        &p_rrc_oam_erab_related_stats->erab_sim_num_stats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutAtt_cause
(
    rrc_oam_ho_IntraEnbOutAtt_cause_t *p_rrc_oam_ho_IntraEnbOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_IntraEnbOutAtt_cause, 0, sizeof(rrc_oam_ho_IntraEnbOutAtt_cause_t));

    /* This function parses rrc_oam_ho_IntraEnbOutAtt_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutAtt_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutAtt_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutAtt_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutAtt_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutAtt_cause->ho_IntraEnbOutAtt_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutAtt_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutSucc_cause
(
    rrc_oam_ho_IntraEnbOutSucc_cause_t *p_rrc_oam_ho_IntraEnbOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_IntraEnbOutSucc_cause, 0, sizeof(rrc_oam_ho_IntraEnbOutSucc_cause_t));

    /* This function parses rrc_oam_ho_IntraEnbOutSucc_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutSucc_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutSucc_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutSucc_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutSucc_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IntraEnbOutSucc_cause->ho_IntraEnbOutSucc_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_IntraEnbOutSucc_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_intra_enb_ho_stats
(
    rrc_oam_intra_enb_ho_stats_t *p_rrc_oam_intra_enb_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_intra_enb_ho_stats, 0, sizeof(rrc_oam_intra_enb_ho_stats_t));

    /* This function parses rrc_oam_intra_enb_ho_stats */

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutAtt_cause(
        &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutAtt_sum, p_src + *p_length_read, "ho_IntraEnbOutAtt_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_IntraEnbOutSucc_cause(
        &p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_intra_enb_ho_stats->ho_IntraEnbOutSucc_sum, p_src + *p_length_read, "ho_IntraEnbOutSucc_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_InterEnbOutAtt_cause
(
    rrc_oam_ho_InterEnbOutAtt_cause_t *p_rrc_oam_ho_InterEnbOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_InterEnbOutAtt_cause, 0, sizeof(rrc_oam_ho_InterEnbOutAtt_cause_t));

    /* This function parses rrc_oam_ho_InterEnbOutAtt_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutAtt_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutAtt_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutAtt_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutAtt_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutAtt_cause->ho_InterEnbOutAtt_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutAtt_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_InterEnbOutSucc_cause
(
    rrc_oam_ho_InterEnbOutSucc_cause_t *p_rrc_oam_ho_InterEnbOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_InterEnbOutSucc_cause, 0, sizeof(rrc_oam_ho_InterEnbOutSucc_cause_t));

    /* This function parses rrc_oam_ho_InterEnbOutSucc_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutSucc_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutSucc_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutSucc_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutSucc_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_InterEnbOutSucc_cause->ho_InterEnbOutSucc_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_InterEnbOutSucc_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_enb_ho_stats
(
    rrc_oam_inter_enb_ho_stats_t *p_rrc_oam_inter_enb_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_inter_enb_ho_stats, 0, sizeof(rrc_oam_inter_enb_ho_stats_t));

    /* This function parses rrc_oam_inter_enb_ho_stats */

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_InterEnbOutAtt_cause(
        &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutAtt_sum, p_src + *p_length_read, "ho_InterEnbOutAtt_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_InterEnbOutSucc_cause(
        &p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutSucc_sum, p_src + *p_length_read, "ho_InterEnbOutSucc_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_enb_ho_stats->ho_InterEnbOutPrepAtt, p_src + *p_length_read, "ho_InterEnbOutPrepAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_mnc
(
    mnc_t *p_mnc,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_mnc, 0, sizeof(mnc_t));

    /* This function parses mnc */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_mnc->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mnc->count < 2) || (p_mnc->count > MAX_MNC_OCTET_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mnc->count] should be in range "
            "2 to MAX_MNC_OCTET_SIZE. Incorrect value %u received.", p_mnc->count);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_mnc->count * sizeof(p_mnc->mnc[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mnc->count; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_mnc->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            if (p_mnc->mnc[loop] > 9)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_mnc->mnc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_mnc->mnc[loop]);
                return OAM_FAILURE;
            }
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_plmn_identity
(
    plmn_identity_t *p_plmn_identity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_plmn_identity, 0, sizeof(plmn_identity_t));

    /* This function parses plmn_identity */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_plmn_identity->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_plmn_identity->presence_bitmask & 1)
    {

    if (*p_length_read + (SInt32)sizeof(p_plmn_identity->mcc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_plmn_identity->mcc); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_plmn_identity->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            if (p_plmn_identity->mcc[loop] > 9)
            {
                OAM_LOG(OAM,OAM_WARNING, "Parameter p_plmn_identity->mcc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_plmn_identity->mcc[loop]);
                return OAM_FAILURE;
            }
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (OAM_FAILURE == oam_rrc_il_parse_mnc(
        &p_plmn_identity->mnc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_cell_global_id_eutra
(
    cell_global_id_eutra_t *p_cell_global_id_eutra,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_cell_global_id_eutra, 0, sizeof(cell_global_id_eutra_t));

    /* This function parses cell_global_id_eutra */

    if (OAM_FAILURE == oam_rrc_il_parse_plmn_identity(
        &p_cell_global_id_eutra->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_cell_global_id_eutra->cell_identity) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_eutra->cell_identity); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_cell_global_id_eutra->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_OutAttTarget_cause
(
    rrc_oam_ho_OutAttTarget_cause_t *p_rrc_oam_ho_OutAttTarget_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_OutAttTarget_cause, 0, sizeof(rrc_oam_ho_OutAttTarget_cause_t));

    /* This function parses rrc_oam_ho_OutAttTarget_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_OutAttTarget_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_OutAttTarget_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_OutAttTarget_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_OutAttTarget_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutAttTarget_cause->ho_OutAttTarget_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_OutAttTarget_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_OutSuccTarget_cause
(
    rrc_oam_ho_OutSuccTarget_cause_t *p_rrc_oam_ho_OutSuccTarget_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_OutSuccTarget_cause, 0, sizeof(rrc_oam_ho_OutSuccTarget_cause_t));

    /* This function parses rrc_oam_ho_OutSuccTarget_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_OutSuccTarget_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_OutSuccTarget_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_OutSuccTarget_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_OutSuccTarget_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_OutSuccTarget_cause->ho_OutSuccTarget_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_OutSuccTarget_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats
(
    rrc_oam_meas_on_neigh_cell_ho_stats_t *p_rrc_oam_meas_on_neigh_cell_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_meas_on_neigh_cell_ho_stats, 0, sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_t));

    /* This function parses rrc_oam_meas_on_neigh_cell_ho_stats */

    if (OAM_FAILURE == oam_rrc_il_parse_cell_global_id_eutra(
        &p_rrc_oam_meas_on_neigh_cell_ho_stats->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_OutAttTarget_cause(
        &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutAttTarget_sum, p_src + *p_length_read, "ho_OutAttTarget_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_OutSuccTarget_cause(
        &p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_meas_on_neigh_cell_ho_stats->ho_OutSuccTarget_sum, p_src + *p_length_read, "ho_OutSuccTarget_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats_list
(
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t *p_rrc_oam_meas_on_neigh_cell_ho_stats_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_meas_on_neigh_cell_ho_stats_list, 0, sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));

    /* This function parses rrc_oam_meas_on_neigh_cell_ho_stats_list */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %u received.", p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_meas_on_neigh_cell_ho_stats_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats(
                &p_rrc_oam_meas_on_neigh_cell_ho_stats_list->meas_on_neigh_cell_ho_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_intra_freq_ho_stats
(
    rrc_oam_intra_freq_ho_stats_t *p_rrc_oam_intra_freq_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_intra_freq_ho_stats, 0, sizeof(rrc_oam_intra_freq_ho_stats_t));

    /* This function parses rrc_oam_intra_freq_ho_stats */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutAtt, p_src + *p_length_read, "ho_IntraFreqOutAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_intra_freq_ho_stats->ho_IntraFreqOutSucc, p_src + *p_length_read, "ho_IntraFreqOutSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_freq_ho_stats
(
    rrc_oam_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_inter_freq_ho_stats, 0, sizeof(rrc_oam_inter_freq_ho_stats_t));

    /* This function parses rrc_oam_inter_freq_ho_stats */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutAtt, p_src + *p_length_read, "ho_InterFreqMeasGapOutAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_freq_ho_stats->ho_InterFreqMeasGapOutSucc, p_src + *p_length_read, "ho_InterFreqMeasGapOutSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutAtt, p_src + *p_length_read, "ho_InterFreqNoMeasGapOutAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutSucc, p_src + *p_length_read, "ho_InterFreqNoMeasGapOutSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_drx_non_drx_ho_stats
(
    rrc_oam_drx_non_drx_ho_stats_t *p_rrc_oam_drx_non_drx_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_drx_non_drx_ho_stats, 0, sizeof(rrc_oam_drx_non_drx_ho_stats_t));

    /* This function parses rrc_oam_drx_non_drx_ho_stats */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutAtt, p_src + *p_length_read, "ho_DrxOutAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_drx_non_drx_ho_stats->ho_DrxOutSucc, p_src + *p_length_read, "ho_DrxOutSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutAtt, p_src + *p_length_read, "ho_NoDrxOutAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_drx_non_drx_ho_stats->ho_NoDrxOutSucc, p_src + *p_length_read, "ho_NoDrxOutSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_utran_geran_cgi
(
    rrc_utran_geran_cgi_t *p_rrc_utran_geran_cgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_utran_geran_cgi, 0, sizeof(rrc_utran_geran_cgi_t));

    /* This function parses rrc_utran_geran_cgi */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_utran_geran_cgi->rat_type, p_src + *p_length_read, "rat_type");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_rrc_il_parse_plmn_identity(
        &p_rrc_utran_geran_cgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_utran_geran_cgi->cell_identity, p_src + *p_length_read, "cell_identity");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_cdma2000_1xrtt_cell_identifier
(
    cdma2000_1xrtt_cell_identifier_t *p_cdma2000_1xrtt_cell_identifier,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_cdma2000_1xrtt_cell_identifier, 0, sizeof(cdma2000_1xrtt_cell_identifier_t));

    /* This function parses cdma2000_1xrtt_cell_identifier */

    if (*p_length_read + (SInt32)sizeof(p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_cdma2000_1xrtt_cell_identifier->cdma2000_1xrtt_cell_id[loop], (void*)(p_src + *p_length_read), "cdma2000_1xrtt_cell_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_cdma2000_hrpd_sector_identifier
(
    cdma2000_hrpd_sector_identifier_t *p_cdma2000_hrpd_sector_identifier,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_cdma2000_hrpd_sector_identifier, 0, sizeof(cdma2000_hrpd_sector_identifier_t));

    /* This function parses cdma2000_hrpd_sector_identifier */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_cdma2000_hrpd_sector_identifier->sector_id_length, p_src + *p_length_read, "sector_id_length");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cdma2000_hrpd_sector_identifier->sector_id_length < 16) || (p_cdma2000_hrpd_sector_identifier->sector_id_length > MAX_SECTOR_ID_LENGTH))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_cdma2000_hrpd_sector_identifier->sector_id_length] should be in range "
            "16 to MAX_SECTOR_ID_LENGTH. Incorrect value %u received.", p_cdma2000_hrpd_sector_identifier->sector_id_length);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(p_cdma2000_hrpd_sector_identifier->sector_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cdma2000_hrpd_sector_identifier->sector_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_cdma2000_hrpd_sector_identifier->sector_id[loop], (void*)(p_src + *p_length_read), "sector_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_inter_rat_global_cgi
(
    rrc_inter_rat_global_cgi_t *p_rrc_inter_rat_global_cgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_inter_rat_global_cgi, 0, sizeof(rrc_inter_rat_global_cgi_t));

    /* This function parses rrc_inter_rat_global_cgi */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_inter_rat_global_cgi->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_utran_geran_cgi(
        &p_rrc_inter_rat_global_cgi->cgi_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_cdma2000_1xrtt_cell_identifier(
        &p_rrc_inter_rat_global_cgi->cell_id_1xrtt,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_inter_rat_global_cgi->bitmask & RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_cdma2000_hrpd_sector_identifier(
        &p_rrc_inter_rat_global_cgi->sector_id_hrpd,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IratOutAtt_cause
(
    rrc_oam_ho_IratOutAtt_cause_t *p_rrc_oam_ho_IratOutAtt_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_IratOutAtt_cause, 0, sizeof(rrc_oam_ho_IratOutAtt_cause_t));

    /* This function parses rrc_oam_ho_IratOutAtt_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_IratOutAtt_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_IratOutAtt_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_IratOutAtt_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_IratOutAtt_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutAtt_cause->ho_IratOutAtt_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_IratOutAtt_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ho_IratOutSucc_cause
(
    rrc_oam_ho_IratOutSucc_cause_t *p_rrc_oam_ho_IratOutSucc_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ho_IratOutSucc_cause, 0, sizeof(rrc_oam_ho_IratOutSucc_cause_t));

    /* This function parses rrc_oam_ho_IratOutSucc_cause */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_radioNetwork[loop], (void*)(p_src + *p_length_read), "ho_IratOutSucc_Cause_radioNetwork[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_transport[loop], (void*)(p_src + *p_length_read), "ho_IratOutSucc_Cause_transport[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_nas[loop], (void*)(p_src + *p_length_read), "ho_IratOutSucc_Cause_nas[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_protocol[loop], (void*)(p_src + *p_length_read), "ho_IratOutSucc_Cause_protocol[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc); loop++)
        {
            OAM_CP_UNPACK_UINT32((void*)&p_rrc_oam_ho_IratOutSucc_cause->ho_IratOutSucc_Cause_misc[loop], (void*)(p_src + *p_length_read), "ho_IratOutSucc_Cause_misc[]");
            *p_length_read += sizeof(UInt32);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats
(
    rrc_oam_inter_rat_ho_stats_t *p_rrc_oam_inter_rat_ho_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_inter_rat_ho_stats, 0, sizeof(rrc_oam_inter_rat_ho_stats_t));

    /* This function parses rrc_oam_inter_rat_ho_stats */

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_inter_rat_global_cgi(
        &p_rrc_oam_inter_rat_ho_stats->target_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_IratOutAtt_cause(
        &p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_rat_ho_stats->ho_IratOutAtt_sum, p_src + *p_length_read, "ho_IratOutAtt_sum");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ho_IratOutSucc_cause(
        &p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_inter_rat_ho_stats->ho_IratOutSucc_sum, p_src + *p_length_read, "ho_IratOutSucc_sum");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats_list
(
    rrc_oam_inter_rat_ho_stats_list_t *p_rrc_oam_inter_rat_ho_stats_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_inter_rat_ho_stats_list, 0, sizeof(rrc_oam_inter_rat_ho_stats_list_t));

    /* This function parses rrc_oam_inter_rat_ho_stats_list */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_inter_rat_ho_stats_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_inter_rat_ho_stats_list->count > RRC_MAX_CONNECTION))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_inter_rat_ho_stats_list->count] should be less than"
            " or equal to RRC_MAX_CONNECTION. Incorrect value %u received.", p_rrc_oam_inter_rat_ho_stats_list->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_inter_rat_ho_stats_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats(
                &p_rrc_oam_inter_rat_ho_stats_list->inter_rat_ho_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_related_stats
(
    rrc_oam_handover_related_stats_t *p_rrc_oam_handover_related_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_handover_related_stats, 0, sizeof(rrc_oam_handover_related_stats_t));

    /* This function parses rrc_oam_handover_related_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_handover_related_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_ENB_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_intra_enb_ho_stats(
        &p_rrc_oam_handover_related_stats->intra_enb_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_ENB_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_enb_ho_stats(
        &p_rrc_oam_handover_related_stats->inter_enb_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats_list(
        &p_rrc_oam_handover_related_stats->meas_on_neigh_cell_ho_stats_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_intra_freq_ho_stats(
        &p_rrc_oam_handover_related_stats->intra_freq_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_FREQ_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_freq_ho_stats(
        &p_rrc_oam_handover_related_stats->inter_freq_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_drx_non_drx_ho_stats(
        &p_rrc_oam_handover_related_stats->drx_non_drx_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_INTER_RAT_HO_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats_list(
        &p_rrc_oam_handover_related_stats->inter_rat_ho_stats_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_handover_related_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_handover_related_stats->ho_failure_num, p_src + *p_length_read, "ho_failure_num");
    *p_length_read += sizeof(UInt32);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_uecc_oam_stat_ue_assoc_s1_connection
(
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t *p_rrc_uecc_oam_stat_ue_assoc_s1_connection,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_uecc_oam_stat_ue_assoc_s1_connection, 0, sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

    /* This function parses rrc_uecc_oam_stat_ue_assoc_s1_connection */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabAtt, p_src + *p_length_read, "s1sig_connEstabAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_uecc_oam_stat_ue_assoc_s1_connection->s1sig_connEstabSucc, p_src + *p_length_read, "s1sig_connEstabSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_csc_oam_stat_paging
(
    rrc_csc_oam_stat_paging_t *p_rrc_csc_oam_stat_paging,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_csc_oam_stat_paging, 0, sizeof(rrc_csc_oam_stat_paging_t));

    /* This function parses rrc_csc_oam_stat_paging */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_csc_oam_stat_paging->pag_discardedNbr, p_src + *p_length_read, "pag_discardedNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_csc_oam_stat_paging->pag_succNbr, p_src + *p_length_read, "pag_succNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_csc_oam_stat_paging->pag_receivedNbr, p_src + *p_length_read, "pag_receivedNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_cell_stats_req
(
    rrc_oam_get_cell_stats_req_t *p_rrc_oam_get_cell_stats_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_cell_stats_req, 0, sizeof(rrc_oam_get_cell_stats_req_t));

    /* This function parses rrc_oam_get_cell_stats_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_cell_stats_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_stats_req->cell_index);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_connection_stats(
        &p_rrc_oam_get_cell_stats_req->rrc_connection_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_erab_stats(
        &p_rrc_oam_get_cell_stats_req->rrc_erab_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_handover_stats(
        &p_rrc_oam_get_cell_stats_req->rrc_handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_req->rrc_ue_assoc_s1_conn_stats, p_src + *p_length_read, "rrc_ue_assoc_s1_conn_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_req->rrc_paging_stats, p_src + *p_length_read, "rrc_paging_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_RESET_CELL_STATISTICS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_req->reset_stats, p_src + *p_length_read, "reset_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_get_cell_stats_req->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_stats_req->rrc_IratIncMobility_LAI, p_src + *p_length_read, "rrc_IratIncMobility_LAI");
    *p_length_read += sizeof(UInt8);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_connection_stats
(
    rrc_oam_connection_stats_t *p_rrc_oam_connection_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_connection_stats, 0, sizeof(rrc_oam_connection_stats_t));

    /* This function parses rrc_oam_connection_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_connection_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_connection_stats->rrc_connection_establishment, p_src + *p_length_read, "rrc_connection_establishment");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_connection_stats->rrc_connection_reestablishment, p_src + *p_length_read, "rrc_connection_reestablishment");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_connection_stats->rrcConnectionNumber, p_src + *p_length_read, "rrcConnectionNumber");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_connection_stats->rrcConnectionSetupTime, p_src + *p_length_read, "rrcConnectionSetupTime");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_connection_stats->bitmask & RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_connection_stats->ue_contxt_rel_stats, p_src + *p_length_read, "ue_contxt_rel_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_stats
(
    rrc_oam_erab_stats_t *p_rrc_oam_erab_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_erab_stats, 0, sizeof(rrc_oam_erab_stats_t));

    /* This function parses rrc_oam_erab_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_erab_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_erab_stats->erab_setup_stats, p_src + *p_length_read, "erab_setup_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_erab_stats->erab_release_stats, p_src + *p_length_read, "erab_release_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_erab_stats->erab_mod_stats, p_src + *p_length_read, "erab_mod_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_erab_stats->bitmask & RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_erab_stats->erab_num_stats, p_src + *p_length_read, "erab_num_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_stats
(
    rrc_oam_handover_stats_t *p_rrc_oam_handover_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_handover_stats, 0, sizeof(rrc_oam_handover_stats_t));

    /* This function parses rrc_oam_handover_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_handover_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->intra_enb_ho_related_stats, p_src + *p_length_read, "intra_enb_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->inter_enb_ho_related_stats, p_src + *p_length_read, "inter_enb_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->meas_on_neigh_cell_ho_related_stats, p_src + *p_length_read, "meas_on_neigh_cell_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->intra_freq_ho_related_stats, p_src + *p_length_read, "intra_freq_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->inter_freq_ho_related_stats, p_src + *p_length_read, "inter_freq_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->drx_non_drx_ho_related_stats, p_src + *p_length_read, "drx_non_drx_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->inter_rat_ho_related_stats, p_src + *p_length_read, "inter_rat_ho_related_stats");
    *p_length_read += sizeof(UInt8);
    }

    if (p_rrc_oam_handover_stats->bitmask & RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_handover_stats->ho_failure_num, p_src + *p_length_read, "ho_failure_num");
    *p_length_read += sizeof(UInt8);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_config_stats_req
(
    rrc_oam_config_stats_req_t *p_rrc_oam_config_stats_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_config_stats_req, 0, sizeof(rrc_oam_config_stats_req_t));

    /* This function parses rrc_oam_config_stats_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_config_stats_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_config_stats_req->rrc_periodic_reporting, p_src + *p_length_read, "rrc_periodic_reporting");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_config_stats_req->rrc_periodic_reporting > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_config_stats_req->rrc_periodic_reporting] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_config_stats_req->rrc_periodic_reporting);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_rrc_connection_stats_info(
        &p_rrc_oam_config_stats_req->rrc_connection_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_erab_stats_info(
        &p_rrc_oam_config_stats_req->rrc_erab_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_handover_stats_info(
        &p_rrc_oam_config_stats_req->rrc_handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_sampling_period_info(
        &p_rrc_oam_config_stats_req->rrc_ue_assoc_s1_conn_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_PAGING_RELATED_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_sampling_period_info(
        &p_rrc_oam_config_stats_req->rrc_paging_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_oam_config_stats_req->bitmask & RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_sampling_period_info(
        &p_rrc_oam_config_stats_req->rrc_IratIncMobility_LAI_stats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_rrc_connection_stats_info
(
    rrc_oam_rrc_connection_stats_info_t *p_rrc_oam_rrc_connection_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_rrc_connection_stats_info, 0, sizeof(rrc_oam_rrc_connection_stats_info_t));

    /* This function parses rrc_oam_rrc_connection_stats_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_rrc_connection_stats_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_rrc_connection_stats_info->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_rrc_connection_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_rrc_connection_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_rrc_connection_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_erab_stats_info
(
    rrc_oam_erab_stats_info_t *p_rrc_oam_erab_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_erab_stats_info, 0, sizeof(rrc_oam_erab_stats_info_t));

    /* This function parses rrc_oam_erab_stats_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_erab_stats_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_erab_stats_info->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_erab_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_erab_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_erab_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_handover_stats_info
(
    rrc_oam_handover_stats_info_t *p_rrc_oam_handover_stats_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_handover_stats_info, 0, sizeof(rrc_oam_handover_stats_info_t));

    /* This function parses rrc_oam_handover_stats_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_handover_stats_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_handover_stats_info->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_oam_handover_stats_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_handover_stats_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_handover_stats_info->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_sampling_period_info
(
    rrc_sampling_period_info_t *p_rrc_sampling_period_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_sampling_period_info, 0, sizeof(rrc_sampling_period_info_t));

    /* This function parses rrc_sampling_period_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_sampling_period_info->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_sampling_period_info->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_sampling_period_info->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_sampling_period_info->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_config_stats_resp
(
    rrc_oam_config_stats_resp_t *p_rrc_oam_config_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_config_stats_resp, 0, sizeof(rrc_oam_config_stats_resp_t));

    /* This function parses rrc_oam_config_stats_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_config_stats_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_config_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_config_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_config_stats_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_rrc_connection_stats_ind
(
    rrc_oam_rrc_connection_stats_ind_t *p_rrc_oam_rrc_connection_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_rrc_connection_stats_ind, 0, sizeof(rrc_oam_rrc_connection_stats_ind_t));

    /* This function parses rrc_oam_rrc_connection_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_rrc_connection_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_rrc_connection_stats_ind->count < 1) || (p_rrc_oam_rrc_connection_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_rrc_connection_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_rrc_connection_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_rrc_connection_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_connection_stats(
                &p_rrc_oam_rrc_connection_stats_ind->rrc_connection[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_connection_stats
(
    rrc_connection_stats_t *p_rrc_connection_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_connection_stats, 0, sizeof(rrc_connection_stats_t));

    /* This function parses rrc_connection_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_connection_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_connection_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_connection_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_connection_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_connection_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_connection_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_connection_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_connection_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_connection_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_connection_establishment(
        &p_rrc_connection_stats->rrc_connection_establishment,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_connection_reestablishment(
        &p_rrc_connection_stats->rrc_connection_reestablishment,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_connection_stats->rrcConnectionNumber, p_src + *p_length_read, "rrcConnectionNumber");
    *p_length_read += sizeof(UInt32);
    }

    if (p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT)
    {

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_connection_stats->rrc_connEstabTime_cause); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_connEstabTime(
                &p_rrc_connection_stats->rrc_connEstabTime_cause[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_rrc_connection_stats->bitmask & RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_ue_context_release(
        &p_rrc_connection_stats->ue_contxt_rel_stats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_handover_stats_ind
(
    rrc_oam_handover_stats_ind_t *p_rrc_oam_handover_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_handover_stats_ind, 0, sizeof(rrc_oam_handover_stats_ind_t));

    /* This function parses rrc_oam_handover_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_handover_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_handover_stats_ind->count < 1) || (p_rrc_oam_handover_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_handover_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_handover_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_handover_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_handover_stats(
                &p_rrc_oam_handover_stats_ind->handover_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_handover_stats
(
    rrc_handover_stats_t *p_rrc_handover_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_handover_stats, 0, sizeof(rrc_handover_stats_t));

    /* This function parses rrc_handover_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_handover_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_handover_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_handover_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_handover_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_handover_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_handover_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_handover_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_handover_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_handover_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_intra_enb_ho_stats(
        &p_rrc_handover_stats->intra_enb_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_enb_ho_stats(
        &p_rrc_handover_stats->inter_enb_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_meas_on_neigh_cell_ho_stats_list(
        &p_rrc_handover_stats->meas_on_neigh_cell_ho_stats_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_intra_freq_ho_stats(
        &p_rrc_handover_stats->intra_freq_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_freq_ho_stats(
        &p_rrc_handover_stats->inter_freq_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_drx_non_drx_ho_stats(
        &p_rrc_handover_stats->drx_non_drx_ho_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_inter_rat_ho_stats_list(
        &p_rrc_handover_stats->inter_rat_ho_stats_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_handover_stats->bitmask & RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_handover_stats->ho_failure_num, p_src + *p_length_read, "ho_failure_num");
    *p_length_read += sizeof(UInt32);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_erab_stats_ind
(
    rrc_oam_erab_stats_ind_t *p_rrc_oam_erab_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_erab_stats_ind, 0, sizeof(rrc_oam_erab_stats_ind_t));

    /* This function parses rrc_oam_erab_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_erab_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_erab_stats_ind->count < 1) || (p_rrc_oam_erab_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_erab_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_erab_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_erab_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_erab_stats(
                &p_rrc_oam_erab_stats_ind->erab_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_erab_stats
(
    rrc_erab_stats_t *p_rrc_erab_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_erab_stats, 0, sizeof(rrc_erab_stats_t));

    /* This function parses rrc_erab_stats */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_erab_stats->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_erab_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_erab_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_erab_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_erab_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_erab_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_erab_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_erab_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_erab_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_setup(
        &p_rrc_erab_stats->erab_setup_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_release(
        &p_rrc_erab_stats->erab_release_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_modification(
        &p_rrc_erab_stats->erab_mod_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_erab_stats->bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_uecc_oam_stat_erab_number(
        &p_rrc_erab_stats->erab_sim_num_stats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_ue_assoc_s1_conn_stats_ind
(
    rrc_oam_ue_assoc_s1_conn_stats_ind_t *p_rrc_oam_ue_assoc_s1_conn_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_ue_assoc_s1_conn_stats_ind, 0, sizeof(rrc_oam_ue_assoc_s1_conn_stats_ind_t));

    /* This function parses rrc_oam_ue_assoc_s1_conn_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ue_assoc_s1_conn_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ue_assoc_s1_conn_stats_ind->count < 1) || (p_rrc_oam_ue_assoc_s1_conn_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_assoc_s1_conn_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_ue_assoc_s1_conn_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_assoc_s1_conn_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_ue_assoc_s1_conn_stats(
                &p_rrc_oam_ue_assoc_s1_conn_stats_ind->ue_assoc_s1_conn_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_ue_assoc_s1_conn_stats
(
    rrc_ue_assoc_s1_conn_stats_t *p_rrc_ue_assoc_s1_conn_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_ue_assoc_s1_conn_stats, 0, sizeof(rrc_ue_assoc_s1_conn_stats_t));

    /* This function parses rrc_ue_assoc_s1_conn_stats */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_ue_assoc_s1_conn_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_assoc_s1_conn_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_ue_assoc_s1_conn_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_ue_assoc_s1_conn_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_ue_assoc_s1_conn_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_ue_assoc_s1_conn_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_ue_assoc_s1_conn_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_ue_assoc_s1_conn_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabAtt, p_src + *p_length_read, "s1sig_connEstabAtt");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_ue_assoc_s1_conn_stats->s1sig_connEstabSucc, p_src + *p_length_read, "s1sig_connEstabSucc");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_paging_stats_ind
(
    rrc_oam_paging_stats_ind_t *p_rrc_oam_paging_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_paging_stats_ind, 0, sizeof(rrc_oam_paging_stats_ind_t));

    /* This function parses rrc_oam_paging_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_paging_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_paging_stats_ind->count < 1) || (p_rrc_oam_paging_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_paging_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_paging_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_paging_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_paging_stats(
                &p_rrc_oam_paging_stats_ind->rrc_paging_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_paging_stats
(
    rrc_paging_stats_t *p_rrc_paging_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_paging_stats, 0, sizeof(rrc_paging_stats_t));

    /* This function parses rrc_paging_stats */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_paging_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_paging_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_paging_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_paging_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_paging_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_paging_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_paging_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_paging_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_paging_stats->pag_discardedNbr, p_src + *p_length_read, "pag_discardedNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_paging_stats->pag_succNbr, p_src + *p_length_read, "pag_succNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_paging_stats->pag_receivedNbr, p_src + *p_length_read, "pag_receivedNbr");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_irat_mobility_stats_ind
(
    rrc_oam_irat_mobility_stats_ind_t *p_rrc_oam_irat_mobility_stats_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_irat_mobility_stats_ind, 0, sizeof(rrc_oam_irat_mobility_stats_ind_t));

    /* This function parses rrc_oam_irat_mobility_stats_ind */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_irat_mobility_stats_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_irat_mobility_stats_ind->count < 1) || (p_rrc_oam_irat_mobility_stats_ind->count > MAX_NUM_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_irat_mobility_stats_ind->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_oam_irat_mobility_stats_ind->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_irat_mobility_stats_ind->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_irat_mobility_stats(
                &p_rrc_oam_irat_mobility_stats_ind->rrc_irat_mobility_stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_irat_mobility_stats
(
    rrc_irat_mobility_stats_t *p_rrc_irat_mobility_stats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_irat_mobility_stats, 0, sizeof(rrc_irat_mobility_stats_t));

    /* This function parses rrc_irat_mobility_stats */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_irat_mobility_stats->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_irat_mobility_stats->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_irat_mobility_stats->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_irat_mobility_stats->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_irat_mobility_stats->sampling_period, p_src + *p_length_read, "sampling_period");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_irat_mobility_stats->sampling_period < 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_irat_mobility_stats->sampling_period] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_irat_mobility_stats->sampling_period);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_irat_mobility_stats->rrc_IratIncMobility_LAI, p_src + *p_length_read, "rrc_IratIncMobility_LAI");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_cell_status_req
(
    rrc_oam_get_cell_status_req_t *p_rrc_oam_get_cell_status_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_cell_status_req, 0, sizeof(rrc_oam_get_cell_status_req_t));

    /* This function parses rrc_oam_get_cell_status_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_status_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_status_req->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_cell_status_resp
(
    rrc_oam_get_cell_status_resp_t *p_rrc_oam_get_cell_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_cell_status_resp, 0, sizeof(rrc_oam_get_cell_status_resp_t));

    /* This function parses rrc_oam_get_cell_status_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_cell_status_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_status_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_cell_status_resp->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_cell_status_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_cell_status_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_cell_status_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_cell_status_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_cell_status_resp->bitmask & RRC_OAM_CELL_STATUS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_cell_status(
        &p_rrc_oam_get_cell_status_resp->cell_status,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_cell_status
(
    rrc_oam_cell_status_t *p_rrc_oam_cell_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_cell_status, 0, sizeof(rrc_oam_cell_status_t));

    /* This function parses rrc_oam_cell_status */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_cell_status->num_ue, p_src + *p_length_read, "num_ue");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_cell_status->num_srb1, p_src + *p_length_read, "num_srb1");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_cell_status->num_srb2, p_src + *p_length_read, "num_srb2");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_cell_status->num_drb, p_src + *p_length_read, "num_drb");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_ue_status_req
(
    rrc_oam_get_ue_status_req_t *p_rrc_oam_get_ue_status_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_ue_status_req, 0, sizeof(rrc_oam_get_ue_status_req_t));

    /* This function parses rrc_oam_get_ue_status_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_ue_status_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ue_status_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ue_status_req->cell_index);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_ue_status_req->bitmask & RRC_OAM_UE_INDEX_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_ue_status_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(UInt16);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_ue_status_resp
(
    rrc_oam_get_ue_status_resp_t *p_rrc_oam_get_ue_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_ue_status_resp, 0, sizeof(rrc_oam_get_ue_status_resp_t));

    /* This function parses rrc_oam_get_ue_status_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_ue_status_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ue_status_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ue_status_resp->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ue_status_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ue_status_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ue_status_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_ue_status_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_get_ue_status_resp->bitmask & RRC_OAM_UE_STATUS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ue_status_list(
        &p_rrc_oam_get_ue_status_resp->ue_status_list,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ue_status
(
    rrc_oam_ue_status_t *p_rrc_oam_ue_status,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_ue_status, 0, sizeof(rrc_oam_ue_status_t));

    /* This function parses rrc_oam_ue_status */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ue_status->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ue_status->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ue_status->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ue_status->ue_state, p_src + *p_length_read, "ue_state");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ue_status->ue_category, p_src + *p_length_read, "ue_category");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ue_status->num_srb, p_src + *p_length_read, "num_srb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_ue_status->num_srb < 1) || (p_rrc_oam_ue_status->num_srb > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->num_srb] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_oam_ue_status->num_srb);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_ue_status->num_drb, p_src + *p_length_read, "num_drb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status->num_drb > 11))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->num_drb] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_oam_ue_status->num_drb);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_ue_status->enb_ue_s1ap_id, p_src + *p_length_read, "enb_ue_s1ap_id");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status->enb_ue_s1ap_id > 16777215))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status->enb_ue_s1ap_id] should be less than"
            " or equal to 16777215. Incorrect value %u received.", p_rrc_oam_ue_status->enb_ue_s1ap_id);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_ue_status->bitmask & RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_oam_ue_status->mme_ue_s1ap_id, p_src + *p_length_read, "mme_ue_s1ap_id");
    *p_length_read += sizeof(UInt32);
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_ue_status_list
(
    rrc_oam_ue_status_list_t *p_rrc_oam_ue_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_ue_status_list, 0, sizeof(rrc_oam_ue_status_list_t));

    /* This function parses rrc_oam_ue_status_list */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_ue_status_list->num_ue_status, p_src + *p_length_read, "num_ue_status");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_ue_status_list->num_ue_status > RRC_MAX_NUM_UE_STATUS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_ue_status_list->num_ue_status] should be less than"
            " or equal to RRC_MAX_NUM_UE_STATUS. Incorrect value %u received.", p_rrc_oam_ue_status_list->num_ue_status);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_ue_status_list->num_ue_status; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ue_status(
                &p_rrc_oam_ue_status_list->ue_status[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_ps_stats_req
(
    rrc_oam_get_ps_stats_req_t *p_rrc_oam_get_ps_stats_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_ps_stats_req, 0, sizeof(rrc_oam_get_ps_stats_req_t));

    /* This function parses rrc_oam_get_ps_stats_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ps_stats_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ps_stats_req->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_ps_stats_resp
(
    rrc_oam_get_ps_stats_resp_t *p_rrc_oam_get_ps_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_ps_stats_resp, 0, sizeof(rrc_oam_get_ps_stats_resp_t));

    /* This function parses rrc_oam_get_ps_stats_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ps_stats_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_get_ps_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_get_ps_stats_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_get_ps_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_get_ps_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_get_ps_stats_resp->response);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_oam_rrc_ps_stats_resp(
        &p_rrc_oam_get_ps_stats_resp->ps_stats_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_ueRrcInfoStatus
(
    ueRrcInfoStatus_t *p_ueRrcInfoStatus,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_ueRrcInfoStatus, 0, sizeof(ueRrcInfoStatus_t));

    /* This function parses ueRrcInfoStatus */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_ueRrcInfoStatus->cRNTI, p_src + *p_length_read, "cRNTI");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_ueRrcInfoStatus->ueIndex, p_src + *p_length_read, "ueIndex");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_ueRrcInfoStatus->enbS1APId, p_src + *p_length_read, "enbS1APId");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_ueRrcInfoStatus->enbS1APId > 16777215))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ueRrcInfoStatus->enbS1APId] should be less than"
            " or equal to 16777215. Incorrect value %u received.", p_ueRrcInfoStatus->enbS1APId);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_ueRrcInfoStatus->mmeS1APId, p_src + *p_length_read, "mmeS1APId");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_ueRrcInfoStatus->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_oam_rrc_ps_stats_resp
(
    oam_rrc_ps_stats_resp_t *p_oam_rrc_ps_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_oam_rrc_ps_stats_resp, 0, sizeof(oam_rrc_ps_stats_resp_t));

    /* This function parses oam_rrc_ps_stats_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_oam_rrc_ps_stats_resp->noOfUe, p_src + *p_length_read, "noOfUe");
    *p_length_read += sizeof(UInt16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_oam_rrc_ps_stats_resp->noOfUe; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_ueRrcInfoStatus(
                &p_oam_rrc_ps_stats_resp->ueInfoStatus[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_reset_cell_stats_req
(
    rrc_oam_reset_cell_stats_req_t *p_rrc_oam_reset_cell_stats_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_reset_cell_stats_req, 0, sizeof(rrc_oam_reset_cell_stats_req_t));

    /* This function parses rrc_oam_reset_cell_stats_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_reset_cell_stats_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_req->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_req->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_req->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_reset_cell_stats_resp
(
    rrc_oam_reset_cell_stats_resp_t *p_rrc_oam_reset_cell_stats_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_reset_cell_stats_resp, 0, sizeof(rrc_oam_reset_cell_stats_resp_t));

    /* This function parses rrc_oam_reset_cell_stats_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_reset_cell_stats_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_resp->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_resp->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_reset_cell_stats_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_reset_cell_stats_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_reset_cell_stats_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_reset_cell_stats_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_cleanup_resp
(
    rrc_oam_cleanup_resp_t *p_rrc_oam_cleanup_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_cleanup_resp, 0, sizeof(rrc_oam_cleanup_resp_t));

    /* This function parses rrc_oam_cleanup_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cleanup_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cleanup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cleanup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_cleanup_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_init_ind
(
    rrc_oam_init_ind_t *p_rrc_oam_init_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_init_ind, 0, sizeof(rrc_oam_init_ind_t));

    /* This function parses rrc_oam_init_ind */

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_cell_traffic_trace_start
(
    rrc_oam_cell_traffic_trace_start_t *p_rrc_oam_cell_traffic_trace_start,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_cell_traffic_trace_start, 0, sizeof(rrc_oam_cell_traffic_trace_start_t));

    /* This function parses rrc_oam_cell_traffic_trace_start */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_start->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_start->interface_to_trace, p_src + *p_length_read, "interface_to_trace");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->interface_to_trace > 7))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->interface_to_trace] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->interface_to_trace);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_start->trace_depth, p_src + *p_length_read, "trace_depth");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start->trace_depth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start->trace_depth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start->trace_depth);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_trace_collection_entity(
        &p_rrc_oam_cell_traffic_trace_start->trace_collection_entity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_cell_traffic_trace_start->traceRef) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_cell_traffic_trace_start->traceRef); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_cell_traffic_trace_start->traceRef[loop], (void*)(p_src + *p_length_read), "traceRef[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_trace_collection_entity
(
    rrc_oam_trace_collection_entity_t *p_rrc_oam_trace_collection_entity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_trace_collection_entity, 0, sizeof(rrc_oam_trace_collection_entity_t));

    /* This function parses rrc_oam_trace_collection_entity */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_trace_collection_entity->length, p_src + *p_length_read, "length");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_collection_entity->length > MAX_TRANSPORT_LAYER_ADDR_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_collection_entity->length] should be less than"
            " or equal to MAX_TRANSPORT_LAYER_ADDR_SIZE. Incorrect value %u received.", p_rrc_oam_trace_collection_entity->length);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_trace_collection_entity->length * sizeof(p_rrc_oam_trace_collection_entity->data[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_trace_collection_entity->length; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_trace_collection_entity->data[loop], (void*)(p_src + *p_length_read), "data[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_cell_traffic_trace_stop
(
    rrc_oam_cell_traffic_trace_stop_t *p_rrc_oam_cell_traffic_trace_stop,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_cell_traffic_trace_stop, 0, sizeof(rrc_oam_cell_traffic_trace_stop_t));

    /* This function parses rrc_oam_cell_traffic_trace_stop */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_stop->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_stop->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_stop->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_stop->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_cell_traffic_trace_start_error_ind
(
    rrc_oam_cell_traffic_trace_start_error_ind_t *p_rrc_oam_cell_traffic_trace_start_error_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_cell_traffic_trace_start_error_ind, 0, sizeof(rrc_oam_cell_traffic_trace_start_error_ind_t));

    /* This function parses rrc_oam_cell_traffic_trace_start_error_ind */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_cell_traffic_trace_start_error_ind->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_cell_traffic_trace_start_error_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_trace_file_transfer_start_ind
(
    rrc_oam_trace_file_transfer_start_ind_t *p_rrc_oam_trace_file_transfer_start_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_trace_file_transfer_start_ind, 0, sizeof(rrc_oam_trace_file_transfer_start_ind_t));

    /* This function parses rrc_oam_trace_file_transfer_start_ind */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_trace_file_transfer_start_ind->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_file_transfer_start_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file_transfer_start_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_file_transfer_start_ind->cell_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_trace_file_transfer_start_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(UInt16);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_e_utran_trace_id(
        &p_rrc_oam_trace_file_transfer_start_ind->e_utran_trace_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_trace_file(
        &p_rrc_oam_trace_file_transfer_start_ind->trace_file,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_trace_collection_entity(
        &p_rrc_oam_trace_file_transfer_start_ind->trace_collection_entity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_e_utran_trace_id
(
    rrc_oam_e_utran_trace_id_t *p_rrc_oam_e_utran_trace_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_e_utran_trace_id, 0, sizeof(rrc_oam_e_utran_trace_id_t));

    /* This function parses rrc_oam_e_utran_trace_id */

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_e_utran_trace_id->trace_reference) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_e_utran_trace_id->trace_reference); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_e_utran_trace_id->trace_reference[loop], (void*)(p_src + *p_length_read), "trace_reference[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_e_utran_trace_id->trace_recording_session_reference, p_src + *p_length_read, "trace_recording_session_reference");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_oam_trace_file
(
    rrc_oam_trace_file_t *p_rrc_oam_trace_file,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_trace_file, 0, sizeof(rrc_oam_trace_file_t));

    /* This function parses rrc_oam_trace_file */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_trace_file->len_file_name, p_src + *p_length_read, "len_file_name");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_trace_file->len_file_name < 1) || (p_rrc_oam_trace_file->len_file_name > MAX_FILE_NAME_SIZE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file->len_file_name] should be in range "
            "1 to MAX_FILE_NAME_SIZE. Incorrect value %u received.", p_rrc_oam_trace_file->len_file_name);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrc_oam_trace_file->len_file_name * sizeof(p_rrc_oam_trace_file->file_name[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_oam_trace_file->len_file_name; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_trace_file->file_name[loop], (void*)(p_src + *p_length_read), "file_name[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_trace_file_transfer_complete_ind
(
    rrc_oam_trace_file_transfer_complete_ind_t *p_rrc_oam_trace_file_transfer_complete_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_trace_file_transfer_complete_ind, 0, sizeof(rrc_oam_trace_file_transfer_complete_ind_t));

    /* This function parses rrc_oam_trace_file_transfer_complete_ind */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_trace_file_transfer_complete_ind->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_file_transfer_complete_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_file_transfer_complete_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_file_transfer_complete_ind->cell_index);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_trace_file(
        &p_rrc_oam_trace_file_transfer_complete_ind->trace_file,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_trace_start_error_ind
(
    rrc_oam_trace_start_error_ind_t *p_rrc_oam_trace_start_error_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_trace_start_error_ind, 0, sizeof(rrc_oam_trace_start_error_ind_t));

    /* This function parses rrc_oam_trace_start_error_ind */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_trace_start_error_ind->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_trace_start_error_ind->cell_index > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_trace_start_error_ind->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_oam_trace_start_error_ind->cell_index);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_trace_failure_list(
        &p_rrc_oam_trace_start_error_ind->trace_failure_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_trace_failure_list_item
(
    rrc_trace_failure_list_item_t *p_rrc_trace_failure_list_item,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_trace_failure_list_item, 0, sizeof(rrc_trace_failure_list_item_t));

    /* This function parses rrc_trace_failure_list_item */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_trace_failure_list_item->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_trace_failure_list_item->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_trace_failure_list
(
    rrc_trace_failure_list_t *p_rrc_trace_failure_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_trace_failure_list, 0, sizeof(rrc_trace_failure_list_t));

    /* This function parses rrc_trace_failure_list */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_trace_failure_list->counter, p_src + *p_length_read, "counter");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_trace_failure_list->counter < 1) || (p_rrc_trace_failure_list->counter > RRC_MAX_NUM_SUPPORTED_UE))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_trace_failure_list->counter] should be in range "
            "1 to RRC_MAX_NUM_SUPPORTED_UE. Incorrect value %u received.", p_rrc_trace_failure_list->counter);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_trace_failure_list->counter; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_trace_failure_list_item(
                &p_rrc_trace_failure_list->item[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_add_lgw_req
(
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_add_lgw_req, 0, sizeof(rrc_oam_add_lgw_req_t));

    /* This function parses rrc_oam_add_lgw_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_add_lgw_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_add_lgw_req->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_add_lgw_req->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_add_lgw_req->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_rrc_oam_add_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrc_oam_add_lgw_req->ipv6_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_oam_add_lgw_req->ipv6_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_rrc_oam_add_lgw_req->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_add_lgw_resp
(
    rrc_oam_add_lgw_resp_t *p_rrc_oam_add_lgw_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_add_lgw_resp, 0, sizeof(rrc_oam_add_lgw_resp_t));

    /* This function parses rrc_oam_add_lgw_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_add_lgw_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_add_lgw_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_add_lgw_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_add_lgw_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_add_lgw_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_add_lgw_resp->bitmask & RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_add_lgw_resp->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_add_lgw_resp->cause < 1) || (p_rrc_oam_add_lgw_resp->cause > 3))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_add_lgw_resp->cause] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_oam_add_lgw_resp->cause);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_delete_lgw_req
(
    rrc_oam_delete_lgw_req_t *p_rrc_oam_delete_lgw_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_delete_lgw_req, 0, sizeof(rrc_oam_delete_lgw_req_t));

    /* This function parses rrc_oam_delete_lgw_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_delete_lgw_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_delete_lgw_resp
(
    rrc_oam_delete_lgw_resp_t *p_rrc_oam_delete_lgw_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_delete_lgw_resp, 0, sizeof(rrc_oam_delete_lgw_resp_t));

    /* This function parses rrc_oam_delete_lgw_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_delete_lgw_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_delete_lgw_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_oam_delete_lgw_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_delete_lgw_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_oam_delete_lgw_resp->response);
        return OAM_FAILURE;
    }

    if (p_rrc_oam_delete_lgw_resp->bitmask & RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrc_oam_delete_lgw_resp->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_oam_delete_lgw_resp->cause < 1) || (p_rrc_oam_delete_lgw_resp->cause > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrc_oam_delete_lgw_resp->cause] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_oam_delete_lgw_resp->cause);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_debug_info_req
(
    rrc_oam_get_debug_info_req_t *p_rrc_oam_get_debug_info_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_debug_info_req, 0, sizeof(rrc_oam_get_debug_info_req_t));

    /* This function parses rrc_oam_get_debug_info_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_debug_info_req->debug_type, p_src + *p_length_read, "debug_type");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_rrc_oam_get_debug_info_resp
(
    rrc_oam_get_debug_info_resp_t *p_rrc_oam_get_debug_info_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_oam_get_debug_info_resp, 0, sizeof(rrc_oam_get_debug_info_resp_t));

    /* This function parses rrc_oam_get_debug_info_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrc_oam_get_debug_info_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(SInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT32(&p_rrc_oam_get_debug_info_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(SInt32);

    if (p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(SInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT32(&p_rrc_oam_get_debug_info_resp->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(SInt32);
    }

    if (p_rrc_oam_get_debug_info_resp->bitmask & RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_MemPoolStats(
        &p_rrc_oam_get_debug_info_resp->csplMemPoolStats,
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_PoolStats
(
    rrc_PoolStats *p_rrc_PoolStats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_PoolStats, 0, sizeof(rrc_PoolStats));

    /* This function parses rrc_PoolStats */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_PoolStats->bufSize, p_src + *p_length_read, "bufSize");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_PoolStats->numOfBuf, p_src + *p_length_read, "numOfBuf");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_PoolStats->numOfAllocBuf, p_src + *p_length_read, "numOfAllocBuf");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_PoolStats->numOfPeakBuf, p_src + *p_length_read, "numOfPeakBuf");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_rrc_MemPoolStats
(
    rrc_MemPoolStats *p_rrc_MemPoolStats,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_rrc_MemPoolStats, 0, sizeof(rrc_MemPoolStats));

    /* This function parses rrc_MemPoolStats */

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_MemPoolStats->stats); loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_rrc_PoolStats(
                &p_rrc_MemPoolStats->stats[loop],
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
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

