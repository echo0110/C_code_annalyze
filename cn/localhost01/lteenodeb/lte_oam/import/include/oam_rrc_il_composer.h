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
 *  File Description : The file oam_rrc_il_composer.h contains the prototypes 
 *                     of RRC-OAM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _OAM_RRC_IL_COMPOSER_H_
#define _OAM_RRC_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_oam_intf.h"
#include "oam_defines.h"
#ifdef _MSC_VER
#include "pc_test.h"
#else
#include "oam_proto.h"
#endif

rrc_length_t
rrc_il_get_rrc_oam_communication_info_req_len
(
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req
);

oam_return_et
rrc_il_compose_rrc_oam_communication_info_req
(
    UInt8  **pp_buffer,
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req
);

rrc_length_t
rrc_il_get_rrc_oam_communication_info_resp_len
(
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp
);

oam_return_et
rrc_il_compose_rrc_oam_communication_info_resp
(
    UInt8  **pp_buffer,
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp
);

rrc_length_t
rrc_il_get_rrc_oam_provision_req_len
(
    rrc_oam_provision_req_t *p_rrc_oam_provision_req
);

oam_return_et
rrc_il_compose_rrc_oam_provision_req
(
    UInt8  **pp_buffer,
    rrc_oam_provision_req_t *p_rrc_oam_provision_req
);

rrc_length_t
rrc_il_get_rrc_oam_provision_resp_len
(
    rrc_oam_provision_resp_t *p_rrc_oam_provision_resp
);

oam_return_et
rrc_il_compose_rrc_oam_provision_resp
(
    UInt8  **pp_buffer,
    rrc_oam_provision_resp_t *p_rrc_oam_provision_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_log_level_resp_len
(
    rrc_oam_get_log_level_resp_t *p_rrc_oam_get_log_level_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_log_level_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_log_level_resp_t *p_rrc_oam_get_log_level_resp
);

rrc_length_t
rrc_il_get_rrc_oam_set_log_level_req_len
(
    rrc_oam_set_log_level_req_t *p_rrc_oam_set_log_level_req
);

oam_return_et
rrc_il_compose_rrc_oam_set_log_level_req
(
    UInt8  **pp_buffer,
    rrc_oam_set_log_level_req_t *p_rrc_oam_set_log_level_req
);

rrc_length_t
rrc_il_get_rrc_oam_set_log_level_resp_len
(
    rrc_oam_set_log_level_resp_t *p_rrc_oam_set_log_level_resp
);

oam_return_et
rrc_il_compose_rrc_oam_set_log_level_resp
(
    UInt8  **pp_buffer,
    rrc_oam_set_log_level_resp_t *p_rrc_oam_set_log_level_resp
);

rrc_length_t
rrc_il_get_rrc_oam_log_enable_req_len
(
    rrc_oam_log_enable_req_t *p_rrc_oam_log_enable_req
);

oam_return_et
rrc_il_compose_rrc_oam_log_enable_req
(
    UInt8  **pp_buffer,
    rrc_oam_log_enable_req_t *p_rrc_oam_log_enable_req
);

rrc_length_t
rrc_il_get_rrc_oam_log_enable_resp_len
(
    rrc_oam_log_enable_resp_t *p_rrc_oam_log_enable_resp
);

oam_return_et
rrc_il_compose_rrc_oam_log_enable_resp
(
    UInt8  **pp_buffer,
    rrc_oam_log_enable_resp_t *p_rrc_oam_log_enable_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_cell_stats_resp_len
(
    rrc_oam_get_cell_stats_resp_t *p_rrc_oam_get_cell_stats_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_cell_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_stats_resp_t *p_rrc_oam_get_cell_stats_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_cell_stats_req_len
(
    rrc_oam_get_cell_stats_req_t *p_rrc_oam_get_cell_stats_req
);

oam_return_et
rrc_il_compose_rrc_oam_get_cell_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_stats_req_t *p_rrc_oam_get_cell_stats_req
);

rrc_length_t
rrc_il_get_rrc_oam_config_stats_req_len
(
    rrc_oam_config_stats_req_t *p_rrc_oam_config_stats_req
);

oam_return_et
rrc_il_compose_rrc_oam_config_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_config_stats_req_t *p_rrc_oam_config_stats_req
);

rrc_length_t
rrc_il_get_rrc_oam_config_stats_resp_len
(
    rrc_oam_config_stats_resp_t *p_rrc_oam_config_stats_resp
);

oam_return_et
rrc_il_compose_rrc_oam_config_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_config_stats_resp_t *p_rrc_oam_config_stats_resp
);

rrc_length_t
rrc_il_get_rrc_oam_rrc_connection_stats_ind_len
(
    rrc_oam_rrc_connection_stats_ind_t *p_rrc_oam_rrc_connection_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_rrc_connection_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_rrc_connection_stats_ind_t *p_rrc_oam_rrc_connection_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_handover_stats_ind_len
(
    rrc_oam_handover_stats_ind_t *p_rrc_oam_handover_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_handover_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_handover_stats_ind_t *p_rrc_oam_handover_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_erab_stats_ind_len
(
    rrc_oam_erab_stats_ind_t *p_rrc_oam_erab_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_erab_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_erab_stats_ind_t *p_rrc_oam_erab_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_ue_assoc_s1_conn_stats_ind_len
(
    rrc_oam_ue_assoc_s1_conn_stats_ind_t *p_rrc_oam_ue_assoc_s1_conn_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_ue_assoc_s1_conn_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_ue_assoc_s1_conn_stats_ind_t *p_rrc_oam_ue_assoc_s1_conn_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_paging_stats_ind_len
(
    rrc_oam_paging_stats_ind_t *p_rrc_oam_paging_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_paging_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_paging_stats_ind_t *p_rrc_oam_paging_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_irat_mobility_stats_ind_len
(
    rrc_oam_irat_mobility_stats_ind_t *p_rrc_oam_irat_mobility_stats_ind
);

oam_return_et
rrc_il_compose_rrc_oam_irat_mobility_stats_ind
(
    UInt8  **pp_buffer,
    rrc_oam_irat_mobility_stats_ind_t *p_rrc_oam_irat_mobility_stats_ind
);

rrc_length_t
rrc_il_get_rrc_oam_get_cell_status_req_len
(
    rrc_oam_get_cell_status_req_t *p_rrc_oam_get_cell_status_req
);

oam_return_et
rrc_il_compose_rrc_oam_get_cell_status_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_status_req_t *p_rrc_oam_get_cell_status_req
);

rrc_length_t
rrc_il_get_rrc_oam_get_cell_status_resp_len
(
    rrc_oam_get_cell_status_resp_t *p_rrc_oam_get_cell_status_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_cell_status_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_cell_status_resp_t *p_rrc_oam_get_cell_status_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_ue_status_req_len
(
    rrc_oam_get_ue_status_req_t *p_rrc_oam_get_ue_status_req
);

oam_return_et
rrc_il_compose_rrc_oam_get_ue_status_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_ue_status_req_t *p_rrc_oam_get_ue_status_req
);

rrc_length_t
rrc_il_get_rrc_oam_get_ue_status_resp_len
(
    rrc_oam_get_ue_status_resp_t *p_rrc_oam_get_ue_status_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_ue_status_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_ue_status_resp_t *p_rrc_oam_get_ue_status_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_ps_stats_req_len
(
    rrc_oam_get_ps_stats_req_t *p_rrc_oam_get_ps_stats_req
);

oam_return_et
rrc_il_compose_rrc_oam_get_ps_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_ps_stats_req_t *p_rrc_oam_get_ps_stats_req
);

rrc_length_t
rrc_il_get_rrc_oam_get_ps_stats_resp_len
(
    rrc_oam_get_ps_stats_resp_t *p_rrc_oam_get_ps_stats_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_ps_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_ps_stats_resp_t *p_rrc_oam_get_ps_stats_resp
);

rrc_length_t
rrc_il_get_rrc_oam_reset_cell_stats_req_len
(
    rrc_oam_reset_cell_stats_req_t *p_rrc_oam_reset_cell_stats_req
);

oam_return_et
rrc_il_compose_rrc_oam_reset_cell_stats_req
(
    UInt8  **pp_buffer,
    rrc_oam_reset_cell_stats_req_t *p_rrc_oam_reset_cell_stats_req
);

rrc_length_t
rrc_il_get_rrc_oam_reset_cell_stats_resp_len
(
    rrc_oam_reset_cell_stats_resp_t *p_rrc_oam_reset_cell_stats_resp
);

oam_return_et
rrc_il_compose_rrc_oam_reset_cell_stats_resp
(
    UInt8  **pp_buffer,
    rrc_oam_reset_cell_stats_resp_t *p_rrc_oam_reset_cell_stats_resp
);

rrc_length_t
rrc_il_get_rrc_oam_cleanup_resp_len
(
    rrc_oam_cleanup_resp_t *p_rrc_oam_cleanup_resp
);

oam_return_et
rrc_il_compose_rrc_oam_cleanup_resp
(
    UInt8  **pp_buffer,
    rrc_oam_cleanup_resp_t *p_rrc_oam_cleanup_resp
);

rrc_length_t
rrc_il_get_rrc_oam_init_ind_len
(
    rrc_oam_init_ind_t *p_rrc_oam_init_ind
);

oam_return_et
rrc_il_compose_rrc_oam_init_ind
(
    UInt8  **pp_buffer,
    rrc_oam_init_ind_t *p_rrc_oam_init_ind
);

rrc_length_t
rrc_il_get_rrc_oam_cell_traffic_trace_start_len
(
    rrc_oam_cell_traffic_trace_start_t *p_rrc_oam_cell_traffic_trace_start
);

oam_return_et
rrc_il_compose_rrc_oam_cell_traffic_trace_start
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_start_t *p_rrc_oam_cell_traffic_trace_start
);

rrc_length_t
rrc_il_get_rrc_oam_cell_traffic_trace_stop_len
(
    rrc_oam_cell_traffic_trace_stop_t *p_rrc_oam_cell_traffic_trace_stop
);

oam_return_et
rrc_il_compose_rrc_oam_cell_traffic_trace_stop
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_stop_t *p_rrc_oam_cell_traffic_trace_stop
);

rrc_length_t
rrc_il_get_rrc_oam_cell_traffic_trace_start_error_ind_len
(
    rrc_oam_cell_traffic_trace_start_error_ind_t *p_rrc_oam_cell_traffic_trace_start_error_ind
);

oam_return_et
rrc_il_compose_rrc_oam_cell_traffic_trace_start_error_ind
(
    UInt8  **pp_buffer,
    rrc_oam_cell_traffic_trace_start_error_ind_t *p_rrc_oam_cell_traffic_trace_start_error_ind
);

rrc_length_t
rrc_il_get_rrc_oam_trace_file_transfer_start_ind_len
(
    rrc_oam_trace_file_transfer_start_ind_t *p_rrc_oam_trace_file_transfer_start_ind
);

oam_return_et
rrc_il_compose_rrc_oam_trace_file_transfer_start_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_transfer_start_ind_t *p_rrc_oam_trace_file_transfer_start_ind
);

rrc_length_t
rrc_il_get_rrc_oam_trace_file_transfer_complete_ind_len
(
    rrc_oam_trace_file_transfer_complete_ind_t *p_rrc_oam_trace_file_transfer_complete_ind
);

oam_return_et
rrc_il_compose_rrc_oam_trace_file_transfer_complete_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_file_transfer_complete_ind_t *p_rrc_oam_trace_file_transfer_complete_ind
);

rrc_length_t
rrc_il_get_rrc_oam_trace_start_error_ind_len
(
    rrc_oam_trace_start_error_ind_t *p_rrc_oam_trace_start_error_ind
);

oam_return_et
rrc_il_compose_rrc_oam_trace_start_error_ind
(
    UInt8  **pp_buffer,
    rrc_oam_trace_start_error_ind_t *p_rrc_oam_trace_start_error_ind
);

rrc_length_t
rrc_il_get_rrc_oam_add_lgw_req_len
(
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req
);

oam_return_et
rrc_il_compose_rrc_oam_add_lgw_req
(
    UInt8  **pp_buffer,
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req
);

rrc_length_t
rrc_il_get_rrc_oam_add_lgw_resp_len
(
    rrc_oam_add_lgw_resp_t *p_rrc_oam_add_lgw_resp
);

oam_return_et
rrc_il_compose_rrc_oam_add_lgw_resp
(
    UInt8  **pp_buffer,
    rrc_oam_add_lgw_resp_t *p_rrc_oam_add_lgw_resp
);

rrc_length_t
rrc_il_get_rrc_oam_delete_lgw_req_len
(
    rrc_oam_delete_lgw_req_t *p_rrc_oam_delete_lgw_req
);

oam_return_et
rrc_il_compose_rrc_oam_delete_lgw_req
(
    UInt8  **pp_buffer,
    rrc_oam_delete_lgw_req_t *p_rrc_oam_delete_lgw_req
);

rrc_length_t
rrc_il_get_rrc_oam_delete_lgw_resp_len
(
    rrc_oam_delete_lgw_resp_t *p_rrc_oam_delete_lgw_resp
);

oam_return_et
rrc_il_compose_rrc_oam_delete_lgw_resp
(
    UInt8  **pp_buffer,
    rrc_oam_delete_lgw_resp_t *p_rrc_oam_delete_lgw_resp
);

rrc_length_t
rrc_il_get_rrc_oam_get_debug_info_req_len
(
    rrc_oam_get_debug_info_req_t *p_rrc_oam_get_debug_info_req
);

oam_return_et
rrc_il_compose_rrc_oam_get_debug_info_req
(
    UInt8  **pp_buffer,
    rrc_oam_get_debug_info_req_t *p_rrc_oam_get_debug_info_req
);

rrc_length_t
rrc_il_get_rrc_oam_get_debug_info_resp_len
(
    rrc_oam_get_debug_info_resp_t *p_rrc_oam_get_debug_info_resp
);

oam_return_et
rrc_il_compose_rrc_oam_get_debug_info_resp
(
    UInt8  **pp_buffer,
    rrc_oam_get_debug_info_resp_t *p_rrc_oam_get_debug_info_resp
);

oam_return_et
oam_rrc_il_send_rrc_oam_communication_info_req
(
    rrc_oam_communication_info_req_t  *p_rrc_oam_communication_info_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_communication_info_resp
(
    rrc_oam_communication_info_resp_t  *p_rrc_oam_communication_info_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_provision_req
(
    rrc_oam_provision_req_t  *p_rrc_oam_provision_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_provision_resp
(
    rrc_oam_provision_resp_t  *p_rrc_oam_provision_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_log_level_resp
(
    rrc_oam_get_log_level_resp_t  *p_rrc_oam_get_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_set_log_level_req
(
    rrc_oam_set_log_level_req_t  *p_rrc_oam_set_log_level_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_set_log_level_resp
(
    rrc_oam_set_log_level_resp_t  *p_rrc_oam_set_log_level_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_log_enable_req
(
    rrc_oam_log_enable_req_t  *p_rrc_oam_log_enable_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_log_enable_resp
(
    rrc_oam_log_enable_resp_t  *p_rrc_oam_log_enable_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_stats_resp
(
    rrc_oam_get_cell_stats_resp_t  *p_rrc_oam_get_cell_stats_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_stats_req
(
    rrc_oam_get_cell_stats_req_t  *p_rrc_oam_get_cell_stats_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_config_stats_req
(
    rrc_oam_config_stats_req_t  *p_rrc_oam_config_stats_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_config_stats_resp
(
    rrc_oam_config_stats_resp_t  *p_rrc_oam_config_stats_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_rrc_connection_stats_ind
(
    rrc_oam_rrc_connection_stats_ind_t  *p_rrc_oam_rrc_connection_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_handover_stats_ind
(
    rrc_oam_handover_stats_ind_t  *p_rrc_oam_handover_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_erab_stats_ind
(
    rrc_oam_erab_stats_ind_t  *p_rrc_oam_erab_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_ue_assoc_s1_conn_stats_ind
(
    rrc_oam_ue_assoc_s1_conn_stats_ind_t  *p_rrc_oam_ue_assoc_s1_conn_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_paging_stats_ind
(
    rrc_oam_paging_stats_ind_t  *p_rrc_oam_paging_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_irat_mobility_stats_ind
(
    rrc_oam_irat_mobility_stats_ind_t  *p_rrc_oam_irat_mobility_stats_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_status_req
(
    rrc_oam_get_cell_status_req_t  *p_rrc_oam_get_cell_status_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_cell_status_resp
(
    rrc_oam_get_cell_status_resp_t  *p_rrc_oam_get_cell_status_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_ue_status_req
(
    rrc_oam_get_ue_status_req_t  *p_rrc_oam_get_ue_status_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_ue_status_resp
(
    rrc_oam_get_ue_status_resp_t  *p_rrc_oam_get_ue_status_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_ps_stats_req
(
    rrc_oam_get_ps_stats_req_t  *p_rrc_oam_get_ps_stats_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_ps_stats_resp
(
    rrc_oam_get_ps_stats_resp_t  *p_rrc_oam_get_ps_stats_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_reset_cell_stats_req
(
    rrc_oam_reset_cell_stats_req_t  *p_rrc_oam_reset_cell_stats_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_reset_cell_stats_resp
(
    rrc_oam_reset_cell_stats_resp_t  *p_rrc_oam_reset_cell_stats_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_cleanup_resp
(
    rrc_oam_cleanup_resp_t  *p_rrc_oam_cleanup_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_init_ind
(
    rrc_oam_init_ind_t  *p_rrc_oam_init_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_start
(
    rrc_oam_cell_traffic_trace_start_t  *p_rrc_oam_cell_traffic_trace_start,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_stop
(
    rrc_oam_cell_traffic_trace_stop_t  *p_rrc_oam_cell_traffic_trace_stop,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_cell_traffic_trace_start_error_ind
(
    rrc_oam_cell_traffic_trace_start_error_ind_t  *p_rrc_oam_cell_traffic_trace_start_error_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_trace_file_transfer_start_ind
(
    rrc_oam_trace_file_transfer_start_ind_t  *p_rrc_oam_trace_file_transfer_start_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_trace_file_transfer_complete_ind
(
    rrc_oam_trace_file_transfer_complete_ind_t  *p_rrc_oam_trace_file_transfer_complete_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_trace_start_error_ind
(
    rrc_oam_trace_start_error_ind_t  *p_rrc_oam_trace_start_error_ind,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_add_lgw_req
(
    rrc_oam_add_lgw_req_t  *p_rrc_oam_add_lgw_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_add_lgw_resp
(
    rrc_oam_add_lgw_resp_t  *p_rrc_oam_add_lgw_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_delete_lgw_req
(
    rrc_oam_delete_lgw_req_t  *p_rrc_oam_delete_lgw_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_delete_lgw_resp
(
    rrc_oam_delete_lgw_resp_t  *p_rrc_oam_delete_lgw_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_debug_info_req
(
    rrc_oam_get_debug_info_req_t  *p_rrc_oam_get_debug_info_req,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_rrc_il_send_rrc_oam_get_debug_info_resp
(
    rrc_oam_get_debug_info_resp_t  *p_rrc_oam_get_debug_info_resp,
    UInt16                 src_module_id,
    UInt16                 dst_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

#endif /* _OAM_RRC_IL_COMPOSER_H_ */
