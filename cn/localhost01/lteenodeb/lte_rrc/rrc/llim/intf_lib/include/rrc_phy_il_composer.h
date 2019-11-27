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
 *  File Description : The file rrc_phy_il_composer.h contains the prototypes 
 *                     of RRC-PHY interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_PHY_IL_COMPOSER_H_
#define _RRC_PHY_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_phy_intf.h"

rrc_length_t
rrc_il_get_rrc_phy_config_cell_req_len
(
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req
);

rrc_return_et
rrc_il_compose_rrc_phy_config_cell_req
(
    U8  **pp_buffer,
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req
);

rrc_length_t
rrc_il_get_rrc_phy_config_cell_cnf_len
(
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_reconfig_cell_req_len
(
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req
);

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_req
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req
);

rrc_length_t
rrc_il_get_rrc_phy_reconfig_cell_cnf_len
(
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_delete_cell_req_len
(
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req
);

rrc_return_et
rrc_il_compose_rrc_phy_delete_cell_req
(
    U8  **pp_buffer,
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req
);

rrc_length_t
rrc_il_get_rrc_phy_delete_cell_cnf_len
(
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_delete_cell_cnf
(
    U8  **pp_buffer,
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_create_ue_entity_req_len
(
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_phy_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_phy_create_ue_entity_cnf_len
(
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_delete_ue_entity_req_len
(
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_phy_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_phy_delete_ue_entity_cnf_len
(
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_reconfig_ue_entity_req_len
(
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_ue_entity_req
(
    U8  **pp_buffer,
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_phy_reconfig_ue_entity_cnf_len
(
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_reconfig_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_change_crnti_req_len
(
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req
);

rrc_return_et
rrc_il_compose_rrc_phy_change_crnti_req
(
    U8  **pp_buffer,
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req
);

rrc_length_t
rrc_il_get_rrc_phy_change_crnti_cnf_len
(
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_cell_start_req_len
(
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req
);

rrc_return_et
rrc_il_compose_rrc_phy_cell_start_req
(
    U8  **pp_buffer,
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req
);

rrc_length_t
rrc_il_get_rrc_phy_cell_start_cnf_len
(
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_cell_start_cnf
(
    U8  **pp_buffer,
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf
);

rrc_length_t
rrc_il_get_rrc_phy_cell_stop_req_len
(
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req
);

rrc_return_et
rrc_il_compose_rrc_phy_cell_stop_req
(
    U8  **pp_buffer,
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req
);

rrc_length_t
rrc_il_get_rrc_phy_cell_stop_cnf_len
(
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf
);

rrc_return_et
rrc_il_compose_rrc_phy_cell_stop_cnf
(
    U8  **pp_buffer,
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf
);

rrc_return_et
rrc_phy_il_send_rrc_phy_config_cell_req
(
    rrc_phy_config_cell_req_t  *p_rrc_phy_config_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_config_cell_cnf
(
    rrc_phy_config_cell_cnf_t  *p_rrc_phy_config_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_cell_req
(
    rrc_phy_reconfig_cell_req_t  *p_rrc_phy_reconfig_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_cell_cnf
(
    rrc_phy_reconfig_cell_cnf_t  *p_rrc_phy_reconfig_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_delete_cell_req
(
    rrc_phy_delete_cell_req_t  *p_rrc_phy_delete_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_delete_cell_cnf
(
    rrc_phy_delete_cell_cnf_t  *p_rrc_phy_delete_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_create_ue_entity_req
(
    rrc_phy_create_ue_entity_req_t  *p_rrc_phy_create_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_create_ue_entity_cnf
(
    rrc_phy_create_ue_entity_cnf_t  *p_rrc_phy_create_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_delete_ue_entity_req
(
    rrc_phy_delete_ue_entity_req_t  *p_rrc_phy_delete_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_delete_ue_entity_cnf
(
    rrc_phy_delete_ue_entity_cnf_t  *p_rrc_phy_delete_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_ue_entity_req
(
    rrc_phy_reconfig_ue_entity_req_t  *p_rrc_phy_reconfig_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_reconfig_ue_entity_cnf
(
    rrc_phy_reconfig_ue_entity_cnf_t  *p_rrc_phy_reconfig_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_change_crnti_req
(
    rrc_phy_change_crnti_req_t  *p_rrc_phy_change_crnti_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_change_crnti_cnf
(
    rrc_phy_change_crnti_cnf_t  *p_rrc_phy_change_crnti_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_cell_start_req
(
    rrc_phy_cell_start_req_t  *p_rrc_phy_cell_start_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_cell_start_cnf
(
    rrc_phy_cell_start_cnf_t  *p_rrc_phy_cell_start_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_cell_stop_req
(
    rrc_phy_cell_stop_req_t  *p_rrc_phy_cell_stop_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_phy_il_send_rrc_phy_cell_stop_cnf
(
    rrc_phy_cell_stop_cnf_t  *p_rrc_phy_cell_stop_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_PHY_IL_COMPOSER_H_ */
