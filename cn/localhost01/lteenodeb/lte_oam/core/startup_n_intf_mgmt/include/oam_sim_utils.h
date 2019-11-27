/********************************************************************************
* File Name        : oam_sim_utils.h
* Description      : This file contains the utility function declaration for SIM 
*                    module
* Revision History :
* Date          Author              Reference              Comments
* May,2012      Brijesh Madolia     OAM Design Document    Initial Version
* June, 2014    Vivek K Gupta       Release 3.0            Changes for xml 
*                                                        add/delete frwk code
* June,2014     Ravi Gupta          SPR 10567 Fix
* July,2014     Aditi Singh         SPR 9620 Fix         Guard timer Added
* August,2014   Swetank             SPR 9614 FIX          New code implementation
* Aug 2014      Aditi singh         SPR 13116
* Oct 2014      Ankita Jain         SPR 13586
* Copyright (c) 2014, Aricent Inc. All Rights Reserved
*********************************************************************************/

#ifndef _OAM_SIM_UTILS_H_
#define _OAM_SIM_UTILS_H_

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include    <oam_defines.h>
#include    <oam_proto.h>
#include    <oam_types.h>
#include    <oam_config_tcb.h>

/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
typedef struct oam_thread_arg
{
    QDRIVER driver_id;
    QWAIT * p_qwait;
    U8      logical_id;
}thread_arg_t;


/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
void 
oam_create_virt_intf
(
    char *p_eth_name,
    char *p_eth_ip
);

oam_return_et 
oam_qcli_send_message
(
   void *p_msg,
   UInt16 trans_id,
   UInt16 msg_size
);

oam_return_et 
oam_maintain_tcb
(
   UInt16 trans_id,
   oam_trans_type_et trans_type,
   UInt16 msg_size,
   void *p_api_buf
);

/* SPR_9620_CHANGES_START */
oam_return_et
oam_maintain_tcb_with_timer
(
   UInt16 trans_id,
   UInt16 sub_module_id,
   oam_trans_type_et trans_type,
   UInt16 msg_size,
   UInt8 *p_api_buf
);
/* SPR_9620_CHANGES_END */

/*! \fn oam_return_et oam_shut_down_single(char * )
 *  \brief shut down the paticular process 
 *  \param p_proc_name	     pointer to name of the process
 */
oam_return_et 
oam_shut_down_single
(
    const char *p_proc_name
);

oam_return_et 
oam_user_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 );

oam_return_et 
oam_layer_intf_process_timer
(
 
oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 );

#ifdef OAM_SHARED_MEM_LOGGER
rrm_void_t
*oam_logger_init(	
		oam_void_t *p_oam_logger_init_data);

S32
oam_logger_process_msg
(
 oam_void_t *p_api_buf/*pointer to api buffer*/,
 oam_void_t *p_gl_ctx/*pointer to global logger context*/);

oam_void_t
oam_logger_process_timer_msg
(
 QTIMER timer_id,
 oam_void_t *p_buf,
 oam_void_t *p_context);
#endif

oam_return_et 
oam_user_intf_process_msg
(
   void *p_api_buf
);
oam_return_et 
oam_layer_intf_process_msg
(
   void *p_api_buf
);

oam_return_et 
oam_send_proc_sup_req
(
    U16                 dest_module_id,  /* Source module identifier */
    U16                 transaction_id  /* Interface transaction identifier */
);

/********************************PM_CODE**************************************/
oam_return_et 
oam_PM_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 );

oam_return_et 
oam_PM_intf_process_msg
(
   void *p_api_buf
);
/********************************PM_CODE**************************************/
oam_return_et
oam_fm_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
);
oam_return_et
oam_compose_and_send_inform_api
(
     oam_inform_api_t **oam_inform_api,
     UInt16 num_of_parent,
     UInt8 *event_code,
     UInt16 dest_id,
     UInt16  sub_module_id
);

oam_return_et oam_inform_value_change_event
(
    oam_inform_api_t  *p_input_inform_api, 
    UInt16             sub_module_id,
    UInt16             dest_module_id
);

oam_return_et oam_maintain_internal_tcb
(
    UInt16            trans_id,
    oam_trans_type_et trans_type,
    UInt16            msg_size,
    UInt8            *p_api_buf
);

oam_return_et
oam_handle_notification_ind_resp
(
    void  *p_buf 
);

extern void
oam_son_execute_reboot
(
    void *p_api_buf
);

extern void
oam_son_reboot_system
(
 void 
);


extern char g_nmm_tmp_str[256];

void 
oam_convert_int_to_byte_string
(
    UInt8  *dest, 
    UInt32  source
);

void 
oam_convert_short_to_byte_string
(
    UInt8  *dest,
    UInt32  source
);

void 
oam_handle_invalid_startup_config
(
/* + SPR 17439 */
void
/* - SPR 17439 */
);

oam_return_et
oam_fill_mcc_mnc_from_plmnid(char *temp ,rrm_oam_cell_plmn_info_t *plmnid );

oam_return_et
oam_process_tcb_for_layer_resp(UInt8* p_api_buf,
                               UInt16 *tid,
                               UInt16 *dest_id);

oam_return_et
oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id
(
    UInt16 trans_id,
    UInt16 *tid,
    UInt16 *dest_id
    );

void oam_maintain_system_state(oam_system_state_event_et oam_event);


UInt8 oam_normalize_indexed_param(Char8 *indexed_param,
                                  Char8 **normalized_param);

#ifdef OAM_SON_ENABLE
void oam_set_son_parameter_update_for_reconfig
(
  oam_son_rach_opt_parameter_update_req_t oam_son_rach_opt_parameter_update_req ,
  oam_rrm_cell_reconfig_req_t *p_oam_rrm_cell_reconfig_req
);

void oam_set_son_parameter_update_for_cell_info
(
  oam_son_rach_opt_parameter_update_req_t oam_son_rach_opt_parameter_update_req ,
  oam_son_cell_info_ind_t *p_oam_son_cell_info_ind
);
void
oam_set_bitmask_for_son_parameter_update_req
(
        oam_rrm_cell_reconfig_req_t *p_oam_rrm_cell_config_req,
        oam_son_rach_opt_parameter_update_req_t oam_son_rach_opt_parameter_update_req
);

/* SPR_13116_CHANGE_START */
/* SPR 17777 fix */
oam_return_et
oam_son_get_loglevel_resp
(
 son_oam_get_log_level_resp_t *p_son_oam_get_log_level_resp,
 void* p_api_buf
 );
/* SPR_13116_CHANGE_END */

/* SPR-16334 START */
oam_void_t
oam_print_plmn_id
(
    oam_cell_plmn_info_t *p_plmn_id
);

oam_void_t
oam_print_enb_id
(
    oam_global_enb_id_t *p_enb_id
);

oam_void_t
oam_x2_convert_l3_plmn_id_to_oam_plmn_id
(
    x2ap_plmn_identity_t *p_l3_plmn_id,
    oam_cell_plmn_info_t *p_oam_plmn_id
);

oam_void_t
oam_x2_convert_l3_enbid_to_oam_enbid
(
    x2_gb_enb_id_t      *p_l3_enbid,
    oam_global_enb_id_t *p_oam_enbid
);

oam_void_t
oam_x2_convert_ip_addr_to_l3_ip_addr
(
    UInt8  *p_oam_ip_addr,
    UInt8  *p_l3_ip_addr
);

oam_return_et
oam_x2_convert_ipv6_addr_to_l3_ipv6_addr
(
    UInt8  *p_oam_ipv6_addr,
    UInt8  *p_l3_ipv6_addr
);

oam_return_et
oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr
(
    UInt8  *p_l3_ipv6_addr,
    UInt8  *p_oam_ipv6_addr
);

oam_void_t
oam_x2_convert_l3_ip_addr_to_oam_ip_addr
(
    UInt8  *p_l3_ip_addr,
    UInt8  *p_oam_ip_addr
);

/* SPR-16334 END */


/* SPR-13586 START */
void oam_extract_ci_from_cell_identity(UInt16 *out_ci,
                                        UInt8 *p_cell_identity);

void oam_extract_lac_from_cell_identity(UInt16 *out_lac,
                                        UInt8 *p_cell_identity);

void oam_extract_value_from_bsic(UInt8 *out_bsic,
                                 son_geran_physical_cell_id_t *p_cell_identity);
/* SPR-13586 END */


#endif /* OAM_SON_ENABLE */

/* SPR_13116_CHANGE_START */
/*SPR 13617 STARt*/
oam_return_et
oam_l3_get_loglevel_resp
(
 oam_rrc_get_log_level_resp_t *p_rrc_get_log_level_resp,
 void *p_api_buf
);

/*SPR 13617 END*/
oam_return_et
oam_l3_s1ap_get_loglevel_resp
(
 oam_s1ap_get_log_level_resp_t *p_s1ap_get_log_level_resp,
 void *p_api_buf
);

oam_return_et
oam_l3_x2ap_get_loglevel_resp
(
 oam_x2ap_get_log_level_resp_t *p_x2ap_get_log_level_resp,
 void *p_api_buf
);


oam_return_et oam_send_get_log_level_resp
(
 UInt16 trans_id,
 UInt16 dest_id,
 oam_error_code_et *error_code
 );

oam_return_et
oam_rrm_get_loglevel_resp
(
 oam_rrm_get_log_level_resp_t   *p_oam_get_loglevel_rrm,
 void* p_api_buf
 );

/* SPR_13116_CHANGE_END */

/* SPR 15604 Fix Start */
void
oam_extract_cid_from_cell_identity(UInt16 *p_out_cid,
                                   UInt8 *p_cell_identity);

void
oam_extract_rncid_from_cell_identity(UInt16 *p_out_rncid,
                                     UInt8 *p_cell_identity);  

/* SPR 15604 Fix End */

void
oam_extract_cid_from_utran_cell_identity(UInt16 *p_out_cid,
                                         UInt8 *p_cell_identity);

void
oam_extract_rncid_from_utran_cell_identity(UInt16 *p_out_rncid,
                                           UInt8 *p_cell_identity);

void oam_encode_rncid_to_utran_cell_identity(UInt8 *p_cell_identity,
                                             UInt16 rncid);

void oam_encode_cid_to_utran_cell_identity(UInt8 *p_cell_identity,
                                           UInt16 cid);
/*CSR 00053551 FIX START*/ 
#define OAM_EGTPU_MAX_STATS_ID 39


typedef struct oam_egtpu_stats_res
{
UInt8 result;
UInt8 statsType;
UInt16 length;
UInt32 value[OAM_EGTPU_MAX_STATS_ID];
}oam_egtpu_stats_res_t;


oam_return_et oam_get_egtpustats
(
 char *stats_type,
 char *reset,
 UInt16 trans_id
);

oam_return_et
oam_parse_egtpu_stats_resp
(
    oam_egtpu_stats_res_t *p_getStatsResp,
    U8  *p_src,
    SInt32 length_left,
    UInt32 p_length_read
);

/* SPR 21412 Fix Start */
/* SPR-21738 Start */
/* Code Deleted */
SInt8
oam_get_min_rrm_cell_config_idx(UInt8 cell_action);
/* Code Deleted */
/* SPR-21738 End */
/* SPR 21412 Fix End */
SInt8       
oam_get_rrm_cell_config_idx(UInt8 cell_index);

SInt8
oam_get_son_cell_nr_list_idx(UInt8 cid);


SInt8
oam_get_son_neigh_list_in_use_idx(UInt8 cid);
/* Start: SPR 10567 */
SInt8
oam_get_cell_idx_from_cell_identity
(
  UInt8 *p_cgi
);
/*End: SPR 10567 */

SInt8
oam_get_son_inter_freq_carrier_idx(UInt8 cid);

SInt8
oam_get_son_serving_cell_idx(UInt8 cid);


oam_return_et
oam_handle_egtpu_stats_resp
(
    /* SPR 17777 fix code removed */
    UInt16                trans_id,
    UInt16                dest_id,
    oam_egtpu_stats_res_t *p_getStatsResp,
    oam_error_code_et     *p_err
);
/*CSR 00053551 FIX END*/ 

    /* RT SPR 9019 FIX START */
oam_freq_type_et oam_find_arfcn_in_available_freqs(UInt32 arfcn,
                                  oam_arfcn_type_et arfcn_type,oam_rrm_cell_config_req_t *p_rrm_cell_config);
    /* RT SPR 9019 FIX END */
/* SPR 16567 START */
oam_return_et oam_send_enb_config_update(void);
/* SPR 16567 END */

oam_return_et
oam_send_call_trace_resp(
   oam_return_et response,
   UInt16 transaction_id,
   UInt16 dest_id,
   oam_error_code_et *p_error_code
   );


Char8* oam_mac_resp_code_to_str(UInt16 resp_code);

Char8* oam_rrc_resp_code_to_str(UInt16 resp_code);

Char8* oam_rrm_resp_code_to_str(SInt32 resp_code);

void oam_handle_init_config_failure(UInt16 alarm_id,
                                    Char8 *p_add_txt,
                                    Char8 *p_add_info);

SInt32 oam_is_duplicate_cell_identity(UInt8 *p_cell_identity);

#ifdef OAM_SON_ENABLE
Char8* oam_son_resp_code_to_str(son_error_et err_code);
#endif

/* SPR 11001 FIX START */
void oam_get_err_string_n_err_code(oam_error_code_et err_code,
                                   UInt16 *usr_err_code,
                                   Char8 **err_str);
/* SPR 11001 FIX END */
SInt8
oam_get_cell_plmnlist_data_idx(UInt8 cid);

oam_return_et
oam_process_tcb_for_layer_resp_ext(UInt8* p_resp,
                                   UInt16 *tid,
                                   UInt16 *dest_id,
                                   xmlNode **p_p_orig_xml_parent,
                                   xmlNode **p_p_req_xml_parent,
                                   xmlNode ***p_p_p_xml_struct_addr);
oam_return_et
oam_maintain_tcb_ext(UInt16 trans_id,
                     oam_trans_type_et trans_type,
                     UInt16 msg_size,
                     UInt8 *p_api_buf,
                     xmlNode *p_orig_parent,
                     xmlNode *p_req_parent,
                     xmlNode **p_p_xml_struct_addr);

xmlNode*
oam_add_new_xsd_validated_xml_child(xmlNode *p_parent,
                                    UInt16 loop,
                                    UInt8 *p_value);

void oam_update_xml_dom(oam_return_et layer_resp,
                        xmlNode *p_orig_parent,
                        xmlNode *p_updated_parent,
                        xmlNode **p_parent_xml_struct_addr);
/* SPR 17777 fix */
oam_return_et oam_cleanup_layers(UInt16 trans_id);

oam_return_et
oam_check_param_input_validity(Char8 *param_val_pair);

/* SPR 13024 FIX START */
SInt8
oam_get_earfcn_pci_select_arr_idx(UInt8 cid);
/* SPR 13024 FIX END */
/* SPR 15604 Fix Start */
void oam_encode_rncid_to_cell_identity(UInt8 *p_cell_identity, UInt16 rncid);

void oam_encode_cid_to_cell_identity(UInt8 *p_cell_identity, UInt16 cid);
/* SPR 15604 Fix End */

/*SPR 9614 FIX start*/
#ifdef OAM_SON_ENABLE
oam_return_et
oam_inform_son_pci_conflict_confusion_ind_hems
(
  oam_son_pci_conflict_confusion_ind_t *p_pci_conflict_confusion,
  oam_error_code_et *p_error_code
);

oam_return_et
oam_populate_parameter_name
( 
  oam_son_pci_conflict_confusion_ind_t *p_pci_conflict_confusion,
  UInt8 *param_name1,
  UInt8 *param_name2,
  UInt8 type
);
#endif
/* SPR 13120 FIX START */
void oam_update_xml_subtree(xmlNode *p_new_subtree, xmlNode *p_orig_subtree);
/* SPR 13120 FIX END */

/* SPR 14167 FIX START */
void oam_clean_nliu_data(UInt8 cell_arr_idx);
/* SPR 14167 FIX END */

/* SPR 14153 FIX START */
xmlNode* oam_make_default_multi_instance_obj(Char8* obj_name, UInt16 inst_id);

xmlNode* oam_make_plmnlist_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_carrier_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_utran_fdd_freq_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_geran_freq_group_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_lte_cell_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_umts_def_xml_obj(UInt16 inst_id);

xmlNode* oam_make_gsm_def_xml_obj(UInt16 inst_id);

xmlNode* oam_xml_add_prev_sibling(xmlNode *p_node, Char8* sibling_name,
                                  Char8* content);

xmlNode* oam_xml_add_next_sibling(xmlNode *p_node, Char8* sibling_name,
                                  Char8* content);

xmlNode* oam_xml_add_first_child(xmlNode *p_parent, Char8* child_name,
                                 Char8* content);
/* SPR 14153 FIX END */
 

/* SPR 16200 FIX START */
oam_return_et
oam_validate_abs_pattern(Char8* abs_pattern,
                         Char8* dup_mode,
                         rrm_oam_sub_frame_asgmnt_et tdd_config);
/* SPR 16200 FIX END */

/* SPR 21889 Fix Start */ 
oam_return_et
oam_transfer_file(Char8 *p_filename,
        UInt8 instance,
        oam_bool_et is_file_pull);
oam_return_et
oam_fill_cksum(Char8 *p_filename, UInt64 *p_cksum);
/* SPR 21889 Fix End */ 
 
#endif /* _OAM_SIM_UTILS_H_ */
