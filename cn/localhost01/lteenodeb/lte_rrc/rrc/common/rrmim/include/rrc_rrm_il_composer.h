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
 *  File Description : The file rrc_rrm_il_composer.h contains the prototypes 
 *                     of RRC-RRM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_RRM_IL_COMPOSER_H_
#define _RRC_RRM_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"

rrc_length_t
rrc_il_get_rrc_rrm_ue_sync_status_len
(
    rrc_rrm_ue_sync_status_t *p_rrc_rrm_ue_sync_status
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_sync_status
(
    U8  **pp_buffer,
    rrc_rrm_ue_sync_status_t *p_rrc_rrm_ue_sync_status
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_reestab_complete_ind_len
(
    rrc_rrm_ue_reestab_complete_ind_t *p_rrc_rrm_ue_reestab_complete_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_reestab_complete_ind
(
    U8  **pp_buffer,
    rrc_rrm_ue_reestab_complete_ind_t *p_rrc_rrm_ue_reestab_complete_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_release_req_len
(
    rrc_rrm_ue_release_req_t *p_rrc_rrm_ue_release_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_release_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_release_req_t *p_rrc_rrm_ue_release_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_release_resp_len
(
    rrc_rrm_ue_release_resp_t *p_rrc_rrm_ue_release_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_release_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_release_resp_t *p_rrc_rrm_ue_release_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_setup_resp_len
(
    rrc_rrm_cell_setup_resp_t *p_rrc_rrm_cell_setup_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_setup_resp
(
    U8  **pp_buffer,
    rrc_rrm_cell_setup_resp_t *p_rrc_rrm_cell_setup_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_reconfig_req_len
(
    rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_reconfig_req
(
    U8  **pp_buffer,
    rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_reconfig_resp_len
(
    rrc_rrm_cell_reconfig_resp_t *p_rrc_rrm_cell_reconfig_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_reconfig_resp
(
    U8  **pp_buffer,
    rrc_rrm_cell_reconfig_resp_t *p_rrc_rrm_cell_reconfig_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_setup_req_len
(
    rrc_rrm_cell_setup_req_t *p_rrc_rrm_cell_setup_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_setup_req
(
    U8  **pp_buffer,
    rrc_rrm_cell_setup_req_t *p_rrc_rrm_cell_setup_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_admission_req_len
(
    rrc_rrm_ue_admission_req_t *p_rrc_rrm_ue_admission_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_admission_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_admission_req_t *p_rrc_rrm_ue_admission_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_admission_resp_len
(
    rrc_rrm_ue_admission_resp_t *p_rrc_rrm_ue_admission_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_admission_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_admission_resp_t *p_rrc_rrm_ue_admission_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_admission_cnf_len
(
    rrc_rrm_ue_admission_cnf_t *p_rrc_rrm_ue_admission_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_admission_cnf
(
    U8  **pp_buffer,
    rrc_rrm_ue_admission_cnf_t *p_rrc_rrm_ue_admission_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_reconfig_req_len
(
    rrc_rrm_ue_reconfig_req_t *p_rrc_rrm_ue_reconfig_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_reconfig_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_reconfig_req_t *p_rrc_rrm_ue_reconfig_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_reconfig_resp_len
(
    rrc_rrm_ue_reconfig_resp_t *p_rrc_rrm_ue_reconfig_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_reconfig_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_reconfig_resp_t *p_rrc_rrm_ue_reconfig_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_setup_req_len
(
    rrc_rrm_erb_setup_req_t *p_rrc_rrm_erb_setup_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_setup_req
(
    U8  **pp_buffer,
    rrc_rrm_erb_setup_req_t *p_rrc_rrm_erb_setup_req
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_setup_resp_len
(
    rrc_rrm_erb_setup_resp_t *p_rrc_rrm_erb_setup_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_setup_resp
(
    U8  **pp_buffer,
    rrc_rrm_erb_setup_resp_t *p_rrc_rrm_erb_setup_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_setup_cnf_len
(
    rrc_rrm_erb_setup_cnf_t *p_rrc_rrm_erb_setup_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_setup_cnf
(
    U8  **pp_buffer,
    rrc_rrm_erb_setup_cnf_t *p_rrc_rrm_erb_setup_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_erab_reconfig_req_len
(
    rrc_rrm_erab_reconfig_req_t *p_rrc_rrm_erab_reconfig_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_erab_reconfig_req
(
    U8  **pp_buffer,
    rrc_rrm_erab_reconfig_req_t *p_rrc_rrm_erab_reconfig_req
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_reconfig_resp_len
(
    rrc_rrm_erb_reconfig_resp_t *p_rrc_rrm_erb_reconfig_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_reconfig_resp
(
    U8  **pp_buffer,
    rrc_rrm_erb_reconfig_resp_t *p_rrc_rrm_erb_reconfig_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_erab_modify_req_len
(
    rrc_rrm_erab_modify_req_t *p_rrc_rrm_erab_modify_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_erab_modify_req
(
    U8  **pp_buffer,
    rrc_rrm_erab_modify_req_t *p_rrc_rrm_erab_modify_req
);

rrc_length_t
rrc_il_get_rrc_rrm_erab_modify_resp_len
(
    rrc_rrm_erab_modify_resp_t *p_rrc_rrm_erab_modify_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_erab_modify_resp
(
    U8  **pp_buffer,
    rrc_rrm_erab_modify_resp_t *p_rrc_rrm_erab_modify_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_modify_cnf_len
(
    rrc_rrm_erb_modify_cnf_t *p_rrc_rrm_erb_modify_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_modify_cnf
(
    U8  **pp_buffer,
    rrc_rrm_erb_modify_cnf_t *p_rrc_rrm_erb_modify_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_connection_release_ind_len
(
    rrc_rrm_ue_connection_release_ind_t *p_rrc_rrm_ue_connection_release_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_connection_release_ind
(
    U8  **pp_buffer,
    rrc_rrm_ue_connection_release_ind_t *p_rrc_rrm_ue_connection_release_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_measurment_results_ind_len
(
    rrc_rrm_measurment_results_ind_t *p_rrc_rrm_measurment_results_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_measurment_results_ind
(
    U8  **pp_buffer,
    rrc_rrm_measurment_results_ind_t *p_rrc_rrm_measurment_results_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_proximity_ind_len
(
    rrc_rrm_proximity_ind_t *p_rrc_rrm_proximity_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_proximity_ind
(
    U8  **pp_buffer,
    rrc_rrm_proximity_ind_t *p_rrc_rrm_proximity_ind
);

rrc_length_t
rrc_il_get_rrm_rrc_cell_del_resp_len
(
    rrm_rrc_cell_del_resp_t *p_rrm_rrc_cell_del_resp
);

rrc_return_et
rrc_il_compose_rrm_rrc_cell_del_resp
(
    U8  **pp_buffer,
    rrm_rrc_cell_del_resp_t *p_rrm_rrc_cell_del_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_del_req_len
(
    rrc_rrm_cell_del_req_t *p_rrc_rrm_cell_del_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_del_req
(
    U8  **pp_buffer,
    rrc_rrm_cell_del_req_t *p_rrc_rrm_cell_del_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_capability_enquiry_req_len
(
    rrc_rrm_ue_capability_enquiry_req_t *p_rrc_rrm_ue_capability_enquiry_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_capability_enquiry_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_capability_enquiry_req_t *p_rrc_rrm_ue_capability_enquiry_req
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_release_req_len
(
    rrc_rrm_erb_release_req_t *p_rrc_rrm_erb_release_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_release_req
(
    U8  **pp_buffer,
    rrc_rrm_erb_release_req_t *p_rrc_rrm_erb_release_req
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_release_resp_len
(
    rrc_rrm_erb_release_resp_t *p_rrc_rrm_erb_release_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_release_resp
(
    U8  **pp_buffer,
    rrc_rrm_erb_release_resp_t *p_rrc_rrm_erb_release_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_release_cnf_len
(
    rrc_rrm_erb_release_cnf_t *p_rrc_rrm_erb_release_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_release_cnf
(
    U8  **pp_buffer,
    rrc_rrm_erb_release_cnf_t *p_rrc_rrm_erb_release_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_erb_release_ind_len
(
    rrc_rrm_erb_release_ind_t *p_rrc_rrm_erb_release_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_erb_release_ind
(
    U8  **pp_buffer,
    rrc_rrm_erb_release_ind_t *p_rrc_rrm_erb_release_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_capability_enquiry_resp_len
(
    rrc_rrm_ue_capability_enquiry_resp_t *p_rrc_rrm_ue_capability_enquiry_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_capability_enquiry_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_capability_enquiry_resp_t *p_rrc_rrm_ue_capability_enquiry_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_capability_ind_len
(
    rrc_rrm_ue_capability_ind_t *p_rrc_rrm_ue_capability_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_capability_ind
(
    U8  **pp_buffer,
    rrc_rrm_ue_capability_ind_t *p_rrc_rrm_ue_capability_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_ho_required_len
(
    rrc_rrm_ho_required_t *p_rrc_rrm_ho_required
);

rrc_return_et
rrc_il_compose_rrc_rrm_ho_required
(
    U8  **pp_buffer,
    rrc_rrm_ho_required_t *p_rrc_rrm_ho_required
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_ho_adm_req_len
(
    rrc_rrm_ue_ho_adm_req_t *p_rrc_rrm_ue_ho_adm_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_ho_adm_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_ho_adm_req_t *p_rrc_rrm_ue_ho_adm_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_ho_adm_resp_len
(
    rrc_rrm_ue_ho_adm_resp_t *p_rrc_rrm_ue_ho_adm_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_ho_adm_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_ho_adm_resp_t *p_rrc_rrm_ue_ho_adm_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_ho_adm_cnf_len
(
    rrc_rrm_ue_ho_adm_cnf_t *p_rrc_rrm_ue_ho_adm_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_ho_adm_cnf
(
    U8  **pp_buffer,
    rrc_rrm_ue_ho_adm_cnf_t *p_rrc_rrm_ue_ho_adm_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_ho_failure_len
(
    rrc_rrm_ho_failure_t *p_rrc_rrm_ho_failure
);

rrc_return_et
rrc_il_compose_rrc_rrm_ho_failure
(
    U8  **pp_buffer,
    rrc_rrm_ho_failure_t *p_rrc_rrm_ho_failure
);

rrc_length_t
rrc_il_get_rrc_rrm_ho_cancel_req_len
(
    rrc_rrm_ho_cancel_req_t *p_rrc_rrm_ho_cancel_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ho_cancel_req
(
    U8  **pp_buffer,
    rrc_rrm_ho_cancel_req_t *p_rrc_rrm_ho_cancel_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ho_cancel_resp_len
(
    rrc_rrm_ho_cancel_resp_t *p_rrc_rrm_ho_cancel_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ho_cancel_resp
(
    U8  **pp_buffer,
    rrc_rrm_ho_cancel_resp_t *p_rrc_rrm_ho_cancel_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ho_restriction_list_ind_len
(
    rrc_rrm_ho_restriction_list_ind_t *p_rrc_rrm_ho_restriction_list_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_ho_restriction_list_ind
(
    U8  **pp_buffer,
    rrc_rrm_ho_restriction_list_ind_t *p_rrc_rrm_ho_restriction_list_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_dl_nas_transport_ind_len
(
    rrc_rrm_dl_nas_transport_ind_t *p_rrc_rrm_dl_nas_transport_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_dl_nas_transport_ind
(
    U8  **pp_buffer,
    rrc_rrm_dl_nas_transport_ind_t *p_rrc_rrm_dl_nas_transport_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_ho_command_request_len
(
    rrc_rrm_ue_ho_command_request_t *p_rrc_rrm_ue_ho_command_request
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_ho_command_request
(
    U8  **pp_buffer,
    rrc_rrm_ue_ho_command_request_t *p_rrc_rrm_ue_ho_command_request
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_ho_command_response_len
(
    rrc_rrm_ue_ho_command_response_t *p_rrc_rrm_ue_ho_command_response
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_ho_command_response
(
    U8  **pp_buffer,
    rrc_rrm_ue_ho_command_response_t *p_rrc_rrm_ue_ho_command_response
);

rrc_length_t
rrc_il_get_rrc_rrm_meas_config_req_len
(
    rrc_rrm_meas_config_req_t *p_rrc_rrm_meas_config_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_meas_config_req
(
    U8  **pp_buffer,
    rrc_rrm_meas_config_req_t *p_rrc_rrm_meas_config_req
);

rrc_length_t
rrc_il_get_rrc_rrm_meas_config_response_len
(
    rrc_rrm_meas_config_response_t *p_rrc_rrm_meas_config_response
);

rrc_return_et
rrc_il_compose_rrc_rrm_meas_config_response
(
    U8  **pp_buffer,
    rrc_rrm_meas_config_response_t *p_rrc_rrm_meas_config_response
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_contxt_mod_req_len
(
    rrc_rrm_ue_contxt_mod_req_t *p_rrc_rrm_ue_contxt_mod_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_contxt_mod_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_contxt_mod_req_t *p_rrc_rrm_ue_contxt_mod_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_contxt_mod_resp_len
(
    rrc_rrm_ue_contxt_mod_resp_t *p_rrc_rrm_ue_contxt_mod_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_contxt_mod_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_contxt_mod_resp_t *p_rrc_rrm_ue_contxt_mod_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_contxt_mod_cnf_len
(
    rrc_rrm_ue_contxt_mod_cnf_t *p_rrc_rrm_ue_contxt_mod_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_contxt_mod_cnf
(
    U8  **pp_buffer,
    rrc_rrm_ue_contxt_mod_cnf_t *p_rrc_rrm_ue_contxt_mod_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_updated_pws_si_list_req_len
(
    rrc_rrm_updated_pws_si_list_req_t *p_rrc_rrm_updated_pws_si_list_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_updated_pws_si_list_req
(
    U8  **pp_buffer,
    rrc_rrm_updated_pws_si_list_req_t *p_rrc_rrm_updated_pws_si_list_req
);

rrc_length_t
rrc_il_get_rrc_rrm_updated_pws_si_list_resp_len
(
    rrc_rrm_updated_pws_si_list_resp_t *p_rrc_rrm_updated_pws_si_list_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_updated_pws_si_list_resp
(
    U8  **pp_buffer,
    rrc_rrm_updated_pws_si_list_resp_t *p_rrc_rrm_updated_pws_si_list_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_updated_pws_si_list_cnf_len
(
    rrc_rrm_updated_pws_si_list_cnf_t *p_rrc_rrm_updated_pws_si_list_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_updated_pws_si_list_cnf
(
    U8  **pp_buffer,
    rrc_rrm_updated_pws_si_list_cnf_t *p_rrc_rrm_updated_pws_si_list_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_inactive_ues_ind_len
(
    rrc_rrm_inactive_ues_ind_t *p_rrc_rrm_inactive_ues_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_inactive_ues_ind
(
    U8  **pp_buffer,
    rrc_rrm_inactive_ues_ind_t *p_rrc_rrm_inactive_ues_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_start_resp_len
(
    rrc_rrm_cell_start_resp_t *p_rrc_rrm_cell_start_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_start_resp
(
    U8  **pp_buffer,
    rrc_rrm_cell_start_resp_t *p_rrc_rrm_cell_start_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_start_req_len
(
    rrc_rrm_cell_start_req_t *p_rrc_rrm_cell_start_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_start_req
(
    U8  **pp_buffer,
    rrc_rrm_cell_start_req_t *p_rrc_rrm_cell_start_req
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_stop_req_len
(
    rrc_rrm_cell_stop_req_t *p_rrc_rrm_cell_stop_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_stop_req
(
    U8  **pp_buffer,
    rrc_rrm_cell_stop_req_t *p_rrc_rrm_cell_stop_req
);

rrc_length_t
rrc_il_get_rrc_rrm_cell_stop_resp_len
(
    rrc_rrm_cell_stop_resp_t *p_rrc_rrm_cell_stop_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_cell_stop_resp
(
    U8  **pp_buffer,
    rrc_rrm_cell_stop_resp_t *p_rrc_rrm_cell_stop_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_intra_enb_ho_ind_len
(
    rrc_rrm_intra_enb_ho_ind_t *p_rrc_rrm_intra_enb_ho_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_intra_enb_ho_ind
(
    U8  **pp_buffer,
    rrc_rrm_intra_enb_ho_ind_t *p_rrc_rrm_intra_enb_ho_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_csfb_parameters_req_cdma2000_len
(
    rrc_rrm_csfb_parameters_req_cdma2000_t *p_rrc_rrm_csfb_parameters_req_cdma2000
);

rrc_return_et
rrc_il_compose_rrc_rrm_csfb_parameters_req_cdma2000
(
    U8  **pp_buffer,
    rrc_rrm_csfb_parameters_req_cdma2000_t *p_rrc_rrm_csfb_parameters_req_cdma2000
);

rrc_length_t
rrc_il_get_rrc_rrm_csfb_parameters_resp_cdma2000_len
(
    rrc_rrm_csfb_parameters_resp_cdma2000_t *p_rrc_rrm_csfb_parameters_resp_cdma2000
);

rrc_return_et
rrc_il_compose_rrc_rrm_csfb_parameters_resp_cdma2000
(
    U8  **pp_buffer,
    rrc_rrm_csfb_parameters_resp_cdma2000_t *p_rrc_rrm_csfb_parameters_resp_cdma2000
);

rrc_length_t
rrc_il_get_rrc_rrm_counter_check_req_len
(
    rrc_rrm_counter_check_req_t *p_rrc_rrm_counter_check_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_counter_check_req
(
    U8  **pp_buffer,
    rrc_rrm_counter_check_req_t *p_rrc_rrm_counter_check_req
);

rrc_length_t
rrc_il_get_rrc_rrm_counter_check_resp_len
(
    rrc_rrm_counter_check_resp_t *p_rrc_rrm_counter_check_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_counter_check_resp
(
    U8  **pp_buffer,
    rrc_rrm_counter_check_resp_t *p_rrc_rrm_counter_check_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_information_req_len
(
    rrc_rrm_ue_information_req_t *p_rrc_rrm_ue_information_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_information_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_information_req_t *p_rrc_rrm_ue_information_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_information_resp_len
(
    rrc_rrm_ue_information_resp_t *p_rrc_rrm_ue_information_resp
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_information_resp
(
    U8  **pp_buffer,
    rrc_rrm_ue_information_resp_t *p_rrc_rrm_ue_information_resp
);

rrc_length_t
rrc_il_get_rrc_rrm_inter_freq_rstd_measurement_ind_len
(
    rrc_rrm_inter_freq_rstd_measurement_ind_t *p_rrc_rrm_inter_freq_rstd_measurement_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_inter_freq_rstd_measurement_ind
(
    U8  **pp_buffer,
    rrc_rrm_inter_freq_rstd_measurement_ind_t *p_rrc_rrm_inter_freq_rstd_measurement_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_local_error_ind_len
(
    rrc_rrm_local_error_ind_t *p_rrc_rrm_local_error_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_local_error_ind
(
    U8  **pp_buffer,
    rrc_rrm_local_error_ind_t *p_rrc_rrm_local_error_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_reestab_config_req_len
(
    rrc_rrm_ue_reestab_config_req_t *p_rrc_rrm_ue_reestab_config_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_reestab_config_req
(
    U8  **pp_buffer,
    rrc_rrm_ue_reestab_config_req_t *p_rrc_rrm_ue_reestab_config_req
);

rrc_length_t
rrc_il_get_rrc_rrm_ue_reestab_config_cnf_len
(
    rrc_rrm_ue_reestab_config_cnf_t *p_rrc_rrm_ue_reestab_config_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_ue_reestab_config_cnf
(
    U8  **pp_buffer,
    rrc_rrm_ue_reestab_config_cnf_t *p_rrc_rrm_ue_reestab_config_cnf
);

rrc_length_t
rrc_il_get_rrc_rrm_wlan_connection_status_ind_len
(
    rrc_rrm_wlan_connection_status_ind *p_rrc_rrm_wlan_connection_status_ind
);

rrc_return_et
rrc_il_compose_rrc_rrm_wlan_connection_status_ind
(
    U8  **pp_buffer,
    rrc_rrm_wlan_connection_status_ind *p_rrc_rrm_wlan_connection_status_ind
);

rrc_length_t
rrc_il_get_rrc_rrm_dc_bearer_change_req_len
(
    rrc_rrm_dc_bearer_change_req_t *p_rrc_rrm_dc_bearer_change_req
);

rrc_return_et
rrc_il_compose_rrc_rrm_dc_bearer_change_req
(
    U8  **pp_buffer,
    rrc_rrm_dc_bearer_change_req_t *p_rrc_rrm_dc_bearer_change_req
);

rrc_length_t
rrc_il_get_rrc_rrm_dc_bearer_change_cnf_len
(
    rrc_rrm_dc_bearer_change_cnf_t *p_rrc_rrm_dc_bearer_change_cnf
);

rrc_return_et
rrc_il_compose_rrc_rrm_dc_bearer_change_cnf
(
    U8  **pp_buffer,
    rrc_rrm_dc_bearer_change_cnf_t *p_rrc_rrm_dc_bearer_change_cnf
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_sync_status
(
    rrc_rrm_ue_sync_status_t  *p_rrc_rrm_ue_sync_status,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_reestab_complete_ind
(
    rrc_rrm_ue_reestab_complete_ind_t  *p_rrc_rrm_ue_reestab_complete_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_release_req
(
    rrc_rrm_ue_release_req_t  *p_rrc_rrm_ue_release_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_release_resp
(
    rrc_rrm_ue_release_resp_t  *p_rrc_rrm_ue_release_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_setup_resp
(
    rrc_rrm_cell_setup_resp_t  *p_rrc_rrm_cell_setup_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_reconfig_req
(
    rrc_rrm_cell_reconfig_req_t  *p_rrc_rrm_cell_reconfig_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_reconfig_resp
(
    rrc_rrm_cell_reconfig_resp_t  *p_rrc_rrm_cell_reconfig_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_setup_req
(
    rrc_rrm_cell_setup_req_t  *p_rrc_rrm_cell_setup_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_admission_req
(
    rrc_rrm_ue_admission_req_t  *p_rrc_rrm_ue_admission_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_admission_resp
(
    rrc_rrm_ue_admission_resp_t  *p_rrc_rrm_ue_admission_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_admission_cnf
(
    rrc_rrm_ue_admission_cnf_t  *p_rrc_rrm_ue_admission_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_reconfig_req
(
    rrc_rrm_ue_reconfig_req_t  *p_rrc_rrm_ue_reconfig_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_reconfig_resp
(
    rrc_rrm_ue_reconfig_resp_t  *p_rrc_rrm_ue_reconfig_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_setup_req
(
    rrc_rrm_erb_setup_req_t  *p_rrc_rrm_erb_setup_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_setup_resp
(
    rrc_rrm_erb_setup_resp_t  *p_rrc_rrm_erb_setup_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_setup_cnf
(
    rrc_rrm_erb_setup_cnf_t  *p_rrc_rrm_erb_setup_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erab_reconfig_req
(
    rrc_rrm_erab_reconfig_req_t  *p_rrc_rrm_erab_reconfig_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_reconfig_resp
(
    rrc_rrm_erb_reconfig_resp_t  *p_rrc_rrm_erb_reconfig_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erab_modify_req
(
    rrc_rrm_erab_modify_req_t  *p_rrc_rrm_erab_modify_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erab_modify_resp
(
    rrc_rrm_erab_modify_resp_t  *p_rrc_rrm_erab_modify_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_modify_cnf
(
    rrc_rrm_erb_modify_cnf_t  *p_rrc_rrm_erb_modify_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_connection_release_ind
(
    rrc_rrm_ue_connection_release_ind_t  *p_rrc_rrm_ue_connection_release_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_measurment_results_ind
(
    rrc_rrm_measurment_results_ind_t  *p_rrc_rrm_measurment_results_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_proximity_ind
(
    rrc_rrm_proximity_ind_t  *p_rrc_rrm_proximity_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrm_rrc_cell_del_resp
(
    rrm_rrc_cell_del_resp_t  *p_rrm_rrc_cell_del_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_del_req
(
    rrc_rrm_cell_del_req_t  *p_rrc_rrm_cell_del_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_capability_enquiry_req
(
    rrc_rrm_ue_capability_enquiry_req_t  *p_rrc_rrm_ue_capability_enquiry_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_release_req
(
    rrc_rrm_erb_release_req_t  *p_rrc_rrm_erb_release_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_release_resp
(
    rrc_rrm_erb_release_resp_t  *p_rrc_rrm_erb_release_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_release_cnf
(
    rrc_rrm_erb_release_cnf_t  *p_rrc_rrm_erb_release_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_erb_release_ind
(
    rrc_rrm_erb_release_ind_t  *p_rrc_rrm_erb_release_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_capability_enquiry_resp
(
    rrc_rrm_ue_capability_enquiry_resp_t  *p_rrc_rrm_ue_capability_enquiry_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_capability_ind
(
    rrc_rrm_ue_capability_ind_t  *p_rrc_rrm_ue_capability_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ho_required
(
    rrc_rrm_ho_required_t  *p_rrc_rrm_ho_required,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_ho_adm_req
(
    rrc_rrm_ue_ho_adm_req_t  *p_rrc_rrm_ue_ho_adm_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_ho_adm_resp
(
    rrc_rrm_ue_ho_adm_resp_t  *p_rrc_rrm_ue_ho_adm_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_ho_adm_cnf
(
    rrc_rrm_ue_ho_adm_cnf_t  *p_rrc_rrm_ue_ho_adm_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ho_failure
(
    rrc_rrm_ho_failure_t  *p_rrc_rrm_ho_failure,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ho_cancel_req
(
    rrc_rrm_ho_cancel_req_t  *p_rrc_rrm_ho_cancel_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ho_cancel_resp
(
    rrc_rrm_ho_cancel_resp_t  *p_rrc_rrm_ho_cancel_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ho_restriction_list_ind
(
    rrc_rrm_ho_restriction_list_ind_t  *p_rrc_rrm_ho_restriction_list_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_dl_nas_transport_ind
(
    rrc_rrm_dl_nas_transport_ind_t  *p_rrc_rrm_dl_nas_transport_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_ho_command_request
(
    rrc_rrm_ue_ho_command_request_t  *p_rrc_rrm_ue_ho_command_request,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_ho_command_response
(
    rrc_rrm_ue_ho_command_response_t  *p_rrc_rrm_ue_ho_command_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_meas_config_req
(
    rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_meas_config_response
(
    rrc_rrm_meas_config_response_t  *p_rrc_rrm_meas_config_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_req
(
    rrc_rrm_ue_contxt_mod_req_t  *p_rrc_rrm_ue_contxt_mod_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_resp
(
    rrc_rrm_ue_contxt_mod_resp_t  *p_rrc_rrm_ue_contxt_mod_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf
(
    rrc_rrm_ue_contxt_mod_cnf_t  *p_rrc_rrm_ue_contxt_mod_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_req
(
    rrc_rrm_updated_pws_si_list_req_t  *p_rrc_rrm_updated_pws_si_list_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_resp
(
    rrc_rrm_updated_pws_si_list_resp_t  *p_rrc_rrm_updated_pws_si_list_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_cnf
(
    rrc_rrm_updated_pws_si_list_cnf_t  *p_rrc_rrm_updated_pws_si_list_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_inactive_ues_ind
(
    rrc_rrm_inactive_ues_ind_t  *p_rrc_rrm_inactive_ues_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_start_resp
(
    rrc_rrm_cell_start_resp_t  *p_rrc_rrm_cell_start_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_start_req
(
    rrc_rrm_cell_start_req_t  *p_rrc_rrm_cell_start_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_stop_req
(
    rrc_rrm_cell_stop_req_t  *p_rrc_rrm_cell_stop_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_cell_stop_resp
(
    rrc_rrm_cell_stop_resp_t  *p_rrc_rrm_cell_stop_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_intra_enb_ho_ind
(
    rrc_rrm_intra_enb_ho_ind_t  *p_rrc_rrm_intra_enb_ho_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_csfb_parameters_req_cdma2000
(
    rrc_rrm_csfb_parameters_req_cdma2000_t  *p_rrc_rrm_csfb_parameters_req_cdma2000,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_csfb_parameters_resp_cdma2000
(
    rrc_rrm_csfb_parameters_resp_cdma2000_t  *p_rrc_rrm_csfb_parameters_resp_cdma2000,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_counter_check_req
(
    rrc_rrm_counter_check_req_t  *p_rrc_rrm_counter_check_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_counter_check_resp
(
    rrc_rrm_counter_check_resp_t  *p_rrc_rrm_counter_check_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_information_req
(
    rrc_rrm_ue_information_req_t  *p_rrc_rrm_ue_information_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_information_resp
(
    rrc_rrm_ue_information_resp_t  *p_rrc_rrm_ue_information_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_inter_freq_rstd_measurement_ind
(
    rrc_rrm_inter_freq_rstd_measurement_ind_t  *p_rrc_rrm_inter_freq_rstd_measurement_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_local_error_ind
(
    rrc_rrm_local_error_ind_t  *p_rrc_rrm_local_error_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_reestab_config_req
(
    rrc_rrm_ue_reestab_config_req_t  *p_rrc_rrm_ue_reestab_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_ue_reestab_config_cnf
(
    rrc_rrm_ue_reestab_config_cnf_t  *p_rrc_rrm_ue_reestab_config_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_wlan_connection_status_ind
(
    rrc_rrm_wlan_connection_status_ind  *p_rrc_rrm_wlan_connection_status_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_dc_bearer_change_req
(
    rrc_rrm_dc_bearer_change_req_t  *p_rrc_rrm_dc_bearer_change_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rrm_il_send_rrc_rrm_dc_bearer_change_cnf
(
    rrc_rrm_dc_bearer_change_cnf_t  *p_rrc_rrm_dc_bearer_change_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_RRM_IL_COMPOSER_H_ */
