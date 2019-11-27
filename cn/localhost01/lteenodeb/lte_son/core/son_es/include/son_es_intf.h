/*! \file son_es_intf.h
 *  \brief This file contains the  structures specific to ES interface with other modules
 * (SMIF/RRM)
 *  \date  Feb, 2012
 *  \author Reetesh/Atul
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_ES_INTF_H_
#define _SON_ES_INTF_H_

/*! \headerfile son_types.h<>
 */
#include <son_types.h>

/*! \  struct son_es_enable_req_t
 *  \brief ES Enable Request Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param enable_req                     enable request message from OAM
 */
typedef struct _son_es_enable_req_t
{
    son_u16 transaction_id;
    son_oam_es_enable_req_t enable_req;
}son_es_enable_req_t;

/*! \  struct son_es_disable_req_t
 *  \brief ES Disable Request Structure  for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param disable_req                      Disable Request from OAM
 */
typedef struct _son_es_disable_req_t
{
    son_u16 transaction_id;
    son_oam_es_disable_req_t disable_req;
}son_es_disable_req_t;

/*! \  struct son_es_enable_res_t
 *  \brief ES Enable Request Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param son_oam_es_enable_resp_t       enable response message to OAM
 */
typedef struct _son_es_enable_res_t
{
    son_u16 transaction_id;
    son_oam_es_enable_resp_t enable_res;
}son_es_enable_res_t;

/*! \  son_es_disable_res_t from son_es_enable_res_t*/
typedef son_es_enable_res_t son_es_disable_res_t;

/*! \  struct son_es_modify_ue_count_interval_req_t
 *  \brief  ES Modify UE count interval structure for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param modify_interval_req              modify interval message from OAM
 */

typedef struct _son_es_modify_ue_count_interval_req_t
{
    son_u16 transaction_id;
    son_oam_es_modify_ue_count_interval_req_t modify_interval_req;
}son_es_modify_ue_count_interval_req_t;

/*! \  struct son_es_modify_ue_count_threshold_req_t
 *  \brief  ES Modify UE count interval structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param modify_threshold_req           modify threshold message from OAM
 */

typedef struct _son_es_modify_ue_count_threshold_req_t
{
    son_u16 transaction_id;
    son_oam_es_modify_ue_count_threshold_req_t modify_threshold_req;
}son_es_modify_ue_count_threshold_req_t;

/*! \  struct son_es_cell_switch_off_req_t
 *  \brief ES Cell Switch off request structure for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param cell_switch_off                  Cell switch off message from OAM
 */

typedef struct _son_es_cell_switch_off_req_t
{
    son_u16 transaction_id;
    son_oam_es_cell_switch_off_req_t switch_off_req;
}son_es_cell_switch_off_req_t;

/*! \  struct son_es_cell_switch_on_req_t
 *  \brief ES Cell Switch on request structure for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param cell_switch_on                   Cell switch on request from OAM
 */

typedef struct _son_es_cell_switch_on_req_t
{
    son_u16 transaction_id;
    son_oam_es_cell_switch_on_req_t switch_on_req;
}son_es_cell_switch_on_req_t;

/*! \  struct son_es_switched_off_cell_info_req_t
 *  \brief Switched off Cell info request
 *  \param transaction_id                   Transaction Identity
 */

typedef struct _son_es_switched_off_cell_info_req_t
{
    son_u16 transaction_id;
}son_es_switched_off_cell_info_req_t;

/*! \  struct son_es_switched_off_cell_info_res_t
 *  \brief Switched off Cell info response
 *  \param transaction_id                   Transaction Identity
 *  \param son_oam_es_switched_off_cell_info_resp_t structure
 *         of type son_oam_es_switched_off_cell_info_resp_t
 */

typedef struct _son_es_switched_off_cell_info_res_t
{
    son_u16 transaction_id;
    son_oam_es_switched_off_cell_info_resp_t switched_off_cell_info;
}son_es_switched_off_cell_info_res_t;

/* -------------------ES 1.2 Changes Start ------------------------*/

/*! \typedef struct son_es_cell_switchon_for_x2_setup_req_t
 *  \brief Switch on Cell for x2 setup info request
 *  \param transaction_id                   Transaction Identity
 */
typedef struct _son_es_cell_switchon_for_x2_setup_req_t
{
    son_u16 transaction_id;
}son_es_cell_switchon_for_x2_setup_req_t;

/*! \typedef struct son_es_cell_switchon_for_x2_setup_res_t
 *  \brief Switch on Cell for x2 setup info response
 *  \param transaction_id                Transaction Identity
 *  \param result                        Success/Failure
 *  \param error_code                    Cause (valid in case of Failure)
 */
typedef struct _son_es_cell_switchon_for_x2_setup_res_t
{
    son_u16 transaction_id;
    son_return_et result;
    son_error_et error_code;
}son_es_cell_switchon_for_x2_setup_res_t;

/* SPR 11689 changes start */ 

typedef struct _son_es_cell_activation_info_t
{
    U16               num_served_cell;
    son_intra_rat_global_cell_id_t  served_cell_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
}son_es_cell_activation_info_t;

/*! \typedef struct son_es_cell_switchon_for_cell_activation_req_t
 *  \brief Switch on Cell for cell activation  info request
 *  \param transaction_id                   Transaction Identity
 */

typedef struct _son_es_cell_activation_req_t
{
    son_global_enb_id_t                 enbid_1;
    son_global_enb_id_t                 enbid_2;
    son_es_cell_activation_info_t           cell_activation_info ; 

}son_es_cell_activation_req_t;

/*! \typedef struct son_es_cell_switchon_for_cell_activation_resp_t
 *  \brief Switch on Cell for cell activation info response
 *  \param transaction_id                Transaction Identity
 *  \param result                        Success/Failure
 *  \param error_code                    Cause (valid in case of Failure)
 */
typedef struct _son_es_cell_activation_resp_t
{
    son_global_enb_id_t                 enbid_1;
    son_global_enb_id_t                 enbid_2;
    son_es_cell_activation_info_t           cell_activation_info ; 
    son_return_et result;
    son_error_et error_code;
}son_es_cell_activation_resp_t;

/* SPR_19619 start */
typedef struct _son_es_cell_act_resp_t
{
    son_u16 transaction_id;
    son_return_et result;
    son_error_et error_code;
}son_es_cell_act_resp_t;
/* SPR_19619 stop */

/* SPR 11689 changes end */ 

/* AUTONOMOUS SWITCH OFF START */
typedef struct _son_es_autonomous_switch_off_config_req_t
{
    son_u16 transaction_id;
    son_oam_es_autonomous_switch_off_config_req_t autonomous_switch_off_config_req;
}son_es_autonomous_switch_off_config_req_t;

typedef struct _son_es_autonomous_switch_off_config_resp_t
{
    son_u16 transaction_id;
    son_oam_es_autonomous_switch_off_config_resp_t autonomous_switch_off_config_resp;
}son_es_autonomous_switch_off_config_resp_t;

/* AUTONOMOUS SWITCH OFF END */

/*! \typedef son_es_cell_switchon_ind_t from  son_oam_es_cell_switch_on_off_ind_t*/
typedef son_oam_es_cell_switch_on_off_ind_t son_es_cell_switchon_ind_t;
/* -------------------ES 1.2 Changes Stop ------------------------*/

/*! \  son_es_request_to_switch_on_cell_t from son_oam_es_request_to_switch_on_cell_t*/
typedef son_oam_es_request_to_switch_on_cell_t son_es_request_to_switch_on_cell_t;

/*! \  son_es_modify_ue_count_interval_res_t from son_generic_response_with_cell_id_t*/
typedef son_generic_response_with_cell_id_t son_es_modify_ue_count_interval_res_t;

/*! \  son_es_modify_ue_count_threshold_res_t from son_generic_response_with_cell_id_t*/
typedef son_generic_response_with_cell_id_t son_es_modify_ue_count_threshold_res_t;

/*! \  son_es_cell_switch_off_res_t from son_generic_response_with_cell_id_t*/
typedef son_generic_response_with_cell_id_t son_es_cell_switch_off_res_t;

/*! \  son_es_cell_switch_on_res_t from son_generic_response_with_cell_id_t*/
typedef son_generic_response_with_cell_id_t son_es_cell_switch_on_res_t;

/*! \  son_es_active_ue_cnt_report_t from rrm_son_active_ue_cnt_report_t*/
typedef rrm_son_active_ue_cnt_report_t son_es_active_ue_cnt_report_t;

/*! \  son_es_threshold_hit_ind_t from rrm_ue_count_threshold_hit_ind_t*/
typedef rrm_ue_count_threshold_hit_ind_t son_es_threshold_hit_ind_t;

/* AUTONOMOUS SWITCH OFF START
 *! \  son_es_load_report_ind_t from rrm_son_load_report_ind_t */
typedef rrm_son_load_report_ind_t son_es_load_report_ind_t;
/* AUTONOMOUS SWITCH OFF END */

/*This section to be moved in rrm_son_intf.h when s1 msgs ind supported*/
typedef enum _s1_msg_e
{
    S1MSG_ETWS = 1,
    S1MSG_UNDEF = 0xFF
} s1_msg_e;

/*! \  struct rrm_s1_msg_ind_t
 *  \brief RRM S1AP mesage Indication
 *  \param  cell_id Serving cell CGI
 *  \param  s1_msg  S1AP message received
 */
typedef struct _rrm_s1_msg_ind_t
{
    son_intra_rat_global_cell_id_t cell_id;                /* CGI of cell*/
    s1_msg_e s1_msg;
} rrm_s1_msg_ind_t;
/*Section end*/

/*! \struct son_es_feature_state_change_ind_t
 *  \brief  Typedef son_es_feature_state_change_ind_t from
 *          son_oam_feature_state_change_ind_t
 */
typedef son_oam_feature_state_change_ind_t son_es_feature_state_change_ind_t;

/* SPR_19619 start */


/*! \  struct son_es_peer_cell_activation_r*  	\brief ES Cell Switch off request structure for CSPL Communication
 *  \brief ES Peer Cell Activation request structure for CSPL Communication
 *  \param Peer Cell Activation                  Peer Cell Activation message from OAM
 */

typedef struct _son_es_peer_cell_activation_req_t
{
    son_u16 transaction_id;
	son_oam_peer_cell_activation_req_t 	peer_cell_activation_req;  /*^ M,0,N,0,0 ^*/
}son_es_peer_cell_activation_req_t;

typedef struct _son_es_peer_cell_activation_resp_t
{
    son_u16 transaction_id;
	son_oam_peer_cell_activation_resp_t 	peer_cell_activation_resp;  /*^ M,0,N,0,0 ^*/
}son_es_peer_cell_activation_resp_t;


/* SPR_19619 stop */



#endif /* _SON_ES_INTF_H_ */
