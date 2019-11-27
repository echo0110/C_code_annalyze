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
 *  File Description : The file rrc_s1apRrm_il_parser.c contains the definitions 
 *                     of rrc-s1apRrm interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifdef _MSC_VER
#include "pc_test.h"
#endif
#include "rrc_cp_common.h"
#include "rrc_s1apRrm_il_parser.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_ext_api.h"
#ifndef _MSC_VER
#include "rrc_common_utils.h"
#endif

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

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
rrc_il_parse_rrm_s1ap_lai_info
(
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_geran_cell_id
(
    rrm_geran_cell_id_t *p_rrm_geran_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_lai_info
(
    rrm_lai_info_t *p_rrm_lai_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_rnc_id
(
    rrm_rnc_id_t *p_rrm_rnc_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_macro_enb_id
(
    rrm_macro_enb_id_t *p_rrm_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_home_enb_id
(
    rrm_home_enb_id_t *p_rrm_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_enb_id_info
(
    rrm_enb_id_info_t *p_rrm_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_gb_enb_id
(
    rrm_gb_enb_id_t *p_rrm_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_supp_ta
(
    rrm_supp_ta_t *p_rrm_supp_ta,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_ip_address
(
    ip_address_t *p_ip_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_trans_layer_addrs
(
    trans_layer_addrs_t *p_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_extend_trans_layer_addrs
(
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_plmn_identity
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_cgi_eutra
(
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cell_id_list
(
    cell_id_list_t *p_cell_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_tracking_area_identifier
(
    tracking_area_identifier_t *p_tracking_area_identifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_tracking_area_id_list
(
    tracking_area_id_list_t *p_tracking_area_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_emergency_area_identifier
(
    emergency_area_identifier_t *p_emergency_area_identifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_emergency_area_id_list
(
    emergency_area_id_list_t *p_emergency_area_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_warning_area_list_info
(
    warning_area_list_info_t *p_warning_area_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_sib_mapping_info
(
    sib_mapping_info_t *p_sib_mapping_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_scheduling_info
(
    scheduling_info_t *p_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_scheduling_info_list
(
    scheduling_info_list_t *p_scheduling_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_si_start_offset
(
    rrc_si_start_offset_t *p_rrc_si_start_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_si_start_offset_info
(
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_si_transmission_info
(
    rrc_si_transmission_info_t *p_rrc_si_transmission_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_si_transmission_info_list
(
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_etws_scheduling_info
(
    etws_scheduling_info_t *p_etws_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cmas_scheduling_info
(
    cmas_scheduling_info_t *p_cmas_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cmas_scheduling_info_list
(
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rrm_pws_contents
(
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rrm_pws_contents_list
(
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_pws_broadcast_resp
(
    pws_broadcast_resp_t *p_pws_broadcast_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_pws_broadcast_list
(
    pws_broadcast_list_t *p_pws_broadcast_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_broadcast_cancellation
(
    broadcast_cancellation_t *p_broadcast_cancellation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_broadcast_cancellation_list
(
    broadcast_cancellation_list_t *p_broadcast_cancellation_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_plmn
(
    rim_plmn_t *p_rim_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_routing_identification
(
    rim_routing_identification_t *p_rim_routing_identification,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_geran_routing_addr
(
    rim_geran_routing_addr_t *p_rim_geran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_utran_routing_addr
(
    rim_utran_routing_addr_t *p_rim_utran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_ta_identity
(
    rim_ta_identity_t *p_rim_ta_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_global_enb_id
(
    rim_global_enb_id_t *p_rim_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_eutran_routing_addr
(
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_routing_info
(
    rim_routing_info_t *p_rim_routing_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_pdu_ind_rim_info_req
(
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_contrn_nacc
(
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_contrn_si3
(
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_contrn_mbms
(
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_contrn_son
(
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_utran_cell_idty
(
    rim_utran_cell_idty_t *p_rim_utran_cell_idty,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_contrn_utra_si
(
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_req_app_container
(
    rim_req_app_container_t *p_rim_req_app_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_son_transfer_app_idty
(
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_pdu_ind_rim_info
(
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_si_psi_individual
(
    rim_si_psi_individual_t *p_rim_si_psi_individual,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_contrn_nacc
(
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_contrn_si3
(
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_contrn_mbms
(
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_contrn_son
(
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_contrn_utra_si
(
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_info_app_container
(
    rim_info_app_container_t *p_rim_info_app_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_contrn_nacc
(
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_contrn_si3
(
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_contrn_mbms
(
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_contrn_son
(
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_contrn_utra_si
(
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_app_err_container
(
    rim_app_err_container_t *p_rim_app_err_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_pdu_ind_rim_app_err
(
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_pdu_in_error
(
    rim_pdu_in_error_t *p_rim_pdu_in_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_group_id
(
    s1ap_served_group_id_t *p_s1ap_served_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_mmec
(
    s1ap_served_mmec_t *p_s1ap_served_mmec,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_gummei_info
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_mme_info
(
    s1ap_mme_info_t *p_s1ap_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_meas_quantifier
(
    lppa_meas_quantifier_t *p_lppa_meas_quantifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_plmn_identity
(
    lppa_plmn_identity_t *p_lppa_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_ecgi
(
    lppa_ecgi_t *p_lppa_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_e_utran_access_point_pos
(
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_result_rsrp
(
    lppa_result_rsrp_t *p_lppa_result_rsrp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_meas_result_rsrp
(
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_result_rsrq
(
    lppa_result_rsrq_t *p_lppa_result_rsrq,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_meas_result_rsrq
(
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_measured_res
(
    lppa_measured_res_t *p_lppa_measured_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_meas_results
(
    lppa_meas_results_t *p_lppa_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_e_cid_meas_result
(
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_cause
(
    lppa_cause_t *p_lppa_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_otdoa_info_type
(
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_otdoa_cells_tac
(
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_otdoa_sfn_init_time
(
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_muting_config_two
(
    lppa_muting_config_two_t *p_lppa_muting_config_two,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_muting_config_four
(
    lppa_muting_config_four_t *p_lppa_muting_config_four,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_muting_config_eight
(
    lppa_muting_config_eight_t *p_lppa_muting_config_eight,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_muting_config_sixteen
(
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_prs_muting_config
(
    lppa_prs_muting_config_t *p_lppa_prs_muting_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_otdoa_cells_info
(
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_otdoa_cells
(
    lppa_otdoa_cells_t *p_lppa_otdoa_cells,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_lppa_criticality_diagno
(
    lppa_criticality_diagno_t *p_lppa_criticality_diagno,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_s1ap_rrm_enb_direct_info_transfer
(
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_enb_direct_info_transfer, 0, sizeof(s1ap_rrm_enb_direct_info_transfer_t));

    /* This function parses s1ap_rrm_enb_direct_info_transfer */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_enb_direct_info_transfer->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_enb_direct_info_transfer->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_direct_info_transfer->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_enb_direct_info_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_enb_direct_info_transfer->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_enb_direct_info_transfer->len_rim_info, p_src + *p_length_read, "len_rim_info");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_direct_info_transfer->len_rim_info > 1024))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_enb_direct_info_transfer->len_rim_info] should be less than"
            " or equal to 1024. Incorrect value %u received.", p_s1ap_rrm_enb_direct_info_transfer->len_rim_info);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_s1ap_rrm_enb_direct_info_transfer->len_rim_info * sizeof(p_s1ap_rrm_enb_direct_info_transfer->rim_info[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_enb_direct_info_transfer->len_rim_info; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_enb_direct_info_transfer->rim_info[loop], (void*)(p_src + *p_length_read), "rim_info[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_s1ap_rrm_enb_direct_info_transfer->bitmask & GERAN_CELL_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_geran_cell_id(
        &p_s1ap_rrm_enb_direct_info_transfer->geran_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_rrm_enb_direct_info_transfer->bitmask & TARGET_RNC_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_rnc_id(
        &p_s1ap_rrm_enb_direct_info_transfer->target_rnc_id,
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
rrc_il_parse_rrm_s1ap_lai_info
(
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_s1ap_lai_info, 0, sizeof(rrm_s1ap_lai_info_t));

    /* This function parses rrm_s1ap_lai_info */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_rrm_s1ap_lai_info->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_s1ap_lai_info->location_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_s1ap_lai_info->location_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_s1ap_lai_info->location_area_code[loop], (void*)(p_src + *p_length_read), "location_area_code[]");
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
rrc_il_parse_rrm_geran_cell_id
(
    rrm_geran_cell_id_t *p_rrm_geran_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_geran_cell_id, 0, sizeof(rrm_geran_cell_id_t));

    /* This function parses rrm_geran_cell_id */

    if (RRC_FAILURE == rrc_il_parse_rrm_s1ap_lai_info(
        &p_rrm_geran_cell_id->lai,
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
    rrc_cp_unpack_U8(&p_rrm_geran_cell_id->routing_area_code, p_src + *p_length_read, "routing_area_code");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(p_rrm_geran_cell_id->ci) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_geran_cell_id->ci); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_geran_cell_id->ci[loop], (void*)(p_src + *p_length_read), "ci[]");
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
rrc_il_parse_rrm_lai_info
(
    rrm_lai_info_t *p_rrm_lai_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_lai_info, 0, sizeof(rrm_lai_info_t));

    /* This function parses rrm_lai_info */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_rrm_lai_info->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_lai_info->location_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_lai_info->location_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_lai_info->location_area_code[loop], (void*)(p_src + *p_length_read), "location_area_code[]");
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
rrc_il_parse_rrm_rnc_id
(
    rrm_rnc_id_t *p_rrm_rnc_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_rnc_id, 0, sizeof(rrm_rnc_id_t));

    /* This function parses rrm_rnc_id */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrm_rnc_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrm_lai_info(
        &p_rrm_rnc_id->lai,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_rnc_id->bitmask & RRM_RNC_ROUTING_AREA_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrm_rnc_id->routing_area_code, p_src + *p_length_read, "routing_area_code");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrm_rnc_id->rnc_id, p_src + *p_length_read, "rnc_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_rnc_id->rnc_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrm_rnc_id->rnc_id] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrm_rnc_id->rnc_id);
        return RRC_FAILURE;
    }

    if (p_rrm_rnc_id->bitmask & RRM_RNC_EXTENDED_RNC_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrm_rnc_id->extended_rnc_id, p_src + *p_length_read, "extended_rnc_id");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_rrm_mme_direct_info_transfer
(
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_mme_direct_info_transfer, 0, sizeof(s1ap_rrm_mme_direct_info_transfer_t));

    /* This function parses s1ap_rrm_mme_direct_info_transfer */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_direct_info_transfer->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_direct_info_transfer->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_mme_direct_info_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_mme_direct_info_transfer->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_mme_direct_info_transfer->len_rim_info, p_src + *p_length_read, "len_rim_info");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_direct_info_transfer->len_rim_info > MAX_BSSGP_PDU_LEN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_mme_direct_info_transfer->len_rim_info] should be less than"
            " or equal to MAX_BSSGP_PDU_LEN. Incorrect value %u received.", p_s1ap_rrm_mme_direct_info_transfer->len_rim_info);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_s1ap_rrm_mme_direct_info_transfer->len_rim_info * sizeof(p_s1ap_rrm_mme_direct_info_transfer->rim_info[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_mme_direct_info_transfer->len_rim_info; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_mme_direct_info_transfer->rim_info[loop], (void*)(p_src + *p_length_read), "rim_info[]");
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
rrc_il_parse_s1ap_rrm_enb_config_transfer
(
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_enb_config_transfer, 0, sizeof(s1ap_rrm_enb_config_transfer_t));

    /* This function parses s1ap_rrm_enb_config_transfer */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_enb_config_transfer->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_enb_config_transfer->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_config_transfer->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_enb_config_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_enb_config_transfer->mme_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrm_gb_enb_id(
        &p_s1ap_rrm_enb_config_transfer->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrm_supp_ta(
        &p_s1ap_rrm_enb_config_transfer->target_supp_ta,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrm_supp_ta(
        &p_s1ap_rrm_enb_config_transfer->source_supp_ta,
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
rrc_il_parse_rrm_macro_enb_id
(
    rrm_macro_enb_id_t *p_rrm_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrm_macro_enb_id, 0, sizeof(rrm_macro_enb_id_t));

    /* This function parses rrm_macro_enb_id */

    if (*p_length_read + (S32)sizeof(p_rrm_macro_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_macro_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
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
rrc_il_parse_rrm_home_enb_id
(
    rrm_home_enb_id_t *p_rrm_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrm_home_enb_id, 0, sizeof(rrm_home_enb_id_t));

    /* This function parses rrm_home_enb_id */

    if (*p_length_read + (S32)sizeof(p_rrm_home_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_home_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
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
rrc_il_parse_rrm_enb_id_info
(
    rrm_enb_id_info_t *p_rrm_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_enb_id_info, 0, sizeof(rrm_enb_id_info_t));

    /* This function parses rrm_enb_id_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrm_enb_id_info->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_enb_id_info->presence_bitmask & 1)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_macro_enb_id(
        &p_rrm_enb_id_info->macro_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_enb_id_info->presence_bitmask & 2)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_home_enb_id(
        &p_rrm_enb_id_info->home_enb_id,
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
rrc_il_parse_rrm_gb_enb_id
(
    rrm_gb_enb_id_t *p_rrm_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_gb_enb_id, 0, sizeof(rrm_gb_enb_id_t));

    /* This function parses rrm_gb_enb_id */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_rrm_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrm_enb_id_info(
        &p_rrm_gb_enb_id->enb_id,
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
rrc_il_parse_rrm_supp_ta
(
    rrm_supp_ta_t *p_rrm_supp_ta,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_supp_ta, 0, sizeof(rrm_supp_ta_t));

    /* This function parses rrm_supp_ta */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_rrm_supp_ta->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_supp_ta->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_supp_ta->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_supp_ta->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
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
rrc_il_parse_s1ap_rrm_mme_config_transfer
(
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_mme_config_transfer, 0, sizeof(s1ap_rrm_mme_config_transfer_t));

    /* This function parses s1ap_rrm_mme_config_transfer */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_mme_config_transfer->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_config_transfer->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_config_transfer->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->mme_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rrm_gb_enb_id(
        &p_s1ap_rrm_mme_config_transfer->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrm_supp_ta(
        &p_s1ap_rrm_mme_config_transfer->source_supp_ta,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rrm_supp_ta(
        &p_s1ap_rrm_mme_config_transfer->target_supp_ta,
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
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_config_transfer->number_of_addr, p_src + *p_length_read, "number_of_addr");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_addr); loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ip_address(
                &p_s1ap_rrm_mme_config_transfer->ip_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr, p_src + *p_length_read, "number_of_ipv6_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr > MAX_NUM_IP_SUPPORTED))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr] should be less than"
            " or equal to MAX_NUM_IP_SUPPORTED. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr);
        return RRC_FAILURE;
    }
    }

    if (p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_v6_addr); loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ip_address(
                &p_s1ap_rrm_mme_config_transfer->ip_v6_addr[loop],
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

    if (p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_config_transfer->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_mme_config_transfer->num_tlas < 1) || (p_s1ap_rrm_mme_config_transfer->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_mme_config_transfer->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_extend_trans_layer_addrs(
                &p_s1ap_rrm_mme_config_transfer->extend_trans_layer_addrs[loop],
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
rrc_il_parse_ip_address
(
    ip_address_t *p_ip_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_ip_address, 0, sizeof(ip_address_t));

    /* This function parses ip_address */

    if (*p_length_read + (S32)sizeof(p_ip_address->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ip_address->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_ip_address->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
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
rrc_il_parse_trans_layer_addrs
(
    trans_layer_addrs_t *p_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_trans_layer_addrs, 0, sizeof(trans_layer_addrs_t));

    /* This function parses trans_layer_addrs */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_trans_layer_addrs->data_length, p_src + *p_length_read, "data_length");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_trans_layer_addrs->data_length);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_trans_layer_addrs->data_length * sizeof(p_trans_layer_addrs->data[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_trans_layer_addrs->data_length; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_trans_layer_addrs->data[loop], (void*)(p_src + *p_length_read), "data[]");
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
rrc_il_parse_extend_trans_layer_addrs
(
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_extend_trans_layer_addrs, 0, sizeof(extend_trans_layer_addrs_t));

    /* This function parses extend_trans_layer_addrs */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_extend_trans_layer_addrs->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_trans_layer_addrs(
        &p_extend_trans_layer_addrs->ipsec_trans_layer_addrs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_extend_trans_layer_addrs->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_extend_trans_layer_addrs->num_tlas < 1) || (p_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_trans_layer_addrs(
                &p_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop],
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
rrc_il_parse_s1ap_rrm_pws_request
(
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_pws_request, 0, sizeof(s1ap_rrm_pws_request_t));

    /* This function parses s1ap_rrm_pws_request */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_request->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_request->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_request->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_request->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_request->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_AREA_LIST)
    {

    if (RRC_FAILURE == rrc_il_parse_warning_area_list_info(
        &p_s1ap_rrm_pws_request->warning_area_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_request->repetition_period, p_src + *p_length_read, "repetition_period");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->repetition_period > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_request->repetition_period] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_s1ap_rrm_pws_request->repetition_period);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_pws_request->extended_repetition_period, p_src + *p_length_read, "extended_repetition_period");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->extended_repetition_period > 131071))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_request->extended_repetition_period] should be less than"
            " or equal to 131071. Incorrect value %u received.", p_s1ap_rrm_pws_request->extended_repetition_period);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_request->number_of_broadcast_requested, p_src + *p_length_read, "number_of_broadcast_requested");
    *p_length_read += sizeof(U16);

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_TYPE)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_request->warning_type) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->warning_type); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_request->warning_type[loop], (void*)(p_src + *p_length_read), "warning_type[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_SECURITY_INFO)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_request->warning_security_info) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->warning_security_info); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_request->warning_security_info[loop], (void*)(p_src + *p_length_read), "warning_security_info[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_DATA_CODING_SCHEME)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_request->data_coding_scheme) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->data_coding_scheme); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_request->data_coding_scheme[loop], (void*)(p_src + *p_length_read), "data_coding_scheme[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_MESSAGE_SIZE)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_request->warning_message_size, p_src + *p_length_read, "warning_message_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->warning_message_size > 9600))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_request->warning_message_size] should be less than"
            " or equal to 9600. Incorrect value %u received.", p_s1ap_rrm_pws_request->warning_message_size);
        return RRC_FAILURE;
    }
    }

    if (p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator, p_src + *p_length_read, "concurrent_warning_msg_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator] should be less than"
            " or equal to 0. Incorrect value %u received.", p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator);
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
rrc_il_parse_s1ap_plmn_identity
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_plmn_identity, 0, sizeof(s1ap_plmn_identity_t));

    /* This function parses s1ap_plmn_identity */

    if (*p_length_read + (S32)sizeof(p_s1ap_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
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
rrc_il_parse_s1ap_cgi_eutra
(
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_cgi_eutra, 0, sizeof(s1ap_cgi_eutra_t));

    /* This function parses s1ap_cgi_eutra */

    if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
        &p_s1ap_cgi_eutra->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_s1ap_cgi_eutra->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_cgi_eutra->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_cgi_eutra->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
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
rrc_il_parse_cell_id_list
(
    cell_id_list_t *p_cell_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_cell_id_list, 0, sizeof(cell_id_list_t));

    /* This function parses cell_id_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_cell_id_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cell_id_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_cgi_eutra(
                &p_cell_id_list->eutran_cgi[loop],
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
rrc_il_parse_tracking_area_identifier
(
    tracking_area_identifier_t *p_tracking_area_identifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_tracking_area_identifier, 0, sizeof(tracking_area_identifier_t));

    /* This function parses tracking_area_identifier */

    if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
        &p_tracking_area_identifier->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_tracking_area_identifier->tracking_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_tracking_area_identifier->tracking_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_tracking_area_identifier->tracking_area_code[loop], (void*)(p_src + *p_length_read), "tracking_area_code[]");
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
rrc_il_parse_tracking_area_id_list
(
    tracking_area_id_list_t *p_tracking_area_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_tracking_area_id_list, 0, sizeof(tracking_area_id_list_t));

    /* This function parses tracking_area_id_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_tracking_area_id_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_tracking_area_id_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_tracking_area_identifier(
                &p_tracking_area_id_list->tracking_area_identifier[loop],
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
rrc_il_parse_emergency_area_identifier
(
    emergency_area_identifier_t *p_emergency_area_identifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_emergency_area_identifier, 0, sizeof(emergency_area_identifier_t));

    /* This function parses emergency_area_identifier */

    if (*p_length_read + (S32)sizeof(p_emergency_area_identifier->emergency_area_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_emergency_area_identifier->emergency_area_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_emergency_area_identifier->emergency_area_id[loop], (void*)(p_src + *p_length_read), "emergency_area_id[]");
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
rrc_il_parse_emergency_area_id_list
(
    emergency_area_id_list_t *p_emergency_area_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_emergency_area_id_list, 0, sizeof(emergency_area_id_list_t));

    /* This function parses emergency_area_id_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_emergency_area_id_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_emergency_area_id_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_emergency_area_identifier(
                &p_emergency_area_id_list->emer_id[loop],
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
rrc_il_parse_warning_area_list_info
(
    warning_area_list_info_t *p_warning_area_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_warning_area_list_info, 0, sizeof(warning_area_list_info_t));

    /* This function parses warning_area_list_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_warning_area_list_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_CELL_ID_LIST)
    {

    if (RRC_FAILURE == rrc_il_parse_cell_id_list(
        &p_warning_area_list_info->cell_id_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST)
    {

    if (RRC_FAILURE == rrc_il_parse_tracking_area_id_list(
        &p_warning_area_list_info->tracking_area_id_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST)
    {

    if (RRC_FAILURE == rrc_il_parse_emergency_area_id_list(
        &p_warning_area_list_info->emergency_area_id_list,
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
rrc_il_parse_s1ap_rrm_pws_response
(
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_pws_response, 0, sizeof(s1ap_rrm_pws_response_t));

    /* This function parses s1ap_rrm_pws_response */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_response->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_pws_response->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_response->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_response->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_pws_response->response);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_pws_response->bitmask & S1AP_RRM_PWS_CONTENTS_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rrm_pws_contents_list(
        &p_s1ap_rrm_pws_response->rrc_rrm_pws_contents_list,
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
rrc_il_parse_sib_mapping_info
(
    sib_mapping_info_t *p_sib_mapping_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_sib_mapping_info, 0, sizeof(sib_mapping_info_t));

    /* This function parses sib_mapping_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_sib_mapping_info->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)(p_sib_mapping_info->count * sizeof(p_sib_mapping_info->sib_type[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_sib_mapping_info->count; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_sib_mapping_info->sib_type[loop], (void*)(p_src + *p_length_read), "sib_type[]");
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
rrc_il_parse_scheduling_info
(
    scheduling_info_t *p_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_scheduling_info, 0, sizeof(scheduling_info_t));

    /* This function parses scheduling_info */

    if (RRC_FAILURE == rrc_il_parse_sib_mapping_info(
        &p_scheduling_info->sib_mapping_info,
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
    rrc_cp_unpack_U8(&p_scheduling_info->si_periodicity, p_src + *p_length_read, "si_periodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_scheduling_info->si_periodicity > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scheduling_info->si_periodicity] should be less than"
            " or equal to 6. Incorrect value %u received.", p_scheduling_info->si_periodicity);
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
rrc_il_parse_scheduling_info_list
(
    scheduling_info_list_t *p_scheduling_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_scheduling_info_list, 0, sizeof(scheduling_info_list_t));

    /* This function parses scheduling_info_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_scheduling_info_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_scheduling_info_list->count < 1) || (p_scheduling_info_list->count > MAX_SI_MESSAGE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scheduling_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %u received.", p_scheduling_info_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_scheduling_info_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_scheduling_info(
                &p_scheduling_info_list->scheduling_info[loop],
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
rrc_il_parse_rrc_si_start_offset
(
    rrc_si_start_offset_t *p_rrc_si_start_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_si_start_offset, 0, sizeof(rrc_si_start_offset_t));

    /* This function parses rrc_si_start_offset */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_si_start_offset->si_start_offset, p_src + *p_length_read, "si_start_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_si_start_offset->si_start_offset > 39))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_si_start_offset->si_start_offset] should be less than"
            " or equal to 39. Incorrect value %u received.", p_rrc_si_start_offset->si_start_offset);
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
rrc_il_parse_rrc_si_start_offset_info
(
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_si_start_offset_info, 0, sizeof(rrc_si_start_offset_info_t));

    /* This function parses rrc_si_start_offset_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_si_start_offset_info->offset_count, p_src + *p_length_read, "offset_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_start_offset_info->offset_count < 1) || (p_rrc_si_start_offset_info->offset_count > MAX_SI_START_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_si_start_offset_info->offset_count] should be in range "
            "1 to MAX_SI_START_OFFSET. Incorrect value %u received.", p_rrc_si_start_offset_info->offset_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_start_offset_info->offset_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_si_start_offset(
                &p_rrc_si_start_offset_info->si_start_offset[loop],
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
rrc_il_parse_rrc_si_transmission_info
(
    rrc_si_transmission_info_t *p_rrc_si_transmission_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_si_transmission_info, 0, sizeof(rrc_si_transmission_info_t));

    /* This function parses rrc_si_transmission_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_si_transmission_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_si_transmission_info->si_index, p_src + *p_length_read, "si_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_si_transmission_info->si_index > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_si_transmission_info->si_index] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_si_transmission_info->si_index);
        return RRC_FAILURE;
    }

    if (p_rrc_si_transmission_info->bitmask & RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_si_transmission_info->num_of_transmissions, p_src + *p_length_read, "num_of_transmissions");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_transmission_info->num_of_transmissions < 1) || (p_rrc_si_transmission_info->num_of_transmissions > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_si_transmission_info->num_of_transmissions] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_si_transmission_info->num_of_transmissions);
        return RRC_FAILURE;
    }
    }

    if (p_rrc_si_transmission_info->bitmask & RRC_RRM_SI_START_OFFSET_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_si_start_offset_info(
        &p_rrc_si_transmission_info->si_start_offset_info,
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
rrc_il_parse_rrc_si_transmission_info_list
(
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_si_transmission_info_list, 0, sizeof(rrc_si_transmission_info_list_t));

    /* This function parses rrc_si_transmission_info_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_si_transmission_info_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_transmission_info_list->count < 1) || (p_rrc_si_transmission_info_list->count > MAX_SI_MESSAGE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_si_transmission_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %u received.", p_rrc_si_transmission_info_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_transmission_info_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_si_transmission_info(
                &p_rrc_si_transmission_info_list->si_transmission[loop],
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
rrc_il_parse_etws_scheduling_info
(
    etws_scheduling_info_t *p_etws_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_etws_scheduling_info, 0, sizeof(etws_scheduling_info_t));

    /* This function parses etws_scheduling_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_etws_scheduling_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_etws_scheduling_info->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_etws_scheduling_info->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_etws_scheduling_info->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_etws_scheduling_info->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_etws_scheduling_info->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_etws_scheduling_info->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_etws_scheduling_info->bitmask & RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_etws_scheduling_info->num_of_segments, p_src + *p_length_read, "num_of_segments");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_etws_scheduling_info->num_of_segments < 1) || (p_etws_scheduling_info->num_of_segments > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_etws_scheduling_info->num_of_segments] should be in range "
            "1 to 64. Incorrect value %u received.", p_etws_scheduling_info->num_of_segments);
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
rrc_il_parse_cmas_scheduling_info
(
    cmas_scheduling_info_t *p_cmas_scheduling_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_cmas_scheduling_info, 0, sizeof(cmas_scheduling_info_t));

    /* This function parses cmas_scheduling_info */

    if (*p_length_read + (S32)sizeof(p_cmas_scheduling_info->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cmas_scheduling_info->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cmas_scheduling_info->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_cmas_scheduling_info->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cmas_scheduling_info->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cmas_scheduling_info->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_cmas_scheduling_info->num_of_segments, p_src + *p_length_read, "num_of_segments");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info->num_of_segments < 1) || (p_cmas_scheduling_info->num_of_segments > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_cmas_scheduling_info->num_of_segments] should be in range "
            "1 to 64. Incorrect value %u received.", p_cmas_scheduling_info->num_of_segments);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_cmas_scheduling_info->num_of_repetitions, p_src + *p_length_read, "num_of_repetitions");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info->num_of_repetitions < 1) || (p_cmas_scheduling_info->num_of_repetitions > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_cmas_scheduling_info->num_of_repetitions] should be in range "
            "1 to 4095. Incorrect value %u received.", p_cmas_scheduling_info->num_of_repetitions);
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
rrc_il_parse_cmas_scheduling_info_list
(
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_cmas_scheduling_info_list, 0, sizeof(cmas_scheduling_info_list_t));

    /* This function parses cmas_scheduling_info_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_cmas_scheduling_info_list->cmas_warning_count, p_src + *p_length_read, "cmas_warning_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info_list->cmas_warning_count < 1) || (p_cmas_scheduling_info_list->cmas_warning_count > MAX_NUM_CMAS_WARNING))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_cmas_scheduling_info_list->cmas_warning_count] should be in range "
            "1 to MAX_NUM_CMAS_WARNING. Incorrect value %u received.", p_cmas_scheduling_info_list->cmas_warning_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cmas_scheduling_info_list->cmas_warning_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_cmas_scheduling_info(
                &p_cmas_scheduling_info_list->cmas_scheduling_info[loop],
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
rrc_il_parse_rrc_rrm_pws_contents
(
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rrm_pws_contents, 0, sizeof(rrc_rrm_pws_contents_t));

    /* This function parses rrc_rrm_pws_contents */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rrm_pws_contents->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rrm_pws_contents->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rrm_pws_contents->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rrm_pws_contents->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rrm_pws_contents->cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_scheduling_info_list(
        &p_rrc_rrm_pws_contents->scheduling_info_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_si_transmission_info_list(
        &p_rrc_rrm_pws_contents->si_transmission_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_etws_scheduling_info(
        &p_rrc_rrm_pws_contents->etws_scheduling_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_cmas_scheduling_info_list(
        &p_rrc_rrm_pws_contents->cmas_scheduling_info_list,
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
rrc_il_parse_rrc_rrm_pws_contents_list
(
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rrm_pws_contents_list, 0, sizeof(rrc_rrm_pws_contents_list_t));

    /* This function parses rrc_rrm_pws_contents_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rrm_pws_contents_list->cell_count, p_src + *p_length_read, "cell_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rrm_pws_contents_list->cell_count < 1) || (p_rrc_rrm_pws_contents_list->cell_count > MAX_NUM_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rrm_pws_contents_list->cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_rrm_pws_contents_list->cell_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rrm_pws_contents_list->cell_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_rrm_pws_contents(
                &p_rrc_rrm_pws_contents_list->rrc_rrm_pws_contents[loop],
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
rrc_il_parse_s1ap_rrm_pws_cnf
(
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_pws_cnf, 0, sizeof(s1ap_rrm_pws_cnf_t));

    /* This function parses s1ap_rrm_pws_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_pws_cnf->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_cnf->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_cnf->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_cnf->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_pws_cnf->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_cnf->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_pws_cnf->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_pws_cnf->pws_resp, p_src + *p_length_read, "pws_resp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_cnf->pws_resp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_pws_cnf->pws_resp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_pws_cnf->pws_resp);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_pws_cnf->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG)
    {

    if (RRC_FAILURE == rrc_il_parse_pws_broadcast_list(
        &p_s1ap_rrm_pws_cnf->pws_broadcast_list,
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
rrc_il_parse_pws_broadcast_resp
(
    pws_broadcast_resp_t *p_pws_broadcast_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_pws_broadcast_resp, 0, sizeof(pws_broadcast_resp_t));

    /* This function parses pws_broadcast_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_pws_broadcast_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pws_broadcast_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pws_broadcast_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_pws_broadcast_resp->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pws_broadcast_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pws_broadcast_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_pws_broadcast_resp->response);
        return RRC_FAILURE;
    }

    if (p_pws_broadcast_resp->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pws_broadcast_resp->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->fail_cause > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pws_broadcast_resp->fail_cause] should be less than"
            " or equal to 3. Incorrect value %u received.", p_pws_broadcast_resp->fail_cause);
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
rrc_il_parse_pws_broadcast_list
(
    pws_broadcast_list_t *p_pws_broadcast_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_pws_broadcast_list, 0, sizeof(pws_broadcast_list_t));

    /* This function parses pws_broadcast_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pws_broadcast_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_pws_broadcast_list->count < 1) || (p_pws_broadcast_list->count > MAX_NUM_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_pws_broadcast_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_pws_broadcast_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_pws_broadcast_list->pws_broadcast_cell_resp); loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_pws_broadcast_resp(
                &p_pws_broadcast_list->pws_broadcast_cell_resp[loop],
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
rrc_il_parse_s1ap_rrm_kill_request
(
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_kill_request, 0, sizeof(s1ap_rrm_kill_request_t));

    /* This function parses s1ap_rrm_kill_request */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_kill_request->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_request->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_request->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_request->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_request->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_request->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_request->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_s1ap_rrm_kill_request->bitmask & KILL_WARNING_AREA_LIST)
    {

    if (RRC_FAILURE == rrc_il_parse_warning_area_list_info(
        &p_s1ap_rrm_kill_request->warning_area_list,
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
rrc_il_parse_s1ap_rrm_kill_response
(
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_kill_response, 0, sizeof(s1ap_rrm_kill_response_t));

    /* This function parses s1ap_rrm_kill_response */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_kill_response->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_response->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_response->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_response->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_response->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_response->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_response->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_kill_response->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_response->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_kill_response->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_kill_response->response);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_FAIL_CAUSE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_kill_response->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_response->fail_cause > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_kill_response->fail_cause] should be less than"
            " or equal to 4. Incorrect value %u received.", p_s1ap_rrm_kill_response->fail_cause);
        return RRC_FAILURE;
    }
    }

    if (p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rrm_pws_contents_list(
        &p_s1ap_rrm_kill_response->rrc_rrm_pws_contents_list,
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
rrc_il_parse_s1ap_rrm_kill_cnf
(
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_kill_cnf, 0, sizeof(s1ap_rrm_kill_cnf_t));

    /* This function parses s1ap_rrm_kill_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_kill_cnf->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_cnf->message_identifier) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_cnf->message_identifier); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_cnf->message_identifier[loop], (void*)(p_src + *p_length_read), "message_identifier[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_kill_cnf->serial_number) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_cnf->serial_number); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_kill_cnf->serial_number[loop], (void*)(p_src + *p_length_read), "serial_number[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_kill_cnf->kill_resp, p_src + *p_length_read, "kill_resp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_cnf->kill_resp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_kill_cnf->kill_resp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_kill_cnf->kill_resp);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_kill_cnf->bitmask & S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_broadcast_cancellation_list(
        &p_s1ap_rrm_kill_cnf->broadcast_cancellation_list,
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
rrc_il_parse_broadcast_cancellation
(
    broadcast_cancellation_t *p_broadcast_cancellation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_broadcast_cancellation, 0, sizeof(broadcast_cancellation_t));

    /* This function parses broadcast_cancellation */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_broadcast_cancellation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_broadcast_cancellation->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_broadcast_cancellation->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_broadcast_cancellation->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_broadcast_cancellation->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_broadcast_cancellation->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_broadcast_cancellation->response);
        return RRC_FAILURE;
    }

    if (p_broadcast_cancellation->bitmask & S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_broadcast_cancellation->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->fail_cause > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_broadcast_cancellation->fail_cause] should be less than"
            " or equal to 4. Incorrect value %u received.", p_broadcast_cancellation->fail_cause);
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
rrc_il_parse_broadcast_cancellation_list
(
    broadcast_cancellation_list_t *p_broadcast_cancellation_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_broadcast_cancellation_list, 0, sizeof(broadcast_cancellation_list_t));

    /* This function parses broadcast_cancellation_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_broadcast_cancellation_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_broadcast_cancellation_list->count < 1) || (p_broadcast_cancellation_list->count > MAX_NUM_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_broadcast_cancellation_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_broadcast_cancellation_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_broadcast_cancellation_list->broadcast_cancellation); loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_broadcast_cancellation(
                &p_broadcast_cancellation_list->broadcast_cancellation[loop],
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
rrc_il_parse_rim_information_req
(
    rim_information_req_t *p_rim_information_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information_req, 0, sizeof(rim_information_req_t));

    /* This function parses rim_information_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_information_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_req->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_req->dst_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_req->src_cell,
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
    rrc_cp_unpack_U8(&p_rim_information_req->app_idty, p_src + *p_length_read, "app_idty");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_req->app_idty < 1) || (p_rim_information_req->app_idty > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_req->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_req->app_idty);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rim_information_req->seq_num, p_src + *p_length_read, "seq_num");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_rim_pdu_ind_rim_info_req(
        &p_rim_information_req->pdu_ind,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rim_information_req->bitmask & RIM_INFO_REQ_PROTOCOL_VER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_req->ver, p_src + *p_length_read, "ver");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_req->ver < 1) || (p_rim_information_req->ver > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_req->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_req->ver);
        return RRC_FAILURE;
    }
    }

    if (p_rim_information_req->bitmask & RIM_INFO_REQ_APP_CONTAINER_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_container(
        &p_rim_information_req->app_cont,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_information_req->bitmask & RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_son_transfer_app_idty(
        &p_rim_information_req->son_app_idty,
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
rrc_il_parse_rim_plmn
(
    rim_plmn_t *p_rim_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_plmn, 0, sizeof(rim_plmn_t));

    /* This function parses rim_plmn */

    if (*p_length_read + (S32)sizeof(p_rim_plmn->plmn) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_plmn->plmn); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_plmn->plmn[loop], (void*)(p_src + *p_length_read), "plmn[]");
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
rrc_il_parse_rim_routing_identification
(
    rim_routing_identification_t *p_rim_routing_identification,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_routing_identification, 0, sizeof(rim_routing_identification_t));

    /* This function parses rim_routing_identification */

    if (RRC_FAILURE == rrc_il_parse_rim_plmn(
        &p_rim_routing_identification->plmn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rim_routing_identification->lac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_routing_identification->lac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_routing_identification->lac[loop], (void*)(p_src + *p_length_read), "lac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_routing_identification->rac, p_src + *p_length_read, "rac");
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
rrc_il_parse_rim_geran_routing_addr
(
    rim_geran_routing_addr_t *p_rim_geran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_geran_routing_addr, 0, sizeof(rim_geran_routing_addr_t));

    /* This function parses rim_geran_routing_addr */

    if (RRC_FAILURE == rrc_il_parse_rim_routing_identification(
        &p_rim_geran_routing_addr->routing_idty,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rim_geran_routing_addr->geran_cell_idty) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_geran_routing_addr->geran_cell_idty); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_geran_routing_addr->geran_cell_idty[loop], (void*)(p_src + *p_length_read), "geran_cell_idty[]");
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
rrc_il_parse_rim_utran_routing_addr
(
    rim_utran_routing_addr_t *p_rim_utran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_utran_routing_addr, 0, sizeof(rim_utran_routing_addr_t));

    /* This function parses rim_utran_routing_addr */

    if (RRC_FAILURE == rrc_il_parse_rim_routing_identification(
        &p_rim_utran_routing_addr->routing_idty,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rim_utran_routing_addr->rnc_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_utran_routing_addr->rnc_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_utran_routing_addr->rnc_id[loop], (void*)(p_src + *p_length_read), "rnc_id[]");
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
rrc_il_parse_rim_ta_identity
(
    rim_ta_identity_t *p_rim_ta_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_ta_identity, 0, sizeof(rim_ta_identity_t));

    /* This function parses rim_ta_identity */

    if (RRC_FAILURE == rrc_il_parse_rim_plmn(
        &p_rim_ta_identity->plmn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rim_ta_identity->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_ta_identity->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_ta_identity->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
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
rrc_il_parse_rim_global_enb_id
(
    rim_global_enb_id_t *p_rim_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_global_enb_id, 0, sizeof(rim_global_enb_id_t));

    /* This function parses rim_global_enb_id */

    if (RRC_FAILURE == rrc_il_parse_rim_plmn(
        &p_rim_global_enb_id->plmn,
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
    rrc_cp_unpack_U8(&p_rim_global_enb_id->enb_type, p_src + *p_length_read, "enb_type");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rim_global_enb_id->enb_id, p_src + *p_length_read, "enb_id");
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
rrc_il_parse_rim_eutran_routing_addr
(
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_eutran_routing_addr, 0, sizeof(rim_eutran_routing_addr_t));

    /* This function parses rim_eutran_routing_addr */

    if (RRC_FAILURE == rrc_il_parse_rim_ta_identity(
        &p_rim_eutran_routing_addr->ta_idty,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_global_enb_id(
        &p_rim_eutran_routing_addr->enb,
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
rrc_il_parse_rim_routing_info
(
    rim_routing_info_t *p_rim_routing_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_routing_info, 0, sizeof(rim_routing_info_t));

    /* This function parses rim_routing_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_routing_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rim_routing_info->bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_geran_routing_addr(
        &p_rim_routing_info->geran_rtng_addr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_routing_info->bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_utran_routing_addr(
        &p_rim_routing_info->utran_rtng_addr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_routing_info->bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_eutran_routing_addr(
        &p_rim_routing_info->eutran_rtng_addr,
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
rrc_il_parse_rim_pdu_ind_rim_info_req
(
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_pdu_ind_rim_info_req, 0, sizeof(rim_pdu_ind_rim_info_req_t));

    /* This function parses rim_pdu_ind_rim_info_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_info_req->pdu_ext, p_src + *p_length_read, "pdu_ext");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info_req->pdu_ext > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_info_req->pdu_ext] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rim_pdu_ind_rim_info_req->pdu_ext);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_info_req->ack, p_src + *p_length_read, "ack");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info_req->ack > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_info_req->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_info_req->ack);
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
rrc_il_parse_rim_req_app_contrn_nacc
(
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_contrn_nacc, 0, sizeof(rim_req_app_contrn_nacc_t));

    /* This function parses rim_req_app_contrn_nacc */

    if (RRC_FAILURE == rrc_il_parse_rim_geran_routing_addr(
        &p_rim_req_app_contrn_nacc->cell_idty,
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
rrc_il_parse_rim_req_app_contrn_si3
(
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_contrn_si3, 0, sizeof(rim_req_app_contrn_si3_t));

    /* This function parses rim_req_app_contrn_si3 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_req_app_contrn_si3->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_req_app_contrn_mbms
(
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_contrn_mbms, 0, sizeof(rim_req_app_contrn_mbms_t));

    /* This function parses rim_req_app_contrn_mbms */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_req_app_contrn_mbms->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_req_app_contrn_son
(
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_contrn_son, 0, sizeof(rim_req_app_contrn_son_t));

    /* This function parses rim_req_app_contrn_son */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_req_app_contrn_son->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_utran_cell_idty
(
    rim_utran_cell_idty_t *p_rim_utran_cell_idty,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_utran_cell_idty, 0, sizeof(rim_utran_cell_idty_t));

    /* This function parses rim_utran_cell_idty */

    if (RRC_FAILURE == rrc_il_parse_rim_plmn(
        &p_rim_utran_cell_idty->plmn,
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
    rrc_cp_unpack_U32(&p_rim_utran_cell_idty->cell_id, p_src + *p_length_read, "cell_id");
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
rrc_il_parse_rim_req_app_contrn_utra_si
(
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_contrn_utra_si, 0, sizeof(rim_req_app_contrn_utra_si_t));

    /* This function parses rim_req_app_contrn_utra_si */

    if (RRC_FAILURE == rrc_il_parse_rim_utran_cell_idty(
        &p_rim_req_app_contrn_utra_si->rpt_cell_id,
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
rrc_il_parse_rim_req_app_container
(
    rim_req_app_container_t *p_rim_req_app_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_req_app_container, 0, sizeof(rim_req_app_container_t));

    /* This function parses rim_req_app_container */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_req_app_container->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_NACC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_nacc(
        &p_rim_req_app_container->nacc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SI3_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_si3(
        &p_rim_req_app_container->si3,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_MBMS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_mbms(
        &p_rim_req_app_container->mbms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SON_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_son(
        &p_rim_req_app_container->son,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_utra_si(
        &p_rim_req_app_container->utra_si,
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
rrc_il_parse_rim_son_transfer_app_idty
(
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_son_transfer_app_idty, 0, sizeof(rim_son_transfer_app_idty_t));

    /* This function parses rim_son_transfer_app_idty */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_son_transfer_app_idty->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rim_information
(
    rim_information_t *p_rim_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information, 0, sizeof(rim_information_t));

    /* This function parses rim_information */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_information->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information->dst_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information->src_cell,
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
    rrc_cp_unpack_U8(&p_rim_information->app_idty, p_src + *p_length_read, "app_idty");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information->app_idty < 1) || (p_rim_information->app_idty > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information->app_idty);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rim_information->seq_num, p_src + *p_length_read, "seq_num");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_rim_pdu_ind_rim_info(
        &p_rim_information->pdu_ind,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rim_information->bitmask & RIM_INFO_PROTOCOL_VER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information->ver, p_src + *p_length_read, "ver");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information->ver < 1) || (p_rim_information->ver > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information->ver);
        return RRC_FAILURE;
    }
    }

    if (p_rim_information->bitmask & RIM_INFO_APP_CONTAINER_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_container(
        &p_rim_information->app_cont,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_information->bitmask & RIM_INFO_APP_ERR_CONTAINER_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_container(
        &p_rim_information->err_cont,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_information->bitmask & RIM_INFO_SON_TRANSFER_APP_IDNTY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_son_transfer_app_idty(
        &p_rim_information->son_app_idty,
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
rrc_il_parse_rim_pdu_ind_rim_info
(
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_pdu_ind_rim_info, 0, sizeof(rim_pdu_ind_rim_info_t));

    /* This function parses rim_pdu_ind_rim_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_info->pdu_ext, p_src + *p_length_read, "pdu_ext");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info->pdu_ext > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_info->pdu_ext] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rim_pdu_ind_rim_info->pdu_ext);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_info->ack, p_src + *p_length_read, "ack");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info->ack > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_info->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_info->ack);
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
rrc_il_parse_rim_si_psi_individual
(
    rim_si_psi_individual_t *p_rim_si_psi_individual,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_si_psi_individual, 0, sizeof(rim_si_psi_individual_t));

    /* This function parses rim_si_psi_individual */

    if (*p_length_read + (S32)sizeof(p_rim_si_psi_individual->si_psi) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_si_psi_individual->si_psi); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_si_psi_individual->si_psi[loop], (void*)(p_src + *p_length_read), "si_psi[]");
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
rrc_il_parse_rim_info_app_contrn_nacc
(
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_contrn_nacc, 0, sizeof(rim_info_app_contrn_nacc_t));

    /* This function parses rim_info_app_contrn_nacc */

    if (RRC_FAILURE == rrc_il_parse_rim_geran_routing_addr(
        &p_rim_info_app_contrn_nacc->cell_idty,
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
    rrc_cp_unpack_U8(&p_rim_info_app_contrn_nacc->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_nacc->type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_info_app_contrn_nacc->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_info_app_contrn_nacc->type);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_info_app_contrn_nacc->num_si_psi, p_src + *p_length_read, "num_si_psi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_nacc->num_si_psi > RIM_MAX_SI_PSI_PAYLOAD))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_info_app_contrn_nacc->num_si_psi] should be less than"
            " or equal to RIM_MAX_SI_PSI_PAYLOAD. Incorrect value %u received.", p_rim_info_app_contrn_nacc->num_si_psi);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_contrn_nacc->num_si_psi; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rim_si_psi_individual(
                &p_rim_info_app_contrn_nacc->si_psi_indiv[loop],
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
rrc_il_parse_rim_info_app_contrn_si3
(
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_contrn_si3, 0, sizeof(rim_info_app_contrn_si3_t));

    /* This function parses rim_info_app_contrn_si3 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_info_app_contrn_si3->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_info_app_contrn_mbms
(
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_contrn_mbms, 0, sizeof(rim_info_app_contrn_mbms_t));

    /* This function parses rim_info_app_contrn_mbms */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_info_app_contrn_mbms->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_info_app_contrn_son
(
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_contrn_son, 0, sizeof(rim_info_app_contrn_son_t));

    /* This function parses rim_info_app_contrn_son */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_info_app_contrn_son->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_info_app_contrn_utra_si
(
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_contrn_utra_si, 0, sizeof(rim_info_app_contrn_utra_si_t));

    /* This function parses rim_info_app_contrn_utra_si */

    if (RRC_FAILURE == rrc_il_parse_rim_utran_cell_idty(
        &p_rim_info_app_contrn_utra_si->rpt_cell_id,
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
    rrc_cp_unpack_U16(&p_rim_info_app_contrn_utra_si->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_utra_si->len > RIM_MAX_UTRA_SI_PAYLOAD))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_info_app_contrn_utra_si->len] should be less than"
            " or equal to RIM_MAX_UTRA_SI_PAYLOAD. Incorrect value %u received.", p_rim_info_app_contrn_utra_si->len);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rim_info_app_contrn_utra_si->len * sizeof(p_rim_info_app_contrn_utra_si->si[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_contrn_utra_si->len; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rim_info_app_contrn_utra_si->si[loop], (void*)(p_src + *p_length_read), "si[]");
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
rrc_il_parse_rim_info_app_container
(
    rim_info_app_container_t *p_rim_info_app_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_info_app_container, 0, sizeof(rim_info_app_container_t));

    /* This function parses rim_info_app_container */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_info_app_container->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_NACC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_nacc(
        &p_rim_info_app_container->nacc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SI3_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_si3(
        &p_rim_info_app_container->si3,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_MBMS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_mbms(
        &p_rim_info_app_container->mbms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SON_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_son(
        &p_rim_info_app_container->son,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_utra_si(
        &p_rim_info_app_container->utra_si,
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
rrc_il_parse_rim_app_err_contrn_nacc
(
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_contrn_nacc, 0, sizeof(rim_app_err_contrn_nacc_t));

    /* This function parses rim_app_err_contrn_nacc */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_contrn_nacc->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_app_err_contrn_nacc->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_app_err_contrn_nacc->cause > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_app_err_contrn_nacc->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rim_app_err_contrn_nacc->cause);
        return RRC_FAILURE;
    }

    if (p_rim_app_err_contrn_nacc->bitmask & RIM_REQ_NACC_PDU_IN_ERROR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_nacc(
        &p_rim_app_err_contrn_nacc->req_err_pdu,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_contrn_nacc->bitmask & RIM_INFO_NACC_PDU_IN_ERROR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_nacc(
        &p_rim_app_err_contrn_nacc->info_err_pdu,
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
rrc_il_parse_rim_app_err_contrn_si3
(
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_contrn_si3, 0, sizeof(rim_app_err_contrn_si3_t));

    /* This function parses rim_app_err_contrn_si3 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_contrn_si3->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_app_err_contrn_mbms
(
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_contrn_mbms, 0, sizeof(rim_app_err_contrn_mbms_t));

    /* This function parses rim_app_err_contrn_mbms */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_contrn_mbms->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_app_err_contrn_son
(
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_contrn_son, 0, sizeof(rim_app_err_contrn_son_t));

    /* This function parses rim_app_err_contrn_son */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_contrn_son->bitmask, p_src + *p_length_read, "bitmask");
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
rrc_il_parse_rim_app_err_contrn_utra_si
(
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_contrn_utra_si, 0, sizeof(rim_app_err_contrn_utra_si_t));

    /* This function parses rim_app_err_contrn_utra_si */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_contrn_utra_si->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_app_err_contrn_utra_si->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_app_err_contrn_utra_si->cause > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_app_err_contrn_utra_si->cause] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rim_app_err_contrn_utra_si->cause);
        return RRC_FAILURE;
    }

    if (p_rim_app_err_contrn_utra_si->bitmask & RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_req_app_contrn_utra_si(
        &p_rim_app_err_contrn_utra_si->req_err_pdu,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_contrn_utra_si->bitmask & RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_info_app_contrn_utra_si(
        &p_rim_app_err_contrn_utra_si->info_err_pdu,
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
rrc_il_parse_rim_app_err_container
(
    rim_app_err_container_t *p_rim_app_err_container,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_app_err_container, 0, sizeof(rim_app_err_container_t));

    /* This function parses rim_app_err_container */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_app_err_container->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_NACC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_contrn_nacc(
        &p_rim_app_err_container->nacc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SI3_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_contrn_si3(
        &p_rim_app_err_container->si3,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_MBMS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_contrn_mbms(
        &p_rim_app_err_container->mbms,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SON_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_contrn_son(
        &p_rim_app_err_container->son,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_contrn_utra_si(
        &p_rim_app_err_container->utra_si,
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
rrc_il_parse_rim_information_ack
(
    rim_information_ack_t *p_rim_information_ack,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information_ack, 0, sizeof(rim_information_ack_t));

    /* This function parses rim_information_ack */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_information_ack->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_ack->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_ack->dst_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_ack->src_cell,
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
    rrc_cp_unpack_U8(&p_rim_information_ack->app_idty, p_src + *p_length_read, "app_idty");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_ack->app_idty < 1) || (p_rim_information_ack->app_idty > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_ack->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_ack->app_idty);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rim_information_ack->seq_num, p_src + *p_length_read, "seq_num");
    *p_length_read += sizeof(U32);

    if (p_rim_information_ack->bitmask & RIM_INFO_ACK_PROTOCOL_VER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_ack->ver, p_src + *p_length_read, "ver");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_ack->ver < 1) || (p_rim_information_ack->ver > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_ack->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_ack->ver);
        return RRC_FAILURE;
    }
    }

    if (p_rim_information_ack->bitmask & RIM_INFO_ACK_SON_TRNF_APP_IDNTY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_son_transfer_app_idty(
        &p_rim_information_ack->son_app_idty,
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
rrc_il_parse_rim_information_app_err
(
    rim_information_app_err_t *p_rim_information_app_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information_app_err, 0, sizeof(rim_information_app_err_t));

    /* This function parses rim_information_app_err */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_information_app_err->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_app_err->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_app_err->dst_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_app_err->src_cell,
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
    rrc_cp_unpack_U8(&p_rim_information_app_err->app_idty, p_src + *p_length_read, "app_idty");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rim_information_app_err->seq_num, p_src + *p_length_read, "seq_num");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_rim_pdu_ind_rim_app_err(
        &p_rim_information_app_err->pdu_ind,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_app_err->ver, p_src + *p_length_read, "ver");
    *p_length_read += sizeof(U8);
    }

    if (RRC_FAILURE == rrc_il_parse_rim_app_err_container(
        &p_rim_information_app_err->err_cont,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_son_transfer_app_idty(
        &p_rim_information_app_err->son_app_idty,
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
rrc_il_parse_rim_pdu_ind_rim_app_err
(
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_pdu_ind_rim_app_err, 0, sizeof(rim_pdu_ind_rim_app_err_t));

    /* This function parses rim_pdu_ind_rim_app_err */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_app_err->pdu_ext, p_src + *p_length_read, "pdu_ext");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_pdu_ind_rim_app_err->pdu_ext < 1) || (p_rim_pdu_ind_rim_app_err->pdu_ext > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_app_err->pdu_ext] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_app_err->pdu_ext);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_pdu_ind_rim_app_err->ack, p_src + *p_length_read, "ack");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_app_err->ack > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_pdu_ind_rim_app_err->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_app_err->ack);
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
rrc_il_parse_rim_information_err
(
    rim_information_err_t *p_rim_information_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information_err, 0, sizeof(rim_information_err_t));

    /* This function parses rim_information_err */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_information_err->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_err->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_err->dst_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
        &p_rim_information_err->src_cell,
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
    rrc_cp_unpack_U8(&p_rim_information_err->app_idty, p_src + *p_length_read, "app_idty");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_err->app_idty < 1) || (p_rim_information_err->app_idty > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_err->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_err->app_idty);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_err->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_information_err->cause > 75))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_err->cause] should be less than"
            " or equal to 75. Incorrect value %u received.", p_rim_information_err->cause);
        return RRC_FAILURE;
    }

    if (p_rim_information_err->bitmask & RIM_INFO_ERR_PROTOCOL_VER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_information_err->ver, p_src + *p_length_read, "ver");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_err->ver < 1) || (p_rim_information_err->ver > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rim_information_err->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_err->ver);
        return RRC_FAILURE;
    }
    }

    if (p_rim_information_err->bitmask & RIM_INFO_PDU_IN_ERROR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_pdu_in_error(
        &p_rim_information_err->err_pdu,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rim_information_err->bitmask & RIM_INFO_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rim_son_transfer_app_idty(
        &p_rim_information_err->son_app_idty,
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
rrc_il_parse_rim_pdu_in_error
(
    rim_pdu_in_error_t *p_rim_pdu_in_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rim_pdu_in_error, 0, sizeof(rim_pdu_in_error_t));

    /* This function parses rim_pdu_in_error */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rim_pdu_in_error->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_rrm_mme_info
(
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_mme_info, 0, sizeof(s1ap_rrm_mme_info_t));

    /* This function parses s1ap_rrm_mme_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_mme_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_mme_info->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(p_s1ap_rrm_mme_info->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_info->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_rrm_mme_info->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_mme_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_s1ap_mme_info(
        &p_s1ap_rrm_mme_info->mme_info,
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
rrc_il_parse_s1ap_served_group_id
(
    s1ap_served_group_id_t *p_s1ap_served_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_group_id, 0, sizeof(s1ap_served_group_id_t));

    /* This function parses s1ap_served_group_id */

    if (*p_length_read + (S32)sizeof(p_s1ap_served_group_id->mmegi) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_served_group_id->mmegi); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_served_group_id->mmegi[loop], (void*)(p_src + *p_length_read), "mmegi[]");
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
rrc_il_parse_s1ap_served_mmec
(
    s1ap_served_mmec_t *p_s1ap_served_mmec,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_mmec, 0, sizeof(s1ap_served_mmec_t));

    /* This function parses s1ap_served_mmec */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_served_mmec->mmec, p_src + *p_length_read, "mmec");
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
rrc_il_parse_s1ap_served_gummei_info
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_gummei_info, 0, sizeof(s1ap_served_gummei_info_t));

    /* This function parses s1ap_served_gummei_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_served_gummei_info->num_served_plmns, p_src + *p_length_read, "num_served_plmns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
                &p_s1ap_served_gummei_info->plmn_identity[loop],
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
    rrc_cp_unpack_U16(&p_s1ap_served_gummei_info->num_served_group_ids, p_src + *p_length_read, "num_served_group_ids");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_served_group_id(
                &p_s1ap_served_gummei_info->served_group_id[loop],
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
    rrc_cp_unpack_U16(&p_s1ap_served_gummei_info->num_served_mmecs, p_src + *p_length_read, "num_served_mmecs");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_served_mmec(
                &p_s1ap_served_gummei_info->served_mmec[loop],
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
rrc_il_parse_s1ap_mme_info
(
    s1ap_mme_info_t *p_s1ap_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_mme_info, 0, sizeof(s1ap_mme_info_t));

    /* This function parses s1ap_mme_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_mme_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_mme_info->mme_name) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_mme_info->mme_name); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_mme_info->mme_name[loop], (void*)(p_src + *p_length_read), "mme_name[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_served_gummei_info(
        &p_s1ap_mme_info->served_gummei_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_mme_info->relative_mme_capacity, p_src + *p_length_read, "relative_mme_capacity");
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
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_init_req
(
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_init_req, 0, sizeof(s1ap_rrm_lppa_ecid_meas_init_req_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_init_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_req->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id, p_src + *p_length_read, "e_smlc_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_req->report_char, p_src + *p_length_read, "report_char");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->report_char > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->report_char] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->report_char);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask & LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity, p_src + *p_length_read, "meas_periodicity");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity > 12))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity] should be less than"
            " or equal to 12. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier, p_src + *p_length_read, "num_meas_quantifier");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier > MAX_NO_OF_MEAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier] should be in range "
            "1 to MAX_NO_OF_MEAS. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_meas_quantifier(
                &p_s1ap_rrm_lppa_ecid_meas_init_req->meas_quantifier[loop],
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
rrc_il_parse_lppa_meas_quantifier
(
    lppa_meas_quantifier_t *p_lppa_meas_quantifier,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_meas_quantifier, 0, sizeof(lppa_meas_quantifier_t));

    /* This function parses lppa_meas_quantifier */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_meas_quantifier->meas_quantity_item, p_src + *p_length_read, "meas_quantity_item");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_meas_quantifier->meas_quantity_item > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_meas_quantifier->meas_quantity_item] should be less than"
            " or equal to 5. Incorrect value %u received.", p_lppa_meas_quantifier->meas_quantity_item);
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
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_init_resp
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_init_resp, 0, sizeof(s1ap_rrm_lppa_ecid_meas_init_resp_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_init_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_resp->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id, p_src + *p_length_read, "e_smlc_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id, p_src + *p_length_read, "enb_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask & LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_e_cid_meas_result(
        &p_s1ap_rrm_lppa_ecid_meas_init_resp->e_cid_meas_result,
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
rrc_il_parse_lppa_plmn_identity
(
    lppa_plmn_identity_t *p_lppa_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_plmn_identity, 0, sizeof(lppa_plmn_identity_t));

    /* This function parses lppa_plmn_identity */

    if (*p_length_read + (S32)sizeof(p_lppa_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
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
rrc_il_parse_lppa_ecgi
(
    lppa_ecgi_t *p_lppa_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_ecgi, 0, sizeof(lppa_ecgi_t));

    /* This function parses lppa_ecgi */

    if (RRC_FAILURE == rrc_il_parse_lppa_plmn_identity(
        &p_lppa_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_lppa_ecgi->eutran_cell_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_ecgi->eutran_cell_id[loop], (void*)(p_src + *p_length_read), "eutran_cell_id[]");
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
rrc_il_parse_lppa_e_utran_access_point_pos
(
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_e_utran_access_point_pos, 0, sizeof(lppa_e_utran_access_point_pos_t));

    /* This function parses lppa_e_utran_access_point_pos */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_e_utran_access_point_pos->latitude_sign, p_src + *p_length_read, "latitude_sign");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->latitude_sign > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->latitude_sign] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->latitude_sign);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_e_utran_access_point_pos->deg_of_latitude, p_src + *p_length_read, "deg_of_latitude");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_latitude > 8388607))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_latitude] should be less than"
            " or equal to 8388607. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->deg_of_latitude);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S32(&p_lppa_e_utran_access_point_pos->deg_of_longitude, p_src + *p_length_read, "deg_of_longitude");
    *p_length_read += sizeof(S32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_longitude < -8388608) || (p_lppa_e_utran_access_point_pos->deg_of_longitude > 8388607))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_longitude] should be in range "
            "-8388608 to 8388607. Incorrect value %d received.", p_lppa_e_utran_access_point_pos->deg_of_longitude);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_e_utran_access_point_pos->deg_of_altitude, p_src + *p_length_read, "deg_of_altitude");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_altitude > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_altitude] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->deg_of_altitude);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_e_utran_access_point_pos->altitude, p_src + *p_length_read, "altitude");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->altitude > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->altitude] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->altitude);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_e_utran_access_point_pos->uncertainty_semi_major, p_src + *p_length_read, "uncertainty_semi_major");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_semi_major > 127))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_semi_major] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_semi_major);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_e_utran_access_point_pos->uncertainty_semi_minor, p_src + *p_length_read, "uncertainty_semi_minor");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_semi_minor > 127))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_semi_minor] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_semi_minor);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_e_utran_access_point_pos->orientation_of_major_axis, p_src + *p_length_read, "orientation_of_major_axis");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->orientation_of_major_axis > 179))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->orientation_of_major_axis] should be less than"
            " or equal to 179. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->orientation_of_major_axis);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_e_utran_access_point_pos->uncertainty_altitude, p_src + *p_length_read, "uncertainty_altitude");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_altitude > 127))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_altitude] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_altitude);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_e_utran_access_point_pos->confidence, p_src + *p_length_read, "confidence");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->confidence > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->confidence] should be less than"
            " or equal to 100. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->confidence);
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
rrc_il_parse_lppa_result_rsrp
(
    lppa_result_rsrp_t *p_lppa_result_rsrp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_result_rsrp, 0, sizeof(lppa_result_rsrp_t));

    /* This function parses lppa_result_rsrp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_result_rsrp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrp->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrp->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_result_rsrp->pci);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrp->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->earfcn > 65535))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrp->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_result_rsrp->earfcn);
        return RRC_FAILURE;
    }

    if (p_lppa_result_rsrp->bitmask & LPPA_RESULT_RSRP_ECGI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_ecgi(
        &p_lppa_result_rsrp->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrp->value_rsrp, p_src + *p_length_read, "value_rsrp");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->value_rsrp > 97))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrp->value_rsrp] should be less than"
            " or equal to 97. Incorrect value %u received.", p_lppa_result_rsrp->value_rsrp);
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
rrc_il_parse_lppa_meas_result_rsrp
(
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_meas_result_rsrp, 0, sizeof(lppa_meas_result_rsrp_t));

    /* This function parses lppa_meas_result_rsrp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_meas_result_rsrp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_meas_result_rsrp->rsrp_count, p_src + *p_length_read, "rsrp_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrp->rsrp_count < 1) || (p_lppa_meas_result_rsrp->rsrp_count > LPPA_MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_meas_result_rsrp->rsrp_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %u received.", p_lppa_meas_result_rsrp->rsrp_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrp->rsrp_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_result_rsrp(
                &p_lppa_meas_result_rsrp->result_rsrp[loop],
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
rrc_il_parse_lppa_result_rsrq
(
    lppa_result_rsrq_t *p_lppa_result_rsrq,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_result_rsrq, 0, sizeof(lppa_result_rsrq_t));

    /* This function parses lppa_result_rsrq */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_result_rsrq->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrq->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrq->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_result_rsrq->pci);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrq->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->earfcn > 65535))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrq->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_result_rsrq->earfcn);
        return RRC_FAILURE;
    }

    if (p_lppa_result_rsrq->bitmask & LPPA_RESULT_RSRQ_ECGI_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_ecgi(
        &p_lppa_result_rsrq->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_result_rsrq->value_rsrq, p_src + *p_length_read, "value_rsrq");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->value_rsrq > 34))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_result_rsrq->value_rsrq] should be less than"
            " or equal to 34. Incorrect value %u received.", p_lppa_result_rsrq->value_rsrq);
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
rrc_il_parse_lppa_meas_result_rsrq
(
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_meas_result_rsrq, 0, sizeof(lppa_meas_result_rsrq_t));

    /* This function parses lppa_meas_result_rsrq */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_meas_result_rsrq->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_meas_result_rsrq->rsrq_count, p_src + *p_length_read, "rsrq_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrq->rsrq_count < 1) || (p_lppa_meas_result_rsrq->rsrq_count > LPPA_MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_meas_result_rsrq->rsrq_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %u received.", p_lppa_meas_result_rsrq->rsrq_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrq->rsrq_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_result_rsrq(
                &p_lppa_meas_result_rsrq->result_rsrq[loop],
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
rrc_il_parse_lppa_measured_res
(
    lppa_measured_res_t *p_lppa_measured_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_measured_res, 0, sizeof(lppa_measured_res_t));

    /* This function parses lppa_measured_res */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_measured_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_measured_res->value_angle_of_arrival, p_src + *p_length_read, "value_angle_of_arrival");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_angle_of_arrival > 719))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_measured_res->value_angle_of_arrival] should be less than"
            " or equal to 719. Incorrect value %u received.", p_lppa_measured_res->value_angle_of_arrival);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_measured_res->value_timing_adv_type_1, p_src + *p_length_read, "value_timing_adv_type_1");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_timing_adv_type_1 > 7690))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_measured_res->value_timing_adv_type_1] should be less than"
            " or equal to 7690. Incorrect value %u received.", p_lppa_measured_res->value_timing_adv_type_1);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_measured_res->value_timing_adv_type_2, p_src + *p_length_read, "value_timing_adv_type_2");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_timing_adv_type_2 > 7690))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_measured_res->value_timing_adv_type_2] should be less than"
            " or equal to 7690. Incorrect value %u received.", p_lppa_measured_res->value_timing_adv_type_2);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_meas_result_rsrp(
        &p_lppa_measured_res->meas_result_rsrp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_meas_result_rsrq(
        &p_lppa_measured_res->meas_result_rsrq,
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
rrc_il_parse_lppa_meas_results
(
    lppa_meas_results_t *p_lppa_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_meas_results, 0, sizeof(lppa_meas_results_t));

    /* This function parses lppa_meas_results */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_meas_results->num_measured_results, p_src + *p_length_read, "num_measured_results");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_meas_results->num_measured_results > MAX_NO_OF_MEAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_meas_results->num_measured_results] should be less than"
            " or equal to MAX_NO_OF_MEAS. Incorrect value %u received.", p_lppa_meas_results->num_measured_results);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_results->num_measured_results; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_measured_res(
                &p_lppa_meas_results->measured_results[loop],
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
rrc_il_parse_lppa_e_cid_meas_result
(
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_e_cid_meas_result, 0, sizeof(lppa_e_cid_meas_result_t));

    /* This function parses lppa_e_cid_meas_result */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_e_cid_meas_result->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_lppa_ecgi(
        &p_lppa_e_cid_meas_result->serving_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_lppa_e_cid_meas_result->serving_cell_tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_e_cid_meas_result->serving_cell_tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_e_cid_meas_result->serving_cell_tac[loop], (void*)(p_src + *p_length_read), "serving_cell_tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_lppa_e_cid_meas_result->bitmask & LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_e_utran_access_point_pos(
        &p_lppa_e_cid_meas_result->e_utran_access_point_pos,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_e_cid_meas_result->bitmask & LPPA_MEASUREMENT_RESULT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_meas_results(
        &p_lppa_e_cid_meas_result->meas_result,
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
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_init_fail
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_init_fail, 0, sizeof(s1ap_rrm_lppa_ecid_meas_init_fail_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_init_fail */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_fail->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_init_fail->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_init_fail->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id, p_src + *p_length_read, "e_smlc_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_lppa_cause(
        &p_s1ap_rrm_lppa_ecid_meas_init_fail->cause,
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
rrc_il_parse_lppa_cause
(
    lppa_cause_t *p_lppa_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_cause, 0, sizeof(lppa_cause_t));

    /* This function parses lppa_cause */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_cause->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_cause->value, p_src + *p_length_read, "value");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_fail_ind, 0, sizeof(s1ap_rrm_lppa_ecid_meas_fail_ind_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_fail_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id, p_src + *p_length_read, "e_smlc_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id, p_src + *p_length_read, "enb_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_lppa_cause(
        &p_s1ap_rrm_lppa_ecid_meas_fail_ind->cause,
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
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_report
(
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_report, 0, sizeof(s1ap_rrm_lppa_ecid_meas_report_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_report */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_report->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_report->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_report->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_report->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id, p_src + *p_length_read, "e_smlc_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id, p_src + *p_length_read, "enb_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_lppa_e_cid_meas_result(
        &p_s1ap_rrm_lppa_ecid_meas_report->e_cid_meas_result,
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
rrc_il_parse_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_ecid_meas_termination_cmd, 0, sizeof(s1ap_rrm_lppa_ecid_meas_termination_cmd_t));

    /* This function parses s1ap_rrm_lppa_ecid_meas_termination_cmd */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id, p_src + *p_length_read, "e_smlc_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id, p_src + *p_length_read, "enb_ue_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id);
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
rrc_il_parse_s1ap_rrm_lppa_otdoa_info_req
(
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_otdoa_info_req, 0, sizeof(s1ap_rrm_lppa_otdoa_info_req_t));

    /* This function parses s1ap_rrm_lppa_otdoa_info_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_otdoa_info_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_req->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_req->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count, p_src + *p_length_read, "otdoa_info_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count > MAX_NO_OTDA_TYPES))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count] should be less than"
            " or equal to MAX_NO_OTDA_TYPES. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_otdoa_info_type(
                &p_s1ap_rrm_lppa_otdoa_info_req->otda_info_type_arr[loop],
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
rrc_il_parse_lppa_otdoa_info_type
(
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_otdoa_info_type, 0, sizeof(lppa_otdoa_info_type_t));

    /* This function parses lppa_otdoa_info_type */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_otdoa_info_type->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_info_type->otdoa_info_item, p_src + *p_length_read, "otdoa_info_item");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_info_type->otdoa_info_item > 11))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_info_type->otdoa_info_item] should be less than"
            " or equal to 11. Incorrect value %u received.", p_lppa_otdoa_info_type->otdoa_info_item);
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
rrc_il_parse_s1ap_rrm_lppa_otdoa_info_resp
(
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_otdoa_info_resp, 0, sizeof(s1ap_rrm_lppa_otdoa_info_resp_t));

    /* This function parses s1ap_rrm_lppa_otdoa_info_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_otdoa_info_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_resp->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_resp->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count, p_src + *p_length_read, "otdoa_cell_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count < 1) || (p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count > MAX_NUM_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_otdoa_cells(
                &p_s1ap_rrm_lppa_otdoa_info_resp->otda_cells_arr[loop],
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
rrc_il_parse_lppa_otdoa_cells_tac
(
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_otdoa_cells_tac, 0, sizeof(lppa_otdoa_cells_tac_t));

    /* This function parses lppa_otdoa_cells_tac */

    if (*p_length_read + (S32)sizeof(p_lppa_otdoa_cells_tac->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_otdoa_cells_tac->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_otdoa_cells_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
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
rrc_il_parse_lppa_otdoa_sfn_init_time
(
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_otdoa_sfn_init_time, 0, sizeof(lppa_otdoa_sfn_init_time_t));

    /* This function parses lppa_otdoa_sfn_init_time */

    if (*p_length_read + (S32)sizeof(p_lppa_otdoa_sfn_init_time->sfn_initiation_time) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_otdoa_sfn_init_time->sfn_initiation_time); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_otdoa_sfn_init_time->sfn_initiation_time[loop], (void*)(p_src + *p_length_read), "sfn_initiation_time[]");
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
rrc_il_parse_lppa_muting_config_two
(
    lppa_muting_config_two_t *p_lppa_muting_config_two,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_muting_config_two, 0, sizeof(lppa_muting_config_two_t));

    /* This function parses lppa_muting_config_two */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_muting_config_two->two, p_src + *p_length_read, "two");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_muting_config_two->two > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_muting_config_two->two] should be less than"
            " or equal to 4. Incorrect value %u received.", p_lppa_muting_config_two->two);
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
rrc_il_parse_lppa_muting_config_four
(
    lppa_muting_config_four_t *p_lppa_muting_config_four,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_muting_config_four, 0, sizeof(lppa_muting_config_four_t));

    /* This function parses lppa_muting_config_four */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_muting_config_four->four, p_src + *p_length_read, "four");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_muting_config_four->four > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_muting_config_four->four] should be less than"
            " or equal to 15. Incorrect value %u received.", p_lppa_muting_config_four->four);
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
rrc_il_parse_lppa_muting_config_eight
(
    lppa_muting_config_eight_t *p_lppa_muting_config_eight,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_muting_config_eight, 0, sizeof(lppa_muting_config_eight_t));

    /* This function parses lppa_muting_config_eight */

    if (*p_length_read + (S32)sizeof(p_lppa_muting_config_eight->eight) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_muting_config_eight->eight); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_muting_config_eight->eight[loop], (void*)(p_src + *p_length_read), "eight[]");
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
rrc_il_parse_lppa_muting_config_sixteen
(
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_muting_config_sixteen, 0, sizeof(lppa_muting_config_sixteen_t));

    /* This function parses lppa_muting_config_sixteen */

    if (*p_length_read + (S32)sizeof(p_lppa_muting_config_sixteen->sixteen) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_muting_config_sixteen->sixteen); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_lppa_muting_config_sixteen->sixteen[loop], (void*)(p_src + *p_length_read), "sixteen[]");
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
rrc_il_parse_lppa_prs_muting_config
(
    lppa_prs_muting_config_t *p_lppa_prs_muting_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_prs_muting_config, 0, sizeof(lppa_prs_muting_config_t));

    /* This function parses lppa_prs_muting_config */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_prs_muting_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_TWO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_muting_config_two(
        &p_lppa_prs_muting_config->muting_config_two,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_muting_config_four(
        &p_lppa_prs_muting_config->muting_config_four,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_muting_config_eight(
        &p_lppa_prs_muting_config->muting_config_eight,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_muting_config_sixteen(
        &p_lppa_prs_muting_config->muting_config_sixteen,
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
rrc_il_parse_lppa_otdoa_cells_info
(
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_otdoa_cells_info, 0, sizeof(lppa_otdoa_cells_info_t));

    /* This function parses lppa_otdoa_cells_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PCI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_otdoa_cells_info->pci);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CELL_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_ecgi(
        &p_lppa_otdoa_cells_info->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_TAC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_otdoa_cells_tac(
        &p_lppa_otdoa_cells_info->otdoa_cells_tac,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_EARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->earfcn > 65535))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_otdoa_cells_info->earfcn);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->prs_band_width, p_src + *p_length_read, "prs_band_width");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->prs_band_width > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->prs_band_width] should be less than"
            " or equal to 5. Incorrect value %u received.", p_lppa_otdoa_cells_info->prs_band_width);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->prs_config_index, p_src + *p_length_read, "prs_config_index");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->prs_config_index > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->prs_config_index] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_lppa_otdoa_cells_info->prs_config_index);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->cp_length, p_src + *p_length_read, "cp_length");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->cp_length > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->cp_length] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_otdoa_cells_info->cp_length);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->no_of_dl_frames, p_src + *p_length_read, "no_of_dl_frames");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->no_of_dl_frames > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->no_of_dl_frames] should be less than"
            " or equal to 3. Incorrect value %u received.", p_lppa_otdoa_cells_info->no_of_dl_frames);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_otdoa_cells_info->no_of_antenna_ports, p_src + *p_length_read, "no_of_antenna_ports");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->no_of_antenna_ports > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells_info->no_of_antenna_ports] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_otdoa_cells_info->no_of_antenna_ports);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_otdoa_sfn_init_time(
        &p_lppa_otdoa_cells_info->otdoa_sfn_init_time,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_e_utran_access_point_pos(
        &p_lppa_otdoa_cells_info->e_utran_acces_point_pos,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_prs_muting_config(
        &p_lppa_otdoa_cells_info->prs_muting_config,
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
rrc_il_parse_lppa_otdoa_cells
(
    lppa_otdoa_cells_t *p_lppa_otdoa_cells,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_lppa_otdoa_cells, 0, sizeof(lppa_otdoa_cells_t));

    /* This function parses lppa_otdoa_cells */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_otdoa_cells->otdoa_cell_info_count, p_src + *p_length_read, "otdoa_cell_info_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells->otdoa_cell_info_count > MAX_NUM_OTDOA_TYPES))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_otdoa_cells->otdoa_cell_info_count] should be less than"
            " or equal to MAX_NUM_OTDOA_TYPES. Incorrect value %u received.", p_lppa_otdoa_cells->otdoa_cell_info_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_otdoa_cells->otdoa_cell_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_lppa_otdoa_cells_info(
                &p_lppa_otdoa_cells->otda_cells_info_arr[loop],
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
rrc_il_parse_s1ap_rrm_lppa_otdoa_info_fail
(
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_otdoa_info_fail, 0, sizeof(s1ap_rrm_lppa_otdoa_info_fail_t));

    /* This function parses s1ap_rrm_lppa_otdoa_info_fail */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_otdoa_info_fail->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_fail->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_fail->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_fail->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_fail->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_otdoa_info_fail->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_lppa_cause(
        &p_s1ap_rrm_lppa_otdoa_info_fail->cause,
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
rrc_il_parse_s1ap_rrm_lppa_error_ind
(
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_error_ind, 0, sizeof(s1ap_rrm_lppa_error_ind_t));

    /* This function parses s1ap_rrm_lppa_error_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_error_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_error_ind->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind->mme_id);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_error_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_error_ind->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_error_ind->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CAUSE_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_cause(
        &p_s1ap_rrm_lppa_error_ind->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CRITIC_DIAG_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_lppa_criticality_diagno(
        &p_s1ap_rrm_lppa_error_ind->criticality_diagnostics,
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
rrc_il_parse_lppa_criticality_diagno
(
    lppa_criticality_diagno_t *p_lppa_criticality_diagno,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_lppa_criticality_diagno, 0, sizeof(lppa_criticality_diagno_t));

    /* This function parses lppa_criticality_diagno */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_lppa_criticality_diagno->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_lppa_criticality_diagno->procedure_code, p_src + *p_length_read, "procedure_code");
    *p_length_read += sizeof(U8);
    }

    if (p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_criticality_diagno->triggering_msg, p_src + *p_length_read, "triggering_msg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->triggering_msg > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_criticality_diagno->triggering_msg] should be less than"
            " or equal to 2. Incorrect value %u received.", p_lppa_criticality_diagno->triggering_msg);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_criticality_diagno->proc_critic, p_src + *p_length_read, "proc_critic");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->proc_critic > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_criticality_diagno->proc_critic] should be less than"
            " or equal to 2. Incorrect value %u received.", p_lppa_criticality_diagno->proc_critic);
        return RRC_FAILURE;
    }
    }

    if (p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_lppa_criticality_diagno->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_lppa_criticality_diagno->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_lppa_criticality_diagno->lppa_trans_id);
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

rrc_return_et
rrc_il_parse_s1ap_rrm_lppa_error_ind_req
(
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_rrm_lppa_error_ind_req, 0, sizeof(s1ap_rrm_lppa_error_ind_req_t));

    /* This function parses s1ap_rrm_lppa_error_ind_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_error_ind_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_error_ind_req->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind_req->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind_req->mme_id);
        return RRC_FAILURE;
    }

    if (p_s1ap_rrm_lppa_error_ind_req->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_rrm_lppa_error_ind_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_rrm_lppa_error_ind_req->routing_id, p_src + *p_length_read, "routing_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id, p_src + *p_length_read, "lppa_trans_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_lppa_cause(
        &p_s1ap_rrm_lppa_error_ind_req->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_lppa_criticality_diagno(
        &p_s1ap_rrm_lppa_error_ind_req->criticality_diagnostics,
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

