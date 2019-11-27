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
 *  File Description : The file oam_x2apCommon_il_composer.h contains the prototypes 
 *                     of OAM-L3 interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APCOMMON_IL_COMPOSER_H_
#define _RRC_X2APCOMMON_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apCommon_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

rrc_length_t
rrc_get_x2ap_enb_config_update_req_len
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
);

oam_return_et
oam_compose_x2ap_enb_config_update_req
(
    UInt8  **pp_buffer,
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
);

rrc_length_t
rrc_get_x2ap_enb_config_update_resp_len
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
);

oam_return_et
oam_compose_x2ap_enb_config_update_resp
(
    UInt8  **pp_buffer,
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
);

rrc_length_t
rrc_get_x2ap_enb_config_update_ind_len
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
);

oam_return_et
oam_compose_x2ap_enb_config_update_ind
(
    UInt8  **pp_buffer,
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
);

rrc_length_t
rrc_get_x2ap_reset_req_len
(
    x2ap_reset_req_t *p_x2ap_reset_req
);

oam_return_et
oam_compose_x2ap_reset_req
(
    UInt8  **pp_buffer,
    x2ap_reset_req_t *p_x2ap_reset_req
);

rrc_length_t
rrc_get_x2ap_reset_resp_len
(
    x2ap_reset_resp_t *p_x2ap_reset_resp
);

oam_return_et
oam_compose_x2ap_reset_resp
(
    UInt8  **pp_buffer,
    x2ap_reset_resp_t *p_x2ap_reset_resp
);

rrc_length_t
rrc_get_x2ap_reset_ind_len
(
    x2ap_reset_ind_t *p_x2ap_reset_ind
);

oam_return_et
oam_compose_x2ap_reset_ind
(
    UInt8  **pp_buffer,
    x2ap_reset_ind_t *p_x2ap_reset_ind
);

rrc_length_t
rrc_get_x2ap_add_enb_req_len
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
);

oam_return_et
oam_compose_x2ap_add_enb_req
(
    UInt8  **pp_buffer,
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
);

rrc_length_t
rrc_get_x2ap_add_enb_res_len
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
);

oam_return_et
oam_compose_x2ap_add_enb_res
(
    UInt8  **pp_buffer,
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
);

rrc_length_t
rrc_get_x2ap_delete_enb_req_len
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
);

oam_return_et
oam_compose_x2ap_delete_enb_req
(
    UInt8  **pp_buffer,
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
);

rrc_length_t
rrc_get_x2ap_delete_enb_res_len
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
);

oam_return_et
oam_compose_x2ap_delete_enb_res
(
    UInt8  **pp_buffer,
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
);

rrc_length_t
rrc_get_x2ap_enb_link_up_ind_len
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
);

oam_return_et
oam_compose_x2ap_enb_link_up_ind
(
    UInt8  **pp_buffer,
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
);

rrc_length_t
rrc_get_x2ap_enb_link_down_ind_len
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
);

oam_return_et
oam_compose_x2ap_enb_link_down_ind
(
    UInt8  **pp_buffer,
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
);

rrc_length_t
rrc_get_x2ap_enb_config_update_wait_ind_len
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
);

oam_return_et
oam_compose_x2ap_enb_config_update_wait_ind
(
    UInt8  **pp_buffer,
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
);

rrc_length_t
rrc_get_x2ap_ho_report_ind_len
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
);

oam_return_et
oam_compose_x2ap_ho_report_ind
(
    UInt8  **pp_buffer,
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
);

rrc_length_t
rrc_get_x2ap_peer_cell_activation_req_len
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
);

oam_return_et
oam_compose_x2ap_peer_cell_activation_req
(
    UInt8  **pp_buffer,
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
);

rrc_length_t
rrc_get_x2ap_peer_cell_activation_resp_len
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
);

oam_return_et
oam_compose_x2ap_peer_cell_activation_resp
(
    UInt8  **pp_buffer,
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
);

rrc_length_t
rrc_get_x2ap_cell_activation_req_len
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
);

oam_return_et
oam_compose_x2ap_cell_activation_req
(
    UInt8  **pp_buffer,
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
);

rrc_length_t
rrc_get_x2ap_cell_activation_response_len
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
);

oam_return_et
oam_compose_x2ap_cell_activation_response
(
    UInt8  **pp_buffer,
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
);

rrc_length_t
rrc_get_x2ap_mobility_change_req_len
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
);

oam_return_et
oam_compose_x2ap_mobility_change_req
(
    UInt8  **pp_buffer,
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
);

rrc_length_t
rrc_get_x2ap_mobility_change_resp_len
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
);

oam_return_et
oam_compose_x2ap_mobility_change_resp
(
    UInt8  **pp_buffer,
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
);

rrc_length_t
rrc_get_x2ap_mobility_setting_change_req_len
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
);

oam_return_et
oam_compose_x2ap_mobility_setting_change_req
(
    UInt8  **pp_buffer,
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
);

rrc_length_t
rrc_get_x2ap_mobility_setting_change_resp_len
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
);

oam_return_et
oam_compose_x2ap_mobility_setting_change_resp
(
    UInt8  **pp_buffer,
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t  *p_x2ap_enb_config_update_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t  *p_x2ap_enb_config_update_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t  *p_x2ap_enb_config_update_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_reset_req
(
    x2ap_reset_req_t  *p_x2ap_reset_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_reset_resp
(
    x2ap_reset_resp_t  *p_x2ap_reset_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_reset_ind
(
    x2ap_reset_ind_t  *p_x2ap_reset_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_add_enb_req
(
    x2ap_add_enb_req_t  *p_x2ap_add_enb_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_add_enb_res
(
    x2ap_add_enb_res_t  *p_x2ap_add_enb_res,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t  *p_x2ap_delete_enb_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t  *p_x2ap_delete_enb_res,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t  *p_x2ap_enb_link_up_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t  *p_x2ap_enb_link_down_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t  *p_x2ap_enb_config_update_wait_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t  *p_x2ap_ho_report_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t  *p_x2ap_peer_cell_activation_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t  *p_x2ap_peer_cell_activation_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t  *p_x2ap_cell_activation_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t  *p_x2ap_cell_activation_response,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t  *p_x2ap_mobility_setting_change_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_x2apCommon_send_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t  *p_x2ap_mobility_setting_change_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

#endif /* _RRC_X2APCOMMON_IL_COMPOSER_H_ */
