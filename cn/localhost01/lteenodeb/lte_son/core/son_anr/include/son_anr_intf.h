/*! \file son_anr_intf.h
 *  \brief This file contains the  structures specific to ANR interface with other modules(SMIF)
 *  \date  Feb, 2012
 *  \author Komal/Sonal
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_ANR_INTF_H_
#define _SON_ANR_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_nmm_apps_intf.h <>
 */
#include <son_nmm_apps_intf.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \  struct son_anr_enable_req_t
 *  \brief It is used to globally enable ANR if cell id is not present
 *         or to enable ANR for the cell whose id is present in the request
 *  \param transaction_id              Transaction identifier
 *  \param enable_req				    Enable Request from OAM
 */
typedef struct _son_anr_enable_req_t
{
    son_u16 transaction_id;
    son_oam_anr_enable_req_t enable_req;
}son_anr_enable_req_t;

/*! \  struct son_anr_enable_res_t
 *  \brief Enable response structure
 *  \param transaction_id              Transaction identifier
 *  \param enable_res				   Enable Response to OAM
 */
typedef struct _son_anr_enable_res_t
{
    son_u16 transaction_id;
    son_oam_anr_enable_resp_t enable_res;
}son_anr_enable_res_t;

/*! \  struct son_anr_disable_req_t
 *  \brief ANR Disable Request Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_req                      Disable Request from OAM
 */
typedef struct _son_anr_disable_req_t
{
    son_u16 transaction_id;
    son_oam_anr_disable_req_t disable_req;
}son_anr_disable_req_t;

/*! \  struct son_anr_disable_res_t
 *  \brief ANR Disable Response Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_res                      Disable Response to OAM
 */
typedef struct _son_anr_disable_res_t
{
    son_u16 transaction_id;
    son_oam_anr_disable_resp_t disable_res;
}son_anr_disable_res_t;

/*! \  struct son_anr_nr_add_req_t
 *  \brief This is the request structure to add NR entry in NRT
 *  \param transaction_id    Transaction identifier
 *  \param add_req           NR add request from OAM
 */
typedef struct _son_anr_nr_add_req_t
{
    son_u16 transaction_id;
    son_oam_nr_add_req_t add_req;
} son_anr_nr_add_req_t;

/*! \  struct _son_anr_nr_add_res_t
 *  \brief This is the response structure
 *  \param transaction_id     Transaction identifier
 *  \param add_res            NR add response to OAM
 */
typedef struct _son_anr_nr_add_res_t
{
    son_u16 transaction_id;
    son_oam_nr_add_resp_t add_res;
} son_anr_nr_add_res_t;

/*! \  struct son_anr_nr_update_req_t
 *  \brief This is the request structure to update NR entry in NRT
 *  \param transaction_id    Transaction identifier
 *  \param upd_req           Update Request from OAM
 */

typedef struct _son_anr_nr_update_req_t
{
    son_u16 transaction_id;
    son_oam_nr_update_req_t upd_req;
}son_anr_nr_update_req_t;

/*! \  struct _son_anr_nr_update_res_t
 *  \brief This is the response structure
 *  \param transaction_id    Transaction identifier
 *  \param upd_res           Update Response to OAM
 */
typedef struct _son_anr_nr_update_res_t
{
    son_u16 transaction_id;
    son_oam_nr_update_resp_t upd_res;
}son_anr_nr_update_res_t;

/*! \  struct son_anr_nr_delete_req_t
 *  \brief This is the request structure to delete NR entry in NRT
 *  \param transaction_id    Transaction identifier
 *  \param del_req           Delete request from OAM
 */

typedef struct _son_anr_nr_delete_req_t
{
    son_u16 transaction_id;
    son_oam_nr_delete_req_t del_req;
} son_anr_nr_delete_req_t;

/*! \  struct _son_anr_nr_delete_res_t
 *  \brief This is the response structure
 *  \param transaction_id    Transaction identifier
 *  \param del_res           Delete response to OAM
 */
typedef struct _son_anr_nr_delete_res_t
{
    son_u16 transaction_id;
    son_oam_nr_delete_resp_t del_res;
} son_anr_nr_delete_res_t;

/*! \  struct son_anr_nr_retrieve_req_t
 *  \brief This is the request of NR retrieval
 *  \param transaction_id    Transaction identifier
 *  \param rtrv_req          Retrieve request
 */

typedef struct _son_anr_nr_retrieve_req_t
{
    son_u16 transaction_id;
    son_oam_nr_retrieve_req_t rtrv_req;
}son_anr_nr_retrieve_req_t;

/*! \  struct son_anr_nr_retrieve_res_t
 *  \brief This is the response of NR retrieval
 *  \param transaction_id    Transaction identifier
 *  \param rtrv_res          Retrieve response
 */
typedef struct _son_anr_nr_retrieve_res_t
{
    son_u16 transaction_id;
    son_oam_nr_retrieve_resp_t rtrv_res;
}son_anr_nr_retrieve_res_t;

/*! \   son_anr_updated_nrt_info_t from son_oam_anr_updated_nrt_info_t */
typedef son_oam_anr_updated_nrt_info_t son_anr_updated_nrt_info_t;

/*! \   struct son_anr_scan_req_t
 *  \brief This is the NR Scan Request structure
 *  \param  eutran_scan_req Eutran Scan Request
 *  \param  utran_scan_req  Utran Scan Request
 */
typedef struct _son_anr_scan_req_t
{
    son_nmm_start_scan_req_t eutran_scan_req;
    son_nmm_start_scan_req_t utran_scan_req;
}son_anr_scan_req_t;

/*! \  struct son_anr_scan_res_t
 *  \brief This structure is used to send ANR SCAN response to SMIF
 *  \param transaction_id    Transaction Identifier
 *  \param scan_res          ANR SCAN response of type son_oam_nr_scan_res_t
 */
typedef struct _son_anr_scan_res_t
{
    son_u16 transaction_id;
    son_oam_nr_scan_resp_t scan_res;
}son_anr_scan_res_t;

/*! \  struct son_anr_pci_in_use_list_req_t
 *  \brief This structure is used to request the currently used PCIs
 *  \param  transaction_id  Transaction Identifier
 *  \param  earfcn  EARFCN frequency
 *  \param  level   Level number
 *  \param  is_nr_nbrs_pci_reqd Is NR neighbors PCI also required.
 *  \param  src_cgi Source Cell ID
 */
typedef struct _son_anr_pci_in_use_list_req_t
{
    son_u16 transaction_id;
    son_u32 earfcn;
    son_nbr_level_et level;
    son_intra_rat_global_cell_id_t src_cgi; /*Added for SPR-12423*/
}son_anr_pci_in_use_list_req_t;

/* Spr 17753 Changes Start*/
/*! \  struct son_used_pci_info_t
 *  \brief This structure contains the info regarding  
 *         currently used PCIs with first and second degree nbr
 *  \param served_cell_pci serving cell pci  
 *  \param num_pci_in_use_w_first_degree No of pci used by first degree nbr  
 *  \param pci_list_level_1 pci list used by first degree nbr 
 *  \param pci_list_level_2 Unique pci list used by Second degree nbr 
 */
typedef struct _son_used_pci_info_t
{
    son_u16 served_cell_pci;
    son_u16 num_pci_in_use_w_first_degree;
    son_u16 pci_list_level_1[SON_MAX_PCI];
    son_bool_et pci_list_level_2[SON_MAX_PCI];
}son_used_pci_info_t;
/* Spr 17753 Changes End*/
/*! \  struct son_anr_pci_in_use_list_res_t
 *  \brief This structure is used to inform PCI reselection module
 *         about the currently used PCIs
 *  \param  transaction_id       Transaction Identifier
 *  \param  result               Result
 *  \param error_code                  Error Code in case of failure
 *  \param pci_list              List of in use PCI 
 *  \param src_cgi               Source Cell ID
 */
typedef struct _son_anr_pci_in_use_list_res_t
{
    son_u16 transaction_id;
    son_return_et result;
    son_error_et error_code;
    son_bool_et pci_list[SON_MAX_PCI];
    son_intra_rat_global_cell_id_t src_cgi; /*Added for SPR-12423*/
    /* Spr 17753 Changes Start*/
    son_used_pci_info_t in_use_pci_list; 
    /* Spr 17753 Changes End*/
}son_anr_pci_in_use_list_res_t;

/*! \  struct son_anr_meas_config_req
 *  \brief This structure is used by OAM to send measurement
 *      configuration request required to get the UE measurement
 *      reports
 *  \param  transaction_id      Transaction Identifier
 *  \param  config_req          Meas config req data
 */
typedef struct _son_anr_meas_config_req_t
{
    son_u16 transaction_id;
    son_oam_anr_meas_config_req_t config_req;
}son_anr_meas_config_req_t;

/*! \  struct son_anr_meas_config_res
 *  \brief This structure is used to send measurement
 *      configuration request response to OAM
 *  \param  transaction_id      Transaction Identifier
 *  \param  config_req          Meas config res data
 */
typedef struct _son_anr_meas_config_res_t
{
    son_u16 transaction_id;
    son_oam_anr_meas_config_res_t config_res;
}son_anr_meas_config_res_t;

/*! \  struct son_rrm_meas_config_req_t
 *  \brief Structure storing the Meas config request data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param config_req           Meas config Request to RRM
 */
typedef struct _son_rrm_meas_config_req_t
{
    son_u16 transaction_id;
    rrm_son_meas_config_req_t config_req;
}son_rrm_meas_config_req_t;

/*! \  struct son_rrm_meas_config_res_t
 *  \brief Structure storing the Meas config response data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param config_res           Meas config Response from RRM
 */
typedef struct _son_rrm_meas_config_res_t
{
    son_u16 transaction_id;
    rrm_son_meas_config_res_t config_res;
}son_rrm_meas_config_res_t;

/*! \  rrm_son_meas_results_ind_t son_rrm_meas_results_ind_t
 */
typedef rrm_son_meas_results_ind_t son_rrm_meas_results_ind_t;

/*! \  struct _son_anr_pci_reg_dereg_req_t
 *  \brief This request structure is used to register/deregister
 *      the APPS(PCI) Module with ANR
 *  \param transaction_id        Transaction identity
 *  \param pci_mode              PCI mode
 *
 */
typedef struct _son_anr_pci_reg_dereg_req_t
{
    son_u16 transaction_id;
    son_pci_mode_et pci_mode;
}son_anr_pci_reg_dereg_req_t;

/*! \  struct _son_anr_pci_reg_dereg_res_t
 *  \brief This structure is used to respond for register/deregister
 *         request of APPS(PCI) module
 *  \param transaction_id      Transaction identity
 *  \param pci_mode            pci mode
 *  \param result              Result value
 *  \param error_code          error code
 *
 */
typedef struct _son_anr_pci_reg_dereg_res_t
{
    son_u16 transaction_id;
    son_pci_mode_et pci_mode;
    son_return_et result;
    son_error_et error_code;
}son_anr_pci_reg_dereg_res_t;

/*! \  son_anr_pci_conflict_confusion_ind_t
 *  \brief Typedef son_anr_pci_conflict_confusion_ind_t from son_oam_pci_conflicc
 * t_ind_t
 */
typedef son_oam_pci_conflict_confusion_ind_t son_anr_pci_conflict_confusion_ind_t;

/*! \  struct son_anr_modify_ue_count_req_t
 *  \brief  ANR Modify number of UEs to configure for UE Measurement Reporting
 *  \param transaction_id                   Transaction Identity
 *  \param modify_ue_cnt_req                Modify number of UEs req from OAM
 */
typedef struct _son_anr_modify_ue_count_req_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_ue_count_req_t modify_ue_cnt_req;
}son_anr_modify_ue_count_req_t;

/*! \  son_anr_modify_ue_count_res_t from son_generic_response_witt
 * h_cell_id_t*/
typedef son_generic_response_with_cell_id_t son_anr_modify_ue_count_res_t;

/*! \  struct son_anr_modify_reporting_interval_req_t
 *  \brief  ANR Modify number of UEs to configure for UE Measurement Reporting
 *  \param transaction_id                   Transaction Identity
 *  \param modify_interval_req              Modify meas reporting interval
 *                                          req from OAM
 */
typedef struct _son_anr_modify_meas_reporting_interval_req_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_meas_reporting_interval_req_t modify_interval_req;
}son_anr_modify_meas_reporting_interval_req_t;

/*! \  son_anr_modify_meas_reporting_interval_res_t from son_generic_response_with_cell_id_t*/
typedef son_generic_response_with_cell_id_t
    son_anr_modify_meas_reporting_interval_res_t;

/*REL 1.2 CHANGES:START*/

/*! \ struct son_anr_nr_delete_from_remove_list_req_t
 *  \brief This is the request structure to delete NR entry in Remove List
 *  \param transaction_id               Transaction identifier
 *  \param delete_from_remove_list_req  Delete from Remove List request from OAM
 */
typedef struct _son_anr_nr_delete_from_remove_list_req_t
{
    son_u16 transaction_id;
    son_oam_nr_delete_from_remove_list_req_t delete_from_remove_list_req;
} son_anr_nr_delete_from_remove_list_req_t;

/*! \ struct _son_anr_nr_delete_from_remove_list_res_t
 *  \brief This is the response structure
 *  \param transaction_id               Transaction identifier
 *  \param delete_from_remove_list_res  Delete from Remove List response to OAM
 */
typedef struct _son_anr_nr_delete_from_remove_list_res_t
{
    son_u16 transaction_id;
    son_oam_nr_delete_from_remove_list_resp_t delete_from_remove_list_res;
} son_anr_nr_delete_from_remove_list_res_t;

/*! \ struct son_anr_modify_removal_attributes_req_t
 *  \brief This is the request structure to modify removal attributes
 *  \param transaction_id                   Transaction identifier
 *  \param modify_removal_attributes_req    Modify removal attributes request from OAM
 */
typedef struct _son_anr_modify_removal_attributes_req_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_removal_attributes_req_t modify_removal_attributes_req;
} son_anr_modify_removal_attributes_req_t;

/*! \ struct son_anr_modify_removal_attributes_res_t
 *  \brief This is the response structure
 *  \param transaction_id                   Transaction identifier
 *  \param modify_removal_attributes_res    Modify removal attributes response to OAM
 */
typedef struct _son_anr_modify_removal_attributes_res_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_removal_attributes_resp_t modify_removal_attributes_res;
} son_anr_modify_removal_attributes_res_t;

/*! \ struct son_anr_modify_pci_confusion_cfg_req_t
 *  \brief This is the request structure to modify pci confusion cfg
 *  \param transaction_id                   Transaction identifier
 *  \param modify_pci_confusion_cfg_req     Modify PCI confusion cfg request from OAM
 */
typedef struct _son_anr_modify_pci_confusion_cfg_req_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_pci_confusion_cfg_req_t modify_pci_confusion_cfg_req;
} son_anr_modify_pci_confusion_cfg_req_t;

/*! \ struct son_anr_modify_pci_confusion_cfg_res_t
 *  \brief This is the response structure
 *  \param transaction_id                   Transaction identifier
 *  \param modify_pci_confusion_cfg_res     Modify PCI confusion cfg response to OAM
 */
typedef struct _son_anr_modify_pci_confusion_cfg_res_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_pci_confusion_cfg_resp_t modify_pci_confusion_cfg_res;
} son_anr_modify_pci_confusion_cfg_res_t;

/* SPR 20653 Fix Start */
/*! \  struct son_anr_modify_attribute_req_t
 *  \brief  ANR Modify attributes
 *  \param transaction_id                   Transaction Identity
 *  \param modify_attribute_req             Modify ANR attributes from OAM
 */
typedef struct _son_anr_modify_attribute_req_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_attribute_req_t modify_attribute_req;
}son_anr_modify_attribute_req_t;

/*! \ struct son_anr_modify_attribute_res_t
 *  \brief This is the response structure
 *  \param transaction_id                   Transaction identifier
 *  \param modify_attribute_res     Modify PCI confusion cfg response to OAM
 */
typedef struct _son_anr_modify_attribute_res_t
{
    son_u16 transaction_id;
    son_oam_anr_modify_attribute_resp_t modify_attribute_res;
} son_anr_modify_attribute_res_t;
/* SPR 20653 Fix End */

/*! \ struct son_anr_enable_disable_x2_req_t
 *  \brief This is the request structure to enable/disable x2
 *  \param transaction_id           Transaction identifier
 *  \param enable_disable_x2_req    Enable/Disable X2 request from OAM
 */
typedef struct _son_anr_enable_disable_x2_req_t
{
    son_u16 transaction_id;
    son_oam_enable_disable_x2_req_t enable_disable_x2_req;
} son_anr_enable_disable_x2_req_t;

/*! \ struct son_anr_enable_disable_x2_res_t
 *  \brief This is the response structure
 *  \param transaction_id           Transaction identifier
 *  \param enable_disable_x2_res    Enable/Disable X2 response to OAM
 */
typedef struct _son_anr_enable_disable_x2_res_t
{
    son_u16 transaction_id;
    son_oam_enable_disable_x2_resp_t enable_disable_x2_res;
} son_anr_enable_disable_x2_res_t;

/*! \ struct son_anr_enable_disable_limited_mode_req_t
 *  \brief This is the request structure to enable/disable limited mode
 *  \param transaction_id                   Transaction identifier
 *  \param enable_disable_limited_mode_req  Enable/Disable limited mode
 *                                          request from OAM
 */
typedef struct _son_anr_enable_disable_limited_mode_req_t
{
    son_u16 transaction_id;
    son_oam_anr_enable_disable_limited_mode_req_t enable_disable_limited_mode_req;
} son_anr_enable_disable_limited_mode_req_t;

/*! \ struct son_anr_enable_disable_limited_mode_res_t
 *  \brief This is the response structure
 *  \param transaction_id                   Transaction identifier
 *  \param enable_disable_limited_mode_res  Enable/Disable limited mode
 *                                          response to OAM
 */
typedef struct _son_anr_enable_disable_limited_mode_res_t
{
    son_u16 transaction_id;
    son_oam_anr_enable_disable_limited_mode_resp_t enable_disable_limited_mode_res;
} son_anr_enable_disable_limited_mode_res_t;

/*! \ struct son_anr_enb_add_req_t
 *  \brief  This is the request structure for adding neighbor eNBs in NRT.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_add_req                      Add neighbor eNB req from OAM
 */
typedef struct _son_anr_enb_add_req_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_add_req_t enb_add_req;
}son_anr_enb_add_req_t;

/*! \ struct son_anr_enb_add_resp_t
 *  \brief  This is the resposne structure.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_add_res                      Add neighbor eNB resp to OAM.
 */
typedef struct _son_anr_enb_add_resp_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_add_resp_t enb_add_res;
}son_anr_enb_add_resp_t;

/*! \ struct son_anr_enb_update_req_t
 *  \brief  This is the request structure for updating neighbor eNBs in NRT.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_update_req                      Update neighbor eNB req from OAM
 */
typedef struct _son_anr_enb_update_req_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_update_req_t enb_update_req;
}son_anr_enb_update_req_t;

/*! \ struct son_anr_enb_update_resp_t
 *  \brief  This is the resposne structure.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_update_res                   Update neighbor eNB resp to OAM.
 */
typedef struct _son_anr_enb_update_resp_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_update_resp_t enb_update_res;
}son_anr_enb_update_resp_t;

/*! \ struct son_anr_enb_delete_req_t
 *  \brief  This is the request structure for deleting neighbor eNBs in NRT.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_del_req                      Delete neighbor eNB req from OAM
 */
typedef struct _son_anr_enb_delete_req_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_delete_req_t enb_del_req;
}son_anr_enb_delete_req_t;

/*! \ struct son_anr_enb_delete_resp_t
 *  \brief  This is the resposne structure.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_del_res                   Delete  neighbor eNB resp to OAM.
 */
typedef struct _son_anr_enb_delete_resp_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_delete_resp_t enb_del_res;
}son_anr_enb_delete_resp_t;

/*! \ struct son_anr_enb_retrieve_req_t
 *  \brief  This is the request structure for retrieving neighbor eNBs in NRT.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_retrieve_req                 Retrieve neighbor eNB req from OAM
 */
typedef struct _son_anr_enb_retrieve_req_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_retrieve_req_t enb_retrieve_req;
}son_anr_enb_retrieve_req_t;

/*! \ struct son_anr_enb_retrieve_resp_t
 *  \brief  This is the resposne structure.
 *  \param transaction_id                   Transaction Identity
 *  \param enb_retrieve_res                 Retrieve  neighbor eNB resp to OAM.
 */
typedef struct _son_anr_enb_retrieve_resp_t
{
    son_u16 transaction_id;
    son_oam_nr_enb_retrieve_resp_t enb_retrieve_res;
}son_anr_enb_retrieve_resp_t;

/*!  \struct son_anr_limited_mode_pci_info_t
 *   \brief  This structure stores limited mode pci info
 */
typedef struct _son_anr_limited_mode_pci_info_t
{
    son_oam_anr_limited_mode_pci_info_t *p_anr_limited_mode_pci_info_t;
}son_anr_limited_mode_pci_info_t;

/*! \ struct son_rrm_tnl_discovery_req_t
 *  \brief  This is the TNL Discovery request structure.
 *  \param transaction_id                   Transaction Identity
 *  \param tnl_req                                      TNL Discovery request to RRM.
 */
typedef struct _son_rrm_tnl_discovery_req_t
{
    son_u16 transaction_id;
    rrm_son_tnl_discovery_req_t tnl_req;
}son_rrm_tnl_discovery_req_t;

/*! \ struct son_rrm_tnl_discovery_res_t
 *  \brief  This is the TNL Discovery response structure.
 *  \param transaction_id                   Transaction Identity
 *  \param tnl_req                                      TNL Discovery response from RRM.
 */
typedef struct _son_rrm_tnl_discovery_res_t
{
    son_u16 transaction_id;
    rrm_son_tnl_discovery_res_t tnl_res;
}son_rrm_tnl_discovery_res_t;

/*! \  rrm_son_ho_report_t son_rrm_ho_report_t
 */
typedef rrm_son_ho_report_t son_rrm_ho_report_t;
/*REL 1.2 CHANGES:END*/

/*! \struct son_anr_feature_state_change_ind_t
 *  \brief  Typedef son_anr_feature_state_change_ind_t from
 *          son_oam_feature_state_change_ind_t
 */
typedef son_oam_feature_state_change_ind_t son_anr_feature_state_change_ind_t;

/*REL 1.3 CHANGES:START*/
typedef struct _son_anr_cell_mode_ind_t
{
    son_intra_rat_global_cell_id_t srv_cgi;
    son_anr_cell_mode_et cell_mode;
}son_anr_cell_mode_ind_t;

/*REL 1.3 CHANGES:END*/

/*! \  rrm_son_ho_attempt_ind_t son_rrm_ho_report_t
 */

/*! \struct son_rrm_ho_attempt_ind_t
 *  \brief  Typedef rrm_son_ho_attempt_ind_t to son_rrm_ho_attempt_ind_t
 */
typedef rrm_son_ho_attempt_ind_t son_rrm_ho_attempt_ind_t;

/*! \struct son_rrm_ho_failure_report_t
 *  \brief  Typedef son_ho_failure_report_t son_rrm_ho_failure_report_t
 */
typedef son_ho_failure_report_t son_rrm_ho_failure_report_t;


/*! \  struct son_anr_meas_config_obj_remove_req_t
 *  \brief This structure is used by OAM to send measurement
 *      object removal request required to delete the UE measurement
 *      frequencies
 *  \param  transaction_id      Transaction Identifier
 *  \param  remove_req          Meas object removal req data
 */
typedef struct _son_anr_meas_config_obj_remove_req_t
{
    son_u16                             transaction_id;
    son_oam_anr_meas_config_obj_remove_req_t   remove_req;
}son_anr_meas_config_obj_remove_req_t;

/*! \  struct son_anr_meas_config_obj_remove_res
 *  \brief This structure is used to send measurement
 *      object removal request response to OAM
 *  \param  transaction_id      Transaction Identifier
 *  \param  remove_req          Meas object removal res data
 */
typedef struct _son_anr_meas_config_obj_remove_res_t
{
    son_u16                             transaction_id;
    son_oam_anr_meas_config_obj_remove_res_t   remove_res;
}son_anr_meas_config_obj_remove_res_t;

/*! \  struct son_rrm_meas_config_obj_remove_req_t
 *  \brief Structure storing the Meas object remove request data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param remove_req           Meas Object removal request to RRM
 */
typedef struct _son_rrm_meas_config_obj_remove_req_t
{
    son_u16                             transaction_id;
    rrm_son_meas_config_obj_remove_req_t       remove_req;
}son_rrm_meas_config_obj_remove_req_t;

/*! \  struct son_rrm_meas_config_obj_remove_res_t
 *  \brief Structure storing the Meas object removal response data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param remove_res           Meas object removal response from RRM
 */
typedef struct _son_rrm_meas_config_obj_remove_res_t
{
    son_u16                             transaction_id;
    rrm_son_meas_config_obj_remove_res_t       remove_res;
}son_rrm_meas_config_obj_remove_res_t;

/* Rel 3.0 Multi Sector code changes Start */
/*! \enum es_cell_switch_on_off_status_et
 *  \brief An enum that specifies whether the cell is switched ON or OFF by SON Energy Saving feature.
 */
typedef enum
{
    ES_CELL_SWITCHED_OFF = 1,
    ES_CELL_SWITCHED_ON
}es_cell_switch_on_off_status_et;

/*! \  struct sonanr_cell_switch_on_off_ind_t
 *  \brief Cell Switch ON/OFF indication message from SON ES.
 *  \param src_cgi                  Serving cell global identifier
 *  \param cell_on_off_status       Switch on off status enum value
 */
typedef struct _sonanr_cell_switch_on_off_ind_t
{
    son_intra_rat_global_cell_id_t  src_cgi;
    es_cell_switch_on_off_status_et cell_on_off_status;
}sonanr_cell_switch_on_off_ind_t;
/* Rel 3.0 Multi Sector code changes Stop */

/* SPR-10331 Fix Starts */
/*! \  rrm_son_enb_config_update_ind_t son_rrm_enb_config_update_ind_t
 */
typedef rrm_son_enb_config_update_ind_t son_rrm_enb_config_update_ind_t;
/* SPR-10331 Fix Ends */

/* eICIC feature related code starts */
/*! \son_anr_neighbor_default_config_req_t
 * \brief this structure store the default NR add request params.
 * \param transaction_id
 * \param son_anr_default_req
 */
typedef struct _son_anr_neighbor_default_config_req_t
{
    son_u16                                     transaction_id;
    son_oam_anr_neighbor_default_config_req_t   son_anr_default_req;
}son_anr_neighbor_default_config_req_t;


/* eICIC feature related code ends */

#endif /* _ANR_OAM_INTF_H_ */
