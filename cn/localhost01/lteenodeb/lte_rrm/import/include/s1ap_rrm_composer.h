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
 *  File Description : The file rrm_il_composer.h contains the prototypes 
 *                     of RRC-S1APRRM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1APRRM_IL_COMPOSER_H_
#define _RRC_S1APRRM_IL_COMPOSER_H_

#include "rrm_defines.h"
#include "rrc_s1apRrm_intf.h"

rrm_length_t
rrm_il_get_s1ap_rrm_enb_direct_info_transfer_len
(
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer
);

rrm_return_et
rrm_il_compose_s1ap_rrm_enb_direct_info_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer
);

rrm_length_t
rrm_il_get_s1ap_rrm_mme_direct_info_transfer_len
(
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer
);

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_direct_info_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer
);

rrm_length_t
rrm_il_get_s1ap_rrm_enb_config_transfer_len
(
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer
);

rrm_return_et
rrm_il_compose_s1ap_rrm_enb_config_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer
);

rrm_length_t
rrm_il_get_s1ap_rrm_mme_config_transfer_len
(
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer
);

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_config_transfer
(
    U8  **pp_buffer,
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer
);

rrm_length_t
rrm_il_get_s1ap_rrm_pws_request_len
(
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request
);

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_request
(
    U8  **pp_buffer,
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request
);

rrm_length_t
rrm_il_get_s1ap_rrm_pws_response_len
(
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response
);

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_response
(
    U8  **pp_buffer,
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response
);

rrm_length_t
rrm_il_get_s1ap_rrm_pws_cnf_len
(
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf
);

rrm_return_et
rrm_il_compose_s1ap_rrm_pws_cnf
(
    U8  **pp_buffer,
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf
);

rrm_length_t
rrm_il_get_s1ap_rrm_kill_request_len
(
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request
);

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_request
(
    U8  **pp_buffer,
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request
);

rrm_length_t
rrm_il_get_s1ap_rrm_kill_response_len
(
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response
);

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_response
(
    U8  **pp_buffer,
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response
);

rrm_length_t
rrm_il_get_s1ap_rrm_kill_cnf_len
(
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf
);

rrm_return_et
rrm_il_compose_s1ap_rrm_kill_cnf
(
    U8  **pp_buffer,
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf
);

rrm_length_t
rrm_il_get_rim_information_req_len
(
    rim_information_req_t *p_rim_information_req
);

rrm_return_et
rrm_il_compose_rim_information_req
(
    U8  **pp_buffer,
    rim_information_req_t *p_rim_information_req
);

rrm_length_t
rrm_il_get_rim_information_len
(
    rim_information_t *p_rim_information
);

rrm_return_et
rrm_il_compose_rim_information
(
    U8  **pp_buffer,
    rim_information_t *p_rim_information
);

rrm_length_t
rrm_il_get_rim_information_ack_len
(
    rim_information_ack_t *p_rim_information_ack
);

rrm_return_et
rrm_il_compose_rim_information_ack
(
    U8  **pp_buffer,
    rim_information_ack_t *p_rim_information_ack
);

rrm_length_t
rrm_il_get_rim_information_app_err_len
(
    rim_information_app_err_t *p_rim_information_app_err
);

rrm_return_et
rrm_il_compose_rim_information_app_err
(
    U8  **pp_buffer,
    rim_information_app_err_t *p_rim_information_app_err
);

rrm_length_t
rrm_il_get_rim_information_err_len
(
    rim_information_err_t *p_rim_information_err
);

rrm_return_et
rrm_il_compose_rim_information_err
(
    U8  **pp_buffer,
    rim_information_err_t *p_rim_information_err
);

rrm_length_t
rrm_il_get_s1ap_rrm_mme_info_len
(
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info
);

rrm_return_et
rrm_il_compose_s1ap_rrm_mme_info
(
    U8  **pp_buffer,
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_req_len
(
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_resp_len
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_resp
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_init_fail_len
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_init_fail
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_fail_ind_len
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_report_len
(
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_report
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_ecid_meas_termination_cmd_len
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_req_len
(
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_resp_len
(
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_resp
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_otdoa_info_fail_len
(
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_otdoa_info_fail
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_error_ind_len
(
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_error_ind
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind
);

rrm_length_t
rrm_il_get_s1ap_rrm_lppa_error_ind_req_len
(
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req
);

rrm_return_et
rrm_il_compose_s1ap_rrm_lppa_error_ind_req
(
    U8  **pp_buffer,
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req
);

rrm_return_et
rrm_il_send_s1ap_rrm_enb_direct_info_transfer
(
    s1ap_rrm_enb_direct_info_transfer_t  *p_s1ap_rrm_enb_direct_info_transfer,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_mme_direct_info_transfer
(
    s1ap_rrm_mme_direct_info_transfer_t  *p_s1ap_rrm_mme_direct_info_transfer,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_enb_config_transfer
(
    s1ap_rrm_enb_config_transfer_t  *p_s1ap_rrm_enb_config_transfer,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_mme_config_transfer
(
    s1ap_rrm_mme_config_transfer_t  *p_s1ap_rrm_mme_config_transfer,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_pws_request
(
    s1ap_rrm_pws_request_t  *p_s1ap_rrm_pws_request,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_pws_response
(
    s1ap_rrm_pws_response_t  *p_s1ap_rrm_pws_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_pws_cnf
(
    s1ap_rrm_pws_cnf_t  *p_s1ap_rrm_pws_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_kill_request
(
    s1ap_rrm_kill_request_t  *p_s1ap_rrm_kill_request,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_kill_response
(
    s1ap_rrm_kill_response_t  *p_s1ap_rrm_kill_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_kill_cnf
(
    s1ap_rrm_kill_cnf_t  *p_s1ap_rrm_kill_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_rim_information_req
(
    rim_information_req_t  *p_rim_information_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_rim_information
(
    rim_information_t  *p_rim_information,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_rim_information_ack
(
    rim_information_ack_t  *p_rim_information_ack,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_rim_information_app_err
(
    rim_information_app_err_t  *p_rim_information_app_err,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_rim_information_err
(
    rim_information_err_t  *p_rim_information_err,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_mme_info
(
    s1ap_rrm_mme_info_t  *p_s1ap_rrm_mme_info,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_req
(
    s1ap_rrm_lppa_ecid_meas_init_req_t  *p_s1ap_rrm_lppa_ecid_meas_init_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_resp
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t  *p_s1ap_rrm_lppa_ecid_meas_init_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_init_fail
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t  *p_s1ap_rrm_lppa_ecid_meas_init_fail,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t  *p_s1ap_rrm_lppa_ecid_meas_fail_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_report
(
    s1ap_rrm_lppa_ecid_meas_report_t  *p_s1ap_rrm_lppa_ecid_meas_report,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t  *p_s1ap_rrm_lppa_ecid_meas_termination_cmd,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_req
(
    s1ap_rrm_lppa_otdoa_info_req_t  *p_s1ap_rrm_lppa_otdoa_info_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_resp
(
    s1ap_rrm_lppa_otdoa_info_resp_t  *p_s1ap_rrm_lppa_otdoa_info_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_otdoa_info_fail
(
    s1ap_rrm_lppa_otdoa_info_fail_t  *p_s1ap_rrm_lppa_otdoa_info_fail,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_error_ind
(
    s1ap_rrm_lppa_error_ind_t  *p_s1ap_rrm_lppa_error_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_il_send_s1ap_rrm_lppa_error_ind_req
(
    s1ap_rrm_lppa_error_ind_req_t  *p_s1ap_rrm_lppa_error_ind_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_S1APRRM_IL_COMPOSER_H_ */
