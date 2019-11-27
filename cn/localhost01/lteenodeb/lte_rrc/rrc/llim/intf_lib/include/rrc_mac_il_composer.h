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
 *  File Description : The file rrc_mac_il_composer.h contains the prototypes 
 *                     of RRC-MAC interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_MAC_IL_COMPOSER_H_
#define _RRC_MAC_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_mac_intf.h"

rrc_length_t
rrc_il_get_rrc_mac_config_cell_req_len
(
    rrc_mac_config_cell_req_t *p_rrc_mac_config_cell_req
);

rrc_return_et
rrc_il_compose_rrc_mac_config_cell_req
(
    U8  **pp_buffer,
    rrc_mac_config_cell_req_t *p_rrc_mac_config_cell_req
);

rrc_length_t
rrc_il_get_rrc_mac_config_cell_cnf_len
(
    rrc_mac_config_cell_cnf_t *p_rrc_mac_config_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_mac_config_cell_cnf_t *p_rrc_mac_config_cell_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_sfn_req_len
(
    rrc_mac_sfn_req_t *p_rrc_mac_sfn_req
);

rrc_return_et
rrc_il_compose_rrc_mac_sfn_req
(
    U8  **pp_buffer,
    rrc_mac_sfn_req_t *p_rrc_mac_sfn_req
);

rrc_length_t
rrc_il_get_rrc_mac_sfn_cnf_len
(
    rrc_mac_sfn_cnf_t *p_rrc_mac_sfn_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_sfn_cnf
(
    U8  **pp_buffer,
    rrc_mac_sfn_cnf_t *p_rrc_mac_sfn_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_sfn_ind_len
(
    rrc_mac_sfn_ind_t *p_rrc_mac_sfn_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_sfn_ind
(
    U8  **pp_buffer,
    rrc_mac_sfn_ind_t *p_rrc_mac_sfn_ind
);

rrc_length_t
rrc_il_get_rrc_mac_reconfig_cell_req_len
(
    rrc_mac_reconfig_cell_req_t *p_rrc_mac_reconfig_cell_req
);

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_cell_req
(
    U8  **pp_buffer,
    rrc_mac_reconfig_cell_req_t *p_rrc_mac_reconfig_cell_req
);

rrc_length_t
rrc_il_get_rrc_mac_reconfig_cell_cnf_len
(
    rrc_mac_reconfig_cell_cnf_t *p_rrc_mac_reconfig_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_cell_cnf
(
    U8  **pp_buffer,
    rrc_mac_reconfig_cell_cnf_t *p_rrc_mac_reconfig_cell_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_create_ue_entity_req_len
(
    rrc_mac_create_ue_entity_req_t *p_rrc_mac_create_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_mac_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_create_ue_entity_req_t *p_rrc_mac_create_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_mac_create_ue_entity_cnf_len
(
    rrc_mac_create_ue_entity_cnf_t *p_rrc_mac_create_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_create_ue_entity_cnf_t *p_rrc_mac_create_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_delete_ue_entity_req_len
(
    rrc_mac_delete_ue_entity_req_t *p_rrc_mac_delete_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_mac_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_delete_ue_entity_req_t *p_rrc_mac_delete_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_mac_delete_ue_entity_cnf_len
(
    rrc_mac_delete_ue_entity_cnf_t *p_rrc_mac_delete_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_delete_ue_entity_cnf_t *p_rrc_mac_delete_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_reconfigure_ue_entity_req_len
(
    rrc_mac_reconfigure_ue_entity_req_t *p_rrc_mac_reconfigure_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_ue_entity_req_t *p_rrc_mac_reconfigure_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_mac_reconfigure_ue_entity_cnf_len
(
    rrc_mac_reconfigure_ue_entity_cnf_t *p_rrc_mac_reconfigure_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_ue_entity_cnf_t *p_rrc_mac_reconfigure_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_ue_entity_power_headroom_ind_len
(
    rrc_mac_ue_entity_power_headroom_ind_t *p_rrc_mac_ue_entity_power_headroom_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_ue_entity_power_headroom_ind
(
    U8  **pp_buffer,
    rrc_mac_ue_entity_power_headroom_ind_t *p_rrc_mac_ue_entity_power_headroom_ind
);

rrc_length_t
rrc_il_get_rrc_mac_ue_drx_cmd_req_len
(
    rrc_mac_ue_drx_cmd_req_t *p_rrc_mac_ue_drx_cmd_req
);

rrc_return_et
rrc_il_compose_rrc_mac_ue_drx_cmd_req
(
    U8  **pp_buffer,
    rrc_mac_ue_drx_cmd_req_t *p_rrc_mac_ue_drx_cmd_req
);

rrc_length_t
rrc_il_get_rrc_mac_ue_con_rej_req_len
(
    rrc_mac_ue_con_rej_req_t *p_rrc_mac_ue_con_rej_req
);

rrc_return_et
rrc_il_compose_rrc_mac_ue_con_rej_req
(
    U8  **pp_buffer,
    rrc_mac_ue_con_rej_req_t *p_rrc_mac_ue_con_rej_req
);

rrc_length_t
rrc_il_get_rrc_mac_bcch_config_req_len
(
    rrc_mac_bcch_config_req_t *p_rrc_mac_bcch_config_req
);

rrc_return_et
rrc_il_compose_rrc_mac_bcch_config_req
(
    U8  **pp_buffer,
    rrc_mac_bcch_config_req_t *p_rrc_mac_bcch_config_req
);

rrc_length_t
rrc_il_get_rrc_mac_pcch_msg_req_len
(
    rrc_mac_pcch_msg_req_t *p_rrc_mac_pcch_msg_req
);

rrc_return_et
rrc_il_compose_rrc_mac_pcch_msg_req
(
    U8  **pp_buffer,
    rrc_mac_pcch_msg_req_t *p_rrc_mac_pcch_msg_req
);

rrc_length_t
rrc_il_get_rrc_mac_ccch_msg_req_len
(
    rrc_mac_ccch_msg_req_t *p_rrc_mac_ccch_msg_req
);

rrc_return_et
rrc_il_compose_rrc_mac_ccch_msg_req
(
    U8  **pp_buffer,
    rrc_mac_ccch_msg_req_t *p_rrc_mac_ccch_msg_req
);

rrc_length_t
rrc_il_get_rrc_mac_ccch_msg_ind_len
(
    rrc_mac_ccch_msg_ind_t *p_rrc_mac_ccch_msg_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_ccch_msg_ind
(
    U8  **pp_buffer,
    rrc_mac_ccch_msg_ind_t *p_rrc_mac_ccch_msg_ind
);

rrc_length_t
rrc_il_get_RrcMacEmtcCcchMsgInd_len
(
    RrcMacEmtcCcchMsgInd_t *p_RrcMacEmtcCcchMsgInd
);

rrc_return_et
rrc_il_compose_RrcMacEmtcCcchMsgInd
(
    U8  **pp_buffer,
    RrcMacEmtcCcchMsgInd_t *p_RrcMacEmtcCcchMsgInd
);

rrc_length_t
rrc_il_get_rrc_mac_ho_rach_resource_req_len
(
    rrc_mac_ho_rach_resource_req_t *p_rrc_mac_ho_rach_resource_req
);

rrc_return_et
rrc_il_compose_rrc_mac_ho_rach_resource_req
(
    U8  **pp_buffer,
    rrc_mac_ho_rach_resource_req_t *p_rrc_mac_ho_rach_resource_req
);

rrc_length_t
rrc_il_get_rrc_mac_ho_rach_resource_resp_len
(
    rrc_mac_ho_rach_resource_resp_t *p_rrc_mac_ho_rach_resource_resp
);

rrc_return_et
rrc_il_compose_rrc_mac_ho_rach_resource_resp
(
    U8  **pp_buffer,
    rrc_mac_ho_rach_resource_resp_t *p_rrc_mac_ho_rach_resource_resp
);

rrc_length_t
rrc_il_get_rrc_mac_ho_rel_rach_resource_ind_len
(
    rrc_mac_ho_rel_rach_resource_ind_t *p_rrc_mac_ho_rel_rach_resource_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_ho_rel_rach_resource_ind
(
    U8  **pp_buffer,
    rrc_mac_ho_rel_rach_resource_ind_t *p_rrc_mac_ho_rel_rach_resource_ind
);

rrc_length_t
rrc_il_get_rrc_mac_reset_ue_entity_req_len
(
    rrc_mac_reset_ue_entity_req_t *p_rrc_mac_reset_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_mac_reset_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_reset_ue_entity_req_t *p_rrc_mac_reset_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_mac_reset_ue_entity_cnf_len
(
    rrc_mac_reset_ue_entity_cnf_t *p_rrc_mac_reset_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_reset_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_reset_ue_entity_cnf_t *p_rrc_mac_reset_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_radio_link_failure_ind_len
(
    rrc_mac_radio_link_failure_ind_t *p_rrc_mac_radio_link_failure_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_radio_link_failure_ind
(
    U8  **pp_buffer,
    rrc_mac_radio_link_failure_ind_t *p_rrc_mac_radio_link_failure_ind
);

rrc_length_t
rrc_il_get_rrc_mac_ue_sync_status_ind_len
(
    rrc_mac_ue_sync_status_ind_t *p_rrc_mac_ue_sync_status_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_ue_sync_status_ind
(
    U8  **pp_buffer,
    rrc_mac_ue_sync_status_ind_t *p_rrc_mac_ue_sync_status_ind
);

rrc_length_t
rrc_il_get_rrc_mac_change_crnti_req_len
(
    rrc_mac_change_crnti_req_t *p_rrc_mac_change_crnti_req
);

rrc_return_et
rrc_il_compose_rrc_mac_change_crnti_req
(
    U8  **pp_buffer,
    rrc_mac_change_crnti_req_t *p_rrc_mac_change_crnti_req
);

rrc_length_t
rrc_il_get_rrc_mac_change_crnti_cnf_len
(
    rrc_mac_change_crnti_cnf_t *p_rrc_mac_change_crnti_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_mac_change_crnti_cnf_t *p_rrc_mac_change_crnti_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_cell_start_cnf_len
(
    rrc_mac_cell_start_cnf_t *p_rrc_mac_cell_start_cnf
);

rrc_return_et
rrc_il_compose_rrc_mac_cell_start_cnf
(
    U8  **pp_buffer,
    rrc_mac_cell_start_cnf_t *p_rrc_mac_cell_start_cnf
);

rrc_length_t
rrc_il_get_rrc_mac_reconfig_complete_ind_len
(
    rrc_mac_reconfig_complete_ind_t *p_rrc_mac_reconfig_complete_ind
);

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_complete_ind
(
    U8  **pp_buffer,
    rrc_mac_reconfig_complete_ind_t *p_rrc_mac_reconfig_complete_ind
);

rrc_return_et
rrc_mac_il_send_rrc_mac_config_cell_req
(
    rrc_mac_config_cell_req_t  *p_rrc_mac_config_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_config_cell_cnf
(
    rrc_mac_config_cell_cnf_t  *p_rrc_mac_config_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_req
(
    rrc_mac_sfn_req_t  *p_rrc_mac_sfn_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_cnf
(
    rrc_mac_sfn_cnf_t  *p_rrc_mac_sfn_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_ind
(
    rrc_mac_sfn_ind_t  *p_rrc_mac_sfn_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_cell_req
(
    rrc_mac_reconfig_cell_req_t  *p_rrc_mac_reconfig_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_cell_cnf
(
    rrc_mac_reconfig_cell_cnf_t  *p_rrc_mac_reconfig_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_create_ue_entity_req
(
    rrc_mac_create_ue_entity_req_t  *p_rrc_mac_create_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_create_ue_entity_cnf
(
    rrc_mac_create_ue_entity_cnf_t  *p_rrc_mac_create_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_delete_ue_entity_req
(
    rrc_mac_delete_ue_entity_req_t  *p_rrc_mac_delete_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_delete_ue_entity_cnf
(
    rrc_mac_delete_ue_entity_cnf_t  *p_rrc_mac_delete_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_req
(
    rrc_mac_reconfigure_ue_entity_req_t  *p_rrc_mac_reconfigure_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_cnf
(
    rrc_mac_reconfigure_ue_entity_cnf_t  *p_rrc_mac_reconfigure_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ue_entity_power_headroom_ind
(
    rrc_mac_ue_entity_power_headroom_ind_t  *p_rrc_mac_ue_entity_power_headroom_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ue_drx_cmd_req
(
    rrc_mac_ue_drx_cmd_req_t  *p_rrc_mac_ue_drx_cmd_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ue_con_rej_req
(
    rrc_mac_ue_con_rej_req_t  *p_rrc_mac_ue_con_rej_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_bcch_config_req
(
    rrc_mac_bcch_config_req_t  *p_rrc_mac_bcch_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rach_resource_req
(
    rrc_mac_ho_rach_resource_req_t  *p_rrc_mac_ho_rach_resource_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rach_resource_resp
(
    rrc_mac_ho_rach_resource_resp_t  *p_rrc_mac_ho_rach_resource_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rel_rach_resource_ind
(
    rrc_mac_ho_rel_rach_resource_ind_t  *p_rrc_mac_ho_rel_rach_resource_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reset_ue_entity_req
(
    rrc_mac_reset_ue_entity_req_t  *p_rrc_mac_reset_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reset_ue_entity_cnf
(
    rrc_mac_reset_ue_entity_cnf_t  *p_rrc_mac_reset_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_radio_link_failure_ind
(
    rrc_mac_radio_link_failure_ind_t  *p_rrc_mac_radio_link_failure_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_ue_sync_status_ind
(
    rrc_mac_ue_sync_status_ind_t  *p_rrc_mac_ue_sync_status_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_change_crnti_req
(
    rrc_mac_change_crnti_req_t  *p_rrc_mac_change_crnti_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_change_crnti_cnf
(
    rrc_mac_change_crnti_cnf_t  *p_rrc_mac_change_crnti_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_cell_start_cnf
(
    rrc_mac_cell_start_cnf_t  *p_rrc_mac_cell_start_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_complete_ind
(
    rrc_mac_reconfig_complete_ind_t  *p_rrc_mac_reconfig_complete_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_MAC_IL_COMPOSER_H_ */
