/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_son_composer.h $
 *
 *******************************************************************************
 *
 * File Description: File rrm_son_composer.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_SON_IL_COMPOSER_H_
#define _RRM_SON_IL_COMPOSER_H_

#include "rrm_api_types.h"
#include "rrm_son_intf.h"

rrm_length_t
rrm_get_rrm_son_register_req_len
(
    rrm_son_register_req_t *p_rrm_son_register_req
);

rrm_return_et
rrm_compose_rrm_son_register_req
(
    U8  **pp_buffer,
    rrm_son_register_req_t *p_rrm_son_register_req
);

rrm_length_t
rrm_get_rrm_son_deregister_req_len
(
    rrm_son_deregister_req_t *p_rrm_son_deregister_req
);

rrm_return_et
rrm_compose_rrm_son_deregister_req
(
    U8  **pp_buffer,
    rrm_son_deregister_req_t *p_rrm_son_deregister_req
);

rrm_length_t
rrm_get_rrm_son_set_attribute_value_req_len
(
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req
);

rrm_return_et
rrm_compose_rrm_son_set_attribute_value_req
(
    U8  **pp_buffer,
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req
);

rrm_length_t
rrm_get_rrm_son_cell_switch_on_res_len
(
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res
);

rrm_return_et
rrm_compose_rrm_son_cell_switch_on_res
(
    U8  **pp_buffer,
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res
);

rrm_length_t
rrm_get_rrm_son_cell_switch_off_res_len
(
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res
);

rrm_return_et
rrm_compose_rrm_son_cell_switch_off_res
(
    U8  **pp_buffer,
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res
);

rrm_length_t
rrm_get_rrm_son_registration_res_len
(
    rrm_son_registration_res_t *p_rrm_son_registration_res
);

rrm_return_et
rrm_compose_rrm_son_registration_res
(
    U8  **pp_buffer,
    rrm_son_registration_res_t *p_rrm_son_registration_res
);

rrm_length_t
rrm_get_rrm_son_deregistration_res_len
(
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res
);

rrm_return_et
rrm_compose_rrm_son_deregistration_res
(
    U8  **pp_buffer,
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res
);

rrm_length_t
rrm_get_rrm_son_set_attr_res_len
(
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res
);

rrm_return_et
rrm_compose_rrm_son_set_attr_res
(
    U8  **pp_buffer,
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res
);

rrm_length_t
rrm_get_rrm_son_active_ue_cnt_report_len
(
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report
);

rrm_return_et
rrm_compose_rrm_son_active_ue_cnt_report
(
    U8  **pp_buffer,
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report
);

rrm_length_t
rrm_get_rrm_ue_count_threshold_hit_ind_len
(
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind
);

rrm_return_et
rrm_compose_rrm_ue_count_threshold_hit_ind
(
    U8  **pp_buffer,
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind
);

rrm_length_t
rrm_get_rrm_son_cell_switch_off_req_len
(
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req
);

rrm_return_et
rrm_compose_rrm_son_cell_switch_off_req
(
    U8  **pp_buffer,
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req
);

rrm_length_t
rrm_get_rrm_son_cell_switch_on_req_len
(
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req
);

rrm_return_et
rrm_compose_rrm_son_cell_switch_on_req
(
    U8  **pp_buffer,
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req
);

rrm_length_t
rrm_get_rrm_son_cell_state_change_ind_len
(
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind
);

rrm_return_et
rrm_compose_rrm_son_cell_state_change_ind
(
    U8  **pp_buffer,
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind
);

rrm_length_t
rrm_get_rrm_son_nmm_prepare_res_len
(
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res
);

rrm_return_et
rrm_compose_rrm_son_nmm_prepare_res
(
    U8  **pp_buffer,
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res
);

rrm_length_t
rrm_get_rrm_son_nmm_complete_res_len
(
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res
);

rrm_return_et
rrm_compose_rrm_son_nmm_complete_res
(
    U8  **pp_buffer,
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res
);

rrm_length_t
rrm_get_rrm_son_meas_config_req_len
(
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req
);

rrm_return_et
rrm_compose_rrm_son_meas_config_req
(
    U8  **pp_buffer,
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req
);

rrm_length_t
rrm_get_rrm_son_meas_config_res_len
(
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res
);

rrm_return_et
rrm_compose_rrm_son_meas_config_res
(
    U8  **pp_buffer,
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res
);

rrm_length_t
rrm_get_rrm_son_meas_config_obj_remove_req_len
(
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req
);

rrm_return_et
rrm_compose_rrm_son_meas_config_obj_remove_req
(
    U8  **pp_buffer,
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req
);

rrm_length_t
rrm_get_rrm_son_meas_config_obj_remove_res_len
(
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res
);

rrm_return_et
rrm_compose_rrm_son_meas_config_obj_remove_res
(
    U8  **pp_buffer,
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res
);

rrm_length_t
rrm_get_rrm_son_meas_results_ind_len
(
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind
);

rrm_return_et
rrm_compose_rrm_son_meas_results_ind
(
    U8  **pp_buffer,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind
);

rrm_length_t
rrm_get_rrm_son_ho_report_len
(
    rrm_son_ho_report_t *p_rrm_son_ho_report
);

rrm_return_et
rrm_compose_rrm_son_ho_report
(
    U8  **pp_buffer,
    rrm_son_ho_report_t *p_rrm_son_ho_report
);

rrm_length_t
rrm_get_rrm_son_load_report_ind_len
(
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind
);

rrm_return_et
rrm_compose_rrm_son_load_report_ind
(
    U8  **pp_buffer,
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind
);

rrm_length_t
rrm_get_rrm_son_ho_attempt_ind_len
(
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind
);

rrm_return_et
rrm_compose_rrm_son_ho_attempt_ind
(
    U8  **pp_buffer,
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind
);

rrm_length_t
rrm_get_rrm_son_ho_failure_report_len
(
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report
);

rrm_return_et
rrm_compose_rrm_son_ho_failure_report
(
    U8  **pp_buffer,
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report
);

rrm_length_t
rrm_get_rrm_son_tnl_discovery_req_len
(
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req
);

rrm_return_et
rrm_compose_rrm_son_tnl_discovery_req
(
    U8  **pp_buffer,
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req
);

rrm_length_t
rrm_get_rrm_son_tnl_discovery_res_len
(
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res
);

rrm_return_et
rrm_compose_rrm_son_tnl_discovery_res
(
    U8  **pp_buffer,
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res
);

rrm_length_t
rrm_get_rrm_son_rach_config_req_len
(
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req
);

rrm_return_et
rrm_compose_rrm_son_rach_config_req
(
    U8  **pp_buffer,
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req
);

rrm_length_t
rrm_get_rrm_son_rach_config_resp_len
(
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp
);

rrm_return_et
rrm_compose_rrm_son_rach_config_resp
(
    U8  **pp_buffer,
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp
);

rrm_length_t
rrm_get_rrm_son_rach_ue_info_ind_len
(
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind
);

rrm_return_et
rrm_compose_rrm_son_rach_ue_info_ind
(
    U8  **pp_buffer,
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind
);

rrm_length_t
rrm_get_rrm_son_l2_rach_perf_report_len
(
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report
);

rrm_return_et
rrm_compose_rrm_son_l2_rach_perf_report
(
    U8  **pp_buffer,
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report
);

rrm_length_t
rrm_get_rrm_son_enb_config_update_ind_len
(
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind
);

rrm_return_et
rrm_compose_rrm_son_enb_config_update_ind
(
    U8  **pp_buffer,
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind
);

rrm_length_t
rrm_get_rrm_son_eicic_enable_disable_ind_len
(
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind
);

rrm_return_et
rrm_compose_rrm_son_eicic_enable_disable_ind
(
    U8  **pp_buffer,
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind
);

rrm_length_t
rrm_get_rrm_son_mbms_sf_info_ind_len
(
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind
);

rrm_return_et
rrm_compose_rrm_son_mbms_sf_info_ind
(
    U8  **pp_buffer,
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind
);

rrm_return_et
rrm_son_send_rrm_son_register_req
(
    rrm_son_register_req_t  *p_rrm_son_register_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_deregister_req
(
    rrm_son_deregister_req_t  *p_rrm_son_deregister_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_set_attribute_value_req
(
    rrm_son_set_attribute_value_req_t  *p_rrm_son_set_attribute_value_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_cell_switch_on_res
(
    rrm_son_cell_switch_on_res_t  *p_rrm_son_cell_switch_on_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_cell_switch_off_res
(
    rrm_son_cell_switch_off_res_t  *p_rrm_son_cell_switch_off_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_registration_res
(
    rrm_son_registration_res_t  *p_rrm_son_registration_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_deregistration_res
(
    rrm_son_deregistration_res_t  *p_rrm_son_deregistration_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_set_attr_res
(
    rrm_son_set_attr_res_t  *p_rrm_son_set_attr_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_active_ue_cnt_report
(
    rrm_son_active_ue_cnt_report_t  *p_rrm_son_active_ue_cnt_report,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_ue_count_threshold_hit_ind
(
    rrm_ue_count_threshold_hit_ind_t  *p_rrm_ue_count_threshold_hit_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_cell_switch_off_req
(
    rrm_son_cell_switch_off_req_t  *p_rrm_son_cell_switch_off_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_cell_switch_on_req
(
    rrm_son_cell_switch_on_req_t  *p_rrm_son_cell_switch_on_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_cell_state_change_ind
(
    rrm_son_cell_state_change_ind_t  *p_rrm_son_cell_state_change_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_nmm_prepare_res
(
    rrm_son_nmm_prepare_res_t  *p_rrm_son_nmm_prepare_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_nmm_complete_res
(
    rrm_son_nmm_complete_res_t  *p_rrm_son_nmm_complete_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_meas_config_req
(
    rrm_son_meas_config_req_t  *p_rrm_son_meas_config_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_meas_config_res
(
    rrm_son_meas_config_res_t  *p_rrm_son_meas_config_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_meas_config_obj_remove_req
(
    rrm_son_meas_config_obj_remove_req_t  *p_rrm_son_meas_config_obj_remove_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_meas_config_obj_remove_res
(
    rrm_son_meas_config_obj_remove_res_t  *p_rrm_son_meas_config_obj_remove_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_meas_results_ind
(
    rrm_son_meas_results_ind_t  *p_rrm_son_meas_results_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_ho_report
(
    rrm_son_ho_report_t  *p_rrm_son_ho_report,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_load_report_ind
(
    rrm_son_load_report_ind_t  *p_rrm_son_load_report_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_ho_attempt_ind
(
    rrm_son_ho_attempt_ind_t  *p_rrm_son_ho_attempt_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_ho_failure_report
(
    rrm_son_ho_failure_report_t  *p_rrm_son_ho_failure_report,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_tnl_discovery_req
(
    rrm_son_tnl_discovery_req_t  *p_rrm_son_tnl_discovery_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_tnl_discovery_res
(
    rrm_son_tnl_discovery_res_t  *p_rrm_son_tnl_discovery_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_rach_config_req
(
    rrm_son_rach_config_req_t  *p_rrm_son_rach_config_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_rach_config_resp
(
    rrm_son_rach_config_resp_t  *p_rrm_son_rach_config_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_rach_ue_info_ind
(
    rrm_son_rach_ue_info_ind_t  *p_rrm_son_rach_ue_info_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_l2_rach_perf_report
(
    rrm_son_l2_rach_perf_report_t  *p_rrm_son_l2_rach_perf_report,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_enb_config_update_ind
(
    rrm_son_enb_config_update_ind_t  *p_rrm_son_enb_config_update_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_eicic_enable_disable_ind
(
    rrm_son_eicic_enable_disable_ind_t  *p_rrm_son_eicic_enable_disable_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrm_return_et
rrm_son_send_rrm_son_mbms_sf_info_ind
(
    rrm_son_mbms_sf_info_ind_t  *p_rrm_son_mbms_sf_info_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRM_SON_IL_COMPOSER_H_ */
