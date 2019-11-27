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
 *  File Description : The file rrm_il_composer.c contains the definitions 
 *                     of rrc-s1apRrm interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "l3_api_compose_parse_utils.h"
#include "s1ap_rrm_composer.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_ext_api.h"
#include "rrm_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrm_length_t
rrm_il_get_mnc_len
(
    mnc_t *p_mnc
);

static
rrm_return_et
rrm_il_compose_mnc
(
    U8  **pp_buffer,
    mnc_t *p_mnc
);

static
rrm_length_t
rrm_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
);

static
rrm_return_et
rrm_il_compose_plmn_identity
(
    U8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
);

static
rrm_length_t
rrm_il_get_rrm_s1ap_lai_info_len
(
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info
);

static
rrm_return_et
rrm_il_compose_rrm_s1ap_lai_info
(
    U8  **pp_buffer,
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info
);

static
rrm_length_t
rrm_il_get_rrm_geran_cell_id_len
(
    rrm_geran_cell_id_t *p_rrm_geran_cell_id
);

static
rrm_return_et
rrm_il_compose_rrm_geran_cell_id
(
    U8  **pp_buffer,
    rrm_geran_cell_id_t *p_rrm_geran_cell_id
);

static
rrm_length_t
rrm_il_get_rrm_lai_info_len
(
    rrm_lai_info_t *p_rrm_lai_info
);

static
rrm_return_et
rrm_il_compose_rrm_lai_info
(
    U8  **pp_buffer,
    rrm_lai_info_t *p_rrm_lai_info
);

static
rrm_length_t
rrm_il_get_rrm_rnc_id_len
(
    rrm_rnc_id_t *p_rrm_rnc_id
);

static
rrm_return_et
rrm_il_compose_rrm_rnc_id
(
    U8  **pp_buffer,
    rrm_rnc_id_t *p_rrm_rnc_id
);

static
rrm_length_t
rrm_il_get_rrm_macro_enb_id_len
(
    rrm_macro_enb_id_t *p_rrm_macro_enb_id
);

static
rrm_return_et
rrm_il_compose_rrm_macro_enb_id
(
    U8  **pp_buffer,
    rrm_macro_enb_id_t *p_rrm_macro_enb_id
);

static
rrm_length_t
rrm_il_get_rrm_home_enb_id_len
(
    rrm_home_enb_id_t *p_rrm_home_enb_id
);

static
rrm_return_et
rrm_il_compose_rrm_home_enb_id
(
    U8  **pp_buffer,
    rrm_home_enb_id_t *p_rrm_home_enb_id
);

static
rrm_length_t
rrm_il_get_rrm_enb_id_info_len
(
    rrm_enb_id_info_t *p_rrm_enb_id_info
);

static
rrm_return_et
rrm_il_compose_rrm_enb_id_info
(
    U8  **pp_buffer,
    rrm_enb_id_info_t *p_rrm_enb_id_info
);

static
rrm_length_t
rrm_il_get_rrm_gb_enb_id_len
(
    rrm_gb_enb_id_t *p_rrm_gb_enb_id
);

static
rrm_return_et
rrm_il_compose_rrm_gb_enb_id
(
    U8  **pp_buffer,
    rrm_gb_enb_id_t *p_rrm_gb_enb_id
);

static
rrm_length_t
rrm_il_get_rrm_supp_ta_len
(
    rrm_supp_ta_t *p_rrm_supp_ta
);

static
rrm_return_et
rrm_il_compose_rrm_supp_ta
(
    U8  **pp_buffer,
    rrm_supp_ta_t *p_rrm_supp_ta
);

static
rrm_length_t
rrm_il_get_ip_address_len
(
    ip_address_t *p_ip_address
);

static
rrm_return_et
rrm_il_compose_ip_address
(
    U8  **pp_buffer,
    ip_address_t *p_ip_address
);

static
rrm_length_t
rrm_il_get_trans_layer_addrs_len
(
    trans_layer_addrs_t *p_trans_layer_addrs
);

static
rrm_return_et
rrm_il_compose_trans_layer_addrs
(
    U8  **pp_buffer,
    trans_layer_addrs_t *p_trans_layer_addrs
);

static
rrm_length_t
rrm_il_get_extend_trans_layer_addrs_len
(
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs
);

static
rrm_return_et
rrm_il_compose_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs
);

static
rrm_length_t
rrm_il_get_s1ap_plmn_identity_len
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
);

static
rrm_return_et
rrm_il_compose_s1ap_plmn_identity
(
    U8  **pp_buffer,
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
);

static
rrm_length_t
rrm_il_get_s1ap_cgi_eutra_len
(
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra
);

static
rrm_return_et
rrm_il_compose_s1ap_cgi_eutra
(
    U8  **pp_buffer,
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra
);

static
rrm_length_t
rrm_il_get_cell_id_list_len
(
    cell_id_list_t *p_cell_id_list
);

static
rrm_return_et
rrm_il_compose_cell_id_list
(
    U8  **pp_buffer,
    cell_id_list_t *p_cell_id_list
);

static
rrm_length_t
rrm_il_get_tracking_area_identifier_len
(
    tracking_area_identifier_t *p_tracking_area_identifier
);

static
rrm_return_et
rrm_il_compose_tracking_area_identifier
(
    U8  **pp_buffer,
    tracking_area_identifier_t *p_tracking_area_identifier
);

static
rrm_length_t
rrm_il_get_tracking_area_id_list_len
(
    tracking_area_id_list_t *p_tracking_area_id_list
);

static
rrm_return_et
rrm_il_compose_tracking_area_id_list
(
    U8  **pp_buffer,
    tracking_area_id_list_t *p_tracking_area_id_list
);

static
rrm_length_t
rrm_il_get_emergency_area_identifier_len
(
    emergency_area_identifier_t *p_emergency_area_identifier
);

static
rrm_return_et
rrm_il_compose_emergency_area_identifier
(
    U8  **pp_buffer,
    emergency_area_identifier_t *p_emergency_area_identifier
);

static
rrm_length_t
rrm_il_get_emergency_area_id_list_len
(
    emergency_area_id_list_t *p_emergency_area_id_list
);

static
rrm_return_et
rrm_il_compose_emergency_area_id_list
(
    U8  **pp_buffer,
    emergency_area_id_list_t *p_emergency_area_id_list
);

static
rrm_length_t
rrm_il_get_warning_area_list_info_len
(
    warning_area_list_info_t *p_warning_area_list_info
);

static
rrm_return_et
rrm_il_compose_warning_area_list_info
(
    U8  **pp_buffer,
    warning_area_list_info_t *p_warning_area_list_info
);

static
rrm_length_t
rrm_il_get_sib_mapping_info_len
(
    sib_mapping_info_t *p_sib_mapping_info
);

static
rrm_return_et
rrm_il_compose_sib_mapping_info
(
    U8  **pp_buffer,
    sib_mapping_info_t *p_sib_mapping_info
);

static
rrm_length_t
rrm_il_get_scheduling_info_len
(
    scheduling_info_t *p_scheduling_info
);

static
rrm_return_et
rrm_il_compose_scheduling_info
(
    U8  **pp_buffer,
    scheduling_info_t *p_scheduling_info
);

static
rrm_length_t
rrm_il_get_scheduling_info_list_len
(
    scheduling_info_list_t *p_scheduling_info_list
);

static
rrm_return_et
rrm_il_compose_scheduling_info_list
(
    U8  **pp_buffer,
    scheduling_info_list_t *p_scheduling_info_list
);

static
rrm_length_t
rrm_il_get_rrc_si_start_offset_len
(
    rrc_si_start_offset_t *p_rrc_si_start_offset
);

static
rrm_return_et
rrm_il_compose_rrc_si_start_offset
(
    U8  **pp_buffer,
    rrc_si_start_offset_t *p_rrc_si_start_offset
);

static
rrm_length_t
rrm_il_get_rrc_si_start_offset_info_len
(
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info
);

static
rrm_return_et
rrm_il_compose_rrc_si_start_offset_info
(
    U8  **pp_buffer,
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info
);

static
rrm_length_t
rrm_il_get_rrc_si_transmission_info_len
(
    rrc_si_transmission_info_t *p_rrc_si_transmission_info
);

static
rrm_return_et
rrm_il_compose_rrc_si_transmission_info
(
    U8  **pp_buffer,
    rrc_si_transmission_info_t *p_rrc_si_transmission_info
);

static
rrm_length_t
rrm_il_get_rrc_si_transmission_info_list_len
(
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
);

static
rrm_return_et
rrm_il_compose_rrc_si_transmission_info_list
(
    U8  **pp_buffer,
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
);

static
rrm_length_t
rrm_il_get_etws_scheduling_info_len
(
    etws_scheduling_info_t *p_etws_scheduling_info
);

static
rrm_return_et
rrm_il_compose_etws_scheduling_info
(
    U8  **pp_buffer,
    etws_scheduling_info_t *p_etws_scheduling_info
);

static
rrm_length_t
rrm_il_get_cmas_scheduling_info_len
(
    cmas_scheduling_info_t *p_cmas_scheduling_info
);

static
rrm_return_et
rrm_il_compose_cmas_scheduling_info
(
    U8  **pp_buffer,
    cmas_scheduling_info_t *p_cmas_scheduling_info
);

static
rrm_length_t
rrm_il_get_cmas_scheduling_info_list_len
(
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list
);

static
rrm_return_et
rrm_il_compose_cmas_scheduling_info_list
(
    U8  **pp_buffer,
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list
);

static
rrm_length_t
rrm_il_get_rrc_rrm_pws_contents_len
(
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents
);

static
rrm_return_et
rrm_il_compose_rrc_rrm_pws_contents
(
    U8  **pp_buffer,
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents
);

static
rrm_length_t
rrm_il_get_rrc_rrm_pws_contents_list_len
(
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list
);

static
rrm_return_et
rrm_il_compose_rrc_rrm_pws_contents_list
(
    U8  **pp_buffer,
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list
);

static
rrm_length_t
rrm_il_get_pws_broadcast_resp_len
(
    pws_broadcast_resp_t *p_pws_broadcast_resp
);

static
rrm_return_et
rrm_il_compose_pws_broadcast_resp
(
    U8  **pp_buffer,
    pws_broadcast_resp_t *p_pws_broadcast_resp
);

static
rrm_length_t
rrm_il_get_pws_broadcast_list_len
(
    pws_broadcast_list_t *p_pws_broadcast_list
);

static
rrm_return_et
rrm_il_compose_pws_broadcast_list
(
    U8  **pp_buffer,
    pws_broadcast_list_t *p_pws_broadcast_list
);

static
rrm_length_t
rrm_il_get_broadcast_cancellation_len
(
    broadcast_cancellation_t *p_broadcast_cancellation
);

static
rrm_return_et
rrm_il_compose_broadcast_cancellation
(
    U8  **pp_buffer,
    broadcast_cancellation_t *p_broadcast_cancellation
);

static
rrm_length_t
rrm_il_get_broadcast_cancellation_list_len
(
    broadcast_cancellation_list_t *p_broadcast_cancellation_list
);

static
rrm_return_et
rrm_il_compose_broadcast_cancellation_list
(
    U8  **pp_buffer,
    broadcast_cancellation_list_t *p_broadcast_cancellation_list
);

static
rrm_length_t
rrm_il_get_rim_plmn_len
(
    rim_plmn_t *p_rim_plmn
);

static
rrm_return_et
rrm_il_compose_rim_plmn
(
    U8  **pp_buffer,
    rim_plmn_t *p_rim_plmn
);

static
rrm_length_t
rrm_il_get_rim_routing_identification_len
(
    rim_routing_identification_t *p_rim_routing_identification
);

static
rrm_return_et
rrm_il_compose_rim_routing_identification
(
    U8  **pp_buffer,
    rim_routing_identification_t *p_rim_routing_identification
);

static
rrm_length_t
rrm_il_get_rim_geran_routing_addr_len
(
    rim_geran_routing_addr_t *p_rim_geran_routing_addr
);

static
rrm_return_et
rrm_il_compose_rim_geran_routing_addr
(
    U8  **pp_buffer,
    rim_geran_routing_addr_t *p_rim_geran_routing_addr
);

static
rrm_length_t
rrm_il_get_rim_utran_routing_addr_len
(
    rim_utran_routing_addr_t *p_rim_utran_routing_addr
);

static
rrm_return_et
rrm_il_compose_rim_utran_routing_addr
(
    U8  **pp_buffer,
    rim_utran_routing_addr_t *p_rim_utran_routing_addr
);

static
rrm_length_t
rrm_il_get_rim_ta_identity_len
(
    rim_ta_identity_t *p_rim_ta_identity
);

static
rrm_return_et
rrm_il_compose_rim_ta_identity
(
    U8  **pp_buffer,
    rim_ta_identity_t *p_rim_ta_identity
);

static
rrm_length_t
rrm_il_get_rim_global_enb_id_len
(
    rim_global_enb_id_t *p_rim_global_enb_id
);

static
rrm_return_et
rrm_il_compose_rim_global_enb_id
(
    U8  **pp_buffer,
    rim_global_enb_id_t *p_rim_global_enb_id
);

static
rrm_length_t
rrm_il_get_rim_eutran_routing_addr_len
(
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr
);

static
rrm_return_et
rrm_il_compose_rim_eutran_routing_addr
(
    U8  **pp_buffer,
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr
);

static
rrm_length_t
rrm_il_get_rim_routing_info_len
(
    rim_routing_info_t *p_rim_routing_info
);

static
rrm_return_et
rrm_il_compose_rim_routing_info
(
    U8  **pp_buffer,
    rim_routing_info_t *p_rim_routing_info
);

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_info_req_len
(
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req
);

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_info_req
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req
);

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_nacc_len
(
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc
);

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_nacc
(
    U8  **pp_buffer,
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc
);

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_si3_len
(
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3
);

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_si3
(
    U8  **pp_buffer,
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3
);

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_mbms_len
(
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms
);

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_mbms
(
    U8  **pp_buffer,
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms
);

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_son_len
(
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son
);

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_son
(
    U8  **pp_buffer,
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son
);

static
rrm_length_t
rrm_il_get_rim_utran_cell_idty_len
(
    rim_utran_cell_idty_t *p_rim_utran_cell_idty
);

static
rrm_return_et
rrm_il_compose_rim_utran_cell_idty
(
    U8  **pp_buffer,
    rim_utran_cell_idty_t *p_rim_utran_cell_idty
);

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_utra_si_len
(
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si
);

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_utra_si
(
    U8  **pp_buffer,
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si
);

static
rrm_length_t
rrm_il_get_rim_req_app_container_len
(
    rim_req_app_container_t *p_rim_req_app_container
);

static
rrm_return_et
rrm_il_compose_rim_req_app_container
(
    U8  **pp_buffer,
    rim_req_app_container_t *p_rim_req_app_container
);

static
rrm_length_t
rrm_il_get_rim_son_transfer_app_idty_len
(
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty
);

static
rrm_return_et
rrm_il_compose_rim_son_transfer_app_idty
(
    U8  **pp_buffer,
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty
);

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_info_len
(
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info
);

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_info
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info
);

static
rrm_length_t
rrm_il_get_rim_si_psi_individual_len
(
    rim_si_psi_individual_t *p_rim_si_psi_individual
);

static
rrm_return_et
rrm_il_compose_rim_si_psi_individual
(
    U8  **pp_buffer,
    rim_si_psi_individual_t *p_rim_si_psi_individual
);

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_nacc_len
(
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc
);

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_nacc
(
    U8  **pp_buffer,
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc
);

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_si3_len
(
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3
);

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_si3
(
    U8  **pp_buffer,
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3
);

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_mbms_len
(
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms
);

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_mbms
(
    U8  **pp_buffer,
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms
);

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_son_len
(
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son
);

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_son
(
    U8  **pp_buffer,
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son
);

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_utra_si_len
(
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si
);

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_utra_si
(
    U8  **pp_buffer,
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si
);

static
rrm_length_t
rrm_il_get_rim_info_app_container_len
(
    rim_info_app_container_t *p_rim_info_app_container
);

static
rrm_return_et
rrm_il_compose_rim_info_app_container
(
    U8  **pp_buffer,
    rim_info_app_container_t *p_rim_info_app_container
);

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_nacc_len
(
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc
);

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_nacc
(
    U8  **pp_buffer,
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc
);

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_si3_len
(
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3
);

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_si3
(
    U8  **pp_buffer,
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3
);

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_mbms_len
(
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms
);

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_mbms
(
    U8  **pp_buffer,
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms
);

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_son_len
(
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son
);

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_son
(
    U8  **pp_buffer,
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son
);

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_utra_si_len
(
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si
);

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_utra_si
(
    U8  **pp_buffer,
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si
);

static
rrm_length_t
rrm_il_get_rim_app_err_container_len
(
    rim_app_err_container_t *p_rim_app_err_container
);

static
rrm_return_et
rrm_il_compose_rim_app_err_container
(
    U8  **pp_buffer,
    rim_app_err_container_t *p_rim_app_err_container
);

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_app_err_len
(
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err
);

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_app_err
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err
);

static
rrm_length_t
rrm_il_get_rim_pdu_in_error_len
(
    rim_pdu_in_error_t *p_rim_pdu_in_error
);

static
rrm_return_et
rrm_il_compose_rim_pdu_in_error
(
    U8  **pp_buffer,
    rim_pdu_in_error_t *p_rim_pdu_in_error
);

static
rrm_length_t
rrm_il_get_s1ap_served_group_id_len
(
    s1ap_served_group_id_t *p_s1ap_served_group_id
);

static
rrm_return_et
rrm_il_compose_s1ap_served_group_id
(
    U8  **pp_buffer,
    s1ap_served_group_id_t *p_s1ap_served_group_id
);

static
rrm_length_t
rrm_il_get_s1ap_served_mmec_len
(
    s1ap_served_mmec_t *p_s1ap_served_mmec
);

static
rrm_return_et
rrm_il_compose_s1ap_served_mmec
(
    U8  **pp_buffer,
    s1ap_served_mmec_t *p_s1ap_served_mmec
);

static
rrm_length_t
rrm_il_get_s1ap_served_gummei_info_len
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
);

static
rrm_return_et
rrm_il_compose_s1ap_served_gummei_info
(
    U8  **pp_buffer,
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
);

static
rrm_length_t
rrm_il_get_s1ap_mme_info_len
(
    s1ap_mme_info_t *p_s1ap_mme_info
);

static
rrm_return_et
rrm_il_compose_s1ap_mme_info
(
    U8  **pp_buffer,
    s1ap_mme_info_t *p_s1ap_mme_info
);

static
rrm_length_t
rrm_il_get_lppa_meas_quantifier_len
(
    lppa_meas_quantifier_t *p_lppa_meas_quantifier
);

static
rrm_return_et
rrm_il_compose_lppa_meas_quantifier
(
    U8  **pp_buffer,
    lppa_meas_quantifier_t *p_lppa_meas_quantifier
);

static
rrm_length_t
rrm_il_get_lppa_plmn_identity_len
(
    lppa_plmn_identity_t *p_lppa_plmn_identity
);

static
rrm_return_et
rrm_il_compose_lppa_plmn_identity
(
    U8  **pp_buffer,
    lppa_plmn_identity_t *p_lppa_plmn_identity
);

static
rrm_length_t
rrm_il_get_lppa_ecgi_len
(
    lppa_ecgi_t *p_lppa_ecgi
);

static
rrm_return_et
rrm_il_compose_lppa_ecgi
(
    U8  **pp_buffer,
    lppa_ecgi_t *p_lppa_ecgi
);

static
rrm_length_t
rrm_il_get_lppa_e_utran_access_point_pos_len
(
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos
);

static
rrm_return_et
rrm_il_compose_lppa_e_utran_access_point_pos
(
    U8  **pp_buffer,
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos
);

static
rrm_length_t
rrm_il_get_lppa_result_rsrp_len
(
    lppa_result_rsrp_t *p_lppa_result_rsrp
);

static
rrm_return_et
rrm_il_compose_lppa_result_rsrp
(
    U8  **pp_buffer,
    lppa_result_rsrp_t *p_lppa_result_rsrp
);

static
rrm_length_t
rrm_il_get_lppa_meas_result_rsrp_len
(
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp
);

static
rrm_return_et
rrm_il_compose_lppa_meas_result_rsrp
(
    U8  **pp_buffer,
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp
);

static
rrm_length_t
rrm_il_get_lppa_result_rsrq_len
(
    lppa_result_rsrq_t *p_lppa_result_rsrq
);

static
rrm_return_et
rrm_il_compose_lppa_result_rsrq
(
    U8  **pp_buffer,
    lppa_result_rsrq_t *p_lppa_result_rsrq
);

static
rrm_length_t
rrm_il_get_lppa_meas_result_rsrq_len
(
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq
);

static
rrm_return_et
rrm_il_compose_lppa_meas_result_rsrq
(
    U8  **pp_buffer,
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq
);

static
rrm_length_t
rrm_il_get_lppa_measured_res_len
(
    lppa_measured_res_t *p_lppa_measured_res
);

static
rrm_return_et
rrm_il_compose_lppa_measured_res
(
    U8  **pp_buffer,
    lppa_measured_res_t *p_lppa_measured_res
);

static
rrm_length_t
rrm_il_get_lppa_meas_results_len
(
    lppa_meas_results_t *p_lppa_meas_results
);

static
rrm_return_et
rrm_il_compose_lppa_meas_results
(
    U8  **pp_buffer,
    lppa_meas_results_t *p_lppa_meas_results
);

static
rrm_length_t
rrm_il_get_lppa_e_cid_meas_result_len
(
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result
);

static
rrm_return_et
rrm_il_compose_lppa_e_cid_meas_result
(
    U8  **pp_buffer,
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result
);

static
rrm_length_t
rrm_il_get_lppa_cause_len
(
    lppa_cause_t *p_lppa_cause
);

static
rrm_return_et
rrm_il_compose_lppa_cause
(
    U8  **pp_buffer,
    lppa_cause_t *p_lppa_cause
);

static
rrm_length_t
rrm_il_get_lppa_otdoa_info_type_len
(
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type
);

static
rrm_return_et
rrm_il_compose_lppa_otdoa_info_type
(
    U8  **pp_buffer,
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type
);

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_tac_len
(
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac
);

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells_tac
(
    U8  **pp_buffer,
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac
);

static
rrm_length_t
rrm_il_get_lppa_otdoa_sfn_init_time_len
(
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time
);

static
rrm_return_et
rrm_il_compose_lppa_otdoa_sfn_init_time
(
    U8  **pp_buffer,
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time
);

static
rrm_length_t
rrm_il_get_lppa_muting_config_two_len
(
    lppa_muting_config_two_t *p_lppa_muting_config_two
);

static
rrm_return_et
rrm_il_compose_lppa_muting_config_two
(
    U8  **pp_buffer,
    lppa_muting_config_two_t *p_lppa_muting_config_two
);

static
rrm_length_t
rrm_il_get_lppa_muting_config_four_len
(
    lppa_muting_config_four_t *p_lppa_muting_config_four
);

static
rrm_return_et
rrm_il_compose_lppa_muting_config_four
(
    U8  **pp_buffer,
    lppa_muting_config_four_t *p_lppa_muting_config_four
);

static
rrm_length_t
rrm_il_get_lppa_muting_config_eight_len
(
    lppa_muting_config_eight_t *p_lppa_muting_config_eight
);

static
rrm_return_et
rrm_il_compose_lppa_muting_config_eight
(
    U8  **pp_buffer,
    lppa_muting_config_eight_t *p_lppa_muting_config_eight
);

static
rrm_length_t
rrm_il_get_lppa_muting_config_sixteen_len
(
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen
);

static
rrm_return_et
rrm_il_compose_lppa_muting_config_sixteen
(
    U8  **pp_buffer,
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen
);

static
rrm_length_t
rrm_il_get_lppa_prs_muting_config_len
(
    lppa_prs_muting_config_t *p_lppa_prs_muting_config
);

static
rrm_return_et
rrm_il_compose_lppa_prs_muting_config
(
    U8  **pp_buffer,
    lppa_prs_muting_config_t *p_lppa_prs_muting_config
);

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_info_len
(
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info
);

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells_info
(
    U8  **pp_buffer,
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info
);

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_len
(
    lppa_otdoa_cells_t *p_lppa_otdoa_cells
);

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells
(
    U8  **pp_buffer,
    lppa_otdoa_cells_t *p_lppa_otdoa_cells
);

static
rrm_length_t
rrm_il_get_lppa_criticality_diagno_len
(
    lppa_criticality_diagno_t *p_lppa_criticality_diagno
);

static
rrm_return_et
rrm_il_compose_lppa_criticality_diagno
(
    U8  **pp_buffer,
    lppa_criticality_diagno_t *p_lppa_criticality_diagno
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_length_t
rrm_il_get_s1ap_rrm_enb_direct_info_transfer_len
(
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_enb_direct_info_transfer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_enb_direct_info_transfer->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_enb_direct_info_transfer->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_enb_direct_info_transfer->len_rim_info);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_s1ap_rrm_enb_direct_info_transfer->len_rim_info * sizeof(p_s1ap_rrm_enb_direct_info_transfer->rim_info[0]));

    /* Optional element */
    if(p_s1ap_rrm_enb_direct_info_transfer->bitmask & GERAN_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrm_geran_cell_id_len(&p_s1ap_rrm_enb_direct_info_transfer->geran_cell_id);
    }

    /* Optional element */
    if(p_s1ap_rrm_enb_direct_info_transfer->bitmask & TARGET_RNC_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrm_rnc_id_len(&p_s1ap_rrm_enb_direct_info_transfer->target_rnc_id);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_enb_direct_info_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_enb_direct_info_transfer != PNULL);

    /* This function composes s1ap_rrm_enb_direct_info_transfer */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_enb_direct_info_transfer->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_direct_info_transfer->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_enb_direct_info_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_enb_direct_info_transfer->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_enb_direct_info_transfer->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_direct_info_transfer->len_rim_info > 1024))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_enb_direct_info_transfer->len_rim_info] should be less than"
            " or equal to 1024. Incorrect value %u received.", p_s1ap_rrm_enb_direct_info_transfer->len_rim_info);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->len_rim_info, "len_rim_info");
    *pp_buffer += sizeof(p_s1ap_rrm_enb_direct_info_transfer->len_rim_info);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_enb_direct_info_transfer->len_rim_info; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->rim_info[loop], "rim_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_s1ap_rrm_enb_direct_info_transfer->bitmask & GERAN_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_geran_cell_id(pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->geran_cell_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_enb_direct_info_transfer->bitmask & TARGET_RNC_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_rnc_id(pp_buffer, &p_s1ap_rrm_enb_direct_info_transfer->target_rnc_id))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_enb_direct_info_transfer
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_ENB_DIRECT_INFO_TRANSFER message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_enb_direct_info_transfer
(
    s1ap_rrm_enb_direct_info_transfer_t  *p_s1ap_rrm_enb_direct_info_transfer,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_enb_direct_info_transfer != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_enb_direct_info_transfer_len(p_s1ap_rrm_enb_direct_info_transfer);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_ENB_DIRECT_INFO_TRANSFER", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_ENB_DIRECT_INFO_TRANSFER, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_ENB_DIRECT_INFO_TRANSFER, msg_length,cell_index);

    /* Fill S1AP_RRM_ENB_DIRECT_INFO_TRANSFER message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_enb_direct_info_transfer(&p_s1apRrm_msg, p_s1ap_rrm_enb_direct_info_transfer))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_mnc_len
(
    mnc_t *p_mnc
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_mnc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_mnc->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_mnc->count * sizeof(p_mnc->mnc[0]));

    return length;
}

static
rrm_return_et
rrm_il_compose_mnc
(
    U8  **pp_buffer,
    mnc_t *p_mnc
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_mnc != PNULL);

    /* This function composes mnc */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mnc->count < 2) || (p_mnc->count > MAX_MNC_OCTET_SIZE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_mnc->count] should be in range "
            "2 to MAX_MNC_OCTET_SIZE. Incorrect value %u received.", p_mnc->count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_mnc->count, "count");
    *pp_buffer += sizeof(p_mnc->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mnc->count; loop++)
        {
            if (p_mnc->mnc[loop] > 9)
            {
                RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter p_mnc->mnc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_mnc->mnc[loop]);
                return RRM_FAILURE;
            }
            rrm_cp_pack_U8(*pp_buffer, &p_mnc->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_plmn_identity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_plmn_identity->presence_bitmask);

    /* Optional element */
    if(p_plmn_identity->presence_bitmask & 1)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_plmn_identity->mcc);
    }

    /* Get length of IE */
    length += rrm_il_get_mnc_len(&p_plmn_identity->mnc);

    return length;
}

static
rrm_return_et
rrm_il_compose_plmn_identity
(
    U8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_plmn_identity != PNULL);

    /* This function composes plmn_identity */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_plmn_identity->presence_bitmask, "presence_bitmask");
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
                RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter p_plmn_identity->mcc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_plmn_identity->mcc[loop]);
                return RRM_FAILURE;
            }
            rrm_cp_pack_U8(*pp_buffer, &p_plmn_identity->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_mnc(pp_buffer, &p_plmn_identity->mnc))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_s1ap_lai_info_len
(
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_s1ap_lai_info != PNULL);

    /* Get length of IE */
    length += rrm_il_get_plmn_identity_len(&p_rrm_s1ap_lai_info->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_s1ap_lai_info->location_area_code);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_s1ap_lai_info
(
    U8  **pp_buffer,
    rrm_s1ap_lai_info_t *p_rrm_s1ap_lai_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_s1ap_lai_info != PNULL);

    /* This function composes rrm_s1ap_lai_info */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_plmn_identity(pp_buffer, &p_rrm_s1ap_lai_info->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_s1ap_lai_info->location_area_code); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_s1ap_lai_info->location_area_code[loop], "location_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_geran_cell_id_len
(
    rrm_geran_cell_id_t *p_rrm_geran_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_geran_cell_id != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rrm_s1ap_lai_info_len(&p_rrm_geran_cell_id->lai);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_geran_cell_id->routing_area_code);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_geran_cell_id->ci);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_geran_cell_id
(
    U8  **pp_buffer,
    rrm_geran_cell_id_t *p_rrm_geran_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_geran_cell_id != PNULL);

    /* This function composes rrm_geran_cell_id */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_s1ap_lai_info(pp_buffer, &p_rrm_geran_cell_id->lai))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_geran_cell_id->routing_area_code, "routing_area_code");
    *pp_buffer += sizeof(p_rrm_geran_cell_id->routing_area_code);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_geran_cell_id->ci); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_geran_cell_id->ci[loop], "ci[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_lai_info_len
(
    rrm_lai_info_t *p_rrm_lai_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_lai_info != PNULL);

    /* Get length of IE */
    length += rrm_il_get_plmn_identity_len(&p_rrm_lai_info->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_lai_info->location_area_code);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_lai_info
(
    U8  **pp_buffer,
    rrm_lai_info_t *p_rrm_lai_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_lai_info != PNULL);

    /* This function composes rrm_lai_info */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_plmn_identity(pp_buffer, &p_rrm_lai_info->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_lai_info->location_area_code); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_lai_info->location_area_code[loop], "location_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_rnc_id_len
(
    rrm_rnc_id_t *p_rrm_rnc_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_rnc_id != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_rnc_id->bitmask);

    /* Get length of IE */
    length += rrm_il_get_rrm_lai_info_len(&p_rrm_rnc_id->lai);

    /* Optional element */
    if(p_rrm_rnc_id->bitmask & RRM_RNC_ROUTING_AREA_CODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_rnc_id->routing_area_code);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_rnc_id->rnc_id);

    /* Optional element */
    if(p_rrm_rnc_id->bitmask & RRM_RNC_EXTENDED_RNC_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_rnc_id->extended_rnc_id);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_rnc_id
(
    U8  **pp_buffer,
    rrm_rnc_id_t *p_rrm_rnc_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_rnc_id != PNULL);

    /* This function composes rrm_rnc_id */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_rnc_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_rnc_id->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_lai_info(pp_buffer, &p_rrm_rnc_id->lai))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_rnc_id->bitmask & RRM_RNC_ROUTING_AREA_CODE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_rnc_id->routing_area_code, "routing_area_code");
    *pp_buffer += sizeof(p_rrm_rnc_id->routing_area_code);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_rnc_id->rnc_id > 4095))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrm_rnc_id->rnc_id] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrm_rnc_id->rnc_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_rnc_id->rnc_id, "rnc_id");
    *pp_buffer += sizeof(p_rrm_rnc_id->rnc_id);

    /* Optional element */
    if(p_rrm_rnc_id->bitmask & RRM_RNC_EXTENDED_RNC_ID_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_rnc_id->extended_rnc_id, "extended_rnc_id");
    *pp_buffer += sizeof(p_rrm_rnc_id->extended_rnc_id);
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_mme_direct_info_transfer_len
(
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_mme_direct_info_transfer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_direct_info_transfer->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_direct_info_transfer->len_rim_info);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_s1ap_rrm_mme_direct_info_transfer->len_rim_info * sizeof(p_s1ap_rrm_mme_direct_info_transfer->rim_info[0]));

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_direct_info_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_mme_direct_info_transfer != PNULL);

    /* This function composes s1ap_rrm_mme_direct_info_transfer */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_direct_info_transfer->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_direct_info_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_mme_direct_info_transfer->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_direct_info_transfer->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_direct_info_transfer->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_direct_info_transfer->len_rim_info > MAX_BSSGP_PDU_LEN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_direct_info_transfer->len_rim_info] should be less than"
            " or equal to MAX_BSSGP_PDU_LEN. Incorrect value %u received.", p_s1ap_rrm_mme_direct_info_transfer->len_rim_info);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_mme_direct_info_transfer->len_rim_info, "len_rim_info");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_direct_info_transfer->len_rim_info);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_mme_direct_info_transfer->len_rim_info; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_direct_info_transfer->rim_info[loop], "rim_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_mme_direct_info_transfer
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_MME_DIRECT_INFO_TRANSFER message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_mme_direct_info_transfer
(
    s1ap_rrm_mme_direct_info_transfer_t  *p_s1ap_rrm_mme_direct_info_transfer,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_mme_direct_info_transfer != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_mme_direct_info_transfer_len(p_s1ap_rrm_mme_direct_info_transfer);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_MME_DIRECT_INFO_TRANSFER", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_MME_DIRECT_INFO_TRANSFER, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_MME_DIRECT_INFO_TRANSFER, msg_length,cell_index);

    /* Fill S1AP_RRM_MME_DIRECT_INFO_TRANSFER message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_mme_direct_info_transfer(&p_s1apRrm_msg, p_s1ap_rrm_mme_direct_info_transfer))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_enb_config_transfer_len
(
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_enb_config_transfer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_enb_config_transfer->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_enb_config_transfer->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rrm_gb_enb_id_len(&p_s1ap_rrm_enb_config_transfer->gb_enb_id);

    /* Get length of IE */
    length += rrm_il_get_rrm_supp_ta_len(&p_s1ap_rrm_enb_config_transfer->target_supp_ta);

    /* Get length of IE */
    length += rrm_il_get_rrm_supp_ta_len(&p_s1ap_rrm_enb_config_transfer->source_supp_ta);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_enb_config_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_enb_config_transfer != PNULL);

    /* This function composes s1ap_rrm_enb_config_transfer */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_enb_config_transfer->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_enb_config_transfer->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_enb_config_transfer->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_enb_config_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_enb_config_transfer->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_enb_config_transfer->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_enb_config_transfer->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_gb_enb_id(pp_buffer, &p_s1ap_rrm_enb_config_transfer->gb_enb_id))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_supp_ta(pp_buffer, &p_s1ap_rrm_enb_config_transfer->target_supp_ta))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_supp_ta(pp_buffer, &p_s1ap_rrm_enb_config_transfer->source_supp_ta))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_enb_config_transfer
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_ENB_CONFIG_TRANSFER message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_enb_config_transfer
(
    s1ap_rrm_enb_config_transfer_t  *p_s1ap_rrm_enb_config_transfer,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_enb_config_transfer != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_enb_config_transfer_len(p_s1ap_rrm_enb_config_transfer);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_ENB_CONFIG_TRANSFER", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_ENB_CONFIG_TRANSFER, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_ENB_CONFIG_TRANSFER, msg_length,cell_index);

    /* Fill S1AP_RRM_ENB_CONFIG_TRANSFER message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_enb_config_transfer(&p_s1apRrm_msg, p_s1ap_rrm_enb_config_transfer))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_macro_enb_id_len
(
    rrm_macro_enb_id_t *p_rrm_macro_enb_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_macro_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_macro_enb_id->eNB_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_macro_enb_id
(
    U8  **pp_buffer,
    rrm_macro_enb_id_t *p_rrm_macro_enb_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_macro_enb_id != PNULL);

    /* This function composes rrm_macro_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_macro_enb_id->eNB_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_macro_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_home_enb_id_len
(
    rrm_home_enb_id_t *p_rrm_home_enb_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_home_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_home_enb_id->eNB_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_home_enb_id
(
    U8  **pp_buffer,
    rrm_home_enb_id_t *p_rrm_home_enb_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_home_enb_id != PNULL);

    /* This function composes rrm_home_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_home_enb_id->eNB_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_home_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_enb_id_info_len
(
    rrm_enb_id_info_t *p_rrm_enb_id_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_enb_id_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrm_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_rrm_enb_id_info->presence_bitmask & 1)
    {

    /* Get length of IE */
    length += rrm_il_get_rrm_macro_enb_id_len(&p_rrm_enb_id_info->macro_enb_id);
    }

    /* Optional element */
    if(p_rrm_enb_id_info->presence_bitmask & 2)
    {

    /* Get length of IE */
    length += rrm_il_get_rrm_home_enb_id_len(&p_rrm_enb_id_info->home_enb_id);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_enb_id_info
(
    U8  **pp_buffer,
    rrm_enb_id_info_t *p_rrm_enb_id_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_enb_id_info != PNULL);

    /* This function composes rrm_enb_id_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_enb_id_info->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_rrm_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_rrm_enb_id_info->presence_bitmask & 1)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_macro_enb_id(pp_buffer, &p_rrm_enb_id_info->macro_enb_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_enb_id_info->presence_bitmask & 2)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_home_enb_id(pp_buffer, &p_rrm_enb_id_info->home_enb_id))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_gb_enb_id_len
(
    rrm_gb_enb_id_t *p_rrm_gb_enb_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_gb_enb_id != PNULL);

    /* Get length of IE */
    length += rrm_il_get_plmn_identity_len(&p_rrm_gb_enb_id->plmn_identity);

    /* Get length of IE */
    length += rrm_il_get_rrm_enb_id_info_len(&p_rrm_gb_enb_id->enb_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_gb_enb_id
(
    U8  **pp_buffer,
    rrm_gb_enb_id_t *p_rrm_gb_enb_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_gb_enb_id != PNULL);

    /* This function composes rrm_gb_enb_id */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_plmn_identity(pp_buffer, &p_rrm_gb_enb_id->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_enb_id_info(pp_buffer, &p_rrm_gb_enb_id->enb_id))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrm_supp_ta_len
(
    rrm_supp_ta_t *p_rrm_supp_ta
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_supp_ta != PNULL);

    /* Get length of IE */
    length += rrm_il_get_plmn_identity_len(&p_rrm_supp_ta->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_supp_ta->tac);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrm_supp_ta
(
    U8  **pp_buffer,
    rrm_supp_ta_t *p_rrm_supp_ta
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_supp_ta != PNULL);

    /* This function composes rrm_supp_ta */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_plmn_identity(pp_buffer, &p_rrm_supp_ta->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_supp_ta->tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_supp_ta->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_mme_config_transfer_len
(
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_mme_config_transfer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_config_transfer->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_config_transfer->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rrm_gb_enb_id_len(&p_s1ap_rrm_mme_config_transfer->gb_enb_id);

    /* Get length of IE */
    length += rrm_il_get_rrm_supp_ta_len(&p_s1ap_rrm_mme_config_transfer->source_supp_ta);

    /* Get length of IE */
    length += rrm_il_get_rrm_supp_ta_len(&p_s1ap_rrm_mme_config_transfer->target_supp_ta);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_config_transfer->number_of_addr);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_addr); loop++)
        {
            length += rrm_il_get_ip_address_len(&p_s1ap_rrm_mme_config_transfer->ip_addr[loop]);
        }
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr);
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_v6_addr); loop++)
        {
            length += rrm_il_get_ip_address_len(&p_s1ap_rrm_mme_config_transfer->ip_v6_addr[loop]);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_config_transfer->num_tlas);
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_mme_config_transfer->num_tlas < 1) || (p_s1ap_rrm_mme_config_transfer->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %d received.", p_s1ap_rrm_mme_config_transfer->num_tlas);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_mme_config_transfer->num_tlas; loop++)
        {
            length += rrm_il_get_extend_trans_layer_addrs_len(&p_s1ap_rrm_mme_config_transfer->extend_trans_layer_addrs[loop]);
        }
    }
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_config_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_mme_config_transfer != PNULL);

    /* This function composes s1ap_rrm_mme_config_transfer */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_mme_config_transfer->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_config_transfer->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_config_transfer->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_config_transfer->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_config_transfer->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_gb_enb_id(pp_buffer, &p_s1ap_rrm_mme_config_transfer->gb_enb_id))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_supp_ta(pp_buffer, &p_s1ap_rrm_mme_config_transfer->source_supp_ta))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrm_supp_ta(pp_buffer, &p_s1ap_rrm_mme_config_transfer->target_supp_ta))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_config_transfer->number_of_addr, "number_of_addr");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_config_transfer->number_of_addr);

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_addr); loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_ip_address(pp_buffer, &p_s1ap_rrm_mme_config_transfer->ip_addr[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr > MAX_NUM_IP_SUPPORTED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr] should be less than"
            " or equal to MAX_NUM_IP_SUPPORTED. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr, "number_of_ipv6_addr");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_config_transfer->number_of_ipv6_addr);
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_X2TNL_IPv6)
    {

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_config_transfer->ip_v6_addr); loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_ip_address(pp_buffer, &p_s1ap_rrm_mme_config_transfer->ip_v6_addr[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_mme_config_transfer->num_tlas < 1) || (p_s1ap_rrm_mme_config_transfer->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_mme_config_transfer->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_s1ap_rrm_mme_config_transfer->num_tlas);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_config_transfer->num_tlas, "num_tlas");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_config_transfer->num_tlas);
    }

    /* Optional element */
    if(p_s1ap_rrm_mme_config_transfer->bitmask & MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_mme_config_transfer->num_tlas; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_extend_trans_layer_addrs(pp_buffer, &p_s1ap_rrm_mme_config_transfer->extend_trans_layer_addrs[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_mme_config_transfer
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_MME_CONFIG_TRANSFER message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_mme_config_transfer
(
    s1ap_rrm_mme_config_transfer_t  *p_s1ap_rrm_mme_config_transfer,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_mme_config_transfer != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_mme_config_transfer_len(p_s1ap_rrm_mme_config_transfer);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_MME_CONFIG_TRANSFER", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_MME_CONFIG_TRANSFER, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_MME_CONFIG_TRANSFER, msg_length,cell_index);

    /* Fill S1AP_RRM_MME_CONFIG_TRANSFER message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_mme_config_transfer(&p_s1apRrm_msg, p_s1ap_rrm_mme_config_transfer))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_ip_address_len
(
    ip_address_t *p_ip_address
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_ip_address != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ip_address->ip_addr);

    return length;
}

static
rrm_return_et
rrm_il_compose_ip_address
(
    U8  **pp_buffer,
    ip_address_t *p_ip_address
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_ip_address != PNULL);

    /* This function composes ip_address */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ip_address->ip_addr); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_ip_address->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_trans_layer_addrs_len
(
    trans_layer_addrs_t *p_trans_layer_addrs
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_trans_layer_addrs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_trans_layer_addrs->data_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_trans_layer_addrs->data_length * sizeof(p_trans_layer_addrs->data[0]));

    return length;
}

static
rrm_return_et
rrm_il_compose_trans_layer_addrs
(
    U8  **pp_buffer,
    trans_layer_addrs_t *p_trans_layer_addrs
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_trans_layer_addrs != PNULL);

    /* This function composes trans_layer_addrs */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_trans_layer_addrs->data_length);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_trans_layer_addrs->data_length, "data_length");
    *pp_buffer += sizeof(p_trans_layer_addrs->data_length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_trans_layer_addrs->data_length; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_trans_layer_addrs->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_extend_trans_layer_addrs_len
(
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_extend_trans_layer_addrs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_trans_layer_addrs_len(&p_extend_trans_layer_addrs->ipsec_trans_layer_addrs);
    }

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_extend_trans_layer_addrs->num_tlas);
    }

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_extend_trans_layer_addrs->num_tlas < 1) || (p_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %d received.", p_extend_trans_layer_addrs->num_tlas);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_extend_trans_layer_addrs->num_tlas; loop++)
        {
            length += rrm_il_get_trans_layer_addrs_len(&p_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]);
        }
    }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    extend_trans_layer_addrs_t *p_extend_trans_layer_addrs
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_extend_trans_layer_addrs != PNULL);

    /* This function composes extend_trans_layer_addrs */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_extend_trans_layer_addrs->bitmask, "bitmask");
    *pp_buffer += sizeof(p_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_trans_layer_addrs(pp_buffer, &p_extend_trans_layer_addrs->ipsec_trans_layer_addrs))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_extend_trans_layer_addrs->num_tlas < 1) || (p_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_extend_trans_layer_addrs->num_tlas);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_extend_trans_layer_addrs->num_tlas, "num_tlas");
    *pp_buffer += sizeof(p_extend_trans_layer_addrs->num_tlas);
    }

    /* Optional element */
    if(p_extend_trans_layer_addrs->bitmask & EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_trans_layer_addrs(pp_buffer, &p_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_pws_request_len
(
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_pws_request != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_request->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_request->serial_number);

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_AREA_LIST)
    {

    /* Get length of IE */
    length += rrm_il_get_warning_area_list_info_len(&p_s1ap_rrm_pws_request->warning_area_list);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->repetition_period);

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->extended_repetition_period);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->number_of_broadcast_requested);

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_TYPE)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_request->warning_type);
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_SECURITY_INFO)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_request->warning_security_info);
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_DATA_CODING_SCHEME)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_request->data_coding_scheme);
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_MESSAGE_SIZE)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->warning_message_size);
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_request
(
    U8  **pp_buffer,
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_pws_request != PNULL);

    /* This function composes s1ap_rrm_pws_request */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_request->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_AREA_LIST)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_warning_area_list_info(pp_buffer, &p_s1ap_rrm_pws_request->warning_area_list))
    {
        return RRM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->repetition_period > 4095))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_request->repetition_period] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_s1ap_rrm_pws_request->repetition_period);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_request->repetition_period, "repetition_period");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->repetition_period);

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->extended_repetition_period > 131071))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_request->extended_repetition_period] should be less than"
            " or equal to 131071. Incorrect value %u received.", p_s1ap_rrm_pws_request->extended_repetition_period);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_pws_request->extended_repetition_period, "extended_repetition_period");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->extended_repetition_period);
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_request->number_of_broadcast_requested, "number_of_broadcast_requested");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->number_of_broadcast_requested);

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_TYPE)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->warning_type); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->warning_type[loop], "warning_type[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_SECURITY_INFO)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->warning_security_info); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->warning_security_info[loop], "warning_security_info[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_DATA_CODING_SCHEME)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_request->data_coding_scheme); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->data_coding_scheme[loop], "data_coding_scheme[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_WARNING_MESSAGE_SIZE)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->warning_message_size > 9600))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_request->warning_message_size] should be less than"
            " or equal to 9600. Incorrect value %u received.", p_s1ap_rrm_pws_request->warning_message_size);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_request->warning_message_size, "warning_message_size");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->warning_message_size);
    }

    /* Optional element */
    if(p_s1ap_rrm_pws_request->bitmask & S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator > 0))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator] should be less than"
            " or equal to 0. Incorrect value %u received.", p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator, "concurrent_warning_msg_indicator");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_request->concurrent_warning_msg_indicator);
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_pws_request
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_PWS_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_pws_request
(
    s1ap_rrm_pws_request_t  *p_s1ap_rrm_pws_request,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_pws_request != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_pws_request_len(p_s1ap_rrm_pws_request);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_PWS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_PWS_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_PWS_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_PWS_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_pws_request(&p_s1apRrm_msg, p_s1ap_rrm_pws_request))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_plmn_identity_len
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_plmn_identity->plmn_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_plmn_identity
(
    U8  **pp_buffer,
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_plmn_identity != PNULL);

    /* This function composes s1ap_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_plmn_identity->plmn_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_cgi_eutra_len
(
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_cgi_eutra != PNULL);

    /* Get length of IE */
    length += rrm_il_get_s1ap_plmn_identity_len(&p_s1ap_cgi_eutra->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_cgi_eutra->cell_identity);

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_cgi_eutra
(
    U8  **pp_buffer,
    s1ap_cgi_eutra_t *p_s1ap_cgi_eutra
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_cgi_eutra != PNULL);

    /* This function composes s1ap_cgi_eutra */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_s1ap_plmn_identity(pp_buffer, &p_s1ap_cgi_eutra->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_cgi_eutra->cell_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_cgi_eutra->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_cell_id_list_len
(
    cell_id_list_t *p_cell_id_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_cell_id_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_cell_id_list->count);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cell_id_list->count; loop++)
        {
            length += rrm_il_get_s1ap_cgi_eutra_len(&p_cell_id_list->eutran_cgi[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_cell_id_list
(
    U8  **pp_buffer,
    cell_id_list_t *p_cell_id_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_cell_id_list != PNULL);

    /* This function composes cell_id_list */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_cell_id_list->count, "count");
    *pp_buffer += sizeof(p_cell_id_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cell_id_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_s1ap_cgi_eutra(pp_buffer, &p_cell_id_list->eutran_cgi[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_tracking_area_identifier_len
(
    tracking_area_identifier_t *p_tracking_area_identifier
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_tracking_area_identifier != PNULL);

    /* Get length of IE */
    length += rrm_il_get_s1ap_plmn_identity_len(&p_tracking_area_identifier->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_tracking_area_identifier->tracking_area_code);

    return length;
}

static
rrm_return_et
rrm_il_compose_tracking_area_identifier
(
    U8  **pp_buffer,
    tracking_area_identifier_t *p_tracking_area_identifier
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_tracking_area_identifier != PNULL);

    /* This function composes tracking_area_identifier */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_s1ap_plmn_identity(pp_buffer, &p_tracking_area_identifier->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_tracking_area_identifier->tracking_area_code); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_tracking_area_identifier->tracking_area_code[loop], "tracking_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_tracking_area_id_list_len
(
    tracking_area_id_list_t *p_tracking_area_id_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_tracking_area_id_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_tracking_area_id_list->count);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_tracking_area_id_list->count; loop++)
        {
            length += rrm_il_get_tracking_area_identifier_len(&p_tracking_area_id_list->tracking_area_identifier[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_tracking_area_id_list
(
    U8  **pp_buffer,
    tracking_area_id_list_t *p_tracking_area_id_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_tracking_area_id_list != PNULL);

    /* This function composes tracking_area_id_list */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_tracking_area_id_list->count, "count");
    *pp_buffer += sizeof(p_tracking_area_id_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_tracking_area_id_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_tracking_area_identifier(pp_buffer, &p_tracking_area_id_list->tracking_area_identifier[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_emergency_area_identifier_len
(
    emergency_area_identifier_t *p_emergency_area_identifier
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_emergency_area_identifier != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_emergency_area_identifier->emergency_area_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_emergency_area_identifier
(
    U8  **pp_buffer,
    emergency_area_identifier_t *p_emergency_area_identifier
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_emergency_area_identifier != PNULL);

    /* This function composes emergency_area_identifier */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_emergency_area_identifier->emergency_area_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_emergency_area_identifier->emergency_area_id[loop], "emergency_area_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_emergency_area_id_list_len
(
    emergency_area_id_list_t *p_emergency_area_id_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_emergency_area_id_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_emergency_area_id_list->count);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_emergency_area_id_list->count; loop++)
        {
            length += rrm_il_get_emergency_area_identifier_len(&p_emergency_area_id_list->emer_id[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_emergency_area_id_list
(
    U8  **pp_buffer,
    emergency_area_id_list_t *p_emergency_area_id_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_emergency_area_id_list != PNULL);

    /* This function composes emergency_area_id_list */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_emergency_area_id_list->count, "count");
    *pp_buffer += sizeof(p_emergency_area_id_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_emergency_area_id_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_emergency_area_identifier(pp_buffer, &p_emergency_area_id_list->emer_id[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_warning_area_list_info_len
(
    warning_area_list_info_t *p_warning_area_list_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_warning_area_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_warning_area_list_info->bitmask);

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_CELL_ID_LIST)
    {

    /* Get length of IE */
    length += rrm_il_get_cell_id_list_len(&p_warning_area_list_info->cell_id_list);
    }

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST)
    {

    /* Get length of IE */
    length += rrm_il_get_tracking_area_id_list_len(&p_warning_area_list_info->tracking_area_id_list);
    }

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST)
    {

    /* Get length of IE */
    length += rrm_il_get_emergency_area_id_list_len(&p_warning_area_list_info->emergency_area_id_list);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_warning_area_list_info
(
    U8  **pp_buffer,
    warning_area_list_info_t *p_warning_area_list_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_warning_area_list_info != PNULL);

    /* This function composes warning_area_list_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_warning_area_list_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_warning_area_list_info->bitmask);

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_CELL_ID_LIST)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_cell_id_list(pp_buffer, &p_warning_area_list_info->cell_id_list))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_tracking_area_id_list(pp_buffer, &p_warning_area_list_info->tracking_area_id_list))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_warning_area_list_info->bitmask & WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_emergency_area_id_list(pp_buffer, &p_warning_area_list_info->emergency_area_id_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_pws_response_len
(
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_pws_response != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_response->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_response->response);

    /* Optional element */
    if(p_s1ap_rrm_pws_response->bitmask & S1AP_RRM_PWS_CONTENTS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrc_rrm_pws_contents_list_len(&p_s1ap_rrm_pws_response->rrc_rrm_pws_contents_list);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_response
(
    U8  **pp_buffer,
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_pws_response != PNULL);

    /* This function composes s1ap_rrm_pws_response */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_response->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_response->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_response->response > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_response->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_pws_response->response);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_response->response, "response");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_response->response);

    /* Optional element */
    if(p_s1ap_rrm_pws_response->bitmask & S1AP_RRM_PWS_CONTENTS_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrc_rrm_pws_contents_list(pp_buffer, &p_s1ap_rrm_pws_response->rrc_rrm_pws_contents_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_pws_response
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_PWS_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_pws_response
(
    s1ap_rrm_pws_response_t  *p_s1ap_rrm_pws_response,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_pws_response != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_pws_response_len(p_s1ap_rrm_pws_response);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_PWS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_PWS_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_PWS_RESP, msg_length,cell_index);

    /* Fill S1AP_RRM_PWS_RESP message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_pws_response(&p_s1apRrm_msg, p_s1ap_rrm_pws_response))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_sib_mapping_info_len
(
    sib_mapping_info_t *p_sib_mapping_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_sib_mapping_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_sib_mapping_info->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_sib_mapping_info->count * sizeof(p_sib_mapping_info->sib_type[0]));

    return length;
}

static
rrm_return_et
rrm_il_compose_sib_mapping_info
(
    U8  **pp_buffer,
    sib_mapping_info_t *p_sib_mapping_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_sib_mapping_info != PNULL);

    /* This function composes sib_mapping_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_sib_mapping_info->count, "count");
    *pp_buffer += sizeof(p_sib_mapping_info->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_sib_mapping_info->count; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_sib_mapping_info->sib_type[loop], "sib_type[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_scheduling_info_len
(
    scheduling_info_t *p_scheduling_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_scheduling_info != PNULL);

    /* Get length of IE */
    length += rrm_il_get_sib_mapping_info_len(&p_scheduling_info->sib_mapping_info);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_scheduling_info->si_periodicity);

    return length;
}

static
rrm_return_et
rrm_il_compose_scheduling_info
(
    U8  **pp_buffer,
    scheduling_info_t *p_scheduling_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_scheduling_info != PNULL);

    /* This function composes scheduling_info */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_sib_mapping_info(pp_buffer, &p_scheduling_info->sib_mapping_info))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_scheduling_info->si_periodicity > 6))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_scheduling_info->si_periodicity] should be less than"
            " or equal to 6. Incorrect value %u received.", p_scheduling_info->si_periodicity);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_scheduling_info->si_periodicity, "si_periodicity");
    *pp_buffer += sizeof(p_scheduling_info->si_periodicity);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_scheduling_info_list_len
(
    scheduling_info_list_t *p_scheduling_info_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_scheduling_info_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_scheduling_info_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_scheduling_info_list->count < 1) || (p_scheduling_info_list->count > MAX_SI_MESSAGE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_scheduling_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %d received.", p_scheduling_info_list->count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_scheduling_info_list->count; loop++)
        {
            length += rrm_il_get_scheduling_info_len(&p_scheduling_info_list->scheduling_info[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_scheduling_info_list
(
    U8  **pp_buffer,
    scheduling_info_list_t *p_scheduling_info_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_scheduling_info_list != PNULL);

    /* This function composes scheduling_info_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_scheduling_info_list->count < 1) || (p_scheduling_info_list->count > MAX_SI_MESSAGE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_scheduling_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %u received.", p_scheduling_info_list->count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_scheduling_info_list->count, "count");
    *pp_buffer += sizeof(p_scheduling_info_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_scheduling_info_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_scheduling_info(pp_buffer, &p_scheduling_info_list->scheduling_info[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_si_start_offset_len
(
    rrc_si_start_offset_t *p_rrc_si_start_offset
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_si_start_offset != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_start_offset->si_start_offset);

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_si_start_offset
(
    U8  **pp_buffer,
    rrc_si_start_offset_t *p_rrc_si_start_offset
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_si_start_offset != PNULL);

    /* This function composes rrc_si_start_offset */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_si_start_offset->si_start_offset > 39))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_start_offset->si_start_offset] should be less than"
            " or equal to 39. Incorrect value %u received.", p_rrc_si_start_offset->si_start_offset);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_si_start_offset->si_start_offset, "si_start_offset");
    *pp_buffer += sizeof(p_rrc_si_start_offset->si_start_offset);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_si_start_offset_info_len
(
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_si_start_offset_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_start_offset_info->offset_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_start_offset_info->offset_count < 1) || (p_rrc_si_start_offset_info->offset_count > MAX_SI_START_OFFSET))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_start_offset_info->offset_count] should be in range "
            "1 to MAX_SI_START_OFFSET. Incorrect value %d received.", p_rrc_si_start_offset_info->offset_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_start_offset_info->offset_count; loop++)
        {
            length += rrm_il_get_rrc_si_start_offset_len(&p_rrc_si_start_offset_info->si_start_offset[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_si_start_offset_info
(
    U8  **pp_buffer,
    rrc_si_start_offset_info_t *p_rrc_si_start_offset_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_si_start_offset_info != PNULL);

    /* This function composes rrc_si_start_offset_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_start_offset_info->offset_count < 1) || (p_rrc_si_start_offset_info->offset_count > MAX_SI_START_OFFSET))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_start_offset_info->offset_count] should be in range "
            "1 to MAX_SI_START_OFFSET. Incorrect value %u received.", p_rrc_si_start_offset_info->offset_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_si_start_offset_info->offset_count, "offset_count");
    *pp_buffer += sizeof(p_rrc_si_start_offset_info->offset_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_start_offset_info->offset_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_rrc_si_start_offset(pp_buffer, &p_rrc_si_start_offset_info->si_start_offset[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_si_transmission_info_len
(
    rrc_si_transmission_info_t *p_rrc_si_transmission_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_si_transmission_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_transmission_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_transmission_info->si_index);

    /* Optional element */
    if(p_rrc_si_transmission_info->bitmask & RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_transmission_info->num_of_transmissions);
    }

    /* Optional element */
    if(p_rrc_si_transmission_info->bitmask & RRC_RRM_SI_START_OFFSET_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrc_si_start_offset_info_len(&p_rrc_si_transmission_info->si_start_offset_info);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_si_transmission_info
(
    U8  **pp_buffer,
    rrc_si_transmission_info_t *p_rrc_si_transmission_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_si_transmission_info != PNULL);

    /* This function composes rrc_si_transmission_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrc_si_transmission_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_si_transmission_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_si_transmission_info->si_index > 9))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_transmission_info->si_index] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_si_transmission_info->si_index);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_si_transmission_info->si_index, "si_index");
    *pp_buffer += sizeof(p_rrc_si_transmission_info->si_index);

    /* Optional element */
    if(p_rrc_si_transmission_info->bitmask & RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_transmission_info->num_of_transmissions < 1) || (p_rrc_si_transmission_info->num_of_transmissions > 8))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_transmission_info->num_of_transmissions] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_si_transmission_info->num_of_transmissions);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_si_transmission_info->num_of_transmissions, "num_of_transmissions");
    *pp_buffer += sizeof(p_rrc_si_transmission_info->num_of_transmissions);
    }

    /* Optional element */
    if(p_rrc_si_transmission_info->bitmask & RRC_RRM_SI_START_OFFSET_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrc_si_start_offset_info(pp_buffer, &p_rrc_si_transmission_info->si_start_offset_info))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_si_transmission_info_list_len
(
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_si_transmission_info_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_si_transmission_info_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_transmission_info_list->count < 1) || (p_rrc_si_transmission_info_list->count > MAX_SI_MESSAGE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_transmission_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %d received.", p_rrc_si_transmission_info_list->count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_transmission_info_list->count; loop++)
        {
            length += rrm_il_get_rrc_si_transmission_info_len(&p_rrc_si_transmission_info_list->si_transmission[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_si_transmission_info_list
(
    U8  **pp_buffer,
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_si_transmission_info_list != PNULL);

    /* This function composes rrc_si_transmission_info_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_si_transmission_info_list->count < 1) || (p_rrc_si_transmission_info_list->count > MAX_SI_MESSAGE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_si_transmission_info_list->count] should be in range "
            "1 to MAX_SI_MESSAGE. Incorrect value %u received.", p_rrc_si_transmission_info_list->count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_si_transmission_info_list->count, "count");
    *pp_buffer += sizeof(p_rrc_si_transmission_info_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_si_transmission_info_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_rrc_si_transmission_info(pp_buffer, &p_rrc_si_transmission_info_list->si_transmission[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_etws_scheduling_info_len
(
    etws_scheduling_info_t *p_etws_scheduling_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_etws_scheduling_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_etws_scheduling_info->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_etws_scheduling_info->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_etws_scheduling_info->serial_number);

    /* Optional element */
    if(p_etws_scheduling_info->bitmask & RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_etws_scheduling_info->num_of_segments);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_etws_scheduling_info
(
    U8  **pp_buffer,
    etws_scheduling_info_t *p_etws_scheduling_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_etws_scheduling_info != PNULL);

    /* This function composes etws_scheduling_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_etws_scheduling_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_etws_scheduling_info->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_etws_scheduling_info->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_etws_scheduling_info->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_etws_scheduling_info->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_etws_scheduling_info->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_etws_scheduling_info->bitmask & RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_etws_scheduling_info->num_of_segments < 1) || (p_etws_scheduling_info->num_of_segments > 64))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_etws_scheduling_info->num_of_segments] should be in range "
            "1 to 64. Incorrect value %u received.", p_etws_scheduling_info->num_of_segments);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_etws_scheduling_info->num_of_segments, "num_of_segments");
    *pp_buffer += sizeof(p_etws_scheduling_info->num_of_segments);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_cmas_scheduling_info_len
(
    cmas_scheduling_info_t *p_cmas_scheduling_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_cmas_scheduling_info != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cmas_scheduling_info->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cmas_scheduling_info->serial_number);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_cmas_scheduling_info->num_of_segments);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_cmas_scheduling_info->num_of_repetitions);

    return length;
}

static
rrm_return_et
rrm_il_compose_cmas_scheduling_info
(
    U8  **pp_buffer,
    cmas_scheduling_info_t *p_cmas_scheduling_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_cmas_scheduling_info != PNULL);

    /* This function composes cmas_scheduling_info */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cmas_scheduling_info->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_cmas_scheduling_info->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cmas_scheduling_info->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_cmas_scheduling_info->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info->num_of_segments < 1) || (p_cmas_scheduling_info->num_of_segments > 64))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_cmas_scheduling_info->num_of_segments] should be in range "
            "1 to 64. Incorrect value %u received.", p_cmas_scheduling_info->num_of_segments);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_cmas_scheduling_info->num_of_segments, "num_of_segments");
    *pp_buffer += sizeof(p_cmas_scheduling_info->num_of_segments);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info->num_of_repetitions < 1) || (p_cmas_scheduling_info->num_of_repetitions > 4095))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_cmas_scheduling_info->num_of_repetitions] should be in range "
            "1 to 4095. Incorrect value %u received.", p_cmas_scheduling_info->num_of_repetitions);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_cmas_scheduling_info->num_of_repetitions, "num_of_repetitions");
    *pp_buffer += sizeof(p_cmas_scheduling_info->num_of_repetitions);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_cmas_scheduling_info_list_len
(
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_cmas_scheduling_info_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_cmas_scheduling_info_list->cmas_warning_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info_list->cmas_warning_count < 1) || (p_cmas_scheduling_info_list->cmas_warning_count > MAX_NUM_CMAS_WARNING))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_cmas_scheduling_info_list->cmas_warning_count] should be in range "
            "1 to MAX_NUM_CMAS_WARNING. Incorrect value %d received.", p_cmas_scheduling_info_list->cmas_warning_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cmas_scheduling_info_list->cmas_warning_count; loop++)
        {
            length += rrm_il_get_cmas_scheduling_info_len(&p_cmas_scheduling_info_list->cmas_scheduling_info[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_cmas_scheduling_info_list
(
    U8  **pp_buffer,
    cmas_scheduling_info_list_t *p_cmas_scheduling_info_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_cmas_scheduling_info_list != PNULL);

    /* This function composes cmas_scheduling_info_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_cmas_scheduling_info_list->cmas_warning_count < 1) || (p_cmas_scheduling_info_list->cmas_warning_count > MAX_NUM_CMAS_WARNING))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_cmas_scheduling_info_list->cmas_warning_count] should be in range "
            "1 to MAX_NUM_CMAS_WARNING. Incorrect value %u received.", p_cmas_scheduling_info_list->cmas_warning_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_cmas_scheduling_info_list->cmas_warning_count, "cmas_warning_count");
    *pp_buffer += sizeof(p_cmas_scheduling_info_list->cmas_warning_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_cmas_scheduling_info_list->cmas_warning_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_cmas_scheduling_info(pp_buffer, &p_cmas_scheduling_info_list->cmas_scheduling_info[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_rrm_pws_contents_len
(
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_rrm_pws_contents != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_rrm_pws_contents->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_rrm_pws_contents->cell_index);

    /* Get length of IE */
    length += rrm_il_get_scheduling_info_list_len(&p_rrc_rrm_pws_contents->scheduling_info_list);

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrc_si_transmission_info_list_len(&p_rrc_rrm_pws_contents->si_transmission_info);
    }

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_etws_scheduling_info_len(&p_rrc_rrm_pws_contents->etws_scheduling_info);
    }

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_cmas_scheduling_info_list_len(&p_rrc_rrm_pws_contents->cmas_scheduling_info_list);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_rrm_pws_contents
(
    U8  **pp_buffer,
    rrc_rrm_pws_contents_t *p_rrc_rrm_pws_contents
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_rrm_pws_contents != PNULL);

    /* This function composes rrc_rrm_pws_contents */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrc_rrm_pws_contents->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rrm_pws_contents->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rrm_pws_contents->cell_index > MAX_CELL_INDEX))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_rrm_pws_contents->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rrm_pws_contents->cell_index);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_rrm_pws_contents->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_rrm_pws_contents->cell_index);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_scheduling_info_list(pp_buffer, &p_rrc_rrm_pws_contents->scheduling_info_list))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrc_si_transmission_info_list(pp_buffer, &p_rrc_rrm_pws_contents->si_transmission_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_etws_scheduling_info(pp_buffer, &p_rrc_rrm_pws_contents->etws_scheduling_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rrm_pws_contents->bitmask & RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_cmas_scheduling_info_list(pp_buffer, &p_rrc_rrm_pws_contents->cmas_scheduling_info_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rrc_rrm_pws_contents_list_len
(
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrc_rrm_pws_contents_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rrc_rrm_pws_contents_list->cell_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rrm_pws_contents_list->cell_count < 1) || (p_rrc_rrm_pws_contents_list->cell_count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_rrm_pws_contents_list->cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_rrc_rrm_pws_contents_list->cell_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rrm_pws_contents_list->cell_count; loop++)
        {
            length += rrm_il_get_rrc_rrm_pws_contents_len(&p_rrc_rrm_pws_contents_list->rrc_rrm_pws_contents[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rrc_rrm_pws_contents_list
(
    U8  **pp_buffer,
    rrc_rrm_pws_contents_list_t *p_rrc_rrm_pws_contents_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrc_rrm_pws_contents_list != PNULL);

    /* This function composes rrc_rrm_pws_contents_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rrm_pws_contents_list->cell_count < 1) || (p_rrc_rrm_pws_contents_list->cell_count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rrc_rrm_pws_contents_list->cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_rrc_rrm_pws_contents_list->cell_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrc_rrm_pws_contents_list->cell_count, "cell_count");
    *pp_buffer += sizeof(p_rrc_rrm_pws_contents_list->cell_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rrm_pws_contents_list->cell_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_rrc_rrm_pws_contents(pp_buffer, &p_rrc_rrm_pws_contents_list->rrc_rrm_pws_contents[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_pws_cnf_len
(
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_pws_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_cnf->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_cnf->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_pws_cnf->serial_number);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_pws_cnf->pws_resp);

    /* Optional element */
    if(p_s1ap_rrm_pws_cnf->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG)
    {

    /* Get length of IE */
    length += rrm_il_get_pws_broadcast_list_len(&p_s1ap_rrm_pws_cnf->pws_broadcast_list);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_cnf
(
    U8  **pp_buffer,
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_pws_cnf != PNULL);

    /* This function composes s1ap_rrm_pws_cnf */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_pws_cnf->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_cnf->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_cnf->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_cnf->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_pws_cnf->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_cnf->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_pws_cnf->pws_resp > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_pws_cnf->pws_resp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_pws_cnf->pws_resp);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_pws_cnf->pws_resp, "pws_resp");
    *pp_buffer += sizeof(p_s1ap_rrm_pws_cnf->pws_resp);

    /* Optional element */
    if(p_s1ap_rrm_pws_cnf->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_pws_broadcast_list(pp_buffer, &p_s1ap_rrm_pws_cnf->pws_broadcast_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_pws_cnf
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_PWS_CNF message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_pws_cnf
(
    s1ap_rrm_pws_cnf_t  *p_s1ap_rrm_pws_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_pws_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_pws_cnf_len(p_s1ap_rrm_pws_cnf);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_PWS_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_PWS_CNF, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_PWS_CNF, msg_length,cell_index);

    /* Fill S1AP_RRM_PWS_CNF message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_pws_cnf(&p_s1apRrm_msg, p_s1ap_rrm_pws_cnf))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_pws_broadcast_resp_len
(
    pws_broadcast_resp_t *p_pws_broadcast_resp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_pws_broadcast_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_pws_broadcast_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_pws_broadcast_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_pws_broadcast_resp->response);

    /* Optional element */
    if(p_pws_broadcast_resp->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_pws_broadcast_resp->fail_cause);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_pws_broadcast_resp
(
    U8  **pp_buffer,
    pws_broadcast_resp_t *p_pws_broadcast_resp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_pws_broadcast_resp != PNULL);

    /* This function composes pws_broadcast_resp */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_pws_broadcast_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_pws_broadcast_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->cell_index > MAX_CELL_INDEX))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_pws_broadcast_resp->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_pws_broadcast_resp->cell_index);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_pws_broadcast_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_pws_broadcast_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->response > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_pws_broadcast_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_pws_broadcast_resp->response);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_pws_broadcast_resp->response, "response");
    *pp_buffer += sizeof(p_pws_broadcast_resp->response);

    /* Optional element */
    if(p_pws_broadcast_resp->bitmask & S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_pws_broadcast_resp->fail_cause > 3))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_pws_broadcast_resp->fail_cause] should be less than"
            " or equal to 3. Incorrect value %u received.", p_pws_broadcast_resp->fail_cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_pws_broadcast_resp->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_pws_broadcast_resp->fail_cause);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_pws_broadcast_list_len
(
    pws_broadcast_list_t *p_pws_broadcast_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_pws_broadcast_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_pws_broadcast_list->count);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_pws_broadcast_list->pws_broadcast_cell_resp); loop++)
        {
            length += rrm_il_get_pws_broadcast_resp_len(&p_pws_broadcast_list->pws_broadcast_cell_resp[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_pws_broadcast_list
(
    U8  **pp_buffer,
    pws_broadcast_list_t *p_pws_broadcast_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_pws_broadcast_list != PNULL);

    /* This function composes pws_broadcast_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_pws_broadcast_list->count < 1) || (p_pws_broadcast_list->count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_pws_broadcast_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_pws_broadcast_list->count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_pws_broadcast_list->count, "count");
    *pp_buffer += sizeof(p_pws_broadcast_list->count);

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_pws_broadcast_list->pws_broadcast_cell_resp); loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_pws_broadcast_resp(pp_buffer, &p_pws_broadcast_list->pws_broadcast_cell_resp[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_kill_request_len
(
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_kill_request != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_request->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_request->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_request->serial_number);

    /* Optional element */
    if(p_s1ap_rrm_kill_request->bitmask & KILL_WARNING_AREA_LIST)
    {

    /* Get length of IE */
    length += rrm_il_get_warning_area_list_info_len(&p_s1ap_rrm_kill_request->warning_area_list);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_request
(
    U8  **pp_buffer,
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_kill_request != PNULL);

    /* This function composes s1ap_rrm_kill_request */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_kill_request->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_request->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_request->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_request->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_request->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_request->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_s1ap_rrm_kill_request->bitmask & KILL_WARNING_AREA_LIST)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_warning_area_list_info(pp_buffer, &p_s1ap_rrm_kill_request->warning_area_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_kill_request
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_KILL_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_kill_request
(
    s1ap_rrm_kill_request_t  *p_s1ap_rrm_kill_request,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_kill_request != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_kill_request_len(p_s1ap_rrm_kill_request);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_KILL_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_KILL_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_KILL_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_KILL_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_kill_request(&p_s1apRrm_msg, p_s1ap_rrm_kill_request))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_kill_response_len
(
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_kill_response != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_response->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_response->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_response->serial_number);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_response->response);

    /* Optional element */
    if(p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_FAIL_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_response->fail_cause);
    }

    /* Optional element */
    if(p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rrc_rrm_pws_contents_list_len(&p_s1ap_rrm_kill_response->rrc_rrm_pws_contents_list);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_response
(
    U8  **pp_buffer,
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_kill_response != PNULL);

    /* This function composes s1ap_rrm_kill_response */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_kill_response->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_response->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_response->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_response->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_response->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_response->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_response->response > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_kill_response->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_kill_response->response);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_response->response, "response");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_response->response);

    /* Optional element */
    if(p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_FAIL_CAUSE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_response->fail_cause > 4))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_kill_response->fail_cause] should be less than"
            " or equal to 4. Incorrect value %u received.", p_s1ap_rrm_kill_response->fail_cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_response->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_response->fail_cause);
    }

    /* Optional element */
    if(p_s1ap_rrm_kill_response->bitmask & S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rrc_rrm_pws_contents_list(pp_buffer, &p_s1ap_rrm_kill_response->rrc_rrm_pws_contents_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_kill_response
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_KILL_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_kill_response
(
    s1ap_rrm_kill_response_t  *p_s1ap_rrm_kill_response,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_kill_response != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_kill_response_len(p_s1ap_rrm_kill_response);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_KILL_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_KILL_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_KILL_RESP, msg_length,cell_index);

    /* Fill S1AP_RRM_KILL_RESP message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_kill_response(&p_s1apRrm_msg, p_s1ap_rrm_kill_response))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_kill_cnf_len
(
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_kill_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_cnf->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_cnf->message_identifier);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_kill_cnf->serial_number);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_kill_cnf->kill_resp);

    /* Optional element */
    if(p_s1ap_rrm_kill_cnf->bitmask & S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_broadcast_cancellation_list_len(&p_s1ap_rrm_kill_cnf->broadcast_cancellation_list);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_cnf
(
    U8  **pp_buffer,
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_kill_cnf != PNULL);

    /* This function composes s1ap_rrm_kill_cnf */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_kill_cnf->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_cnf->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_cnf->message_identifier); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_cnf->message_identifier[loop], "message_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_kill_cnf->serial_number); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_cnf->serial_number[loop], "serial_number[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_kill_cnf->kill_resp > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_kill_cnf->kill_resp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_kill_cnf->kill_resp);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_kill_cnf->kill_resp, "kill_resp");
    *pp_buffer += sizeof(p_s1ap_rrm_kill_cnf->kill_resp);

    /* Optional element */
    if(p_s1ap_rrm_kill_cnf->bitmask & S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_broadcast_cancellation_list(pp_buffer, &p_s1ap_rrm_kill_cnf->broadcast_cancellation_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_kill_cnf
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_KILL_CNF message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_kill_cnf
(
    s1ap_rrm_kill_cnf_t  *p_s1ap_rrm_kill_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_kill_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_kill_cnf_len(p_s1ap_rrm_kill_cnf);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_KILL_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_KILL_CNF, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_KILL_CNF, msg_length,cell_index);

    /* Fill S1AP_RRM_KILL_CNF message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_kill_cnf(&p_s1apRrm_msg, p_s1ap_rrm_kill_cnf))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_broadcast_cancellation_len
(
    broadcast_cancellation_t *p_broadcast_cancellation
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_broadcast_cancellation != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_broadcast_cancellation->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_broadcast_cancellation->cell_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_broadcast_cancellation->response);

    /* Optional element */
    if(p_broadcast_cancellation->bitmask & S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_broadcast_cancellation->fail_cause);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_broadcast_cancellation
(
    U8  **pp_buffer,
    broadcast_cancellation_t *p_broadcast_cancellation
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_broadcast_cancellation != PNULL);

    /* This function composes broadcast_cancellation */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_broadcast_cancellation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_broadcast_cancellation->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->cell_index > MAX_CELL_INDEX))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_broadcast_cancellation->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_broadcast_cancellation->cell_index);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_broadcast_cancellation->cell_index, "cell_index");
    *pp_buffer += sizeof(p_broadcast_cancellation->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->response > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_broadcast_cancellation->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_broadcast_cancellation->response);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_broadcast_cancellation->response, "response");
    *pp_buffer += sizeof(p_broadcast_cancellation->response);

    /* Optional element */
    if(p_broadcast_cancellation->bitmask & S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_broadcast_cancellation->fail_cause > 4))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_broadcast_cancellation->fail_cause] should be less than"
            " or equal to 4. Incorrect value %u received.", p_broadcast_cancellation->fail_cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_broadcast_cancellation->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_broadcast_cancellation->fail_cause);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_broadcast_cancellation_list_len
(
    broadcast_cancellation_list_t *p_broadcast_cancellation_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_broadcast_cancellation_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_broadcast_cancellation_list->count);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_broadcast_cancellation_list->broadcast_cancellation); loop++)
        {
            length += rrm_il_get_broadcast_cancellation_len(&p_broadcast_cancellation_list->broadcast_cancellation[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_broadcast_cancellation_list
(
    U8  **pp_buffer,
    broadcast_cancellation_list_t *p_broadcast_cancellation_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_broadcast_cancellation_list != PNULL);

    /* This function composes broadcast_cancellation_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_broadcast_cancellation_list->count < 1) || (p_broadcast_cancellation_list->count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_broadcast_cancellation_list->count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_broadcast_cancellation_list->count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_broadcast_cancellation_list->count, "count");
    *pp_buffer += sizeof(p_broadcast_cancellation_list->count);

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_broadcast_cancellation_list->broadcast_cancellation); loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_broadcast_cancellation(pp_buffer, &p_broadcast_cancellation_list->broadcast_cancellation[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_rim_information_req_len
(
    rim_information_req_t *p_rim_information_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_information_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_req->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_req->dst_cell);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_req->src_cell);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_req->app_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_req->seq_num);

    /* Get length of IE */
    length += rrm_il_get_rim_pdu_ind_rim_info_req_len(&p_rim_information_req->pdu_ind);

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_PROTOCOL_VER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_req->ver);
    }

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_APP_CONTAINER_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_container_len(&p_rim_information_req->app_cont);
    }

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_son_transfer_app_idty_len(&p_rim_information_req->son_app_idty);
    }

    return length;
}

rrm_return_et
rrm_il_compose_rim_information_req
(
    U8  **pp_buffer,
    rim_information_req_t *p_rim_information_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_information_req != PNULL);

    /* This function composes rim_information_req */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_information_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_information_req->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_req->mme_id, "mme_id");
    *pp_buffer += sizeof(p_rim_information_req->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_req->dst_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_req->src_cell))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_req->app_idty < 1) || (p_rim_information_req->app_idty > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_req->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_req->app_idty);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_req->app_idty, "app_idty");
    *pp_buffer += sizeof(p_rim_information_req->app_idty);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_information_req->seq_num, "seq_num");
    *pp_buffer += sizeof(p_rim_information_req->seq_num);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_pdu_ind_rim_info_req(pp_buffer, &p_rim_information_req->pdu_ind))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_PROTOCOL_VER_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_req->ver < 1) || (p_rim_information_req->ver > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_req->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_req->ver);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_req->ver, "ver");
    *pp_buffer += sizeof(p_rim_information_req->ver);
    }

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_APP_CONTAINER_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_container(pp_buffer, &p_rim_information_req->app_cont))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_information_req->bitmask & RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_son_transfer_app_idty(pp_buffer, &p_rim_information_req->son_app_idty))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_rim_information_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_RIM_INFO_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_rim_information_req
(
    rim_information_req_t  *p_rim_information_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_rim_information_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_rim_information_req_len(p_rim_information_req);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_RIM_INFO_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_RIM_INFO_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_rim_information_req(&p_s1apRrm_msg, p_rim_information_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_plmn_len
(
    rim_plmn_t *p_rim_plmn
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_plmn != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_plmn->plmn);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_plmn
(
    U8  **pp_buffer,
    rim_plmn_t *p_rim_plmn
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_plmn != PNULL);

    /* This function composes rim_plmn */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_plmn->plmn); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_plmn->plmn[loop], "plmn[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_routing_identification_len
(
    rim_routing_identification_t *p_rim_routing_identification
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_routing_identification != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_plmn_len(&p_rim_routing_identification->plmn);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_routing_identification->lac);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_routing_identification->rac);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_routing_identification
(
    U8  **pp_buffer,
    rim_routing_identification_t *p_rim_routing_identification
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_routing_identification != PNULL);

    /* This function composes rim_routing_identification */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_plmn(pp_buffer, &p_rim_routing_identification->plmn))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_routing_identification->lac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_routing_identification->lac[loop], "lac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_routing_identification->rac, "rac");
    *pp_buffer += sizeof(p_rim_routing_identification->rac);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_geran_routing_addr_len
(
    rim_geran_routing_addr_t *p_rim_geran_routing_addr
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_geran_routing_addr != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_identification_len(&p_rim_geran_routing_addr->routing_idty);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_geran_routing_addr->geran_cell_idty);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_geran_routing_addr
(
    U8  **pp_buffer,
    rim_geran_routing_addr_t *p_rim_geran_routing_addr
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_geran_routing_addr != PNULL);

    /* This function composes rim_geran_routing_addr */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_identification(pp_buffer, &p_rim_geran_routing_addr->routing_idty))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_geran_routing_addr->geran_cell_idty); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_geran_routing_addr->geran_cell_idty[loop], "geran_cell_idty[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_utran_routing_addr_len
(
    rim_utran_routing_addr_t *p_rim_utran_routing_addr
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_utran_routing_addr != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_identification_len(&p_rim_utran_routing_addr->routing_idty);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_utran_routing_addr->rnc_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_utran_routing_addr
(
    U8  **pp_buffer,
    rim_utran_routing_addr_t *p_rim_utran_routing_addr
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_utran_routing_addr != PNULL);

    /* This function composes rim_utran_routing_addr */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_identification(pp_buffer, &p_rim_utran_routing_addr->routing_idty))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_utran_routing_addr->rnc_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_utran_routing_addr->rnc_id[loop], "rnc_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_ta_identity_len
(
    rim_ta_identity_t *p_rim_ta_identity
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_ta_identity != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_plmn_len(&p_rim_ta_identity->plmn);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_ta_identity->tac);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_ta_identity
(
    U8  **pp_buffer,
    rim_ta_identity_t *p_rim_ta_identity
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_ta_identity != PNULL);

    /* This function composes rim_ta_identity */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_plmn(pp_buffer, &p_rim_ta_identity->plmn))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_ta_identity->tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_ta_identity->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_global_enb_id_len
(
    rim_global_enb_id_t *p_rim_global_enb_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_global_enb_id != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_plmn_len(&p_rim_global_enb_id->plmn);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_global_enb_id->enb_type);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_global_enb_id->enb_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_global_enb_id
(
    U8  **pp_buffer,
    rim_global_enb_id_t *p_rim_global_enb_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_global_enb_id != PNULL);

    /* This function composes rim_global_enb_id */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_plmn(pp_buffer, &p_rim_global_enb_id->plmn))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_global_enb_id->enb_type, "enb_type");
    *pp_buffer += sizeof(p_rim_global_enb_id->enb_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_global_enb_id->enb_id, "enb_id");
    *pp_buffer += sizeof(p_rim_global_enb_id->enb_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_eutran_routing_addr_len
(
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_eutran_routing_addr != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_ta_identity_len(&p_rim_eutran_routing_addr->ta_idty);

    /* Get length of IE */
    length += rrm_il_get_rim_global_enb_id_len(&p_rim_eutran_routing_addr->enb);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_eutran_routing_addr
(
    U8  **pp_buffer,
    rim_eutran_routing_addr_t *p_rim_eutran_routing_addr
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_eutran_routing_addr != PNULL);

    /* This function composes rim_eutran_routing_addr */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_ta_identity(pp_buffer, &p_rim_eutran_routing_addr->ta_idty))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_global_enb_id(pp_buffer, &p_rim_eutran_routing_addr->enb))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_routing_info_len
(
    rim_routing_info_t *p_rim_routing_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_routing_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_routing_info->bitmask);

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_geran_routing_addr_len(&p_rim_routing_info->geran_rtng_addr);
    }

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_utran_routing_addr_len(&p_rim_routing_info->utran_rtng_addr);
    }

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_eutran_routing_addr_len(&p_rim_routing_info->eutran_rtng_addr);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_routing_info
(
    U8  **pp_buffer,
    rim_routing_info_t *p_rim_routing_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_routing_info != PNULL);

    /* This function composes rim_routing_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_routing_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_routing_info->bitmask);

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_geran_routing_addr(pp_buffer, &p_rim_routing_info->geran_rtng_addr))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_utran_routing_addr(pp_buffer, &p_rim_routing_info->utran_rtng_addr))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_routing_info->bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_eutran_routing_addr(pp_buffer, &p_rim_routing_info->eutran_rtng_addr))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_info_req_len
(
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_pdu_ind_rim_info_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_info_req->pdu_ext);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_info_req->ack);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_info_req
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_info_req_t *p_rim_pdu_ind_rim_info_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_pdu_ind_rim_info_req != PNULL);

    /* This function composes rim_pdu_ind_rim_info_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info_req->pdu_ext > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_info_req->pdu_ext] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rim_pdu_ind_rim_info_req->pdu_ext);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_info_req->pdu_ext, "pdu_ext");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_info_req->pdu_ext);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info_req->ack > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_info_req->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_info_req->ack);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_info_req->ack, "ack");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_info_req->ack);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_nacc_len
(
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_contrn_nacc != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_geran_routing_addr_len(&p_rim_req_app_contrn_nacc->cell_idty);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_nacc
(
    U8  **pp_buffer,
    rim_req_app_contrn_nacc_t *p_rim_req_app_contrn_nacc
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_contrn_nacc != PNULL);

    /* This function composes rim_req_app_contrn_nacc */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_geran_routing_addr(pp_buffer, &p_rim_req_app_contrn_nacc->cell_idty))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_si3_len
(
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_contrn_si3 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_req_app_contrn_si3->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_si3
(
    U8  **pp_buffer,
    rim_req_app_contrn_si3_t *p_rim_req_app_contrn_si3
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_contrn_si3 != PNULL);

    /* This function composes rim_req_app_contrn_si3 */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_req_app_contrn_si3->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_req_app_contrn_si3->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_mbms_len
(
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_contrn_mbms != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_req_app_contrn_mbms->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_mbms
(
    U8  **pp_buffer,
    rim_req_app_contrn_mbms_t *p_rim_req_app_contrn_mbms
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_contrn_mbms != PNULL);

    /* This function composes rim_req_app_contrn_mbms */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_req_app_contrn_mbms->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_req_app_contrn_mbms->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_son_len
(
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_contrn_son != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_req_app_contrn_son->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_son
(
    U8  **pp_buffer,
    rim_req_app_contrn_son_t *p_rim_req_app_contrn_son
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_contrn_son != PNULL);

    /* This function composes rim_req_app_contrn_son */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_req_app_contrn_son->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_req_app_contrn_son->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_utran_cell_idty_len
(
    rim_utran_cell_idty_t *p_rim_utran_cell_idty
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_utran_cell_idty != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_plmn_len(&p_rim_utran_cell_idty->plmn);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_utran_cell_idty->cell_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_utran_cell_idty
(
    U8  **pp_buffer,
    rim_utran_cell_idty_t *p_rim_utran_cell_idty
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_utran_cell_idty != PNULL);

    /* This function composes rim_utran_cell_idty */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_plmn(pp_buffer, &p_rim_utran_cell_idty->plmn))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_utran_cell_idty->cell_id, "cell_id");
    *pp_buffer += sizeof(p_rim_utran_cell_idty->cell_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_contrn_utra_si_len
(
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_contrn_utra_si != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_utran_cell_idty_len(&p_rim_req_app_contrn_utra_si->rpt_cell_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_contrn_utra_si
(
    U8  **pp_buffer,
    rim_req_app_contrn_utra_si_t *p_rim_req_app_contrn_utra_si
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_contrn_utra_si != PNULL);

    /* This function composes rim_req_app_contrn_utra_si */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_utran_cell_idty(pp_buffer, &p_rim_req_app_contrn_utra_si->rpt_cell_id))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_req_app_container_len
(
    rim_req_app_container_t *p_rim_req_app_container
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_req_app_container != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_req_app_container->bitmask);

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_NACC_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_nacc_len(&p_rim_req_app_container->nacc);
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SI3_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_si3_len(&p_rim_req_app_container->si3);
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_MBMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_mbms_len(&p_rim_req_app_container->mbms);
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SON_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_son_len(&p_rim_req_app_container->son);
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_utra_si_len(&p_rim_req_app_container->utra_si);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_req_app_container
(
    U8  **pp_buffer,
    rim_req_app_container_t *p_rim_req_app_container
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_req_app_container != PNULL);

    /* This function composes rim_req_app_container */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_req_app_container->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_req_app_container->bitmask);

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_NACC_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_nacc(pp_buffer, &p_rim_req_app_container->nacc))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SI3_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_si3(pp_buffer, &p_rim_req_app_container->si3))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_MBMS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_mbms(pp_buffer, &p_rim_req_app_container->mbms))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_SON_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_son(pp_buffer, &p_rim_req_app_container->son))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container->bitmask & RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_utra_si(pp_buffer, &p_rim_req_app_container->utra_si))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_son_transfer_app_idty_len
(
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_son_transfer_app_idty != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_son_transfer_app_idty->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_son_transfer_app_idty
(
    U8  **pp_buffer,
    rim_son_transfer_app_idty_t *p_rim_son_transfer_app_idty
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_son_transfer_app_idty != PNULL);

    /* This function composes rim_son_transfer_app_idty */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_son_transfer_app_idty->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_son_transfer_app_idty->bitmask);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_rim_information_len
(
    rim_information_t *p_rim_information
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_information != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information->dst_cell);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information->src_cell);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information->app_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information->seq_num);

    /* Get length of IE */
    length += rrm_il_get_rim_pdu_ind_rim_info_len(&p_rim_information->pdu_ind);

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_PROTOCOL_VER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information->ver);
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_APP_CONTAINER_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_container_len(&p_rim_information->app_cont);
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_APP_ERR_CONTAINER_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_container_len(&p_rim_information->err_cont);
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_SON_TRANSFER_APP_IDNTY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_son_transfer_app_idty_len(&p_rim_information->son_app_idty);
    }

    return length;
}

rrm_return_et
rrm_il_compose_rim_information
(
    U8  **pp_buffer,
    rim_information_t *p_rim_information
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_information != PNULL);

    /* This function composes rim_information */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_information->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_information->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information->mme_id, "mme_id");
    *pp_buffer += sizeof(p_rim_information->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information->dst_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information->src_cell))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information->app_idty < 1) || (p_rim_information->app_idty > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information->app_idty);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information->app_idty, "app_idty");
    *pp_buffer += sizeof(p_rim_information->app_idty);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_information->seq_num, "seq_num");
    *pp_buffer += sizeof(p_rim_information->seq_num);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_pdu_ind_rim_info(pp_buffer, &p_rim_information->pdu_ind))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_PROTOCOL_VER_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information->ver < 1) || (p_rim_information->ver > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information->ver);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information->ver, "ver");
    *pp_buffer += sizeof(p_rim_information->ver);
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_APP_CONTAINER_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_container(pp_buffer, &p_rim_information->app_cont))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_APP_ERR_CONTAINER_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_container(pp_buffer, &p_rim_information->err_cont))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_information->bitmask & RIM_INFO_SON_TRANSFER_APP_IDNTY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_son_transfer_app_idty(pp_buffer, &p_rim_information->son_app_idty))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_rim_information
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_RIM_INFO message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_rim_information
(
    rim_information_t  *p_rim_information,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_rim_information != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_rim_information_len(p_rim_information);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_RIM_INFO", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO, msg_length,cell_index);

    /* Fill S1AP_RRM_RIM_INFO message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_rim_information(&p_s1apRrm_msg, p_rim_information))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_info_len
(
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_pdu_ind_rim_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_info->pdu_ext);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_info->ack);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_info
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_info_t *p_rim_pdu_ind_rim_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_pdu_ind_rim_info != PNULL);

    /* This function composes rim_pdu_ind_rim_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info->pdu_ext > 4))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_info->pdu_ext] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rim_pdu_ind_rim_info->pdu_ext);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_info->pdu_ext, "pdu_ext");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_info->pdu_ext);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_info->ack > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_info->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_info->ack);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_info->ack, "ack");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_info->ack);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_si_psi_individual_len
(
    rim_si_psi_individual_t *p_rim_si_psi_individual
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_si_psi_individual != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rim_si_psi_individual->si_psi);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_si_psi_individual
(
    U8  **pp_buffer,
    rim_si_psi_individual_t *p_rim_si_psi_individual
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_si_psi_individual != PNULL);

    /* This function composes rim_si_psi_individual */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_si_psi_individual->si_psi); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_si_psi_individual->si_psi[loop], "si_psi[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_nacc_len
(
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_contrn_nacc != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_geran_routing_addr_len(&p_rim_info_app_contrn_nacc->cell_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_nacc->type);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_nacc->num_si_psi);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_nacc->num_si_psi > RIM_MAX_SI_PSI_PAYLOAD))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_info_app_contrn_nacc->num_si_psi] should be less than"
            " or equal to RIM_MAX_SI_PSI_PAYLOAD. Incorrect value %d received.", p_rim_info_app_contrn_nacc->num_si_psi);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_contrn_nacc->num_si_psi; loop++)
        {
            length += rrm_il_get_rim_si_psi_individual_len(&p_rim_info_app_contrn_nacc->si_psi_indiv[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_nacc
(
    U8  **pp_buffer,
    rim_info_app_contrn_nacc_t *p_rim_info_app_contrn_nacc
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_contrn_nacc != PNULL);

    /* This function composes rim_info_app_contrn_nacc */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_geran_routing_addr(pp_buffer, &p_rim_info_app_contrn_nacc->cell_idty))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_nacc->type > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_info_app_contrn_nacc->type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_info_app_contrn_nacc->type);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_info_app_contrn_nacc->type, "type");
    *pp_buffer += sizeof(p_rim_info_app_contrn_nacc->type);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_nacc->num_si_psi > RIM_MAX_SI_PSI_PAYLOAD))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_info_app_contrn_nacc->num_si_psi] should be less than"
            " or equal to RIM_MAX_SI_PSI_PAYLOAD. Incorrect value %u received.", p_rim_info_app_contrn_nacc->num_si_psi);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_info_app_contrn_nacc->num_si_psi, "num_si_psi");
    *pp_buffer += sizeof(p_rim_info_app_contrn_nacc->num_si_psi);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_contrn_nacc->num_si_psi; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_rim_si_psi_individual(pp_buffer, &p_rim_info_app_contrn_nacc->si_psi_indiv[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_si3_len
(
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_contrn_si3 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_si3->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_si3
(
    U8  **pp_buffer,
    rim_info_app_contrn_si3_t *p_rim_info_app_contrn_si3
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_contrn_si3 != PNULL);

    /* This function composes rim_info_app_contrn_si3 */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_info_app_contrn_si3->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_info_app_contrn_si3->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_mbms_len
(
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_contrn_mbms != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_mbms->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_mbms
(
    U8  **pp_buffer,
    rim_info_app_contrn_mbms_t *p_rim_info_app_contrn_mbms
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_contrn_mbms != PNULL);

    /* This function composes rim_info_app_contrn_mbms */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_info_app_contrn_mbms->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_info_app_contrn_mbms->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_son_len
(
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_contrn_son != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_son->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_son
(
    U8  **pp_buffer,
    rim_info_app_contrn_son_t *p_rim_info_app_contrn_son
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_contrn_son != PNULL);

    /* This function composes rim_info_app_contrn_son */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_info_app_contrn_son->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_info_app_contrn_son->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_contrn_utra_si_len
(
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_contrn_utra_si != PNULL);

    /* Get length of IE */
    length += rrm_il_get_rim_utran_cell_idty_len(&p_rim_info_app_contrn_utra_si->rpt_cell_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_contrn_utra_si->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rim_info_app_contrn_utra_si->len * sizeof(p_rim_info_app_contrn_utra_si->si[0]));

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_contrn_utra_si
(
    U8  **pp_buffer,
    rim_info_app_contrn_utra_si_t *p_rim_info_app_contrn_utra_si
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_contrn_utra_si != PNULL);

    /* This function composes rim_info_app_contrn_utra_si */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_utran_cell_idty(pp_buffer, &p_rim_info_app_contrn_utra_si->rpt_cell_id))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_info_app_contrn_utra_si->len > RIM_MAX_UTRA_SI_PAYLOAD))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_info_app_contrn_utra_si->len] should be less than"
            " or equal to RIM_MAX_UTRA_SI_PAYLOAD. Incorrect value %u received.", p_rim_info_app_contrn_utra_si->len);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_info_app_contrn_utra_si->len, "len");
    *pp_buffer += sizeof(p_rim_info_app_contrn_utra_si->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_contrn_utra_si->len; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rim_info_app_contrn_utra_si->si[loop], "si[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_info_app_container_len
(
    rim_info_app_container_t *p_rim_info_app_container
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_info_app_container != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_info_app_container->bitmask);

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_NACC_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_nacc_len(&p_rim_info_app_container->nacc);
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SI3_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_si3_len(&p_rim_info_app_container->si3);
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_MBMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_mbms_len(&p_rim_info_app_container->mbms);
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SON_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_son_len(&p_rim_info_app_container->son);
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_utra_si_len(&p_rim_info_app_container->utra_si);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_info_app_container
(
    U8  **pp_buffer,
    rim_info_app_container_t *p_rim_info_app_container
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_info_app_container != PNULL);

    /* This function composes rim_info_app_container */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_info_app_container->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_info_app_container->bitmask);

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_NACC_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_nacc(pp_buffer, &p_rim_info_app_container->nacc))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SI3_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_si3(pp_buffer, &p_rim_info_app_container->si3))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_MBMS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_mbms(pp_buffer, &p_rim_info_app_container->mbms))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_SON_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_son(pp_buffer, &p_rim_info_app_container->son))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_container->bitmask & RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_utra_si(pp_buffer, &p_rim_info_app_container->utra_si))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_nacc_len
(
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_contrn_nacc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_nacc->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_nacc->cause);

    /* Optional element */
    if(p_rim_app_err_contrn_nacc->bitmask & RIM_REQ_NACC_PDU_IN_ERROR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_nacc_len(&p_rim_app_err_contrn_nacc->req_err_pdu);
    }

    /* Optional element */
    if(p_rim_app_err_contrn_nacc->bitmask & RIM_INFO_NACC_PDU_IN_ERROR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_nacc_len(&p_rim_app_err_contrn_nacc->info_err_pdu);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_nacc
(
    U8  **pp_buffer,
    rim_app_err_contrn_nacc_t *p_rim_app_err_contrn_nacc
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_contrn_nacc != PNULL);

    /* This function composes rim_app_err_contrn_nacc */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_contrn_nacc->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_contrn_nacc->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_app_err_contrn_nacc->cause > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_app_err_contrn_nacc->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rim_app_err_contrn_nacc->cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_app_err_contrn_nacc->cause, "cause");
    *pp_buffer += sizeof(p_rim_app_err_contrn_nacc->cause);

    /* Optional element */
    if(p_rim_app_err_contrn_nacc->bitmask & RIM_REQ_NACC_PDU_IN_ERROR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_nacc(pp_buffer, &p_rim_app_err_contrn_nacc->req_err_pdu))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_contrn_nacc->bitmask & RIM_INFO_NACC_PDU_IN_ERROR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_nacc(pp_buffer, &p_rim_app_err_contrn_nacc->info_err_pdu))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_si3_len
(
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_contrn_si3 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_si3->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_si3
(
    U8  **pp_buffer,
    rim_app_err_contrn_si3_t *p_rim_app_err_contrn_si3
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_contrn_si3 != PNULL);

    /* This function composes rim_app_err_contrn_si3 */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_contrn_si3->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_contrn_si3->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_mbms_len
(
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_contrn_mbms != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_mbms->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_mbms
(
    U8  **pp_buffer,
    rim_app_err_contrn_mbms_t *p_rim_app_err_contrn_mbms
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_contrn_mbms != PNULL);

    /* This function composes rim_app_err_contrn_mbms */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_contrn_mbms->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_contrn_mbms->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_son_len
(
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_contrn_son != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_son->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_son
(
    U8  **pp_buffer,
    rim_app_err_contrn_son_t *p_rim_app_err_contrn_son
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_contrn_son != PNULL);

    /* This function composes rim_app_err_contrn_son */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_contrn_son->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_contrn_son->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_contrn_utra_si_len
(
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_contrn_utra_si != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_utra_si->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_contrn_utra_si->cause);

    /* Optional element */
    if(p_rim_app_err_contrn_utra_si->bitmask & RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_req_app_contrn_utra_si_len(&p_rim_app_err_contrn_utra_si->req_err_pdu);
    }

    /* Optional element */
    if(p_rim_app_err_contrn_utra_si->bitmask & RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_info_app_contrn_utra_si_len(&p_rim_app_err_contrn_utra_si->info_err_pdu);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_contrn_utra_si
(
    U8  **pp_buffer,
    rim_app_err_contrn_utra_si_t *p_rim_app_err_contrn_utra_si
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_contrn_utra_si != PNULL);

    /* This function composes rim_app_err_contrn_utra_si */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_contrn_utra_si->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_contrn_utra_si->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_app_err_contrn_utra_si->cause > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_app_err_contrn_utra_si->cause] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rim_app_err_contrn_utra_si->cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_app_err_contrn_utra_si->cause, "cause");
    *pp_buffer += sizeof(p_rim_app_err_contrn_utra_si->cause);

    /* Optional element */
    if(p_rim_app_err_contrn_utra_si->bitmask & RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_req_app_contrn_utra_si(pp_buffer, &p_rim_app_err_contrn_utra_si->req_err_pdu))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_contrn_utra_si->bitmask & RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_info_app_contrn_utra_si(pp_buffer, &p_rim_app_err_contrn_utra_si->info_err_pdu))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_app_err_container_len
(
    rim_app_err_container_t *p_rim_app_err_container
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_app_err_container != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_app_err_container->bitmask);

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_NACC_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_contrn_nacc_len(&p_rim_app_err_container->nacc);
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SI3_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_contrn_si3_len(&p_rim_app_err_container->si3);
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_MBMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_contrn_mbms_len(&p_rim_app_err_container->mbms);
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SON_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_contrn_son_len(&p_rim_app_err_container->son);
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_contrn_utra_si_len(&p_rim_app_err_container->utra_si);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_app_err_container
(
    U8  **pp_buffer,
    rim_app_err_container_t *p_rim_app_err_container
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_app_err_container != PNULL);

    /* This function composes rim_app_err_container */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_app_err_container->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_app_err_container->bitmask);

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_NACC_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_contrn_nacc(pp_buffer, &p_rim_app_err_container->nacc))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SI3_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_contrn_si3(pp_buffer, &p_rim_app_err_container->si3))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_MBMS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_contrn_mbms(pp_buffer, &p_rim_app_err_container->mbms))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_SON_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_contrn_son(pp_buffer, &p_rim_app_err_container->son))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_app_err_container->bitmask & RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_contrn_utra_si(pp_buffer, &p_rim_app_err_container->utra_si))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_rim_information_ack_len
(
    rim_information_ack_t *p_rim_information_ack
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_information_ack != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_ack->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_ack->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_ack->dst_cell);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_ack->src_cell);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_ack->app_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_ack->seq_num);

    /* Optional element */
    if(p_rim_information_ack->bitmask & RIM_INFO_ACK_PROTOCOL_VER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_ack->ver);
    }

    /* Optional element */
    if(p_rim_information_ack->bitmask & RIM_INFO_ACK_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_son_transfer_app_idty_len(&p_rim_information_ack->son_app_idty);
    }

    return length;
}

rrm_return_et
rrm_il_compose_rim_information_ack
(
    U8  **pp_buffer,
    rim_information_ack_t *p_rim_information_ack
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_information_ack != PNULL);

    /* This function composes rim_information_ack */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_information_ack->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_information_ack->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_ack->mme_id, "mme_id");
    *pp_buffer += sizeof(p_rim_information_ack->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_ack->dst_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_ack->src_cell))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_ack->app_idty < 1) || (p_rim_information_ack->app_idty > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_ack->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_ack->app_idty);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_ack->app_idty, "app_idty");
    *pp_buffer += sizeof(p_rim_information_ack->app_idty);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_information_ack->seq_num, "seq_num");
    *pp_buffer += sizeof(p_rim_information_ack->seq_num);

    /* Optional element */
    if(p_rim_information_ack->bitmask & RIM_INFO_ACK_PROTOCOL_VER_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_ack->ver < 1) || (p_rim_information_ack->ver > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_ack->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_ack->ver);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_ack->ver, "ver");
    *pp_buffer += sizeof(p_rim_information_ack->ver);
    }

    /* Optional element */
    if(p_rim_information_ack->bitmask & RIM_INFO_ACK_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_son_transfer_app_idty(pp_buffer, &p_rim_information_ack->son_app_idty))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_rim_information_ack
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_RIM_INFO_ACK message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_rim_information_ack
(
    rim_information_ack_t  *p_rim_information_ack,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_rim_information_ack != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_rim_information_ack_len(p_rim_information_ack);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_RIM_INFO_ACK", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_ACK, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_ACK, msg_length,cell_index);

    /* Fill S1AP_RRM_RIM_INFO_ACK message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_rim_information_ack(&p_s1apRrm_msg, p_rim_information_ack))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_rim_information_app_err_len
(
    rim_information_app_err_t *p_rim_information_app_err
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_information_app_err != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_app_err->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_app_err->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_app_err->dst_cell);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_app_err->src_cell);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_app_err->app_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_app_err->seq_num);

    /* Get length of IE */
    length += rrm_il_get_rim_pdu_ind_rim_app_err_len(&p_rim_information_app_err->pdu_ind);

    /* Optional element */
    if(p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_app_err->ver);
    }

    /* Get length of IE */
    length += rrm_il_get_rim_app_err_container_len(&p_rim_information_app_err->err_cont);

    /* Optional element */
    if(p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_son_transfer_app_idty_len(&p_rim_information_app_err->son_app_idty);
    }

    return length;
}

rrm_return_et
rrm_il_compose_rim_information_app_err
(
    U8  **pp_buffer,
    rim_information_app_err_t *p_rim_information_app_err
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_information_app_err != PNULL);

    /* This function composes rim_information_app_err */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_information_app_err->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_information_app_err->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_app_err->mme_id, "mme_id");
    *pp_buffer += sizeof(p_rim_information_app_err->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_app_err->dst_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_app_err->src_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_app_err->app_idty, "app_idty");
    *pp_buffer += sizeof(p_rim_information_app_err->app_idty);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rim_information_app_err->seq_num, "seq_num");
    *pp_buffer += sizeof(p_rim_information_app_err->seq_num);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_pdu_ind_rim_app_err(pp_buffer, &p_rim_information_app_err->pdu_ind))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_app_err->ver, "ver");
    *pp_buffer += sizeof(p_rim_information_app_err->ver);
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_app_err_container(pp_buffer, &p_rim_information_app_err->err_cont))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rim_information_app_err->bitmask & RIM_INFO_APP_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_son_transfer_app_idty(pp_buffer, &p_rim_information_app_err->son_app_idty))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_rim_information_app_err
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_RIM_INFO_APP_ERROR message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_rim_information_app_err
(
    rim_information_app_err_t  *p_rim_information_app_err,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_rim_information_app_err != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_rim_information_app_err_len(p_rim_information_app_err);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_RIM_INFO_APP_ERROR", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_APP_ERROR, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_APP_ERROR, msg_length,cell_index);

    /* Fill S1AP_RRM_RIM_INFO_APP_ERROR message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_rim_information_app_err(&p_s1apRrm_msg, p_rim_information_app_err))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_pdu_ind_rim_app_err_len
(
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_pdu_ind_rim_app_err != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_app_err->pdu_ext);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_ind_rim_app_err->ack);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_pdu_ind_rim_app_err
(
    U8  **pp_buffer,
    rim_pdu_ind_rim_app_err_t *p_rim_pdu_ind_rim_app_err
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_pdu_ind_rim_app_err != PNULL);

    /* This function composes rim_pdu_ind_rim_app_err */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_pdu_ind_rim_app_err->pdu_ext < 1) || (p_rim_pdu_ind_rim_app_err->pdu_ext > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_app_err->pdu_ext] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_app_err->pdu_ext);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_app_err->pdu_ext, "pdu_ext");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_app_err->pdu_ext);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_pdu_ind_rim_app_err->ack > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_pdu_ind_rim_app_err->ack] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rim_pdu_ind_rim_app_err->ack);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_rim_app_err->ack, "ack");
    *pp_buffer += sizeof(p_rim_pdu_ind_rim_app_err->ack);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_rim_information_err_len
(
    rim_information_err_t *p_rim_information_err
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_information_err != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_err->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_err->mme_id);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_err->dst_cell);

    /* Get length of IE */
    length += rrm_il_get_rim_routing_info_len(&p_rim_information_err->src_cell);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_err->app_idty);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_err->cause);

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_ERR_PROTOCOL_VER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_information_err->ver);
    }

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_PDU_IN_ERROR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_pdu_in_error_len(&p_rim_information_err->err_pdu);
    }

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_rim_son_transfer_app_idty_len(&p_rim_information_err->son_app_idty);
    }

    return length;
}

rrm_return_et
rrm_il_compose_rim_information_err
(
    U8  **pp_buffer,
    rim_information_err_t *p_rim_information_err
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_information_err != PNULL);

    /* This function composes rim_information_err */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_information_err->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_information_err->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_err->mme_id, "mme_id");
    *pp_buffer += sizeof(p_rim_information_err->mme_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_err->dst_cell))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_routing_info(pp_buffer, &p_rim_information_err->src_cell))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_err->app_idty < 1) || (p_rim_information_err->app_idty > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_err->app_idty] should be in range "
            "1 to 5. Incorrect value %u received.", p_rim_information_err->app_idty);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_err->app_idty, "app_idty");
    *pp_buffer += sizeof(p_rim_information_err->app_idty);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_information_err->cause > 75))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_err->cause] should be less than"
            " or equal to 75. Incorrect value %u received.", p_rim_information_err->cause);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_err->cause, "cause");
    *pp_buffer += sizeof(p_rim_information_err->cause);

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_ERR_PROTOCOL_VER_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rim_information_err->ver < 1) || (p_rim_information_err->ver > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_rim_information_err->ver] should be in range "
            "1 to 1. Incorrect value %u received.", p_rim_information_err->ver);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rim_information_err->ver, "ver");
    *pp_buffer += sizeof(p_rim_information_err->ver);
    }

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_PDU_IN_ERROR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_pdu_in_error(pp_buffer, &p_rim_information_err->err_pdu))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_information_err->bitmask & RIM_INFO_ERR_SON_TRNF_APP_IDNTY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_rim_son_transfer_app_idty(pp_buffer, &p_rim_information_err->son_app_idty))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_rim_information_err
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_RIM_INFO_ERROR message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_rim_information_err
(
    rim_information_err_t  *p_rim_information_err,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_rim_information_err != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_rim_information_err_len(p_rim_information_err);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_RIM_INFO_ERROR", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_ERROR, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_RIM_INFO_ERROR, msg_length,cell_index);

    /* Fill S1AP_RRM_RIM_INFO_ERROR message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_rim_information_err(&p_s1apRrm_msg, p_rim_information_err))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_rim_pdu_in_error_len
(
    rim_pdu_in_error_t *p_rim_pdu_in_error
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rim_pdu_in_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_rim_pdu_in_error->bitmask);

    return length;
}

static
rrm_return_et
rrm_il_compose_rim_pdu_in_error
(
    U8  **pp_buffer,
    rim_pdu_in_error_t *p_rim_pdu_in_error
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rim_pdu_in_error != PNULL);

    /* This function composes rim_pdu_in_error */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rim_pdu_in_error->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rim_pdu_in_error->bitmask);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_mme_info_len
(
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_mme_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_info->mme_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_rrm_mme_info->ip_addr);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_mme_info->port);

    /* Get length of IE */
    length += rrm_il_get_s1ap_mme_info_len(&p_s1ap_rrm_mme_info->mme_info);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_info
(
    U8  **pp_buffer,
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_mme_info != PNULL);

    /* This function composes s1ap_rrm_mme_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_mme_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_info->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_info->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_info->mme_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_rrm_mme_info->ip_addr); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_mme_info->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_mme_info->port, "port");
    *pp_buffer += sizeof(p_s1ap_rrm_mme_info->port);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_s1ap_mme_info(pp_buffer, &p_s1ap_rrm_mme_info->mme_info))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_mme_info
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_MME_INFO message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_mme_info
(
    s1ap_rrm_mme_info_t  *p_s1ap_rrm_mme_info,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_mme_info != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_mme_info_len(p_s1ap_rrm_mme_info);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_MME_INFO", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_MME_INFO, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_MME_INFO, msg_length,cell_index);

    /* Fill S1AP_RRM_MME_INFO message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_mme_info(&p_s1apRrm_msg, p_s1ap_rrm_mme_info))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_served_group_id_len
(
    s1ap_served_group_id_t *p_s1ap_served_group_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_served_group_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_served_group_id->mmegi);

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_served_group_id
(
    U8  **pp_buffer,
    s1ap_served_group_id_t *p_s1ap_served_group_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_served_group_id != PNULL);

    /* This function composes s1ap_served_group_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_served_group_id->mmegi); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_served_group_id->mmegi[loop], "mmegi[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_served_mmec_len
(
    s1ap_served_mmec_t *p_s1ap_served_mmec
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_served_mmec != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_served_mmec->mmec);

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_served_mmec
(
    U8  **pp_buffer,
    s1ap_served_mmec_t *p_s1ap_served_mmec
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_served_mmec != PNULL);

    /* This function composes s1ap_served_mmec */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_served_mmec->mmec, "mmec");
    *pp_buffer += sizeof(p_s1ap_served_mmec->mmec);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_served_gummei_info_len
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_served_gummei_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_served_gummei_info->num_served_plmns);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            length += rrm_il_get_s1ap_plmn_identity_len(&p_s1ap_served_gummei_info->plmn_identity[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_served_gummei_info->num_served_group_ids);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            length += rrm_il_get_s1ap_served_group_id_len(&p_s1ap_served_gummei_info->served_group_id[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_served_gummei_info->num_served_mmecs);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            length += rrm_il_get_s1ap_served_mmec_len(&p_s1ap_served_gummei_info->served_mmec[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_served_gummei_info
(
    U8  **pp_buffer,
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_served_gummei_info != PNULL);

    /* This function composes s1ap_served_gummei_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_served_gummei_info->num_served_plmns, "num_served_plmns");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_plmns);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_s1ap_plmn_identity(pp_buffer, &p_s1ap_served_gummei_info->plmn_identity[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_served_gummei_info->num_served_group_ids, "num_served_group_ids");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_group_ids);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_s1ap_served_group_id(pp_buffer, &p_s1ap_served_gummei_info->served_group_id[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_served_gummei_info->num_served_mmecs, "num_served_mmecs");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_mmecs);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_s1ap_served_mmec(pp_buffer, &p_s1ap_served_gummei_info->served_mmec[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_s1ap_mme_info_len
(
    s1ap_mme_info_t *p_s1ap_mme_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_mme_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_mme_info->bitmask);

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_mme_info->mme_name);
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_s1ap_served_gummei_info_len(&p_s1ap_mme_info->served_gummei_info);
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_mme_info->relative_mme_capacity);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_s1ap_mme_info
(
    U8  **pp_buffer,
    s1ap_mme_info_t *p_s1ap_mme_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_mme_info != PNULL);

    /* This function composes s1ap_mme_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_mme_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_mme_info->bitmask);

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_mme_info->mme_name); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_s1ap_mme_info->mme_name[loop], "mme_name[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_s1ap_served_gummei_info(pp_buffer, &p_s1ap_mme_info->served_gummei_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_mme_info->relative_mme_capacity, "relative_mme_capacity");
    *pp_buffer += sizeof(p_s1ap_mme_info->relative_mme_capacity);
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_req_len
(
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->report_char);

    /* Optional element */
    if(p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask & LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier > MAX_NO_OF_MEAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier] should be in range "
            "1 to MAX_NO_OF_MEAS. Incorrect value %d received.", p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier; loop++)
        {
            length += rrm_il_get_lppa_meas_quantifier_len(&p_s1ap_rrm_lppa_ecid_meas_init_req->meas_quantifier[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_req != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_init_req */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id, "e_smlc_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->e_smlc_meas_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->report_char > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->report_char] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->report_char);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->report_char, "report_char");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->report_char);

    /* Optional element */
    if(p_s1ap_rrm_lppa_ecid_meas_init_req->bitmask & LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity > 12))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity] should be less than"
            " or equal to 12. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity, "meas_periodicity");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->meas_periodicity);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier > MAX_NO_OF_MEAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier] should be in range "
            "1 to MAX_NO_OF_MEAS. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier, "num_meas_quantifier");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_ecid_meas_init_req->num_meas_quantifier; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_meas_quantifier(pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_req->meas_quantifier[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_req
(
    s1ap_rrm_lppa_ecid_meas_init_req_t  *p_s1ap_rrm_lppa_ecid_meas_init_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_req_len(p_s1ap_rrm_lppa_ecid_meas_init_req);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_req(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_init_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_meas_quantifier_len
(
    lppa_meas_quantifier_t *p_lppa_meas_quantifier
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_meas_quantifier != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_quantifier->meas_quantity_item);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_meas_quantifier
(
    U8  **pp_buffer,
    lppa_meas_quantifier_t *p_lppa_meas_quantifier
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_meas_quantifier != PNULL);

    /* This function composes lppa_meas_quantifier */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_meas_quantifier->meas_quantity_item > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_quantifier->meas_quantity_item] should be less than"
            " or equal to 5. Incorrect value %u received.", p_lppa_meas_quantifier->meas_quantity_item);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_meas_quantifier->meas_quantity_item, "meas_quantity_item");
    *pp_buffer += sizeof(p_lppa_meas_quantifier->meas_quantity_item);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_resp_len
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask & LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_e_cid_meas_result_len(&p_s1ap_rrm_lppa_ecid_meas_init_resp->e_cid_meas_result);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_resp
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_resp != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_init_resp */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id, "e_smlc_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->e_smlc_ue_meas_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id, "enb_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_resp->enb_ue_meas_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_ecid_meas_init_resp->bitmask & LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_e_cid_meas_result(pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_resp->e_cid_meas_result))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_resp
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t  *p_s1ap_rrm_lppa_ecid_meas_init_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_resp_len(p_s1ap_rrm_lppa_ecid_meas_init_resp);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_resp(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_init_resp))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_plmn_identity_len
(
    lppa_plmn_identity_t *p_lppa_plmn_identity
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_plmn_identity->plmn_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_plmn_identity
(
    U8  **pp_buffer,
    lppa_plmn_identity_t *p_lppa_plmn_identity
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_plmn_identity != PNULL);

    /* This function composes lppa_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_plmn_identity->plmn_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_ecgi_len
(
    lppa_ecgi_t *p_lppa_ecgi
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_ecgi != PNULL);

    /* Get length of IE */
    length += rrm_il_get_lppa_plmn_identity_len(&p_lppa_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_ecgi->eutran_cell_id);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_ecgi
(
    U8  **pp_buffer,
    lppa_ecgi_t *p_lppa_ecgi
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_ecgi != PNULL);

    /* This function composes lppa_ecgi */
    

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_plmn_identity(pp_buffer, &p_lppa_ecgi->plmn_identity))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_ecgi->eutran_cell_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_ecgi->eutran_cell_id[loop], "eutran_cell_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_e_utran_access_point_pos_len
(
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_e_utran_access_point_pos != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->latitude_sign);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->deg_of_latitude);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->deg_of_longitude);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->deg_of_altitude);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->altitude);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->uncertainty_semi_major);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->uncertainty_semi_minor);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->orientation_of_major_axis);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->uncertainty_altitude);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_utran_access_point_pos->confidence);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_e_utran_access_point_pos
(
    U8  **pp_buffer,
    lppa_e_utran_access_point_pos_t *p_lppa_e_utran_access_point_pos
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_e_utran_access_point_pos != PNULL);

    /* This function composes lppa_e_utran_access_point_pos */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->latitude_sign > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->latitude_sign] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->latitude_sign);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_e_utran_access_point_pos->latitude_sign, "latitude_sign");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->latitude_sign);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_latitude > 8388607))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_latitude] should be less than"
            " or equal to 8388607. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->deg_of_latitude);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_e_utran_access_point_pos->deg_of_latitude, "deg_of_latitude");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->deg_of_latitude);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_longitude < -8388608) || (p_lppa_e_utran_access_point_pos->deg_of_longitude > 8388607))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_longitude] should be in range "
            "-8388608 to 8388607. Incorrect value %d received.", p_lppa_e_utran_access_point_pos->deg_of_longitude);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S32(*pp_buffer, &p_lppa_e_utran_access_point_pos->deg_of_longitude, "deg_of_longitude");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->deg_of_longitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->deg_of_altitude > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->deg_of_altitude] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->deg_of_altitude);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_e_utran_access_point_pos->deg_of_altitude, "deg_of_altitude");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->deg_of_altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->altitude > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->altitude] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->altitude);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_e_utran_access_point_pos->altitude, "altitude");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_semi_major > 127))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_semi_major] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_semi_major);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_utran_access_point_pos->uncertainty_semi_major, "uncertainty_semi_major");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->uncertainty_semi_major);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_semi_minor > 127))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_semi_minor] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_semi_minor);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_utran_access_point_pos->uncertainty_semi_minor, "uncertainty_semi_minor");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->uncertainty_semi_minor);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->orientation_of_major_axis > 179))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->orientation_of_major_axis] should be less than"
            " or equal to 179. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->orientation_of_major_axis);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_utran_access_point_pos->orientation_of_major_axis, "orientation_of_major_axis");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->orientation_of_major_axis);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->uncertainty_altitude > 127))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->uncertainty_altitude] should be less than"
            " or equal to 127. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->uncertainty_altitude);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_utran_access_point_pos->uncertainty_altitude, "uncertainty_altitude");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->uncertainty_altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_e_utran_access_point_pos->confidence > 100))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_e_utran_access_point_pos->confidence] should be less than"
            " or equal to 100. Incorrect value %u received.", p_lppa_e_utran_access_point_pos->confidence);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_utran_access_point_pos->confidence, "confidence");
    *pp_buffer += sizeof(p_lppa_e_utran_access_point_pos->confidence);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_result_rsrp_len
(
    lppa_result_rsrp_t *p_lppa_result_rsrp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_result_rsrp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrp->pci);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrp->earfcn);

    /* Optional element */
    if(p_lppa_result_rsrp->bitmask & LPPA_RESULT_RSRP_ECGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_ecgi_len(&p_lppa_result_rsrp->ecgi);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrp->value_rsrp);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_result_rsrp
(
    U8  **pp_buffer,
    lppa_result_rsrp_t *p_lppa_result_rsrp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_result_rsrp != PNULL);

    /* This function composes lppa_result_rsrp */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_result_rsrp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_result_rsrp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->pci > 503))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrp->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_result_rsrp->pci);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrp->pci, "pci");
    *pp_buffer += sizeof(p_lppa_result_rsrp->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->earfcn > 65535))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrp->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_result_rsrp->earfcn);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrp->earfcn, "earfcn");
    *pp_buffer += sizeof(p_lppa_result_rsrp->earfcn);

    /* Optional element */
    if(p_lppa_result_rsrp->bitmask & LPPA_RESULT_RSRP_ECGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_ecgi(pp_buffer, &p_lppa_result_rsrp->ecgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrp->value_rsrp > 97))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrp->value_rsrp] should be less than"
            " or equal to 97. Incorrect value %u received.", p_lppa_result_rsrp->value_rsrp);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrp->value_rsrp, "value_rsrp");
    *pp_buffer += sizeof(p_lppa_result_rsrp->value_rsrp);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_meas_result_rsrp_len
(
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_meas_result_rsrp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_result_rsrp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_result_rsrp->rsrp_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrp->rsrp_count < 1) || (p_lppa_meas_result_rsrp->rsrp_count > LPPA_MAX_CELL_REPORT))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_result_rsrp->rsrp_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %d received.", p_lppa_meas_result_rsrp->rsrp_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrp->rsrp_count; loop++)
        {
            length += rrm_il_get_lppa_result_rsrp_len(&p_lppa_meas_result_rsrp->result_rsrp[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_meas_result_rsrp
(
    U8  **pp_buffer,
    lppa_meas_result_rsrp_t *p_lppa_meas_result_rsrp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_meas_result_rsrp != PNULL);

    /* This function composes lppa_meas_result_rsrp */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_meas_result_rsrp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_meas_result_rsrp->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrp->rsrp_count < 1) || (p_lppa_meas_result_rsrp->rsrp_count > LPPA_MAX_CELL_REPORT))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_result_rsrp->rsrp_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %u received.", p_lppa_meas_result_rsrp->rsrp_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_meas_result_rsrp->rsrp_count, "rsrp_count");
    *pp_buffer += sizeof(p_lppa_meas_result_rsrp->rsrp_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrp->rsrp_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_result_rsrp(pp_buffer, &p_lppa_meas_result_rsrp->result_rsrp[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_result_rsrq_len
(
    lppa_result_rsrq_t *p_lppa_result_rsrq
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_result_rsrq != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrq->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrq->pci);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrq->earfcn);

    /* Optional element */
    if(p_lppa_result_rsrq->bitmask & LPPA_RESULT_RSRQ_ECGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_ecgi_len(&p_lppa_result_rsrq->ecgi);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_result_rsrq->value_rsrq);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_result_rsrq
(
    U8  **pp_buffer,
    lppa_result_rsrq_t *p_lppa_result_rsrq
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_result_rsrq != PNULL);

    /* This function composes lppa_result_rsrq */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_result_rsrq->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_result_rsrq->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->pci > 503))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrq->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_result_rsrq->pci);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrq->pci, "pci");
    *pp_buffer += sizeof(p_lppa_result_rsrq->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->earfcn > 65535))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrq->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_result_rsrq->earfcn);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrq->earfcn, "earfcn");
    *pp_buffer += sizeof(p_lppa_result_rsrq->earfcn);

    /* Optional element */
    if(p_lppa_result_rsrq->bitmask & LPPA_RESULT_RSRQ_ECGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_ecgi(pp_buffer, &p_lppa_result_rsrq->ecgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_result_rsrq->value_rsrq > 34))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_result_rsrq->value_rsrq] should be less than"
            " or equal to 34. Incorrect value %u received.", p_lppa_result_rsrq->value_rsrq);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_result_rsrq->value_rsrq, "value_rsrq");
    *pp_buffer += sizeof(p_lppa_result_rsrq->value_rsrq);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_meas_result_rsrq_len
(
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_meas_result_rsrq != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_result_rsrq->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_result_rsrq->rsrq_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrq->rsrq_count < 1) || (p_lppa_meas_result_rsrq->rsrq_count > LPPA_MAX_CELL_REPORT))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_result_rsrq->rsrq_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %d received.", p_lppa_meas_result_rsrq->rsrq_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrq->rsrq_count; loop++)
        {
            length += rrm_il_get_lppa_result_rsrq_len(&p_lppa_meas_result_rsrq->result_rsrq[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_meas_result_rsrq
(
    U8  **pp_buffer,
    lppa_meas_result_rsrq_t *p_lppa_meas_result_rsrq
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_meas_result_rsrq != PNULL);

    /* This function composes lppa_meas_result_rsrq */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_meas_result_rsrq->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_meas_result_rsrq->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_lppa_meas_result_rsrq->rsrq_count < 1) || (p_lppa_meas_result_rsrq->rsrq_count > LPPA_MAX_CELL_REPORT))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_result_rsrq->rsrq_count] should be in range "
            "1 to LPPA_MAX_CELL_REPORT. Incorrect value %u received.", p_lppa_meas_result_rsrq->rsrq_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_meas_result_rsrq->rsrq_count, "rsrq_count");
    *pp_buffer += sizeof(p_lppa_meas_result_rsrq->rsrq_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_result_rsrq->rsrq_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_result_rsrq(pp_buffer, &p_lppa_meas_result_rsrq->result_rsrq[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_measured_res_len
(
    lppa_measured_res_t *p_lppa_measured_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_measured_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_measured_res->bitmask);

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_measured_res->value_angle_of_arrival);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_measured_res->value_timing_adv_type_1);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_measured_res->value_timing_adv_type_2);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_meas_result_rsrp_len(&p_lppa_measured_res->meas_result_rsrp);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_meas_result_rsrq_len(&p_lppa_measured_res->meas_result_rsrq);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_measured_res
(
    U8  **pp_buffer,
    lppa_measured_res_t *p_lppa_measured_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_measured_res != PNULL);

    /* This function composes lppa_measured_res */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_measured_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_measured_res->bitmask);

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_angle_of_arrival > 719))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_measured_res->value_angle_of_arrival] should be less than"
            " or equal to 719. Incorrect value %u received.", p_lppa_measured_res->value_angle_of_arrival);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_measured_res->value_angle_of_arrival, "value_angle_of_arrival");
    *pp_buffer += sizeof(p_lppa_measured_res->value_angle_of_arrival);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_timing_adv_type_1 > 7690))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_measured_res->value_timing_adv_type_1] should be less than"
            " or equal to 7690. Incorrect value %u received.", p_lppa_measured_res->value_timing_adv_type_1);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_measured_res->value_timing_adv_type_1, "value_timing_adv_type_1");
    *pp_buffer += sizeof(p_lppa_measured_res->value_timing_adv_type_1);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_measured_res->value_timing_adv_type_2 > 7690))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_measured_res->value_timing_adv_type_2] should be less than"
            " or equal to 7690. Incorrect value %u received.", p_lppa_measured_res->value_timing_adv_type_2);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_measured_res->value_timing_adv_type_2, "value_timing_adv_type_2");
    *pp_buffer += sizeof(p_lppa_measured_res->value_timing_adv_type_2);
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_meas_result_rsrp(pp_buffer, &p_lppa_measured_res->meas_result_rsrp))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_measured_res->bitmask & LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_meas_result_rsrq(pp_buffer, &p_lppa_measured_res->meas_result_rsrq))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_meas_results_len
(
    lppa_meas_results_t *p_lppa_meas_results
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_meas_results != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_meas_results->num_measured_results);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_meas_results->num_measured_results > MAX_NO_OF_MEAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_results->num_measured_results] should be less than"
            " or equal to MAX_NO_OF_MEAS. Incorrect value %d received.", p_lppa_meas_results->num_measured_results);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_results->num_measured_results; loop++)
        {
            length += rrm_il_get_lppa_measured_res_len(&p_lppa_meas_results->measured_results[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_meas_results
(
    U8  **pp_buffer,
    lppa_meas_results_t *p_lppa_meas_results
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_meas_results != PNULL);

    /* This function composes lppa_meas_results */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_meas_results->num_measured_results > MAX_NO_OF_MEAS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_meas_results->num_measured_results] should be less than"
            " or equal to MAX_NO_OF_MEAS. Incorrect value %u received.", p_lppa_meas_results->num_measured_results);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_meas_results->num_measured_results, "num_measured_results");
    *pp_buffer += sizeof(p_lppa_meas_results->num_measured_results);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_meas_results->num_measured_results; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_measured_res(pp_buffer, &p_lppa_meas_results->measured_results[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_e_cid_meas_result_len
(
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_e_cid_meas_result != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_e_cid_meas_result->bitmask);

    /* Get length of IE */
    length += rrm_il_get_lppa_ecgi_len(&p_lppa_e_cid_meas_result->serving_cell_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_e_cid_meas_result->serving_cell_tac);

    /* Optional element */
    if(p_lppa_e_cid_meas_result->bitmask & LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_e_utran_access_point_pos_len(&p_lppa_e_cid_meas_result->e_utran_access_point_pos);
    }

    /* Optional element */
    if(p_lppa_e_cid_meas_result->bitmask & LPPA_MEASUREMENT_RESULT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_meas_results_len(&p_lppa_e_cid_meas_result->meas_result);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_e_cid_meas_result
(
    U8  **pp_buffer,
    lppa_e_cid_meas_result_t *p_lppa_e_cid_meas_result
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_e_cid_meas_result != PNULL);

    /* This function composes lppa_e_cid_meas_result */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_e_cid_meas_result->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_e_cid_meas_result->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_ecgi(pp_buffer, &p_lppa_e_cid_meas_result->serving_cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_e_cid_meas_result->serving_cell_tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_e_cid_meas_result->serving_cell_tac[loop], "serving_cell_tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_lppa_e_cid_meas_result->bitmask & LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_e_utran_access_point_pos(pp_buffer, &p_lppa_e_cid_meas_result->e_utran_access_point_pos))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_e_cid_meas_result->bitmask & LPPA_MEASUREMENT_RESULT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_meas_results(pp_buffer, &p_lppa_e_cid_meas_result->meas_result))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_fail_len
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_fail != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id);

    /* Get length of IE */
    length += rrm_il_get_lppa_cause_len(&p_s1ap_rrm_lppa_ecid_meas_init_fail->cause);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_fail
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_fail != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_init_fail */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id, "e_smlc_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_init_fail->e_smlc_ue_meas_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_cause(pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_init_fail->cause))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_fail
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_fail
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t  *p_s1ap_rrm_lppa_ecid_meas_init_fail,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_init_fail != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_fail_len(p_s1ap_rrm_lppa_ecid_meas_init_fail);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_fail(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_init_fail))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_cause_len
(
    lppa_cause_t *p_lppa_cause
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_cause != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_cause->type);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_cause->value);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_cause
(
    U8  **pp_buffer,
    lppa_cause_t *p_lppa_cause
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_cause != PNULL);

    /* This function composes lppa_cause */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_cause->type, "type");
    *pp_buffer += sizeof(p_lppa_cause->type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_cause->value, "value");
    *pp_buffer += sizeof(p_lppa_cause->value);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_fail_ind_len
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_fail_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id);

    /* Get length of IE */
    length += rrm_il_get_lppa_cause_len(&p_s1ap_rrm_lppa_ecid_meas_fail_ind->cause);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_fail_ind != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_fail_ind */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id, "e_smlc_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->e_smlc_ue_meas_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id, "enb_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_fail_ind->enb_ue_meas_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_cause(pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_fail_ind->cause))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_fail_ind
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t  *p_s1ap_rrm_lppa_ecid_meas_fail_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_fail_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_fail_ind_len(p_s1ap_rrm_lppa_ecid_meas_fail_ind);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_fail_ind(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_fail_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_report_len
(
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_report != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id);

    /* Get length of IE */
    length += rrm_il_get_lppa_e_cid_meas_result_len(&p_s1ap_rrm_lppa_ecid_meas_report->e_cid_meas_result);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_report
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_report != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_report */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id, "e_smlc_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->e_smlc_ue_meas_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id, "enb_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_report->enb_ue_meas_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_e_cid_meas_result(pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_report->e_cid_meas_result))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_report
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_REPORT message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_report
(
    s1ap_rrm_lppa_ecid_meas_report_t  *p_s1ap_rrm_lppa_ecid_meas_report,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_report != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_report_len(p_s1ap_rrm_lppa_ecid_meas_report);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_REPORT", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_REPORT, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_REPORT, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_REPORT message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_report(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_report))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_termination_cmd_len
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_termination_cmd != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->ue_index);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_termination_cmd != PNULL);

    /* This function composes s1ap_rrm_lppa_ecid_meas_termination_cmd */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->ue_index);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id, "e_smlc_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->e_smlc_ue_meas_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id < 1) || (p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id] should be in range "
            "1 to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id, "enb_ue_meas_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_ecid_meas_termination_cmd->enb_ue_meas_id);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_ecid_meas_termination_cmd
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t  *p_s1ap_rrm_lppa_ecid_meas_termination_cmd,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_ecid_meas_termination_cmd != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_ecid_meas_termination_cmd_len(p_s1ap_rrm_lppa_ecid_meas_termination_cmd);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_ecid_meas_termination_cmd(&p_s1apRrm_msg, p_s1ap_rrm_lppa_ecid_meas_termination_cmd))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_req_len
(
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_req->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_req->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count > MAX_NO_OTDA_TYPES))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count] should be less than"
            " or equal to MAX_NO_OTDA_TYPES. Incorrect value %d received.", p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count; loop++)
        {
            length += rrm_il_get_lppa_otdoa_info_type_len(&p_s1ap_rrm_lppa_otdoa_info_req->otda_info_type_arr[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_req != PNULL);

    /* This function composes s1ap_rrm_lppa_otdoa_info_req */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_req->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_req->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_req->lppa_trans_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count > MAX_NO_OTDA_TYPES))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count] should be less than"
            " or equal to MAX_NO_OTDA_TYPES. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count, "otdoa_info_count");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_req->otdoa_info_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_otdoa_info_type(pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_req->otda_info_type_arr[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_otdoa_info_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_OTDOA_INFO_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_req
(
    s1ap_rrm_lppa_otdoa_info_req_t  *p_s1ap_rrm_lppa_otdoa_info_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_otdoa_info_req_len(p_s1ap_rrm_lppa_otdoa_info_req);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_OTDOA_INFO_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_OTDOA_INFO_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_otdoa_info_req(&p_s1apRrm_msg, p_s1ap_rrm_lppa_otdoa_info_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_otdoa_info_type_len
(
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_otdoa_info_type != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_info_type->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_info_type->otdoa_info_item);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_otdoa_info_type
(
    U8  **pp_buffer,
    lppa_otdoa_info_type_t *p_lppa_otdoa_info_type
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_otdoa_info_type != PNULL);

    /* This function composes lppa_otdoa_info_type */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_otdoa_info_type->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_otdoa_info_type->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_info_type->otdoa_info_item > 11))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_info_type->otdoa_info_item] should be less than"
            " or equal to 11. Incorrect value %u received.", p_lppa_otdoa_info_type->otdoa_info_item);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_info_type->otdoa_info_item, "otdoa_info_item");
    *pp_buffer += sizeof(p_lppa_otdoa_info_type->otdoa_info_item);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_resp_len
(
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count < 1) || (p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %d received.", p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count; loop++)
        {
            length += rrm_il_get_lppa_otdoa_cells_len(&p_s1ap_rrm_lppa_otdoa_info_resp->otda_cells_arr[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_resp
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_resp != PNULL);

    /* This function composes s1ap_rrm_lppa_otdoa_info_resp */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->lppa_trans_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count < 1) || (p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count > MAX_NUM_CELLS))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count] should be in range "
            "1 to MAX_NUM_CELLS. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count, "otdoa_cell_count");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_rrm_lppa_otdoa_info_resp->otdoa_cell_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_otdoa_cells(pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_resp->otda_cells_arr[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_otdoa_info_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_OTDOA_INFO_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_resp
(
    s1ap_rrm_lppa_otdoa_info_resp_t  *p_s1ap_rrm_lppa_otdoa_info_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_otdoa_info_resp_len(p_s1ap_rrm_lppa_otdoa_info_resp);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_OTDOA_INFO_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_RESP, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_OTDOA_INFO_RESP message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_otdoa_info_resp(&p_s1apRrm_msg, p_s1ap_rrm_lppa_otdoa_info_resp))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_tac_len
(
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_otdoa_cells_tac != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_otdoa_cells_tac->tac);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells_tac
(
    U8  **pp_buffer,
    lppa_otdoa_cells_tac_t *p_lppa_otdoa_cells_tac
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_otdoa_cells_tac != PNULL);

    /* This function composes lppa_otdoa_cells_tac */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_otdoa_cells_tac->tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_otdoa_cells_tac->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_otdoa_sfn_init_time_len
(
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_otdoa_sfn_init_time != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_otdoa_sfn_init_time->sfn_initiation_time);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_otdoa_sfn_init_time
(
    U8  **pp_buffer,
    lppa_otdoa_sfn_init_time_t *p_lppa_otdoa_sfn_init_time
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_otdoa_sfn_init_time != PNULL);

    /* This function composes lppa_otdoa_sfn_init_time */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_otdoa_sfn_init_time->sfn_initiation_time); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_otdoa_sfn_init_time->sfn_initiation_time[loop], "sfn_initiation_time[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_muting_config_two_len
(
    lppa_muting_config_two_t *p_lppa_muting_config_two
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_muting_config_two != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_muting_config_two->two);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_muting_config_two
(
    U8  **pp_buffer,
    lppa_muting_config_two_t *p_lppa_muting_config_two
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_muting_config_two != PNULL);

    /* This function composes lppa_muting_config_two */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_muting_config_two->two > 4))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_muting_config_two->two] should be less than"
            " or equal to 4. Incorrect value %u received.", p_lppa_muting_config_two->two);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_muting_config_two->two, "two");
    *pp_buffer += sizeof(p_lppa_muting_config_two->two);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_muting_config_four_len
(
    lppa_muting_config_four_t *p_lppa_muting_config_four
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_muting_config_four != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_muting_config_four->four);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_muting_config_four
(
    U8  **pp_buffer,
    lppa_muting_config_four_t *p_lppa_muting_config_four
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_muting_config_four != PNULL);

    /* This function composes lppa_muting_config_four */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_muting_config_four->four > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_muting_config_four->four] should be less than"
            " or equal to 15. Incorrect value %u received.", p_lppa_muting_config_four->four);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_muting_config_four->four, "four");
    *pp_buffer += sizeof(p_lppa_muting_config_four->four);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_muting_config_eight_len
(
    lppa_muting_config_eight_t *p_lppa_muting_config_eight
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_muting_config_eight != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_muting_config_eight->eight);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_muting_config_eight
(
    U8  **pp_buffer,
    lppa_muting_config_eight_t *p_lppa_muting_config_eight
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_muting_config_eight != PNULL);

    /* This function composes lppa_muting_config_eight */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_muting_config_eight->eight); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_muting_config_eight->eight[loop], "eight[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_muting_config_sixteen_len
(
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_muting_config_sixteen != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_lppa_muting_config_sixteen->sixteen);

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_muting_config_sixteen
(
    U8  **pp_buffer,
    lppa_muting_config_sixteen_t *p_lppa_muting_config_sixteen
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_muting_config_sixteen != PNULL);

    /* This function composes lppa_muting_config_sixteen */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_lppa_muting_config_sixteen->sixteen); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_lppa_muting_config_sixteen->sixteen[loop], "sixteen[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_prs_muting_config_len
(
    lppa_prs_muting_config_t *p_lppa_prs_muting_config
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_prs_muting_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_prs_muting_config->bitmask);

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_TWO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_muting_config_two_len(&p_lppa_prs_muting_config->muting_config_two);
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_muting_config_four_len(&p_lppa_prs_muting_config->muting_config_four);
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_muting_config_eight_len(&p_lppa_prs_muting_config->muting_config_eight);
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_muting_config_sixteen_len(&p_lppa_prs_muting_config->muting_config_sixteen);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_prs_muting_config
(
    U8  **pp_buffer,
    lppa_prs_muting_config_t *p_lppa_prs_muting_config
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_prs_muting_config != PNULL);

    /* This function composes lppa_prs_muting_config */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_prs_muting_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_prs_muting_config->bitmask);

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_TWO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_muting_config_two(pp_buffer, &p_lppa_prs_muting_config->muting_config_two))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_muting_config_four(pp_buffer, &p_lppa_prs_muting_config->muting_config_four))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_muting_config_eight(pp_buffer, &p_lppa_prs_muting_config->muting_config_eight))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_muting_config_sixteen(pp_buffer, &p_lppa_prs_muting_config->muting_config_sixteen))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_info_len
(
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_otdoa_cells_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->bitmask);

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PCI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->pci);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_ecgi_len(&p_lppa_otdoa_cells_info->cell_id);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_TAC_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_otdoa_cells_tac_len(&p_lppa_otdoa_cells_info->otdoa_cells_tac);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_EARFCN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->earfcn);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->prs_band_width);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->prs_config_index);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->cp_length);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->no_of_dl_frames);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells_info->no_of_antenna_ports);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_otdoa_sfn_init_time_len(&p_lppa_otdoa_cells_info->otdoa_sfn_init_time);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_e_utran_access_point_pos_len(&p_lppa_otdoa_cells_info->e_utran_acces_point_pos);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_prs_muting_config_len(&p_lppa_otdoa_cells_info->prs_muting_config);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells_info
(
    U8  **pp_buffer,
    lppa_otdoa_cells_info_t *p_lppa_otdoa_cells_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_otdoa_cells_info != PNULL);

    /* This function composes lppa_otdoa_cells_info */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->bitmask);

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PCI_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->pci > 503))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_lppa_otdoa_cells_info->pci);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->pci, "pci");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->pci);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_ecgi(pp_buffer, &p_lppa_otdoa_cells_info->cell_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_TAC_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_otdoa_cells_tac(pp_buffer, &p_lppa_otdoa_cells_info->otdoa_cells_tac))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_EARFCN_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->earfcn > 65535))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->earfcn] should be less than"
            " or equal to 65535. Incorrect value %u received.", p_lppa_otdoa_cells_info->earfcn);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->earfcn, "earfcn");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->earfcn);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->prs_band_width > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->prs_band_width] should be less than"
            " or equal to 5. Incorrect value %u received.", p_lppa_otdoa_cells_info->prs_band_width);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->prs_band_width, "prs_band_width");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->prs_band_width);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->prs_config_index > 4095))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->prs_config_index] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_lppa_otdoa_cells_info->prs_config_index);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->prs_config_index, "prs_config_index");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->prs_config_index);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->cp_length > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->cp_length] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_otdoa_cells_info->cp_length);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->cp_length, "cp_length");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->cp_length);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->no_of_dl_frames > 3))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->no_of_dl_frames] should be less than"
            " or equal to 3. Incorrect value %u received.", p_lppa_otdoa_cells_info->no_of_dl_frames);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->no_of_dl_frames, "no_of_dl_frames");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->no_of_dl_frames);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells_info->no_of_antenna_ports > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells_info->no_of_antenna_ports] should be less than"
            " or equal to 1. Incorrect value %u received.", p_lppa_otdoa_cells_info->no_of_antenna_ports);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_otdoa_cells_info->no_of_antenna_ports, "no_of_antenna_ports");
    *pp_buffer += sizeof(p_lppa_otdoa_cells_info->no_of_antenna_ports);
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_otdoa_sfn_init_time(pp_buffer, &p_lppa_otdoa_cells_info->otdoa_sfn_init_time))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_e_utran_access_point_pos(pp_buffer, &p_lppa_otdoa_cells_info->e_utran_acces_point_pos))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_lppa_otdoa_cells_info->bitmask & LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_prs_muting_config(pp_buffer, &p_lppa_otdoa_cells_info->prs_muting_config))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_otdoa_cells_len
(
    lppa_otdoa_cells_t *p_lppa_otdoa_cells
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_otdoa_cells != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_otdoa_cells->otdoa_cell_info_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells->otdoa_cell_info_count > MAX_NUM_OTDOA_TYPES))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells->otdoa_cell_info_count] should be less than"
            " or equal to MAX_NUM_OTDOA_TYPES. Incorrect value %d received.", p_lppa_otdoa_cells->otdoa_cell_info_count);
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_otdoa_cells->otdoa_cell_info_count; loop++)
        {
            length += rrm_il_get_lppa_otdoa_cells_info_len(&p_lppa_otdoa_cells->otda_cells_info_arr[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_otdoa_cells
(
    U8  **pp_buffer,
    lppa_otdoa_cells_t *p_lppa_otdoa_cells
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_otdoa_cells != PNULL);

    /* This function composes lppa_otdoa_cells */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_otdoa_cells->otdoa_cell_info_count > MAX_NUM_OTDOA_TYPES))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_otdoa_cells->otdoa_cell_info_count] should be less than"
            " or equal to MAX_NUM_OTDOA_TYPES. Incorrect value %u received.", p_lppa_otdoa_cells->otdoa_cell_info_count);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_otdoa_cells->otdoa_cell_info_count, "otdoa_cell_info_count");
    *pp_buffer += sizeof(p_lppa_otdoa_cells->otdoa_cell_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_lppa_otdoa_cells->otdoa_cell_info_count; loop++)
        {
            if (RRM_FAILURE == rrm_il_compose_lppa_otdoa_cells_info(pp_buffer, &p_lppa_otdoa_cells->otda_cells_info_arr[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_fail_len
(
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_fail != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->mme_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id);

    /* Get length of IE */
    length += rrm_il_get_lppa_cause_len(&p_s1ap_rrm_lppa_otdoa_info_fail->cause);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_fail
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_fail != PNULL);

    /* This function composes s1ap_rrm_lppa_otdoa_info_fail */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_fail->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_fail->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_fail->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_fail->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_fail->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->mme_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_fail->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_otdoa_info_fail->lppa_trans_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_cause(pp_buffer, &p_s1ap_rrm_lppa_otdoa_info_fail->cause))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_otdoa_info_fail
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_OTDOA_INFO_FAIL message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_fail
(
    s1ap_rrm_lppa_otdoa_info_fail_t  *p_s1ap_rrm_lppa_otdoa_info_fail,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_otdoa_info_fail != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_otdoa_info_fail_len(p_s1ap_rrm_lppa_otdoa_info_fail);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_OTDOA_INFO_FAIL", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_FAIL, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_OTDOA_INFO_FAIL, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_OTDOA_INFO_FAIL message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_otdoa_info_fail(&p_s1apRrm_msg, p_s1ap_rrm_lppa_otdoa_info_fail))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_error_ind_len
(
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind->mme_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind->ue_index);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind->lppa_trans_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CAUSE_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_cause_len(&p_s1ap_rrm_lppa_error_ind->cause);
    }

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CRITIC_DIAG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_il_get_lppa_criticality_diagno_len(&p_s1ap_rrm_lppa_error_ind->criticality_diagnostics);
    }

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_error_ind
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind != PNULL);

    /* This function composes s1ap_rrm_lppa_error_ind */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_error_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_error_ind->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind->mme_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_error_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind->ue_index);
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_error_ind->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_error_ind->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind->lppa_trans_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CAUSE_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_cause(pp_buffer, &p_s1ap_rrm_lppa_error_ind->cause))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind->bitmask & LPPA_ERROR_IND_CRITIC_DIAG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_criticality_diagno(pp_buffer, &p_s1ap_rrm_lppa_error_ind->criticality_diagnostics))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_error_ind
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ERROR_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_error_ind
(
    s1ap_rrm_lppa_error_ind_t  *p_s1ap_rrm_lppa_error_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_error_ind_len(p_s1ap_rrm_lppa_error_ind);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ERROR_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ERROR_IND, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ERROR_IND, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ERROR_IND message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_error_ind(&p_s1apRrm_msg, p_s1ap_rrm_lppa_error_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_il_get_lppa_criticality_diagno_len
(
    lppa_criticality_diagno_t *p_lppa_criticality_diagno
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_lppa_criticality_diagno != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_criticality_diagno->bitmask);

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_criticality_diagno->procedure_code);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_criticality_diagno->triggering_msg);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_criticality_diagno->proc_critic);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_lppa_criticality_diagno->lppa_trans_id);
    }

    return length;
}

static
rrm_return_et
rrm_il_compose_lppa_criticality_diagno
(
    U8  **pp_buffer,
    lppa_criticality_diagno_t *p_lppa_criticality_diagno
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_lppa_criticality_diagno != PNULL);

    /* This function composes lppa_criticality_diagno */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_lppa_criticality_diagno->bitmask, "bitmask");
    *pp_buffer += sizeof(p_lppa_criticality_diagno->bitmask);

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_lppa_criticality_diagno->procedure_code, "procedure_code");
    *pp_buffer += sizeof(p_lppa_criticality_diagno->procedure_code);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->triggering_msg > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_criticality_diagno->triggering_msg] should be less than"
            " or equal to 2. Incorrect value %u received.", p_lppa_criticality_diagno->triggering_msg);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_criticality_diagno->triggering_msg, "triggering_msg");
    *pp_buffer += sizeof(p_lppa_criticality_diagno->triggering_msg);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->proc_critic > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_criticality_diagno->proc_critic] should be less than"
            " or equal to 2. Incorrect value %u received.", p_lppa_criticality_diagno->proc_critic);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_criticality_diagno->proc_critic, "proc_critic");
    *pp_buffer += sizeof(p_lppa_criticality_diagno->proc_critic);
    }

    /* Optional element */
    if(p_lppa_criticality_diagno->bitmask & LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_lppa_criticality_diagno->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_lppa_criticality_diagno->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_lppa_criticality_diagno->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_lppa_criticality_diagno->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_lppa_criticality_diagno->lppa_trans_id);
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_error_ind_req_len
(
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind_req->bitmask);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind_req->mme_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind_req->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind_req->ue_index);
    }
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind_req->routing_id);
    /* Get length of parameter of basic type */
    length += (rrm_length_t)sizeof(p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id);

    /* Get length of IE */
    length += rrm_il_get_lppa_cause_len(&p_s1ap_rrm_lppa_error_ind_req->cause);

    /* Get length of IE */
    length += rrm_il_get_lppa_criticality_diagno_len(&p_s1ap_rrm_lppa_error_ind_req->criticality_diagnostics);

    return length;
}

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_error_ind_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind_req != PNULL);

    /* This function composes s1ap_rrm_lppa_error_ind_req */
    

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind_req->mme_id > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind_req->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind_req->mme_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind_req->mme_id);

    /* Optional element */
    if(p_s1ap_rrm_lppa_error_ind_req->bitmask & LPPA_ERROR_IND_UE_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind_req->ue_index);
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->routing_id, "routing_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind_req->routing_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id > 32767))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_WARNING, "Parameter [p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id);
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id, "lppa_trans_id");
    *pp_buffer += sizeof(p_s1ap_rrm_lppa_error_ind_req->lppa_trans_id);

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_cause(pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->cause))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_il_compose_lppa_criticality_diagno(pp_buffer, &p_s1ap_rrm_lppa_error_ind_req->criticality_diagnostics))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_il_send_s1ap_rrm_lppa_error_ind_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_RRM_LPPA_ERROR_IND_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_il_send_s1ap_rrm_lppa_error_ind_req
(
    s1ap_rrm_lppa_error_ind_req_t  *p_s1ap_rrm_lppa_error_ind_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apRrm_msg;

    RRM_ASSERT(p_s1ap_rrm_lppa_error_ind_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_il_get_s1ap_rrm_lppa_error_ind_req_len(p_s1ap_rrm_lppa_error_ind_req);
    
    if(msg_length == RRM_FAILURE)
        {
            return RRM_FAILURE;
    }
    RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_INFO, "Src(%u)->Dst(%u):S1AP_RRM_LPPA_ERROR_IND_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL,RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_s1apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_s1apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ERROR_IND_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_s1apRrm_msg, transaction_id, src_module_id,
        dst_module_id, S1AP_RRM_LPPA_ERROR_IND_REQ, msg_length,cell_index);

    /* Fill S1AP_RRM_LPPA_ERROR_IND_REQ message */
    p_s1apRrm_msg = p_s1apRrm_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_il_compose_s1ap_rrm_lppa_error_ind_req(&p_s1apRrm_msg, p_s1ap_rrm_lppa_error_ind_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_s1apRrm_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dst_module_id);

    return RRM_SUCCESS;
}

