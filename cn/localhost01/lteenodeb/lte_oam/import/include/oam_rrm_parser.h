/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrm_parser.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM API parser file to declare the RRM OAM APIs parser 
 *                   function
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_OAM_IL_PARSER_H_
#define _RRM_OAM_IL_PARSER_H_

#include "rrm_defines.h"
#include "rrm_oam_intf.h"

oam_return_et
oam_parse_rrm_oam_init_config_req
(
    rrm_oam_init_config_req_t *p_rrm_oam_init_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_init_config_resp
(
    rrm_oam_init_config_resp_t *p_rrm_oam_init_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_set_log_level_req
(
    rrm_oam_set_log_level_req_t *p_rrm_oam_set_log_level_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_set_log_level_resp
(
    rrm_oam_set_log_level_resp_t *p_rrm_oam_set_log_level_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_log_enable_disable_req
(
    rrm_oam_log_enable_disable_req_t *p_rrm_oam_log_enable_disable_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_log_enable_disable_resp
(
    rrm_oam_log_enable_disable_resp_t *p_rrm_oam_log_enable_disable_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_shutdown_req
(
    rrm_oam_shutdown_req_t *p_rrm_oam_shutdown_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_shutdown_resp
(
    rrm_oam_shutdown_resp_t *p_rrm_oam_shutdown_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_resume_service_resp
(
    rrm_oam_resume_service_resp_t *p_rrm_oam_resume_service_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_start_req
(
    rrm_oam_cell_start_req_t *p_rrm_oam_cell_start_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_start_resp
(
    rrm_oam_cell_start_resp_t *p_rrm_oam_cell_start_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_stop_req
(
    rrm_oam_cell_stop_req_t *p_rrm_oam_cell_stop_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_stop_resp
(
    rrm_oam_cell_stop_resp_t *p_rrm_oam_cell_stop_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_delete_req
(
    rrm_oam_cell_delete_req_t *p_rrm_oam_cell_delete_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_delete_resp
(
    rrm_oam_delete_resp_t *p_rrm_oam_delete_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_config_resp
(
    rrm_oam_cell_config_resp_t *p_rrm_oam_cell_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_reconfig_req
(
    rrm_oam_cell_reconfig_req_t *p_rrm_oam_cell_reconfig_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_reconfig_resp
(
    rrm_oam_cell_reconfig_resp_t *p_rrm_oam_cell_reconfig_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_rac_enable_disable_req
(
    rrm_oam_rac_enable_disable_req_t *p_rrm_oam_rac_enable_disable_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_rac_enable_disable_resp
(
    rrm_oam_rac_enable_disable_resp_t *p_rrm_oam_rac_enable_disable_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_context_print_req
(
    rrm_oam_cell_context_print_req_t *p_rrm_oam_cell_context_print_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_ue_release_req
(
    rrm_oam_ue_release_req_t *p_rrm_oam_ue_release_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_block_req
(
    rrm_oam_cell_block_req_t *p_rrm_oam_cell_block_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_block_resp
(
    rrm_oam_cell_block_resp_t *p_rrm_oam_cell_block_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_unblock_cmd
(
    rrm_oam_cell_unblock_cmd_t *p_rrm_oam_cell_unblock_cmd,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_ready_for_cell_block_ind
(
    rrm_oam_ready_for_cell_block_ind_t *p_rrm_oam_ready_for_cell_block_ind,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_update_req
(
    rrm_oam_cell_update_req_t *p_rrm_oam_cell_update_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_update_resp
(
    rrm_oam_cell_update_resp_t *p_rrm_oam_cell_update_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_ver_id_resp
(
    rrm_oam_get_ver_id_resp_t *p_rrm_oam_get_ver_id_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_event_notification
(
    rrm_oam_event_notification_t *p_rrm_oam_event_notification,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_load_config_req
(
    rrm_oam_load_config_req_t *p_rrm_oam_load_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_load_config_resp
(
    rrm_oam_load_config_resp_t *p_rrm_oam_load_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_load_report_ind
(
    rrm_oam_load_report_ind_t *p_rrm_oam_load_report_ind,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_ecn_capacity_enhance_req
(
    rrm_oam_cell_ecn_capacity_enhance_req_t *p_rrm_oam_cell_ecn_capacity_enhance_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_cell_ecn_capacity_enhance_resp
(
    rrm_oam_cell_ecn_capacity_enhance_resp_t *p_rrm_oam_cell_ecn_capacity_enhance_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_event_config_req
(
    rrm_oam_event_config_req_t *p_rrm_oam_event_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_event_config_resp
(
    rrm_oam_event_config_resp_t *p_rrm_oam_event_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_kpi_ind
(
    rrm_oam_kpi_ind_t *p_rrm_oam_kpi_ind,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_config_kpi_req
(
    rrm_oam_config_kpi_req_t *p_rrm_oam_config_kpi_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_config_kpi_resp
(
    rrm_oam_config_kpi_resp_t *p_rrm_oam_config_kpi_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_kpi_req
(
    rrm_oam_get_kpi_req_t *p_rrm_oam_get_kpi_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_kpi_resp
(
    rrm_oam_get_kpi_resp_t *p_rrm_oam_get_kpi_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_chk_health_req
(
    rrm_oam_chk_health_req_t *p_rrm_oam_chk_health_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_chk_health_resp
(
    rrm_oam_chk_health_resp_t *p_rrm_oam_chk_health_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_enb_config_req
(
    rrm_oam_enb_config_req_t *p_rrm_oam_enb_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_enb_config_resp
(
    rrm_oam_enb_config_resp_t *p_rrm_oam_enb_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_log_level_req
(
    rrm_oam_get_log_level_req_t *p_rrm_oam_get_log_level_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_log_level_resp
(
    rrm_oam_get_log_level_resp_t *p_rrm_oam_get_log_level_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_debug_info_req
(
    rrm_oam_get_debug_info_req_t *p_rrm_oam_get_debug_info_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_get_debug_info_resp
(
    rrm_oam_get_debug_info_resp_t *p_rrm_oam_get_debug_info_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

 

oam_return_et
oam_parse_cell_config_rrm_oam_cell_config_req
(
    rrm_oam_cell_config_req_t *p_rrm_oam_cell_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _RRM_OAM_IL_PARSER_H_ */

