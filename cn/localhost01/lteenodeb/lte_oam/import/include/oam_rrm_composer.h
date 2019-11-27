/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrm_composer.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM Composer file used to declare RRM OAM APIs 
 *                   function
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _OAM_RRM_COMPOSER_H_
#define _OAM_RRM_COMPOSER_H_

#include "rrm_defines.h"
#include "rrm_oam_intf.h"
#include "oam_types.h"

rrm_length_t
rrm_get_rrm_oam_init_config_req_len
(
    rrm_oam_init_config_req_t *p_rrm_oam_init_config_req
);

oam_return_et
oam_compose_rrm_oam_init_config_req
(
    U8  **pp_buffer,
    rrm_oam_init_config_req_t *p_rrm_oam_init_config_req
);

rrm_length_t
rrm_get_rrm_oam_init_config_resp_len
(
    rrm_oam_init_config_resp_t *p_rrm_oam_init_config_resp
);

oam_return_et
oam_compose_rrm_oam_init_config_resp
(
    U8  **pp_buffer,
    rrm_oam_init_config_resp_t *p_rrm_oam_init_config_resp
);

rrm_length_t
rrm_get_rrm_oam_set_log_level_req_len
(
    rrm_oam_set_log_level_req_t *p_rrm_oam_set_log_level_req
);

oam_return_et
oam_compose_rrm_oam_set_log_level_req
(
    U8  **pp_buffer,
    rrm_oam_set_log_level_req_t *p_rrm_oam_set_log_level_req
);

rrm_length_t
rrm_get_rrm_oam_set_log_level_resp_len
(
    rrm_oam_set_log_level_resp_t *p_rrm_oam_set_log_level_resp
);

oam_return_et
oam_compose_rrm_oam_set_log_level_resp
(
    U8  **pp_buffer,
    rrm_oam_set_log_level_resp_t *p_rrm_oam_set_log_level_resp
);

rrm_length_t
rrm_get_rrm_oam_log_enable_disable_req_len
(
    rrm_oam_log_enable_disable_req_t *p_rrm_oam_log_enable_disable_req
);

oam_return_et
oam_compose_rrm_oam_log_enable_disable_req
(
    U8  **pp_buffer,
    rrm_oam_log_enable_disable_req_t *p_rrm_oam_log_enable_disable_req
);

rrm_length_t
rrm_get_rrm_oam_log_enable_disable_resp_len
(
    rrm_oam_log_enable_disable_resp_t *p_rrm_oam_log_enable_disable_resp
);

oam_return_et
oam_compose_rrm_oam_log_enable_disable_resp
(
    U8  **pp_buffer,
    rrm_oam_log_enable_disable_resp_t *p_rrm_oam_log_enable_disable_resp
);

rrm_length_t
rrm_get_rrm_oam_shutdown_req_len
(
    rrm_oam_shutdown_req_t *p_rrm_oam_shutdown_req
);

oam_return_et
oam_compose_rrm_oam_shutdown_req
(
    U8  **pp_buffer,
    rrm_oam_shutdown_req_t *p_rrm_oam_shutdown_req
);

rrm_length_t
rrm_get_rrm_oam_shutdown_resp_len
(
    rrm_oam_shutdown_resp_t *p_rrm_oam_shutdown_resp
);

oam_return_et
oam_compose_rrm_oam_shutdown_resp
(
    U8  **pp_buffer,
    rrm_oam_shutdown_resp_t *p_rrm_oam_shutdown_resp
);

rrm_length_t
rrm_get_rrm_oam_resume_service_resp_len
(
    rrm_oam_resume_service_resp_t *p_rrm_oam_resume_service_resp
);

oam_return_et
oam_compose_rrm_oam_resume_service_resp
(
    U8  **pp_buffer,
    rrm_oam_resume_service_resp_t *p_rrm_oam_resume_service_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_start_req_len
(
    rrm_oam_cell_start_req_t *p_rrm_oam_cell_start_req
);

oam_return_et
oam_compose_rrm_oam_cell_start_req
(
    U8  **pp_buffer,
    rrm_oam_cell_start_req_t *p_rrm_oam_cell_start_req
);

rrm_length_t
rrm_get_rrm_oam_cell_start_resp_len
(
    rrm_oam_cell_start_resp_t *p_rrm_oam_cell_start_resp
);

oam_return_et
oam_compose_rrm_oam_cell_start_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_start_resp_t *p_rrm_oam_cell_start_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_stop_req_len
(
    rrm_oam_cell_stop_req_t *p_rrm_oam_cell_stop_req
);

oam_return_et
oam_compose_rrm_oam_cell_stop_req
(
    U8  **pp_buffer,
    rrm_oam_cell_stop_req_t *p_rrm_oam_cell_stop_req
);

rrm_length_t
rrm_get_rrm_oam_cell_stop_resp_len
(
    rrm_oam_cell_stop_resp_t *p_rrm_oam_cell_stop_resp
);

oam_return_et
oam_compose_rrm_oam_cell_stop_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_stop_resp_t *p_rrm_oam_cell_stop_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_delete_req_len
(
    rrm_oam_cell_delete_req_t *p_rrm_oam_cell_delete_req
);

oam_return_et
oam_compose_rrm_oam_cell_delete_req
(
    U8  **pp_buffer,
    rrm_oam_cell_delete_req_t *p_rrm_oam_cell_delete_req
);

rrm_length_t
rrm_get_rrm_oam_delete_resp_len
(
    rrm_oam_delete_resp_t *p_rrm_oam_delete_resp
);

oam_return_et
oam_compose_rrm_oam_delete_resp
(
    U8  **pp_buffer,
    rrm_oam_delete_resp_t *p_rrm_oam_delete_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_config_resp_len
(
    rrm_oam_cell_config_resp_t *p_rrm_oam_cell_config_resp
);

oam_return_et
oam_compose_rrm_oam_cell_config_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_config_resp_t *p_rrm_oam_cell_config_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_reconfig_req_len
(
    rrm_oam_cell_reconfig_req_t *p_rrm_oam_cell_reconfig_req
);

oam_return_et
oam_compose_rrm_oam_cell_reconfig_req
(
    U8  **pp_buffer,
    rrm_oam_cell_reconfig_req_t *p_rrm_oam_cell_reconfig_req
);

rrm_length_t
rrm_get_rrm_oam_cell_reconfig_resp_len
(
    rrm_oam_cell_reconfig_resp_t *p_rrm_oam_cell_reconfig_resp
);

oam_return_et
oam_compose_rrm_oam_cell_reconfig_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_reconfig_resp_t *p_rrm_oam_cell_reconfig_resp
);

rrm_length_t
rrm_get_rrm_oam_rac_enable_disable_req_len
(
    rrm_oam_rac_enable_disable_req_t *p_rrm_oam_rac_enable_disable_req
);

oam_return_et
oam_compose_rrm_oam_rac_enable_disable_req
(
    U8  **pp_buffer,
    rrm_oam_rac_enable_disable_req_t *p_rrm_oam_rac_enable_disable_req
);

rrm_length_t
rrm_get_rrm_oam_rac_enable_disable_resp_len
(
    rrm_oam_rac_enable_disable_resp_t *p_rrm_oam_rac_enable_disable_resp
);

oam_return_et
oam_compose_rrm_oam_rac_enable_disable_resp
(
    U8  **pp_buffer,
    rrm_oam_rac_enable_disable_resp_t *p_rrm_oam_rac_enable_disable_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_context_print_req_len
(
    rrm_oam_cell_context_print_req_t *p_rrm_oam_cell_context_print_req
);

oam_return_et
oam_compose_rrm_oam_cell_context_print_req
(
    U8  **pp_buffer,
    rrm_oam_cell_context_print_req_t *p_rrm_oam_cell_context_print_req
);

rrm_length_t
rrm_get_rrm_oam_ue_release_req_len
(
    rrm_oam_ue_release_req_t *p_rrm_oam_ue_release_req
);

oam_return_et
oam_compose_rrm_oam_ue_release_req
(
    U8  **pp_buffer,
    rrm_oam_ue_release_req_t *p_rrm_oam_ue_release_req
);

rrm_length_t
rrm_get_rrm_oam_cell_block_req_len
(
    rrm_oam_cell_block_req_t *p_rrm_oam_cell_block_req
);

oam_return_et
oam_compose_rrm_oam_cell_block_req
(
    U8  **pp_buffer,
    rrm_oam_cell_block_req_t *p_rrm_oam_cell_block_req
);

rrm_length_t
rrm_get_rrm_oam_cell_block_resp_len
(
    rrm_oam_cell_block_resp_t *p_rrm_oam_cell_block_resp
);

oam_return_et
oam_compose_rrm_oam_cell_block_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_block_resp_t *p_rrm_oam_cell_block_resp
);

rrm_length_t
rrm_get_rrm_oam_cell_unblock_cmd_len
(
    rrm_oam_cell_unblock_cmd_t *p_rrm_oam_cell_unblock_cmd
);

oam_return_et
oam_compose_rrm_oam_cell_unblock_cmd
(
    U8  **pp_buffer,
    rrm_oam_cell_unblock_cmd_t *p_rrm_oam_cell_unblock_cmd
);

rrm_length_t
rrm_get_rrm_oam_ready_for_cell_block_ind_len
(
    rrm_oam_ready_for_cell_block_ind_t *p_rrm_oam_ready_for_cell_block_ind
);

oam_return_et
oam_compose_rrm_oam_ready_for_cell_block_ind
(
    U8  **pp_buffer,
    rrm_oam_ready_for_cell_block_ind_t *p_rrm_oam_ready_for_cell_block_ind
);

rrm_length_t
rrm_get_rrm_oam_cell_update_req_len
(
    rrm_oam_cell_update_req_t *p_rrm_oam_cell_update_req
);

oam_return_et
oam_compose_rrm_oam_cell_update_req
(
    U8  **pp_buffer,
    rrm_oam_cell_update_req_t *p_rrm_oam_cell_update_req
);

rrm_length_t
rrm_get_rrm_oam_cell_update_resp_len
(
    rrm_oam_cell_update_resp_t *p_rrm_oam_cell_update_resp
);

oam_return_et
oam_compose_rrm_oam_cell_update_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_update_resp_t *p_rrm_oam_cell_update_resp
);

rrm_length_t
rrm_get_rrm_oam_get_ver_id_resp_len
(
    rrm_oam_get_ver_id_resp_t *p_rrm_oam_get_ver_id_resp
);

oam_return_et
oam_compose_rrm_oam_get_ver_id_resp
(
    U8  **pp_buffer,
    rrm_oam_get_ver_id_resp_t *p_rrm_oam_get_ver_id_resp
);

rrm_length_t
rrm_get_rrm_oam_event_notification_len
(
    rrm_oam_event_notification_t *p_rrm_oam_event_notification
);

oam_return_et
oam_compose_rrm_oam_event_notification
(
    U8  **pp_buffer,
    rrm_oam_event_notification_t *p_rrm_oam_event_notification
);

rrm_length_t
rrm_get_rrm_oam_load_config_req_len
(
    rrm_oam_load_config_req_t *p_rrm_oam_load_config_req
);

oam_return_et
oam_compose_rrm_oam_load_config_req
(
    U8  **pp_buffer,
    rrm_oam_load_config_req_t *p_rrm_oam_load_config_req
);

rrm_length_t
rrm_get_rrm_oam_load_config_resp_len
(
    rrm_oam_load_config_resp_t *p_rrm_oam_load_config_resp
);

oam_return_et
oam_compose_rrm_oam_load_config_resp
(
    U8  **pp_buffer,
    rrm_oam_load_config_resp_t *p_rrm_oam_load_config_resp
);

rrm_length_t
rrm_get_rrm_oam_load_report_ind_len
(
    rrm_oam_load_report_ind_t *p_rrm_oam_load_report_ind
);

oam_return_et
oam_compose_rrm_oam_load_report_ind
(
    U8  **pp_buffer,
    rrm_oam_load_report_ind_t *p_rrm_oam_load_report_ind
);

rrm_length_t
rrm_get_rrm_oam_cell_ecn_capacity_enhance_req_len
(
    rrm_oam_cell_ecn_capacity_enhance_req_t *p_rrm_oam_cell_ecn_capacity_enhance_req
);

oam_return_et
oam_compose_rrm_oam_cell_ecn_capacity_enhance_req
(
    U8  **pp_buffer,
    rrm_oam_cell_ecn_capacity_enhance_req_t *p_rrm_oam_cell_ecn_capacity_enhance_req
);

rrm_length_t
rrm_get_rrm_oam_cell_ecn_capacity_enhance_resp_len
(
    rrm_oam_cell_ecn_capacity_enhance_resp_t *p_rrm_oam_cell_ecn_capacity_enhance_resp
);

oam_return_et
oam_compose_rrm_oam_cell_ecn_capacity_enhance_resp
(
    U8  **pp_buffer,
    rrm_oam_cell_ecn_capacity_enhance_resp_t *p_rrm_oam_cell_ecn_capacity_enhance_resp
);

rrm_length_t
rrm_get_rrm_oam_event_config_req_len
(
    rrm_oam_event_config_req_t *p_rrm_oam_event_config_req
);

oam_return_et
oam_compose_rrm_oam_event_config_req
(
    U8  **pp_buffer,
    rrm_oam_event_config_req_t *p_rrm_oam_event_config_req
);

rrm_length_t
rrm_get_rrm_oam_event_config_resp_len
(
    rrm_oam_event_config_resp_t *p_rrm_oam_event_config_resp
);

oam_return_et
oam_compose_rrm_oam_event_config_resp
(
    U8  **pp_buffer,
    rrm_oam_event_config_resp_t *p_rrm_oam_event_config_resp
);

rrm_length_t
rrm_get_rrm_oam_kpi_ind_len
(
    rrm_oam_kpi_ind_t *p_rrm_oam_kpi_ind
);

oam_return_et
oam_compose_rrm_oam_kpi_ind
(
    U8  **pp_buffer,
    rrm_oam_kpi_ind_t *p_rrm_oam_kpi_ind
);

rrm_length_t
rrm_get_rrm_oam_config_kpi_req_len
(
    rrm_oam_config_kpi_req_t *p_rrm_oam_config_kpi_req
);

oam_return_et
oam_compose_rrm_oam_config_kpi_req
(
    U8  **pp_buffer,
    rrm_oam_config_kpi_req_t *p_rrm_oam_config_kpi_req
);

rrm_length_t
rrm_get_rrm_oam_config_kpi_resp_len
(
    rrm_oam_config_kpi_resp_t *p_rrm_oam_config_kpi_resp
);

oam_return_et
oam_compose_rrm_oam_config_kpi_resp
(
    U8  **pp_buffer,
    rrm_oam_config_kpi_resp_t *p_rrm_oam_config_kpi_resp
);

rrm_length_t
rrm_get_rrm_oam_get_kpi_req_len
(
    rrm_oam_get_kpi_req_t *p_rrm_oam_get_kpi_req
);

oam_return_et
oam_compose_rrm_oam_get_kpi_req
(
    U8  **pp_buffer,
    rrm_oam_get_kpi_req_t *p_rrm_oam_get_kpi_req
);

rrm_length_t
rrm_get_rrm_oam_get_kpi_resp_len
(
    rrm_oam_get_kpi_resp_t *p_rrm_oam_get_kpi_resp
);

oam_return_et
oam_compose_rrm_oam_get_kpi_resp
(
    U8  **pp_buffer,
    rrm_oam_get_kpi_resp_t *p_rrm_oam_get_kpi_resp
);

rrm_length_t
rrm_get_rrm_oam_chk_health_req_len
(
    rrm_oam_chk_health_req_t *p_rrm_oam_chk_health_req
);

oam_return_et
oam_compose_rrm_oam_chk_health_req
(
    U8  **pp_buffer,
    rrm_oam_chk_health_req_t *p_rrm_oam_chk_health_req
);

rrm_length_t
rrm_get_rrm_oam_chk_health_resp_len
(
    rrm_oam_chk_health_resp_t *p_rrm_oam_chk_health_resp
);

oam_return_et
oam_compose_rrm_oam_chk_health_resp
(
    U8  **pp_buffer,
    rrm_oam_chk_health_resp_t *p_rrm_oam_chk_health_resp
);

rrm_length_t
rrm_get_rrm_oam_enb_config_req_len
(
    rrm_oam_enb_config_req_t *p_rrm_oam_enb_config_req
);

oam_return_et
oam_compose_rrm_oam_enb_config_req
(
    U8  **pp_buffer,
    rrm_oam_enb_config_req_t *p_rrm_oam_enb_config_req
);

rrm_length_t
rrm_get_rrm_oam_enb_config_resp_len
(
    rrm_oam_enb_config_resp_t *p_rrm_oam_enb_config_resp
);

oam_return_et
oam_compose_rrm_oam_enb_config_resp
(
    U8  **pp_buffer,
    rrm_oam_enb_config_resp_t *p_rrm_oam_enb_config_resp
);

rrm_length_t
rrm_get_rrm_oam_get_log_level_req_len
(
    rrm_oam_get_log_level_req_t *p_rrm_oam_get_log_level_req
);

oam_return_et
oam_compose_rrm_oam_get_log_level_req
(
    U8  **pp_buffer,
    rrm_oam_get_log_level_req_t *p_rrm_oam_get_log_level_req
);

rrm_length_t
rrm_get_rrm_oam_get_log_level_resp_len
(
    rrm_oam_get_log_level_resp_t *p_rrm_oam_get_log_level_resp
);

oam_return_et
oam_compose_rrm_oam_get_log_level_resp
(
    U8  **pp_buffer,
    rrm_oam_get_log_level_resp_t *p_rrm_oam_get_log_level_resp
);

rrm_length_t
rrm_get_rrm_oam_get_debug_info_req_len
(
    rrm_oam_get_debug_info_req_t *p_rrm_oam_get_debug_info_req
);

oam_return_et
oam_compose_rrm_oam_get_debug_info_req
(
    U8  **pp_buffer,
    rrm_oam_get_debug_info_req_t *p_rrm_oam_get_debug_info_req
);

rrm_length_t
rrm_get_rrm_oam_get_debug_info_resp_len
(
    rrm_oam_get_debug_info_resp_t *p_rrm_oam_get_debug_info_resp
);

oam_return_et
oam_compose_rrm_oam_get_debug_info_resp
(
    U8  **pp_buffer,
    rrm_oam_get_debug_info_resp_t *p_rrm_oam_get_debug_info_resp
);

oam_return_et
oam_rrm_send_rrm_oam_init_config_req
(
    rrm_oam_init_config_req_t  *p_rrm_oam_init_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_init_config_resp
(
    rrm_oam_init_config_resp_t  *p_rrm_oam_init_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_set_log_level_req
(
    rrm_oam_set_log_level_req_t  *p_rrm_oam_set_log_level_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_set_log_level_resp
(
    rrm_oam_set_log_level_resp_t  *p_rrm_oam_set_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_log_enable_disable_req
(
    rrm_oam_log_enable_disable_req_t  *p_rrm_oam_log_enable_disable_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_log_enable_disable_resp
(
    rrm_oam_log_enable_disable_resp_t  *p_rrm_oam_log_enable_disable_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_shutdown_req
(
    rrm_oam_shutdown_req_t  *p_rrm_oam_shutdown_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_shutdown_resp
(
    rrm_oam_shutdown_resp_t  *p_rrm_oam_shutdown_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_resume_service_resp
(
    rrm_oam_resume_service_resp_t  *p_rrm_oam_resume_service_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_start_req
(
    rrm_oam_cell_start_req_t  *p_rrm_oam_cell_start_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_start_resp
(
    rrm_oam_cell_start_resp_t  *p_rrm_oam_cell_start_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_stop_req
(
    rrm_oam_cell_stop_req_t  *p_rrm_oam_cell_stop_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_stop_resp
(
    rrm_oam_cell_stop_resp_t  *p_rrm_oam_cell_stop_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_delete_req
(
    rrm_oam_cell_delete_req_t  *p_rrm_oam_cell_delete_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_delete_resp
(
    rrm_oam_delete_resp_t  *p_rrm_oam_delete_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_config_resp
(
    rrm_oam_cell_config_resp_t  *p_rrm_oam_cell_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_reconfig_req
(
    rrm_oam_cell_reconfig_req_t  *p_rrm_oam_cell_reconfig_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_reconfig_resp
(
    rrm_oam_cell_reconfig_resp_t  *p_rrm_oam_cell_reconfig_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_rac_enable_disable_req
(
    rrm_oam_rac_enable_disable_req_t  *p_rrm_oam_rac_enable_disable_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_rac_enable_disable_resp
(
    rrm_oam_rac_enable_disable_resp_t  *p_rrm_oam_rac_enable_disable_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_context_print_req
(
    rrm_oam_cell_context_print_req_t  *p_rrm_oam_cell_context_print_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_ue_release_req
(
    rrm_oam_ue_release_req_t  *p_rrm_oam_ue_release_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_block_req
(
    rrm_oam_cell_block_req_t  *p_rrm_oam_cell_block_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_block_resp
(
    rrm_oam_cell_block_resp_t  *p_rrm_oam_cell_block_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_unblock_cmd
(
    rrm_oam_cell_unblock_cmd_t  *p_rrm_oam_cell_unblock_cmd,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_ready_for_cell_block_ind
(
    rrm_oam_ready_for_cell_block_ind_t  *p_rrm_oam_ready_for_cell_block_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_update_req
(
    rrm_oam_cell_update_req_t  *p_rrm_oam_cell_update_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_update_resp
(
    rrm_oam_cell_update_resp_t  *p_rrm_oam_cell_update_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_ver_id_resp
(
    rrm_oam_get_ver_id_resp_t  *p_rrm_oam_get_ver_id_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_event_notification
(
    rrm_oam_event_notification_t  *p_rrm_oam_event_notification,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_load_config_req
(
    rrm_oam_load_config_req_t  *p_rrm_oam_load_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_load_config_resp
(
    rrm_oam_load_config_resp_t  *p_rrm_oam_load_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_load_report_ind
(
    rrm_oam_load_report_ind_t  *p_rrm_oam_load_report_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_ecn_capacity_enhance_req
(
    rrm_oam_cell_ecn_capacity_enhance_req_t  *p_rrm_oam_cell_ecn_capacity_enhance_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_cell_ecn_capacity_enhance_resp
(
    rrm_oam_cell_ecn_capacity_enhance_resp_t  *p_rrm_oam_cell_ecn_capacity_enhance_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_event_config_req
(
    rrm_oam_event_config_req_t  *p_rrm_oam_event_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_event_config_resp
(
    rrm_oam_event_config_resp_t  *p_rrm_oam_event_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_kpi_ind
(
    rrm_oam_kpi_ind_t  *p_rrm_oam_kpi_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_config_kpi_req
(
    rrm_oam_config_kpi_req_t  *p_rrm_oam_config_kpi_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_config_kpi_resp
(
    rrm_oam_config_kpi_resp_t  *p_rrm_oam_config_kpi_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_kpi_req
(
    rrm_oam_get_kpi_req_t  *p_rrm_oam_get_kpi_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_kpi_resp
(
    rrm_oam_get_kpi_resp_t  *p_rrm_oam_get_kpi_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_chk_health_req
(
    rrm_oam_chk_health_req_t  *p_rrm_oam_chk_health_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_chk_health_resp
(
    rrm_oam_chk_health_resp_t  *p_rrm_oam_chk_health_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_enb_config_req
(
    rrm_oam_enb_config_req_t  *p_rrm_oam_enb_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_enb_config_resp
(
    rrm_oam_enb_config_resp_t  *p_rrm_oam_enb_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_log_level_req
(
    rrm_oam_get_log_level_req_t  *p_rrm_oam_get_log_level_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_log_level_resp
(
    rrm_oam_get_log_level_resp_t  *p_rrm_oam_get_log_level_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_debug_info_req
(
    rrm_oam_get_debug_info_req_t  *p_rrm_oam_get_debug_info_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrm_send_rrm_oam_get_debug_info_resp
(
    rrm_oam_get_debug_info_resp_t  *p_rrm_oam_get_debug_info_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

 


rrm_length_t
rrm_get_rrm_oam_cell_config_req_len
(
    rrm_oam_cell_config_req_t *p_rrm_oam_cell_config_req
);

oam_return_et
oam_compose_cell_config_rrm_oam_cell_config_req
(
    U8  **pp_buffer,
    rrm_oam_cell_config_req_t *p_rrm_oam_cell_config_req
);

oam_return_et
oam_rrm_send_rrm_oam_cell_config_req
(
    rrm_oam_cell_config_req_t  *p_rrm_oam_cell_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _OAM_RRM_COMPOSER_H_ */

