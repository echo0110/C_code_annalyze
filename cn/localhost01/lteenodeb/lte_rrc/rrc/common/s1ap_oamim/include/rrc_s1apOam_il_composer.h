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
 *  File Description : The file rrc_s1apOam_il_composer.h contains the prototypes 
 *                     of RRC-S1APOAM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1APOAM_IL_COMPOSER_H_
#define _RRC_S1APOAM_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_len
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
);

rrc_return_et
rrc_il_compose_s1ap_oam_enb_config_update
(
    U8  **pp_buffer,
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
);

rrc_length_t
rrc_il_get_s1ap_oam_mme_conn_status_response_len
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
);

rrc_return_et
rrc_il_compose_s1ap_oam_mme_conn_status_response
(
    U8  **pp_buffer,
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
);

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_response_len
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
);

rrc_return_et
rrc_il_compose_s1ap_oam_enb_config_update_response
(
    U8  **pp_buffer,
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
);

rrc_length_t
rrc_il_get_s1ap_oam_close_mme_conn_len
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
);

rrc_return_et
rrc_il_compose_s1ap_oam_close_mme_conn
(
    U8  **pp_buffer,
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
);

rrc_length_t
rrc_il_get_s1ap_oam_reestab_mme_conn_len
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
);

rrc_return_et
rrc_il_compose_s1ap_oam_reestab_mme_conn
(
    U8  **pp_buffer,
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
);

rrc_length_t
rrc_il_get_s1ap_oam_provision_req_len
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
);

rrc_return_et
rrc_il_compose_s1ap_oam_provision_req
(
    U8  **pp_buffer,
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
);

rrc_length_t
rrc_il_get_s1ap_oam_provision_resp_len
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_provision_resp
(
    U8  **pp_buffer,
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_get_log_level_resp_len
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_get_log_level_resp
(
    U8  **pp_buffer,
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_req_len
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
);

rrc_return_et
rrc_il_compose_s1ap_oam_set_log_level_req
(
    U8  **pp_buffer,
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
);

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_resp_len
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_set_log_level_resp
(
    U8  **pp_buffer,
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_req_len
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
);

rrc_return_et
rrc_il_compose_s1ap_oam_log_enable_req
(
    U8  **pp_buffer,
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
);

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_resp_len
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_log_enable_resp
(
    U8  **pp_buffer,
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_reset_resp_len
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_reset_resp
(
    U8  **pp_buffer,
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_s1ap_link_status_ind_len
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
);

rrc_return_et
rrc_il_compose_s1ap_oam_s1ap_link_status_ind
(
    U8  **pp_buffer,
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
);

rrc_length_t
rrc_il_get_s1ap_oam_status_resp_len
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_status_resp
(
    U8  **pp_buffer,
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_mme_info_len
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
);

rrc_return_et
rrc_il_compose_s1ap_oam_mme_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
);

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_req_len
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
);

rrc_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_req
(
    U8  **pp_buffer,
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
);

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_resp_len
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
);

rrc_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_resp
(
    U8  **pp_buffer,
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t  *p_s1ap_oam_enb_config_update,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t  *p_s1ap_oam_mme_conn_status_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t  *p_s1ap_oam_enb_config_update_response,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t  *p_s1ap_oam_close_mme_conn,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t  *p_s1ap_oam_reestab_mme_conn,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t  *p_s1ap_oam_provision_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t  *p_s1ap_oam_provision_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t  *p_s1ap_oam_get_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t  *p_s1ap_oam_set_log_level_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t  *p_s1ap_oam_set_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t  *p_s1ap_oam_log_enable_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t  *p_s1ap_oam_log_enable_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t  *p_s1ap_oam_reset_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t  *p_s1ap_oam_s1ap_link_status_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t  *p_s1ap_oam_status_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#ifdef LINUX_PC_TEST
rrc_return_et
rrc_s1apOam_il_send_s1ap_gb_status_resp
(
    s1ap_gb_status_resp_t  *p_s1ap_gb_status_resp, 
    U16                 src_module_id, 
    U16                 dst_module_id, 
    U16                 transaction_id,
    U8                  cell_index     
);

rrc_length_t
	rrc_il_get_s1ap_gb_status_resp_len

(
    s1ap_gb_status_resp_t *p_s1ap_gb_status_resp
);

rrc_return_et
	rrc_il_compose_s1ap_gb_status_resp

(
    U8  **pp_buffer,
    s1ap_gb_status_resp_t *p_s1ap_gb_status_resp
);
#endif

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t  *p_s1ap_oam_mme_info,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t  *p_s1ap_oam_get_sctp_status_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t  *p_s1ap_oam_get_sctp_status_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_S1APOAM_IL_COMPOSER_H_ */
