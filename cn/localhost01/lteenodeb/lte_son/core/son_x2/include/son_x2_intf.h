/*! \file son_x2_intf.h
 *  \brief This file contains the  structures of the messages which X2 uses to
 *  interface with other internal and external entities(ES, ANR, OAM and L3 X2AP)
 *  \date  Sep, 2012
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_X2_INTF_H_
#define _SON_X2_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile rrc_x2apCommon_intf.h <>
 */
#include <rrc_x2apCommon_intf.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/*! \headerfile son_enb_table.h <>
 */
#include <son_enb_table.h>

/*! \  struct son_nbr_enb_hash_node_t
 *  \brief Structure containing add eNB request list nodes data
 *  \param anchor   YLNODE structure storing the next and previous pointer to
 *                  the list nodes
 *  \param data     Data stored in the list nodes (Pointer to the eNB hash
 *                  record)
 */
typedef struct _son_nbr_enb_hash_node_t
{
    SON_LIST_NODE anchor;
    son_nbr_enb_hash_rec_t  *data;
}son_nbr_enb_hash_node_t;

/*! \  struct son_x2_add_enb_req_t
 *  \brief Message structure from ANR to add a peer eNB.
 *  Direction of the message: SON ANR to SON X2
 *  \param transaction_id  Transaction identifier
 *  \param enb_list         List containing information of the eNBs to be added
 */
typedef struct _son_x2_add_enb_req_t
{
    son_u16 transaction_id;
    SON_LIST enb_list;  /* List of son_nbr_enb_hash_rec_t*/
}son_x2_add_enb_req_t;

/*! \  struct son_enb_add_status_t
 *  \brief Structure containing the data stored in the eNB add response list
 *  \param p_enb_hash_rec   Pointer to the eNB hash record
 *  \param result           Success/Failure
 */
typedef struct _son_enb_add_status_t
{
    son_nbr_enb_hash_rec_t  *p_enb_hash_rec;
    son_return_et result;
}son_enb_add_status_t;

/*! \  struct son_enb_add_status_node_t
 *  \brief Structure containing add eNB response list nodes data
 *  \param anchor   YLNODE structure storing the next and previous pointer to
 * the list nodes
 *  \param data     Data stored in the list nodes (Refer son_enb_add_status_t)
 */
typedef struct _son_enb_add_status_node_t
{
    SON_LIST_NODE anchor;
    son_enb_add_status_t data;
}son_enb_add_status_node_t;

/*! \  struct son_x2_add_enb_res_t
 *  \brief Add eNB response message structure.
 *   Direction of the message: SON X2 to SON ANR
 *  \param transaction_id   Transaction identifier
 *  \param enb_list         List containing result of the eNBs addition
 */
typedef struct _son_x2_add_enb_res_t
{
    son_u16 transaction_id;
    SON_LIST enb_list; /* List of son_enb_add_res_t*/
}son_x2_add_enb_res_t;

/*! \  struct son_x2_link_up_ind_t
 *  \brief Link Up indication message structure.
 *   Direction of the message: SON X2 to SON ANR
 *  \param p_enb_rec        Pointer to the eNB hash record
 *  \param num_served_cell  Number of serving cells
 *  \param served_cell_info Serving cell information array
 *  \param is_x2_conn_triggered_by_oam Indicates if Link Up
 indication received for OAM requested X2 setup or not
 *  \param gu_group_id GU Group Id List of peer eNB
 */
typedef struct _son_x2_link_up_ind_t
{
    son_nbr_enb_hash_rec_t      *p_enb_rec;
    son_u16 num_served_cell;
    x2ap_served_cell_info_arr_t *p_served_cell_info;
    /* SPR 11340 Fix Starts */
    son_bool_et                 is_x2_conn_triggered_by_oam;
    /* SPR 11340 Fix Ends */
    /* SPR-10331 Fix Starts */
    x2ap_gu_group_id_list_t     gu_group_id;
    /* SPR-10331 Fix Ends */
}son_x2_link_up_ind_t;

/*! \  struct son_x2_link_down_ind_t
 *  \brief Link Down indication message structure.
 *  Direction of the message: SON X2 to SON ANR
 *  \param  p_enb_rec        Pointer to the eNB hash record
 */
typedef struct _son_x2_link_down_ind_t
{
    son_nbr_enb_hash_rec_t          *p_enb_rec;
}son_x2_link_down_ind_t;

/*! \  struct son_x2_delete_enb_req_t
 *  \brief Message structure from ANR to delete a peer eNB.
 *  Direction of the message: SON ANR to SON X2
 *  \param transaction_id  Transaction identifier
 *  \param enb_list         List containing information of the eNBs to be
 *  deleted
 */
typedef struct _son_x2_delete_enb_req_t
{
    son_u16 transaction_id;
    SON_LIST enb_list;  /* List of son_nbr_enb_hash_rec_t*/
}son_x2_delete_enb_req_t;

/*! \  struct son_x2_delete_enb_res_t
 *  \brief Delete eNB response message structure.
 *   Direction of the message: SON X2 to SON ANR
 *  \param transaction_id   Transaction identifier
 *  \param p_enb_rec        Pointer to the eNB hash record
 *  \param result           Success/Failure
 */
typedef struct _son_x2_delete_enb_res_t
{
    son_u16 transaction_id;
    son_nbr_enb_hash_rec_t  *p_enb_data_list;
    son_return_et result;
}son_x2_delete_enb_res_t;

/*! \  struct son_rrc_reset_req_t
 *  \brief ENB reset request message structure.
 *  Direction of the message: SON X2 to L3 X2AP
 *  \param transaction_id   Transaction identifier
 *  \param reset_req        Reset request data
 */
typedef struct _son_rrc_reset_req_t
{
    son_u16 transaction_id;
    x2ap_reset_req_t reset_req;
}son_rrc_reset_req_t;

/*! \  struct son_rrc_reset_res_t
 *  \brief ENB reset response message structure.
 *  Direction of the message: L3 X2AP to SON X2
 *  \param transaction_id   Transaction identifier
 *  \param reset_res        Reset response data
 */
typedef struct _son_rrc_reset_res_t
{
    son_u16 transaction_id;
    x2ap_reset_resp_t reset_res;
}son_rrc_reset_res_t;

/*! \  struct son_rrc_enb_config_update_req_t
 *  \brief eNB config update request message structure.
 *  Direction of the message: SON X2 to L3 X2AP
 *  \param transaction_id           Transaction identifier
 *  \param enb_config_update_req    eNB config update request data
 */
typedef struct _son_rrc_enb_config_update_req_t
{
    son_u16 transaction_id;
    x2ap_enb_config_update_req_t enb_config_update_req;
}son_rrc_enb_config_update_req_t;

/*! \  struct son_rrc_enb_config_update_res_t
 *  \brief eNB config update response message structure.
 *  Direction of the message: L3 X2AP to SON X2
 *  \param transaction_id           Transaction identifier
 *  \param enb_config_update_res    eNB config update response data
 */
typedef struct _son_rrc_enb_config_update_res_t
{
    son_u16 transaction_id;
    x2ap_enb_config_update_resp_t enb_config_update_res;
}son_rrc_enb_config_update_res_t;

/*! \ x2ap_enb_config_update_ind_t son_rrc_enb_config_update_ind_t
 *  \brief eNB config update indication message structure.
 *  Direction of the message: L3 X2AP to SON X2
 */
typedef x2ap_enb_config_update_ind_t son_rrc_enb_config_update_ind_t;

/*! \  struct son_x2_enb_config_update_ind_t
 *  \brief eNB config update indication message structure.
 *  Direction of the message: SON X2 to SON ANR
 *  \param p_enb_rec        Pointer to the eNB hash record
 *  \param p_update_ind     Pointer to the eNB Config Update indication data
 *                          received from L3 X2AP
 */
typedef struct _son_x2_enb_config_update_ind_t
{
    /* SPR_19279_START */
    son_global_enb_id_t             old_eNB_id;
    son_global_enb_id_t             new_eNB_id;
    /* SPR_19279_END */
    x2ap_enb_config_update_ind_t    *p_update_ind;
}son_x2_enb_config_update_ind_t;

/*! \  struct son_srv_cell_switch_on_off_info_t
 *  \brief Cell Switch ON/OFF indication message cell specific data.
 *  \param src_cgi                  Serving cell global identifier
 *  \param deactivation_indication  Deactivation Indication bit
 */
typedef struct _son_srv_cell_switch_on_off_info_t
{
    son_intra_rat_global_cell_id_t src_cgi;
    x2_deactivation_flag_et deactivation_indication;
}son_srv_cell_switch_on_off_info_t;

/*! \  struct son_x2_cell_switch_on_off_ind_t
 *  \brief Cell Switch ON/OFF indication message structure.
 * Direction of the message: SON ES to SON X2
 *  \param num_srv_cells_list_size      Number of served cells
 *  \param cell_switch_on_off_data_list List containing serving cells switch
 * on/off data
 */
typedef struct _son_x2_cell_switch_on_off_ind_t
{
    son_u16 num_srv_cells_list_size;
    son_srv_cell_switch_on_off_info_t cell_switch_on_off_data_list[MAX_SERVED_CELLS];
}son_x2_cell_switch_on_off_ind_t;

/*! \  struct son_srv_cell_updated_info_t
 *  \brief Structure containing cell Id and the operation performed by ANR to be
 * informed to the peer eNB via eNB config update request message.
 *  \param src_cgi      Serving cell global identifier
 *  \param nrt_op       Operation performed
 */
typedef struct _son_srv_cell_updated_info_t
{
    son_intra_rat_global_cell_id_t src_cgi;
    son_nrt_operation_performed_et nrt_op;
}son_srv_cell_updated_info_t;

/*! \  struct son_x2_nrt_update_ind_t
 *  \brief This is the structure of the message from ANR to send eNB config
 *  update request to peer eNBs. Direction of the message: SON ANR to SON X2
 *  \param p_enb_rec         	Pointer to the eNB record of peer to which eNB Config //SPR 16523 fix start,SPR 16523 fix stop
 * Update request will not be send
 *  \param num_srv_cells_list_size      Number of serving cells in be sent in
 *  the request
 *  \param srv_cell_updated_info_list   Array containg serving cell Id and
 *  the operation performed
 */
typedef struct _son_x2_nrt_update_ind_t
{
    /*SPR 16523 fix start*/
    son_nbr_enb_hash_rec_t      *p_enb_rec;
    /*SPR 16523 fix stop*/
    /* SPR Fix Start */
    son_bool_et is_local_update;
    /* SPR Fix End */
    son_u16 num_srv_cells_list_size;
    son_srv_cell_updated_info_t srv_cell_updated_info_list[MAX_SERVED_CELLS];
}son_x2_nrt_update_ind_t;

/*! \  struct son_rrc_add_enb_req_t
 *  \brief add eNB (X2 set up) request message structure.
 *   Direction of the message: SON X2 to L3 X2AP
 *  \param transaction_id       Transaction Identifier
 *  \param add_enb_req          add eNB request data
 */
typedef struct _son_rrc_add_enb_req_t
{
    son_u16 transaction_id;
    x2ap_add_enb_req_t add_enb_req;
}son_rrc_add_enb_req_t;

/*! \  struct son_rrc_add_enb_res_t
 *  \brief add eNB (X2 set up) response message structure.
 *   Direction of the message: L3 X2AP to SON X2
 *  \param transaction_id       Transaction Identifier
 *  \param add_enb_res          add eNB response data
 */
typedef struct _son_rrc_add_enb_res_t
{
    son_u16 transaction_id;
    x2ap_add_enb_res_t add_enb_res;
}son_rrc_add_enb_res_t;

/*! \ x2ap_enb_link_up_ind_t son_rrc_link_up_ind_t
 *  \brief Link UP indication message structure.
 * Direction of the message: L3 X2AP to SON X2
 */
typedef x2ap_enb_link_up_ind_t son_rrc_link_up_ind_t;

/*! \ x2ap_enb_link_down_ind_t son_rrc_link_down_ind_t
 *  \brief Link DOWN indication message structure.
 * Direction of the message: L3 X2AP to SON X2
 */
typedef x2ap_enb_link_down_ind_t son_rrc_link_down_ind_t;

/*! \  struct son_rrc_delete_enb_req_t
 *  \brief delete eNB (X2 termination) request message structure.
 * Direction of the message: SON X2 to L3 X2AP
 *  \param transaction_id       Transaction Identifier
 *  \param del_enb_req          delete eNB request data
 */
typedef struct _son_rrc_delete_enb_req_t
{
    son_u16 transaction_id;
    x2ap_delete_enb_req_t del_enb_req;
}son_rrc_delete_enb_req_t;

/*! \  struct son_rrc_delete_enb_res_t
 *  \brief delete eNB (X2 termination) response message structure.
 * Direction of the message: L3 X2AP to SON X2
 *  \param transaction_id       Transaction Identifier
 *  \param del_enb_res          delete eNB response data
 */
typedef struct _son_rrc_delete_enb_res_t
{
    son_u16 transaction_id;
    x2ap_delete_enb_res_t del_enb_res;
}son_rrc_delete_enb_res_t;

/*SPR 11689 changes start */

typedef struct _son_rrc_cell_activation_req_t
{
    son_u16 transaction_id;
    x2ap_cell_activation_req_t cell_activation_req;

}son_rrc_cell_activation_req_t;

typedef struct _son_rrc_cell_activation_resp_t
{
    son_u16 transaction_id;
    x2ap_cell_activation_response_t cell_activation_res;

}son_rrc_cell_activation_resp_t;

/*SPR 11689 changes end */


/*! \ son_oam_x2_link_up_ind_t son_x2_oam_link_up_ind_t
 *  \brief Link UP indication message structure.
 * Direction of the message: SON X2 to OAM
 */
typedef son_oam_x2_link_up_ind_t son_x2_oam_link_up_ind_t;

/*! \ son_oam_x2_link_down_ind_t son_x2_oam_link_down_ind_t
 *  \brief Link DOWN indication message structure.
 * Direction of the message: SON X2 to OAM
 */
typedef son_oam_x2_link_down_ind_t son_x2_oam_link_down_ind_t;

/*! \ x2ap_enb_config_update_wait_ind_t son_x2_enb_config_update_wait_ind
 *  \brief eNB config update wait indcation message structure.
 * Direction of the message: L3 X2AP to SON X2
 */
typedef x2ap_enb_config_update_wait_ind_t son_x2_enb_config_update_wait_ind;

#ifdef SON_MRO_FEATURE

/*! \ x2ap_ho_report_ind_t son_rrc_ho_report_ind_t
 *  \brief HO Report indication message structure.
 *  Direction of the message: L3 X2AP to SON X2
 */
typedef x2ap_ho_report_ind_t son_rrc_ho_report_ind_t;
#endif

typedef struct _son_rrc_mobility_change_resp_t
{
    son_u16 transaction_id;
    x2ap_mobility_change_resp_t mob_change_res;
}son_rrc_mobility_change_resp_t;

typedef struct _son_rrc_mobility_change_req_t
{
    son_u16 transaction_id;
    x2ap_mobility_change_req_t mob_change_req;
}son_rrc_mobility_change_req_t;

typedef struct _son_rrc_mobility_setting_change_resp_t
{
    son_u16 transaction_id;
    x2ap_mobility_setting_change_resp_t mob_setting_change_res;
}son_rrc_mobility_setting_change_resp_t;

typedef struct _son_rrc_mobility_setting_change_req_t
{
    son_u16 transaction_id;
    x2ap_mobility_setting_change_req_t mob_setting_change_req;
}son_rrc_mobility_settings_change_req_t;

/* SPR-10331 Fix Starts */
/*! \  struct son_x2_gu_group_id_config_req_t
 *  \brief Configure GU Group Ids of local eNB request message structure.
 *  Direction of the message: OAM to SON X2
 *  \param transaction_id           Transaction Identifier
 *  \param gu_group_id_config_req   Configure GU Group Ids of local eNB request structure
 */
typedef struct _son_x2_gu_group_id_config_req_t
{
    son_u16 transaction_id;
    son_oam_anr_gu_group_id_config_req_t gu_group_id_config_req;
}son_x2_gu_group_id_config_req_t;

/*! \  struct son_x2_gu_group_id_config_resp_t
 *  \brief Configure GU Group Ids of local eNB response message structure.
 *  Direction of the message: SON X2 to OAM
 *  \param transaction_id           Transaction Identifier
 *  \param gu_group_id_config_resp  Configure GU Group Ids of local eNB response structure
 */
typedef struct _son_x2_gu_group_id_config_resp_t
{
    son_u16 transaction_id;
    son_oam_anr_gu_group_id_config_resp_t gu_group_id_config_resp;
}son_x2_gu_group_id_config_resp_t;
/* SPR-10331 Fix Ends */

/* SPR_19619 start */

/***********************************************************************************
 * * SON_X2_PEER_CELL_ACTIVATION_REQ

 *   son_x2_peer_cell_activation_req_t 
 *   brief Send Peer Cell Activation Request to X2AP
 *   Direction of the message: SON ES to SON X2
 *   param transaction_id           Transaction Identifier
 *   param peer_cell_activation_req   Configure peer cell activation structure
 ***********************************************************************************/
typedef struct
{
    son_u16                                 transaction_id;
    son_oam_peer_cell_activation_req_t 	    peer_cell_activation_req;
}son_x2_peer_cell_activation_req_t;


typedef struct _son_rrc_peer_cell_activation_req_t
{
    son_u16 transaction_id;
    x2ap_peer_cell_activation_req_t         activation_req;
}son_rrc_peer_cell_activation_req_t;


typedef struct _son_rrc_peer_cell_activation_resp_t
{
    son_u16 transaction_id;
    x2ap_peer_cell_activation_resp_t        activation_res;
}son_rrc_peer_cell_activation_resp_t;

/* SPR_19619 stop */

#endif /* _SON_X2_INTF_H_ */
