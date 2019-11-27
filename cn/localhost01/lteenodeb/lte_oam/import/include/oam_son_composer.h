/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_son_composer.h $
 *
 *******************************************************************************
 *
 * File Description: File oam_son_composer.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifdef OAM_SON_ENABLE
#ifndef _OAM_SON_COMPOSER_H_
#define _OAM_SON_COMPOSER_H_

#include "son_defines.h"
#include "son_oam_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"
#include "oam_types.h"

son_length_t
son_get_son_oam_init_config_req_len
(
    son_oam_init_config_req_t *p_son_oam_init_config_req
);

oam_return_et
son_compose_son_oam_init_config_req
(
    U8  **pp_buffer,
    son_oam_init_config_req_t *p_son_oam_init_config_req
);

son_length_t
son_get_son_oam_get_log_level_req_len
(
    son_oam_get_log_level_req_t *p_son_oam_get_log_level_req
);

oam_return_et
son_compose_son_oam_get_log_level_req
(
    U8  **pp_buffer,
    son_oam_get_log_level_req_t *p_son_oam_get_log_level_req
);

son_length_t
son_get_son_oam_get_log_level_resp_len
(
    son_oam_get_log_level_resp_t *p_son_oam_get_log_level_resp
);

oam_return_et
son_compose_son_oam_get_log_level_resp
(
    U8  **pp_buffer,
    son_oam_get_log_level_resp_t *p_son_oam_get_log_level_resp
);

son_length_t
son_get_son_oam_init_config_resp_len
(
    son_oam_init_config_resp_t *p_son_oam_init_config_resp
);

oam_return_et
son_compose_son_oam_init_config_resp
(
    U8  **pp_buffer,
    son_oam_init_config_resp_t *p_son_oam_init_config_resp
);

son_length_t
son_get_son_oam_shutdown_req_len
(
    son_oam_shutdown_req_t *p_son_oam_shutdown_req
);

oam_return_et
son_compose_son_oam_shutdown_req
(
    U8  **pp_buffer,
    son_oam_shutdown_req_t *p_son_oam_shutdown_req
);

son_length_t
son_get_son_oam_shutdown_resp_len
(
    son_oam_shutdown_resp_t *p_son_oam_shutdown_resp
);

oam_return_et
son_compose_son_oam_shutdown_resp
(
    U8  **pp_buffer,
    son_oam_shutdown_resp_t *p_son_oam_shutdown_resp
);

son_length_t
son_get_son_oam_set_log_level_req_len
(
    son_oam_set_log_level_req_t *p_son_oam_set_log_level_req
);

oam_return_et
son_compose_son_oam_set_log_level_req
(
    U8  **pp_buffer,
    son_oam_set_log_level_req_t *p_son_oam_set_log_level_req
);

son_length_t
son_get_son_oam_set_log_level_resp_len
(
    son_oam_set_log_level_resp_t *p_son_oam_set_log_level_resp
);

oam_return_et
son_compose_son_oam_set_log_level_resp
(
    U8  **pp_buffer,
    son_oam_set_log_level_resp_t *p_son_oam_set_log_level_resp
);

son_length_t
son_get_son_oam_log_enable_disable_req_len
(
    son_oam_log_enable_disable_req_t *p_son_oam_log_enable_disable_req
);

oam_return_et
son_compose_son_oam_log_enable_disable_req
(
    U8  **pp_buffer,
    son_oam_log_enable_disable_req_t *p_son_oam_log_enable_disable_req
);

son_length_t
son_get_son_oam_log_enable_disable_resp_len
(
    son_oam_log_enable_disable_resp_t *p_son_oam_log_enable_disable_resp
);

oam_return_et
son_compose_son_oam_log_enable_disable_resp
(
    U8  **pp_buffer,
    son_oam_log_enable_disable_resp_t *p_son_oam_log_enable_disable_resp
);

son_length_t
son_get_son_oam_cell_info_ind_len
(
    son_oam_cell_info_ind_t *p_son_oam_cell_info_ind
);

oam_return_et
son_compose_son_oam_cell_info_ind
(
    U8  **pp_buffer,
    son_oam_cell_info_ind_t *p_son_oam_cell_info_ind
);

son_length_t
son_get_son_oam_peer_cell_activation_req_len
(
    son_oam_peer_cell_activation_req_t *p_son_oam_peer_cell_activation_req
);

oam_return_et
son_compose_son_oam_peer_cell_activation_req
(
    U8  **pp_buffer,
    son_oam_peer_cell_activation_req_t *p_son_oam_peer_cell_activation_req
);

son_length_t
son_get_son_oam_peer_cell_activation_resp_len
(
    son_oam_peer_cell_activation_resp_t *p_son_oam_peer_cell_activation_resp
);

oam_return_et
son_compose_son_oam_peer_cell_activation_resp
(
    U8  **pp_buffer,
    son_oam_peer_cell_activation_resp_t *p_son_oam_peer_cell_activation_resp
);

son_length_t
son_get_son_oam_nw_scan_req_len
(
    son_oam_nw_scan_req_t *p_son_oam_nw_scan_req
);

oam_return_et
son_compose_son_oam_nw_scan_req
(
    U8  **pp_buffer,
    son_oam_nw_scan_req_t *p_son_oam_nw_scan_req
);

son_length_t
son_get_son_oam_bulk_nw_scan_resp_len
(
    son_oam_bulk_nw_scan_resp_t *p_son_oam_bulk_nw_scan_resp
);

oam_return_et
son_compose_son_oam_bulk_nw_scan_resp
(
    U8  **pp_buffer,
    son_oam_bulk_nw_scan_resp_t *p_son_oam_bulk_nw_scan_resp
);

son_length_t
son_get_son_oam_nr_scan_req_len
(
    son_oam_nr_scan_req_t *p_son_oam_nr_scan_req
);

oam_return_et
son_compose_son_oam_nr_scan_req
(
    U8  **pp_buffer,
    son_oam_nr_scan_req_t *p_son_oam_nr_scan_req
);

son_length_t
son_get_son_oam_nr_scan_resp_len
(
    son_oam_nr_scan_resp_t *p_son_oam_nr_scan_resp
);

oam_return_et
son_compose_son_oam_nr_scan_resp
(
    U8  **pp_buffer,
    son_oam_nr_scan_resp_t *p_son_oam_nr_scan_resp
);

son_length_t
son_get_son_oam_anr_enable_req_len
(
    son_oam_anr_enable_req_t *p_son_oam_anr_enable_req
);

oam_return_et
son_compose_son_oam_anr_enable_req
(
    U8  **pp_buffer,
    son_oam_anr_enable_req_t *p_son_oam_anr_enable_req
);

son_length_t
son_get_son_oam_anr_enable_resp_len
(
    son_oam_anr_enable_resp_t *p_son_oam_anr_enable_resp
);

oam_return_et
son_compose_son_oam_anr_enable_resp
(
    U8  **pp_buffer,
    son_oam_anr_enable_resp_t *p_son_oam_anr_enable_resp
);

son_length_t
son_get_son_oam_anr_disable_req_len
(
    son_oam_anr_disable_req_t *p_son_oam_anr_disable_req
);

oam_return_et
son_compose_son_oam_anr_disable_req
(
    U8  **pp_buffer,
    son_oam_anr_disable_req_t *p_son_oam_anr_disable_req
);

son_length_t
son_get_son_oam_anr_disable_resp_len
(
    son_oam_anr_disable_resp_t *p_son_oam_anr_disable_resp
);

oam_return_et
son_compose_son_oam_anr_disable_resp
(
    U8  **pp_buffer,
    son_oam_anr_disable_resp_t *p_son_oam_anr_disable_resp
);

son_length_t
son_get_son_oam_nr_add_req_len
(
    son_oam_nr_add_req_t *p_son_oam_nr_add_req
);

oam_return_et
son_compose_son_oam_nr_add_req
(
    U8  **pp_buffer,
    son_oam_nr_add_req_t *p_son_oam_nr_add_req
);

son_length_t
son_get_son_oam_nr_add_resp_len
(
    son_oam_nr_add_resp_t *p_son_oam_nr_add_resp
);

oam_return_et
son_compose_son_oam_nr_add_resp
(
    U8  **pp_buffer,
    son_oam_nr_add_resp_t *p_son_oam_nr_add_resp
);

son_length_t
son_get_son_oam_nr_update_req_len
(
    son_oam_nr_update_req_t *p_son_oam_nr_update_req
);

oam_return_et
son_compose_son_oam_nr_update_req
(
    U8  **pp_buffer,
    son_oam_nr_update_req_t *p_son_oam_nr_update_req
);

son_length_t
son_get_son_oam_nr_update_resp_len
(
    son_oam_nr_update_resp_t *p_son_oam_nr_update_resp
);

oam_return_et
son_compose_son_oam_nr_update_resp
(
    U8  **pp_buffer,
    son_oam_nr_update_resp_t *p_son_oam_nr_update_resp
);

son_length_t
son_get_son_oam_nr_delete_req_len
(
    son_oam_nr_delete_req_t *p_son_oam_nr_delete_req
);

oam_return_et
son_compose_son_oam_nr_delete_req
(
    U8  **pp_buffer,
    son_oam_nr_delete_req_t *p_son_oam_nr_delete_req
);

son_length_t
son_get_son_oam_nr_delete_resp_len
(
    son_oam_nr_delete_resp_t *p_son_oam_nr_delete_resp
);

oam_return_et
son_compose_son_oam_nr_delete_resp
(
    U8  **pp_buffer,
    son_oam_nr_delete_resp_t *p_son_oam_nr_delete_resp
);

son_length_t
son_get_son_oam_nr_retrieve_req_len
(
    son_oam_nr_retrieve_req_t *p_son_oam_nr_retrieve_req
);

oam_return_et
son_compose_son_oam_nr_retrieve_req
(
    U8  **pp_buffer,
    son_oam_nr_retrieve_req_t *p_son_oam_nr_retrieve_req
);

son_length_t
son_get_son_oam_nr_retrieve_resp_len
(
    son_oam_nr_retrieve_resp_t *p_son_oam_nr_retrieve_resp
);

oam_return_et
son_compose_son_oam_nr_retrieve_resp
(
    U8  **pp_buffer,
    son_oam_nr_retrieve_resp_t *p_son_oam_nr_retrieve_resp
);

son_length_t
son_get_son_oam_anr_updated_nrt_info_len
(
    son_oam_anr_updated_nrt_info_t *p_son_oam_anr_updated_nrt_info
);

oam_return_et
son_compose_son_oam_anr_updated_nrt_info
(
    U8  **pp_buffer,
    son_oam_anr_updated_nrt_info_t *p_son_oam_anr_updated_nrt_info
);

son_length_t
son_get_son_oam_es_enable_req_len
(
    son_oam_es_enable_req_t *p_son_oam_es_enable_req
);

oam_return_et
son_compose_son_oam_es_enable_req
(
    U8  **pp_buffer,
    son_oam_es_enable_req_t *p_son_oam_es_enable_req
);

son_length_t
son_get_son_oam_es_enable_resp_len
(
    son_oam_es_enable_resp_t *p_son_oam_es_enable_resp
);

oam_return_et
son_compose_son_oam_es_enable_resp
(
    U8  **pp_buffer,
    son_oam_es_enable_resp_t *p_son_oam_es_enable_resp
);

son_length_t
son_get_son_oam_es_disable_req_len
(
    son_oam_es_disable_req_t *p_son_oam_es_disable_req
);

oam_return_et
son_compose_son_oam_es_disable_req
(
    U8  **pp_buffer,
    son_oam_es_disable_req_t *p_son_oam_es_disable_req
);

son_length_t
son_get_son_oam_es_disable_resp_len
(
    son_oam_es_disable_resp_t *p_son_oam_es_disable_resp
);

oam_return_et
son_compose_son_oam_es_disable_resp
(
    U8  **pp_buffer,
    son_oam_es_disable_resp_t *p_son_oam_es_disable_resp
);

son_length_t
son_get_son_oam_es_modify_ue_count_interval_req_len
(
    son_oam_es_modify_ue_count_interval_req_t *p_son_oam_es_modify_ue_count_interval_req
);

oam_return_et
son_compose_son_oam_es_modify_ue_count_interval_req
(
    U8  **pp_buffer,
    son_oam_es_modify_ue_count_interval_req_t *p_son_oam_es_modify_ue_count_interval_req
);

son_length_t
son_get_son_oam_es_modify_ue_count_interval_resp_len
(
    son_oam_es_modify_ue_count_interval_resp_t *p_son_oam_es_modify_ue_count_interval_resp
);

oam_return_et
son_compose_son_oam_es_modify_ue_count_interval_resp
(
    U8  **pp_buffer,
    son_oam_es_modify_ue_count_interval_resp_t *p_son_oam_es_modify_ue_count_interval_resp
);

son_length_t
son_get_son_oam_es_cell_switch_off_req_len
(
    son_oam_es_cell_switch_off_req_t *p_son_oam_es_cell_switch_off_req
);

oam_return_et
son_compose_son_oam_es_cell_switch_off_req
(
    U8  **pp_buffer,
    son_oam_es_cell_switch_off_req_t *p_son_oam_es_cell_switch_off_req
);

son_length_t
son_get_son_oam_es_cell_switch_off_resp_len
(
    son_oam_es_cell_switch_off_resp_t *p_son_oam_es_cell_switch_off_resp
);

oam_return_et
son_compose_son_oam_es_cell_switch_off_resp
(
    U8  **pp_buffer,
    son_oam_es_cell_switch_off_resp_t *p_son_oam_es_cell_switch_off_resp
);

son_length_t
son_get_son_oam_es_switched_off_cell_info_resp_len
(
    son_oam_es_switched_off_cell_info_resp_t *p_son_oam_es_switched_off_cell_info_resp
);

oam_return_et
son_compose_son_oam_es_switched_off_cell_info_resp
(
    U8  **pp_buffer,
    son_oam_es_switched_off_cell_info_resp_t *p_son_oam_es_switched_off_cell_info_resp
);

son_length_t
son_get_son_oam_es_cell_switch_on_req_len
(
    son_oam_es_cell_switch_on_req_t *p_son_oam_es_cell_switch_on_req
);

oam_return_et
son_compose_son_oam_es_cell_switch_on_req
(
    U8  **pp_buffer,
    son_oam_es_cell_switch_on_req_t *p_son_oam_es_cell_switch_on_req
);

son_length_t
son_get_son_oam_es_cell_switch_on_resp_len
(
    son_oam_es_cell_switch_on_resp_t *p_son_oam_es_cell_switch_on_resp
);

oam_return_et
son_compose_son_oam_es_cell_switch_on_resp
(
    U8  **pp_buffer,
    son_oam_es_cell_switch_on_resp_t *p_son_oam_es_cell_switch_on_resp
);

son_length_t
son_get_son_oam_es_active_ue_count_report_len
(
    son_oam_es_active_ue_count_report_t *p_son_oam_es_active_ue_count_report
);

oam_return_et
son_compose_son_oam_es_active_ue_count_report
(
    U8  **pp_buffer,
    son_oam_es_active_ue_count_report_t *p_son_oam_es_active_ue_count_report
);

son_length_t
son_get_son_oam_es_modify_ue_count_threshold_req_len
(
    son_oam_es_modify_ue_count_threshold_req_t *p_son_oam_es_modify_ue_count_threshold_req
);

oam_return_et
son_compose_son_oam_es_modify_ue_count_threshold_req
(
    U8  **pp_buffer,
    son_oam_es_modify_ue_count_threshold_req_t *p_son_oam_es_modify_ue_count_threshold_req
);

son_length_t
son_get_son_oam_es_modify_ue_count_threshold_resp_len
(
    son_oam_es_modify_ue_count_threshold_resp_t *p_son_oam_es_modify_ue_count_threshold_resp
);

oam_return_et
son_compose_son_oam_es_modify_ue_count_threshold_resp
(
    U8  **pp_buffer,
    son_oam_es_modify_ue_count_threshold_resp_t *p_son_oam_es_modify_ue_count_threshold_resp
);

son_length_t
son_get_son_oam_es_ue_count_threshold_hit_ind_len
(
    son_oam_es_ue_count_threshold_hit_ind_t *p_son_oam_es_ue_count_threshold_hit_ind
);

oam_return_et
son_compose_son_oam_es_ue_count_threshold_hit_ind
(
    U8  **pp_buffer,
    son_oam_es_ue_count_threshold_hit_ind_t *p_son_oam_es_ue_count_threshold_hit_ind
);

son_length_t
son_get_son_oam_es_request_to_switch_on_cell_len
(
    son_oam_es_request_to_switch_on_cell_t *p_son_oam_es_request_to_switch_on_cell
);

oam_return_et
son_compose_son_oam_es_request_to_switch_on_cell
(
    U8  **pp_buffer,
    son_oam_es_request_to_switch_on_cell_t *p_son_oam_es_request_to_switch_on_cell
);

son_length_t
son_get_son_oam_periodic_nw_scan_req_len
(
    son_oam_periodic_nw_scan_req_t *p_son_oam_periodic_nw_scan_req
);

oam_return_et
son_compose_son_oam_periodic_nw_scan_req
(
    U8  **pp_buffer,
    son_oam_periodic_nw_scan_req_t *p_son_oam_periodic_nw_scan_req
);

son_length_t
son_get_son_oam_anr_meas_config_req_len
(
    son_oam_anr_meas_config_req_t *p_son_oam_anr_meas_config_req
);

oam_return_et
son_compose_son_oam_anr_meas_config_req
(
    U8  **pp_buffer,
    son_oam_anr_meas_config_req_t *p_son_oam_anr_meas_config_req
);

son_length_t
son_get_son_oam_anr_meas_config_res_len
(
    son_oam_anr_meas_config_res_t *p_son_oam_anr_meas_config_res
);

oam_return_et
son_compose_son_oam_anr_meas_config_res
(
    U8  **pp_buffer,
    son_oam_anr_meas_config_res_t *p_son_oam_anr_meas_config_res
);

son_length_t
son_get_son_oam_anr_modify_ue_count_req_len
(
    son_oam_anr_modify_ue_count_req_t *p_son_oam_anr_modify_ue_count_req
);

oam_return_et
son_compose_son_oam_anr_modify_ue_count_req
(
    U8  **pp_buffer,
    son_oam_anr_modify_ue_count_req_t *p_son_oam_anr_modify_ue_count_req
);

son_length_t
son_get_son_oam_anr_modify_ue_count_resp_len
(
    son_oam_anr_modify_ue_count_resp_t *p_son_oam_anr_modify_ue_count_resp
);

oam_return_et
son_compose_son_oam_anr_modify_ue_count_resp
(
    U8  **pp_buffer,
    son_oam_anr_modify_ue_count_resp_t *p_son_oam_anr_modify_ue_count_resp
);

son_length_t
son_get_son_oam_anr_modify_meas_reporting_interval_req_len
(
    son_oam_anr_modify_meas_reporting_interval_req_t *p_son_oam_anr_modify_meas_reporting_interval_req
);

oam_return_et
son_compose_son_oam_anr_modify_meas_reporting_interval_req
(
    U8  **pp_buffer,
    son_oam_anr_modify_meas_reporting_interval_req_t *p_son_oam_anr_modify_meas_reporting_interval_req
);

son_length_t
son_get_son_oam_anr_modify_meas_reporting_interval_resp_len
(
    son_oam_anr_modify_meas_reporting_interval_resp_t *p_son_oam_anr_modify_meas_reporting_interval_resp
);

oam_return_et
son_compose_son_oam_anr_modify_meas_reporting_interval_resp
(
    U8  **pp_buffer,
    son_oam_anr_modify_meas_reporting_interval_resp_t *p_son_oam_anr_modify_meas_reporting_interval_resp
);

son_length_t
son_get_son_oam_pci_conflict_enable_disable_req_len
(
    son_oam_pci_conflict_enable_disable_req_t *p_son_oam_pci_conflict_enable_disable_req
);

oam_return_et
son_compose_son_oam_pci_conflict_enable_disable_req
(
    U8  **pp_buffer,
    son_oam_pci_conflict_enable_disable_req_t *p_son_oam_pci_conflict_enable_disable_req
);

son_length_t
son_get_son_oam_pci_conflict_enable_disable_resp_len
(
    son_oam_pci_conflict_enable_disable_resp_t *p_son_oam_pci_conflict_enable_disable_resp
);

oam_return_et
son_compose_son_oam_pci_conflict_enable_disable_resp
(
    U8  **pp_buffer,
    son_oam_pci_conflict_enable_disable_resp_t *p_son_oam_pci_conflict_enable_disable_resp
);

son_length_t
son_get_son_oam_pci_selection_req_len
(
    son_oam_pci_selection_req_t *p_son_oam_pci_selection_req
);

oam_return_et
son_compose_son_oam_pci_selection_req
(
    U8  **pp_buffer,
    son_oam_pci_selection_req_t *p_son_oam_pci_selection_req
);

son_length_t
son_get_son_oam_pci_selection_resp_len
(
    son_oam_pci_selection_resp_t *p_son_oam_pci_selection_resp
);

oam_return_et
son_compose_son_oam_pci_selection_resp
(
    U8  **pp_buffer,
    son_oam_pci_selection_resp_t *p_son_oam_pci_selection_resp
);

son_length_t
son_get_son_oam_pci_reselection_req_len
(
    son_oam_pci_reselection_req_t *p_son_oam_pci_reselection_req
);

oam_return_et
son_compose_son_oam_pci_reselection_req
(
    U8  **pp_buffer,
    son_oam_pci_reselection_req_t *p_son_oam_pci_reselection_req
);

son_length_t
son_get_son_oam_pci_reselection_resp_len
(
    son_oam_pci_reselection_resp_t *p_son_oam_pci_reselection_resp
);

oam_return_et
son_compose_son_oam_pci_reselection_resp
(
    U8  **pp_buffer,
    son_oam_pci_reselection_resp_t *p_son_oam_pci_reselection_resp
);

son_length_t
son_get_son_oam_pci_conflict_confusion_ind_len
(
    son_oam_pci_conflict_confusion_ind_t *p_son_oam_pci_conflict_confusion_ind
);

oam_return_et
son_compose_son_oam_pci_conflict_confusion_ind
(
    U8  **pp_buffer,
    son_oam_pci_conflict_confusion_ind_t *p_son_oam_pci_conflict_confusion_ind
);

son_length_t
son_get_son_oam_carrier_freq_selection_req_len
(
    son_oam_carrier_freq_selection_req_t *p_son_oam_carrier_freq_selection_req
);

oam_return_et
son_compose_son_oam_carrier_freq_selection_req
(
    U8  **pp_buffer,
    son_oam_carrier_freq_selection_req_t *p_son_oam_carrier_freq_selection_req
);

son_length_t
son_get_son_oam_carrier_freq_selection_resp_len
(
    son_oam_carrier_freq_selection_resp_t *p_son_oam_carrier_freq_selection_resp
);

oam_return_et
son_compose_son_oam_carrier_freq_selection_resp
(
    U8  **pp_buffer,
    son_oam_carrier_freq_selection_resp_t *p_son_oam_carrier_freq_selection_resp
);

son_length_t
son_get_son_oam_set_nw_scan_config_cmd_len
(
    son_oam_set_nw_scan_config_cmd_t *p_son_oam_set_nw_scan_config_cmd
);

oam_return_et
son_compose_son_oam_set_nw_scan_config_cmd
(
    U8  **pp_buffer,
    son_oam_set_nw_scan_config_cmd_t *p_son_oam_set_nw_scan_config_cmd
);

son_length_t
son_get_son_oam_es_cell_switch_on_off_ind_len
(
    son_oam_es_cell_switch_on_off_ind_t *p_son_oam_es_cell_switch_on_off_ind
);

oam_return_et
son_compose_son_oam_es_cell_switch_on_off_ind
(
    U8  **pp_buffer,
    son_oam_es_cell_switch_on_off_ind_t *p_son_oam_es_cell_switch_on_off_ind
);

son_length_t
son_get_son_oam_nr_delete_from_remove_list_req_len
(
    son_oam_nr_delete_from_remove_list_req_t *p_son_oam_nr_delete_from_remove_list_req
);

oam_return_et
son_compose_son_oam_nr_delete_from_remove_list_req
(
    U8  **pp_buffer,
    son_oam_nr_delete_from_remove_list_req_t *p_son_oam_nr_delete_from_remove_list_req
);

son_length_t
son_get_son_oam_nr_delete_from_remove_list_resp_len
(
    son_oam_nr_delete_from_remove_list_resp_t *p_son_oam_nr_delete_from_remove_list_resp
);

oam_return_et
son_compose_son_oam_nr_delete_from_remove_list_resp
(
    U8  **pp_buffer,
    son_oam_nr_delete_from_remove_list_resp_t *p_son_oam_nr_delete_from_remove_list_resp
);

son_length_t
son_get_son_oam_anr_modify_removal_attributes_req_len
(
    son_oam_anr_modify_removal_attributes_req_t *p_son_oam_anr_modify_removal_attributes_req
);

oam_return_et
son_compose_son_oam_anr_modify_removal_attributes_req
(
    U8  **pp_buffer,
    son_oam_anr_modify_removal_attributes_req_t *p_son_oam_anr_modify_removal_attributes_req
);

son_length_t
son_get_son_oam_anr_modify_removal_attributes_resp_len
(
    son_oam_anr_modify_removal_attributes_resp_t *p_son_oam_anr_modify_removal_attributes_resp
);

oam_return_et
son_compose_son_oam_anr_modify_removal_attributes_resp
(
    U8  **pp_buffer,
    son_oam_anr_modify_removal_attributes_resp_t *p_son_oam_anr_modify_removal_attributes_resp
);

son_length_t
son_get_son_oam_anr_modify_pci_confusion_cfg_req_len
(
    son_oam_anr_modify_pci_confusion_cfg_req_t *p_son_oam_anr_modify_pci_confusion_cfg_req
);

oam_return_et
son_compose_son_oam_anr_modify_pci_confusion_cfg_req
(
    U8  **pp_buffer,
    son_oam_anr_modify_pci_confusion_cfg_req_t *p_son_oam_anr_modify_pci_confusion_cfg_req
);

son_length_t
son_get_son_oam_anr_modify_pci_confusion_cfg_resp_len
(
    son_oam_anr_modify_pci_confusion_cfg_resp_t *p_son_oam_anr_modify_pci_confusion_cfg_resp
);

oam_return_et
son_compose_son_oam_anr_modify_pci_confusion_cfg_resp
(
    U8  **pp_buffer,
    son_oam_anr_modify_pci_confusion_cfg_resp_t *p_son_oam_anr_modify_pci_confusion_cfg_resp
);

son_length_t
son_get_son_oam_anr_modify_attribute_req_len
(
    son_oam_anr_modify_attribute_req_t *p_son_oam_anr_modify_attribute_req
);

oam_return_et
son_compose_son_oam_anr_modify_attribute_req
(
    U8  **pp_buffer,
    son_oam_anr_modify_attribute_req_t *p_son_oam_anr_modify_attribute_req
);

son_length_t
son_get_son_oam_anr_modify_attribute_resp_len
(
    son_oam_anr_modify_attribute_resp_t *p_son_oam_anr_modify_attribute_resp
);

oam_return_et
son_compose_son_oam_anr_modify_attribute_resp
(
    U8  **pp_buffer,
    son_oam_anr_modify_attribute_resp_t *p_son_oam_anr_modify_attribute_resp
);

son_length_t
son_get_son_oam_enable_disable_x2_req_len
(
    son_oam_enable_disable_x2_req_t *p_son_oam_enable_disable_x2_req
);

oam_return_et
son_compose_son_oam_enable_disable_x2_req
(
    U8  **pp_buffer,
    son_oam_enable_disable_x2_req_t *p_son_oam_enable_disable_x2_req
);

son_length_t
son_get_son_oam_enable_disable_x2_resp_len
(
    son_oam_enable_disable_x2_resp_t *p_son_oam_enable_disable_x2_resp
);

oam_return_et
son_compose_son_oam_enable_disable_x2_resp
(
    U8  **pp_buffer,
    son_oam_enable_disable_x2_resp_t *p_son_oam_enable_disable_x2_resp
);

son_length_t
son_get_son_oam_x2_link_up_ind_len
(
    son_oam_x2_link_up_ind_t *p_son_oam_x2_link_up_ind
);

oam_return_et
son_compose_son_oam_x2_link_up_ind
(
    U8  **pp_buffer,
    son_oam_x2_link_up_ind_t *p_son_oam_x2_link_up_ind
);

son_length_t
son_get_son_oam_x2_link_down_ind_len
(
    son_oam_x2_link_down_ind_t *p_son_oam_x2_link_down_ind
);

oam_return_et
son_compose_son_oam_x2_link_down_ind
(
    U8  **pp_buffer,
    son_oam_x2_link_down_ind_t *p_son_oam_x2_link_down_ind
);

son_length_t
son_get_son_oam_anr_enable_disable_limited_mode_req_len
(
    son_oam_anr_enable_disable_limited_mode_req_t *p_son_oam_anr_enable_disable_limited_mode_req
);

oam_return_et
son_compose_son_oam_anr_enable_disable_limited_mode_req
(
    U8  **pp_buffer,
    son_oam_anr_enable_disable_limited_mode_req_t *p_son_oam_anr_enable_disable_limited_mode_req
);

son_length_t
son_get_son_oam_anr_enable_disable_limited_mode_resp_len
(
    son_oam_anr_enable_disable_limited_mode_resp_t *p_son_oam_anr_enable_disable_limited_mode_resp
);

oam_return_et
son_compose_son_oam_anr_enable_disable_limited_mode_resp
(
    U8  **pp_buffer,
    son_oam_anr_enable_disable_limited_mode_resp_t *p_son_oam_anr_enable_disable_limited_mode_resp
);

son_length_t
son_get_son_oam_anr_limited_mode_pci_info_len
(
    son_oam_anr_limited_mode_pci_info_t *p_son_oam_anr_limited_mode_pci_info
);

oam_return_et
son_compose_son_oam_anr_limited_mode_pci_info
(
    U8  **pp_buffer,
    son_oam_anr_limited_mode_pci_info_t *p_son_oam_anr_limited_mode_pci_info
);

son_length_t
son_get_son_oam_nr_enb_add_req_len
(
    son_oam_nr_enb_add_req_t *p_son_oam_nr_enb_add_req
);

oam_return_et
son_compose_son_oam_nr_enb_add_req
(
    U8  **pp_buffer,
    son_oam_nr_enb_add_req_t *p_son_oam_nr_enb_add_req
);

son_length_t
son_get_son_oam_nr_enb_add_resp_len
(
    son_oam_nr_enb_add_resp_t *p_son_oam_nr_enb_add_resp
);

oam_return_et
son_compose_son_oam_nr_enb_add_resp
(
    U8  **pp_buffer,
    son_oam_nr_enb_add_resp_t *p_son_oam_nr_enb_add_resp
);

son_length_t
son_get_son_oam_nr_enb_update_req_len
(
    son_oam_nr_enb_update_req_t *p_son_oam_nr_enb_update_req
);

oam_return_et
son_compose_son_oam_nr_enb_update_req
(
    U8  **pp_buffer,
    son_oam_nr_enb_update_req_t *p_son_oam_nr_enb_update_req
);

son_length_t
son_get_son_oam_nr_enb_update_resp_len
(
    son_oam_nr_enb_update_resp_t *p_son_oam_nr_enb_update_resp
);

oam_return_et
son_compose_son_oam_nr_enb_update_resp
(
    U8  **pp_buffer,
    son_oam_nr_enb_update_resp_t *p_son_oam_nr_enb_update_resp
);

son_length_t
son_get_son_oam_nr_enb_delete_req_len
(
    son_oam_nr_enb_delete_req_t *p_son_oam_nr_enb_delete_req
);

oam_return_et
son_compose_son_oam_nr_enb_delete_req
(
    U8  **pp_buffer,
    son_oam_nr_enb_delete_req_t *p_son_oam_nr_enb_delete_req
);

son_length_t
son_get_son_oam_nr_enb_delete_resp_len
(
    son_oam_nr_enb_delete_resp_t *p_son_oam_nr_enb_delete_resp
);

oam_return_et
son_compose_son_oam_nr_enb_delete_resp
(
    U8  **pp_buffer,
    son_oam_nr_enb_delete_resp_t *p_son_oam_nr_enb_delete_resp
);

son_length_t
son_get_son_oam_nr_enb_retrieve_req_len
(
    son_oam_nr_enb_retrieve_req_t *p_son_oam_nr_enb_retrieve_req
);

oam_return_et
son_compose_son_oam_nr_enb_retrieve_req
(
    U8  **pp_buffer,
    son_oam_nr_enb_retrieve_req_t *p_son_oam_nr_enb_retrieve_req
);

son_length_t
son_get_son_oam_nr_enb_retrieve_resp_len
(
    son_oam_nr_enb_retrieve_resp_t *p_son_oam_nr_enb_retrieve_resp
);

oam_return_et
son_compose_son_oam_nr_enb_retrieve_resp
(
    U8  **pp_buffer,
    son_oam_nr_enb_retrieve_resp_t *p_son_oam_nr_enb_retrieve_resp
);

son_length_t
son_get_son_oam_proc_sup_resp_len
(
    son_oam_proc_sup_resp_t *p_son_oam_proc_sup_resp
);

oam_return_et
son_compose_son_oam_proc_sup_resp
(
    U8  **pp_buffer,
    son_oam_proc_sup_resp_t *p_son_oam_proc_sup_resp
);

son_length_t
son_get_son_oam_feature_state_change_ind_len
(
    son_oam_feature_state_change_ind_t *p_son_oam_feature_state_change_ind
);

oam_return_et
son_compose_son_oam_feature_state_change_ind
(
    U8  **pp_buffer,
    son_oam_feature_state_change_ind_t *p_son_oam_feature_state_change_ind
);

son_length_t
son_get_son_oam_mro_enable_req_len
(
    son_oam_mro_enable_req_t *p_son_oam_mro_enable_req
);

oam_return_et
son_compose_son_oam_mro_enable_req
(
    U8  **pp_buffer,
    son_oam_mro_enable_req_t *p_son_oam_mro_enable_req
);

son_length_t
son_get_son_oam_mro_enable_resp_len
(
    son_oam_mro_enable_resp_t *p_son_oam_mro_enable_resp
);

oam_return_et
son_compose_son_oam_mro_enable_resp
(
    U8  **pp_buffer,
    son_oam_mro_enable_resp_t *p_son_oam_mro_enable_resp
);

son_length_t
son_get_son_oam_mro_disable_req_len
(
    son_oam_mro_disable_req_t *p_son_oam_mro_disable_req
);

oam_return_et
son_compose_son_oam_mro_disable_req
(
    U8  **pp_buffer,
    son_oam_mro_disable_req_t *p_son_oam_mro_disable_req
);

son_length_t
son_get_son_oam_mro_disable_resp_len
(
    son_oam_mro_disable_resp_t *p_son_oam_mro_disable_resp
);

oam_return_et
son_compose_son_oam_mro_disable_resp
(
    U8  **pp_buffer,
    son_oam_mro_disable_resp_t *p_son_oam_mro_disable_resp
);

son_length_t
son_get_son_oam_mro_modify_config_params_req_len
(
    son_oam_mro_modify_config_params_req_t *p_son_oam_mro_modify_config_params_req
);

oam_return_et
son_compose_son_oam_mro_modify_config_params_req
(
    U8  **pp_buffer,
    son_oam_mro_modify_config_params_req_t *p_son_oam_mro_modify_config_params_req
);

son_length_t
son_get_son_oam_mro_modify_config_params_resp_len
(
    son_oam_mro_modify_config_params_resp_t *p_son_oam_mro_modify_config_params_resp
);

oam_return_et
son_compose_son_oam_mro_modify_config_params_resp
(
    U8  **pp_buffer,
    son_oam_mro_modify_config_params_resp_t *p_son_oam_mro_modify_config_params_resp
);

son_length_t
son_get_son_oam_mro_eutran_ttt_update_ind_len
(
    son_oam_mro_eutran_ttt_update_ind_t *p_son_oam_mro_eutran_ttt_update_ind
);

oam_return_et
son_compose_son_oam_mro_eutran_ttt_update_ind
(
    U8  **pp_buffer,
    son_oam_mro_eutran_ttt_update_ind_t *p_son_oam_mro_eutran_ttt_update_ind
);

son_length_t
son_get_son_oam_mro_ho_counter_report_len
(
    son_oam_mro_ho_counter_report_t *p_son_oam_mro_ho_counter_report
);

oam_return_et
son_compose_son_oam_mro_ho_counter_report
(
    U8  **pp_buffer,
    son_oam_mro_ho_counter_report_t *p_son_oam_mro_ho_counter_report
);

son_length_t
son_get_son_oam_mlb_enable_req_len
(
    son_oam_mlb_enable_req_t *p_son_oam_mlb_enable_req
);

oam_return_et
son_compose_son_oam_mlb_enable_req
(
    U8  **pp_buffer,
    son_oam_mlb_enable_req_t *p_son_oam_mlb_enable_req
);

son_length_t
son_get_son_oam_mlb_enable_resp_len
(
    son_oam_mlb_enable_resp_t *p_son_oam_mlb_enable_resp
);

oam_return_et
son_compose_son_oam_mlb_enable_resp
(
    U8  **pp_buffer,
    son_oam_mlb_enable_resp_t *p_son_oam_mlb_enable_resp
);

son_length_t
son_get_son_oam_mlb_disable_req_len
(
    son_oam_mlb_disable_req_t *p_son_oam_mlb_disable_req
);

oam_return_et
son_compose_son_oam_mlb_disable_req
(
    U8  **pp_buffer,
    son_oam_mlb_disable_req_t *p_son_oam_mlb_disable_req
);

son_length_t
son_get_son_oam_mlb_disable_resp_len
(
    son_oam_mlb_disable_resp_t *p_son_oam_mlb_disable_resp
);

oam_return_et
son_compose_son_oam_mlb_disable_resp
(
    U8  **pp_buffer,
    son_oam_mlb_disable_resp_t *p_son_oam_mlb_disable_resp
);

son_length_t
son_get_son_oam_mlb_modify_attributes_req_len
(
    son_oam_mlb_modify_attributes_req_t *p_son_oam_mlb_modify_attributes_req
);

oam_return_et
son_compose_son_oam_mlb_modify_attributes_req
(
    U8  **pp_buffer,
    son_oam_mlb_modify_attributes_req_t *p_son_oam_mlb_modify_attributes_req
);

son_length_t
son_get_son_oam_mlb_modify_attributes_resp_len
(
    son_oam_mlb_modify_attributes_resp_t *p_son_oam_mlb_modify_attributes_resp
);

oam_return_et
son_compose_son_oam_mlb_modify_attributes_resp
(
    U8  **pp_buffer,
    son_oam_mlb_modify_attributes_resp_t *p_son_oam_mlb_modify_attributes_resp
);

son_length_t
son_get_son_oam_event_notification_len
(
    son_oam_event_notification_t *p_son_oam_event_notification
);

oam_return_et
son_compose_son_oam_event_notification
(
    U8  **pp_buffer,
    son_oam_event_notification_t *p_son_oam_event_notification
);

son_length_t
son_get_son_oam_anr_gu_group_id_config_req_len
(
    son_oam_anr_gu_group_id_config_req_t *p_son_oam_anr_gu_group_id_config_req
);

oam_return_et
son_compose_son_oam_anr_gu_group_id_config_req
(
    U8  **pp_buffer,
    son_oam_anr_gu_group_id_config_req_t *p_son_oam_anr_gu_group_id_config_req
);

son_length_t
son_get_son_oam_anr_gu_group_id_config_resp_len
(
    son_oam_anr_gu_group_id_config_resp_t *p_son_oam_anr_gu_group_id_config_resp
);

oam_return_et
son_compose_son_oam_anr_gu_group_id_config_resp
(
    U8  **pp_buffer,
    son_oam_anr_gu_group_id_config_resp_t *p_son_oam_anr_gu_group_id_config_resp
);

son_length_t
son_get_son_oam_anr_neighbor_default_config_req_len
(
    son_oam_anr_neighbor_default_config_req_t *p_son_oam_anr_neighbor_default_config_req
);

oam_return_et
son_compose_son_oam_anr_neighbor_default_config_req
(
    U8  **pp_buffer,
    son_oam_anr_neighbor_default_config_req_t *p_son_oam_anr_neighbor_default_config_req
);

son_length_t
son_get_son_oam_anr_neighbor_default_config_resp_len
(
    son_oam_anr_neighbor_default_config_resp_t *p_son_oam_anr_neighbor_default_config_resp
);

oam_return_et
son_compose_son_oam_anr_neighbor_default_config_resp
(
    U8  **pp_buffer,
    son_oam_anr_neighbor_default_config_resp_t *p_son_oam_anr_neighbor_default_config_resp
);

son_length_t
son_get_son_oam_carrier_freq_and_dl_tx_power_req_len
(
    son_oam_carrier_freq_and_dl_tx_power_req_t *p_son_oam_carrier_freq_and_dl_tx_power_req
);

oam_return_et
son_compose_son_oam_carrier_freq_and_dl_tx_power_req
(
    U8  **pp_buffer,
    son_oam_carrier_freq_and_dl_tx_power_req_t *p_son_oam_carrier_freq_and_dl_tx_power_req
);

son_length_t
son_get_son_oam_carrier_freq_and_dl_tx_power_resp_len
(
    son_oam_carrier_freq_and_dl_tx_power_resp_t *p_son_oam_carrier_freq_and_dl_tx_power_resp
);

oam_return_et
son_compose_son_oam_carrier_freq_and_dl_tx_power_resp
(
    U8  **pp_buffer,
    son_oam_carrier_freq_and_dl_tx_power_resp_t *p_son_oam_carrier_freq_and_dl_tx_power_resp
);

son_length_t
son_get_son_oam_last_scan_timestamp_ind_len
(
    son_oam_last_scan_timestamp_ind_t *p_son_oam_last_scan_timestamp_ind
);

oam_return_et
son_compose_son_oam_last_scan_timestamp_ind
(
    U8  **pp_buffer,
    son_oam_last_scan_timestamp_ind_t *p_son_oam_last_scan_timestamp_ind
);

son_length_t
son_get_son_oam_mlb_params_info_len
(
    son_oam_mlb_params_info_t *p_son_oam_mlb_params_info
);

oam_return_et
son_compose_son_oam_mlb_params_info
(
    U8  **pp_buffer,
    son_oam_mlb_params_info_t *p_son_oam_mlb_params_info
);

son_length_t
son_get_son_oam_mlb_offset_change_req_len
(
    son_oam_mlb_offset_change_req_t *p_son_oam_mlb_offset_change_req
);

oam_return_et
son_compose_son_oam_mlb_offset_change_req
(
    U8  **pp_buffer,
    son_oam_mlb_offset_change_req_t *p_son_oam_mlb_offset_change_req
);

son_length_t
son_get_son_oam_mlb_offset_change_resp_len
(
    son_oam_mlb_offset_change_resp_t *p_son_oam_mlb_offset_change_resp
);

oam_return_et
son_compose_son_oam_mlb_offset_change_resp
(
    U8  **pp_buffer,
    son_oam_mlb_offset_change_resp_t *p_son_oam_mlb_offset_change_resp
);

son_length_t
son_get_son_oam_mlb_peer_offset_change_req_len
(
    son_oam_mlb_peer_offset_change_req_t *p_son_oam_mlb_peer_offset_change_req
);

oam_return_et
son_compose_son_oam_mlb_peer_offset_change_req
(
    U8  **pp_buffer,
    son_oam_mlb_peer_offset_change_req_t *p_son_oam_mlb_peer_offset_change_req
);

son_length_t
son_get_son_oam_mlb_peer_offset_change_resp_len
(
    son_oam_mlb_peer_offset_change_resp_t *p_son_oam_mlb_peer_offset_change_resp
);

oam_return_et
son_compose_son_oam_mlb_peer_offset_change_resp
(
    U8  **pp_buffer,
    son_oam_mlb_peer_offset_change_resp_t *p_son_oam_mlb_peer_offset_change_resp
);

son_length_t
son_get_son_oam_mro_optimization_notification_len
(
    son_oam_mro_optimization_notification_t *p_son_oam_mro_optimization_notification
);

oam_return_et
son_compose_son_oam_mro_optimization_notification
(
    U8  **pp_buffer,
    son_oam_mro_optimization_notification_t *p_son_oam_mro_optimization_notification
);

son_length_t
son_get_son_oam_es_autonomous_switch_off_config_req_len
(
    son_oam_es_autonomous_switch_off_config_req_t *p_son_oam_es_autonomous_switch_off_config_req
);

oam_return_et
son_compose_son_oam_es_autonomous_switch_off_config_req
(
    U8  **pp_buffer,
    son_oam_es_autonomous_switch_off_config_req_t *p_son_oam_es_autonomous_switch_off_config_req
);

son_length_t
son_get_son_oam_es_autonomous_switch_off_config_resp_len
(
    son_oam_es_autonomous_switch_off_config_resp_t *p_son_oam_es_autonomous_switch_off_config_resp
);

oam_return_et
son_compose_son_oam_es_autonomous_switch_off_config_resp
(
    U8  **pp_buffer,
    son_oam_es_autonomous_switch_off_config_resp_t *p_son_oam_es_autonomous_switch_off_config_resp
);

son_length_t
son_get_son_oam_chk_health_req_len
(
    son_oam_chk_health_req_t *p_son_oam_chk_health_req
);

oam_return_et
son_compose_son_oam_chk_health_req
(
    U8  **pp_buffer,
    son_oam_chk_health_req_t *p_son_oam_chk_health_req
);

son_length_t
son_get_son_oam_chk_health_resp_len
(
    son_oam_chk_health_resp_t *p_son_oam_chk_health_resp
);

oam_return_et
son_compose_son_oam_chk_health_resp
(
    U8  **pp_buffer,
    son_oam_chk_health_resp_t *p_son_oam_chk_health_resp
);

son_length_t
son_get_son_oam_anr_meas_config_obj_remove_req_len
(
    son_oam_anr_meas_config_obj_remove_req_t *p_son_oam_anr_meas_config_obj_remove_req
);

oam_return_et
son_compose_son_oam_anr_meas_config_obj_remove_req
(
    U8  **pp_buffer,
    son_oam_anr_meas_config_obj_remove_req_t *p_son_oam_anr_meas_config_obj_remove_req
);

son_length_t
son_get_son_oam_anr_meas_config_obj_remove_res_len
(
    son_oam_anr_meas_config_obj_remove_res_t *p_son_oam_anr_meas_config_obj_remove_res
);

oam_return_et
son_compose_son_oam_anr_meas_config_obj_remove_res
(
    U8  **pp_buffer,
    son_oam_anr_meas_config_obj_remove_res_t *p_son_oam_anr_meas_config_obj_remove_res
);

son_length_t
son_get_son_oam_rach_opt_enable_req_len
(
    son_oam_rach_opt_enable_req_t *p_son_oam_rach_opt_enable_req
);

oam_return_et
son_compose_son_oam_rach_opt_enable_req
(
    U8  **pp_buffer,
    son_oam_rach_opt_enable_req_t *p_son_oam_rach_opt_enable_req
);

son_length_t
son_get_son_oam_rach_opt_enable_resp_len
(
    son_oam_rach_opt_enable_resp_t *p_son_oam_rach_opt_enable_resp
);

oam_return_et
son_compose_son_oam_rach_opt_enable_resp
(
    U8  **pp_buffer,
    son_oam_rach_opt_enable_resp_t *p_son_oam_rach_opt_enable_resp
);

son_length_t
son_get_son_oam_rach_opt_disable_req_len
(
    son_oam_rach_opt_disable_req_t *p_son_oam_rach_opt_disable_req
);

oam_return_et
son_compose_son_oam_rach_opt_disable_req
(
    U8  **pp_buffer,
    son_oam_rach_opt_disable_req_t *p_son_oam_rach_opt_disable_req
);

son_length_t
son_get_son_oam_rach_opt_disable_resp_len
(
    son_oam_rach_opt_disable_resp_t *p_son_oam_rach_opt_disable_resp
);

oam_return_et
son_compose_son_oam_rach_opt_disable_resp
(
    U8  **pp_buffer,
    son_oam_rach_opt_disable_resp_t *p_son_oam_rach_opt_disable_resp
);

son_length_t
son_get_son_oam_rach_opt_modify_config_params_req_len
(
    son_oam_rach_opt_modify_config_params_req_t *p_son_oam_rach_opt_modify_config_params_req
);

oam_return_et
son_compose_son_oam_rach_opt_modify_config_params_req
(
    U8  **pp_buffer,
    son_oam_rach_opt_modify_config_params_req_t *p_son_oam_rach_opt_modify_config_params_req
);

son_length_t
son_get_son_oam_rach_opt_modify_config_params_resp_len
(
    son_oam_rach_opt_modify_config_params_resp_t *p_son_oam_rach_opt_modify_config_params_resp
);

oam_return_et
son_compose_son_oam_rach_opt_modify_config_params_resp
(
    U8  **pp_buffer,
    son_oam_rach_opt_modify_config_params_resp_t *p_son_oam_rach_opt_modify_config_params_resp
);

son_length_t
son_get_son_oam_rach_opt_modify_rach_optmode_req_len
(
    son_oam_rach_opt_modify_rach_optmode_req_t *p_son_oam_rach_opt_modify_rach_optmode_req
);

oam_return_et
son_compose_son_oam_rach_opt_modify_rach_optmode_req
(
    U8  **pp_buffer,
    son_oam_rach_opt_modify_rach_optmode_req_t *p_son_oam_rach_opt_modify_rach_optmode_req
);

son_length_t
son_get_son_oam_rach_opt_modify_rach_optmode_resp_len
(
    son_oam_rach_opt_modify_rach_optmode_resp_t *p_son_oam_rach_opt_modify_rach_optmode_resp
);

oam_return_et
son_compose_son_oam_rach_opt_modify_rach_optmode_resp
(
    U8  **pp_buffer,
    son_oam_rach_opt_modify_rach_optmode_resp_t *p_son_oam_rach_opt_modify_rach_optmode_resp
);

son_length_t
son_get_son_oam_rach_opt_failure_notification_len
(
    son_oam_rach_opt_failure_notification_t *p_son_oam_rach_opt_failure_notification
);

oam_return_et
son_compose_son_oam_rach_opt_failure_notification
(
    U8  **pp_buffer,
    son_oam_rach_opt_failure_notification_t *p_son_oam_rach_opt_failure_notification
);

son_length_t
son_get_son_oam_rach_opt_parameter_update_req_len
(
    son_oam_rach_opt_parameter_update_req_t *p_son_oam_rach_opt_parameter_update_req
);

oam_return_et
son_compose_son_oam_rach_opt_parameter_update_req
(
    U8  **pp_buffer,
    son_oam_rach_opt_parameter_update_req_t *p_son_oam_rach_opt_parameter_update_req
);

son_length_t
son_get_son_oam_rach_opt_ue_info_report_len
(
    son_oam_rach_opt_ue_info_report_t *p_son_oam_rach_opt_ue_info_report
);

oam_return_et
son_compose_son_oam_rach_opt_ue_info_report
(
    U8  **pp_buffer,
    son_oam_rach_opt_ue_info_report_t *p_son_oam_rach_opt_ue_info_report
);

son_length_t
son_get_son_oam_rach_opt_l2_perf_report_len
(
    son_oam_rach_opt_l2_perf_report_t *p_son_oam_rach_opt_l2_perf_report
);

oam_return_et
son_compose_son_oam_rach_opt_l2_perf_report
(
    U8  **pp_buffer,
    son_oam_rach_opt_l2_perf_report_t *p_son_oam_rach_opt_l2_perf_report
);

son_length_t
son_get_son_oam_cell_delete_req_len
(
    son_oam_cell_delete_req_t *p_son_oam_cell_delete_req
);

oam_return_et
son_compose_son_oam_cell_delete_req
(
    U8  **pp_buffer,
    son_oam_cell_delete_req_t *p_son_oam_cell_delete_req
);

son_length_t
son_get_son_oam_cell_delete_resp_len
(
    son_oam_cell_delete_resp_t *p_son_oam_cell_delete_resp
);

oam_return_et
son_compose_son_oam_cell_delete_resp
(
    U8  **pp_buffer,
    son_oam_cell_delete_resp_t *p_son_oam_cell_delete_resp
);

son_length_t
son_get_son_oam_get_debug_info_req_len
(
    son_oam_get_debug_info_req_t *p_son_oam_get_debug_info_req
);

oam_return_et
son_compose_son_oam_get_debug_info_req
(
    U8  **pp_buffer,
    son_oam_get_debug_info_req_t *p_son_oam_get_debug_info_req
);

son_length_t
son_get_son_oam_get_debug_info_resp_len
(
    son_oam_get_debug_info_resp_t *p_son_oam_get_debug_info_resp
);

oam_return_et
son_compose_son_oam_get_debug_info_resp
(
    U8  **pp_buffer,
    son_oam_get_debug_info_resp_t *p_son_oam_get_debug_info_resp
);

oam_return_et
oam_son_send_son_oam_init_config_req
(
    son_oam_init_config_req_t  *p_son_oam_init_config_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_get_log_level_req
(
    son_oam_get_log_level_req_t  *p_son_oam_get_log_level_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_get_log_level_resp
(
    son_oam_get_log_level_resp_t  *p_son_oam_get_log_level_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_init_config_resp
(
    son_oam_init_config_resp_t  *p_son_oam_init_config_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_shutdown_req
(
    son_oam_shutdown_req_t  *p_son_oam_shutdown_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_shutdown_resp
(
    son_oam_shutdown_resp_t  *p_son_oam_shutdown_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_set_log_level_req
(
    son_oam_set_log_level_req_t  *p_son_oam_set_log_level_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_set_log_level_resp
(
    son_oam_set_log_level_resp_t  *p_son_oam_set_log_level_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_log_enable_disable_req
(
    son_oam_log_enable_disable_req_t  *p_son_oam_log_enable_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_log_enable_disable_resp
(
    son_oam_log_enable_disable_resp_t  *p_son_oam_log_enable_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_cell_info_ind
(
    son_oam_cell_info_ind_t  *p_son_oam_cell_info_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_peer_cell_activation_req
(
    son_oam_peer_cell_activation_req_t  *p_son_oam_peer_cell_activation_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_peer_cell_activation_resp
(
    son_oam_peer_cell_activation_resp_t  *p_son_oam_peer_cell_activation_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nw_scan_req
(
    son_oam_nw_scan_req_t  *p_son_oam_nw_scan_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_bulk_nw_scan_resp
(
    son_oam_bulk_nw_scan_resp_t  *p_son_oam_bulk_nw_scan_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_scan_req
(
    son_oam_nr_scan_req_t  *p_son_oam_nr_scan_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_scan_resp
(
    son_oam_nr_scan_resp_t  *p_son_oam_nr_scan_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_enable_req
(
    son_oam_anr_enable_req_t  *p_son_oam_anr_enable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_enable_resp
(
    son_oam_anr_enable_resp_t  *p_son_oam_anr_enable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_disable_req
(
    son_oam_anr_disable_req_t  *p_son_oam_anr_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_disable_resp
(
    son_oam_anr_disable_resp_t  *p_son_oam_anr_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_add_req
(
    son_oam_nr_add_req_t  *p_son_oam_nr_add_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_add_resp
(
    son_oam_nr_add_resp_t  *p_son_oam_nr_add_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_update_req
(
    son_oam_nr_update_req_t  *p_son_oam_nr_update_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_update_resp
(
    son_oam_nr_update_resp_t  *p_son_oam_nr_update_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_delete_req
(
    son_oam_nr_delete_req_t  *p_son_oam_nr_delete_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_delete_resp
(
    son_oam_nr_delete_resp_t  *p_son_oam_nr_delete_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_retrieve_req
(
    son_oam_nr_retrieve_req_t  *p_son_oam_nr_retrieve_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_retrieve_resp
(
    son_oam_nr_retrieve_resp_t  *p_son_oam_nr_retrieve_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_updated_nrt_info
(
    son_oam_anr_updated_nrt_info_t  *p_son_oam_anr_updated_nrt_info,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_enable_req
(
    son_oam_es_enable_req_t  *p_son_oam_es_enable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_enable_resp
(
    son_oam_es_enable_resp_t  *p_son_oam_es_enable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_disable_req
(
    son_oam_es_disable_req_t  *p_son_oam_es_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_disable_resp
(
    son_oam_es_disable_resp_t  *p_son_oam_es_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_modify_ue_count_interval_req
(
    son_oam_es_modify_ue_count_interval_req_t  *p_son_oam_es_modify_ue_count_interval_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_modify_ue_count_interval_resp
(
    son_oam_es_modify_ue_count_interval_resp_t  *p_son_oam_es_modify_ue_count_interval_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_cell_switch_off_req
(
    son_oam_es_cell_switch_off_req_t  *p_son_oam_es_cell_switch_off_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_cell_switch_off_resp
(
    son_oam_es_cell_switch_off_resp_t  *p_son_oam_es_cell_switch_off_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_switched_off_cell_info_resp
(
    son_oam_es_switched_off_cell_info_resp_t  *p_son_oam_es_switched_off_cell_info_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_cell_switch_on_req
(
    son_oam_es_cell_switch_on_req_t  *p_son_oam_es_cell_switch_on_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_cell_switch_on_resp
(
    son_oam_es_cell_switch_on_resp_t  *p_son_oam_es_cell_switch_on_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_active_ue_count_report
(
    son_oam_es_active_ue_count_report_t  *p_son_oam_es_active_ue_count_report,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_modify_ue_count_threshold_req
(
    son_oam_es_modify_ue_count_threshold_req_t  *p_son_oam_es_modify_ue_count_threshold_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_modify_ue_count_threshold_resp
(
    son_oam_es_modify_ue_count_threshold_resp_t  *p_son_oam_es_modify_ue_count_threshold_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_ue_count_threshold_hit_ind
(
    son_oam_es_ue_count_threshold_hit_ind_t  *p_son_oam_es_ue_count_threshold_hit_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_request_to_switch_on_cell
(
    son_oam_es_request_to_switch_on_cell_t  *p_son_oam_es_request_to_switch_on_cell,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_periodic_nw_scan_req
(
    son_oam_periodic_nw_scan_req_t  *p_son_oam_periodic_nw_scan_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_meas_config_req
(
    son_oam_anr_meas_config_req_t  *p_son_oam_anr_meas_config_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_meas_config_res
(
    son_oam_anr_meas_config_res_t  *p_son_oam_anr_meas_config_res,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_ue_count_req
(
    son_oam_anr_modify_ue_count_req_t  *p_son_oam_anr_modify_ue_count_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_ue_count_resp
(
    son_oam_anr_modify_ue_count_resp_t  *p_son_oam_anr_modify_ue_count_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_meas_reporting_interval_req
(
    son_oam_anr_modify_meas_reporting_interval_req_t  *p_son_oam_anr_modify_meas_reporting_interval_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_meas_reporting_interval_resp
(
    son_oam_anr_modify_meas_reporting_interval_resp_t  *p_son_oam_anr_modify_meas_reporting_interval_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_conflict_enable_disable_req
(
    son_oam_pci_conflict_enable_disable_req_t  *p_son_oam_pci_conflict_enable_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_conflict_enable_disable_resp
(
    son_oam_pci_conflict_enable_disable_resp_t  *p_son_oam_pci_conflict_enable_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_selection_req
(
    son_oam_pci_selection_req_t  *p_son_oam_pci_selection_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_selection_resp
(
    son_oam_pci_selection_resp_t  *p_son_oam_pci_selection_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_reselection_req
(
    son_oam_pci_reselection_req_t  *p_son_oam_pci_reselection_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_reselection_resp
(
    son_oam_pci_reselection_resp_t  *p_son_oam_pci_reselection_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_pci_conflict_confusion_ind
(
    son_oam_pci_conflict_confusion_ind_t  *p_son_oam_pci_conflict_confusion_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_carrier_freq_selection_req
(
    son_oam_carrier_freq_selection_req_t  *p_son_oam_carrier_freq_selection_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_carrier_freq_selection_resp
(
    son_oam_carrier_freq_selection_resp_t  *p_son_oam_carrier_freq_selection_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_set_nw_scan_config_cmd
(
    son_oam_set_nw_scan_config_cmd_t  *p_son_oam_set_nw_scan_config_cmd,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_cell_switch_on_off_ind
(
    son_oam_es_cell_switch_on_off_ind_t  *p_son_oam_es_cell_switch_on_off_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_delete_from_remove_list_req
(
    son_oam_nr_delete_from_remove_list_req_t  *p_son_oam_nr_delete_from_remove_list_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_delete_from_remove_list_resp
(
    son_oam_nr_delete_from_remove_list_resp_t  *p_son_oam_nr_delete_from_remove_list_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_removal_attributes_req
(
    son_oam_anr_modify_removal_attributes_req_t  *p_son_oam_anr_modify_removal_attributes_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_removal_attributes_resp
(
    son_oam_anr_modify_removal_attributes_resp_t  *p_son_oam_anr_modify_removal_attributes_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_pci_confusion_cfg_req
(
    son_oam_anr_modify_pci_confusion_cfg_req_t  *p_son_oam_anr_modify_pci_confusion_cfg_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_pci_confusion_cfg_resp
(
    son_oam_anr_modify_pci_confusion_cfg_resp_t  *p_son_oam_anr_modify_pci_confusion_cfg_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_attribute_req
(
    son_oam_anr_modify_attribute_req_t  *p_son_oam_anr_modify_attribute_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_modify_attribute_resp
(
    son_oam_anr_modify_attribute_resp_t  *p_son_oam_anr_modify_attribute_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_enable_disable_x2_req
(
    son_oam_enable_disable_x2_req_t  *p_son_oam_enable_disable_x2_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_enable_disable_x2_resp
(
    son_oam_enable_disable_x2_resp_t  *p_son_oam_enable_disable_x2_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_x2_link_up_ind
(
    son_oam_x2_link_up_ind_t  *p_son_oam_x2_link_up_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_x2_link_down_ind
(
    son_oam_x2_link_down_ind_t  *p_son_oam_x2_link_down_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_enable_disable_limited_mode_req
(
    son_oam_anr_enable_disable_limited_mode_req_t  *p_son_oam_anr_enable_disable_limited_mode_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_enable_disable_limited_mode_resp
(
    son_oam_anr_enable_disable_limited_mode_resp_t  *p_son_oam_anr_enable_disable_limited_mode_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_limited_mode_pci_info
(
    son_oam_anr_limited_mode_pci_info_t  *p_son_oam_anr_limited_mode_pci_info,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_add_req
(
    son_oam_nr_enb_add_req_t  *p_son_oam_nr_enb_add_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_add_resp
(
    son_oam_nr_enb_add_resp_t  *p_son_oam_nr_enb_add_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_update_req
(
    son_oam_nr_enb_update_req_t  *p_son_oam_nr_enb_update_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_update_resp
(
    son_oam_nr_enb_update_resp_t  *p_son_oam_nr_enb_update_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_delete_req
(
    son_oam_nr_enb_delete_req_t  *p_son_oam_nr_enb_delete_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_delete_resp
(
    son_oam_nr_enb_delete_resp_t  *p_son_oam_nr_enb_delete_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_retrieve_req
(
    son_oam_nr_enb_retrieve_req_t  *p_son_oam_nr_enb_retrieve_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_nr_enb_retrieve_resp
(
    son_oam_nr_enb_retrieve_resp_t  *p_son_oam_nr_enb_retrieve_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_proc_sup_resp
(
    son_oam_proc_sup_resp_t  *p_son_oam_proc_sup_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_feature_state_change_ind
(
    son_oam_feature_state_change_ind_t  *p_son_oam_feature_state_change_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_enable_req
(
    son_oam_mro_enable_req_t  *p_son_oam_mro_enable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_enable_resp
(
    son_oam_mro_enable_resp_t  *p_son_oam_mro_enable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_disable_req
(
    son_oam_mro_disable_req_t  *p_son_oam_mro_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_disable_resp
(
    son_oam_mro_disable_resp_t  *p_son_oam_mro_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_modify_config_params_req
(
    son_oam_mro_modify_config_params_req_t  *p_son_oam_mro_modify_config_params_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_modify_config_params_resp
(
    son_oam_mro_modify_config_params_resp_t  *p_son_oam_mro_modify_config_params_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_eutran_ttt_update_ind
(
    son_oam_mro_eutran_ttt_update_ind_t  *p_son_oam_mro_eutran_ttt_update_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_ho_counter_report
(
    son_oam_mro_ho_counter_report_t  *p_son_oam_mro_ho_counter_report,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_enable_req
(
    son_oam_mlb_enable_req_t  *p_son_oam_mlb_enable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_enable_resp
(
    son_oam_mlb_enable_resp_t  *p_son_oam_mlb_enable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_disable_req
(
    son_oam_mlb_disable_req_t  *p_son_oam_mlb_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_disable_resp
(
    son_oam_mlb_disable_resp_t  *p_son_oam_mlb_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_modify_attributes_req
(
    son_oam_mlb_modify_attributes_req_t  *p_son_oam_mlb_modify_attributes_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_modify_attributes_resp
(
    son_oam_mlb_modify_attributes_resp_t  *p_son_oam_mlb_modify_attributes_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_event_notification
(
    son_oam_event_notification_t  *p_son_oam_event_notification,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_gu_group_id_config_req
(
    son_oam_anr_gu_group_id_config_req_t  *p_son_oam_anr_gu_group_id_config_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_gu_group_id_config_resp
(
    son_oam_anr_gu_group_id_config_resp_t  *p_son_oam_anr_gu_group_id_config_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_neighbor_default_config_req
(
    son_oam_anr_neighbor_default_config_req_t  *p_son_oam_anr_neighbor_default_config_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_neighbor_default_config_resp
(
    son_oam_anr_neighbor_default_config_resp_t  *p_son_oam_anr_neighbor_default_config_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_carrier_freq_and_dl_tx_power_req
(
    son_oam_carrier_freq_and_dl_tx_power_req_t  *p_son_oam_carrier_freq_and_dl_tx_power_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_carrier_freq_and_dl_tx_power_resp
(
    son_oam_carrier_freq_and_dl_tx_power_resp_t  *p_son_oam_carrier_freq_and_dl_tx_power_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_last_scan_timestamp_ind
(
    son_oam_last_scan_timestamp_ind_t  *p_son_oam_last_scan_timestamp_ind,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_params_info
(
    son_oam_mlb_params_info_t  *p_son_oam_mlb_params_info,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_offset_change_req
(
    son_oam_mlb_offset_change_req_t  *p_son_oam_mlb_offset_change_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_offset_change_resp
(
    son_oam_mlb_offset_change_resp_t  *p_son_oam_mlb_offset_change_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_peer_offset_change_req
(
    son_oam_mlb_peer_offset_change_req_t  *p_son_oam_mlb_peer_offset_change_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mlb_peer_offset_change_resp
(
    son_oam_mlb_peer_offset_change_resp_t  *p_son_oam_mlb_peer_offset_change_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_mro_optimization_notification
(
    son_oam_mro_optimization_notification_t  *p_son_oam_mro_optimization_notification,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_autonomous_switch_off_config_req
(
    son_oam_es_autonomous_switch_off_config_req_t  *p_son_oam_es_autonomous_switch_off_config_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_es_autonomous_switch_off_config_resp
(
    son_oam_es_autonomous_switch_off_config_resp_t  *p_son_oam_es_autonomous_switch_off_config_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_chk_health_req
(
    son_oam_chk_health_req_t  *p_son_oam_chk_health_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_chk_health_resp
(
    son_oam_chk_health_resp_t  *p_son_oam_chk_health_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_meas_config_obj_remove_req
(
    son_oam_anr_meas_config_obj_remove_req_t  *p_son_oam_anr_meas_config_obj_remove_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_anr_meas_config_obj_remove_res
(
    son_oam_anr_meas_config_obj_remove_res_t  *p_son_oam_anr_meas_config_obj_remove_res,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_enable_req
(
    son_oam_rach_opt_enable_req_t  *p_son_oam_rach_opt_enable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_enable_resp
(
    son_oam_rach_opt_enable_resp_t  *p_son_oam_rach_opt_enable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_disable_req
(
    son_oam_rach_opt_disable_req_t  *p_son_oam_rach_opt_disable_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_disable_resp
(
    son_oam_rach_opt_disable_resp_t  *p_son_oam_rach_opt_disable_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_modify_config_params_req
(
    son_oam_rach_opt_modify_config_params_req_t  *p_son_oam_rach_opt_modify_config_params_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_modify_config_params_resp
(
    son_oam_rach_opt_modify_config_params_resp_t  *p_son_oam_rach_opt_modify_config_params_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_modify_rach_optmode_req
(
    son_oam_rach_opt_modify_rach_optmode_req_t  *p_son_oam_rach_opt_modify_rach_optmode_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_modify_rach_optmode_resp
(
    son_oam_rach_opt_modify_rach_optmode_resp_t  *p_son_oam_rach_opt_modify_rach_optmode_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_failure_notification
(
    son_oam_rach_opt_failure_notification_t  *p_son_oam_rach_opt_failure_notification,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_parameter_update_req
(
    son_oam_rach_opt_parameter_update_req_t  *p_son_oam_rach_opt_parameter_update_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_ue_info_report
(
    son_oam_rach_opt_ue_info_report_t  *p_son_oam_rach_opt_ue_info_report,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_rach_opt_l2_perf_report
(
    son_oam_rach_opt_l2_perf_report_t  *p_son_oam_rach_opt_l2_perf_report,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_cell_delete_req
(
    son_oam_cell_delete_req_t  *p_son_oam_cell_delete_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_cell_delete_resp
(
    son_oam_cell_delete_resp_t  *p_son_oam_cell_delete_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_get_debug_info_req
(
    son_oam_get_debug_info_req_t  *p_son_oam_get_debug_info_req,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

oam_return_et
oam_son_send_son_oam_get_debug_info_resp
(
    son_oam_get_debug_info_resp_t  *p_son_oam_get_debug_info_resp,
    U16                 src_module_id,
    U16                 dest_module_id, 
    U16                 transaction_id,
    U8                  cell_index 
);

#endif /* _OAM_SON_COMPOSER_H_ */
#endif
