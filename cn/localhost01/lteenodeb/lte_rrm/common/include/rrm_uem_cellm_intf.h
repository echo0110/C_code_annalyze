/*! \file rrm_uem_cellm_intf.h
 *  \brief This file contains Data Structures for interface between
 *       Cell_Manager and UE Manager.
 *  \date January 12, 2012
 *  \author gur11059
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
      
#ifndef _RRM_UEM_CELLM_INTF_H_
#define _RRM_UEM_CELLM_INTF_H_
/*
* Header File Includes
*/

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"

/*! \headerfile rrm_cell_rrmim_intf.h <>
 */
#include "rrm_cell_rrmim_intf.h"

/*  DYNAMIC ICIC CHANGES START  */

/*! \ rrmuem_cm_update_ue_info_req_t
 *  \brief This struct delete all UE context request
 *  \param transaction_id transacation ID
 *  \param cell_index     cell index
 */
typedef struct _rrmuem_cm_update_ue_info_req_t
{
#define     RRMUEM_CM_UE_LOCATION_UPDATE_ACTION_PRESENT           0x0001
#define     RRMUEM_CM_CC_UE_OLPC_REDUCE_POWER_ACTION_PRESENT      0x0002
    rrm_bitmask_t                       bitmask;       /*!< Bitmask */
	U16			                        transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t                    cell_index;		/*!< Cell Index */		
    rrm_location_update_action_et       ue_location_update_action; /*!<  Location action  */
    rrm_olpc_reduce_power_action_et     ue_reduce_power_action; /*!<  Reduce UL Power action  */
    U8                                  location_update_percentage; /*!<  Change %age of UEs location  */
}rrmuem_cm_update_ue_info_req_t;   /*^ API, RRMUEM_CM_UPDATE_UE_LOCATION_REQ ^*/

/*! \rrmuem_cm_update_ue_info_resp_t 
 *  \brief This struct contains transaction id,cell index and response
 *  \param transaction_id transacation ID
 *  \param cell_index    cell index   
 *  \param response      response
 */
typedef struct _rrmuem_cm_update_ue_info_resp_t
{
	U16		            	     transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t             cell_index;		/*!< Cell Index */		
	rrm_icic_action_response_et  response;		/*!< Delete All Ue Context Response */
}rrmuem_cm_update_ue_info_resp_t;   /*^ API, RRMUEM_CM_UPDATE_UE_INFO_RESP ^*/
/*  DYNAMIC ICIC CHANGES END  */
/* CA_Stage3_Change: Start */
typedef struct _rrmuem_cm_cell_info_change_ind_resp_t
{
    rrm_cell_index_t         pcell_index;
    rrm_cell_index_t         scell_index;
    rrm_return_et            status;
    /*SPR 13316:start*/
    U16                      trans_id;
    /*SPR 13316:end*/
}rrmuem_cm_cell_info_change_ind_resp_t;
/* CA_Stage3_Change: End */

/* BUG_11648_FIX_START */
typedef struct _rrmuem_cm_cell_stop_broadcast_resp_t
{
    rrm_cell_index_t         pcell_index;
    rrm_cell_index_t         scell_index;
    rrm_return_et            status;
}rrmuem_cm_cell_stop_broadcast_resp_t;
/* BUG_11648_FIX_END */

/*! \ rrmuem_cm_delete_all_ue_context_req_t
 *  \brief This struct delete all UE context request
 *  \param transaction_id transacation ID
 *  \param cell_index     cell index
 */
typedef struct _rrmuem_cm_delete_all_ue_context_req_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t cell_index;		/*!< Cell Index */		
}rrmuem_cm_delete_all_ue_context_req_t;   /*^ API, RRMUEM_CM_DELETE_ALL_UE_CONTEXT_REQ ^*/
/* Bug_944_start */
/*! \rrmuem_cm_redirect_info_t
 *  \brief This Struct contains MARD information that needs to be taken
 *  redirection of UE on load based scenario.
 *  \param bitmask to check optional field is present or not
 *  \param mard_present to check mard is present or not
 *  \param rwr_timer indicates measurement based rwr timer value 
 *  the duration for which RRM will wait for the measurement report 
 *  to select the suitable frequency.
 */
#define RRMUEM_CM_MARD_PRESENT 0x01
#define RRMUEM_CM_MEAS_BASED_RWR_TIMER_PRESENT 0x02

typedef struct _rrmuem_cm_redirect_info_t
{
    rrm_bitmask_t   bitmask;        /*^ BITMASK ^*/
    rrm_bool_t      mard_present;   /*^ O, RRMUEM_CM_MARD_PRESENT, B, 0,1 ^*/
    U8              rwr_timer;      /*^ O, RRMUEM_CM_MEAS_BASED_RWR_TIMER_PRESENT, B, 5,120 ^*/
}rrmuem_cm_redirect_info_t;
/* Bug_944_end */

/*! \rrmuem_cm_redirect_rel_ind_t
 *  \brief This Struct contains redirection release indication
 *  information that needs to be taken redirection of UE on 
 *  load based scenario.
 *  \param bitmask to check optional field is present or not
 *  \param cell_index
 *  \param ue_index
 *  \param mard_info indicate mard information that need to be taken for
 *  redirection of UE on load based scenario.
 */
#define RRMUEM_CM_MARD_INFO_PRESENT 0x01

typedef struct _rrmuem_cm_redirect_rel_ind_t
{
    /* Bug_944_start */
    rrm_bitmask_t               bitmask;
    /* Bug_944_end */
    rrm_cell_index_t            cell_index;
    rrm_ue_index_t              ue_index;
    /* Bug_944_start */
    rrmuem_cm_redirect_info_t   mard_info; /*^ O, RRMUEM_CM_MARD_INFO_PRESENT, N, 0, 0, ^*/
    /* Bug_944_end */
}rrmuem_cm_redirect_rel_ind_t;
/* RRM CELLM UEM INTERFACE APIS */
/*! \ rrmuem_cm_delete_all_ue_context_res_t
 *  \brief This struct contains transaction id,cell index and response
 *  \param transaction_id transacation ID
 *  \param cell_index    cell index   
 *  \param response      response
 */
typedef struct _rrmuem_cm_delete_all_ue_context_res_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t cell_index;		/*!< Cell Index */		
	rrm_bool_et             response;		/*!< Delete All Ue Context Response */
}rrmuem_cm_delete_all_ue_context_res_t;   /*^ API, RRMUEM_CM_DELETE_ALL_UE_CONTEXT_RESP ^*/

/*! \ rrmcm_uem_meas_config_req_t
 *  \brief This struct is used for UEM measurement configuration request 
 */
typedef rrmcm_rmif_cell_son_meas_config_req_t rrmcm_uem_meas_config_req_t;


/*! \ rrmuem_cm_ecn_qci_list_t
 *  \brief this struct contains the direction and congestion status
 *  \param qci
 *  \param direction  direction
 *  \param congestion_status status of congestion
*/
typedef struct _rrmuem_cm_ecn_qci_list_t
{
    U8 qci;
    U8 direction;
    U8 congestion_status;
}rrmuem_cm_ecn_qci_list_t;
/*! \ rrmuem_cm_ecn_set_unset_ind_t
 *  \brief This struct contains parameters to set and unset index
 *  \param trans_id transacation ID
 *  \param cell_index cell index
 *  \param ue_index ue index
 *  \param count count 
 *  \param ue_qci_list[RRM_MAX_QCI] 
*/
typedef struct _rrmuem_cm_ecn_set_unset_ind_t
{
    U16 trans_id;
    rrm_cell_index_t cell_index;
    rrm_ue_index_t ue_index;
    U8 count;
    rrmuem_cm_ecn_qci_list_t ue_qci_list[RRM_MAX_QCI];
}rrmuem_cm_ecn_set_unset_ind_t;

/*! \ rrmuem_cm_unset_for_all_ue_ind_t
 *  \brief This struct ids for all ue indication information
 *  \param trans_id transaction ID
 *  \param cell_index cell index
*/
typedef struct _rrmuem_cm_unset_for_all_ue_ind_t
{
    U16 trans_id;
    rrm_cell_index_t cell_index;
}rrmuem_cm_unset_for_all_ue_ind_t;

/*uplink mu mimo chnages start*/

/* /def RRM_MAX_CELL_REGION_ALLOWED
  */
#define RRM_MAX_CELL_REGION_ALLOWED           2

/*! \enum rrm_cell_region_et
 *  \brief Enumeration for categorizing the UE's cell region
 */
typedef enum
{
    RRM_CELL_CENTER_UE = 0,
    RRM_CELL_EDGE_UE = 1
}rrm_cell_region_et;


typedef struct _rrmuem_cm_prb_threshold_reached_ind_t
{
    U16                    trans_id;
    rrm_cell_index_t       cell_index;
    rrm_cell_region_et     cell_region_enable;
    rrm_cell_region_et     cell_region_disable;
    rrm_bool_et            both_cell;
    rrm_bool_et            enable;
    rrm_bool_et            disable;
    rrm_bool_et            only_single_region;

}rrmuem_cm_prb_threshold_reached_ind_t;

/* SPR 16406 Start */
/* SPR 16406 18sep Start */
#define RRM_UEM_HO_TRIGGER_IND_CGI_PRESENT 0x01

typedef struct _rrm_cellm_uem_ho_trigger_ind_t
{
    U8 bitmask;
    rrm_cell_index_t cell_index;
    U16 pci;
    U16 earfcn;
    rrm_oam_eutran_global_cell_id_t cgi;  
/* SPR 16406 18sep End */
}rrm_cellm_uem_ho_trigger_ind_t;

/* SPR 16406 End */

/* Carrier Aggregation start*/

typedef struct _rrm_cell_info_transfer_object 
{
    rrm_cell_index_t          cell_index; /* cell index which is modified during cell reconfiguration */
    U64                       ca_config_bitmask;
    /*SPR 13316:start*/
    U16                       trans_id;
    /*SPR 13316:end*/
}rrm_cell_info_transfer_object;

typedef struct _rrm_cell_change_obj
{
    rrm_cell_info_transfer_object  cell_transfer_obj;
    rrm_bool_et                    is_cell_changed;
}rrm_cell_change_object;

typedef struct _rrm_cell_change_notification_object
{
    rrm_cell_index_t                cell_index;/* cell index whom to notify */
    rrm_cell_info_transfer_object   cell_broadcast_ca_info;
    /*SPR 13316:start*/
    U16 trans_id;
    /*SPR 13316:end*/
}rrm_cell_change_notification_object_t;

typedef struct _rrm_scell_deactivation_user_t
{

    rrm_cell_index_t    cell_index;
    U16                 num_of_user;

}rrm_scell_deactivation_user_t;

typedef struct _rrm_scell_deactivation_notification_t
{
    rrm_cell_index_t                cell_index;
    rrm_scell_deactivation_user_t   scell_deact_user;
    /*SPR 13316:start*/
    U16                             trans_id;
    /*SPR 13316:end*/
}rrm_scell_deactivation_notification_t;
/* Carrier Aggregation end */
/*uplink mu mimo changes end*/

/** eICIC_PHASE_1_2_CHANGES_START */

#define RRMUEM_CELLM_EICIC_PROVISION_RECONFIG  0X01
/*eicic_subset_pattern_fix_start*/
#define RRMUEM_CELLM_MEAS_SF_PATTERN_RECONFIG 0X02
/*eicic_subset_pattern_fix_end*/
/*! \ rrmuem_cellm_eicic_provision_reconfig_req_t
 *  \brief This struct contains the provision type reconfig data
 *
 *  \param transaction_id       transacation ID
 *  \param cell_index           cell index of the reconfigured cell
 */
typedef struct _rrmuem_cellm_eicic_provision_reconfig_req_t
{
    rrm_bitmask_t                bitmask;         /*!< Bitmask */                
    U16			                 transaction_id; /*!< Transaction Id */
	rrm_cell_index_t             cell_index;     /*!< Cell Index */	
    rrm_oam_eicic_node_type_et   eicic_provision_reconfig; /*^ O, RRMUEM_CELLM_EICIC_PROVISION_RECONFIG, H, 0, 3 ^*/  
}rrmuem_cellm_eicic_provision_reconfig_req_t;   /*^ API, RRMUEM_CELLM_EICIC_PROVISION_RECONFIG_REQ ^*/

/*! \rrmuem_cellm_eicic_provision_reconfig_resp_t
 *  \brief This struct contains transaction id,cell index ,response 
 *
 *  \param transaction_id transacation ID
 *  \param cell_index     cell index of the Victim cell 
 *  \param response       Success if aggressor is found else failure
 */
typedef struct _rrmuem_cellm_eicic_provision_reconfig_resp_t
{
	U16		            	     transaction_id;  /*!< Transaction Id */
	rrm_cell_index_t             cell_index;	  /*!< Cell Index */		
	rrm_return_et                response;		  
}rrmuem_cellm_eicic_provision_reconfig_resp_t;   /*^ API, RRMUEM_CELLM_EICIC_PROVISION_RECONFIG_RESP ^*/

/** eICIC_PHASE_1_2_CHANGES_END */

/* eICIC_PHASE_1_2_CHANGES_START */
#define RRM_MAX_AGGRESSOR_CNT       16
/*! \ rrmuem_cm_get_aggressor_req_t
 *  \brief This struct contains the data reuired to find aggressor node
 *
 *  \param transaction_id       transacation ID
 *  \param cell_index           cell index of the victim cell
 *                              for interfering cells detection.
 */
typedef struct _rrmuem_cm_get_aggressor_req_t
{
	U16			                        transaction_id; /*!< Transaction Id */
	rrm_cell_index_t                    cell_index;     /*!< Cell Index */	
}rrmuem_cm_get_aggressor_req_t;   /*^ API, RRMUEM_CM_GET_AGGRESSOR_REQ ^*/

/*! \rrmuem_cm_get_aggressor_resp_t
 *  \brief This struct contains transaction id,cell index ,response and 
 *  \      PCI of the aggressor node.
 *
 *  \param transaction_id transacation ID
 *  \param cell_index     cell index of the Victim cell 
 *  \param response       Success if aggressor is found else failure
 *  \param phy_cell_id    PCI of the aggressor Node
 */
typedef struct _rrmuem_cm_get_aggressor_resp_t
{
	U16		            	     transaction_id;  /*!< Transaction Id */
	rrm_cell_index_t             cell_index;	  /*!< Cell Index */		
	rrm_return_et                response;		  
    U8                           num_of_aggressor;
    U16                          phy_cell_id[RRM_MAX_AGGRESSOR_CNT];
}rrmuem_cm_get_aggressor_resp_t;   /*^ API, RRMUEM_CM_GET_AGGRESSOR_RESP ^*/

    /* SPR 14576 start */ 
typedef enum
{
    RRM_VICTIM_SCHEDULE_WITH_ABS_CHANGE,
    RRM_VICTIM_SCHEDULE_WITH_NO_ABS_CHANGE
}rrm_victim_schedule_cause_et;
    /* SPR 14576 end */ 
/*! \rrmuem_cm_schedule_victim_ue_ind_t
 *  \brief This struct contains transaction id,cell index
 *
 *  \param transaction_id     transacation ID
 *  \param cell_index         cell index of the Victim cell 
 */
typedef struct _rrmuem_cm_schedule_victim_ue_ind_t
{
	U16		            	    transaction_id;    /*!< Transaction Id */
    rrm_cell_index_t            cell_index;	       /*!< Cell Index */	
    /* SPR 14576 start */ 
    rrm_victim_schedule_cause_et schedule_cause;
        /* SPR 14576 end */ 
    U8                          aggressor_count;
    /* SPR 14463 start */
    U16                          valid_aggressor_list[RRM_MAX_AGGRESSOR_CNT];
    /* SPR 14463 end */
}rrmuem_cm_schedule_victim_ue_ind_t;   /*^ API, RRMUEM_CM_SCHEDULE_VICTIM_UE_IND ^*/

/* eICIC_PHASE_1_2_CHANGES_END */

#ifdef LTE_EMBMS_SUPPORTED
typedef struct _rrmuem_cm_mbms_reconfig_ue_ind_t
{
    rrm_cell_index_t            cell_index;        /*!< Cell Index */
    rrm_bool_et                 reconfig_cqi;
}rrmuem_cm_mbms_reconfig_ue_ind_t;   /*^ API, RRMUEM_CM_MBMS_RECONFIG_UE_IND ^*/
#endif

#endif  /* _RRM_UEM_CELLM_INTF_H_ */
