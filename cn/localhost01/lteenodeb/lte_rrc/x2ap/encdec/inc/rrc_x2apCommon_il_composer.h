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
 *  File Description : The file rrc_x2apCommon_il_composer.h contains the prototypes 
 *                     of RRC-X2APCOMMON interface message composing functions.
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

rrc_length_t
rrc_get_x2ap_enb_config_update_req_len
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
);

rrc_return_et
rrc_compose_x2ap_enb_config_update_req
(
    U8  **pp_buffer,
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
);

rrc_length_t
rrc_get_x2ap_enb_config_update_resp_len
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
);

rrc_return_et
rrc_compose_x2ap_enb_config_update_resp
(
    U8  **pp_buffer,
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
);

rrc_length_t
rrc_get_x2ap_enb_config_update_ind_len
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
);

rrc_return_et
rrc_compose_x2ap_enb_config_update_ind
(
    U8  **pp_buffer,
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
);

rrc_length_t
rrc_get_x2ap_reset_req_len
(
    x2ap_reset_req_t *p_x2ap_reset_req
);

rrc_return_et
rrc_compose_x2ap_reset_req
(
    U8  **pp_buffer,
    x2ap_reset_req_t *p_x2ap_reset_req
);

rrc_length_t
rrc_get_x2ap_reset_resp_len
(
    x2ap_reset_resp_t *p_x2ap_reset_resp
);

rrc_return_et
rrc_compose_x2ap_reset_resp
(
    U8  **pp_buffer,
    x2ap_reset_resp_t *p_x2ap_reset_resp
);

rrc_length_t
rrc_get_x2ap_reset_ind_len
(
    x2ap_reset_ind_t *p_x2ap_reset_ind
);

rrc_return_et
rrc_compose_x2ap_reset_ind
(
    U8  **pp_buffer,
    x2ap_reset_ind_t *p_x2ap_reset_ind
);

rrc_length_t
rrc_get_x2ap_add_enb_req_len
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
);

rrc_return_et
rrc_compose_x2ap_add_enb_req
(
    U8  **pp_buffer,
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
);

rrc_length_t
rrc_get_x2ap_add_enb_res_len
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
);

rrc_return_et
rrc_compose_x2ap_add_enb_res
(
    U8  **pp_buffer,
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
);

rrc_length_t
rrc_get_x2ap_delete_enb_req_len
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
);

rrc_return_et
rrc_compose_x2ap_delete_enb_req
(
    U8  **pp_buffer,
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
);

rrc_length_t
rrc_get_x2ap_delete_enb_res_len
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
);

rrc_return_et
rrc_compose_x2ap_delete_enb_res
(
    U8  **pp_buffer,
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
);

rrc_length_t
rrc_get_x2ap_enb_link_up_ind_len
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
);

rrc_return_et
rrc_compose_x2ap_enb_link_up_ind
(
    U8  **pp_buffer,
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
);

rrc_length_t
rrc_get_x2ap_enb_link_down_ind_len
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
);

rrc_return_et
rrc_compose_x2ap_enb_link_down_ind
(
    U8  **pp_buffer,
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
);

rrc_length_t
rrc_get_x2ap_enb_config_update_wait_ind_len
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
);

rrc_return_et
rrc_compose_x2ap_enb_config_update_wait_ind
(
    U8  **pp_buffer,
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
);

rrc_length_t
rrc_get_x2ap_ho_report_ind_len
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
);

rrc_return_et
rrc_compose_x2ap_ho_report_ind
(
    U8  **pp_buffer,
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
);

rrc_length_t
rrc_get_x2ap_peer_cell_activation_req_len
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
);

rrc_return_et
rrc_compose_x2ap_peer_cell_activation_req
(
    U8  **pp_buffer,
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
);

rrc_length_t
rrc_get_x2ap_peer_cell_activation_resp_len
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
);

rrc_return_et
rrc_compose_x2ap_peer_cell_activation_resp
(
    U8  **pp_buffer,
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
);

rrc_length_t
rrc_get_x2ap_cell_activation_req_len
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
);

rrc_return_et
rrc_compose_x2ap_cell_activation_req
(
    U8  **pp_buffer,
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
);

rrc_length_t
rrc_get_x2ap_cell_activation_response_len
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
);

rrc_return_et
rrc_compose_x2ap_cell_activation_response
(
    U8  **pp_buffer,
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
);

rrc_length_t
rrc_get_x2ap_mobility_change_req_len
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
);

rrc_return_et
rrc_compose_x2ap_mobility_change_req
(
    U8  **pp_buffer,
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
);

rrc_length_t
rrc_get_x2ap_mobility_change_resp_len
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
);

rrc_return_et
rrc_compose_x2ap_mobility_change_resp
(
    U8  **pp_buffer,
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
);

rrc_length_t
rrc_get_x2ap_mobility_setting_change_req_len
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
);

rrc_return_et
rrc_compose_x2ap_mobility_setting_change_req
(
    U8  **pp_buffer,
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
);

rrc_length_t
rrc_get_x2ap_mobility_setting_change_resp_len
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
);

rrc_return_et
rrc_compose_x2ap_mobility_setting_change_resp
(
    U8  **pp_buffer,
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t  *p_x2ap_enb_config_update_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t  *p_x2ap_enb_config_update_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t  *p_x2ap_enb_config_update_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_reset_req
(
    x2ap_reset_req_t  *p_x2ap_reset_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_reset_resp
(
    x2ap_reset_resp_t  *p_x2ap_reset_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_reset_ind
(
    x2ap_reset_ind_t  *p_x2ap_reset_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_add_enb_req
(
    x2ap_add_enb_req_t  *p_x2ap_add_enb_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_add_enb_res
(
    x2ap_add_enb_res_t  *p_x2ap_add_enb_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t  *p_x2ap_delete_enb_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t  *p_x2ap_delete_enb_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t  *p_x2ap_enb_link_up_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t  *p_x2ap_enb_link_down_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t  *p_x2ap_enb_config_update_wait_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t  *p_x2ap_ho_report_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t  *p_x2ap_peer_cell_activation_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t  *p_x2ap_peer_cell_activation_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t  *p_x2ap_cell_activation_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t  *p_x2ap_cell_activation_response,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t  *p_x2ap_mobility_setting_change_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t  *p_x2ap_mobility_setting_change_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_X2APCOMMON_IL_COMPOSER_H_ */
