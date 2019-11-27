/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_son_parser.h $
 *
 *******************************************************************************
 *
 * File Description: File rrm_son_parser.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_SON_IL_PARSER_H_
#define _RRM_SON_IL_PARSER_H_

#include "rrm_api_types.h"
#include "rrm_son_intf.h"

rrm_return_et
rrm_parse_rrm_son_register_req
(
    rrm_son_register_req_t *p_rrm_son_register_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_deregister_req
(
    rrm_son_deregister_req_t *p_rrm_son_deregister_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_set_attribute_value_req
(
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_cell_switch_on_res
(
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_cell_switch_off_res
(
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_registration_res
(
    rrm_son_registration_res_t *p_rrm_son_registration_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_deregistration_res
(
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_set_attr_res
(
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_active_ue_cnt_report
(
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_ue_count_threshold_hit_ind
(
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_cell_switch_off_req
(
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_cell_switch_on_req
(
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_cell_state_change_ind
(
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_nmm_prepare_res
(
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_nmm_complete_res
(
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_meas_config_req
(
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_meas_config_res
(
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_meas_config_obj_remove_req
(
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_meas_config_obj_remove_res
(
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_meas_results_ind
(
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_ho_report
(
    rrm_son_ho_report_t *p_rrm_son_ho_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_load_report_ind
(
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_ho_attempt_ind
(
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_ho_failure_report
(
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_tnl_discovery_req
(
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_tnl_discovery_res
(
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_rach_config_req
(
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_rach_config_resp
(
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_rach_ue_info_ind
(
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_l2_rach_perf_report
(
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_enb_config_update_ind
(
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_eicic_enable_disable_ind
(
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_parse_rrm_son_mbms_sf_info_ind
(
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRM_SON_IL_PARSER_H_ */
