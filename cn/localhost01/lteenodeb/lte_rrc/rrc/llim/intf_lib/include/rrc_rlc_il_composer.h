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
 *  File Description : The file rrc_rlc_il_composer.h contains the prototypes 
 *                     of RRC-RLC interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_RLC_IL_COMPOSER_H_
#define _RRC_RLC_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_rlc_intf.h"

rrc_length_t
rrc_il_get_rrc_rlc_create_ue_entity_req_len
(
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_create_ue_entity_cnf_len
(
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_reconfig_ue_entity_req_len
(
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_reconfig_ue_entity_cnf_len
(
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_delete_ue_entity_req_len
(
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_delete_ue_entity_cnf_len
(
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_req_len
(
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_cnf_len
(
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf
);

rrc_length_t
rrc_il_get_rlc_ue_entity_error_ind_len
(
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind
);

rrc_return_et
rrc_il_compose_rlc_ue_entity_error_ind
(
    U8  **pp_buffer,
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind
);

rrc_length_t
rrc_il_get_rrc_rlc_change_crnti_req_len
(
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_change_crnti_req
(
    U8  **pp_buffer,
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req
);

rrc_length_t
rrc_il_get_rrc_rlc_change_crnti_cnf_len
(
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_ind_len
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind
);

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_ind
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind
);

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_cnf_len
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_create_mbms_area_entity_req_len
(
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_create_mbms_area_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_create_mbms_area_entity_cnf_len
(
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_create_mbms_area_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_delete_mbms_area_entity_req_len
(
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_delete_mbms_area_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req
);

rrc_length_t
rrc_il_get_rrc_rlc_delete_mbms_area_entity_cnf_len
(
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_delete_mbms_area_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf
);

rrc_length_t
rrc_il_get_rrc_rlc_config_cell_req_len
(
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req
);

rrc_return_et
rrc_il_compose_rrc_rlc_config_cell_req
(
    U8  **pp_buffer,
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req
);

rrc_length_t
rrc_il_get_rrc_rlc_config_cell_cnf_len
(
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf
);

rrc_return_et
rrc_il_compose_rrc_rlc_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_ue_entity_req
(
    rrc_rlc_create_ue_entity_req_t  *p_rrc_rlc_create_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_ue_entity_cnf
(
    rrc_rlc_create_ue_entity_cnf_t  *p_rrc_rlc_create_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_req
(
    rrc_rlc_reconfig_ue_entity_req_t  *p_rrc_rlc_reconfig_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_cnf
(
    rrc_rlc_reconfig_ue_entity_cnf_t  *p_rrc_rlc_reconfig_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_ue_entity_req
(
    rrc_rlc_delete_ue_entity_req_t  *p_rrc_rlc_delete_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_ue_entity_cnf
(
    rrc_rlc_delete_ue_entity_cnf_t  *p_rrc_rlc_delete_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_req
(
    rrc_rlc_re_establish_ue_entity_req_t  *p_rrc_rlc_re_establish_ue_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_cnf
(
    rrc_rlc_re_establish_ue_entity_cnf_t  *p_rrc_rlc_re_establish_ue_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rlc_ue_entity_error_ind
(
    rlc_ue_entity_error_ind_t  *p_rlc_ue_entity_error_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_change_crnti_req
(
    rrc_rlc_change_crnti_req_t  *p_rrc_rlc_change_crnti_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_change_crnti_cnf
(
    rrc_rlc_change_crnti_cnf_t  *p_rrc_rlc_change_crnti_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_ind
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t  *p_rrc_rlc_re_establish_ue_entity_complete_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t  *p_rrc_rlc_re_establish_ue_entity_complete_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_req
(
    rrc_rlc_create_mbms_area_entity_req_t  *p_rrc_rlc_create_mbms_area_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_cnf
(
    rrc_rlc_create_mbms_area_entity_cnf_t  *p_rrc_rlc_create_mbms_area_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_req
(
    rrc_rlc_delete_mbms_area_entity_req_t  *p_rrc_rlc_delete_mbms_area_entity_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_cnf
(
    rrc_rlc_delete_mbms_area_entity_cnf_t  *p_rrc_rlc_delete_mbms_area_entity_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_config_cell_req
(
    rrc_rlc_config_cell_req_t  *p_rrc_rlc_config_cell_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_rlc_il_send_rrc_rlc_config_cell_cnf
(
    rrc_rlc_config_cell_cnf_t  *p_rrc_rlc_config_cell_cnf,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_RLC_IL_COMPOSER_H_ */
