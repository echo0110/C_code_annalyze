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
 *  File Description : The file oam_s1ap_il_composer.h contains the prototypes 
 *                     of RRC-S1APOAM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _OAM_S1AP_IL_COMPOSER_H_
#define _OAM_S1AP_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_len
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
);

oam_return_et
rrc_il_compose_s1ap_oam_enb_config_update
(
    UInt8  **pp_buffer,
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
);

rrc_length_t
rrc_il_get_s1ap_oam_mme_conn_status_response_len
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
);

oam_return_et
rrc_il_compose_s1ap_oam_mme_conn_status_response
(
    UInt8  **pp_buffer,
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
);

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_response_len
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
);

oam_return_et
rrc_il_compose_s1ap_oam_enb_config_update_response
(
    UInt8  **pp_buffer,
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
);

rrc_length_t
rrc_il_get_s1ap_oam_close_mme_conn_len
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
);

oam_return_et
rrc_il_compose_s1ap_oam_close_mme_conn
(
    UInt8  **pp_buffer,
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
);

rrc_length_t
rrc_il_get_s1ap_oam_reestab_mme_conn_len
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
);

oam_return_et
rrc_il_compose_s1ap_oam_reestab_mme_conn
(
    UInt8  **pp_buffer,
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
);

rrc_length_t
rrc_il_get_s1ap_oam_provision_req_len
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
);

oam_return_et
rrc_il_compose_s1ap_oam_provision_req
(
    UInt8  **pp_buffer,
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
);

rrc_length_t
rrc_il_get_s1ap_oam_provision_resp_len
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_provision_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_get_log_level_resp_len
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_get_log_level_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_req_len
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
);

oam_return_et
rrc_il_compose_s1ap_oam_set_log_level_req
(
    UInt8  **pp_buffer,
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
);

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_resp_len
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_set_log_level_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_req_len
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
);

oam_return_et
rrc_il_compose_s1ap_oam_log_enable_req
(
    UInt8  **pp_buffer,
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
);

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_resp_len
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_log_enable_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_reset_resp_len
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_reset_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_s1ap_link_status_ind_len
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
);

oam_return_et
rrc_il_compose_s1ap_oam_s1ap_link_status_ind
(
    UInt8  **pp_buffer,
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
);

rrc_length_t
rrc_il_get_s1ap_oam_status_resp_len
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_status_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
);

rrc_length_t
rrc_il_get_s1ap_oam_mme_info_len
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
);

oam_return_et
rrc_il_compose_s1ap_oam_mme_info
(
    UInt8  **pp_buffer,
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
);

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_req_len
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
);

oam_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_req
(
    UInt8  **pp_buffer,
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
);

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_resp_len
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
);

oam_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_resp
(
    UInt8  **pp_buffer,
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t  *p_s1ap_oam_enb_config_update,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t  *p_s1ap_oam_mme_conn_status_response,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t  *p_s1ap_oam_enb_config_update_response,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t  *p_s1ap_oam_close_mme_conn,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t  *p_s1ap_oam_reestab_mme_conn,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t  *p_s1ap_oam_provision_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t  *p_s1ap_oam_provision_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t  *p_s1ap_oam_get_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t  *p_s1ap_oam_set_log_level_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t  *p_s1ap_oam_set_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t  *p_s1ap_oam_log_enable_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t  *p_s1ap_oam_log_enable_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t  *p_s1ap_oam_reset_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t  *p_s1ap_oam_s1ap_link_status_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t  *p_s1ap_oam_status_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t  *p_s1ap_oam_mme_info,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t  *p_s1ap_oam_get_sctp_status_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1ap_il_send_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t  *p_s1ap_oam_get_sctp_status_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

#endif /* _OAM_S1AP_IL_COMPOSER_H_ */
