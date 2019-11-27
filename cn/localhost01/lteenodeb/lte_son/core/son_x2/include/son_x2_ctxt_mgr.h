/*! \file son_x2_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_x2_ctxt_mgr.c
 *  \date  Aug, 2012
 *  \author Komal/Shekhar/Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_X2_CTXT_MGR_H_
#define _SON_X2_CTXT_MGR_H_

/*! \headerfile son_enb_table.h <>
 */
#include <son_enb_table.h>

/*! \headerfile son_x2_intf.h <>
 */
#include <son_x2_intf.h>

/*! \headerfile son_api_defines.h <>
 */
#include <son_api_defines.h>

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/** A constant global array SON_X2_FSM_STATES_NAMES
 * \brief X2 Fsm State Names
 */
extern const son_s8 *SON_X2_FSM_STATES_NAMES[];

/** A constant global array SON_X2_ENB_FSM_STATES_NAMES
 * \brief X2 ENB FSm State Names
 */
extern const son_s8 *SON_X2_ENB_FSM_STATES_NAMES[];

/* SPR-12708 Fix Starts */
/*! \def SON_X2_DEFAULT_TIMER
 *  \brief This macro is used to store the timer value used by X2 module
 *  to find the expiry of various pending response messages. The value is in
 *  seconds.
 */
#define SON_X2_DEFAULT_TIMER 5
/* SPR-12708 Fix Ends */

/*! \def SON_X2_SCTP_DEFAULT_PORT
 *  \brief This macro is used to store the default port of eNodeB on which
 * SCTP association needs to be initiated with the peer
 */
#define SON_X2_SCTP_DEFAULT_PORT 36422

/*! \def DEFAULT_ENB_RESET_RES_WAIT_TIME
 *  \brief This macro is used to store the waiting time for eNB reset response
 * after the eNB reset request message is sent by SON X2
 */
#define DEFAULT_ENB_RESET_RES_WAIT_TIME 5

/*! \def DEFAULT_ENB_CONFIG_UPDATE_RES_WAIT_TIME
 *  \brief This macro is used to store the waiting time for eNb config update
 *  response after the eNB config update response message is sent by SON X2
 */
#define DEFAULT_ENB_CONFIG_UPDATE_RES_WAIT_TIME 5

/*! \def SONX2_FSM_SET_STATE(curr_state, new_state)
 *  \brief This macro is used to update the state of Global FSM of SONX2
 */
#define SONX2_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, \
            SON_INFO, \
            "### eNB_FSM: State changed %s->%s ###", \
            SON_X2_FSM_STATES_NAMES[curr_state], \
            SON_X2_FSM_STATES_NAMES[new_state]); \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONX2_ENB_FSM_SET_STATE(p_enb_ctxt, prev_state, curr_state, new_state)
 *  \brief This macro is used to update the state of SONX2 ENodeB FSM
 */
#define SONX2_ENB_FSM_SET_STATE(p_enb_ctxt, prev_state, curr_state, new_state) \
    do \
    { \
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, \
            SON_INFO, \
            "### eNB_FSM State changed for eNodeB 0x%x %s->%s->%s ###", \
            son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id), \
            SON_X2_ENB_FSM_STATES_NAMES[prev_state], \
            SON_X2_ENB_FSM_STATES_NAMES[curr_state], \
            SON_X2_ENB_FSM_STATES_NAMES[new_state]); \
        prev_state = curr_state; \
        curr_state = new_state; \
    } \
    while (0)

/*! \def X2_GET_LOG_MODE anr_get_log_mode()
 *  \brief This macro is used to get the log level for the X2 module
 */
#define X2_GET_LOG_MODE anr_get_log_mode()

/*! \enum x2_global_fsm_state_et
 *  \brief An enum that defines the X2 global FSM states
 */
typedef enum
{
    X2_STATE_INIT,
    X2_STATE_ACTIVE,
    X2_STATE_CONNECTING,
    X2_STATE_CONNECTED,
    X2_STATE_SHUTDOWN,
    X2_NUM_OF_STATES
}x2_global_fsm_state_et;

/*! \enum son_x2_result_et
 *  \brief An enum that defines whether the message received by X2 eNB FSM was
 * expected or unexpected message
 */
typedef enum
{
    X2_ENB_MSG_EXPECTED,
    X2_ENB_MSG_UNEXPECTED
}son_x2_result_et;

/*! \  x2_hash_pool_t from STACKCPOOL
 */
typedef STACKCPOOL x2_hash_pool_t;

/*! \  struct x2_global_context_t
 *  \brief Structure defining the X2 module global context
 *  \param x2_global_fsm_state      X2 global FSM state
 *  \param nbr_enb_context_table    eNB hash table
 *  \param nbr_enb_hash_rec_pool    eNB Hash Context Pool
 *  \param hash_node_pool           Hash Table Pool
 */
typedef struct _x2_global_context_t
{
    x2_global_fsm_state_et x2_global_fsm_state;
    SON_HASH nbr_enb_context_table;
    SON_HASH nbr_enb_ip_addr_map;
    /* SPR Fix Start */
    SON_HASH nbr_ecgi_enb_id_map;
    /* SPR Fix End */
    x2_hash_pool_t nbr_enb_hash_rec_pool;
    x2_hash_pool_t hash_node_pool;
    x2_hash_pool_t nbr_enb_ip_addr_hash_rec_pool;
    /* SPR Fix Start */
    x2_hash_pool_t nbr_ecgi_enb_id_hash_rec_pool;
    /* SPR Fix End */
}x2_global_context_t;

/*! \fn x2_hash_pool_t* x2_get_hash_node_pool()
 * \brief This function returns hash table pool
 */
x2_hash_pool_t *
x2_get_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn x2_hash_pool_t *x2_get_enb_hash_rec_pool()
 * \brief This function returns hash table context pool
 */
x2_hash_pool_t *
x2_get_enb_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn x2_hash_pool_t *x2_get_enb_ip_addr_hash_rec_pool()
 * \brief This function returns hash table context pool
 */
x2_hash_pool_t *
x2_get_enb_ip_addr_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_HASH* x2_get_enb_nrt()
 * \brief This function is used to get the pointer to the eNB hash table
 */
SON_HASH *
x2_get_enb_nrt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn const son_void_t * x2_key_of_enb_config_cell_list(
 *          const SON_LIST_NODE*)
 * \brief This function determines the key in ENB config cell list
 * \param p_node - anchor field of the enb config cell list record
 */
const son_void_t *
x2_key_of_enb_config_cell_list
(
    const SON_LIST_NODE *p_node
);

/* SPR-10331 Fix Starts */
/* Lines Deleted */
/* SPR-10331 Fix Ends */

/*! \fn son_bool_et x2_modify_cell_in_enb_cfg_pending_list(
 *          son_enb_config_cell_list_t *, son_intra_rat_global_cell_id_t*,
 *          son_nrt_operation_performed_et, x2_deactivation_flag_et)
 *  \brief This function modifies cell in ENB Config Pending List
 *  \param p_enb_config_cell_list : ENB config cell list
 *  \param cell_id   : Intra rat global cell id
 *  \param last_op   : Last operation performed on NRT
 *  \param deac_flag : Deactivation flag
 */
son_bool_et
x2_modify_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t    *cell_id,
    son_nrt_operation_performed_et last_op,
    x2_deactivation_flag_et deac_flag
);

/*! \fn son_bool_et x2_delete_cell_in_enb_cfg_pending_list(
 *          son_enb_config_cell_list_t *, son_intra_rat_global_cell_id_t*)
 *  \brief This function deletes cell in ENB Config Pending List
 *  \param p_enb_config_cell_list : ENB config cell list
 *  \param cell_id : Intra rat global cell id
 */
son_bool_et
x2_delete_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t   *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t *cell_id
);

/*! \fn son_bool_et x2_clear_enb_cfg_pending_list(son_enb_config_cell_list_t *)
 *  \brief This function clears ENB Config Pending List
 *  \param p_enb_config_cell_list : ENB config cell list
 */
son_bool_et
x2_clear_enb_cfg_pending_list
(
    son_enb_config_cell_list_t *p_enb_config_cell_list
);

/*! \fn enb_config_cell_data_t* x2_find_cell_in_enb_cfg_pending_list(
 *          son_enb_config_cell_list_t *, son_intra_rat_global_cell_id_t*)
 *  \brief This function finds a cell in ENB Config Pending List
 *  \param p_enb_config_cell_list : ENB config cell list
 *  \param cell_id : Intra rat global cell id
 */
enb_config_cell_data_t *
x2_find_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t *cell_id
);

/*GUR32802*/

/*! \fn son_nbr_enb_hash_node_t *x2_insert_eNB_in_list(SON_LIST *p_list, son_nbr_enb_hash_rec_t *p_enb_rec);
 *  \brief This function inserts a eNB into the list
 *  \param p_list       - Pointer to the list
 *  \param p_enb_rec    - Pointer to eNB to inserted into the list
 */
son_nbr_enb_hash_node_t *
x2_insert_eNB_in_list
(
    SON_LIST                *p_list,
    son_nbr_enb_hash_rec_t   *p_enb_rec
);

/*! \fn son_nbr_enb_hash_node_t *x2_get_first_eNB_rec(SON_LIST *p_list);
 *  \brief This function returns pointer to first record in the list
 *  \param p_list       - Pointer to the list
 */
son_nbr_enb_hash_node_t *
x2_get_first_eNB_rec
(
    SON_LIST *p_list
);

/*! \fn son_nbr_enb_hash_node_t *x2_get_next_eNB_rec(son_nbr_enb_hash_node_t *p_node);
 *  \brief This function returns pointer to next eNb node in the list
 *  \param p_node   - Pointer to the eNb node
 */
son_nbr_enb_hash_node_t *
x2_get_next_eNB_rec
(
    son_nbr_enb_hash_node_t *p_node
);

/*! \fn son_void_t x2_clear_eNB_rec_list(SON_LIST *p_list)
 *  \brief This function clears all the eNb records in the list
 *  \param p_list   - Pointer to the list
 */
son_void_t
x2_clear_eNB_rec_list
(
    SON_LIST *p_list
);

/*! \fn son_void_t x2_convert_l3_plmn_id_to_son_plmn_id(x2ap_plmn_identity_t *p_l3_plmn_id, son_cell_plmn_info_t *p_son_plmn_id)
 *  \brief This function L3 plmn id format to son plmn id format
 *  \param p_l3_plmn_id     - Pointer to l3 plmn id
 *  \param p_son_plmn_id    - Pointer to son plmn id
 */
son_void_t
x2_convert_l3_plmn_id_to_son_plmn_id
(
    x2ap_plmn_identity_t *p_l3_plmn_id,
    son_cell_plmn_info_t *p_son_plmn_id
);

/*! \fn son_return_et x2_init_global_context()
 *  \brief This function initializes the X2 global context
 */
son_return_et
x2_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_return_et x2_init_global_context()
 *  \brief This function cleans up X2 global context
 */
son_void_t
x2_clear_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et x2_clean_up_enb_cfg_pending_data (son_nbr_enb_hash_rec_t *p_enb_ctxt)
 *  \brief This function cleans up the enb pending list for X2
 * 8  \param p_enb_ctxt   - Pointer to enb hash record
 */
son_bool_et
x2_clean_up_enb_cfg_pending_data
(
    son_nbr_enb_hash_rec_t      *p_enb_ctxt
);

/*! \fn x2_hash_pool_t *x2_get_nbr_enb_hash_rec_pool()
 *  \brief This function gets pointer to the eNB context pool
 */
x2_hash_pool_t *
x2_get_nbr_enb_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn x2_hash_pool_t *x2_get_nbr_enb_ip_addr_hash_rec_pool()
 *  \brief This function gets pointer to the eNB IP address context pool
 */
x2_hash_pool_t *
x2_get_nbr_enb_ip_addr_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn x2_hash_pool_t *x2_get_enb_hash_node_pool()
 *  \brief This function gets pointer to the eNB table pool
 */
x2_hash_pool_t *
x2_get_enb_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_enb_add_status_node_t *x2_insert_add_res_node(SON_LIST *p_list, son_enb_add_status_t *p_enb_add_status)
 *  \brief This Function adds eNB in X2 response list
 *  \param p_list           - Pointer to the list
 *  \param p_enb_add_status - Pointer to the structure containing the data stored in the eNB add response list
 */
son_enb_add_status_node_t *
x2_insert_add_res_node
(
    SON_LIST                *p_list,
    son_enb_add_status_t    *p_enb_add_status
);

/*GUR32802*/

/*! \fn son_void_t x2_convert_l3_enbid_to_son_enbid(x2_gb_enb_id_t *p_l3_enbid, son_global_enb_id_t *p_son_enbid)
 *  \brief This function is used to convert eNB Id value provided by L3 X2AP to the eNB Id value required by SON
 *  \param p_l3_enbid   Pointer to the eNB Id value defined by L3 X2AP
 *  \param p_son_enbid  Pointer to the eNB Id value defined by SON
 */
son_void_t
x2_convert_l3_enbid_to_son_enbid
(
    x2_gb_enb_id_t      *p_l3_enbid,
    son_global_enb_id_t *p_son_enbid
);

/*! \fn son_void_t x2_convert_son_enbid_to_l3_enbid(son_global_enb_id_t *p_son_enbid, x2_gb_enb_id_t *p_l3_enbid)
 *  \brief This function is used to convert eNB Id value provided by SON to the eNB Id value required by L3 X2AP
 *  \param p_son_enbid  Pointer to the eNB Id value defined by SON
 *  \param p_l3_enbid   Pointer to the eNB Id value defined by L3 X2AP
 */
son_void_t
x2_convert_son_enbid_to_l3_enbid
(
    son_global_enb_id_t *p_son_enbid,
    x2_gb_enb_id_t      *p_l3_enbid
);

/*! \fn son_void_t x2_populate_srv_cell_info(son_intra_rat_global_cell_id_t *p_cell_id, x2ap_served_cell_info_arr_t *p_cell_info)
 *  \brief This function populates the given serving cell and its neighbor
 * information
 *  \param p_cell_id    Pointer to the serving cell's global cell id
 *  \param p_cell_info  Pointer to the serving cell information strutcture to be filled
 */
son_void_t
x2_populate_srv_cell_info
(
    son_intra_rat_global_cell_id_t  *p_cell_id,
    x2ap_served_cell_info_arr_t     *p_cell_info
);

/*! \fn son_void_t x2_populate_cells_to_modify_list_info(son_intra_rat_global_cell_id_t *p_src_cgi, x2_deactivation_flag_et deacitvation_ind, x2ap_served_cells_to_modify_info_t *p_mod_info)
 *  \brief This function populates the cells to modify list of eNB config update request message.
 *  \param p_src_cgi            Pointer to the source cell Id
 *  \param deacitvation_ind     Deactivation Indication bi
 *  \param p_mod_info           Pointer to the cells to modify list
 */
son_void_t
x2_populate_cells_to_modify_list_info
(
    son_intra_rat_global_cell_id_t      *p_src_cgi,
    x2_deactivation_flag_et deacitvation_ind,
    x2ap_served_cells_to_modify_info_t  *p_mod_info
);

/* SPR_19619 start */

/*! \fn son_void_t x2_populate_cells_for_peer_cell_activation(son_intra_rat_global_cell_id_t *p_src_cgi, x2ap_served_cells_to_modify_info_t *p_mod_info)
 *  \brief This function populates the cells for peer cell activation request message.
 *  \param p_src_cgi            Pointer to the source cell Id
 *  \param p_mod_info           Pointer to the cells 
 */
son_void_t
x2_populate_cells_for_peer_cell_activation
(
    son_intra_rat_global_cell_id_t      *p_src_cgi,
    x2ap_served_cells_to_modify_info_t  *p_mod_info
);

/* SPR_19619 stop */


/*Start: GUR32802*/

/*! \fn son_void_t x2_insert_cell_in_enb_cfg(son_nbr_enb_hash_rec_t *p_enb_rec, son_intra_rat_global_cell_id_t *p_cell_id, son_nrt_operation_performed_et last_op, x2_deactivation_flag_et deac_flag)
 *  \brief This Function inserts new cell in ENB Config Pending List
 *  \param p_enb_rec            - Pointer to eNB record node
 *  \param p_cell_id            - Pointer to EUTRAN global cell id
 *  \param last_op              - Operation to be performed on NRT
 *  \param deac_flag            - Deactivation indication bit for a cell
 */
son_void_t
x2_insert_cell_in_enb_cfg
(
    son_nbr_enb_hash_rec_t *p_enb_rec,
    son_intra_rat_global_cell_id_t *p_cell_id,
    son_nrt_operation_performed_et last_op,
    x2_deactivation_flag_et deac_flag
);

/*! \fn son_void_t x2_create_send_enb_config_update_req_msg(
 * son_enb_config_cell_list_t *p_pnding_cell_list, son_nbr_enb_hash_rec_t *p_enb_ctxt)
 *  \param p_pnding_cell_list   - Pointer to eNB config update request data
 *  \param p_enb_ctxt           - Pointer to eNB record node
 */
son_void_t
x2_create_send_enb_config_update_req_msg
(
    son_enb_config_cell_list_t      *p_pnding_cell_list,
    son_nbr_enb_hash_rec_t          *p_enb_ctxt
);

/*END: GUR32802*/

/*! \fn son_void_t x2_populate_cell_id_for_l3(son_u8 *p_son_cell_identity, son_u8 *p_l3_cell_identity)
 *  \brief This function populates the cells Id to be sent to L3.
 *  \param p_son_cell_identity      Pointer to the input cell identity array
 *  \param p_l3_cell_identity       Pointer to the cell Id value for L3
 */
son_void_t
x2_populate_cell_id_for_l3
(
    son_u8  *p_son_cell_identity,
    son_u8  *p_l3_cell_identity
);

/*! \fn son_void_t x2_print_enb_id(son_global_enb_id_t *p_enb_id)
 *  \brief This function prints the value of eNB Identifier.
 *  \param p_enb_id     Pointer to the eNB Id
 */
son_void_t
x2_print_enb_id
(
    son_global_enb_id_t *p_enb_id
);

/*! \fn son_void_t x2_print_plmn_id(son_cell_plmn_info_t *p_plmn_id)
 *  \brief This function prints the value of Plmn Identifier.
 *  \param p_plmn_id     Pointer to the plmn Id
 */
son_void_t
x2_print_plmn_id
(
    son_cell_plmn_info_t *p_plmn_id
);

/*! \fn son_void_t print_tnl_address_list(son_u8 list_size, son_enb_tnl_address_t *p_tnl_add_list)
 *  \brief This function prints the TNL addresses for a peer eNB.
 *  \param list_size            Size of the TNL address list
 *  \param p_tnl_add_list       Pointer to the TNL address list
 */
son_void_t
print_tnl_address_list
(
    son_u8 list_size,
    son_enb_tnl_address_t   *p_tnl_add_list
);

/*! \fn son_void_t x2_print_enb_table()
 *  \brief This function prints the peer eNBs hash table data.
 */
son_void_t
x2_print_enb_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t x2_print_pending_enb_config_update_req_data(son_enb_config_cell_list_t *p_enb_config_cell_list)
 *  \brief This function prints the pending data stored for eNB config update request to peer eNB.
 *  \param *p_enb_config_cell_list  Pointer to the eNB Config Update Request pending data list
 */
son_void_t
x2_print_pending_enb_config_update_req_data
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list
);

/*! \fn son_void_t x2_convert_son_ip_addr_to_l3_ip_addr(son_u8 *p_son_ip_addr,
 * son_u8 *p_l3_ip_addr)
 *  \brief This function converts the IP address from the notation used by SON to
 * the notation used by L3 X2AP.
 *  \param p_son_ip_addr    Pointer to IP address in the notation used by SON
 *  \param p_l3_ip_addr     Pointer to the IP address in the notation used by L3 X2AP
 */
son_void_t
x2_convert_son_ip_addr_to_l3_ip_addr
(
    son_u8  *p_son_ip_addr,
    son_u8  *p_l3_ip_addr
);

/*! \fn son_void_t x2_convert_l3_ip_addr_to_son_ip_addr(son_u8 *p_l3_ip_addr,
 * son_u8 *p_son_ip_addr)
 *  \brief This function converts the IP address from the notation used by L3
 * X2AP to the notation used by SON.
 *  \param p_l3_ip_addr     Pointer to the IP address in the notation used by L3 X2AP
 *  \param p_son_ip_addr    Pointer to IP address in the notation used by SON
 */
son_void_t
x2_convert_l3_ip_addr_to_son_ip_addr
(
    son_u8  *p_l3_ip_addr,
    son_u8  *p_son_ip_addr
);

/* SPR 7528 IPV6 Support Start */
/*! \fn son_return_et x2_convert_son_ipv6_addr_to_l3_ipv6_addr(son_u8  *p_son_ipv6_addr,
 * son_u8  *p_l3_ipv6_addr)
 *  \brief This function converts the IP address from the notation used by SON to the notation
 * used by L3 X2AP.
 *  \param  p_son_ipv6_addr     Pointer to IP address in the notation used by SON
 *  \param  p_l3_ipv6_addr      Pointer to the IP address in the notation used by L3 X2AP
 */
son_return_et
x2_convert_son_ipv6_addr_to_l3_ipv6_addr
(
    son_u8  *p_son_ipv6_addr,
    son_u8  *p_l3_ipv6_addr
);

/*! \fn son_return_et x2_convert_l3_ipv6_addr_to_son_ipv6_addr(son_u8  *p_l3_ipv6_addr,
 * son_u8  *p_son_ipv6_addr)
 *  \brief This function converts the IP address from the notation used by
 * L3 X2AP to the notation used by SON.
 *  \param  p_l3_ipv6_addr      Pointer to IP address received from L3 X2AP
 *  \param  p_son_ipv6_addr     Pointer to the IP address in the notation used by SON.
 */
son_return_et
x2_convert_l3_ipv6_addr_to_son_ipv6_addr
(
    son_u8  *p_l3_ipv6_addr,
    son_u8  *p_son_ipv6_addr
);
/* SPR 7528 IPV6 Support End */

/*! \fn son_void_t x2_convert_l3_ecgi_to_son_ecgi(x2ap_ecgi_t *p_x2_cgi, son_intra_rat_global_cell_id_t *p_son_cgi)
 *  \brief This is the function which is used to convert CGI value received from L3 to the CGI value required by SON
 *  \param p_x2_cgi     Pointer to the CGI value defined by L3 X2AP
 *  \param p_son_cgi    Pointer to the CGI value defined by SON
 */
son_void_t
x2_convert_l3_ecgi_to_son_ecgi
(
    x2ap_ecgi_t                     *p_x2_cgi,   /* Input parameter */
    son_intra_rat_global_cell_id_t  *p_son_cgi /* Output parameter */
);

#ifdef SON_MRO_FEATURE


/*! \fn son_void_t x2_map_ho_report_type_enum(son_ul32 *p_l3_ho_report_type, rrm_ho_cause_et *p_rrm_ho_report_type)
 *  \brief This is the function which is used to map the HO report type value recieved from L3 X2AP to the HO report enum value defined by RRM.
 *  \param p_l3_ho_report_type  Pointer to the HO report type.
 *  \param p_rrm_ho_report_type Pointer to the HO report type enum value defined by RRM
 */
son_void_t
x2_map_ho_report_type_enum
(
    son_ul32                *p_l3_ho_report_type,
    rrm_ho_cause_et         *p_rrm_ho_report_type
);

#endif

/*! \fn son_void_t x2_convert_son_ecgi_to_l3_ecgi(son_intra_rat_global_cell_id_t *p_son_cgi, x2ap_ecgi_t *p_x2_cgi)
 *  \brief This is the function which is used to convert CGI value stored by SON to the CGI value required by L3 X2AP
 *  \param p_son_cgi    Pointer to the CGI value defined by SON
 *  \param p_x2_cgi     Pointer to the CGI value defined by L3 X2AP
 */
son_void_t
x2_convert_son_ecgi_to_l3_ecgi
(
    son_intra_rat_global_cell_id_t  *p_son_cgi, /* Input parameter */
    x2ap_ecgi_t                     *p_x2_cgi   /* Output parameter */
);

/* SPR Fix Start */

/*! \fn x2_hash_pool_t *x2_get_nbr_ecgi_enb_id_hash_rec_pool()
 *  \brief This function gets pointer to the map storing the mapping of Neighbo ECGI and eNB Id
 */
x2_hash_pool_t *
x2_get_nbr_ecgi_enb_id_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 x2_get_active_nbr_count()
 *  \brief This function is used to calculate the total number of active X2 connections.
 */
son_u32
x2_get_active_nbr_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/* SPR Fix End */

/* SPR-10331 Fix Starts */
/*! \fn son_void_t x2_convert_son_gu_group_id_to_l3_gu_group_id(son_gu_group_id_t *p_son_gu_grp_id, x2ap_gu_group_id_t *p_x2_gu_grp_id)
 *  \brief This is the function which is used to convert GU Group Id value stored by SON to the GU Group Id value required  by L3 X2AP
 *  \param p_son_gu_grp_id  Pointer to the GU Group Id value defined by SON
 *  \param p_x2_gu_grp_id   Pointer to the GU Group Id value defined by L3 X2APdefined by L3 X2AP
 */
son_void_t
x2_convert_son_gu_group_id_to_l3_gu_group_id
(
    son_gu_group_id_t   *p_son_gu_grp_id,   /* Input parameter */
    x2ap_gu_group_id_t  *p_x2_gu_grp_id     /* Output parameter */
);

/*! \fn son_void_t x2_print_pending_gu_group_id_data(son_nbr_enb_hash_rec_t *p_enb_rec)
 *  \brief This function prints the pending GU Group Id data stored for eNB config update request to peer eNB.
 *  \param p_enb_rec   Pointer to the eNB context
 */
son_void_t
x2_print_pending_gu_group_id_data
(
    son_nbr_enb_hash_rec_t *p_enb_rec
);

/*! \fn son_void_t x2_print_sent_gu_group_id_data(son_nbr_enb_hash_rec_t *p_enb_rec)
 *  \brief This function prints the sent GU group Id data in eNB config update request to peer eNB whose response is awaited.
 *  \param p_enb_rec   Pointer to the eNB context
 */
son_void_t
x2_print_sent_gu_group_id_data
(
    son_nbr_enb_hash_rec_t *p_enb_rec
);
/* SPR-10331 Fix Ends */
/*SPR_16835_START*/
/*! \fn son_void_t x2_delete_enb_config_cell_list(son_enb_config_cell_list_t      *p_pnding_cell_list)
 *  \brief Function shall delete the pending eNB list.
 *  \param p_pnding_cell_list :- Pending list pointer
 */
son_void_t
x2_delete_enb_config_cell_list(
son_enb_config_cell_list_t      *p_pnding_cell_list
);
/*SPR_16835_END*/

#endif /* _SON_X2_CTXT_MGR_H_ */
