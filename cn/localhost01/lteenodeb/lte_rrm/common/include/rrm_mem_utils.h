/*****************************************************************************
 * File Name   : rrm_mem_utils.h
 *
 * Description : This file contains the prototypes for memory utility functions
 *
 *
 * Revision History:
 *
 * Date         Author           Reference          Comments
 * -----        ------           ---------          --------
 * November,2012  Nitin Agarwal                        Initial
 *
 * Copyright (c) 2012, Aricent Group.
 ****************************************************************************/
#ifndef RRM_MEM_UTILS_H
#define RRM_MEM_UTILS_H

//#include "rrm_utils.h" 
#include "rrm_mif_db.h"
#include "rrm_oam_types.h" 
#include "rrm_oam_intf.h" 
#include "rrm_api_types.h" 
#include "rrm_cell_rrmim_intf.h" 
#include "rrm_uem_rrmim_intf.h" 
#include "rrm_uem_cellm_intf.h" 
#include "rrm_ue_context.h"
#include "rrm_cell_context.h"
#include "rrm_l2_intf.h"

/*CODE OPT START*/
void
rrm_memcpy_rrm_son_tnl_discovery_res(
        rrm_son_tnl_discovery_res_t *p_dst,
        rrm_son_tnl_discovery_res_t *p_src);

void
rrm_memcpy_rrm_oam_cell_plmn_info(
        rrm_oam_cell_plmn_info_t *p_dst,
        rrm_oam_cell_plmn_info_t *p_src);


 void
rrm_memcpy_rrc_uplink_um_rlc_erb_service_profile(
        rrc_uplink_um_rlc_erb_service_profile_t *p_dst,
        rrc_uplink_um_rlc_erb_service_profile_t *p_src);

void
rrm_memcpy_rrm_son_tnl_discovery_req(
        rrm_son_tnl_discovery_req_t *p_dst,
        rrm_son_tnl_discovery_req_t *p_src);
 void
rrm_memcpy_rrmcm_rmif_generic_resp(
        rrmcm_rmif_generic_resp_t *p_dst,
        rrmcm_rmif_generic_resp_t *p_src);

 void
rrm_memcpy_rrmuem_rmif_emergency_call_active_res(
        rrmuem_rmif_emergency_call_active_res_t *p_dst,
        rrmuem_rmif_emergency_call_active_res_t *p_src);

 void
rrm_memcpy_rrmuem_rmif_non_emrgncy_active_calls_ho_resp(
        rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *p_dst,
        rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *p_src);

 void
rrm_memcpy_rrmuem_rmif_no_active_ue_ind(
        rrmuem_rmif_no_active_ue_ind_t *p_dst,
        rrmuem_rmif_no_active_ue_ind_t *p_src);

void
rrm_memcpy_rrmcm_rmif_cell_config_resp(
        rrmcm_rmif_cell_config_resp *p_dst,
        rrmcm_rmif_cell_config_resp *p_src);

void
rrm_memcpy_rrm_global_cell_id(
        rrm_global_cell_id_t *p_dst,
        rrm_global_cell_id_t *p_src);

void
rrm_memcpy_rrmcm_rmif_rac_enable_disable_res(
        rrmcm_rmif_rac_enable_disable_res_t *p_dst,
        rrmcm_rmif_rac_enable_disable_res_t *p_src);
 void
rrm_memcpy_rrm_ue_count_threshold_reached_ind(
        rrm_ue_count_threshold_reached_ind_t *p_dst,
        rrm_ue_count_threshold_reached_ind_t *p_src);

 void
rrm_memcpy_rrmcm_rmif_cell_active_ue_report_ind(
        rrmcm_rmif_cell_active_ue_report_ind_t *p_dst,
        rrmcm_rmif_cell_active_ue_report_ind_t *p_src);


 void
rrm_memcpy_rrmcm_rmif_cell_state_change_ind(
        rrmcm_rmif_cell_state_change_ind_t *p_dst,
        rrmcm_rmif_cell_state_change_ind_t *p_src);

void
rrm_memcpy_rrm_oam_eutran_global_cell_id(
        rrm_oam_eutran_global_cell_id_t *p_dst,
        rrm_oam_eutran_global_cell_id_t *p_src);

void
rrm_memcpy_rrm_son_utran_physical_cell_id(
        rrm_son_utran_physical_cell_id_t *p_dst,
        rrm_son_utran_physical_cell_id_t *p_src);

void
rrm_memcpy_rrm_son_utran_geran_global_cell_id(
        rrm_son_utran_geran_global_cell_id_t *p_dst,
        rrm_son_utran_geran_global_cell_id_t *p_src);
void
rrm_memcpy_lac(
        U8 *p_dst,
        U8 *p_src,
        U16 size);

void
rrm_memcpy_tac(
        U8 *p_dst,
        U8 *p_src);
void

rrm_memcpy_mcc(
        U8 *p_dst,
        U8 *p_src);

void
rrm_memcpy_mnc(
        U8 *p_dst,
        U8 *p_src,
        U8 size);


void
rrm_memcpy_u8(
        U8 *p_dst,
        U8 *p_src,
        U8 size);

void
rrm_memcpy_u16(
        U16 *p_dst,
        U16 *p_src,
        U16 size);

void
rrm_memcpy_plmn_id(
        rrm_oam_cell_plmn_info_t *p_dst,
        rrm_oam_cell_plmn_info_t *p_src);

 void
rrm_memcpy_rrm_son_geran_physical_cell_id(
        rrm_son_geran_physical_cell_id_t *p_dst,
        rrm_son_geran_physical_cell_id_t *p_src);
void
rrm_memcpy_rrm_son_registration_based_res(
        rrm_son_registration_based_res_t *p_dst,
        rrm_son_registration_based_res_t *p_src);


void
rrm_memcpy_rrm_scheduling_info_list(
        rrm_scheduling_info_list_t *p_dst,
        rrm_scheduling_info_list_t *p_src);

void
rrm_memcpy_rrm_ue_erab_setup_item_list(
        rrm_ue_erab_setup_item_list_t *p_dst,
        rrm_ue_erab_setup_item_list_t *p_src);

void
rrm_memcpy_ho_restriction_list(
        ho_restriction_list_t *p_dst,
        ho_restriction_list_t *p_src);


 void
rrm_memcpy_rrc_eutran_type(
        rrc_eutran_type_t *p_dst,
        rrc_eutran_type_t *p_src);

 void
rrm_memcpy_rrm_l2_get_snr_report(
        rrm_l2_get_snr_report_t *p_dst,
        rrm_l2_get_snr_report_t *p_src);

 void
rrm_memcpy_rrm_l2_snr_report(
        rrm_l2_snr_report_t *p_dst,
        rrm_l2_snr_report_t *p_src);

/*BUG 13332:start*/
 /* code removed */
/*BUG 13332:end*/

void
rrm_memcpy_rrm_scheduling_info_list(
        rrm_scheduling_info_list_t *p_dst,
        rrm_scheduling_info_list_t *p_src);

void
rrm_memcpy_rrm_scheduling_info(
        rrm_scheduling_info_t *p_dst,
        rrm_scheduling_info_t *p_src);

void
rrm_memcpy_rrm_sib_mapping_info(
        rrm_sib_mapping_info_t *p_dst,
        rrm_sib_mapping_info_t *p_src);



void
rrm_memcpy_inter_freq_cells(
        inter_freq_cells_t *p_dst,
        inter_freq_cells_t *p_src);



void
rrm_memset_rrm_oam_cell_plmn_info(
        rrm_oam_cell_plmn_info_t *p_dst,
        U8 val);


void
rrm_memset_rrm_oam_eutran_global_cell_id(
        rrm_oam_eutran_global_cell_id_t *p_dst,
        U8 val);

void 
rrm_memset_rrm_son_eutran_neighbor_cell(
        rrm_son_eutran_neighbor_cell_t *p_dst,
        U8 val);

void 
rrm_memset_rrm_son_eutran_meas_results(
        rrm_son_eutran_meas_results_t *p_dst,
        U8 val);

void
rrm_memset_rrm_son_utran_neighbor_cell(
        rrm_son_utran_neighbor_cell_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_utran_meas_results(
        rrm_son_utran_meas_results_t *p_dst,
        U8 val);

void
rrm_memset_rrm_son_geran_neighbor_cell( 
        rrm_son_geran_neighbor_cell_t *p_dst,
        U8 val);

void 
rrm_memset_rrm_son_geran_meas_results(
        rrm_son_geran_meas_results_t *p_dst,
        U8 val);


void
rrm_memset_rrm_cell_meas_data(
        rrm_cell_meas_data_t *p_dst,
        U8 val);


 void
rrm_memset_rrm_oam_ue_release_req(
        rrm_oam_ue_release_req_t *p_dst,
        U8 val);

void
rrm_memset_rrm_son_meas_config_res(
        rrm_son_meas_config_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_meas_results_ind(
        rrm_son_meas_results_ind_t *p_dst,
        U8 val);


void
rrm_memset_rrm_oam_cell_config_resp(
        rrm_oam_cell_config_resp_t *p_dst,
        U8 val);

void
rrm_memset_rrm_oam_cell_reconfig_resp(
        rrm_oam_cell_reconfig_resp_t *p_dst,
        U8 val);


void
rrm_memset_rrm_oam_delete_resp(
        rrm_oam_delete_resp_t *p_dst,
        U8 val);

void
rrm_memset_rrm_oam_cell_start_resp(
        rrm_oam_cell_start_resp_t *p_dst,
        U8 val);


void
rrm_memset_rrm_oam_cell_stop_resp(
        rrm_oam_cell_stop_resp_t *p_dst,
        U8 val);


void
rrm_memset_rrm_oam_cell_block_resp(
        rrm_oam_cell_block_resp_t *p_dst,
        U8 val);


void
rrm_memset_son__and_dl_power_params_req(
        rrmcm_rmif_cell_son_register_deregister_req_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_registration_based_res(
        rrm_son_registration_based_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_set_attr_res(
        rrm_son_set_attr_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_registration_res(
        rrm_son_registration_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_deregistration_res(
        rrm_son_deregistration_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_cell_switch_off_res(
        rrm_son_cell_switch_off_res_t *p_dst,
        U8 val);


void
rrm_memset_rrm_son_cell_switch_on_res(
        rrm_son_cell_switch_on_res_t *p_dst,
        U8 val);

void
rrm_memset_rrm_son_cell_switch_on_req(
        rrm_son_cell_switch_on_req_t *p_dst,
        U8 val);

void
rrm_memset_rrm_oam_ready_for_cell_block_ind(
        rrm_oam_ready_for_cell_block_ind_t *p_dst,
        U8 val);



void
rrm_memset_rrm_enb_tnl_address(
        rrm_enb_tnl_address_t *p_dst,
        U8 val);


void 
rrm_memset_rrm_son_tnl_discovery_res(
        rrm_son_tnl_discovery_res_t *p_dst,
        U8 val);

/* RACH_OPTIMIZATION_CHANGES_START */
void
rrm_memset_rrm_son_rach_info_res(
        rrm_son_rach_config_resp_t *p_dst,
        U8 val);
/* RACH_OPTIMIZATION_CHANGES_END */


/* SPR 10730 Fix Start */
/* Code Deleted */
/* SPR 10730 Fix End */
/*CODE OPT END*/

/*  DYNAMIC ICIC CHANGES START  */

void rrm_memcpy_alpha_pathloss_target_sinr_map(
               AlphaBasedPathLossToTargetSINRMap            *d_ptr,
        rrm_alpha_based_pathloss_target_sinr_map_t   *s_ptr);

void rrm_memcpy_oam_alpha_pathloss_target_sinr_map(
        rrm_alpha_based_pathloss_target_sinr_map_t      *d_ptr,
        rrm_oam_alpha_based_pathloss_target_sinr_map_t  *s_ptr);

void rrm_memcpy_mif_resource_partition(
        resource_partition_t    *d_ptr,
        rrm_oam_resource_partition_t *s_ptr);

/* SPR_10352_change_start */
/* Function prototype deleted: rrm_memcpy_mif_alpha_pathloss_target_sinr_map */
/* SPR_10352_change_end */

void rrm_memcpy_rrm_delta_sinr_to_tpc_map_pusch(
                        DeltaSINRToTPCMapForPUSCH    *d_ptr,
        rrm_delta_sinr_to_tpc_map_pusch     *s_ptr);

void rrm_memcpy_bler_to_tpc_map_for_pucch(
                            DeltaBLERToTPCMapForPUCCH    *d_ptr,
        rrm_bler_to_tpc_map_for_pucch_t     *s_ptr);


void rrm_memcpy_delta_sinr_tpc_map_pucch_info(
                                    DeltaSINRToTPCMapForPUCCH        *d_ptr,
        rrm_delta_sinr_to_tpc_map_pucch_info_t  *s_ptr);

void rrm_memcpy_rrm_l2_cqi_to_phich_power(
                            CqiToPHICHPower      *d_ptr,
        rrm_cqi_to_phich_power_t    *s_ptr);

void rrm_memcpy_rrm_sps_dl_scheduling_Info_per_tti(
                            SpsDlSchedulingInfoPerTti        *d_ptr,
        rrm_sps_dl_scheduling_Info_per_tti_t    *s_ptr);


void rrm_memcpy_rrm_sps_ul_scheduling_Info_per_tti(
                            SpsUlSchedulingInfoPerTti        *d_ptr,
        rrm_sps_ul_scheduling_Info_per_tti_t    *s_ptr);

void rrm_memcpy_rrm_dynamic_cfi_extension_params(
                        DynamicCFIExtensionParams    *d_ptr,
        rrm_dynamic_cfi_extension_params_t  *s_ptr);

void rrm_memcpy_rrm_aggregation_power_offset(
                                    AggregationPowerOffset   *d_ptr,
        rrm_aggregation_power_offset_t  *s_ptr);


/*  DYNAMIC ICIC CHANGES END    */

#endif
