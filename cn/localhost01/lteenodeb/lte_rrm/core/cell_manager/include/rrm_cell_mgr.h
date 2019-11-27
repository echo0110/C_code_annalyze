/*! \file rrm_cell_mgr.h
 *  \brief This header file contains declaration of function in points of CELL.
 *  \date January 20, 2012
 *  \author Gur29220
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
/*******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 * 1 May, 2014      gur32417     CA STG 3        Carrier Aggregation Stage 3
 *                                               code changes
 * 2 July, 2014    gur35095      Bug-fix11907    Broadcast cell_del_req to all cells
 *                                               after resp from all cells, Send 
 *                                               cell_del_req to rrc 
 ******************************************************************************/

#ifndef _RRM_CELL_MGR_H_
/* ! \def _RRM_CELL_MGR_H_
 */
#define _RRM_CELL_MGR_H_

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"

/*! \headerfile rrm_cell_fsm.h <>
 */
#include "rrm_cell_fsm.h"

/*! \headerfile rrm_utils.h <>
 */
#include "rrm_utils.h"

/*! \headerfile rrm_cell_context.h <>
 */
#include "rrm_cell_context.h"

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \headerfile rrm_mif_db.h <> */
#include "rrm_mif_db.h"

/* SRS_FREQ_HOPPING_START */
/*! \headerfile rrm_ue_context.h <> */
#include "rrm_ue_context.h"
/* SRS_FREQ_HOPPING_END */


/*SPR 17777 +-*/

/*SPR_12535_FIX_START*/
#ifdef TDD_MODE_FLAG

#define MAX_SUB_FRAME_ASSIGNMENTS_COUNT 7
#define MAX_SPECIAL_SUBFRAME_PATTERNS_COUNT 9

typedef struct
{
    U8 num_dl_schedulable;
    U8 num_ul_schedulable;
}rrm_prb_usage_factor_t;
#endif
/*SPR_12535_FIX_END*/

/*! \def  p_g_rrm_cell_ctx
 *  \brief RRM global context
 */
extern rrm_global_context_t    *p_g_rrm_cell_ctx;
/**CLPC_MR_START*/
/*! \fn rrm_ue_tpc_rnti_tbl_pusch_t *rrm_cellm_get_tpc_rnti_allocation_table_pusch(rrm_cell_index_t cell_index)
 *  \brief This function is used for getting TPC Rnti allocation table PUSCH information
 *  \param cell_index Cell Index
 */
rrm_ue_tpc_rnti_tbl_pusch_t*
rrm_cellm_get_tpc_rnti_allocation_table_pusch
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_get_tpc_rnti_allocation_table_pucch(rrm_cell_index_t cell_index)
 *  \brief returns rnti table for pucch
 *  \param cell_index cell index
 */
/*EICIC_TDD_CHANGES_START*/
U8
rrm_find_abs_frame_size(tdd_frame_structure_t *p_physical_layer_param_tdd_frame_structure);
/*EICIC_TDD_CHANGES_END*/

rrm_ue_tpc_rnti_tbl_pucch_t*
rrm_cellm_get_tpc_rnti_allocation_table_pucch
(
 rrm_cell_index_t cell_index
 );
/**CLPC_MR_END*/

/*! \fn rrm_return_et rrm_rrc_cell_setup_resp_process_msg (U8 *p_api, U16 api_id, U16 data_len, U16 trans_id)
 *  \brief handle cell setup response msg from RRC
 *  \param p_api    pointer to input API buffer
 *  \param api_id   api id
 *  \param data_len Data length
 *  \param trans_id TRANSACTION id
 */
rrm_return_et 
rrm_rrc_cell_setup_resp_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id);

/* PRS CHANGES START */
/*! \fn rrm_return_et rrm_rrc_lppa_otdoa_info_req_process_msg(U8 *p_api, U16 api_id, U16 data_len, U16 trans_id)
 *  \brief handle lppa_otdoa_info_req  msg from RRC
 *  \param p_api    pointer to input API buffer
 *  \param api_id   api id
 *  \param data_len Data length
 *  \param trans_id TRANSACTION id
 */
rrm_return_et
rrm_rrc_lppa_otdoa_info_req_process_msg
(
 U8      *p_api, /* Pointer to incoming Api buffer*/
 U16     api_id, /* incoming api id*/
 U16     data_len, /* incoming data length*/
 U16     trans_id /* Incoming transaction Id*/
 );
/*EICIC_TDD_CHANGES_START*/
rrm_return_et
rrm_validate_abs_pattern_for_tdd
(
U8   *abs_pattern,
U8    abs_size
);
/*EICIC_TDD_CHANGES_END*/
/*! \fn fill_otdoa_information_from_cellm 
 *  \brief fill otdoa requested information from RRC
 *  \param cell_index   cell index for which information have to fetch 
 *  \param otdoa_info_item information for which information need to fill up 
 *  \param rrm_s1ap_lppa_otdoa_info_resp message in which information need to fill
 *  \param trans_id TRANSACTION id
 */
rrm_return_et
fill_otdoa_information_from_cellm
(
 rrm_cell_index_t cell_index, 
 U32 otdoa_info_item, 
 s1ap_rrm_lppa_otdoa_info_resp_t * rrm_s1ap_lppa_otdoa_info_resp,
 rrm_cause_t *p_otdoa_cause
 );

/* DYNAMIC ICIC CHANGES START */
/*! \fn rrm_return_et rrm_rrc_x2ap_rrm_load_info_res_process_msg(U8 *p_api, U16 api_id, U16 data_len, U16 trans_id)
 *  \brief handle load info setup response msg from RRC
 *  \param p_api    pointer to input API buffer
 *  \param api_id   api id
 *  \param data_len Data length
 *  \param trans_id TRANSACTION id
 */
rrm_return_et
rrm_rrc_x2ap_rrm_load_info_res_process_msg
(
 U8      *p_api, /* Pointer to incoming Api buffer*/
 U16     api_id, /* incoming api id*/
 U16     data_len, /* incoming data length*/
 U16     trans_id /* Incoming transaction Id*/
 );

/*! \fn rrm_return_et  build_and_send_x2ap_icic_load_information 
 *  \param rrm_cell_context_t
 */
rrm_return_et build_and_send_x2ap_icic_load_information
(
 rrm_cell_context_t *p_cell_ctxt        /* cell context */
 );

rrm_return_et build_and_send_l2_cell_config_req
(
 rrm_resource_partition_info_t *p_dl_resource_partition_info,
 rrm_resource_partition_info_t *p_ul_res_partition_info,
 /* eICIC_PHASE_1_2_CHANGES_START */
 U8                           *serving_abs_pattern,
 U8                           *usable_abs_pattern,
 /* eICIC_PHASE_1_2_CHANGES_END */
 rrm_cell_index_t           cell_index,
 U16                        trans_id
 );


rrm_void_t
rrm_cellm_increment_dynamic_icic_counter_in_cell
(
 rrm_cell_index_t  cell_index,
 rrm_cellm_dynamic_icic_counters_et counter_to_be_updated
 );



rrm_void_t
rrm_cellm_decrement_dynamic_icic_counter_in_cell
(
 rrm_cell_index_t  cell_index,
 rrm_cellm_dynamic_icic_counters_et counter_to_be_updated
 );


rrm_void_t
rrm_request_l2_cell_config_req
(
    /*SPR 17777 +-*/
 /* eICIC_PHASE_1_2_CHANGES_START */
 rrm_cell_context_t  * p_cell_ctx
 /* eICIC_PHASE_1_2_CHANGES_END */
 );

rrm_return_et build_and_send_l2_ue_reconfig_req
(
 rrm_cell_index_t     cell_index ,
 rrm_ue_index_t *p_ue_index_list,
 U32             num_of_ues,
 rrm_location_update_action_et ue_location_update_action
 );

rrm_void_t
rrm_l2_build_send_reconfig_schedule_params
(
 rrm_cell_context_t *p_cell_ctx
 );

rrm_return_et build_and_send_l2_ue_reconfig_req 
(
 rrm_cell_index_t     cell_index ,
 rrm_ue_index_t *p_ue_index_list,
 U32             num_of_ues,
 rrm_location_update_action_et ue_location_update_action
 );

rrm_void_t
rrm_cellm_decrement_dynamic_icic_counter_in_cell
( 
 rrm_cell_index_t  cell_index,
 rrm_cellm_dynamic_icic_counters_et counter_to_be_updated
 );


rrm_icic_scheme_type_et
rrm_cellm_get_icic_scheme_configured_in_cell
( 
 rrm_cell_index_t  cell_index
 );


rrm_void_t
rrm_cellm_increment_dynamic_icic_counter_in_cell
( 
 rrm_cell_index_t  cell_index,
 rrm_cellm_dynamic_icic_counters_et counter_to_be_updated
 ); 

rrm_cellm_dynamic_icic_data_t*
rrm_cellm_get_dynamic_icic_data_values
(
 rrm_cell_index_t cell_index
 );


rrm_return_et build_and_send_uem_update_ue_info
(
 rrm_cell_context_t                       *p_cell_ctx,
 rrmuem_cm_update_ue_info_req_t           *p_update_ue_info
 );

U8 action_on_oi_reports_from_serving_cell 
(
 rrm_cell_context_t  *p_cell_context,
 rrm_bool_et         *p_change_ue_categorization
 );

U32 action_on_rntp_reports_from_serving_cell 
(
 rrm_cell_context_t  *p_rrm_cell_ctx,
 rrm_partitioning_info_action_type_et     *p_action,
 U32                                      *p_new_ce_start_prb
 );

S8 predict_hii_map_based_on_ul_ce_prb_usage
(
 rrm_cell_context_t  *p_cell_context,
 rrm_partitioning_info_action_type_et     *p_action
 );

rrm_void_t mandate_search_for_region_to_left_or_right_of_cnfg_partition 
(
 U32                      current_ce_prb_start_index,
 U32                      current_ce_prb_count,
 U32                      prb_to_squeeze,
 U32                      *p_new_start_rb,
 U32                      prb_configured_in_srv_cell
 );
/* DYNAMIC ICIC CHANGES END */

/*! \fn rrm_return_et cellm_fsm_process_event(U8 event,rrm_cell_context_t *cell_context)
 *  \brief handle process event of cell
 *  \param event 
 *  \param cell_context cell context
 */
rrm_return_et 
cellm_fsm_process_event(U8 event,rrm_cell_context_t *cell_context);

/*! \fn rrm_return_et rrm_cellm_cell_config_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell config request
 *  \param p_api     pointer to input API buffer
 *  \param api_id    api id
 *  \param data_len  Data length
 */
rrm_return_et
rrm_cellm_cell_config_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_ue_del_resp_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell config request
 *  \param p_api     pointer to input API buffer
 *  \param api_id    api id 
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_ue_del_resp_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );
/*! \fn rrm_return_et rrm_cellm_cell_reconfig_req_process_msg(void *p_api, U16 api_id, U16 data_len)
 *  \brief handle cell Reconfig request
 *  \param p_api pointer to input API buffer
 *  \param api_id api id
 *  \param data_len Data length
 */
rrm_return_et 
rrm_cellm_cell_reconfig_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*SPR 16494 START*/
/*! \fn rrm_void_t updating_enb_id_node_in_ncl_info(rrm_enb_gu_group_id_node_t *p_enb_id_node,rrm_bool_et node_deleted)
 *  \brief updates enb_id in ncl.
 *  \param p_enb_id_node
 *  \param node_deleted
*/
rrm_void_t
updating_enb_id_node_in_ncl_info
(
 rrm_enb_gu_group_id_node_t    *p_enb_id_node,
 rrm_bool_et    node_deleted
);
/*SPR 16494 END*/

/*! \fn rrm_return_et rrm_cellm_cell_init_config_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell Init config request
 *  \param p_api  pointer to input API buffer
 *  \param api_id api id
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_init_config_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_set_log_level_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle log level set request
 *  \param p_api  pointer to input API buffer
 *  \param api_id api id
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_set_log_level_req_process_msg
(
 void *p_api /*pointer to input API buffer*/
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_register_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle register requset for cell
 *  \param p_api  pointer to input API buffer
 *  \param api_id api id
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_register_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_deregister_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle deregister request for cell
 *  \param p_api    pointer to input API buffer
 *  \param api_id   api id
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_deregister_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_set_attr_req_process_msg(U8 *p_api,U16 api_id,U16 data_len)
 *  \brief handle set attribute requset
 *  \param p_api pointer to input API buffer
 *  \param api_id   api id
 *  \param data_len Data length
 */
rrm_return_et
rrm_cellm_cell_set_attr_req_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_start_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell start request
 *  \param p_api pointer pt api buffer
 *  \param api_id ADI id of incoming message
 *  \param data_len data length of incoming message
 */
rrm_return_et
rrm_cellm_cell_start_req_process_msg(
        void *p_api,/*pointer pt api buffer*/
        U16 api_id /*ADI id of incoming message*/
    /*SPR 17777 +-*/
        );

/*! \fn rrm_return_et rrm_cellm_cell_stop_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell stop request
 *  \param p_api pointer to input API buffer
 *  \param api_id api id of incoming messagepointer pt api buffer
 *  \param data_len Data length incoming message
 */
rrm_return_et
rrm_cellm_cell_stop_req_process_msg(
        void *p_api/*pointer pt api buffer*/
    /*SPR 17777 +-*/
        );

/*! \fn rrm_return_et rrm_cellm_cell_update_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell Update request
 *  \param p_api     pointer to input API buffer
 *  \param api_id    ADI id of incoming message
 *  \param data_len  data length of incoming message
 */
rrm_return_et
rrm_cellm_cell_update_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_cellm_cell_stop_adm_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell stop admission request
 *  \param p_api    pointer pt api buffer
 *  \param api_id   ADI id of incoming message
 *  \param data_len data length of incoming message
 */
rrm_return_et
rrm_cellm_cell_stop_adm_req_process_msg(
        void *p_api,/*pointer pt api buffer*/
        U16 api_id /*ADI id of incoming message*/
    /*SPR 17777 +-*/
        );

/*! \fn rrm_return_et rrm_cellm_cell_start_adm_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell start admission request
 *  \param p_api    pointer pt api buffer
 *  \param api_id   ADI id of incoming message
 *  \param data_len data length of incoming message
 */
rrm_return_et
rrm_cellm_cell_start_adm_req_process_msg(

        void *p_api,/*pointer pt api buffer*/
        U16 api_id /*ADI id of incoming message*/
    /*SPR 17777 +-*/
        );

/*! \fn rrm_return_et rrm_cellm_cell_delete_req_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief handle cell delete request
 *  \param p_api    pointer to input API buffer
 *  \param api_id   ADI id of incoming message
 *  \param data_len data length of incoming message
 */

rrm_return_et 
rrm_cellm_cell_delete_req_process_msg
(
 void *p_api, /*pointer to input API buffer*/
 U16 api_id
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_rrc_cell_start_resp_process_msg(U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle cell start response
 *  \param p_api  pointer to input API buffer
 *  \param api_id ADI id of incoming message
 *  \param data_len data length of incoming message 
 *  \param trans_id Transaction id
 */
rrm_return_et
rrm_rrc_cell_start_resp_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len,
 U16 trans_id
 );

/*! \fn rrm_return_et cell_reconfig_resp_process_msg(U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle cell reconfig response
 *  \param p_api     pointer to input API buffer
 *  \param api_id    ADI id of incoming message
 *  \param data_len  ADI id of incoming messagedata length of incoming message
 *  \param trans_id  Transaction id
 */
rrm_return_et 
cell_reconfig_resp_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len,
 U16 trans_id
 );

/*! \fn rrm_return_et rrm_rrc_cell_delete_resp_process_msg(U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief This function handles cell delete response
 *  \param p_api  pointer to input API buffer
 *  \param api_id  ADI id of incoming message
 *  \param data_len data length of incoming message
 *  \param trans_id Transaction id 
 */
rrm_return_et 
rrm_rrc_cell_delete_resp_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len,
 U16 trans_id
 );
/*RIM changes start*/
/*! \fn rrm_rrc_rim_info_process_msg(U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \breif handle rim info process message
 *  \param p_api
 *  \param api_id
 *  \param data_len
 *  \param trans_id
 */
rrm_return_et
rrm_rrc_rim_info_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len
    /*SPR 17777 +-*/
 );

void
rrm_send_rim_ack
(
 U8 count,
 rim_app_identity_t app_id,
 rrm_cell_context_t *p_cell_ctx
 );

void
fill_plmn_id_for_eutran_cell
(
 rrm_cell_context_t *p_cell_ctx,
    /*SPR 17777 +-*/
 U8 plmn[3]
 );

void
fill_plmn_id_for_geran_cell
(
 /* Spr 16211 Changes Start*/
 geran_freq_ncl_t   * p_cell,
 U8 plmn[RRM_THREE]
 /* Spr 16211 Changes End*/
 );

void
fill_plmn_id_for_utran_cell
(
 /* Spr 16211 Changes Start*/
 /* Spr 19647 Changes Start*/
 plmn_identity_t *p_plmn_identity, 
 /* Spr 19647 Changes End*/
 U8 plmn[RRM_THREE]
 /* Spr 16211 Changes End*/
 );
/* Spr 19647 Changes Start*/
void
fill_plmn_id_for_utran_geran_cell_for_dst_rtng_addr_IE
(
 plmn_identity_t *p_plmn_identity, 
 U8 plmn[RRM_THREE]
 );
 void
fill_global_enb_plmn_id_for_eutran_cell
(
 rrm_cell_context_t *p_cell_ctx,
 U8 plmn[RRM_THREE]
);
/* Spr 19647 Changes End*/

rrm_bool_t
rrm_check_for_stale_rsn
(
 U32 rsn_recd,
 U32 rsn_prev
 );

/*RIM changes end*/

/* __MR__ BEGIN */

/*! \fn epc_params_t* rrm_cellm_get_erb_service_profile(rrm_cell_index_t cell_index)
 *  \brief This function gets the erb service profile 
 *  \param cell_index cell index
 */
epc_params_t*
rrm_cellm_get_erb_service_profile
(
 rrm_cell_index_t cell_index
 );

/*csg start*/
/*! \fn  rrm_cell_info_for_ue_mgr_t * rrm_cellm_get_cell_resource_info(rrm_cell_index_t cell_index)
 *  \brief This function gets cell resource information
 *  \param cell_index cell index
 */
rrm_cell_info_for_ue_mgr_t*
rrm_cellm_get_cell_resource_info(
        rrm_cell_index_t cell_index
        );
/*! \fn access_mgmt_params_t* rrm_cellm_get_access_mgmt_params(rrm_cell_index_t cell_index)
 *  \brief This function is for getting access management parameters info  
 *  \param cell_index cell index
 */ 
access_mgmt_params_t* 
rrm_cellm_get_access_mgmt_params(
        rrm_cell_index_t cell_index
        );
/*csg end*/

/*! \fn common_eutran_params_t* rrm_cellm_get_common_eutran_params(rrm_cell_index_t cell_index)
 *  \brief This function gets common eutran params
 *  \param cell_index cell index
 */
common_eutran_params_t*
rrm_cellm_get_common_eutran_params
(
 rrm_cell_index_t cell_index
 );
/*! \fn admission_control_info_t* rrm_cellm_get_admission_control_info(rrm_cell_index_t cell_index)
 *  \brief This function gets admission control info
 *  \param cell_index cell index
 */
admission_control_info_t*
rrm_cellm_get_admission_control_info
(
 rrm_cell_index_t cell_index
 );


/*! \fn rrmc_mac_config_t* rrm_cellm_get_rrmc_mac_config(rrm_cell_index_t cell_index)
 *  \brief This function gets rrm mac config
 *  \param cell_index cell index
 */
rrmc_mac_config_t*
rrm_cellm_get_rrmc_mac_config
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_return_et rrm_cellm_get_p_max_sib1( rrm_cell_index_t cell_index,S8 *p_p_max_sib1)
 *  \brief outputs p_max_sib1 if exists
 *  \param cell_index cell index
 *  \param p_p_max_sib1 
 */

rrm_return_et
rrm_cellm_get_p_max_sib1
(
 rrm_cell_index_t cell_index,
 S8               *p_p_max_sib1
 );

/*! \fn U8 rrm_cellm_get_time_alignment_timer( rrm_cell_index_t cell_index) 
 *  \brief returns time alignment timer
 *  \param cell_index cell index
 */
U8
rrm_cellm_get_time_alignment_timer
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_return_et rrm_cellm_get_antenna_port(rrm_cell_index_t cell_index,U8 *antenna_port)
 *  \brief This function contains the info about the antenna port
 *  \param cell_index cell index
 *  \param antenna_port
 */
rrm_return_et
rrm_cellm_get_antenna_port
(
 rrm_cell_index_t cell_index,
 U8 *antenna_port
 );


/*! \fn rrm_return_et rrm_get_mxn_anetnna_support(rrm_cell_index_t ,rrm_antenna_mxn_et  *)
 *  \brief This function return the mxn support at eNB
 *  \param cell_index cell index
 *  \param antenna_port
 */
rrm_return_et
rrm_get_mxn_anetnna_support(rrm_cell_index_t , rrm_antenna_mxn_et *);

/*! \fn rrm_return_et rrm_cellm_get_rrmc_band_width(rrm_cell_index_t cell_index, U8 *dl_bandwidth)
 *  \brief This function contains the downlink b/w
 *  \param cell_index cell index
 *  \param dl_bandwidth downlink bandwidth
 */
rrm_return_et
rrm_cellm_get_rrmc_band_width
(
 rrm_cell_index_t cell_index,
 U8 *dl_bandwidth
 );

/*! \fn rlc_layer_params_t* rrm_cellm_get_srb_info(rrm_cell_index_t cell_index)
 *  \brief This function contains srb info
 *  \param cell_index cell index
 */
rlc_layer_params_t*
rrm_cellm_get_srb_info
(
 rrm_cell_index_t cell_index
 );

/*! \fn mac_layer_params_t* rrm_cellm_mac_layer_params(rrm_cell_index_t cell_index)
 *  \brief This function contains mac layer params
 *  \param cell_index cell index
 */
mac_layer_params_t*
rrm_cellm_mac_layer_params
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrc_timers_t* rrm_cellm_get_timer_info(rrm_cell_index_t cell_index)
 *  \brief this function contains cell timer info
 *  \param cell_index cell index
 */
rrc_timers_t*
rrm_cellm_get_timer_info
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_physical_layer_params_t* rrm_cellm_get_physical_layer_params(rrm_cell_index_t cell_index)
 *  \brief This function gets physical layer params
 *  \param cell_index cell index
 */
rrm_physical_layer_params_t*
rrm_cellm_get_physical_layer_params
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrc_rrm_ue_service_profile_t* rrm_cellm_get_ue_service_profile(rrm_cell_index_t cell_index)
 *  \brief This function gets ue service profile
 *  \param cell_index cell index
 */
rrc_rrm_ue_service_profile_t*
rrm_cellm_get_ue_service_profile
(
 rrm_cell_index_t cell_index
 );
/* __MR__ END */

/*!\fn rrm_void_t rrm_cellm_mark_sr_index_free(rrm_cell_index_t cell_index,U8 row_num, U16 col_num)
 * \brief This function contains the cell information that needs to be freed
 * \param cell_index cell index
 * \param row_num    row number
 * \param col_num    column number
 */

/* Coverity 19oct Start : 54929 */
rrm_void_t
/* Coverity 19oct End : 54929 */
rrm_cellm_mark_sr_index_free
(
 rrm_cell_index_t cell_index,
 U8               row_num,
 U16              col_num
 );

/*!\fn rrm_void_t rrm_cellm_mark_cqi_index_free(rrm_cell_index_t cell_index, U8 row_num,U16 col_num)
 * \brief This function contains the cell information that needs to be freed
 * \param cell_index cell index
 * \param row_num    row number
 * \param col_num    column number
 */
/* Coverity 19oct Start : 54928 */
rrm_void_t
/* Coverity 19oct End : 54928 */
rrm_cellm_mark_cqi_index_free
(
 rrm_cell_index_t cell_index,
 U8 row_num,
 U16 col_num
 );

/* SPR 14509 start */
U8 rrm_cellm_get_dl_gbr_abs_in_use(rrm_cell_index_t cell_index);

U8 rrm_cellm_get_dl_ngbr_abs_in_use(rrm_cell_index_t cell_index);
/* SPR 14509 end */


U8 rrm_cellm_get_ngbr_threshold                 (rrm_cell_index_t cell_index);
/* ! \fn U8 rrm_cellm_get_dl_gbr_prb_in_use(rrm_cell_index_t cell_index)
 * \brief This function gets downlink GBR PRB in use
 * \param cell_index cell index
 */
U8 rrm_cellm_get_dl_gbr_prb_in_use              (rrm_cell_index_t cell_index);
/* ! \fn U8 rrm_cellm_get_ul_gbr_prb_in_use(rrm_cell_index_t cell_index)
 * \brief This function gets uplink GBR PRB in use
 * \param cell_index cell index
 */
U8 rrm_cellm_get_ul_gbr_prb_in_use              (rrm_cell_index_t cell_index);
/* ! \fn U8 rrm_cellm_get_dl_ngbr_prb_in_use(rrm_cell_index_t cell_index)
 * \brief This function is for getting downlink non GBR PRB in use
 * \param cell_index cell index
 */
U8 rrm_cellm_get_dl_ngbr_prb_in_use             (rrm_cell_index_t cell_index);
/* ! \fn U8 rrm_cellm_get_ul_ngbr_prb_in_use(rrm_cell_index_t cell_index)
 * \brief This function is for getting uplink non GBR PRB in use
 * \param cell_index cell index
 */
/*SPR 18654 START*/
U8 rrm_cellm_get_ul_ngbr_prb_in_use             (rrm_cell_index_t cell_index);
/* ! \fn U8 rrm_cellm_get_dl_allocated_gbr_prb(rrm_cell_index_t cell_index)
 * \brief This function is for getting downlink allocated  GBR information
 * \param cell_index cell index
*/
U64 rrm_cellm_get_dl_allocated_gbr_prb           (rrm_cell_index_t cell_index);
/*SPR 18654 END*/
/* ! \fn U8 rrm_cellm_get_dl_allocated_gbr_prb(rrm_cell_index_t cell_index)
 * \brief This function is for getting downlink allocated  GBR information
 * \param cell_index cell index
 */
/*SPR 18654 START*/
U64 rrm_cellm_get_dl_allocated_gbr_prb           (rrm_cell_index_t cell_index);
/*SPR 18654 END*/
/* ! \fn U8 rrm_cellm_get_ul_allocated_gbr_prb(rrm_cell_index_t cell_index)
 * \brief This function is for getting uplink allocated GBR information
 * \param cell_index cell index
 */
/*SPR 18654 START*/
U64 rrm_cellm_get_ul_allocated_gbr_prb           (rrm_cell_index_t cell_index);
/*SPR 18654 END*/
/* ! \fn U8 rrm_cellm_get_dl_allocated_ngbr_prb(rrm_cell_index_t cell_index)
 * \brief This function is for getting downlink allocated non GBR information
 * \param cell_index cell index
 */
/*SPR 18654 START*/
U64 rrm_cellm_get_dl_allocated_ngbr_prb          (rrm_cell_index_t cell_index);
/*SPR 18654 END*/
/* ! \fn U8 rrm_cellm_get_ul_allocated_ngbr_prb(rrm_cell_index_t cell_index)
 * \brief This function is for getting uplink allocated non GBR information
 * \param cell_index cell index
 */
/*SPR 18654 START*/
U64 rrm_cellm_get_ul_allocated_ngbr_prb          (rrm_cell_index_t cell_index);
/*SPR 18654 END*/
/* ! \fn rrm_return_et rrm_cellm_increment_num_active_ue (rrm_cell_index_t cell_index)
 * \brief This function is for incrementing the number of active users
 * \param cell_index cell index
 */
rrm_return_et rrm_cellm_increment_num_active_ue (rrm_cell_index_t cell_index);
/* ! \fn rrm_return_et rrm_cellm_decrement_num_active_ue (rrm_cell_index_t cell_index)
 * \brief This function is for decrement the number of active users
 * \param cell_index cell index
 */
rrm_return_et rrm_cellm_decrement_num_active_ue (rrm_cell_index_t cell_index);
/*! \fn lte_ncl_t *rrm_cellm_get_lte_ncl_info(rrm_cell_index_t cell_index)
 *  \brief This function is used to get intra rat Neighbor Cell list information
 *  \param cell_index Cell Index
 */
lte_ncl_t                *rrm_cellm_get_lte_ncl_info
(rrm_cell_index_t cell_index);
/* InterRAT changes start */
/*! \fn ncl_params_t *rrm_cellm_get_ncl_info(rrm_cell_index_t cell_index)
 *  \brief This function is used to get Neighbor Cell list information
 *  \param cell_index Cell Index
 */
ncl_params_t                *rrm_cellm_get_ncl_info
(rrm_cell_index_t cell_index);
/* InterRAT changes end */
/*! \fn ncl_params_t *rrm_cellm_get_ncl_info(rrm_cell_index_t cell_index)
 *  \brief This function is used to get operator info
 *  \param cell_index Cell Index
 */
rrm_sib_type_3_info_t    *rrm_cellm_get_operator_info
(rrm_cell_index_t cell_index);
/*! \fn rrm_common_params_for_eutra_t *rrm_cellm_get_connected_mode_common_params(rrm_cell_index_t cell_index)
 *  \brief This function is used for EUTRA params info
 *  \param cell_index Cell Index
 */
rrm_common_params_for_eutra_t *rrm_cellm_get_connected_mode_common_params
(rrm_cell_index_t cell_index);


/****************************************************************************
 * Function Name  : rrm_cellm_get_connected_mode_irat_params
 * Inputs         : cell index
 * Outputs        : None
 * Returns        : connected mode irat parameters 
 * Description    : Returns the connected mode irat params 
 *                  that is stored in global context
 ****************************************************************************/
/*! \fn rrm_irat_params_t *rrm_cellm_get_connected_mode_irat_params(rrm_cell_index_t cell_index)
 *  \brief This function Returns the connected mode irat params that is stored in global context
 *  \param cell_index cell index
 */
rrm_irat_params_t *rrm_cellm_get_connected_mode_irat_params
(
 rrm_cell_index_t cell_index
 );
/* ! \fn idle_mode_mobility_params_t *rrm_cellm_get_idle_mode_params(rrm_cell_index_t cell_index)
 *   \brief This function Returns the idle mode params
 *   \param cell_index cell index
 */
idle_mode_mobility_params_t *rrm_cellm_get_idle_mode_params
(rrm_cell_index_t cell_index);
/* ! \fn rf_params_t *rrm_cellm_get_rf_params( rrm_cell_index_t cell_index)
 *   \brief This function returns the radio frequency params 
 *   \param cell_index cell index
 */
rf_params_t         *rrm_cellm_get_rf_params
(
 rrm_cell_index_t cell_index
 );
/* ! \fn rrm_trigger_quantity_et  get_a3_a4_trigger_quantity( rrm_cell_index_t cell_index)
 * \brief This function Returns the radio frequency params stored in global 
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_a3_a4_trigger_quantity
(
 rrm_cell_index_t cell_index
 );
/* ! \fn rrm_trigger_quantity_et  get_b2_eutra_trigger_quantity(rrm_cell_index_t cell_index)
 * \brief This function Returns the radio frequency params stored in global
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_b2_eutra_trigger_quantity
(
 rrm_cell_index_t cell_index
 );
/* ! \fn rrm_trigger_quantity_et  get_b2_utra_trigger_quantity(rrm_cell_index_t cell_index)
 *  \brief This function Returns the radio frequency params stored in global
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_b2_utra_trigger_quantity
(
 rrm_cell_index_t cell_index
 );
/* ! \fn rrm_trigger_quantity_et get_b1_trigger_quantity(rrm_cell_index_t cell_index)
 *  \brief This function Returns the radio frequency params stored in global
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_b1_trigger_quantity
(
 rrm_cell_index_t cell_index
 );
/* ! \fn rrm_trigger_quantity_et get_a5_trigger_quantity(rrm_cell_index_t cell_index)
 * \brief This function gets a5 trigger quantity information
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_a5_trigger_quantity
(
 rrm_cell_index_t cell_index
 );

/* ! \fn rrm_cell_context_t *rrm_cellm_get_cell_context(rrm_cell_index_t cell_index)
 *  \brief this function gets cell context
 *  \param cell_index cell index
 */ 
rrm_cell_context_t       *rrm_cellm_get_cell_context
(
 rrm_cell_index_t cell_index
 );

/* ! \fn rrm_trigger_quantity_et  get_delayed_a3_configure(rrm_cell_index_t cell_index)
 * \brief Returns the radio frequency params 
 * \param cell_index cell index
 */
rrm_trigger_quantity_et  get_delayed_a3_configure
(
 rrm_cell_index_t cell_index
 );

/* ! \fn U8 get_a4_meas_cfg_enabled(rrm_cell_index_t cell_index)
 * \brief This function gets meas config enabled information
 * \param cell_index cell index
 */
U8 get_a4_meas_cfg_enabled
(
 rrm_cell_index_t cell_index
 );
/* ! \fn U8 get_priority_value_for_emergency_call(rrm_cell_index_t cell_index)
 * \brief This function gets priority value for emergency calls
 * \param cell_index cell index
 */
U8 get_priority_value_for_emergency_call
(
 rrm_cell_index_t cell_index
 );

/* ! \fn U16 get_csfb_blind_timer_duration(rrm_cell_index_t cell_index)
 *   \brief This function gives the blind timer duration
 *   \param cell_index cell index
 */
U16 get_csfb_blind_timer_duration
(
 rrm_cell_index_t cell_index
 );

/* ! \fn U8 get_b2_event_eutran_thresh_rsrx(rrm_cell_index_t cell_index) 
 *  \brief This function gets B2 event eutran threshold
 *  \param cell_index cell index
 */
U8  get_b2_event_eutran_thresh_rsrx
(
 rrm_cell_index_t cell_index
 );

/* ! \fn S8  get_irat_offset(rrm_cell_index_t cell_index)
 *\brief This function gets irat offset
 *  \param cell_index cell index
 */
S8  get_irat_offset
(
 rrm_cell_index_t cell_index
 );

/* ! \fn rrm_void_t rrm_cellm_set_dl_allocated_gbr_prb(rrm_cell_index_t cell_index, U8 dl_allocated_gbr_prb)
 *   \brief This function is used  for storing the allocated downlink PRBs
 *   \param cell_index cell index cell index
 *   \param dl_allocated_gbr_prb allocated downlink PRBs for GBR
 */
rrm_void_t 
rrm_cellm_set_dl_allocated_gbr_prb   
/*SPR 18654 START*/
(
  rrm_cell_index_t cell_index,
   U64 dl_allocated_gbr_prb
    );
/*SPR 18654 END*/
/* ! \fn rrm_void_t rrm_cellm_set_ul_allocated_gbr_prb(rrm_cell_index_t cell_index, U8 ul_allocated_gbr_prb)
 *   \brief This function is used  for storing the allocated uplink PRBs
 *   \param cell_index cell index cell index
 *   \param ul_allocated_gbr_prb allocated downlink PRBs for GBR
 */
rrm_void_t 
rrm_cellm_set_ul_allocated_gbr_prb   
/*SPR 18654 START*/
(
     rrm_cell_index_t cell_index,
         U64 ul_allocated_gbr_prb
         );
/*SPR 18654 END*/
/* ! \fn rrm_void_t rrm_cellm_set_dl_allocated_ngbr_prb(rrm_cell_index_t cell_index,U8 dl_allocated_ngbr_prb)
 *   \brief This function is used  for storing the allocated downlink PRBs for Non GBR bearers in the cell context
 *   \param cell_index cell index  
 *    \param dl_allocated_ngbr_prb allocated downlink PRBs for NonGBR bearers
 */
rrm_void_t 
rrm_cellm_set_dl_allocated_ngbr_prb  
(
 rrm_cell_index_t cell_index, 
 /*SPR 18654 START*/
    U64 dl_allocated_ngbr_prb
 /*SPR 18654 END*/
 );
/* ! \fn rrm_void_t rrm_cellm_set_ul_allocated_ngbr_prb(rrm_cell_index_t cell_index,U8 ul_allocated_ngbr_prb)
 *   \brief This function is used  for storing the allocated uplink PRBs for Non GBR bearers in the cell context
 *   \param cell_index cell index
 *   \param ul_allocated_ngbr_prb allocated downlink PRBs for NonGBR bearers
 */
rrm_void_t 
rrm_cellm_set_ul_allocated_ngbr_prb  
(
 rrm_cell_index_t cell_index, 
 /*SPR 18654 START*/
    U64 ul_allocated_ngbr_prb
 /*SPR 18654 START*/
 );
/*spr_fix_12573_start*/
/*spr_fix_12573_end*/
/*! \fn rrm_void_t rrm_cellm_init_rac_rbc_params(rrm_cell_context_t *p_rrm_cell_ctx)
 *  \brief This function is used for RAC RBC params initialization
 *  \param p_rrm_cell_ctx Pointer to Cell Context
 */
rrm_void_t rrm_cellm_init_rac_rbc_params        (rrm_cell_context_t *p_rrm_cell_ctx);
/*! \fn U8 rrm_cellm_get_dl_prb_budget_combined(rrm_cell_index_t cell_index)
 *  \brief This function is used to downlink PRB budget combined information
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_dl_prb_budget_combined         (rrm_cell_index_t cell_index);
/*! \fn U8 rrm_cellm_get_ul_prb_budget_combined (rrm_cell_index_t cell_index)
 *  \brief This function is used to get Uplink PRB budget combined information
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_ul_prb_budget_combined         (rrm_cell_index_t cell_index);

/*! \fn U8 rrm_cellm_get_dl_prb_budget_gbr (rrm_cell_index_t cell_index)
 *  \brief This function is used  to get downlink prb budget gbr info
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_dl_prb_budget_gbr              (rrm_cell_index_t cell_index);

/*! \fn U8 rrm_cellm_get_ul_prb_budget_gbr(rrm_cell_index_t cell_index)
 *  \brief This function is used to get uplink prb budget gbr info
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_ul_prb_budget_gbr              (rrm_cell_index_t cell_index);

/*! \fn U8 rrm_cellm_get_dl_prb_budget_ngbr(rrm_cell_index_t cell_index)
 *  \brief This function is used to get downlink prb budget non gbr info
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_dl_prb_budget_ngbr             (rrm_cell_index_t cell_index);

/*! \fn U8 rrm_cellm_get_ul_prb_budget_ngbr (rrm_cell_index_t cell_index)
 *  \brief This function is used to get uplink prb budget non gbr info
 *  \param  cell_index Cell Index
 */
U8 rrm_cellm_get_ul_prb_budget_ngbr             (rrm_cell_index_t cell_index);

/* ! \fn U8 rrm_cellm_get_num_prb_required(U64 bitrate,U32 bw_per_prb,S8  snr,float bw_eff,float snr_eff)
 *  \brief This function gets the number of prb bearers required
 *  \param bitrate bit rate
 *  \param bw_per_prb bandwidth per prb
 *  \param snr        SNR
 *  \param bw_eff     bandwidth efficiency
 *  \param snr_eff     SNR efficiency
 */
U8 
rrm_cellm_get_num_prb_required               
(
 U64 bitrate, 
 U32 bw_per_prb, 
 S8  snr, 
 float bw_eff, 
 float snr_eff
 );
/*! \fn U8 rrm_cellm_get_dl_usable_bw_prb_for_bearers   (rrm_cell_index_t cell_index)
 *  \brief This function is used to get downlink usable bandwidth PRB for bearers
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_dl_usable_bw_prb_for_bearers   (rrm_cell_index_t cell_index);
/*! \fn U8 rrm_cellm_get_ul_usable_bw_prb_for_bearers   (rrm_cell_index_t cell_index)
 *  \brief This function is used to get uplink usable bandwidth PRB for bearers
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_ul_usable_bw_prb_for_bearers   (rrm_cell_index_t cell_index);
/*! \fn U8 rrm_cellm_get_non_default_bearer_multiplier  (rrm_cell_index_t cell_index)
 *  \brief This function is used to get non default bearer multiplier
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_non_default_bearer_multiplier  (rrm_cell_index_t cell_index);
/*! \fn U64 rrm_cellm_get_minimum_bitrate(rrm_cell_index_t cell_index)
 *  \brief This function is for getting minimum bitrate information
 *  \param cell_index Cell Index
 */
U64 rrm_cellm_get_minimum_bitrate                (rrm_cell_index_t cell_index);
/*! \fn U16 rrm_cellm_get_reserved_resource_factor(rrm_cell_index_t cell_index)
 *  \brief This function is used to get reserved resource factor 
 *  \param cell_index Cell Index
 */
U16 rrm_cellm_get_reserved_resource_factor       (rrm_cell_index_t cell_index);

/*! \fn U8 rrm_cellm_get_cpu_utilization_limit(rrm_cell_index_t cell_index)
 *  \brief This function is used get CPU utilization limit
 *  \param cell_index Cell Index
 */
U8 rrm_cellm_get_cpu_utilization_limit          (rrm_cell_index_t cell_index);
/*! \fn U8 rrm_cellm_get_max_num_ue_per_cell(rrm_cell_index_t cell_index)
 *  \brief This function is used to get max num UE per cell
 *  \param cell_index Cell Index
 */
/* +- SPR 18268 */
U16 rrm_cellm_get_max_num_ue_per_cell            (rrm_cell_index_t cell_index);
/* +- SPR 18268 */
/* SPR 21527 Start */
/*! \fn U16 rrm_cellm_get_num_active_ue(rrm_cell_index_t cell_index)
 *  \brief This function is used get number active UE
 *  \param cell_index Cell Index
 */
U16 rrm_cellm_get_num_active_ue                  (rrm_cell_index_t cell_index);
/* SPR 21527 End */

/** TNL feature */
/*! \fn U64 rrm_cellm_get_total_backhaul_capacity(rrm_cell_index_t cell_index)
 *  \brief This function is used total backhaul capacity
 *  \param cell_index rrm_cell_index_t
 */
U64 rrm_cellm_get_total_backhaul_capacity       (rrm_cell_index_t cell_index);
/*! \fn U8 rrm_cellm_get_capacity_threshold(rrm_cell_index_t cell_index)
 *  \brief This function is used to get capacity threshold
 *  \param cell_index Cell Index
 */
U8  rrm_cellm_get_capacity_threshold            (rrm_cell_index_t cell_index);
/** TNL feature */


/* ! \fn rrm_return_et rrm_cellm_get_sr_resources (rrm_cell_index_tcell_index, U8 sr_res_index,U8 *sr_config_index,U8   *row, U16  *col)
 *  \brief Checks the availability of SR resource for UE admission
 *  \param cell_index cell index
 *  \param sr_res_index sr res index
 *  \param sr_config_index  sr config index
 *  \param row row
 *  \param col column
 */
rrm_return_et 
rrm_cellm_get_sr_resources        
(
 rrm_cell_index_t cell_index, 
 U8               *sr_res_index, 
 U8 *sr_config_index, 
 U8               *row,
 /*SPR 15713 Fix Start*/
 U16              *col,
 rrm_bool_et      mark_sr_busy
 /*SPR 15713 Fix End*/

 );


/* ! \fn rrm_return_et rrm_cellm_get_available_row_and_column_and_cqi_res_index(rrm_cell_index_t cell_index,U8 *row, U16 *col,U16  *cqi_config_index, U8   *cqi_pucch_res_index,U16   *ri_index, U8    sr_config_index)
 *  \brief This function Checks the availability of CQI resource index in any frame and slot
 *  \param cell_index cell index
 *  \param row row 
 *  \param col column
 *  \param cqi_config_index cqi config index
 *  \param cqi_pucch_res_index cqi pucch index
 *  \param ri_index ri index
 *  \param sr_config_index sr config index
 */
rrm_return_et
rrm_cellm_get_available_row_and_column_and_cqi_res_index
(
 rrm_cell_index_t cell_index,
 U8               *row,
 U16              *col,
 U16              *cqi_config_index,
 U8               *cqi_pucch_res_index,
 U16              *ri_index,
 U8               sr_config_index,
     /* SPR14123start*/
 /* eICIC_PHASE_1_2_CHANGES_START */
 rrm_abs_type_et    abs_type
 /* eICIC_PHASE_1_2_CHANGES_END */
     /* SPR14123end*/
 );

/* ! \fn rrm_return_et rrm_cell_get_free_cqi_res_index(rrm_cell_index_t cell_index, U8  n1_idx, U16   sr_idx, U16  *cqi_config_idx,U8   *cqi_pucch_res_idx,U16  *ri_res_index)
 *  \brief This function Checks the availability of CQI resource index by frame and slot in the CQI resource list
 *  \param cell_index cell index
 *  \param n1_idx 
 *  \param sr_idx
 *  \param cqi_config_idx cqi config index
 *  \param cqi_pucch_res_idx cqi pucch index
 *  \param ri_res_index      ri res index
 */
rrm_return_et
rrm_cell_get_free_cqi_res_index
(
 rrm_cell_index_t cell_index,
 U8               n1_idx,
 U16              sr_idx,
 U16              *cqi_config_idx,
 U8               *cqi_pucch_res_idx,
 U16              *ri_res_index
 );

/* ! \fn rrm_return_et rrm_cellm_get_cqi_resources(rrm_cell_index_t cell_index, U8 *cqi_pucch_res_index, U16 *cqi_config_index, U16 *ri_index, U8 sr_config_index, U8 *row, U16 *col)
 * \brief This function Checks the availability of CQI resource for UE admission
 * \param cell_index cell index
 * \param cqi_pucch_res_index  CQI PUCCH res index
 * \param cqi_config_index CQI config index
 * \param ri_index        ri index
 * \param sr_config_index sr config index
 * \param row row
 * \param col column
 */
rrm_return_et 
rrm_cellm_get_cqi_resources       
(
 rrm_cell_index_t cell_index, 
 U8                *cqi_pucch_res_index,
 U16               *cqi_config_index, 
 U16               *ri_index,
 U8                sr_config_index,
 U8                *row,
 U16               *col,
     /* SPR14123start*/
        /* eICIC_PHASE_1_2_CHANGES_START */
 rrm_abs_type_et    abs_type
        /* eICIC_PHASE_1_2_CHANGES_END */
     /* SPR14123end*/
 );

/* ! \fn rrm_return_et rrm_compare_sub_frame_offset_for_sr_and_cqi(U8  sr_periodicity, 
   U8  cqi_periodicity, U8   sr_config_index, U16  cqi_config_index)
 * \brief This function checks whether the subfram offsets allocatedfor CQI and SR are same or not
 * \param sr_periodicity sr periodicity
 * \param cqi_periodicity cqi periodicity
 * \param sr_config_index sr config index
 * \param cqi_config_index cqi config index
 */

rrm_return_et
rrm_compare_sub_frame_offset_for_sr_and_cqi(
        U8  sr_periodicity,
        U8  cqi_periodicity,
        U8   sr_config_index,
        U16  cqi_config_index,
        /* SPR 16427 fix start */
        rrm_cell_context_t* p_rrm_cell_ctx
        /* SPR 16427 fix end */
        );

/* ! \fn U8 rrm_cellm_get_config_index_at_available_frame_slot_based_on_cqi_periodicity
   (U8 cqi_periodicity, U8 frame_num, U8 slot_num)
 * \brief This function gets config index based on cqi periodicity 
 * \param cqi_periodicity cqi periodicity
 * \param frame_num frame number
 * \param slot_num slot number
 */
U8
rrm_cellm_get_config_index_at_available_frame_slot_based_on_cqi_periodicity
( 
 U8 cqi_periodicity,
 U8 frame_num,
 U8 slot_num
 );
/* - SPR 17439 */
/* ! \fn U8 rrm_cellm_get_config_index_at_available_frame_slot_based_on_sr_periodicity(U8 sr_periodicity, U8 frame_num, U8 slot_num)
 * \brief This function gets config index based on sr periodicity
 * \param sr_periodicity sr periodicity
 * \param frame_num frame number
 * \param slot_num slot number
 */
U8
rrm_cellm_get_config_index_at_available_frame_slot_based_on_sr_periodicity
(
 U8 sr_periodicity,
 U8 frame_num,
 U8 slot_num
 );
/* ! \fn rrm_return_et rrm_cellm_get_available_row_and_colum_and_sr_res_index(
   rrm_cell_index_t  cell_index, U8  *row,U16 *colum, U8  *sr_config, U8  *sr_pucch_res)
 *  \brief This function Checks the availability of SR resource index in any frame 
 *  \param cell_index cell index
 *  \param row row
 *  \param colum  column
 *  \param sr_config sr config
 *  \param sr_pucch_res SR PUCCH respons
 */
rrm_return_et
rrm_cellm_get_available_row_and_colum_and_sr_res_index
(
 rrm_cell_index_t  cell_index,
 U8  *row,
 U16 *colum,
 U8  *sr_config,
 /*SPR 15713 Fix Start*/
 U8  *sr_pucch_res,
 rrm_bool_et mark_sr_busy
 /*SPR 15713 Fix End*/
 );
/* ! \fn rrm_cell_mark_sr_index_busy(rrm_cell_index_t cell_index, U8  n1_idx, U16 sr_idx)
 *   \brief Marks a SR resource index as busy by popping the corresponding 
 *   \param cell_index Index of Cell
 *   \param n1_idx n1 Index 
 *   \param sr_idx sr Index
 */
void
rrm_cell_mark_sr_index_busy
(
 rrm_cell_index_t cell_index,
 U8  n1_idx,
 /*SPR 15713 Fix Start*/
 U16 sr_idx,
 rrm_bool_et sr_mark_resolve
 /*SPR 15713 Fix End*/

 );

/*! \fn void rrm_cell_mark_cqi_index_busy(rrm_cell_index_t cell_index,U8 row,U16 col)
 *  \brief This function is used to mark CQI index busy
 *  \param cell_index Cell Index
 *  \param row Row no
 *  \param col Column no
 */
void
rrm_cell_mark_cqi_index_busy
(
 rrm_cell_index_t cell_index,
 U8 row,
 U16 col
 );

/*! \fn rrm_void_t find_cqi_periodicity_from_cell_context(rrm_cell_context_t *p_cell_context,U16 *p_cqi_periodicity)
 *  \brief This function is used to find  CQI periodicity from cell context 
 *  \param p_cell_context Cell Context
 *  \param p_cqi_periodicity CQI periodicity
 */
rrm_void_t find_cqi_periodicity_from_cell_context
(
 rrm_cell_context_t *p_cell_context,
 U16                *p_cqi_periodicity
 );

/*! \fn rrm_void_t find_sr_periodicity_from_cell_context(rrm_cell_context_t *p_cell_context,U16 *p_sr_periodicity,U8 *p_n_one_pucch_an)
 *  \brief This function is used to find SR periodicity from Cell Context 
 *  \param p_cell_context Cell Context
 *  \param p_sr_periodicity SR periodicity
 *  \param p_n_one_pucch_an U8 type variable
 */
rrm_void_t find_sr_periodicity_from_cell_context
(
 rrm_cell_context_t *p_cell_context,
 U16                *p_sr_periodicity,
 /* SPR 11003 Fix Start */
 U16                *p_n_one_pucch_an
 /* SPR 11003 Fix End */
 );

/*! \fn rrm_return_et rrm_cell_get_free_sr_res_index(rrm_cell_index_t cell_index,U8  n1_idx,U16 sr_idx,U8  *sr_config_idx,U8  *sr_pucch_res_idx)
 *  \brief This function is used to get free SR response index
 *  \param cell_index Cell index
 *  \param n1_idx n1 index
 *  \param sr_idx SR index
 *  \param sr_config_idx SR config index
 *  \param sr_pucch_res_idx PUCCH Response Index
 */
rrm_return_et
rrm_cell_get_free_sr_res_index
(
 rrm_cell_index_t cell_index,
 U8  n1_idx,
 U16 sr_idx,
 U8  *sr_config_idx,
 U8  *sr_pucch_res_idx
 );


/*! \fn rrm_return_et rrm_cellm_cell_rrmim_rac_disable_enable(U8 *p_api, U16 api_id, U16 data_len)
 *  \brief to send the disable and enable req to MIF
 *  \param[in] p_api Input Api buffer
 *  \param[in] api_id Incoming Api id
 *  \param[in] data_len Incoming data length
 */
rrm_return_et
rrm_cellm_cell_rrmim_rac_disable_enable
(
 U8 *p_api, /*Input Api buffer*/
 U16 api_id /*Incoming Api id*/
    /*SPR 17777 +-*/
 );

/*! \fn rrm_return_et rrm_log_cell_info(const rrm_cell_context_t *ptr_rrm_cell_context)
 *  \brief to write the cell info in file
 *  \param ptr_rrm_cell_context constant Pointer to rrm_cell_context_t
 */
rrm_return_et
rrm_log_cell_info(const rrm_cell_context_t *ptr_rrm_cell_context);
/*! \fn rrm_cellm_get_freq_priority_params (rrm_cell_index_t cell_index)
 *  \brief This function is used to get frequency priority parameters
 *  \param cell_index Cell index
 */
rrm_freq_priority_list_t*
rrm_cellm_get_freq_priority_params
(
 rrm_cell_index_t cell_index
 );
/*! \fn rrm_cellm_get_priority_cfg_params(rrm_cell_index_t cell_index)
 *  \brief This function is used to get priority config parameters
 *  \param cell_index Cell Index
 */
rrm_priority_cfg_t*
rrm_cellm_get_priority_cfg_params
(
 rrm_cell_index_t cell_index
 );
/*! \fn U16 rrm_cellm_get_num_prb_based_on_snr(rrm_cell_index_t cell_index,U8  snr,rrm_bool_et is_uplink)
 *  \brief This function is used to get number of PRB based on SNR
 *  \param cell_index Cell Index
 *  \param snr SNR 
 *  \param is_uplink Is uplink boolean type
 */
U16
rrm_cellm_get_num_prb_based_on_snr
(
 rrm_cell_index_t cell_index,
 rrm_bool_et is_uplink
 );
/*GBR LIMIT START */
/*! \fn U64 rrm_cellm_get_used_dl_bitrate(rrm_cell_index_t cell_index)
 *  \brief This function is used to get Downlink bitrate information
 *  \param cell_index Cell Index
 */
U64
rrm_cellm_get_used_dl_bitrate(rrm_cell_index_t cell_index);
/*! \fn U64 rrm_cellm_get_used_ul_bitrate(rrm_cell_index_t cell_index)
 *  \brief This function is used to get Uplink bitrate information
 *  \param cell_index Cell Index
 */
U64 
rrm_cellm_get_used_ul_bitrate(rrm_cell_index_t cell_index);
/*! \fn void rrm_cellm_set_used_dl_bitrate_during_erb_release(rrm_cell_index_t cell_index, U64 dl_bitrate)
 *  \brief This function is used for downlink bitrate during erb release
 *  \param cell_index Cell Index
 *  \param dl_bitrate Downlink Bitrate
 */
void 
rrm_cellm_set_used_dl_bitrate_during_erb_release(rrm_cell_index_t cell_index, U64 dl_bitrate);
/*! \fn void rrm_cellm_set_used_ul_bitrate_during_erb_release(rrm_cell_index_t cell_index, U64 ul_bitrate)
 *  \brief This function is used for uplink bitrate during erb release
 *  \param cell_index Cell Index
 *  \param ul_bitrate Uplinlk Bitrate
 */
void 
rrm_cellm_set_used_ul_bitrate_during_erb_release(rrm_cell_index_t cell_index, U64 ul_bitrate);
/*! \fn U16 rrm_cellm_get_dl_gbr_limit(rrm_cell_index_t cell_index)
 *  \brief This function is used to get Downlink GBR limit information
 *  \param cell_index rrm_cell_index_t
 */
U16 
rrm_cellm_get_dl_gbr_limit(rrm_cell_index_t cell_index);
/*! \fn U16 rrm_cellm_get_ul_gbr_limit(rrm_cell_index_t cell_index)
 *  \brief This function is used to get uplink GBR limit
 *  \param cell_index rrm_cell_index_t
 */
U16 
rrm_cellm_get_ul_gbr_limit(rrm_cell_index_t cell_index);
/*! \fn void rrm_cellm_set_used_dl_bitrate_during_erb_setup_modify(rrm_cell_index_t cell_index,U64 dl_bitrate)
 *  \brief This function is used to set downlink during ERB setup modification                                    
 *  \param cell_index Cell Index
 *  \param dl_bitrate Downlink Bitrate
 */
void 
rrm_cellm_set_used_dl_bitrate_during_erb_setup_modify(rrm_cell_index_t cell_index, U64 dl_bitrate);
/*! \fn void rrm_cellm_set_used_ul_bitrate_during_erb_setup_modify(rrm_cell_index_t cell_index,U64 dl_bitrate)
 *  \brief This function is used for Uplink Bitrate during ERB setup modify
 *  \param cell_index rrm_cell_index_t
 *  \param dl_bitrate downlink bitrate
 */
void 
rrm_cellm_set_used_ul_bitrate_during_erb_setup_modify(rrm_cell_index_t cell_index, U64 dl_bitrate);
/*GBR LIMIT END */

/* UPDATED NRT INFO CHANGES START */ 
/****************************************************************************
 * Function Name  : rrm_cellm_updated_nrt_info_process_msg
 * Inputs         : void *p_api  : pointer to api buffer
 *		          : U16 api_id : api id
 *		          : U16 data_len : data length
 * Outputs        : None
 * Returns        : RRM_FAILURE/RRM_SUCCESS
 * Description    : Handles the updated_nrt_info message to cell rrm
 ****************************************************************************/
/*! \fn rrm_return_et rrm_cellm_updated_nrt_info_process_msg(void *p_api,U16 api_id,U16 data_len)
 *  \brief This function Handles the updated_nrt_info message to cell rrm
 *  \param p_api pointer pt api buffer
 *  \param api_id API id of incoming message
 *  \param data_len data length of incoming message
 */
rrm_return_et
rrm_cellm_updated_nrt_info_process_msg(
        void *p_api,/*pointer pt api buffer*/
        U16 api_id /*API id of incoming message*/
    /*SPR 17777 +-*/
        );
/* UPDATED NRT INFO CHANGES END */ 

/*! \fn rrm_cellm_get_ncl_params(rrm_cell_index_t cell_index)
 *  \brief This function is for getting NCL parameters
 *  \param cell_index
 */
lte_ncl_t*
rrm_cellm_get_ncl_params
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_sib_type_3_info_t *rrm_cellm_get_sib3_info(rrm_cell_index_t cell_index)
 *  \brief This function is used to get SIB type 3 information
 *  \param cell_index Cell index
 */
rrm_sib_type_3_info_t    *rrm_cellm_get_sib3_info
(
 rrm_cell_index_t cell_index
 );

/* Inter RAT changes start */
/****************************************************************************
 ** Function Name  : rrm_cellm_get_inter_rat_ncl_info
 ** Inputs         : cell index
 ** Outputs        : None
 ** Returns        : inter_rat_ncl_info
 ** Description    : Returns the inter_rat_ncl_info params 
 **                  that is stored in global context
 *****************************************************************************/
/*! \fn inter_rat_ncl_t *rrm_cellm_get_inter_rat_ncl_info(rrm_cell_index_t cell_index)
 *  \brief This function Returns the inter_rat_ncl_info params that is stored in global context
 *  \param cell_index Cell index
 */
inter_rat_ncl_t  *rrm_cellm_get_inter_rat_ncl_info
(
 rrm_cell_index_t cell_index
 );
/* Inter RAT changes end */
/*! \fn rrmcm_search_earfcn_in_cell_context(
  rrm_cell_context_t *p_cell_ctx, U16 earfcn)
 *  \brief  This function searches the EARFCN in the configured
 freq list in cell context and returns the index at which
 it is stored
 *  \param  p_cell_ctx Pointer to cell context
 *  \param  earfcn     Freq to be searched
 */
S16
rrmcm_search_earfcn_in_cell_context(
        rrm_cell_context_t              *p_cell_ctx,
        U32                             earfcn
        );

/*! \fn S16 rrmcm_search_uarfcn_fdd_in_cell_context(rrm_cell_context_t *p_cell_ctx, U16 uarfcn)
 *  \brief  This function searches the FDD UARFCN in the configured
 freq list in cell context and returns the index at which
 it is stored
 *  \param  p_cell_ctx Pointer to cell context
 *  \param  uarfcn     Freq to be searched
 */
S16
rrmcm_search_uarfcn_fdd_in_cell_context(
        rrm_cell_context_t              *p_cell_ctx,
        U16                             uarfcn
        );

/* TDD Support SON Start */
/*! \fn S16 rrmcm_search_uarfcn_tdd_in_cell_context(rrm_cell_context_t *p_cell_ctx, U16 uarfcn)
 *  \brief  This function searches the TDD UARFCN in the configured
 freq list in cell context and returns the index at which
 it is stored
 *  \param  p_cell_ctx Pointer to cell context
 *  \param  uarfcn     Freq to be searched
 */
S16
rrmcm_search_uarfcn_tdd_in_cell_context(
        rrm_cell_context_t              *p_cell_ctx,
        U16                             uarfcn_tdd
        );
/* TDD Support SON End */

/*! \fn S16 rrmcm_search_geran_arfcn_in_cell_context(rrm_cell_context_t *p_cell_ctx,U16 geran_arfcn,rrm_son_geran_band_indicator_et band_ind)
 *  \brief  This function searches the GERAN freq in the configured
 freq list in cell context and returns the index at which
 it is stored
 *  \param  p_cell_ctx      Pointer to cell context
 *  \param  geran_arfcn     Freq to be searched
 *  \param  band_ind        band indicator information
 */
S16
rrmcm_search_geran_arfcn_in_cell_context(
        rrm_cell_context_t              *p_cell_ctx,
        U16                             geran_arfcn,
        rrm_son_geran_band_indicator_et band_ind
        );

/* BLR Team HO status report to SON Changes Start */
/*! \fn rrm_bool_et rrm_cellm_get_ho_report_status ( rrm_cell_index_t cell_index)
 *  \brief This function is used by cellm to get HO report status
 *  \param cell_index Cell index
 */
rrm_bool_et
rrm_cellm_get_ho_report_status
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_bool_et rrm_cellm_get_mlb_ho_report_status ( rrm_cell_index_t cell_index)
 *  \brief This function is used by cellm to get mlb HO report status
 *  \param cell_index Cell index
 */
rrm_bool_et
rrm_cellm_get_mlb_ho_report_status
(
 rrm_cell_index_t cell_index
 );

/* BLR Team HO status report to SON Changes End */
/* SRS Start */
/*! \fn rrm_bool_et rrm_cellm_get_srs_status(rrm_cell_index_t cell_index)
 *  \brief SRS flag status
 *  \param  cell_index
 */
rrm_bool_et
rrm_cellm_get_srs_status
(
 rrm_cell_index_t cell_index
 );

/*  \fn rrm_return_et rrm_cellm_mark_srs_index_free(rrm_cell_index_t cell_index,U16 srs_anchor)
 *  \brief Checks the BUSY index and marked the the index for 
 *         released UE.
 *  \param cell_index Cell Index
 *  \param srs_anchor SRS  Anchor
 */
rrm_return_et
rrm_cellm_mark_srs_index_free
(
 rrm_cell_index_t cell_index,
 U16   srs_anchor
 );


/*! \fn rrm_cellm_pkt_delay_bdgt_value(U8 pkt_delay_value)
 *  \brief This function is for Packet Delay budget value information
 *  \param pkt_delay_value
 */
U16 rrm_cellm_pkt_delay_bdgt_value(U8 pkt_delay_value);


/*! \fn rrm_cellm_pkt_err_loss_rate_value(U8 pkt_err_loss_rate)
 *  \brief This function is for packet error loss rate value information
 *  \param pkt_err_loss_rate error loss rate
 */
U32 rrm_cellm_pkt_err_loss_rate_value(U8 pkt_err_loss_rate);

/*LTE_RRM_TM_SWITCH_START*/
/*! \fn rrm_get_dl_sinr_quality_threshold(rrm_cell_index_t cell_index)
 *  \brief This function get sinr threshold information
 *  \param cell_index Cell index
 */
U8 rrm_get_dl_sinr_quality_threshold(rrm_cell_index_t cell_index);


/* uplink mu mimo chnage start*/

/*! \fn rrm_prb_usage_reached_threshold_ind(rrm_cell_index_t cell_index )
 *  \brief this function trigger the event for prb threshold reached indication
 *  \param cell_index Cell index
 */
rrm_void_t
rrm_prb_usage_reached_threshold_ind( rrm_cell_index_t cell_index);

/*! \fn rrm_get_uplink_cell_center_prb_budget(rrm_cell_index_t cell_index )
 *  \brief this function returns the uplink cell edge prb usage
 *  \param cell_index Cell index
 */
U32 rrm_get_uplink_cell_center_prb_budget( rrm_cell_index_t cell_index);

/*! \fn rrm_get_uplink_cell_edge_prb_budget(rrm_cell_index_t cell_index )
 *  \brief this function returns the uplink cell edge prb usage
 *  \param cell_index Cell index
 */
U32 rrm_get_uplink_cell_edge_prb_budget( rrm_cell_index_t cell_index);

/*! \fn rrm_get_uplink_cell_edge_prb_usage(rrm_cell_index_t cell_index )
 *  \brief this function returns the uplink cell edge prb usage
 *  \param cell_index Cell index
 */
U32 rrm_get_uplink_cell_edge_prb_usage( rrm_cell_index_t    cell_index );

/*SPR 7892 FIX START*/
/*! \fn rrm_set_mac_ue_reconfig_mutex(rrm_cell_index_t cell_index,rrm_mac_ue_reconfig_mutex_val_et ue_mac_reconfig_mutex)
 *  \brief settet funtion set the ue mac reconfig mutex
 *  \param cell_index Cell index
 *  \param ue_mac_reconfig_mutex
 */
rrm_void_t rrm_set_mac_ue_reconfig_mutex( rrm_cell_index_t  cell_index,
        rrm_mac_ue_reconfig_mutex_val_et ue_mac_reconfig_mutex );
/*SPR 7892 FIX END*/

/*! \fn rrm_get_uplink_pathloss_threshold_for_ue(rrm_cell_index_t cell_index)
 *  \brief This function get ue reconfif mutex
 *  \param cell_index Cell index
 */
/*SPR 7892 FIX START*/
rrm_mac_ue_reconfig_mutex_val_et  rrm_get_mac_ue_reconfig_mutex( rrm_cell_index_t  cell_index);
/*SPR 7892 FIX END*/

/*! \fn rrm_get_uplink_pathloss_threshold_for_ue(rrm_cell_index_t cell_index)
 *  \brief This function get uplink sinr threshold information
 *  \param cell_index Cell index
 */
U32 rrm_get_uplink_pathloss_threshold_for_ue(rrm_cell_index_t cell_index);

/*! \fn rrm_get_uplink_sinr_threshold(rrm_cell_index_t cell_index)
 *  \brief This function get uplink sinr threshold information
 *  \param cell_index Cell index
 */
U8 rrm_get_uplink_sinr_threshold(rrm_cell_index_t cell_index);


/*! \fn rrm_get_cell_uplink_prb_usage_threshold_percentage(rrm_cell_index_t cell_index)
 *  \brief This function get uplink sinr threshold information
 *  \param cell_index Cell index
 */
U8 rrm_get_cell_uplink_prb_usage_threshold_percentage(rrm_cell_index_t cell_index);

/*! \fn rrm_get_cell_prb_usage_down_threshold_percentage(rrm_cell_index_t cell_index)
 *  \brief This function get uplink sinr threshold information
 *  \param cell_index Cell index
 */
U8 rrm_get_cell_prb_usage_down_threshold_percentage(rrm_cell_index_t cell_index );

/*! \fn rrm_get_total_uplink_prb_usage(rrm_cell_index_t cell_index)
 *  \brief This function get uplink prb usage information
 *  \param cell_index Cell index
 */
U64 rrm_get_total_uplink_prb_usage( rrm_cell_index_t    cell_index);
/* uplink mu mimo chnage end*/

/*! \fn rrm_get_packet_delay_budget(rrm_cell_index_t cell_index,U8 qci_index)
 *  \brief This get Packet delay budget information
 *  \param cell_index Cell Index
 *  \param qci_index QCI index
 */
U16
rrm_get_packet_delay_budget(rrm_cell_index_t    cell_index,
        U8                  qci_index);

/*! \fn U32 rrm_get_packet_error_loss_rate(rrm_cell_index_t cell_index,U8 qci_index)
 *  \brief This function is to get packet error loss rate information
 *  \param cell_index Cell Index
 *  \param qci_index QCI index
 */
U32
rrm_get_packet_error_loss_rate(rrm_cell_index_t    cell_index,
        U8                  qci_index);

/*LTE_RRM_TM_SWITCH_END*/

/*! \fn rrm_fill_plmn_identity_from_config_transfer(
  rrm_oam_cell_plmn_info_t *p_rrm_plmn_id, plmn_identity_t *p_mme_plmn_id)
 * \brief This function convert the mme_plmn_id_t to rrm_plmn_id_t
 * \param p_rrm_plmn_id 
 * \param p_mme_plmn_id
 */
void rrm_fill_plmn_identity_from_config_transfer
(
 rrm_oam_cell_plmn_info_t *p_rrm_plmn_id,
 plmn_identity_t *p_mme_plmn_id
 );

/*! \fn rrm_return_et rrm_build_and_send_enb_config_tranfer_req(
  rrmcm_rmif_cell_son_tnl_discovery_req_t  *p_tnl_discovery_req,rrm_enb_context_t  enb_ctx)
 * \brief This function builds and send enb config transfer request
 * \param p_tnl_discovery_req
 * \param enb_ctx
 */
rrm_return_et
rrm_build_and_send_enb_config_tranfer_req
(
 rrmcm_rmif_cell_son_tnl_discovery_req_t  *p_tnl_discovery_req,
 /* SPR_17367_Start */
 rrm_enb_context_t  *p_enb_ctx
 /* SPR_17367_End */
 );
/*IPv6 changes begin*/
/*! \fn rrm_return_et rrm_build_and_send_enb_config_tranfer_resp (s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer, U16 trans_id)
 * \brief This function builds and send enb config transfer response
 * \param p_mme_config_transfer
 * \param trans_id Transaction id
 */
rrm_return_et
rrm_build_and_send_enb_config_tranfer_resp
(
 s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
 U16 trans_id
 );
/*Code cleaned*/
/*IPv6 changes end*/

/*! \fn rrm_block_cell_params_ho_t *rrm_cellm_get_block_cell_ho_params(rrm_cell_index_t cell_index)
 * \brief This function gets ho params
 * \param cell_index cell index
 */
rrm_block_cell_params_ho_t * 
rrm_cellm_get_block_cell_ho_params
(
 rrm_cell_index_t cell_index
 );
/*! \fn rrm_return_et rrm_cellm_process_etws_scheduling_req(
  s1ap_rrm_pws_request_t  *p_s1ap_rrm_pws_request,rrm_cell_context_t *p_cell_ctx)
 * \brief Processes etws scheduling
 * \param p_s1ap_rrm_pws_request
 * \param p_cell_ctx pointer to cell context
 */
/* warning removal: prototype declaration added, 10/04/2012 */
rrm_return_et
rrm_cellm_process_etws_scheduling_req(
        s1ap_rrm_pws_request_t  *p_s1ap_rrm_pws_request, 
        rrm_cell_context_t      *p_cell_ctx
        );
/*! \fn rrm_return_et rrm_cellm_process_cmas_scheduling_req(
  s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request,rrm_cell_context_t *p_cell_ctx)
 * \brief Processes cmas scheduling
 * \param p_s1ap_rrm_pws_request
 * \param p_cell_ctx pointer to cell context
 */
/* warning removal: prototype declaration added, 10/04/2012 */
rrm_return_et
rrm_cellm_process_cmas_scheduling_req(
        s1ap_rrm_pws_request_t  *p_s1ap_rrm_pws_request, 
        rrm_cell_context_t      *p_cell_ctx
        );

/*! \fn rrm_return_et rrm_cellm_update_etws_scheduling_info_list(
  s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf, rrm_cell_context_t      *p_cell_ctx)
 * \brief Update scheduling info list
 * \param p_s1ap_rrm_pws_cnf
 * \param p_cell_ctx pointer to cell context
 */
/* warning removal: prototype declaration added, 10/04/2012 */
rrm_return_et
rrm_cellm_update_etws_scheduling_info_list(
        s1ap_rrm_pws_cnf_t  *p_s1ap_rrm_pws_cnf, 
        rrm_cell_context_t      *p_cell_ctx
        );

/*! \fn rrm_return_et rrm_rrc_prepare_update_pws_list_resp(
  rrc_rrm_updated_pws_si_list_req_t   *p_rrc_rrm_updated_pws_si_list_req, rrm_cell_context_t *p_cell_context, rrc_rrm_updated_pws_si_list_resp_t  *p_rrc_rrm_updated_pws_si_list_resp)
 * \brief This function Processes update pws list request from RRC
 * \param p_rrc_rrm_updated_pws_si_list_req 
 * \param p_cell_context pointer to cell context
 * \param p_rrc_rrm_updated_pws_si_list_resp rrc updated pws si response list
 */
/* warning removal: prototype declaration added, 10/04/2012 */
rrm_return_et
rrm_rrc_prepare_update_pws_list_resp(
        rrc_rrm_updated_pws_si_list_req_t   *p_rrc_rrm_updated_pws_si_list_req,
        rrm_cell_context_t                  *p_cell_context,
        rrc_rrm_updated_pws_si_list_resp_t  *p_rrc_rrm_updated_pws_si_list_resp
        );

/*! \fn rrm_return_et rrm_rrc_update_cell_pws_list_on_cnf(
  rrc_rrm_updated_pws_si_list_cnf_t   *p_rrc_rrm_updated_pws_si_list_cnf, rrm_cell_context_t *p_cell_context)
 * \brief This function processes update pws list cnf from RRC
 * \param p_rrc_rrm_updated_pws_si_list_cnf
 * \param p_cell_context pointer to cell context
 */


/* warning removal: prototype declaration added, 10/04/2012 */
rrm_return_et
rrm_rrc_update_cell_pws_list_on_cnf(
        rrc_rrm_updated_pws_si_list_cnf_t   *p_rrc_rrm_updated_pws_si_list_cnf,
        rrm_cell_context_t                  *p_cell_context
        );


/* warning removal: prototype declaration added, 10/04/2012 */
/*! \fn rrm_return_et rrm_cellm_update_cmas_scheduling_info_list(
  s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf, rrm_cell_context_t *p_cell_ctx)
 * \brief Processes cmas scheduling
 * \param p_s1ap_rrm_pws_cnf
 * \param p_cell_ctx pointer to cell context
 */
rrm_return_et
rrm_cellm_update_cmas_scheduling_info_list(
        s1ap_rrm_pws_cnf_t  *p_s1ap_rrm_pws_cnf, 
        rrm_cell_context_t      *p_cell_ctx
        );

/* warning removal: prototype declaration added, 10/04/2012 */
/* SRS_FREQ_HOPPING_START */
/*! \fn rrm_void_t rrm_set_srs_resources_free(rrm_cell_context_t *p_cell_ctx)
 * \brief This function sets srs resource free
 * \param p_cell_ctx pointer to cell context
 */
rrm_void_t
rrm_set_srs_resources_free
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_void_t print_srs_resource_info(rrm_cell_context_t *p_cell_context)
 * \brief This function prints srs resource info
 * \param p_cell_context pointer to cell context
 */
rrm_void_t
print_srs_resource_info
(
 rrm_cell_context_t *p_cell_context
 );
/*! \fn rrm_return_et rrm_cellm_get_srs_resources(rrm_cell_index_t  cell_index,
  rrm_ue_srs_params_t *p_rrm_ue_srs_params)
 * \brief This function gets srs resource
 * \param cell_index cell index
 * \param p_rrm_ue_srs_params rrm ue srs parameters
 */
rrm_return_et
rrm_cellm_get_srs_resources
(
 rrm_cell_index_t  cell_index,
 rrm_ue_srs_params_t *p_rrm_ue_srs_params
 );

/*! \fn rrm_return_et rrm_cellm_mark_srs_index_free(
  rrm_cell_index_t cell_index, U16   srs_anchor)
 * \brief Checks the BUSY index and marked the the index for released UE
 * \param cell_index cell index
 * \param srs_anchor srs anchor
 */
rrm_return_et
rrm_cellm_mark_srs_index_free
(
 rrm_cell_index_t cell_index,
 U16   srs_anchor
 );

/*! \fn rrm_return_et rrm_cellm_mark_srs_index_busy(rrm_cell_index_t cell_index,
  U16   srs_anchor)
 * \brief Checks the BUSY index and marked the the index for released UE
 * \param cell_index cell index
 * \param srs_anchor srs anchor
 */
rrm_return_et
rrm_cellm_mark_srs_index_busy
(
 rrm_cell_index_t cell_index,
 U16   srs_anchor
 );
/* SRS_FREQ_HOPPING_END */

/* warning removal: prototype declaration added, 10/04/2012 */
/*! \fn rrm_return_et rrm_cellm_mark_srs_index_free(rrm_cell_index_t cell_index,U16              srs_anchor)
 * \brief Checks the FREE index and marked the the index for released UE
 * \param cell_index cell index
 * \param srs_anchor srs anchor
 */
rrm_return_et
rrm_cellm_mark_srs_index_free
(
 rrm_cell_index_t cell_index,
 U16              srs_anchor
 );


/* warning removal: prototype declaration added, 10/15/2012 */
/*! \fn rrm_allocate_tpc_rnti_table_pusch(rrm_cell_context_t *p_rrm_cell_ctx)
 *  \brief Allocates the tpc rnti alocation table for pusch
 *  \param p_rrm_cell_ctx
 */
rrm_return_et
rrm_allocate_tpc_rnti_table_pusch
(
 rrm_cell_context_t *p_rrm_cell_ctx
 );

/* warning removal: prototype declaration added, 10/15/2012 */
/*! \fn rrm_allocate_tpc_rnti_table_pucch(rrm_cell_context_t *p_rrm_cell_ctx)
 *  \brief This function Allocates the tpc rnti alocation table for pucch
 *  \param p_rrm_cell_ctx
 */
rrm_return_et 
rrm_allocate_tpc_rnti_table_pucch
(
 rrm_cell_context_t *p_rrm_cell_ctx
 );


/* LTE_RRM_WARNING_REMOVAL_START  prototype added */

/*! \fn rrm_cellm_get_subband_size(rrm_cell_index_t cell_index)
 *  \brief This function gets cellm subband size
 *  \param cell_index cell index
 */
U8
rrm_cellm_get_subband_size
(void); 
    /*SPR 17777 +-*/

/* LTE_RRM_WARNING_REMOVAL_END */


/*! \fn rrm_oam_spid_table_t* rrm_cellm_get_spid_table_params(rrm_cell_index_t cell_index
  )
 *  \brief This function returns address of the rrm_oam_spid_table_t
 *  \param cell_index cell index
 */
rrm_oam_spid_table_t*
rrm_cellm_get_spid_table_params
(
 rrm_cell_index_t cell_index
 );

/* SPS related changes start */

/*! \fn rrm_cellm_decrement_num_sps_active_ue(rrm_cell_index_t cell_index, U16              ue_sps_n1_pucch_indx)
 *  \brief Decrement the no. of SPS active UEs in the cell context
 *  \param cell_index cell index
 *  \param ue_sps_n1_pucch_indx 
 */
void
rrm_cellm_decrement_num_sps_active_ue
(
 rrm_cell_index_t cell_index,
 U16              ue_sps_n1_pucch_indx
 );

/* SPS related changes end */
/* Code for MLB Start */

#define RRM_CELLM_MAX_HASH_BUCKECT      10
typedef struct _rrm_cellm_enb_info_t
{
    rrm_oam_eutran_global_cell_id_t     cell_id;
    rrmcm_rmif_cell_x2_status_et        x2_status;
    rrm_bool_t                          x2_link_up_status;
    rrm_bool_t                          broadcast_status;
    rrm_bool_t                          blacklisted;
    S32                                 ho_status;
    /* eICIC_PHASE_1_2_CHANGES_START */
    rrm_invoke_ind_status_et         invoke_ind_status;
    /* eICIC_PHASE_1_2_CHANGES_END */
}rrm_cellm_enb_info_t;
typedef struct _rrm_cellm_enb_id_list_t
{
    YLNODE                  node;
    rrm_cellm_enb_info_t    enb_info;
}rrm_cellm_enb_id_lnode_t;

typedef struct _rrm_cellm_enb_id_hnode_t
{
    YHNODE                  node;
    YLIST                   list;
    U32                     enb_id;
}rrm_cellm_enb_id_hnode_t;

int rrm_cmp_enb_id
(
 const rrm_void_t *p_val_1,
 const rrm_void_t *p_val_2
 );

Ulong32 rrm_cellm_hashing_func
(
 const rrm_void_t *p_enb_id
 );

const rrm_void_t *
rrm_keyof_func
(
 const YHNODE *p_hNode
 );

rrm_return_et
rrm_build_and_send_rsu_req_to_macro_enb
(
 rrm_cellm_enb_id_hnode_t *p_enb_hnode,
 U32 periodic,
 rrm_cell_index_t cell_index
 );

rrm_return_et
rrm_cellm_find_and_send_rsu_req_to_cells_on_same_enb
(
 lte_ncl_t *lte_ncl,
 rrm_cell_index_t cell_index,
 U32 periodic
 );

/*! \fn rrm_rrc_x2ap_rrm_rsu_enb_start_res_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle enb start response msg from RRC
 *  \param p_api pointer to api
 *  \param api_id API identifier
 *  \param data_len data length of the mesaage
 *  \param trans_id transaction id
 */
rrm_return_et
rrm_rrc_x2ap_rrm_rsu_enb_start_res_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len,
 U16 trans_id
 );
/*! \fn rrm_rrc_x2ap_rrm_rsu_enb_start_req_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle enb start req msg from RRC
 *  \param p_api pointer to api
 *  \param data_len data length of the mesaage
 *  \param trans_id transaction id
 *  \param api_id API identifier
 */
rrm_return_et
rrm_rrc_x2ap_rrm_rsu_enb_start_req_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len
    /*SPR 17777 +-*/
 );
/*! \fn rrm_rrc_x2ap_rrm_rsu_enb_stop_req_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle enb stop req msg from RRC
 *  \param p_api pointer to input API buffer
 *  \param api_id API identifier
 *  \param data_len data length of the mesaage
 *  \param trans_id transaction id
 */
rrm_return_et
rrm_rrc_x2ap_rrm_rsu_enb_stop_req_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len
    /*SPR 17777 +-*/
 );
/*! \fn rrm_rrc_x2ap_rrm_rsu_enb_stop_res_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle cell setup response from RRC
 *  \param p_api     pointer to input API buffer
 *  \param data_len  data length of the mesaage
 *  \param api_id API identifier
 *  \param trans_id  transaction id
 */
rrm_return_et
rrm_rrc_x2ap_rrm_rsu_enb_stop_res_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len
    /*SPR 17777 +-*/
 );
/*! \fn rrm_rrc_x2ap_rrm_rsu_enb_update_ind_process_msg (U8 *p_api,U16 api_id,U16 data_len,U16 trans_id)
 *  \brief handle enb update msg from RRC
 *  \param p_api     pointer to input API buffer
 *  \param api_id API identifier
 *  \param data_len  data length of the mesaage
 *  \param trans_id  transaction id
 */
rrm_return_et
rrm_rrc_x2ap_rrm_rsu_enb_update_ind_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
 U16 api_id,
 U16 data_len
    /*SPR 17777 +-*/
 );
/*! \fn rrm_cell_context_t *get_cell_cntxt_n_cell_idx_from_gb_cell_id(rrm_oam_eutran_global_cell_id_t *p_gb_cell_id,U16 *p_cell_index)
 *  \brief  get cell context and index from gbl cell id
 *  \param p_gb_cell_id 
 *  \param p_cell_index pointer to cel index
 */
rrm_cell_context_t *get_cell_cntxt_n_cell_idx_from_gb_cell_id
(
 rrm_oam_eutran_global_cell_id_t *p_gb_cell_id,
 U16 *p_cell_index
 );
/*! \fn rrm_return_et rrm_cellm_load_config_req_process_msg (void *p_api,U16 api_id,U16 data_len)
 *  \brief Handle cellm_load_config_req 
 *  \param p_api    Input Api buffer
 *  \param api_id   Incoming Api id
 *  \param data_len Incoming data length
 */
rrm_return_et
rrm_cellm_load_config_req_process_msg(
        void *p_api, /*Input Api buffer*/
        U16 api_id /*Incoming Api id*/
    /*SPR 17777 +-*/
        );

/*! \fn rrm_map_ld_lvl_to_ld_prcnt 
 *  \brief Map load level to load percentage
 *  \param ld_lvl
 *  \param *p_ld_actn
 */
U8 rrm_map_ld_lvl_to_ld_prcnt
(
 rrm_load_type ld_lvl,
 rrm_cell_load_action_t *p_ld_acnt
 );

/*! \fn rrm_map_rrm_oam_ld_lvl
 *  \brief Map RRM load level to Oam load lvl
 *  \param rrm_ld_lvl
 */
rrm_rs_load_lvl_et
rrm_map_rrm_oam_ld_lvl
(
 rrm_load_type rrm_ld_lvl
 );

/*! \fn rrm_cellm_start_ld_process_timer 
 *  \brief Start a periodic timer to process Cell load on expiry of Timer.
 *  \param *p_cell_ctx
 */
rrm_void_t
rrm_cellm_start_ld_process_timer
(
 rrm_cell_context_t *p_cell_ctx
 );
/* Code for MLB End */

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \fn rrm_meas_config_info_t* rrm_cellm_get_meas_config_info(
  rrm_cell_index_t cell_index)
 *  \brief This function returns address of meas_config_info within cell_context
 *  \param cell_index cell index
 */
rrm_meas_config_info_t*
rrm_cellm_get_meas_config_info
(
 rrm_cell_index_t cell_index
 );

/* HO config params */
rrm_ho_config_params_t*
rrm_cellm_get_ho_config_params
(
 rrm_cell_index_t cell_index
 );


rrm_return_et
rrm_send_rsu_to_neighbour
(
 U32 meas_id
 );

/* !\fn rrm_return_et rrm_map_rrm_x2ap_glb_cell_id(
   rrm_cell_global_id_eutra_t *p_gb_x2ap_cell_id, rrm_oam_eutran_global_cell_id_t *p_gb_rrm_cell_id)
 *   \brief  This function 
 *   \param p_gb_x2ap_cell_id
 *   \param p_gb_rrm_cell_id
 */
rrm_return_et rrm_map_rrm_x2ap_glb_cell_id
(
 rrm_cell_global_id_eutra_t *p_gb_x2ap_cell_id,
 rrm_oam_eutran_global_cell_id_t *p_gb_rrm_cell_id
 );

/* !\fn rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_rrm_stop_res
   (U32 enb_meas_id, x2_gb_enb_id_t *p_gb_enb_id, rrm_return_et resp, x2ap_rrm_cause_t *p_cause)
 *  \brief 
 *  \param enb_meas_id  enb meas id
 *  \param p_gb_enb_id  enb id
 *  \param resp         response
 *  \param p_cause      cause
 */
rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_rrm_stop_res
(
 U32 enb_meas_id,
 x2_gb_enb_id_t *p_gb_enb_id,
 rrm_return_et resp,
 x2ap_rrm_cause_t *p_cause
 );
/* !\fn rrm_return_et build_and_send_rsu_to_son(rrm_cell_context_t *p_cell_ctx)
 *  \brief thi function builds and sends rsu to son
 *  \param p_cell_ctx cell context
 */
rrm_return_et
build_and_send_rsu_to_son
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );

/* !\fn rrm_return_et build_and_send_rsu_to_oam(rrm_cell_context_t *p_cell_ctx)
 *  \brief thi function builds and sends rsu to oam
 *  \param p_cell_ctx cell context
 */
rrm_return_et
build_and_send_rsu_to_oam
(
 rrm_cell_context_t *p_cell_ctxt        /* cell context */
 );

/* !\fn rrm_return_et rrm_hndl_rsu_start_req_for_ncl(
   rrm_cell_context_t *p_cell_ctx, U32 periodic)
 * \brief This function handles rsu start request for ncl
 * \param p_cell_ctx cell context
 * \param periodic periodic
 */ 
rrm_return_et rrm_hndl_rsu_start_req_for_ncl
(
 rrm_cell_context_t *p_cell_ctx,
 U32 periodic
 );

/* !\fn rrm_void_t rrm_fill_x2_gb_enb_id (x2_gb_enb_id_t *p_x2_gb_enb_id,
   rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id)
 * \brief  This function fills x2 gnb enb id
 * \param p_x2_gb_enb_id
 * \param p_rrm_gb_enb_id
 */
rrm_void_t rrm_fill_x2_gb_enb_id
(
 x2_gb_enb_id_t     *p_x2_gb_enb_id,
 rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id
 );

/* !\fn rrm_bool_et rrm_is_bitset(
   U8 *p_bitmap, U8 bit_pos)
 * \brief This function checks for rrm bitset
 * \param p_bitmap bitmap
 * \param bit_pos bit position
 */
rrm_bool_et
rrm_is_bitset
(
 U8 *p_bitmap,
 U8 bit_pos
 );

/* !\fn rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_rrm_start_res(
   U32 enb_meas_id, x2_gb_enb_id_t *p_gb_enb_id, rrm_return_et resp, x2ap_rrm_cause_t *p_cause)
 * \brief This function builds and sends rrm x2ap rrm rsu start res
 * \param enb_meas_id enb meas id
 * \param p_gb_enb_id gb enb id
 * \param resp response
 * \param p_cause cause
 * \param resource meas response list
 */
rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_rrm_start_res
(
 U32 enb_meas_id,
 x2_gb_enb_id_t *p_gb_enb_id,
 rrm_return_et resp,
 x2ap_rrm_cause_t *p_cause,
 /*Rel10 CR 393 changes start*/
 x2ap_rrm_rsu_measurement_res_t *p_rrm_rsu_measurement_res_list
 /*Rel10 CR 393 changes end*/
 );
/* !\fn rrm_void_t rrm_setbit(U8 *p_bitmap, U8 bit_pos)
 *  \brief This function is used for set bit
 *  \param p_bitmap bit map
 *  \param bit_pos bit position
 */
rrm_void_t rrm_setbit
(
 U8 *p_bitmap,
 U8 bit_pos
 );
/* !\fn rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_stop_req(
   U32 enb_meas_id, rrm_x2_gb_enb_id_t *p_gb_enb_id)
 *  \brief This function buildds and sends rrm rsu stop request
 *  \param enb_meas_id end meas id
 *  \param p_gb_enb_id gb enb id
 */
rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_stop_req
(
 U32 enb_meas_id,
 rrm_x2_gb_enb_id_t *p_gb_enb_id
 );

/* !\fn rrm_void_t rrm_fill_rrm_x2_gb_enb_id(
   x2_gb_enb_id_t *p_x2_gb_enb_id, rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id)
 *  \brief This function fills end id
 *  \param p_x2_gb_enb_id
 *  \param p_rrm_gb_enb_id
 */
rrm_void_t rrm_fill_rrm_x2_gb_enb_id
(
 x2_gb_enb_id_t     *p_x2_gb_enb_id,
 rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id
 );

/* eICIC_PHASE_1_2_CHANGES_START */
/* !\fn rrm_void_t rrm_hndl_rsu_stop_req_for_ncl(rrm_cell_context_t *p_cell_ctx)
 *  \brief  This function handles rsu stop request for ncl
 *  \param p_cell_ctx cell context
 */
rrm_void_t
rrm_hndl_rsu_stop_req_for_ncl(
        rrm_cell_context_t *p_cell_ctx,
        intra_freq_cells_t *p_target_freq_cell 
        );

     /* SPR14123start*/
rrm_return_et
rrm_uem_find_free_cqi_index_for_victim(
		U16 cqi_config_index,
		U8  cqi_periodicity,
        rrm_cell_index_t cell_index,
#ifndef LTE_EMBMS_SUPPORTED
        rrm_abs_type_et    abs_type
#else
        rrm_abs_type_et    abs_type,
        U8                 check_for_embms 
#endif
		);
     /* SPR14123end*/

/* eICIC_PHASE_1_2_CHANGES_END */
/* !\fn rrm_return_et rrm_cellm_get_wideband_srs_resources(
   rrm_cell_index_t     cell_index, rrm_ue_srs_params_t  *p_rrm_ue_srs_params)
 *  \brief This function gets wideband srs resources
 *  \param cell_index cell index
 *  \param p_rrm_ue_srs_params pointer to rrm ue srs params
 */
rrm_return_et
rrm_cellm_get_wideband_srs_resources
(
 rrm_cell_index_t     cell_index,
 rrm_ue_srs_params_t  *p_rrm_ue_srs_params
 );

rrm_return_et
rrm_cellm_get_subband_srs_resources
(
 rrm_cell_index_t     cell_index,
 rrm_ue_srs_params_t  *p_rrm_ue_srs_params

 );

rrm_void_t rrm_update_link_status_ind(
        x2ap_rrm_x2_link_up_ind_st  *p_x2ap_link_up_ind,
        x2_gb_enb_id_t *p_peer_enb_id,
        U16 api_id);

rrm_void_t
update_pucch_pusch_resources_on_reconfig
(
 rrm_cell_context_t *p_cell_ctx,
 srs_t *p_srs_info_bfr_reconfig,
 pucch_t *p_cqi_srs_info_bfr_reconfig,
/* +- SPR 18268 */
 U16 max_num_ue_per_cell_bfr_reconfig
/* +- SPR 18268 */
 );
rrm_void_t
update_srs_config_res_on_reconfig
(
 rrm_cell_context_t *p_cell_ctx,
 rrm_srs_action_on_reconfig_et rrm_srs_action_on_reconfig
 );
rrm_void_t
update_cqi_sr_res_decision
(
 rrm_cell_context_t  *p_cell_ctx , 
 pucch_t *p_cqi_srs_info_bfr_reconfig,
 rrm_bool_t *p_update_cqi_info , 
 rrm_bool_t *p_update_sr_info,
/* +- SPR 18268 */
 U16 max_num_ue_per_cell_bfr_reconfig
/* +- SPR 18268 */
 );

rrm_void_t
update_cqi_sr_config_res_on_reconfig
(
 rrm_cell_context_t *p_cell_ctx,
 pucch_t *p_cqi_sr_info_bfr_reconfig,
/* +- SPR 18268 */
 U16 max_num_ue_per_cell_bfr_reconfig
/* +- SPR 18268 */
 );

rrm_srs_action_on_reconfig_et
rrm_get_srs_action_after_reconfig
(
 rrm_cell_context_t *p_cell_ctx,
 srs_t *p_srs_info_bfr_reconfig,
/*SPR 21223 START*/ 
 U16 max_num_ue_per_cell_bfr_reconfig
/*SPR 21223 END*/ 
 );

rrm_void_t
update_si_index_in_etws_warning_list
(
 YLIST *p_scheduled_etws_warning_list,
 U8 cmas_updated_index,
 rrm_cell_context_t *p_cell_ctx
 );

rrm_void_t
update_si_index_in_cmas_warning_list
(
 YLIST *p_scheduled_cmas_warning_list,
 U8 etws_updated_index,
 rrm_cell_context_t *p_cell_ctx
 );
U64
rrm_cellm_get_cell_thp_dl
(
 rrm_cell_index_t cell_index
 );

U64
rrm_cellm_get_cell_thp_ul
(
 rrm_cell_index_t cell_index
 );

rrm_daho_cell_config_info_t*
rrm_cellm_get_daho_cell_config_info_params
( 
 rrm_cell_index_t cell_index
 );
rrm_void_t
prepare_sr_index_list
(
 U16 *p_sr_index_list,
 U8 tdd_subfrm_config ,
 U8  sr_periodicity ,
 U16 sr_start_index,
 U8 *p_num_sr_indexes
 );
rrm_void_t
prepare_cqi_index_list
(
 U16 *p_cqi_index_list,
 U8 tdd_subfrm_config ,
 U8  cqi_periodicity ,
 U16 cqi_start_index,
 U16 *p_num_cqi_indexes
 );
rrm_void_t
map_cqi_ri_indexes_for_one_cqi_period
(
 U16 *p_cqi_indexval_for_configured_periodicity,
 U16 *p_ri_index,
 cqi_ri_map_info_per_ue *cqi_ri_table_map,
 U16 num_cqi_indexes,
 /*SPR 17777 +-*/
 U16 ri_start_index
 );
rrm_void_t
get_valid_cqi_periodicity_for_tdd
(
 U16 cqi_periodicity,
 U8 tdd_subfrm_config,
 U16 *p_new_cqi_periodicity_tdd
 );

rrm_return_et
rrm_cellm_ttt_update_ind_process_msg
(
 void *p_api,
 U16 api_id
    /*SPR 17777 +-*/
 );

/* Carrier_Aggregation_Start */
/* !\fn rrm_void_t
    \rrm_cellm_chk_and_change_x2_status
 *  \brief This function is used to update the x2_status
 *  \param lte_ncl_t *lte_ncl
 */

/* SPR 10779 start */
/* + SPR 17439 */
rrm_void_t
rrm_cellm_chk_and_change_x2_status (void);
/* - SPR 17439 */
/* SPR 10779 end */

/* !\fn rrm_return_et
    \rrm_update_cell_change_object  
 *  \brief This function is used to update cell change notification
 *  \param rrm_cell_index_t cell_index
 *  \param U64 cell_change_notify_bitmask
 */
rrm_void_t
rrm_update_cell_change_object
(
    rrm_cell_index_t cell_index,
    U64 cell_change_notify_bitmask
);

/* !\fn rrm_return_et
    \  rrm_cellm_enb_config_req_process_msg
 *  \brief This function is used to process enb config req api
 *  \param void *p_api
 *  \param U16 api_id
 *  \param U16 data_len
 */
rrm_return_et
rrm_cellm_enb_config_req_process_msg
(

    void *p_api
    /*SPR 17777 +-*/
 );

/* !\fn rrm_void_t
    \rrm_commit_ca_reconfig_mac_sch_info
 *  \brief This function is used to commit ca reconfig mac scheduler info
 *  \param operator_info_t *operator_info
 *  \param rrm_ca_config_t *p_ca_reconfig
 */
rrm_void_t
rrm_commit_ca_reconfig_mac_sch_info 
(
    operator_info_t *operator_info,
    rrm_ca_config_t *p_ca_reconfig
);

/* !\fn rrm_void_t
    \rrm_commit_ca_reconfig_info
 *  \brief This function is used to commit ca reconfig info
 *  \param operator_info_t *operator_info
 *  \param rrm_ca_config_t *p_ca_reconfig
 */
rrm_void_t
rrm_commit_ca_reconfig_info 
(
    operator_info_t *operator_info,
    rrm_ca_config_t *p_ca_reconfig
);

/* Carrier_Aggregation_End */

rrm_return_et
rrm_cellm_process_cell_start_adm_req_for_cell_unblock
(
 void *p_api,
 U16 api_id
    /*SPR 17777 +-*/
 );

rrm_return_et
rrm_cellm_process_cell_stop_adm_req_for_cell_block
(
 void *p_api,
 U16 api_id
    /*SPR 17777 +-*/
 );


rrm_bool_et
rrmcm_compare_geran_nbr(
        rrm_son_utran_geran_global_cell_id_t    *p_updated_nrt_cgi,
        geran_freq_ncl_t                        *p_geran_neigh
        );

rrm_enable_ue_measurement_et  get_enable_ue_measurement
(
 rrm_cell_index_t cell_index
 );

/*fix 866 start*/
U8  get_s_measure
(
 rrm_cell_index_t cell_index
 );
/*fix 866 end*/

/* DYNAMIC ICIC CHANGES */
/* !\fn rrm_void_t build_and_send_x2ap_rrm_load_information_req 
 *  \brief This function build and send x2ap_rrm_load_information_req 
 *  \param p_cell_context
 *  \param p_eutran_global_cell_id
 *  \num_cell
 *  \enodeb_type
 */
rrm_void_t build_and_send_x2ap_rrm_load_information_req
(
 rrm_cell_context_t                 *p_cell_context,
 rrm_oam_eutran_global_cell_id_t    *p_eutran_global_cell_id,
 U16                                num_cell,
 rrm_enb_type_et                    enodeb_type
 );

rrm_return_et compare_x2_reported_global_cell_id_with_serving(
        rrm_cell_global_id_eutra_t          *p_rcvd,
        rrm_global_cell_id_t                *p_strd);

rrm_void_t 
reconfigure_cell_center_region
(
 rrm_resource_partition_info_t      *p_resource_partition_info, 
 U32                                 prb_configured_in_srv_cell
 );

rrm_void_t 
get_minimum_interfered_prb_region
(
 U8         *p_start_region,
 U32        ce_region_start_rb,
 U32        num_of_rb,
 U32        cutoff_limit_for_ce,
 U32        prb_configured_in_srv_cell,
 U32        *p_new_start_index_for_ce_region
 );

rrm_return_et
rrm_build_and_send_x2ap_rrm_hii_req_to_macro_enb
(
 rrm_cellm_enb_id_hnode_t *p_enb_hnode,
 rrm_cell_context_t       *p_cell_ctxt        /* cell context */
 );


/* DYNAMIC ICIC CHANGES END*/

/*Multisector changes start: aditya*/
/*BUG 585 START*/
rrm_cell_index_t
rrm_get_cell_index_from_global_cell_id
(
 rrm_global_cell_id_t *global_cell_id
 );
/*BUG 585 END*/
/*Multisector changes end*/

U8
rrm_cellm_get_ps_ho_flag
(
 rrm_cell_index_t cell_index
 );

rrm_return_et
rrm_request_rim_info_req
(
 rrm_cell_context_t *p_cell_ctx,
 /* Spr 16211 Changes Start*/
 inter_rat_ncl_t *p_inter_rat_ncl, 
 /* Spr 16211 Changes End*/
 U8 pdu_ext_type
 );

U8
rrm_cellm_get_ps_ho_flag
(
 rrm_cell_index_t cell_index
 );
/*DYNAMIC ICIC START*/
rrm_return_et
rrmcm_rrm_process_cell_reconfig_for_icic_tmr
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );
rrm_void_t
commit_dynamic_icic_param
(
 rrm_cell_context_t *p_cell_ctx
 );

/*DYNAMIC ICIC END*/
rrm_load_type 
rrm_get_cur_load_level (rrm_cell_load_action_t *p_load_lvl, U8 cur_ld);

rrm_ue_position_et
rrm_cellm_get_initial_ue_loc_configured_in_cell
( 
 rrm_cell_index_t  cell_index
 ); 

rrm_bool_et
rrm_cellm_ce_partition_configured_in_cell
( 
 rrm_cell_index_t  cell_index
 ); 


/* RRM ES Start */
/* !\fn rrm_return_et rrm_fill_srv_cell_load_info(rrm_cell_context_t *p_cell_ctx,
   rrm_son_load_report_ind_t   *p_resrc_upd)
 *  \brief thi function fill serving cell load info
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rrm_fill_srv_cell_load_info
(
 rrm_cell_context_t *p_cell_ctx,        /* cell context */
 rrm_son_load_report_ind_t   *p_resrc_upd
 );
rrm_return_et
rrmcm_rrm_es_periodic_resrc_update
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );

U32
rrm_get_tm_sinr_threshold
(
 rrm_cell_index_t cell_index
 );

U32
rrm_get_tm_pathloss_threshold
(
 rrm_cell_index_t cell_index
 );

/* !\fn rrm_transmission_mode_table_t * rrm_get_tm_mode_table 
 *  \brief This function fills end id
 *  \param cell_index
 */
const rrm_transmission_mode_table_t*
rrm_get_tm_mode_table
(
 rrm_cell_index_t cell_index
 );


/* !\fn rrm_mac_beam_forming_info_t * rrm_cm_get_mac_beam_forming_info  
 *  \brief This function fills end id
 *  \param cell_index
 */
const rrm_mac_beam_forming_info_t*
rrm_cm_get_mac_beam_forming_info(
        rrm_cell_index_t cell_index
        );

rrm_enb_mode_t
rrm_cellm_get_fdd_tdd_mode(
        rrm_cell_index_t cell_index
        );

/* !\fn antenna_info_t * rrm_cellm_get_antenna_info
 *  \brief This function fills end id
 *  \param cell_index
 */
antenna_info_t*
rrm_cellm_get_antenna_info(
        rrm_cell_index_t cell_index
        );


/* !\fn rrm_enb_mode_t rrm_cm_get_mrrm_tm_mode_additional_info(
 *  \brief This function fills end id
 *  \param cell_index
 */
const rrm_tm_mode_additional_info_t*
rrm_cm_get_mrrm_tm_mode_additional_info(
        rrm_cell_index_t cell_index
        );
/* RRM ES End */

/*spr 7466 start*/
rrm_void_t
rrm_cellm_reset_dl_allocated_gbr_prb
(
 rrm_cell_index_t cell_index,
 /*SPR 18654 START*/
 U64 dl_allocated_gbr_prb
 /*SPR 18654 END*/
 );

rrm_void_t
rrm_cellm_reset_ul_allocated_gbr_prb
(
 rrm_cell_index_t cell_index,
 /*SPR 18654 START*/
 U64 ul_allocated_gbr_prb
 /*SPR 18654 END*/
 );
/*spr 7466 end*/

/*Bug_776_changes_start*/
rrm_void_t
rrm_cellm_increment_dl_allocated_ngbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               dl_allocated_ngbr_prb
 );

rrm_void_t
rrm_cellm_increment_ul_allocated_ngbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               ul_allocated_ngbr_prb
 );


U8
rrm_cellm_get_ul_allocated_gbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               erab_id
 );
U8
rrm_cellm_get_dl_allocated_gbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               erab_id
 );

rrm_void_t
rrm_cellm_decrement_ul_allocated_gbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               erab_id
 );

rrm_void_t
rrm_cellm_decrement_dl_allocated_gbr_prb_per_ue
(
 rrm_ue_context_t    *p_ue_context,
 U8               erab_id
 );


/*Bug_776_changes_end*/

/*spr 7996 start*/
/*SPR 17777 +-*/
/*spr 7996 end*/
/* UE positioning start */
U32 
rrm_convert_drx_sf_in_ms
(
 rrm_oam_long_drx_cycle_start_offset_et  long_drx_cycle
 );

#ifdef ENDC_ENABLED
U32 
rrm_convert_drx_sf_in_ms_r15
(
    rrm_oam_long_drx_cycle_start_offset_r15_et  long_drx_cycle
);
rrm_void_t
rrm_update_cellm_update_peer_gnb_pci_list
(
    U16  pci
);
#endif

U32 
rrm_get_drx_cycle_period
(
 rrm_cell_index_t                cell_index,
 U32                                     applied_drx_profile_index
 );
/* UE positioning end */


rrm_void_t
rrm_calculate_uplink_downlink_ue_count_for_scheduling( rrm_cell_context_t *,
        U8* , U8* );

rrm_return_et
rrm_schedule_downlink_uplink_ue( rrm_cell_context_t  *p_cell_context );

U32 rrm_get_l2_cell_prefs_stats_bitmap( U8 cell_index );

U32 rrm_get_l2_ue_prefs_stats_bitmap( U8 cell_index );

rrm_void_t rrm_set_cell_prefs_stats_bitmap( U8 cell_index , U32 cell_prefs_stats_bitmap );

rrm_void_t rrm_set_ue_prefs_stats_bitmap( U8 cell_index, U32 ue_prefs_stats_bitmap );

rrm_void_t
rrm_send_active_ue_report_enable_prefs( U8  cell_index );

U8 rrm_get_configurable_active_ue_report_timer( U8 cell_index );

/*spr 7984 start*/
rrm_bool_et
rrm_cellm_get_enable_cqi_mask_r9
(
 rrm_cell_index_t cell_index
 );
/*spr 7984 end*/
/*Start:SPR 8715*/
void
rrm_assign_num_tpc_id_dci_3_3a(
        rrm_cell_context_t *p_rrm_cell_ctx
        );
U8
rrm_cm_get_num_tpc_id_dci3(
        rrm_cell_index_t cell_index
        );
U8
rrm_cm_get_num_tpc_id_dci3a(
        rrm_cell_index_t cell_index
        );
/*End:SPR 8715*/
/* SPR 10450 Fix Start */

rrm_cell_drx_config_resources_t*
rrm_cellm_get_drx_config_resource
(
     rrm_cell_index_t cell_index
);
rrm_void_t
rrm_set_drx_resources_free
(
 rrm_cell_context_t *p_cell_ctx
 );
/* SPR 10450 Fix End */

/* CA: SCell Selection start */
rrm_void_t
rrm_fetch_scell_eligible_cells_from_cellm(
        rrm_cell_index_t         cell_index,
        rrm_cell_index_t        *scell_list
        );

rrm_void_t
rrm_sort_scell_list_by_cell_capacity(
        rrm_cell_index_t        *scell_list,
        rrm_cell_index_t        *scell_cap_list,
        U8                       count
        );
/* CA: SCell Selection end */

/* Carrier Aggregation start */

rrm_void_t
/*SPR 13316:start*/
rrm_cell_changed_from_CA_to_NON_CA( rrm_cell_index_t  cell_index, U16 trans_id );
/*SPR 13316:end*/


rrm_bool_et rrm_cell_have_scell_ue( rrm_cell_context_t  *p_cell_ctx );

YLIST * rrm_get_scell_list_of_cell( rrm_cell_index_t  pcell_index,
                                    rrm_cell_index_t  scell_index );


const void * rrm_scell_search_ue_by_ue_index( const YLNODE * p_scell_node );


int rrm_compare_scell_key_value( const void * p_seach_ue_index, const void * p_match_ue );

rrm_void_t rrm_add_ue_in_scell_list_of_cell( rrm_cell_index_t    pcell_index,
                                             rrm_cell_index_t    scell_index,
				             rrm_ue_context_t    *p_ue_context );
rrm_void_t rrm_delete_ue_in_scell_list_of_cell( rrm_cell_index_t   pcell_index,
                                                rrm_cell_index_t   scell_index,
                                                rrm_ue_context_t   *p_ue_context);
rrm_void_t rrm_reset_cell_config_transfer_object( rrm_cell_index_t  cell_index );

rrm_void_t rrm_notify_to_ue_mgr_about_cell_change( rrm_cell_change_notification_object_t *);

/* + SPR 17439 */
rrm_cell_context_t ** rrm_get_all_cells_context_at_eNB(void);

rrm_cell_index_t  rrm_get_cell_index_by_cell_id(void);
/* - SPR 17439 */

rrm_void_t
rrm_broadcast_resource_update_info_at_eNB( const cell_timer_buf_t   *p_timer_buf );

rrm_void_t
rrm_fill_report_resource_update_info_at_same_enodeB_cells( rrm_cell_index_t    cell_index,
                                                           rrm_cell_rsu_load_update_info_t 
                                                                               *p_rsu_report_info );


rrm_void_t
rrm_broadcast_carrier_agg_configuration_change_event_at_eNB( rrm_cell_index_t  cell_index,
                                                             U64               ca_config_bitmap,
                                                             U16               trans_id);
rrm_return_et
rrm_parse_cell_broadcasted_resource_load_msg( rrm_void_t    *p_api,
    /*SPR 17777 +-*/
                                              U16            data_len );

rrm_return_et
rrm_process_cellm_message( void                    *p_api/*pointer to input API buffer*/);
    /*SPR 17777 +-*/

 
rrm_return_et
rrm_send_mac_scheduler_ca_agg_config_param
(
    rrm_cell_context_t  *p_cell_ctx,
    operator_info_t     *operator_info
);


/* SPR 13383 changes start */

/* !\fn rrm_is_band_intra_contigious
 *  \brief This function return true if band in contigious 
 */
rrm_bool_et
rrm_is_band_intra_contigious( rrm_cell_context_t *p_cc_1_cell_ctxt , rrm_cell_context_t *p_cc_2_cell_ctxt );

/* !\fn rrm_calculate_intra_band_non_contigious_offset
 *  \brief This function create the intra band table indexes 
 *  \param p_band
 */
U16
rrm_calculate_intra_band_non_contigious_offset( U8 *p_band );


/* !\fn rrm_function_create_intra_band_non_contigious_index
 *  \brief This function create the intra band table indexes 
 *  \param p_cell_gb_context
 */
rrm_void_t
rrm_function_create_intra_band_non_contigious_index( rrm_global_context_t    *p_cell_gb_context );

/* !\fn rrm_function_create_intra_band_non_contigious_index
 *  \brief This function create the intra band table indexes 
 *  \param band
 *  \param cc_1_bandwidth
 *  \param cc_2_bandwidth
 *  \param *carrier_1_bw_class
 *  \param *carrier_2_bw_class
 *  \param *bandwidth_comb_set
 */
S8
rrm_find_intra_band_non_contigious_entry( U8                    band,
                                          rrm_oam_band_width_et cc_1_bandwidth,
                                          rrm_oam_band_width_et cc_2_bandwidth ,
                                          U8                    *carrier_1_bw_class ,
                                          U8                    *carrier_2_bw_class ,
                                          U32                   *bandwidth_comb_set );


/* !\fn rrm_calculate_intra_band_cont_offset 
 *  \brief This function calculate intraband tables offset
 *  \param p_pband
 */
U16
rrm_calculate_intra_band_cont_offset( U8  *p_band );


/* !\fn rrm_create_ca_intra_band_contigious_table_index 
 *  \brief This function create the intra band table indexes 
 *  \param p_cell_gb_context
 */
rrm_void_t
rrm_create_ca_intra_band_contigious_table_index(
                              rrm_global_context_t    *p_cell_gb_context );

/* !\fn rrm_function_create_sband_index 
 *  \brief This function create the sband index for inter band table 
 *  \param p_rrm_inter_band_tb
 */
rrm_void_t
rrm_function_create_sband_index( rrm_interband_table_t   *p_rrm_inter_band_tb );

/* !\fn rrm_calculate_sband_offset 
 *  \brief This function calculate the sband offset 
 *  \param pband
 */

U16 rrm_calculate_sband_offset( U8 *p_band );

/* !\fn rrm_create_ca_inter_band_table_index
 *  \brief This function create the inter band table indexes 
 *  \param p_cell_gb_context
 */
rrm_void_t
rrm_create_ca_inter_band_table_index(
                      rrm_global_context_t    *p_cell_gb_context );

/* !\fn rrm_get_matched_sband_info
 *  \brief This function return the matched sband in sband info table
 *  \param p_band
 *  \param scell_band
 */
U8*
rrm_get_matched_sband_info( rrm_sband_table_t  *p_band , U8 scell_band );

/* !\fn rrm_get_bitmask_corresponding_class
 *  \brief This function returns the bitmask corresponding enum bandwidth class
 *  \param bw_class
 */
U8
rrm_get_bitmask_corresponding_class( rrm_ca_band_width_class_r10_et  bandwidth_class );


/* !\fn rrm_get_bitmask_correspoding_bandwidth
 *  \brief This function returns the bitmask corresponding enum bandwidth 
 *  \param cell_bandwidth
 */
U8
rrm_get_bitmask_correspoding_bandwidth( rrm_oam_band_width_et  cell_bandwidth );

/* !\fn rrm_find_entry_of_scell_in_ca_intra_band_cont_table
 *  \brief This function search the sband entry in intra band table 
 *  \param carrier
 *  \param cc_1_bandwidth
 *  \param cc_2_bandwidth
 */
S8
rrm_find_entry_of_carrier_component_in_ca_intra_band_cont_table( U8                    carrier,
                                                                 rrm_oam_band_width_et cc_1_bandwidth,
                                                                 rrm_oam_band_width_et cc_2_bandwidth,
                                                                 U8                    *p_carreir_bw_class,
                                                                 U32                   *p_combination_set );

/* !\fn rrm_find_entry_of_carrier_component_in_ca_inter_band_table
 *  \brief This function finds the entry of sband in sband table of inter band ca
 *  \param carrier_1_band
 *  \param carrier_2_band
 *  \param cc_1_bandwidth
 *  \param cc_2_bandwidth
 *  \param *p_carrier_1_bw_class
 *  \param *p_carrier_2_bw_class
 *  \param *bandwidth_comb_set
 */
S8
rrm_find_entry_of_carrier_component_in_ca_inter_band_table( U8                    carrier_1_band , 
                                                            U8                    carrier_2_band ,
                                                            rrm_oam_band_width_et cc_1_bandwidth ,
                                                            rrm_oam_band_width_et cc_2_bandwidth ,
                                                            U8                    *p_carrier_1_bw_class,
                                                            U8                    *p_carrier_2_bw_class,
                                                            U32                   *bandwidth_comb_set);
/* SPR 13383 changes end */


/* Carrier Aggregation end */

/* CA getter Function Prototype : Start */
rrm_return_et 
get_scell_mac_main_config_ext_dl_num_harq_process(
		rrm_cell_index_t cell_index,
		U8 *dl_num_harq_process);

rrm_return_et 
get_scell_mac_main_config_ext_dl_i_mcs(
		rrm_cell_index_t cell_index,
		U8 *dl_i_mcs);

rrm_return_et 
get_scell_mac_main_config_ext_num_of_layer(
		rrm_cell_index_t scell_cell_index,
		U8 *num_of_layer,
		rrm_ue_context_t *p_rrm_ue_context);

rrm_return_et 
get_scell_mac_main_config_ext_code_book_index(
    /*SPR 17777 +-*/
		U8 *code_book_index);

rrm_return_et 
get_scell_mac_main_config_ext_user_location_type(
    /*SPR 17777 +-*/
		U8 *user_location_type);

rrm_return_et 
get_scell_mac_main_config_ext_scell_activate_status(
    /*SPR 17777 +-*/
		U8 *scell_activate_status);

U8 rrm_cellm_get_scell_phy_pdsch_config_pa
(
 rrm_cell_index_t cell_index
 );

S8 rrm_cellm_get_scell_non_ul_config_pdsch_ref_signal_power
(
 rrm_cell_index_t cell_index
 );

U8 rrm_cellm_get_scell_non_ul_config_pdsch_pb
(
 rrm_cell_index_t cell_index
 );

U8 rrm_cellm_get_scell_non_ul_config_phich_resource
(
 rrm_cell_index_t cell_index
 );

U8 rrm_cellm_get_scell_non_ul_config_phich_duration 
(
 rrm_cell_index_t cell_index
 );

U8 rrm_cellm_get_scell_non_ul_config_antenna_port_count
(
 rrm_cell_index_t cell_index
 );

U8 rrm_cellm_get_scell_non_ul_config_dl_bandwidth
(
 rrm_cell_index_t cell_index
 );

U16 rrm_cellm_get_scell_dl_carrier_freq
(
 rrm_cell_index_t cell_index
 );
rrm_cell_index_t rrm_cellm_get_scell_index
(
 rrm_cell_index_t cell_index
 );

rrc_phys_cell_id_t rrm_cellm_get_scell_pci 
(
 rrm_cell_index_t cell_index
 );


const rrm_global_cell_id_t *rrm_get_ecg_id_of_scell
(
 rrm_cell_index_t cell_index
 );

rrm_return_et
rrm_get_scell_deact_timer_for_rrc(rrm_scell_deactivation_timer_et scell_deactivation_timer, U8 *scell_deact_timer);
/* CA getter Function Prototype : End */

/* CA_Stage_3_Scell_TM_change: Start */
rrm_bool_et
rrm_check_cqi_mode_config_aperiodic_on_scell(rrm_cell_index_t cell_index);

U8
rrm_cellm_get_scell_phy_pdsch_config_pa_for_ce_ue
(
rrm_cell_index_t cell_index
);

rrm_return_et
uplink_power_control_common_r10
(rrm_cell_context_t                  *p_cell_context,
 uplink_power_control_common_v1020_t *p_ul_power_control_info_r10);

rrm_return_et
map_delta_f_pucch_format_3_r10_info(
          rrmc_delta_f_pucch_format_three_r10_et   delta_f_pucch_format_3_r10_val,
          uplink_power_control_common_v1020_t      *p_ul_power_control_info_r10);

rrm_return_et
map_delta_f_pucch_format_1b_cs_r10_info(
          rrmc_delta_f_pucch_format_one_b_cs_r10_et  delta_f_pucch_format_1b_cs_r10_val,
          uplink_power_control_common_v1020_t        *p_ul_power_control_info_r10);

rrm_return_et 
rrm_cellm_get_scell_antenna_port
( 
 rrm_cell_index_t cell_index, 
 U8 *antenna_port
 );
/* CA_Stage_3_Scell_TM_change: End */

/* CA Stage 3 Start */
rrm_return_et
rrmcm_check_updated_ncl_for_csg_id_and_bl_change(
            rrm_cell_context_t            *p_cell_ctx,
            /*15011_start*/            
            U16                             bitmask
            /*15011_end*/
            );

rrm_return_et
rrm_check_scell_csg_status_when_ue_non_mem_to_mem(
            rrm_ue_context_t   *p_ue_context
            );

rrm_return_et
rrm_check_scell_csg_status_when_ue_mem_to_non_mem(
            rrm_ue_context_t   *p_ue_context
            );
/* CA Stage 3 End */

/* CA_Stage3_Change: Start */
rrm_return_et
rrmcm_cm_cell_delete_resp
(
 rrm_cell_context_t *p_cell_ctx
);
/* CA_Stage3_Change: End */

/* CA: Stage3 HO code start */
rrm_void_t
rrm_fetch_scell_candidate_cell_index_from_cellm(
        U16                   *scell_pci_list,
        rrm_cell_index_t      *scell_index_list,
        rrm_cell_context_t    *p_cell_context
        );

rrm_void_t
rrm_get_cell_index_from_ncl_pci(
        U8                                   num_of_scell,
        rrm_cell_index_t                    *p_arr_cell_index,
        rrm_oam_eutran_global_cell_id_t      cell_id
        );
/* CA: Stage3 HO code end */
/*10329:start*/
rrm_return_et
rrm_cellm_enb_config_update_ind
(
 rrm_void_t *p_api/*pointer to api buffer*/
    /*SPR 17777 +-*/
 );
/*SPR 10329:end*/

/* Spr 16142 Fix Start */
rrm_return_et
rrm_cellm_handle_stop_tnl_discovery_ind(
        void *p_api/*pointer pt api buffer*/
        );
rrm_return_et
rrm_remove_tnl_discovery_trans_id
(
 rrm_global_enb_id_t     target_enb_id
 );
/* Spr 16142 Fix End */

/* SPR 10450 Fix Start */

rrm_cell_drx_config_resources_t*
rrm_cellm_get_drx_config_resource
(
     rrm_cell_index_t cell_index
);
rrm_void_t
rrm_set_drx_resources_free
(
 rrm_cell_context_t *p_cell_ctx
 );
/* SPR 10450 Fix End */
/* SPR 12387 Fix Start */
#ifdef TDD_MODE_FLAG
/*! \fn find_ul_opportunity_for_sr_periodicity_calc(rrm_oam_sub_frame_asgmnt_et sf_assignment)
 *  *  \brief This function is used to get the num of UL opportunity in tdd frame
 *   *  \param p_cell_context cell context
 *    */
U16 find_ul_opportunity_for_sr_periodicity_calc
(
   rrm_oam_sub_frame_asgmnt_et sf_assignment
 );
tdd_frame_structure_t rrm_cellm_get_scell_tdd_config(rrm_cell_index_t cell_index);
#endif
/* SPR 12387 Fix End */
/* eICIC_PHASE_1_2_CHANGES_START */
rrm_return_et
build_and_send_uem_get_aggressor_req
(
 rrm_cell_context_t *p_cell_ctx      
 );

rrm_return_et
rrm_cellm_find_and_apply_common_abs_pattern
(
 rrm_cell_context_t *p_cell_ctx      
);

rrm_return_et
rrm_cellm_cell_ue_update_aggressor_info(
        rrm_void_t *p_api,
        U16 api_id
    /*SPR 17777 +-*/
        );

rrm_return_et
rrm_uem_get_aggressor_resp_process_info(
        rrm_cell_context_t *p_cell_ctx
        );

rrm_void_t
build_and_send_uem_schedule_victim_ind
(
 rrm_cell_context_t *p_cell_ctx,
/* SPR 14576 start */
 rrm_victim_schedule_cause_et schedule_cause  
/* SPR 14576 end */
);

rrm_bool_et
rrm_cellm_is_aggressor_node
(
 rrm_cell_context_t *p_cell_ctx
);

rrm_bool_et
rrm_cellm_is_victim_node
(
 rrm_cell_context_t *p_cell_ctx
);
/* SPR14157start */
rrm_bool_et
rrm_check_for_x2_link_status
(
 rrm_cell_context_t *p_cell_ctx
);
/* SPR14157end */

rrm_bool_et
rrm_cellm_increase_abs
(
 U8   *p_abs_used_by_victim,
 U8   *p_abs_applied_at_aggr,
 U8    total_num_of_abs,
 U8   *p_new_abs_pattern
 );
/*SPR 14400 start*/
rrm_bool_et
rrm_cellm_decrease_abs
(
    /*SPR 17777 +-*/
 U8   *p_abs_sent_to_victim,
 U8   *p_meas_abs_applied,
 U8    total_num_of_abs,
 U8   *p_new_abs_pattern
 );
/*SPR 14400 end*/
rrm_void_t
rrm_cellm_tune_abs_pattern
(
 rrm_cell_context_t     *p_cell_ctxt
);

rrm_return_et rrm_cellm_fill_x2ap_eicic_load_info_ind 
(
 rrm_cell_context_t                 *p_cell_context,
 rrm_oam_eutran_global_cell_id_t    *p_eutran_global_cell_id,
 rrm_enb_type_et                     enodeb_type,
 rrm_eicic_load_info_type_et         load_info_type
);

rrm_void_t
rrm_cellm_process_abs_info_frm_agg_same_enb
(
 rrm_cell_context_t               *p_srvng_cell_ctx,
 rrm_cell_context_t               *p_neigh_cell_ctx,
 U8                                target_ncl_count,
 rrm_abs_pattern_status_et         abs_pattern_status
);

rrm_void_t
rrm_cellm_get_aggressor_pos_in_ncl
(
 rrm_cell_context_t *p_cell_ctx,
 U16                 aggressor_pci,
 U8                 *p_ncl_pos
);

/* bug_13898_start */
rrm_eicic_load_info_send_status_et build_and_send_x2ap_eicic_load_information
(
 rrm_cell_context_t             *p_cell_ctxt ,       /* cell context */
 rrm_eicic_load_info_type_et     load_info_type,
 U8                              target_ncl_count
);
 /* bug_13898_end */

rrm_return_et
rrm_send_rsr_start_req_to_targt_cell
(
 rrm_cell_context_t     *p_cell_ctxt,
 U8                   target_ncl_cnt,
 U8                   periodicity,
 /*Bug 586 Changes Start*/
 rrm_bool_et           abs_enable
 /*Bug 586 Changes End*/
);

rrm_void_t
rrm_convert_x2ap_cell_id_to_ncl_cell
(
   rrm_cell_global_id_eutra_t       x2ap_source_cell_id,
   rrm_oam_eutran_global_cell_id_t  *p_target_cell_id
);

rrm_void_t
rrm_cellm_get_target_cell_from_ncl
(
  rrm_cell_context_t              *p_cell_ctx, 
  U8                              *p_target_ncl_count,
  rrm_oam_eutran_global_cell_id_t  target_cell_id
);

rrm_void_t
rrm_cellm_process_abs_info_frm_aggressor
(
 rrm_cell_context_t               *p_cell_ctx,
 x2ap_rrm_li_abs_information_t    *p_x2ap_abs_info,
 U8                                target_ncl_count,
 rrm_abs_pattern_status_et         abs_pattern_status
);

rrm_return_et
rrm_cellm_find_and_update_common_abs_pattern
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
);

rrm_bool_et
rrm_check_abs_pattern_is_null
(
  U8 *p_abs_pattern
);
rrm_return_et
rrm_cellm_cell_ue_provision_reconfig_resp(
         rrm_void_t *p_api,/*pointer pt api buffer*/
         U16 api_id /*ADI id of incoming message*/
    /*SPR 17777 +-*/
         );
rrm_return_et
build_and_send_uem_eicic_provision_reconfig_req
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );

rrm_return_et build_and_send_l2_cell_config_req_for_eicic_reconfig
(
   mac_config_data_t            *p_mac_config_data,
   rrm_cell_index_t             cell_index,
   U16                          trans_id
);
rrm_return_et
rmcm_mac_abs_reconfig_req
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
);
rrm_bool_et
rrm_cellm_first_invoke_recieved
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
);
rrm_return_et
rrm_cellm_get_current_load_abs_pattern
( 
    rrm_cell_context_t *p_cell_ctx,        /* cell context */
    U8                 *p_load_abs_pattern
);

/* bug_13898_start */ 
rrm_void_t
rrm_print_abs_pattern
(U8 *p_abs_pattern);
/* bug_13898_end */ 
/* bug_13935_start */  
rrm_bool_et
rrm_compare_configured_abs_pattern_vd_new_abs
(
rrm_cell_context_t    *p_cell_ctx
);
/* bug_13935_end */  
/* bug_13972_start */  
rrm_return_et
rrm_build_and_send_eicic_load_info_req_to_macro_enb
(
   rrm_cellm_enb_id_hnode_t *p_enb_hnode,
 rrm_cell_context_t       *p_cell_ctxt        /* cell context */
);
/* bug_13972_end */

/*13987_fix_start*/
rrm_void_t
rrm_cellm_handle_eicic_timer_for_reconfig
(
    rrm_cell_context_t *p_cell_ctx
);
/*13987_fix_end*/

/* bug_14010_start */
rrm_return_et
rrm_cellm_get_highest_load_abs
(
    rrm_cell_context_t *p_cell_ctx,
    U8                 *p_new_abs
);

U8 rrm_get_num_of_abs_count
(
   U8    *p_abs_pattern,
   U8    total_num_of_abs
);
/* bug_14010_end */
/* SPR_15241_Start */
U32 rrm_cellm_get_dl_earfcn
(
 rrm_cell_index_t cell_index
  );

/* SPR_15241_End */
/* eICIC_PHASE_1_2_CHANGES_END */

/* SPR 16427 fix start */
rrm_return_et
rrm_get_sr_periodicity_in_ms(
        rrm_cell_context_t *p_cell_context,
        U8 *p_sr_periodicity_ms );

rrm_return_et
rrm_get_cqi_periodicity_in_ms(
        rrm_cell_context_t *p_rrm_cell_ctx,
        U8 *p_cqi_periodicity_ms );

rrm_bool_et
rrm_is_sr_cqi_collide(
        U8 sr_sub_frame_offset,
        U8 sr_periodicity_ms,
        U8 cqi_sub_frame_offset,
        U8 cqi_periodicity_ms );
/* SPR 16427 fix end */
/*SPR 16477 Fix Start*/
rrm_return_et
rrm_rrc_s1ap_mme_info_process_msg
(
    U8      *p_api,     
    U16     data_len 
    /*SPR 17777 +-*/
);

/* SPR_21459 Fix - Start */
rrm_bool_et
rrm_checks_for_x2_ho_based_on_gummei_info
(
    rrm_gu_group_id_list_t      *p_enb_gu_group_id_list,
    rrm_connected_mme_list_t    *p_connected_mme_list,
    rrc_rrm_ho_required_t       *p_rrc_rrm_ho_required,
    U8                          mme_id
);
/* SPR_21459 Fix - End */
/*SPR 16477 Fix Stop*/

/* Spr 16211 Changes Start*/
rrm_void_t 
rrm_get_delta_ncl_from_updated_nrt
(
 rrm_cell_context_t * p_cell_ctx, 
 inter_rat_ncl_t * p_inter_rat_ncl_added,
 inter_rat_ncl_t *p_inter_rat_ncl_deleted
 );

rrm_bool_et
rrm_compare_geran_neighbor
(
 geran_freq_ncl_t   *p_updated_geran_nbr,
 geran_freq_ncl_t   *p_geran_neigh
 );
rrm_bool_et
rrm_compare_utran_neighbor
(
 utran_freq_cells_t *p_updated_utran_nbr,
 utran_freq_cells_t *p_utran_neigh
 );
/* Spr 16211 Changes End*/

/* SPR_18881 Fix start */
rrm_void_t
initialize_cell_context_for_drx_resources
(
rrm_cell_context_t *p_cell_ctx
);
/* SPR_18881 Fix End */
/*SPR 18994 START*/
void  
rrm_set_report_charaterstics
(
 U32 *report_characteristics,
 rrm_report_characteristics_et characteristics
);

rrm_bool_et
rrm_check_set_report_charaterstics
(
  U32 *report_characteristics,
  rrm_report_characteristics_et characteristics
);
void
rrm_reset_report_charaterstics
(
  U32 *report_characteristics,
  rrm_report_characteristics_et characteristics
);          
void 
rrm_rearrange_report_characteristics_bits
(
 U32 *p_report_characteristics
);

/*SPR 18994 END*/

/* SPR 20221 Fix Start */
rrm_void_t
rrm_cellm_get_valid_n2_pucch_index
(
 rrm_cell_context_t           *p_cell_context,
 U16                          *p_n2_pucch_res_ind_tbl,
 U16                          *p_no_of_valid_n2_pucch_ind
 );
/* SPR 20221 Fix End */
/*SPR 22151 Fix Start*/
/*SPR 18654 START*/
rrm_return_et
rrm_get_rb_factor_for_bitrate
(
 rrm_cell_index_t cell_index,
 U16 * p_rb_factor_for_bitrate
);
/*SPR 18654 END*/
rrm_prb_t rrm_convert_bitrate_to_prb_dl
(
rrm_cell_index_t cell_index,
rrm_prb_t dl_bitrate
);
rrm_prb_t rrm_convert_bitrate_to_prb_ul
(
rrm_cell_index_t cell_index,
rrm_prb_t ul_bitrate
);
/*SPR 22151 Fix End*/
/* SPR 22248 MBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
rrm_return_et
rrmcm_sf_info_req_failure
(
    rrm_cell_context_t *p_cell_ctx        /* cell context */
);
#endif
/* SPR 22248 MBMS Changes Stop */

/* SPR_21459 Fix - Start */
rrm_connected_mme_node_t *
rrm_cellm_search_connected_mme_node_in_global_list
(
    U8              mme_id
);
/* SPR_21459 Fix - End */
/*SPR 22347 Fix Start*/
rrm_bool_et
rrm_is_sr_measgap_collide(
        U8 sr_offset,
        U8 sr_periodicity_ms,
        U8 measgap_offset,
        U8 measgap_periodicity_ms);
/*SPR 22347 Fix Stop*/

#endif /* _RRM_CELL_MGR_H_ */
