/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_son_parser.c $
 *
 *******************************************************************************
 *
 * File Description: File rrm_son_parser.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_utils.h"
#include "rrm_pack_unpack_utils.h"
#include "rrm_son_parser.h"
#include "rrm_son_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrm_return_et
rrm_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_cell_object_id
(
    rrm_cell_object_id_t *p_rrm_cell_object_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_object_id
(
    rrm_object_id_t *p_rrm_object_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_trgr_periodic_value
(
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_trgr_threshold_value
(
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_trgr_event_value
(
    rrm_trgr_event_value_t *p_rrm_trgr_event_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_trigger
(
    rrm_trigger_t *p_rrm_trigger,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_attribute
(
    rrm_attribute_t *p_rrm_attribute,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_object
(
    object_t *p_object,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_register_deregister_format
(
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_non_registration_based_res
(
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_registration_based_res
(
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_nmm_res
(
    rrm_son_nmm_res_t *p_rrm_son_nmm_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_speed_state_scale_factors
(
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_eutran
(
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_pci_fdd_range
(
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran_fdd
(
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran_tdd
(
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran
(
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_following_arfcns
(
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_carrier_freqs
(
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_physical_cell_id
(
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_carrier_freq_info
(
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_geran
(
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_carrier_freq_info
(
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_sys_time_info
(
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_pre_reg_hrpd_info
(
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_hrpd_info
(
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_csfb_reg_param_1XRTT
(
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_1xrtt_info
(
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_csfb_reg_param_1xrtt_r9
(
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_ac_barring_config_1xrtt
(
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_config_cdma
(
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_inter_rat_meas_config
(
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_setup_meas_gap_config
(
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_gap_config
(
    rrm_son_gap_config_t *p_rrm_son_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_gap_config
(
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_eutran_neighbor_cell
(
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_eutran_meas_results
(
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_physical_cell_id
(
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_geran_global_cell_id
(
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_neighbor_cell
(
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_meas_results
(
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_neighbor_cell
(
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_meas_results
(
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_global_cell_id
(
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_neighbor_cell
(
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cdma_meas_results
(
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_utran_geran_global_cell_id
(
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_others_global_cell_id
(
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_inter_rat_global_cell_id
(
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_oam_global_cell_id
(
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_offset_attr
(
    rrm_son_offset_attr_t *p_rrm_son_offset_attr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_eutran_strength
(
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_utran_strength
(
    rrm_son_utran_strength_t *p_rrm_son_utran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_geran_strength
(
    rrm_son_geran_strength_t *p_rrm_son_geran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_meas_strength
(
    rrm_son_meas_strength_t *p_rrm_son_meas_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_hw_load_ind
(
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_s1_tnl_load_ind
(
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_rrs_load_ind
(
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_comp_avl_cap
(
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_comp_avl_cap_grp
(
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_abs_status
(
    rrm_son_abs_status_t *p_rrm_son_abs_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_cell_load_info
(
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_global_enb_id
(
    rrm_global_enb_id_t *p_rrm_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_enb_tnl_address
(
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_strong_cell_info
(
    strong_cell_info_t *p_strong_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_ue_prach_data_info
(
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_oam_gu_group_id
(
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_son_gu_group_id_list
(
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_subframe_allocation
(
    rrm_subframe_allocation_t *p_rrm_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_mbsfn_config_info
(
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_mbsfn_subframe_config_list
(
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_cell_sib2_mbsfn_data
(
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_return_et
rrm_parse_rrm_son_register_req
(
    rrm_son_register_req_t *p_rrm_son_register_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_register_req, 0, sizeof(rrm_son_register_req_t));

    /* This function parses rrm_son_register_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_register_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_register_deregister_format(
        &p_rrm_son_register_req->register_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_oam_cell_plmn_info, 0, sizeof(rrm_oam_cell_plmn_info_t));

    /* This function parses rrm_oam_cell_plmn_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_oam_cell_plmn_info");


    if (*p_length_read + (S32)sizeof(p_rrm_oam_cell_plmn_info->mcc) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_cell_plmn_info->mcc); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_oam_cell_plmn_info->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_oam_cell_plmn_info->num_mnc_digit, p_src + *p_length_read, "num_mnc_digit");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_plmn_info->num_mnc_digit < 2) || (p_rrm_oam_cell_plmn_info->num_mnc_digit > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_oam_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= 3] !", p_rrm_oam_cell_plmn_info->num_mnc_digit);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_oam_cell_plmn_info->num_mnc_digit * sizeof(p_rrm_oam_cell_plmn_info->mnc[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_cell_plmn_info->num_mnc_digit; loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_oam_cell_plmn_info->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_oam_eutran_global_cell_id, 0, sizeof(rrm_oam_eutran_global_cell_id_t));

    /* This function parses rrm_oam_eutran_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_oam_eutran_global_cell_id");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_oam_eutran_global_cell_id->primary_plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_oam_eutran_global_cell_id->cell_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_eutran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_oam_eutran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_cell_object_id
(
    rrm_cell_object_id_t *p_rrm_cell_object_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_cell_object_id, 0, sizeof(rrm_cell_object_id_t));

    /* This function parses rrm_cell_object_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_cell_object_id");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_cell_object_id->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_object_id
(
    rrm_object_id_t *p_rrm_object_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_object_id, 0, sizeof(rrm_object_id_t));

    /* This function parses rrm_object_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_object_id");


    
    if (RRM_FAILURE == rrm_parse_rrm_cell_object_id(
        &p_rrm_object_id->object_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_trgr_periodic_value
(
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_trgr_periodic_value, 0, sizeof(rrm_trgr_periodic_value_t));

    /* This function parses rrm_trgr_periodic_value */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_trgr_periodic_value");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_trgr_periodic_value->interval, p_src + *p_length_read, "interval");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_trgr_threshold_value
(
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_trgr_threshold_value, 0, sizeof(rrm_trgr_threshold_value_t));

    /* This function parses rrm_trgr_threshold_value */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_trgr_threshold_value");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_trgr_threshold_value->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_trgr_event_value
(
    rrm_trgr_event_value_t *p_rrm_trgr_event_value,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_trgr_event_value, 0, sizeof(rrm_trgr_event_value_t));

    /* This function parses rrm_trgr_event_value */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_trgr_event_value");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_trgr_event_value->enable, p_src + *p_length_read, "enable");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_trigger
(
    rrm_trigger_t *p_rrm_trigger,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_trigger, 0, sizeof(rrm_trigger_t));

    /* This function parses rrm_trigger */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_trigger");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_trigger->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_trigger->id, p_src + *p_length_read, "id");
    *p_length_read += sizeof(U32);

    if (p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_PERIODIC)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_trgr_periodic_value(
        &p_rrm_trigger->periodic,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_THRESHOLD)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_trgr_threshold_value(
        &p_rrm_trigger->threshold,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_EVENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_trgr_event_value(
        &p_rrm_trigger->event,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_attribute
(
    rrm_attribute_t *p_rrm_attribute,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_attribute, 0, sizeof(rrm_attribute_t));

    /* This function parses rrm_attribute */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_attribute");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_attribute->id, p_src + *p_length_read, "id");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_attribute->trgr_list_size, p_src + *p_length_read, "trgr_list_size");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_attribute->trgr_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_trigger(
                &p_rrm_attribute->trgr_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_attribute->value, p_src + *p_length_read, "value");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_object
(
    object_t *p_object,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_object, 0, sizeof(object_t));

    /* This function parses object */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing object");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_object->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_object_id(
        &p_object->id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_object->attr_list_size, p_src + *p_length_read, "attr_list_size");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_object->attr_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_attribute(
                &p_object->p_attr_lst[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_register_deregister_format
(
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_register_deregister_format, 0, sizeof(rrm_son_register_deregister_format_t));

    /* This function parses rrm_son_register_deregister_format */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_register_deregister_format");


    
    if (RRM_FAILURE == rrm_parse_object(
        &p_rrm_son_register_deregister_format->object,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_deregister_req
(
    rrm_son_deregister_req_t *p_rrm_son_deregister_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_deregister_req, 0, sizeof(rrm_son_deregister_req_t));

    /* This function parses rrm_son_deregister_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_deregister_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_register_deregister_format(
        &p_rrm_son_deregister_req->deregister_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_set_attribute_value_req
(
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_set_attribute_value_req, 0, sizeof(rrm_son_set_attribute_value_req_t));

    /* This function parses rrm_son_set_attribute_value_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_set_attribute_value_req");


    
    if (RRM_FAILURE == rrm_parse_object(
        &p_rrm_son_set_attribute_value_req->object,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_cell_switch_on_res
(
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_switch_on_res, 0, sizeof(rrm_son_cell_switch_on_res_t));

    /* This function parses rrm_son_cell_switch_on_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_switch_on_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_non_registration_based_res(
        &p_rrm_son_cell_switch_on_res->cell_switchon_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_non_registration_based_res
(
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_non_registration_based_res, 0, sizeof(rrm_son_non_registration_based_res_t));

    /* This function parses rrm_son_non_registration_based_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_non_registration_based_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_non_registration_based_res->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_non_registration_based_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_non_registration_based_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_cell_switch_off_res
(
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_switch_off_res, 0, sizeof(rrm_son_cell_switch_off_res_t));

    /* This function parses rrm_son_cell_switch_off_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_switch_off_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_non_registration_based_res(
        &p_rrm_son_cell_switch_off_res->cell_switchoff_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_registration_res
(
    rrm_son_registration_res_t *p_rrm_son_registration_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_registration_res, 0, sizeof(rrm_son_registration_res_t));

    /* This function parses rrm_son_registration_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_registration_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_registration_based_res(
        &p_rrm_son_registration_res->registration_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_registration_based_res
(
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_registration_based_res, 0, sizeof(rrm_son_registration_based_res_t));

    /* This function parses rrm_son_registration_based_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_registration_based_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_object_id(
        &p_rrm_son_registration_based_res->id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_registration_based_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_registration_based_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_deregistration_res
(
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_deregistration_res, 0, sizeof(rrm_son_deregistration_res_t));

    /* This function parses rrm_son_deregistration_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_deregistration_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_registration_based_res(
        &p_rrm_son_deregistration_res->deregistration_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_set_attr_res
(
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_set_attr_res, 0, sizeof(rrm_son_set_attr_res_t));

    /* This function parses rrm_son_set_attr_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_set_attr_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_registration_based_res(
        &p_rrm_son_set_attr_res->set_attr_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_set_attr_res->id, p_src + *p_length_read, "id");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_active_ue_cnt_report
(
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_active_ue_cnt_report, 0, sizeof(rrm_son_active_ue_cnt_report_t));

    /* This function parses rrm_son_active_ue_cnt_report */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_active_ue_cnt_report");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_active_ue_cnt_report->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_active_ue_cnt_report->ue_cnt, p_src + *p_length_read, "ue_cnt");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_ue_count_threshold_hit_ind
(
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_ue_count_threshold_hit_ind, 0, sizeof(rrm_ue_count_threshold_hit_ind_t));

    /* This function parses rrm_ue_count_threshold_hit_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_ue_count_threshold_hit_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_ue_count_threshold_hit_ind->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_ue_count_threshold_hit_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_cell_switch_off_req
(
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_switch_off_req, 0, sizeof(rrm_son_cell_switch_off_req_t));

    /* This function parses rrm_son_cell_switch_off_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_switch_off_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_cell_switch_off_req->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cell_switch_off_req->switchoff_type, p_src + *p_length_read, "switchoff_type");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cell_switch_off_req->switchoff_time, p_src + *p_length_read, "switchoff_time");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_cell_switch_on_req
(
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_switch_on_req, 0, sizeof(rrm_son_cell_switch_on_req_t));

    /* This function parses rrm_son_cell_switch_on_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_switch_on_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_cell_switch_on_req->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_cell_state_change_ind
(
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_state_change_ind, 0, sizeof(rrm_son_cell_state_change_ind_t));

    /* This function parses rrm_son_cell_state_change_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_state_change_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_cell_state_change_ind->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cell_state_change_ind->cell_state, p_src + *p_length_read, "cell_state");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cell_state_change_ind->cell_state_change_cause, p_src + *p_length_read, "cell_state_change_cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_nmm_prepare_res
(
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_nmm_prepare_res, 0, sizeof(rrm_son_nmm_prepare_res_t));

    /* This function parses rrm_son_nmm_prepare_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_nmm_prepare_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_nmm_res(
        &p_rrm_son_nmm_prepare_res->prepare_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_nmm_res
(
    rrm_son_nmm_res_t *p_rrm_son_nmm_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_nmm_res, 0, sizeof(rrm_son_nmm_res_t));

    /* This function parses rrm_son_nmm_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_nmm_res");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_nmm_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_nmm_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_nmm_complete_res
(
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_nmm_complete_res, 0, sizeof(rrm_son_nmm_complete_res_t));

    /* This function parses rrm_son_nmm_complete_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_nmm_complete_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_nmm_res(
        &p_rrm_son_nmm_complete_res->complete_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_meas_config_req
(
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_req, 0, sizeof(rrm_son_meas_config_req_t));

    /* This function parses rrm_son_meas_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_req");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_meas_config_req->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_meas_config_req->bitmask & RRM_SON_UE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_meas_config_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_req->eutran_config_list_size, p_src + *p_length_read, "eutran_config_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_req->eutran_config_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_req->eutran_config_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_req->eutran_config_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_req->eutran_config_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_eutran(
                &p_rrm_son_meas_config_req->eutran_config_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    
    if (RRM_FAILURE == rrm_parse_rrm_son_inter_rat_meas_config(
        &p_rrm_son_meas_config_req->inter_rat_meas_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_meas_config_req->bitmask & RRM_SON_MEAS_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_meas_gap_config(
        &p_rrm_son_meas_config_req->meas_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_speed_state_scale_factors
(
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_speed_state_scale_factors, 0, sizeof(rrm_son_speed_state_scale_factors_t));

    /* This function parses rrm_son_speed_state_scale_factors */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_speed_state_scale_factors");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_speed_state_scale_factors->sf_medium, p_src + *p_length_read, "sf_medium");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_speed_state_scale_factors->sf_medium > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_speed_state_scale_factors->sf_medium] should be in range [%d <= 3] !", p_rrm_son_speed_state_scale_factors->sf_medium);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_speed_state_scale_factors->sf_high, p_src + *p_length_read, "sf_high");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_speed_state_scale_factors->sf_high > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_speed_state_scale_factors->sf_high] should be in range [%d <= 3] !", p_rrm_son_speed_state_scale_factors->sf_high);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_eutran
(
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_eutran, 0, sizeof(rrm_son_meas_config_eutran_t));

    /* This function parses rrm_son_meas_config_eutran */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_eutran");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_eutran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_eutran->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_eutran->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->meas_bandwidth > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->meas_bandwidth] should be in range [%d <= 5] !", p_rrm_son_meas_config_eutran->meas_bandwidth);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->presence_antenna_port, p_src + *p_length_read, "presence_antenna_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->presence_antenna_port > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->presence_antenna_port] should be in range [%d <= 1] !", p_rrm_son_meas_config_eutran->presence_antenna_port);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->neighbor_cell_config, p_src + *p_length_read, "neighbor_cell_config");
    *p_length_read += sizeof(U8);
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_eutran->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->offset_frequency > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->offset_frequency] should be in range [%d <= 30] !", p_rrm_son_meas_config_eutran->offset_frequency);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_eutran->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->q_rx_lev_min < -70) || (p_rrm_son_meas_config_eutran->q_rx_lev_min > -22))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->q_rx_lev_min] should be in range [-70 <= %d <= -22] !", p_rrm_son_meas_config_eutran->q_rx_lev_min);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_eutran->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->p_max < -30) || (p_rrm_son_meas_config_eutran->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->p_max] should be in range [-30 <= %d <= 33] !", p_rrm_son_meas_config_eutran->p_max);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_eutran->t_reselection);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_SF_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_speed_state_scale_factors(
        &p_rrm_son_meas_config_eutran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshX_high);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshX_low);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_eutran->cell_reselection_priority);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size, p_src + *p_length_read, "cells_for_which_to_report_cgi_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi[loop], (void*)(p_src + *p_length_read), "cells_for_which_to_report_cgi[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_QUAL_MIN_R9)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_eutran->q_qualmin_r9, p_src + *p_length_read, "q_qualmin_r9");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->q_qualmin_r9 < -34) || (p_rrm_son_meas_config_eutran->q_qualmin_r9 > -3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->q_qualmin_r9] should be in range [-34 <= %d <= -3] !", p_rrm_son_meas_config_eutran->q_qualmin_r9);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->threshx_highq_r9, p_src + *p_length_read, "threshx_highq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshx_highq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshx_highq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshx_highq_r9);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_LOW_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_eutran->threshx_lowq_r9, p_src + *p_length_read, "threshx_lowq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshx_lowq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshx_lowq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshx_lowq_r9);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_eutran->cm_offset_frequency, p_src + *p_length_read, "cm_offset_frequency");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->cm_offset_frequency > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cm_offset_frequency] should be in range [%d <= 30] !", p_rrm_son_meas_config_eutran->cm_offset_frequency);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_pci_fdd_range
(
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_pci_fdd_range, 0, sizeof(rrm_son_utran_pci_fdd_range_t));

    /* This function parses rrm_son_utran_pci_fdd_range */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_pci_fdd_range");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_pci_fdd_range->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_utran_pci_fdd_range->start, p_src + *p_length_read, "start");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_pci_fdd_range->start > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_pci_fdd_range->start] should be in range [%d <= 511] !", p_rrm_son_utran_pci_fdd_range->start);
        return RRM_FAILURE;
    }

    if (p_rrm_son_utran_pci_fdd_range->bitmask & RRM_SON_UTRAN_PCI_FDD_RANGE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_utran_pci_fdd_range->range, p_src + *p_length_read, "range");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_pci_fdd_range->range < 2) || (p_rrm_son_utran_pci_fdd_range->range > 512))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_pci_fdd_range->range] should be in range [2 <= %d <= 512] !", p_rrm_son_utran_pci_fdd_range->range);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran_fdd
(
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_utran_fdd, 0, sizeof(rrm_son_meas_config_utran_fdd_t));

    /* This function parses rrm_son_meas_config_utran_fdd */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_utran_fdd");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_utran_fdd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_meas_config_utran_fdd->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_meas_config_utran_fdd->uarfcn);
        return RRM_FAILURE;
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_fdd->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->offset_frequency < -15) || (p_rrm_son_meas_config_utran_fdd->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_meas_config_utran_fdd->offset_frequency);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran_fdd->cell_reselection_priority);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshX_high);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshX_low);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_fdd->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->q_rx_lev_min < -60) || (p_rrm_son_meas_config_utran_fdd->q_rx_lev_min > -13))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_rrm_son_meas_config_utran_fdd->q_rx_lev_min);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_fdd->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->p_max < -50) || (p_rrm_son_meas_config_utran_fdd->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->p_max] should be in range [-50 <= %d <= 33] !", p_rrm_son_meas_config_utran_fdd->p_max);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_fdd->q_qual_min, p_src + *p_length_read, "q_qual_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->q_qual_min < -24) || (p_rrm_son_meas_config_utran_fdd->q_qual_min > 0))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->q_qual_min] should be in range [-24 <= %d <= 0] !", p_rrm_son_meas_config_utran_fdd->q_qual_min);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size, p_src + *p_length_read, "csg_allowed_reporting_cell_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size < 1) || (p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size > MAX_PCI_RANGE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size] should be in range [1 <= %d <= MAX_PCI_RANGE] !", p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_utran_pci_fdd_range(
                &p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size, p_src + *p_length_read, "cells_for_which_to_report_cgi_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi[loop], (void*)(p_src + *p_length_read), "cells_for_which_to_report_cgi[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_HIGH_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->threshx_highq_r9, p_src + *p_length_read, "threshx_highq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshx_highq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshx_highq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshx_highq_r9);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_LOW_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9, p_src + *p_length_read, "threshx_lowq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran_tdd
(
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_utran_tdd, 0, sizeof(rrm_son_meas_config_utran_tdd_t));

    /* This function parses rrm_son_meas_config_utran_tdd */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_utran_tdd");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_utran_tdd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_meas_config_utran_tdd->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_meas_config_utran_tdd->uarfcn);
        return RRM_FAILURE;
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_tdd->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->offset_frequency < -15) || (p_rrm_son_meas_config_utran_tdd->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_meas_config_utran_tdd->offset_frequency);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_tdd->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran_tdd->cell_reselection_priority);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_tdd->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_tdd->threshX_high);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_tdd->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_tdd->threshX_low);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_tdd->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->q_rx_lev_min < -60) || (p_rrm_son_meas_config_utran_tdd->q_rx_lev_min > -13))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_rrm_son_meas_config_utran_tdd->q_rx_lev_min);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_meas_config_utran_tdd->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->p_max < -50) || (p_rrm_son_meas_config_utran_tdd->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->p_max] should be in range [-50 <= %d <= 33] !", p_rrm_son_meas_config_utran_tdd->p_max);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size, p_src + *p_length_read, "cells_for_which_to_report_cgi_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi[loop], (void*)(p_src + *p_length_read), "cells_for_which_to_report_cgi[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_utran
(
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_utran, 0, sizeof(rrm_son_meas_config_utran_t));

    /* This function parses rrm_son_meas_config_utran */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_utran");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_utran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran->utran_fdd_list_size, p_src + *p_length_read, "utran_fdd_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_fdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_fdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_fdd_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_fdd_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_utran_fdd(
                &p_rrm_son_meas_config_utran->utran_fdd_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran->utran_tdd_list_size, p_src + *p_length_read, "utran_tdd_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_tdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_tdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_tdd_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_tdd_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_utran_tdd(
                &p_rrm_son_meas_config_utran->utran_tdd_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_utran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_utran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran->t_reselection);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_SF_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_speed_state_scale_factors(
        &p_rrm_son_meas_config_utran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_following_arfcns
(
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_following_arfcns, 0, sizeof(rrm_son_geran_following_arfcns_t));

    /* This function parses rrm_son_geran_following_arfcns */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_following_arfcns");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_following_arfcns->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_following_arfcns->exp_arfcn_list_size, p_src + *p_length_read, "exp_arfcn_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->exp_arfcn_list_size < 1) || (p_rrm_son_geran_following_arfcns->exp_arfcn_list_size > RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->exp_arfcn_list_size] should be in range [1 <= %d <= RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE] !", p_rrm_son_geran_following_arfcns->exp_arfcn_list_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_geran_following_arfcns->exp_arfcn_list_size * sizeof(p_rrm_son_geran_following_arfcns->exp_arfcn_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_following_arfcns->exp_arfcn_list_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_geran_following_arfcns->exp_arfcn_list[loop], (void*)(p_src + *p_length_read), "exp_arfcn_list[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_following_arfcns->arfcn_spacing, p_src + *p_length_read, "arfcn_spacing");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->arfcn_spacing < 1) || (p_rrm_son_geran_following_arfcns->arfcn_spacing > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->arfcn_spacing] should be in range [1 <= %d <= 8] !", p_rrm_son_geran_following_arfcns->arfcn_spacing);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_following_arfcns->num_of_following_arfcn, p_src + *p_length_read, "num_of_following_arfcn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_following_arfcns->num_of_following_arfcn > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->num_of_following_arfcn] should be in range [%d <= 31] !", p_rrm_son_geran_following_arfcns->num_of_following_arfcn);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size, p_src + *p_length_read, "arfcn_bmp_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size < 1) || (p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size > RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size] should be in range [1 <= %d <= RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP] !", p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size * sizeof(p_rrm_son_geran_following_arfcns->arfcn_bmp_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size; loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_geran_following_arfcns->arfcn_bmp_list[loop], (void*)(p_src + *p_length_read), "arfcn_bmp_list[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_carrier_freqs
(
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_carrier_freqs, 0, sizeof(rrm_son_geran_carrier_freqs_t));

    /* This function parses rrm_son_geran_carrier_freqs */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_carrier_freqs");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_geran_carrier_freqs->starting_arfcn, p_src + *p_length_read, "starting_arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freqs->starting_arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freqs->starting_arfcn] should be in range [%d <= 1023] !", p_rrm_son_geran_carrier_freqs->starting_arfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_carrier_freqs->band_ind, p_src + *p_length_read, "band_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freqs->band_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freqs->band_ind] should be in range [%d <= 1] !", p_rrm_son_geran_carrier_freqs->band_ind);
        return RRM_FAILURE;
    }

    
    if (RRM_FAILURE == rrm_parse_rrm_son_geran_following_arfcns(
        &p_rrm_son_geran_carrier_freqs->following_arfcns,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_physical_cell_id
(
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_physical_cell_id, 0, sizeof(rrm_son_geran_physical_cell_id_t));

    /* This function parses rrm_son_geran_physical_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_physical_cell_id");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_physical_cell_id->ncc, p_src + *p_length_read, "ncc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_physical_cell_id->ncc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_physical_cell_id->ncc] should be in range [%d <= 7] !", p_rrm_son_geran_physical_cell_id->ncc);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_physical_cell_id->bscc, p_src + *p_length_read, "bscc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_physical_cell_id->bscc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_physical_cell_id->bscc] should be in range [%d <= 7] !", p_rrm_son_geran_physical_cell_id->bscc);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_carrier_freq_info
(
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_carrier_freq_info, 0, sizeof(rrm_son_geran_carrier_freq_info_t));

    /* This function parses rrm_son_geran_carrier_freq_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_carrier_freq_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_carrier_freq_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_geran_carrier_freqs(
        &p_rrm_son_geran_carrier_freq_info->geran_car_freqs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->ncc_permitted, p_src + *p_length_read, "ncc_permitted");
    *p_length_read += sizeof(U8);
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->q_rx_lev_min > 45))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->q_rx_lev_min] should be in range [%d <= 45] !", p_rrm_son_geran_carrier_freq_info->q_rx_lev_min);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->p_max_geran, p_src + *p_length_read, "p_max_geran");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->p_max_geran > 39))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->p_max_geran] should be in range [%d <= 39] !", p_rrm_son_geran_carrier_freq_info->p_max_geran);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->threshX_high] should be in range [%d <= 31] !", p_rrm_son_geran_carrier_freq_info->threshX_high);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->threshX_low] should be in range [%d <= 31] !", p_rrm_son_geran_carrier_freq_info->threshX_low);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_geran_carrier_freq_info->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->offset_frequency < -15) || (p_rrm_son_geran_carrier_freq_info->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_geran_carrier_freq_info->offset_frequency);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->meas_obj_id, p_src + *p_length_read, "meas_obj_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->meas_obj_id < 1) || (p_rrm_son_geran_carrier_freq_info->meas_obj_id > 12))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_geran_carrier_freq_info->meas_obj_id);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size, p_src + *p_length_read, "cells_for_which_to_report_cgi_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_geran_physical_cell_id(
                &p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_geran
(
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_geran, 0, sizeof(rrm_son_meas_config_geran_t));

    /* This function parses rrm_son_meas_config_geran */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_geran");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_geran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_geran->geran_freq_info_list_size, p_src + *p_length_read, "geran_freq_info_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_geran->geran_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_geran->geran_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_geran->geran_freq_info_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_geran->geran_freq_info_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_geran_carrier_freq_info(
                &p_rrm_son_meas_config_geran->geran_freq_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_geran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_geran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_geran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_geran->t_reselection);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_speed_state_scale_factors(
        &p_rrm_son_meas_config_geran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_carrier_freq_info
(
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_carrier_freq_info, 0, sizeof(rrm_son_cdma_carrier_freq_info_t));

    /* This function parses rrm_son_cdma_carrier_freq_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_carrier_freq_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_carrier_freq_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_carrier_freq_info->cdma_type, p_src + *p_length_read, "cdma_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->cdma_type > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cdma_type] should be in range [%d <= 1] !", p_rrm_son_cdma_carrier_freq_info->cdma_type);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_carrier_freq_info->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->band_class] should be in range [%d <= 31] !", p_rrm_son_cdma_carrier_freq_info->band_class);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_carrier_freq_info->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->arfcn > 2047))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->arfcn] should be in range [%d <= 2047] !", p_rrm_son_cdma_carrier_freq_info->arfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->threshX_high > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->threshX_high] should be in range [%d <= 63] !", p_rrm_son_cdma_carrier_freq_info->threshX_high);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->threshX_low > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->threshX_low] should be in range [%d <= 63] !", p_rrm_son_cdma_carrier_freq_info->threshX_low);
        return RRM_FAILURE;
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_cdma_carrier_freq_info->offset_freq, p_src + *p_length_read, "offset_freq");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->offset_freq < -15) || (p_rrm_son_cdma_carrier_freq_info->offset_freq > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->offset_freq] should be in range [-15 <= %d <= 15] !", p_rrm_son_cdma_carrier_freq_info->offset_freq);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->srch_wnd_size, p_src + *p_length_read, "srch_wnd_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->srch_wnd_size > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->srch_wnd_size] should be in range [%d <= 15] !", p_rrm_son_cdma_carrier_freq_info->srch_wnd_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->meas_obj_id, p_src + *p_length_read, "meas_obj_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->meas_obj_id < 1) || (p_rrm_son_cdma_carrier_freq_info->meas_obj_id > 12))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_cdma_carrier_freq_info->meas_obj_id);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size, p_src + *p_length_read, "cells_for_which_to_report_cgi_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi[loop], (void*)(p_src + *p_length_read), "cells_for_which_to_report_cgi[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_sys_time_info
(
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_sys_time_info, 0, sizeof(rrm_son_cdma_sys_time_info_t));

    /* This function parses rrm_son_cdma_sys_time_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_sys_time_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync, p_src + *p_length_read, "is_cdma_nw_sync");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync] should be in range [%d <= 1] !", p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_sys_time_info->cdma_sys_time_size, p_src + *p_length_read, "cdma_sys_time_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_sys_time_info->cdma_sys_time_size < 5) || (p_rrm_son_cdma_sys_time_info->cdma_sys_time_size > RRM_SON_MAX_CDMA_SYS_TIME))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_sys_time_info->cdma_sys_time_size] should be in range [5 <= %d <= RRM_SON_MAX_CDMA_SYS_TIME] !", p_rrm_son_cdma_sys_time_info->cdma_sys_time_size);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_son_cdma_sys_time_info->cdma_sys_time_size * sizeof(p_rrm_son_cdma_sys_time_info->cdma_sys_time[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_sys_time_info->cdma_sys_time_size; loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_cdma_sys_time_info->cdma_sys_time[loop], (void*)(p_src + *p_length_read), "cdma_sys_time[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_pre_reg_hrpd_info
(
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_pre_reg_hrpd_info, 0, sizeof(rrm_son_pre_reg_hrpd_info_t));

    /* This function parses rrm_son_pre_reg_hrpd_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_pre_reg_hrpd_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_pre_reg_hrpd_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed, p_src + *p_length_read, "pre_reg_allowed");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed] should be in range [%d <= 1] !", p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed);
        return RRM_FAILURE;
    }

    if (p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_pre_reg_hrpd_info->pre_reg_zone_id, p_src + *p_length_read, "pre_reg_zone_id");
    *p_length_read += sizeof(U8);
    }

    if (p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size, p_src + *p_length_read, "scndry_pre_reg_zone_id_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size < 1) || (p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size > RRM_SON_MAX_SEC_PRE_REG_ZONES))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size] should be in range [1 <= %d <= RRM_SON_MAX_SEC_PRE_REG_ZONES] !", p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)(p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size * sizeof(p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size; loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[loop], (void*)(p_src + *p_length_read), "scndry_pre_reg_zone_id_list[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_hrpd_info
(
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_hrpd_info, 0, sizeof(rrm_son_cdma_hrpd_info_t));

    /* This function parses rrm_son_cdma_hrpd_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_hrpd_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_hrpd_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_pre_reg_hrpd_info(
        &p_rrm_son_cdma_hrpd_info->hrpd_reg_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_hrpd_info->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_hrpd_info->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_hrpd_info->t_reselection] should be in range [%d <= 7] !", p_rrm_son_cdma_hrpd_info->t_reselection);
        return RRM_FAILURE;
    }

    if (p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_speed_state_scale_factors(
        &p_rrm_son_cdma_hrpd_info->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_csfb_reg_param_1XRTT
(
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_csfb_reg_param_1XRTT, 0, sizeof(rrm_son_cdma_csfb_reg_param_1XRTT_t));

    /* This function parses rrm_son_cdma_csfb_reg_param_1XRTT */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_csfb_reg_param_1XRTT");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_csfb_reg_param_1XRTT->sid, p_src + *p_length_read, "sid");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_csfb_reg_param_1XRTT->nid, p_src + *p_length_read, "nid");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid, p_src + *p_length_read, "multiple_sid");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid, p_src + *p_length_read, "multiple_nid");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg, p_src + *p_length_read, "home_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg, p_src + *p_length_read, "foreign_sid_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg, p_src + *p_length_read, "foreign_nid_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg, p_src + *p_length_read, "parameter_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg, p_src + *p_length_read, "power_up_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_period, p_src + *p_length_read, "reg_period");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_zone, p_src + *p_length_read, "reg_zone");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_csfb_reg_param_1XRTT->total_zone, p_src + *p_length_read, "total_zone");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_csfb_reg_param_1XRTT->zone_timer, p_src + *p_length_read, "zone_timer");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_1xrtt_info
(
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_1xrtt_info, 0, sizeof(rrm_son_cdma_1xrtt_info_t));

    /* This function parses rrm_son_cdma_1xrtt_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_1xrtt_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_1xrtt_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_1xrtt_info->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_1xrtt_info->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_1xrtt_info->t_reselection] should be in range [%d <= 7] !", p_rrm_son_cdma_1xrtt_info->t_reselection);
        return RRM_FAILURE;
    }

    if (p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_csfb_reg_param_1XRTT(
        &p_rrm_son_cdma_1xrtt_info->xrtt_reg_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code[loop], (void*)(p_src + *p_length_read), "cdma_1xrtt_long_code[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_speed_state_scale_factors(
        &p_rrm_son_cdma_1xrtt_info->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_csfb_reg_param_1xrtt_r9
(
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9, 0, sizeof(rrm_son_cdma_csfb_reg_param_1xrtt_r9_t));

    /* This function parses rrm_son_cdma_csfb_reg_param_1xrtt_r9 */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_csfb_reg_param_1xrtt_r9");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9, p_src + *p_length_read, "power_down_reg_r9");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9 > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_ac_barring_config_1xrtt
(
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_ac_barring_config_1xrtt, 0, sizeof(rrm_son_ac_barring_config_1xrtt_t));

    /* This function parses rrm_son_ac_barring_config_1xrtt */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_ac_barring_config_1xrtt");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_ac_barring_config_1xrtt->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9, p_src + *p_length_read, "ac_barring_0_to_9_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9 > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9] should be in range [%d <= 63] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9, p_src + *p_length_read, "ac_barring_10_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9, p_src + *p_length_read, "ac_barring_11_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9, p_src + *p_length_read, "ac_barring_12_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9, p_src + *p_length_read, "ac_barring_13_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9, p_src + *p_length_read, "ac_barring_14_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9, p_src + *p_length_read, "ac_barring_15_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9, p_src + *p_length_read, "ac_barring_msg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9, p_src + *p_length_read, "ac_barring_reg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9, p_src + *p_length_read, "ac_barring_emg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_config_cdma
(
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_cdma, 0, sizeof(rrm_son_meas_config_cdma_t));

    /* This function parses rrm_son_meas_config_cdma */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_cdma");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_cdma->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_cdma->cdma_freq_info_list_size, p_src + *p_length_read, "cdma_freq_info_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->cdma_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_cdma->cdma_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_cdma->cdma_freq_info_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_cdma->cdma_freq_info_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_cdma_carrier_freq_info(
                &p_rrm_son_meas_config_cdma->cdma_freq_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_sys_time_info(
        &p_rrm_son_meas_config_cdma->sys_time_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_cdma->srch_wnd_size, p_src + *p_length_read, "srch_wnd_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->srch_wnd_size > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_cdma->srch_wnd_size] should be in range [%d <= 15] !", p_rrm_son_meas_config_cdma->srch_wnd_size);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_hrpd_info(
        &p_rrm_son_meas_config_cdma->hrpd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_1xrtt_info(
        &p_rrm_son_meas_config_cdma->cdma_1xrtt_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9, p_src + *p_length_read, "csfb_support_for_dual_Rx_Ues_r9");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9 > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9] should be in range [%d <= 1] !", p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_csfb_reg_param_1xrtt_r9(
        &p_rrm_son_meas_config_cdma->csfb_registration_param_1xrtt_r9,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_ac_barring_config_1xrtt(
        &p_rrm_son_meas_config_cdma->ac_barring_config_1xrtt_r9,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_inter_rat_meas_config
(
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_inter_rat_meas_config, 0, sizeof(rrm_son_inter_rat_meas_config_t));

    /* This function parses rrm_son_inter_rat_meas_config */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_inter_rat_meas_config");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_utran(
        &p_rrm_son_inter_rat_meas_config->utran_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_geran(
        &p_rrm_son_inter_rat_meas_config->geran_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_son_meas_config_cdma(
        &p_rrm_son_inter_rat_meas_config->cdma_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_setup_meas_gap_config
(
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_setup_meas_gap_config, 0, sizeof(rrm_son_setup_meas_gap_config_t));

    /* This function parses rrm_son_setup_meas_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_setup_meas_gap_config");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_setup_meas_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP0_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_setup_meas_gap_config->gap_offset_gp0, p_src + *p_length_read, "gap_offset_gp0");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_setup_meas_gap_config->gap_offset_gp0 > 39))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_setup_meas_gap_config->gap_offset_gp0] should be in range [%d <= 39] !", p_rrm_son_setup_meas_gap_config->gap_offset_gp0);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP1_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_setup_meas_gap_config->gap_offset_gp1, p_src + *p_length_read, "gap_offset_gp1");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_setup_meas_gap_config->gap_offset_gp1 > 79))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_setup_meas_gap_config->gap_offset_gp1] should be in range [%d <= 79] !", p_rrm_son_setup_meas_gap_config->gap_offset_gp1);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_gap_config
(
    rrm_son_gap_config_t *p_rrm_son_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_gap_config, 0, sizeof(rrm_son_gap_config_t));

    /* This function parses rrm_son_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_gap_config");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_gap_config->bitmask & RRM_SON_SETUP_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_setup_meas_gap_config(
        &p_rrm_son_gap_config->setup_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_gap_config
(
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_gap_config, 0, sizeof(rrm_son_meas_gap_config_t));

    /* This function parses rrm_son_meas_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_gap_config");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_meas_gap_config->bitmask & RRM_SON_EUTRAN_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gap_config(
        &p_rrm_son_meas_gap_config->eutran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_gap_config->bitmask & RRM_SON_UTRAN_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gap_config(
        &p_rrm_son_meas_gap_config->utran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_gap_config->bitmask & RRM_SON_GERAN_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gap_config(
        &p_rrm_son_meas_gap_config->geran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_gap_config->bitmask & RRM_SON_CDMA2000_GAP_CONFIG_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gap_config(
        &p_rrm_son_meas_gap_config->cdma2000_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_meas_config_res
(
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_res, 0, sizeof(rrm_son_meas_config_res_t));

    /* This function parses rrm_son_meas_config_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_non_registration_based_res(
        &p_rrm_son_meas_config_res->meas_config_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_meas_config_obj_remove_req
(
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_obj_remove_req, 0, sizeof(rrm_son_meas_config_obj_remove_req_t));

    /* This function parses rrm_son_meas_config_obj_remove_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_obj_remove_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_meas_config_obj_remove_req->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size, p_src + *p_length_read, "earfcn_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list[loop], (void*)(p_src + *p_length_read), "earfcn_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size, p_src + *p_length_read, "uarfcn_fdd_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list[loop], (void*)(p_src + *p_length_read), "uarfcn_fdd_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size, p_src + *p_length_read, "uarfcn_tdd_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size; loop++)
        {
            rrm_cp_unpack_U16((void*)&p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list[loop], (void*)(p_src + *p_length_read), "uarfcn_tdd_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size, p_src + *p_length_read, "geran_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_geran_carrier_freqs(
                &p_rrm_son_meas_config_obj_remove_req->geran_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size, p_src + *p_length_read, "cdma_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_cdma_carrier_freq_info(
                &p_rrm_son_meas_config_obj_remove_req->cdma_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_meas_config_obj_remove_res
(
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_config_obj_remove_res, 0, sizeof(rrm_son_meas_config_obj_remove_res_t));

    /* This function parses rrm_son_meas_config_obj_remove_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_config_obj_remove_res");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_non_registration_based_res(
        &p_rrm_son_meas_config_obj_remove_res->meas_config_obj_remove_res,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_meas_results_ind
(
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_results_ind, 0, sizeof(rrm_son_meas_results_ind_t));

    /* This function parses rrm_son_meas_results_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_results_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_meas_results_ind->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_meas_results_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_results_ind->meas_report_event, p_src + *p_length_read, "meas_report_event");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->meas_report_event > 599))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_results_ind->meas_report_event] should be in range [%d <= 599] !", p_rrm_son_meas_results_ind->meas_report_event);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_results_ind->eutran_meas_list_size, p_src + *p_length_read, "eutran_meas_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->eutran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_results_ind->eutran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->eutran_meas_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->eutran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_eutran_meas_results(
                &p_rrm_son_meas_results_ind->eutran_meas_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_results_ind->utran_meas_list_size, p_src + *p_length_read, "utran_meas_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->utran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_results_ind->utran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->utran_meas_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->utran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_utran_meas_results(
                &p_rrm_son_meas_results_ind->utran_meas[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_results_ind->geran_meas_list_size, p_src + *p_length_read, "geran_meas_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->geran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_results_ind->geran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->geran_meas_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->geran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_geran_meas_results(
                &p_rrm_son_meas_results_ind->geran_meas[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_meas_results_ind->cdma_meas_list_size, p_src + *p_length_read, "cdma_meas_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->cdma_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_meas_results_ind->cdma_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->cdma_meas_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->cdma_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_cdma_meas_results(
                &p_rrm_son_meas_results_ind->cdma_meas[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_eutran_neighbor_cell
(
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_eutran_neighbor_cell, 0, sizeof(rrm_son_eutran_neighbor_cell_t));

    /* This function parses rrm_son_eutran_neighbor_cell */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_eutran_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_eutran_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_eutran_neighbor_cell->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->pci] should be in range [%d <= 503] !", p_rrm_son_eutran_neighbor_cell->pci);
        return RRM_FAILURE;
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CGI_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_eutran_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_eutran_neighbor_cell->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_eutran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_eutran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_eutran_neighbor_cell->num_plmn_id);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_neighbor_cell->num_plmn_id; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
                &p_rrm_son_eutran_neighbor_cell->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_TAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_eutran_neighbor_cell->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_eutran_neighbor_cell->tac); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_eutran_neighbor_cell->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_eutran_neighbor_cell->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_eutran_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_eutran_neighbor_cell->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_eutran_neighbor_cell->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->rsrp] should be in range [%d <= 128] !", p_rrm_son_eutran_neighbor_cell->rsrp);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_eutran_neighbor_cell->rsrq, p_src + *p_length_read, "rsrq");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->rsrq > 34))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->rsrq] should be in range [%d <= 34] !", p_rrm_son_eutran_neighbor_cell->rsrq);
        return RRM_FAILURE;
    }

    if (p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_eutran_neighbor_cell->csg_membershipStatus, p_src + *p_length_read, "csg_membershipStatus");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->csg_membershipStatus > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->csg_membershipStatus] should be in range [%d <= 2] !", p_rrm_son_eutran_neighbor_cell->csg_membershipStatus);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_eutran_meas_results
(
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_eutran_meas_results, 0, sizeof(rrm_son_eutran_meas_results_t));

    /* This function parses rrm_son_eutran_meas_results */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_eutran_meas_results");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_eutran_meas_results->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_eutran_meas_results->neighbor_cell_list_size, p_src + *p_length_read, "neighbor_cell_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_eutran_meas_results->neighbor_cell_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_eutran_neighbor_cell(
                &p_rrm_son_eutran_meas_results->neighbor_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_physical_cell_id
(
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_physical_cell_id, 0, sizeof(rrm_son_utran_physical_cell_id_t));

    /* This function parses rrm_son_utran_physical_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_physical_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_physical_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_FDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_utran_physical_cell_id->pci_fdd, p_src + *p_length_read, "pci_fdd");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_physical_cell_id->pci_fdd > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_physical_cell_id->pci_fdd] should be in range [%d <= 511] !", p_rrm_son_utran_physical_cell_id->pci_fdd);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_TDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_utran_physical_cell_id->pci_tdd, p_src + *p_length_read, "pci_tdd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_physical_cell_id->pci_tdd > 127))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_physical_cell_id->pci_tdd] should be in range [%d <= 127] !", p_rrm_son_utran_physical_cell_id->pci_tdd);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_geran_global_cell_id
(
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_geran_global_cell_id, 0, sizeof(rrm_son_utran_geran_global_cell_id_t));

    /* This function parses rrm_son_utran_geran_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_geran_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_geran_global_cell_id->rat_type, p_src + *p_length_read, "rat_type");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_son_utran_geran_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_son_utran_geran_global_cell_id->cell_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_geran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_utran_geran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_neighbor_cell
(
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_neighbor_cell, 0, sizeof(rrm_son_utran_neighbor_cell_t));

    /* This function parses rrm_son_utran_neighbor_cell */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_son_utran_physical_cell_id(
        &p_rrm_son_utran_neighbor_cell->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CGI_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_utran_geran_global_cell_id(
        &p_rrm_son_utran_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_LAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_utran_neighbor_cell->lac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_neighbor_cell->lac); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_utran_neighbor_cell->lac[loop], (void*)(p_src + *p_length_read), "lac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_utran_neighbor_cell->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_utran_neighbor_cell->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_utran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_utran_neighbor_cell->num_plmn_id);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_neighbor_cell->num_plmn_id; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
                &p_rrm_son_utran_neighbor_cell->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_utran_neighbor_cell->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_utran_neighbor_cell->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_utran_neighbor_cell->rscp, p_src + *p_length_read, "rscp");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_neighbor_cell->rscp < -5) || (p_rrm_son_utran_neighbor_cell->rscp > 91))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->rscp] should be in range [-5 <= %d <= 91] !", p_rrm_son_utran_neighbor_cell->rscp);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_utran_neighbor_cell->ecno, p_src + *p_length_read, "ecno");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_neighbor_cell->ecno > 49))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->ecno] should be in range [%d <= 49] !", p_rrm_son_utran_neighbor_cell->ecno);
        return RRM_FAILURE;
    }

    if (p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_neighbor_cell->csg_membershipStatus, p_src + *p_length_read, "csg_membershipStatus");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_neighbor_cell->csg_membershipStatus > 0))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->csg_membershipStatus] should be in range [%d <= 0] !", p_rrm_son_utran_neighbor_cell->csg_membershipStatus);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_meas_results
(
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_meas_results, 0, sizeof(rrm_son_utran_meas_results_t));

    /* This function parses rrm_son_utran_meas_results */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_meas_results");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_utran_meas_results->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_meas_results->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_meas_results->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_utran_meas_results->uarfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_utran_meas_results->neighbor_cell_list_size, p_src + *p_length_read, "neighbor_cell_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_utran_meas_results->neighbor_cell_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_utran_neighbor_cell(
                &p_rrm_son_utran_meas_results->neighbor_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_neighbor_cell
(
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_neighbor_cell, 0, sizeof(rrm_son_geran_neighbor_cell_t));

    /* This function parses rrm_son_geran_neighbor_cell */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_geran_neighbor_cell->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->arfcn] should be in range [%d <= 1023] !", p_rrm_son_geran_neighbor_cell->arfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_neighbor_cell->band_ind, p_src + *p_length_read, "band_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->band_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->band_ind] should be in range [%d <= 1] !", p_rrm_son_geran_neighbor_cell->band_ind);
        return RRM_FAILURE;
    }

    
    if (RRM_FAILURE == rrm_parse_rrm_son_geran_physical_cell_id(
        &p_rrm_son_geran_neighbor_cell->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_utran_geran_global_cell_id(
        &p_rrm_son_geran_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_neighbor_cell->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_neighbor_cell->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->rssi > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->rssi] should be in range [%d <= 63] !", p_rrm_son_geran_neighbor_cell->rssi);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_meas_results
(
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_meas_results, 0, sizeof(rrm_son_geran_meas_results_t));

    /* This function parses rrm_son_geran_meas_results */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_meas_results");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_meas_results->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_geran_meas_results->bitmask & RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_meas_results->meas_obj_id, p_src + *p_length_read, "meas_obj_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_meas_results->meas_obj_id < 1) || (p_rrm_son_geran_meas_results->meas_obj_id > 12))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_meas_results->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_geran_meas_results->meas_obj_id);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_meas_results->neighbor_cell_list_size, p_src + *p_length_read, "neighbor_cell_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_geran_meas_results->neighbor_cell_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_geran_neighbor_cell(
                &p_rrm_son_geran_meas_results->neighbor_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_global_cell_id
(
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_global_cell_id, 0, sizeof(rrm_son_cdma_global_cell_id_t));

    /* This function parses rrm_son_cdma_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_1X_RTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT[loop], (void*)(p_src + *p_length_read), "cell_global_id_1X_RTT[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_HRPD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD[loop], (void*)(p_src + *p_length_read), "cell_global_id_HRPD[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_neighbor_cell
(
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_neighbor_cell, 0, sizeof(rrm_son_cdma_neighbor_cell_t));

    /* This function parses rrm_son_cdma_neighbor_cell */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_neighbor_cell->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->arfcn] should be in range [%d <= 1023] !", p_rrm_son_cdma_neighbor_cell->arfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_neighbor_cell->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->band_class] should be in range [%d <= 31] !", p_rrm_son_cdma_neighbor_cell->band_class);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_neighbor_cell->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    if (p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_cdma_global_cell_id(
        &p_rrm_son_cdma_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_cdma_neighbor_cell->pilot_pn_phase, p_src + *p_length_read, "pilot_pn_phase");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->pilot_pn_phase > 32767))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->pilot_pn_phase] should be in range [%d <= 32767] !", p_rrm_son_cdma_neighbor_cell->pilot_pn_phase);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_neighbor_cell->pilot_strength, p_src + *p_length_read, "pilot_strength");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->pilot_strength > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->pilot_strength] should be in range [%d <= 63] !", p_rrm_son_cdma_neighbor_cell->pilot_strength);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cdma_meas_results
(
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cdma_meas_results, 0, sizeof(rrm_son_cdma_meas_results_t));

    /* This function parses rrm_son_cdma_meas_results */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cdma_meas_results");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cdma_meas_results->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_cdma_meas_results->bitmask & RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_meas_results->meas_obj_id, p_src + *p_length_read, "meas_obj_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_meas_results->meas_obj_id < 1) || (p_rrm_son_cdma_meas_results->meas_obj_id > 12))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_meas_results->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_cdma_meas_results->meas_obj_id);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_cdma_meas_results->neighbor_cell_list_size, p_src + *p_length_read, "neighbor_cell_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_cdma_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_cdma_meas_results->neighbor_cell_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_cdma_neighbor_cell(
                &p_rrm_son_cdma_meas_results->neighbor_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_ho_report
(
    rrm_son_ho_report_t *p_rrm_son_ho_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_ho_report, 0, sizeof(rrm_son_ho_report_t));

    /* This function parses rrm_son_ho_report */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_ho_report");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_ho_report->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_report->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_global_cell_id(
        &p_rrm_son_ho_report->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_ho_report->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ho_report->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ho_report->result] should be in range [%d <= 1] !", p_rrm_son_ho_report->result);
        return RRM_FAILURE;
    }

    if (p_rrm_son_ho_report->bitmask & HO_REPORT_RE_EST_CGI_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_global_cell_id(
        &p_rrm_son_ho_report->restablishment_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_ho_report->bitmask & HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_eutran_strength(
        &p_rrm_son_ho_report->serving_meas_strength,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_ho_report->bitmask & HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_meas_strength(
        &p_rrm_son_ho_report->target_meas_strength,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_ho_report->bitmask & HO_REPORT_OFFSET_ATTR_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_offset_attr(
        &p_rrm_son_ho_report->offset_attr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_utran_geran_global_cell_id
(
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_utran_geran_global_cell_id, 0, sizeof(rrm_utran_geran_global_cell_id_t));

    /* This function parses rrm_utran_geran_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_utran_geran_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_utran_geran_global_cell_id->rat_type, p_src + *p_length_read, "rat_type");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_utran_geran_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_utran_geran_global_cell_id->cell_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_utran_geran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_utran_geran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_others_global_cell_id
(
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_others_global_cell_id, 0, sizeof(rrm_others_global_cell_id_t));

    /* This function parses rrm_others_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_others_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_others_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_inter_rat_global_cell_id
(
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_inter_rat_global_cell_id, 0, sizeof(rrm_inter_rat_global_cell_id_t));

    /* This function parses rrm_inter_rat_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_inter_rat_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_inter_rat_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_inter_rat_global_cell_id->bitmask & RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_utran_geran_global_cell_id(
        &p_rrm_inter_rat_global_cell_id->utran_geran_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_inter_rat_global_cell_id->bitmask & RRM_OTHERS_GLOBAL_CELL_ID_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_others_global_cell_id(
        &p_rrm_inter_rat_global_cell_id->others_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_global_cell_id
(
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_oam_global_cell_id, 0, sizeof(rrm_oam_global_cell_id_t));

    /* This function parses rrm_oam_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_oam_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_oam_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_oam_global_cell_id->bitmask & RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_oam_global_cell_id->intra_rat_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_global_cell_id->bitmask & RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_inter_rat_global_cell_id(
        &p_rrm_oam_global_cell_id->inter_rat_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_offset_attr
(
    rrm_son_offset_attr_t *p_rrm_son_offset_attr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_offset_attr, 0, sizeof(rrm_son_offset_attr_t));

    /* This function parses rrm_son_offset_attr */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_offset_attr");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_offset_attr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_offset_attr->ofs, p_src + *p_length_read, "ofs");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ofs < -24) || (p_rrm_son_offset_attr->ofs > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_offset_attr->ofs] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ofs);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OCS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_offset_attr->ocs, p_src + *p_length_read, "ocs");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ocs < -24) || (p_rrm_son_offset_attr->ocs > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_offset_attr->ocs] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ocs);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_offset_attr->ofn, p_src + *p_length_read, "ofn");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ofn < -24) || (p_rrm_son_offset_attr->ofn > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_offset_attr->ofn] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ofn);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFF_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_offset_attr->off, p_src + *p_length_read, "off");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->off < -24) || (p_rrm_son_offset_attr->off > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_offset_attr->off] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->off);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_HYS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_offset_attr->hys, p_src + *p_length_read, "hys");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_offset_attr->hys > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_offset_attr->hys] should be in range [%d <= 30] !", p_rrm_son_offset_attr->hys);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_eutran_strength
(
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_eutran_strength, 0, sizeof(rrm_son_eutran_strength_t));

    /* This function parses rrm_son_eutran_strength */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_eutran_strength");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_eutran_strength->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_eutran_strength->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_strength->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_strength->rsrp] should be in range [%d <= 128] !", p_rrm_son_eutran_strength->rsrp);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_eutran_strength->rsrq, p_src + *p_length_read, "rsrq");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_strength->rsrq > 34))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eutran_strength->rsrq] should be in range [%d <= 34] !", p_rrm_son_eutran_strength->rsrq);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_offset_attr(
        &p_rrm_son_eutran_strength->offset_attr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_utran_strength
(
    rrm_son_utran_strength_t *p_rrm_son_utran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_utran_strength, 0, sizeof(rrm_son_utran_strength_t));

    /* This function parses rrm_son_utran_strength */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_utran_strength");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_utran_strength->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_rrm_son_utran_strength->rscp, p_src + *p_length_read, "rscp");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_strength->rscp < -5) || (p_rrm_son_utran_strength->rscp > 91))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_strength->rscp] should be in range [-5 <= %d <= 91] !", p_rrm_son_utran_strength->rscp);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_ECNO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_utran_strength->ecno, p_src + *p_length_read, "ecno");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_strength->ecno > 49))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_utran_strength->ecno] should be in range [%d <= 49] !", p_rrm_son_utran_strength->ecno);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_geran_strength
(
    rrm_son_geran_strength_t *p_rrm_son_geran_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_geran_strength, 0, sizeof(rrm_son_geran_strength_t));

    /* This function parses rrm_son_geran_strength */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_geran_strength");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_geran_strength->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_geran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_geran_strength->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_strength->rssi > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_geran_strength->rssi] should be in range [%d <= 63] !", p_rrm_son_geran_strength->rssi);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_meas_strength
(
    rrm_son_meas_strength_t *p_rrm_son_meas_strength,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_meas_strength, 0, sizeof(rrm_son_meas_strength_t));

    /* This function parses rrm_son_meas_strength */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_meas_strength");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_meas_strength->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_meas_strength->bitmask & RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_eutran_strength(
        &p_rrm_son_meas_strength->eutran_cell_strength,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_strength->bitmask & RRM_SON_UTRAN_CELL_STRENGTH_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_utran_strength(
        &p_rrm_son_meas_strength->utran_cell_strength,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_meas_strength->bitmask & RRM_SON_GERAN_CELL_STRENGTH_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_geran_strength(
        &p_rrm_son_meas_strength->geran_cell_strength,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_load_report_ind
(
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_load_report_ind, 0, sizeof(rrm_son_load_report_ind_t));

    /* This function parses rrm_son_load_report_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_load_report_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_cell_load_info(
        &p_rrm_son_load_report_ind->serv_cell_load_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_load_report_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_load_report_ind->count > RRM_MAX_NUM_NGBR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_load_report_ind->count] should be in range [%d <= RRM_MAX_NUM_NGBR_CELLS] !", p_rrm_son_load_report_ind->count);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_load_report_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_son_cell_load_info(
                &p_rrm_son_load_report_ind->neigh_cell_load_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_hw_load_ind
(
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_hw_load_ind, 0, sizeof(rrm_son_hw_load_ind_t));

    /* This function parses rrm_son_hw_load_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_hw_load_ind");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_hw_load_ind->dl, p_src + *p_length_read, "dl");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_hw_load_ind->ul, p_src + *p_length_read, "ul");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_s1_tnl_load_ind
(
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_s1_tnl_load_ind, 0, sizeof(rrm_son_s1_tnl_load_ind_t));

    /* This function parses rrm_son_s1_tnl_load_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_s1_tnl_load_ind");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_s1_tnl_load_ind->dl, p_src + *p_length_read, "dl");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_s1_tnl_load_ind->ul, p_src + *p_length_read, "ul");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_rrs_load_ind
(
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_rrs_load_ind, 0, sizeof(rrm_son_rrs_load_ind_t));

    /* This function parses rrm_son_rrs_load_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_rrs_load_ind");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->dl_gbr_prb_usage, p_src + *p_length_read, "dl_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_gbr_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_gbr_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->ul_gbr_prb_usage, p_src + *p_length_read, "ul_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_gbr_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_gbr_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage, p_src + *p_length_read, "dl_non_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage, p_src + *p_length_read, "ul_non_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->dl_total_prb_usage, p_src + *p_length_read, "dl_total_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_total_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_total_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_total_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rrs_load_ind->ul_total_prb_usage, p_src + *p_length_read, "ul_total_prb_usage");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_total_prb_usage > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_total_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_total_prb_usage);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_comp_avl_cap
(
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_comp_avl_cap, 0, sizeof(rrm_son_comp_avl_cap_t));

    /* This function parses rrm_son_comp_avl_cap */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_comp_avl_cap");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_comp_avl_cap->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_son_comp_avl_cap->bitmask & RRM_SON_CELL_CAP_CLASS_VAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_comp_avl_cap->cell_cap_class_val, p_src + *p_length_read, "cell_cap_class_val");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_comp_avl_cap->cell_cap_class_val < 1) || (p_rrm_son_comp_avl_cap->cell_cap_class_val > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_comp_avl_cap->cell_cap_class_val] should be in range [1 <= %d <= 100] !", p_rrm_son_comp_avl_cap->cell_cap_class_val);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_comp_avl_cap->cap_val, p_src + *p_length_read, "cap_val");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_comp_avl_cap->cap_val > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_comp_avl_cap->cap_val] should be in range [%d <= 100] !", p_rrm_son_comp_avl_cap->cap_val);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_comp_avl_cap_grp
(
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_comp_avl_cap_grp, 0, sizeof(rrm_son_comp_avl_cap_grp_t));

    /* This function parses rrm_son_comp_avl_cap_grp */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_comp_avl_cap_grp");


    
    if (RRM_FAILURE == rrm_parse_rrm_son_comp_avl_cap(
        &p_rrm_son_comp_avl_cap_grp->dl_comp_avl_cap,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_son_comp_avl_cap(
        &p_rrm_son_comp_avl_cap_grp->ul_comp_avl_cap,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_abs_status
(
    rrm_son_abs_status_t *p_rrm_son_abs_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_abs_status, 0, sizeof(rrm_son_abs_status_t));

    /* This function parses rrm_son_abs_status */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_abs_status");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_abs_status->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_abs_status->dl_abs_status, p_src + *p_length_read, "dl_abs_status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_abs_status->dl_abs_status > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_abs_status->dl_abs_status] should be in range [%d <= 100] !", p_rrm_son_abs_status->dl_abs_status);
        return RRM_FAILURE;
    }

    if (p_rrm_son_abs_status->bitmask & RRM_SON_TDD_FDD_CONFIG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_abs_status->tdd_fdd_config, p_src + *p_length_read, "tdd_fdd_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_abs_status->tdd_fdd_config > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_abs_status->tdd_fdd_config] should be in range [%d <= 3] !", p_rrm_son_abs_status->tdd_fdd_config);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(p_rrm_son_abs_status->usable_abs_pattern_info_tdd) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_abs_status->usable_abs_pattern_info_tdd); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_abs_status->usable_abs_pattern_info_tdd[loop], (void*)(p_src + *p_length_read), "usable_abs_pattern_info_tdd[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_cell_load_info
(
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_cell_load_info, 0, sizeof(rrm_son_cell_load_info_t));

    /* This function parses rrm_son_cell_load_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_cell_load_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_cell_load_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_cell_load_info->g_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_hw_load_ind(
        &p_rrm_son_cell_load_info->hw_load,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_s1_tnl_load_ind(
        &p_rrm_son_cell_load_info->s1_tnl_load,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_RRS_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_rrs_load_ind(
        &p_rrm_son_cell_load_info->rrs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cell_load_info->bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_comp_avl_cap_grp(
        &p_rrm_son_cell_load_info->comp_avl_cap_grp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_cell_load_info->bitmask & RRM_SON_ABS_STATUS_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_abs_status(
        &p_rrm_son_cell_load_info->abs_status,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_ho_attempt_ind
(
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_ho_attempt_ind, 0, sizeof(rrm_son_ho_attempt_ind_t));

    /* This function parses rrm_son_ho_attempt_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_ho_attempt_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_attempt_ind->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_attempt_ind->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_ho_attempt_ind->attempt_count, p_src + *p_length_read, "attempt_count");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_ho_failure_report
(
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_ho_failure_report, 0, sizeof(rrm_son_ho_failure_report_t));

    /* This function parses rrm_son_ho_failure_report */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_ho_failure_report");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_ho_failure_report->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_failure_report->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_failure_report->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_ho_failure_report->bitmask & HO_FAILURE_REPORT_RE_EST_CGI_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_ho_failure_report->restablishment_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_ho_failure_report->ho_cause, p_src + *p_length_read, "ho_cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ho_failure_report->ho_cause > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_ho_failure_report->ho_cause] should be in range [%d <= 2] !", p_rrm_son_ho_failure_report->ho_cause);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_tnl_discovery_req
(
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_tnl_discovery_req, 0, sizeof(rrm_son_tnl_discovery_req_t));

    /* This function parses rrm_son_tnl_discovery_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_tnl_discovery_req");


    
    if (RRM_FAILURE == rrm_parse_rrm_global_enb_id(
        &p_rrm_son_tnl_discovery_req->target_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_son_tnl_discovery_req->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_tnl_discovery_req->tac); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_son_tnl_discovery_req->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_global_enb_id
(
    rrm_global_enb_id_t *p_rrm_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_global_enb_id, 0, sizeof(rrm_global_enb_id_t));

    /* This function parses rrm_global_enb_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_global_enb_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_global_enb_id->enb_type, p_src + *p_length_read, "enb_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_global_enb_id->enb_type < 1) || (p_rrm_global_enb_id->enb_type > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_global_enb_id->enb_type] should be in range [1 <= %d <= 2] !", p_rrm_global_enb_id->enb_type);
        return RRM_FAILURE;
    }

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_global_enb_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_global_enb_id->eNb_id) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_global_enb_id->eNb_id); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_global_enb_id->eNb_id[loop], (void*)(p_src + *p_length_read), "eNb_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_tnl_discovery_res
(
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_tnl_discovery_res, 0, sizeof(rrm_son_tnl_discovery_res_t));

    /* This function parses rrm_son_tnl_discovery_res */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_tnl_discovery_res");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_tnl_discovery_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_tnl_discovery_res->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_tnl_discovery_res->result] should be in range [%d <= 1] !", p_rrm_son_tnl_discovery_res->result);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_tnl_discovery_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_global_enb_id(
        &p_rrm_son_tnl_discovery_res->target_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size, p_src + *p_length_read, "enb_tnl_address_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size > MAX_NO_ENB_X2_TLAS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size] should be in range [%d <= MAX_NO_ENB_X2_TLAS] !", p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_enb_tnl_address(
                &p_rrm_son_tnl_discovery_res->enb_tnl_address_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_enb_tnl_address
(
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_enb_tnl_address, 0, sizeof(rrm_enb_tnl_address_t));

    /* This function parses rrm_enb_tnl_address */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_enb_tnl_address");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_enb_tnl_address->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_rrm_enb_tnl_address->bitmask & RRM_IPV4_ADD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_enb_tnl_address->ipv4_add) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_enb_tnl_address->ipv4_add); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_enb_tnl_address->ipv4_add[loop], (void*)(p_src + *p_length_read), "ipv4_add[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_enb_tnl_address->bitmask & RRM_IPV6_ADD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_rrm_enb_tnl_address->ipv6_add) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_enb_tnl_address->ipv6_add); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_enb_tnl_address->ipv6_add[loop], (void*)(p_src + *p_length_read), "ipv6_add[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_rach_config_req
(
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_rach_config_req, 0, sizeof(rrm_son_rach_config_req_t));

    /* This function parses rrm_son_rach_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_rach_config_req");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_rach_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_rach_config_req->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_rach_config_req->bitmask & L2_REPORT_PERIODICITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_rach_config_req->l2_reports_periodicity, p_src + *p_length_read, "l2_reports_periodicity");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->l2_reports_periodicity > 600))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rach_config_req->l2_reports_periodicity] should be in range [%d <= 600] !", p_rrm_son_rach_config_req->l2_reports_periodicity);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_rach_config_req->bitmask & UE_REPORT_PERIODICITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_rach_config_req->ue_reports_periodicity, p_src + *p_length_read, "ue_reports_periodicity");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->ue_reports_periodicity > 600))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rach_config_req->ue_reports_periodicity] should be in range [%d <= 600] !", p_rrm_son_rach_config_req->ue_reports_periodicity);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_rach_config_req->bitmask & RACH_SAMPLE_COUNT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_rach_config_req->rach_sample_count, p_src + *p_length_read, "rach_sample_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->rach_sample_count > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rach_config_req->rach_sample_count] should be in range [%d <= 1000] !", p_rrm_son_rach_config_req->rach_sample_count);
        return RRM_FAILURE;
    }
    }

    if (p_rrm_son_rach_config_req->bitmask & NUM_STRONGEST_CELLS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_rach_config_req->num_of_strongest_cells, p_src + *p_length_read, "num_of_strongest_cells");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->num_of_strongest_cells > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rach_config_req->num_of_strongest_cells] should be in range [%d <= 3] !", p_rrm_son_rach_config_req->num_of_strongest_cells);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_rach_config_resp
(
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_rach_config_resp, 0, sizeof(rrm_son_rach_config_resp_t));

    /* This function parses rrm_son_rach_config_resp */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_rach_config_resp");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_rach_config_resp->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_rach_config_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_rach_config_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_rach_ue_info_ind
(
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_rach_ue_info_ind, 0, sizeof(rrm_son_rach_ue_info_ind_t));

    /* This function parses rrm_son_rach_ue_info_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_rach_ue_info_ind");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_rach_ue_info_ind->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_rach_ue_info_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_ue_info_ind->count > MAX_NUMBER_OF_PRACH_SAMPLES))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_rach_ue_info_ind->count] should be in range [%d <= MAX_NUMBER_OF_PRACH_SAMPLES] !", p_rrm_son_rach_ue_info_ind->count);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_rach_ue_info_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_ue_prach_data_info(
                &p_rrm_son_rach_ue_info_ind->rach_data[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_strong_cell_info
(
    strong_cell_info_t *p_strong_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_strong_cell_info, 0, sizeof(strong_cell_info_t));

    /* This function parses strong_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing strong_cell_info");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_strong_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_strong_cell_info->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_strong_cell_info->pci] should be in range [%d <= 503] !", p_strong_cell_info->pci);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_strong_cell_info->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_strong_cell_info->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_strong_cell_info->rsrp] should be in range [%d <= 128] !", p_strong_cell_info->rsrp);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_ue_prach_data_info
(
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_ue_prach_data_info, 0, sizeof(rrm_ue_prach_data_info_t));

    /* This function parses rrm_ue_prach_data_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_ue_prach_data_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_ue_prach_data_info->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_ue_prach_data_info->cause < 1) || (p_rrm_ue_prach_data_info->cause > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_ue_prach_data_info->cause] should be in range [1 <= %d <= 3] !", p_rrm_ue_prach_data_info->cause);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_ue_prach_data_info->number_of_preambles_sent, p_src + *p_length_read, "number_of_preambles_sent");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_ue_prach_data_info->contention_detected, p_src + *p_length_read, "contention_detected");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_ue_prach_data_info->ue_location, p_src + *p_length_read, "ue_location");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_ue_prach_data_info->ue_location > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_ue_prach_data_info->ue_location] should be in range [%d <= 1] !", p_rrm_ue_prach_data_info->ue_location);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_ue_prach_data_info->strong_cell_info_count, p_src + *p_length_read, "strong_cell_info_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_ue_prach_data_info->strong_cell_info_count > STRONG_CELL_INFO_COUNT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_ue_prach_data_info->strong_cell_info_count] should be in range [%d <= STRONG_CELL_INFO_COUNT] !", p_rrm_ue_prach_data_info->strong_cell_info_count);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_ue_prach_data_info->strong_cell_info_count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_strong_cell_info(
                &p_rrm_ue_prach_data_info->strong_cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_l2_rach_perf_report
(
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_l2_rach_perf_report, 0, sizeof(rrm_son_l2_rach_perf_report_t));

    /* This function parses rrm_son_l2_rach_perf_report */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_l2_rach_perf_report");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_l2_rach_perf_report->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_rrm_son_l2_rach_perf_report->num_of_l2_perf_accumulated_reports, p_src + *p_length_read, "num_of_l2_perf_accumulated_reports");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cbra, p_src + *p_length_read, "total_rcvd_msg3_random_access_preambles_cbra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cfra, p_src + *p_length_read, "total_rcvd_msg3_random_access_preambles_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cbra, p_src + *p_length_read, "total_failed_random_access_preambles_msg2_cbra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cfra, p_src + *p_length_read, "total_failed_random_access_preambles_msg2_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_allocated_random_access_preambles, p_src + *p_length_read, "total_allocated_random_access_preambles");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta); loop++)
        {
            rrm_cp_unpack_U64((void*)&p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta[loop], (void*)(p_src + *p_length_read), "total_succ_random_access_ta[]");
            *p_length_read += sizeof(U64);
        }
    }

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_unassigned_false_cfra, p_src + *p_length_read, "total_unassigned_false_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->total_unassigned_sum_cfra, p_src + *p_length_read, "total_unassigned_sum_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_a, p_src + *p_length_read, "num_random_access_preambles_group_a");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U64(&p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_b, p_src + *p_length_read, "num_random_access_preambles_group_b");
    *p_length_read += sizeof(U64);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_enb_config_update_ind
(
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_enb_config_update_ind, 0, sizeof(rrm_son_enb_config_update_ind_t));

    /* This function parses rrm_son_enb_config_update_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_enb_config_update_ind");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_enb_config_update_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_global_enb_id(
        &p_rrm_son_enb_config_update_ind->nbr_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gu_group_id_list(
        &p_rrm_son_enb_config_update_ind->add_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_son_gu_group_id_list(
        &p_rrm_son_enb_config_update_ind->delete_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_gu_group_id
(
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_oam_gu_group_id, 0, sizeof(rrm_oam_gu_group_id_t));

    /* This function parses rrm_oam_gu_group_id */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_oam_gu_group_id");


    
    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_oam_gu_group_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_oam_gu_group_id->mme_group_id) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_gu_group_id->mme_group_id); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_oam_gu_group_id->mme_group_id[loop], (void*)(p_src + *p_length_read), "mme_group_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_son_gu_group_id_list
(
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_gu_group_id_list, 0, sizeof(rrm_son_gu_group_id_list_t));

    /* This function parses rrm_son_gu_group_id_list */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_gu_group_id_list");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_gu_group_id_list->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_gu_group_id_list->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_gu_group_id_list->num_gu_group_id < 1) || (p_rrm_son_gu_group_id_list->num_gu_group_id > RRM_MME_MAX_POOLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= RRM_MME_MAX_POOLS] !", p_rrm_son_gu_group_id_list->num_gu_group_id);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_oam_gu_group_id(
                &p_rrm_son_gu_group_id_list->gu_id_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_eicic_enable_disable_ind
(
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_eicic_enable_disable_ind, 0, sizeof(rrm_son_eicic_enable_disable_ind_t));

    /* This function parses rrm_son_eicic_enable_disable_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_eicic_enable_disable_ind");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_eicic_enable_disable_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_son_eicic_enable_disable_ind->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type, p_src + *p_length_read, "eicic_provisioned_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type] should be in range [%d <= 3] !", p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_rrm_son_mbms_sf_info_ind
(
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_son_mbms_sf_info_ind, 0, sizeof(rrm_son_mbms_sf_info_ind_t));

    /* This function parses rrm_son_mbms_sf_info_ind */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_son_mbms_sf_info_ind");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_son_mbms_sf_info_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_mbms_sf_info_ind->count < 1) || (p_rrm_son_mbms_sf_info_ind->count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_son_mbms_sf_info_ind->count] should be in range [1 <= %d <= 8] !", p_rrm_son_mbms_sf_info_ind->count);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_mbms_sf_info_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_cell_sib2_mbsfn_data(
                &p_rrm_son_mbms_sf_info_ind->cell_sib2_mbsfn_data[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_subframe_allocation
(
    rrm_subframe_allocation_t *p_rrm_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_rrm_subframe_allocation, 0, sizeof(rrm_subframe_allocation_t));

    /* This function parses rrm_subframe_allocation */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_subframe_allocation");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_subframe_allocation->sf_alloc_choice, p_src + *p_length_read, "sf_alloc_choice");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_subframe_allocation->sf_alloc_choice > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_subframe_allocation->sf_alloc_choice] should be in range [%d <= 1] !", p_rrm_subframe_allocation->sf_alloc_choice);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_rrm_subframe_allocation->subframe_allocation) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_subframe_allocation->subframe_allocation); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_rrm_subframe_allocation->subframe_allocation[loop], (void*)(p_src + *p_length_read), "subframe_allocation[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_mbsfn_config_info
(
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_mbsfn_config_info, 0, sizeof(rrm_mbsfn_config_info_t));

    /* This function parses rrm_mbsfn_config_info */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_mbsfn_config_info");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_mbsfn_config_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_mbsfn_config_info->radio_frame_allocation_period, p_src + *p_length_read, "radio_frame_allocation_period");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_mbsfn_config_info->radio_frame_allocation_period > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_mbsfn_config_info->radio_frame_allocation_period] should be in range [%d <= 5] !", p_rrm_mbsfn_config_info->radio_frame_allocation_period);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_mbsfn_config_info->radio_frame_allocation_offset, p_src + *p_length_read, "radio_frame_allocation_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_mbsfn_config_info->radio_frame_allocation_offset > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_mbsfn_config_info->radio_frame_allocation_offset] should be in range [%d <= 7] !", p_rrm_mbsfn_config_info->radio_frame_allocation_offset);
        return RRM_FAILURE;
    }

    
    if (RRM_FAILURE == rrm_parse_rrm_subframe_allocation(
        &p_rrm_mbsfn_config_info->subframe_allocation,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_mbsfn_config_info->bitmask & RRM_OAM_SYNC_STRATUM_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_mbsfn_config_info->sync_stratum_id, p_src + *p_length_read, "sync_stratum_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_mbsfn_config_info->sync_stratum_id < 1) || (p_rrm_mbsfn_config_info->sync_stratum_id > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_mbsfn_config_info->sync_stratum_id] should be in range [1 <= %d <= 8] !", p_rrm_mbsfn_config_info->sync_stratum_id);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_mbsfn_subframe_config_list
(
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_mbsfn_subframe_config_list, 0, sizeof(rrm_mbsfn_subframe_config_list_t));

    /* This function parses rrm_mbsfn_subframe_config_list */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_mbsfn_subframe_config_list");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_mbsfn_subframe_config_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_mbsfn_subframe_config_list->count < 1) || (p_rrm_mbsfn_subframe_config_list->count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Parameter [p_rrm_mbsfn_subframe_config_list->count] should be in range [1 <= %d <= 8] !", p_rrm_mbsfn_subframe_config_list->count);
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
    
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_mbsfn_subframe_config_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_mbsfn_config_info(
                &p_rrm_mbsfn_subframe_config_list->mbsfn_subframe_config_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_cell_sib2_mbsfn_data
(
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_rrm_cell_sib2_mbsfn_data, 0, sizeof(rrm_cell_sib2_mbsfn_data_t));

    /* This function parses rrm_cell_sib2_mbsfn_data */
    SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_COMP_PARSE, "Parsing rrm_cell_sib2_mbsfn_data");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_cell_sib2_mbsfn_data->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    
    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_cell_sib2_mbsfn_data->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_cell_sib2_mbsfn_data->bitmask & RRM_SON_MBMS_CONFIG_APPLY_PRESENT)
    {

    
    if (RRM_FAILURE == rrm_parse_rrm_mbsfn_subframe_config_list(
        &p_rrm_cell_sib2_mbsfn_data->rrm_mbsfn_subframe_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_log_facility_name, SON_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

