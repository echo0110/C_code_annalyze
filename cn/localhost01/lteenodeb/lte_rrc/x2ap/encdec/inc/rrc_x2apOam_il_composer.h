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
 *  File Description : The file rrc_x2apOam_il_composer.h contains the prototypes 
 *                     of RRC-X2APOAM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APOAM_IL_COMPOSER_H_
#define _RRC_X2APOAM_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apOam_intf.h"

rrc_length_t
rrc_il_get_x2ap_oam_provision_req_len
(
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
);

rrc_return_et
rrc_il_compose_x2ap_oam_provision_req
(
    U8  **pp_buffer,
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
);

rrc_length_t
rrc_il_get_x2ap_oam_provision_resp_len
(
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_provision_resp
(
    U8  **pp_buffer,
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_get_log_level_resp_len
(
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_get_log_level_resp
(
    U8  **pp_buffer,
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_req_len
(
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
);

rrc_return_et
rrc_il_compose_x2ap_oam_set_log_level_req
(
    U8  **pp_buffer,
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
);

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_resp_len
(
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_set_log_level_resp
(
    U8  **pp_buffer,
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_req_len
(
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
);

rrc_return_et
rrc_il_compose_x2ap_oam_log_enable_req
(
    U8  **pp_buffer,
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
);

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_resp_len
(
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_log_enable_resp
(
    U8  **pp_buffer,
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_down_len
(
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
);

rrc_return_et
rrc_il_compose_x2ap_oam_x2_link_down
(
    U8  **pp_buffer,
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_up_len
(
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
);

rrc_return_et
rrc_il_compose_x2ap_oam_x2_link_up
(
    U8  **pp_buffer,
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_req_len
(
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
);

rrc_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_req
(
    U8  **pp_buffer,
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_resp_len
(
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_resp
(
    U8  **pp_buffer,
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_cleanup_resp_len
(
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_cleanup_resp
(
    U8  **pp_buffer,
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_req_len
(
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
);

rrc_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_req
(
    U8  **pp_buffer,
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
);

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_resp_len
(
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
);

rrc_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_resp
(
    U8  **pp_buffer,
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_provision_req
(
    x2ap_oam_provision_req_t  *p_x2ap_oam_provision_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_provision_resp
(
    x2ap_oam_provision_resp_t  *p_x2ap_oam_provision_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_get_log_level_resp
(
    x2ap_oam_get_log_level_resp_t  *p_x2ap_oam_get_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_set_log_level_req
(
    x2ap_oam_set_log_level_req_t  *p_x2ap_oam_set_log_level_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_set_log_level_resp
(
    x2ap_oam_set_log_level_resp_t  *p_x2ap_oam_set_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_log_enable_req
(
    x2ap_oam_log_enable_req_t  *p_x2ap_oam_log_enable_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_log_enable_resp
(
    x2ap_oam_log_enable_resp_t  *p_x2ap_oam_log_enable_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_x2_link_down
(
    x2ap_oam_x2_link_down_t  *p_x2ap_oam_x2_link_down,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_x2_link_up
(
    x2ap_oam_x2_link_up_t  *p_x2ap_oam_x2_link_up,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_x2_link_setup_req
(
    x2ap_oam_x2_link_setup_req_t  *p_x2ap_oam_x2_link_setup_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_x2_link_setup_resp
(
    x2ap_oam_x2_link_setup_resp_t  *p_x2ap_oam_x2_link_setup_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_cleanup_resp
(
    x2ap_oam_cleanup_resp_t  *p_x2ap_oam_cleanup_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apOam_il_send_x2ap_oam_get_sctp_status_resp
(
    x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_X2APOAM_IL_COMPOSER_H_ */
