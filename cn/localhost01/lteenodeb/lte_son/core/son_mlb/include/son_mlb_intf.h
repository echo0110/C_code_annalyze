/*! \file son_mlb_intf.h
 *  \brief This file contains the  structures specific to ANR interface with other modules(SMIF)
 *  \date  Feb, 2012
 *  \author Komal/Sonal 
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MLB_INTF_H_
#define _SON_MLB_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_nmm_apps_intf.h <>
 */
#include <son_nmm_apps_intf.h>
#include <son_oam_intf.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \  struct son_mlb_enable_req_t
 *  \brief It is used to globally enable MLB if cell id is not present
 *         or to enable MLB for the cell whose id is present in the request
 *  \param transaction_id              Transaction identifier
 *  \param enable_req				    Enable Request from OAM
 */
typedef enum _son_mlb_status_e
{
    ENABLE,
    DISABLE
}son_mlb_status_et;
typedef struct _son_mlb_enable_req_t
{
    son_u16                         transaction_id;
    son_oam_mlb_enable_req_t     	enable_req;
}son_mlb_enable_req_t;

/*! \  struct son_mlb_enable_res_t
 *  \brief Enable response structure
 *  \param transaction_id              Transaction identifier
 *  \param enable_res				   Enable Response to OAM
 */
typedef struct _son_mlb_enable_res_t
{
    son_u16                         transaction_id;
    son_oam_mlb_enable_resp_t  	  	enable_res;
}son_mlb_enable_res_t;

/*! \  struct son_mlb_disable_req_t
 *  \brief MLB Disable Request Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_req                      Disable Request from OAM
 */
typedef struct _son_mlb_disable_req_t
{ 
    son_u16                    		transaction_id;
    son_oam_mlb_disable_req_t    	disable_req;
}son_mlb_disable_req_t;

/*! \  struct son_mlb_disable_res_t
 *  \brief MLB Disable Response Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_res                      Disable Response to OAM
 */
typedef struct _son_mlb_disable_res_t
{
    son_u16                    		transaction_id;
    son_oam_mlb_disable_resp_t    	disable_res;
}son_mlb_disable_res_t;

/*! \  struct son_mlb_modify_attributes_req_t
 *  \brief MLB Modify attributes Request    Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_res                      Modify attributes request from OAM
 */
typedef struct _son_mlb_modify_req_t
{
    son_u16                               transaction_id;
    son_oam_mlb_modify_attributes_req_t   modify_attributes_req;
}son_mlb_modify_req_t;

/*! \  struct son_mlb_modify_attributes_res_t
 *  \brief MLB Modify attributes Response   Structure
 *  \param transaction_id                   Transaction Identity
 *  \param disable_res                      Modify attributes response to OAM
 */
typedef struct _son_mlb_modify_res_t
{
    son_u16                               transaction_id;
    son_oam_mlb_modify_attributes_resp_t   modify_attributes_res;
}son_mlb_modify_res_t;

typedef son_oam_feature_state_change_ind_t son_mlb_feature_state_change_ind_t;

typedef struct   _son_anr_disable_ind_t	
{
    son_intra_rat_global_cell_id_t src_cgi;
    son_mlb_status_et    status;
}son_anr_disable_ind_t;

typedef son_anr_disable_ind_t son_mlb_anr_disable_ind_t;

/************************************ x2 start **********************************************/

#define MAN_NUM_ERROR 256

typedef enum _triggering_message_et
{
    INITIATING_MESSAGE,
    SUCCESSFULL_OUTCOME,
    UNSUCCESSFULL_OUTCOME
}triggering_message_et;

typedef struct _msg_type_t
{
    son_u16				procedure_code; /* M */
    triggering_message_et		msg_type;	/* M */
}msg_type_t;

typedef enum _procedure_criticality_et 
{
    REJECT,
    IGNORE,
    NOTIFY
}procedure_criticality_et;

typedef enum _typeof_error_et
{
    NOT_UNDERSTOOD,
    MISSING
}typeof_error_et;    

typedef struct _criticality_diagnostics
{
    procedure_criticality_et		ie_criti;
    son_u16        			ie_id; 
    typeof_error_et 			typ_error;
}criticality_diagnostics_err_t; 

typedef struct _critical_diagnostic_t
{
    son_u16 				procedure_code; 		/*O*/
    triggering_message_et		tri_msg;         		/*O*/
    procedure_criticality_et 		proced_criti;    		/*O*/
    criticality_diagnostics_err_t   	criti_diago[MAN_NUM_ERROR];   	/*M*/
}critical_diagnostic_t;    

typedef enum  _transport_layer_cause_et
{
    TRANSPORT_RESOURCE_UNAVILABLE,
    UNSPECIFIDE
}transport_layer_cause_et;    

typedef enum _protocol_cause_et	
{
    TRANSFER_SYNTAX_ERROR,
    ABSTRACT_SYNTAX_ERROR,
    MESSAGE_NOT_CAMPATIBLE_WITH_RECEIVER_STATE,
    SEMANTIC_ERROR,
}protocol_cause_et;	

typedef enum _miscellaneous_Cause
{
    CONTROL_PROCESSING_OVERLOAD,
    HARDWARE_FAILURE,
    OAM_INTERVENTION,
    NOT_ENOUGH_USER_PLANE_PROCESSING_RESOURCE,
}miscellaneous_cause_et;

typedef enum _radio_network_layer_cause_et
{
    HO_DESIRABLE_FOR_RADIO_REASONS,
    TIME_CRITICAL_HO,
    RESOURCE_OPTIMISATION_HO,
    REDUCE_LOAD_IN_SERCELL,
    PARTIAL_HANDOVER,
    UNKNOWN_NEW_eNB_UE_X2AP_ID,
    HO_TARGET_NOT_ALLOWED,
    CELL_NOT_AVAILABLE,
    NO_RADIO_RESOURCE_AVAILABLE_IN_TARGET_CELL,
    INVALID_MME_GROUP_ID,
    NO_REPORT_PERIODICITY,
    UNKNOWN_eNB,
    MEASUREMENT_ID,
    LOAD_BALANCING,
    SWITCHOFF_ONGOING,
    NOT_SUPPORTED_QCI_VALUE
}radio_network_layer_cause_et;

typedef union _choice_cause_group_t
{
    son_u8 choice;
    radio_network_layer_cause_et		radio_nw_layer_cause;	/* M */
    transport_layer_cause_et			transport_layer_cause;	/* M */
    protocol_cause_et				protocol_cause;		/* M */
    miscellaneous_cause_et			miscellaneous_cause;	/* M */
}choice_cause_group_t;

typedef struct
{
	son_u8   type; /*^ M,0,N,0,0 ^*/
	son_u16  value; /*^M,0,N,0,0 ^*/
} cause_t;

typedef struct _mlb_mobility_change_req_t
{
    son_u8                              bitmask;
    son_intra_rat_global_cell_id_t     	src_cgi1;
    son_intra_rat_global_cell_id_t     	src_cgi2;
    son_global_enb_id_t                 trgt_enbid;
    son_s8  				                mobility_params;		/* O */
    son_s8  				                proposed_mobility_params;
    cause_t 				            cause;
}mlb_mobility_change_req_t;

typedef struct _sonmlb_mobility_change_ack_t
{
    son_intra_rat_global_cell_id_t     	src_cgi1;
    son_intra_rat_global_cell_id_t     	src_cgi2;
    son_global_enb_id_t                 src_enbid;
    son_global_enb_id_t                 trgt_enbid;
    critical_diagnostic_t       	crit_diago;		/* O */
}sonmlb_mobility_change_ack_t;    

#define MOBILITY_PARAM_RANGE_PRESENT 0x02

typedef struct _sonmlb_mobility_change_failure_t
{
    son_u8                              bitmask;
    son_mob_param_range_t               mob_param_range;
    son_intra_rat_global_cell_id_t      src_cgi1;
    son_intra_rat_global_cell_id_t     	src_cgi2;
    son_global_enb_id_t                 src_enbid;
    son_global_enb_id_t                 trgt_enbid;
    cause_t				                cause;
    son_eutran_q_offset_config_t        eutra_q_offset_config;		/* O */
    critical_diagnostic_t       	    crit_diago;		/* O */
}sonmlb_mobility_change_failure_t;

/*************************************x2 end *********************************/

/*SONMLB-- Centralized support start*/
typedef son_oam_mlb_params_info_t son_mlb_params_info_t;
/*! \  struct son_mlb_offset_change_req_t
 *  \brief OAM sends this message to SONMLB in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param transaction_id              Transaction identifier
 *  \param offset_change_req           Offset Change Request from OAM
 */
typedef struct _son_mlb_offset_change_req_t
{
    son_u16 transaction_id;
    son_oam_mlb_offset_change_req_t offset_change_req;
}son_mlb_offset_change_req_t;

/*! \  struct son_mlb_offset_change_resp_t
 *  \brief SONMLB sends this message to OAM in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param transaction_id              Transaction identifier
 *  \param offset_change_resp           Offset Change Response from SONMLB
 */
typedef struct _son_mlb_offset_change_resp_t
{
    son_u16 transaction_id;
    son_oam_mlb_offset_change_resp_t offset_change_resp;
}son_mlb_offset_change_resp_t;

/*! \  struct son_mlb_peer_offset_change_req_t
 *  \brief At target eNB,SON-MLB sends this message to OAM in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param transaction_id              Transaction identifier
 *  \param peer_offset_change_req           Offset Change Request from SON-MLB
 */
typedef struct _son_mlb_peer_offset_change_req_t
{
    son_u16 transaction_id;
    son_oam_mlb_peer_offset_change_req_t peer_offset_change_req;
}son_mlb_peer_offset_change_req_t;

/*! \  struct son_mlb_peer_offset_change_resp_t
 *  \brief At target eNB,OAM sends this message to SON-MLB in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param transaction_id              Transaction identifier
 *  \param peer_offset_change_resp     Offset Change Response from OAM
 */
typedef struct _son_mlb_peer_offset_change_resp_t
{
    son_u16 transaction_id;
    son_oam_mlb_peer_offset_change_resp_t peer_offset_change_resp;
}son_mlb_peer_offset_change_resp_t;

/*SONMLB-- Centralized support end*/

#endif /* _MLB_OAM_INTF_H_ */



