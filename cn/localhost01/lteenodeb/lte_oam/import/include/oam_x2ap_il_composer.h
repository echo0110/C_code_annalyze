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
 *  File Description : The file oam_x2ap_il_composer.h contains the prototypes 
 *                     of RRC-X2APOAM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _OAM_X2AP_IL_COMPOSER_H_
#define _OAM_X2AP_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apOam_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

rrc_length_t
rrc_il_get_x2ap_oam_provision_req_len
(
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
);

oam_return_et
rrc_il_compose_x2ap_oam_provision_req
(
    UInt8  **pp_buffer,
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
);

rrc_length_t
rrc_il_get_x2ap_oam_provision_resp_len
(
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_provision_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_get_log_level_resp_len
(
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_get_log_level_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_req_len
(
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
);

oam_return_et
rrc_il_compose_x2ap_oam_set_log_level_req
(
    UInt8  **pp_buffer,
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
);

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_resp_len
(
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_set_log_level_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_req_len
(
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
);

oam_return_et
rrc_il_compose_x2ap_oam_log_enable_req
(
    UInt8  **pp_buffer,
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
);

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_resp_len
(
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_log_enable_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_down_len
(
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
);

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_down
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_up_len
(
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
);

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_up
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_req_len
(
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
);

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_req
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
);

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_resp_len
(
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_cleanup_resp_len
(
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_cleanup_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
);

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_req_len
(
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
);

oam_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_req
(
    UInt8  **pp_buffer,
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
);

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_resp_len
(
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
);

oam_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_provision_req
(
    x2ap_oam_provision_req_t  *p_x2ap_oam_provision_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_provision_resp
(
    x2ap_oam_provision_resp_t  *p_x2ap_oam_provision_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_get_log_level_resp
(
    x2ap_oam_get_log_level_resp_t  *p_x2ap_oam_get_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_set_log_level_req
(
    x2ap_oam_set_log_level_req_t  *p_x2ap_oam_set_log_level_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_set_log_level_resp
(
    x2ap_oam_set_log_level_resp_t  *p_x2ap_oam_set_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_log_enable_req
(
    x2ap_oam_log_enable_req_t  *p_x2ap_oam_log_enable_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_log_enable_resp
(
    x2ap_oam_log_enable_resp_t  *p_x2ap_oam_log_enable_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_down
(
    x2ap_oam_x2_link_down_t  *p_x2ap_oam_x2_link_down,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_up
(
    x2ap_oam_x2_link_up_t  *p_x2ap_oam_x2_link_up,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_setup_req
(
    x2ap_oam_x2_link_setup_req_t  *p_x2ap_oam_x2_link_setup_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_setup_resp
(
    x2ap_oam_x2_link_setup_resp_t  *p_x2ap_oam_x2_link_setup_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_cleanup_resp
(
    x2ap_oam_cleanup_resp_t  *p_x2ap_oam_cleanup_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2ap_il_send_x2ap_oam_get_sctp_status_resp
(
    x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

#endif /* _OAM_X2AP_IL_COMPOSER_H_ */
