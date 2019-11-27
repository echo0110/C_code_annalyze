/*! \file son_enb_table.h
 *  \brief This file contains the prototypes of functions defined in son_anr_enb_table.c
 *  \date  Aug, 2012
 *  \author Komal/Shekhar
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_X2_ENB_TABLE_H_
#define _SON_X2_ENB_TABLE_H_

/*! \headerfile son_hash.h <>
 */
#include <son_hash.h>

/*! \headerfile son_list.h<>\
 */
#include <son_list.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/* SPR 10331 Fix Starts */
/*! \headerfile rrc_x2apCommon_intf.h <>
 */
#include <rrc_x2apCommon_intf.h>
/* SPR 10331 Fix Ends */

/*! \def X2_MAX_INITIAL_NR_TBL_BKT
 *  \brief A macro that stores number of buckets of the hash table
 */
#define X2_MAX_INITIAL_NR_TBL_BKT 50

/*! \def X2_INIT_HASH_CTXT_POOL
 *  \brief A macro that stores initial size context and table pool
 */
#define X2_INIT_HASH_CTXT_POOL   100

/*! \def X2_MAX_HASH_CTXT_POOL
 *  \brief A macro that stores maximum size context and table pool
 */
#define X2_MAX_HASH_CTXT_POOL    SON_MAX_NO_NEIGHBOUR_ENBS

/*SPR_16835_START*/
/*! \def SON_INVALID_TXN_ID
 *  \brief Invalid Transaction ID As txn id is U16
 */
#define SON_INVALID_TXN_ID        0x10000 
/*SPR_16835_END*/

/*! \enum x2_enb_fsm_state_et
 *  \brief An enum that defines the X2 eNB FSM states
 */
typedef enum
{
    X2_ENB_STATE_DISCONNECTED,
    X2_ENB_STATE_CONNECTING,
    X2_ENB_STATE_CONNECTED,
    X2_ENB_STATE_DISCONNECTING,
    X2_ENB_STATE_RESETTING,
    X2_ENB_NUM_OF_STATES
}x2_enb_fsm_state_et;

/*! \enum x2_deactivation_flag_et
 *  \brief An enum that is used to store the deactivation indication
 *     bit for a particular cell
 */
typedef enum
{
    X2_CELL_DEACTIVATION_IND_NULL = 0,
    X2_CELL_DEACTIVATION_TRUE,
    X2_CELL_DEACTIVATION_FALSE,
    X2_CELL_DEACTIVATION_UNDEFINED = 0xFF
}x2_deactivation_flag_et;

/*! SON_LIST son_enb_config_cell_list_t
 *  \brief Linked list containg eNB config update request data to be send to
 * peer eNBs in eNB config update request message
 */
typedef SON_LIST son_enb_config_cell_list_t;

/*! \  struct enb_cell_data_t
 *  \brief Structure containing the data stored in son_enb_config_cell_list_t
 *  list
 *  \param cell_id          CGI of source cell
 *  \param nrt_op           NRT Operation performed on the cell
 *  \param deacivation_flag deactivation indication bit
 */
typedef struct _enb_cell_data_t
{
    son_intra_rat_global_cell_id_t cell_id;
    son_nrt_operation_performed_et last_op;
    x2_deactivation_flag_et deacivation_flag;
    son_module_id_et module_id;
}enb_cell_data_t;

/*! \  struct enb_config_cell_data_t
 *  \brief Structure containing son_enb_config_cell_list_t list nodes data
 *  \param enb_cell_node    YLNODE structure storing the next and
 *                          previous pointer to the list nodes
 *  \param enb_cell_data    Data stored in the list nodes
 *                          (Refer enb_cell_data_t)
 */
typedef struct _enb_config_cell_data_t
{
    SON_LIST_NODE enb_cell_node;
    enb_cell_data_t enb_cell_data;
}enb_config_cell_data_t;

/* SPR 10331 Fix Starts */

typedef SON_LIST x2_gu_group_id_list_t;

/*! \typedef struct  son_nbr_enb_gu_group_id_info_t
 *  \brief Strucutre storing the GU Group Id information of peer eNB
 *  \param add_gu_id            GU Group Id to add list
 *  \param delete_gu_id         GU Group Id to delete list
 */
typedef struct _son_nbr_enb_gu_group_id_info_t
{
    x2_gu_group_id_list_t add_gu_id_list;
    x2_gu_group_id_list_t delete_gu_id_list;
}son_nbr_enb_gu_group_id_info_t;

/*! \  struct x2_gu_group_id_node_t
 *  \brief Structure containing x2_gu_group_id_list_t list nodes data
 *  \param anchor                   YLNODE structure storing the next and
 *                                  previous pointer to the list nodes
 *  \param gu_grp_id                Data (GU Group Id)stored in the list nodes
 */
typedef struct _x2_gu_group_id_node_t
{
    SON_LIST_NODE                   anchor;
    x2ap_gu_group_id_t              gu_group_id;
}x2_gu_group_id_node_t;

/* SPR 10331 Fix Ends */

/*! \typedef struct  son_nbr_enb_hash_rec_t
 *  \brief It is the structure for eNB record node
 *  \param enb_id                       Global eNB Id
 *  \param tac_list_size                TAC list size
 *  \param tac_list                     tac_list
 *  \param enb_tnl_address_list_size    Number of IP Addresses
 *  \param enb_tnl_address_list         Array containing IP address of eNB on which SCTP association is established
 *  \param x2_status                    Indicates whether X2 connection can be used for HO or not with the corresponding eNB
 *  \param nr_status                    Indicates whether HO is allowed or not to the corresponding eNB
 *  \param enb_prev_fsm_state           eNB FSM previous state
 *  \param enb_curr_fsm_state           eNB FSM current state
 *  \param time_d_wait_expiry           time_d_wait expiry value if time_d_wait is received from corresponding peer eNB
 *  \param enb_config_cell_info         Structure to the list (Refer son_enb_config_cell_list_t)
 *  \param is_self_del_enb_req_triggered Field indicates if X2 module has initiated delete eNB requeset on its own or not
 *  \param is_enb_config_update_res_awaited Flag indicating if eNB config update response message is awaited 
 *  \param pending_gu_group_id_data     Stores pending eNB config update request's GU group Id data 
 *  \param sent_gu_group_id_data        Stores eNB config update request's GU group Id data which has been sent to L3 X2AP and its response is awaited 
 */
typedef struct _son_nbr_enb_hash_rec_t
{
    son_global_enb_id_t enb_id;
    son_u8 tac_list_size;
    son_tac_t tac_list[SON_MAX_SERVED_CELLS_AT_NBR_ENB];
    son_u8 enb_tnl_address_list_size;
    son_enb_tnl_address_t enb_tnl_address_list[SON_MAX_NO_ENB_X2_TLAS];
    son_x2_status_et x2_status;
    son_nr_status_et nr_status;
    x2_enb_fsm_state_et enb_prev_fsm_state;
    x2_enb_fsm_state_et enb_curr_fsm_state;
    son_time_t time_d_wait_expiry;
    /* SPR 11340 Fix Starts */
    /* Line deleted */
    son_bool_et is_enb_config_update_res_awaited;
    /* SPR 11340 Fix Ends */
    son_enb_config_cell_list_t enb_config_cell_info;
    son_bool_et is_self_del_enb_req_triggered;
    /* SPR 10331 Fix Starts */
    son_nbr_enb_gu_group_id_info_t pending_gu_group_id_data;
    son_nbr_enb_gu_group_id_info_t sent_gu_group_id_data;
    /* SPR 10331 Fix Ends */
    /* SPR_16835_START */
    son_u32 txn_id_for_del_req;
    /* SPR_16835_END */
}son_nbr_enb_hash_rec_t;

/*! \typedef struct  son_nbr_enb_ip_addr_hash_rec_t
 *  \brief It is the structure for eNB IP record node
 *  \param enb_id                       Global eNB Id
 *  \param enb_tnl_address              IP Addresses
 */
typedef struct _son_nbr_enb_ip_addr_hash_rec_t
{
    son_global_enb_id_t enb_id;
    son_enb_tnl_address_t enb_tnl_address;
}son_nbr_enb_ip_addr_hash_rec_t;

/*! \typedef struct  son_nbr_enb_hash_tuple_t
 *  \brief It is the structure for nbr eNB
 *  \param anchor           anchor field of the node
 *  \param data             data field to contain eNB information
 */
typedef struct  _son_nbr_enb_hash_tuple_t
{
    SON_HASH_NODE anchor;
    son_nbr_enb_hash_rec_t data;
}son_nbr_enb_hash_tuple_t;

/*! \typedef struct  son_nbr_enb_ip_addr_hash_tuple_t
 *  \brief It is the structure for nbr eNB
 *  \param anchor           anchor field of the node
 *  \param data             data field to contain eNB IP information
 */
typedef struct  _son_nbr_enb_ip_addr_hash_tuple_t
{
    SON_HASH_NODE anchor;
    son_nbr_enb_ip_addr_hash_rec_t data;
}son_nbr_enb_ip_addr_hash_tuple_t;

/* SPR Fix Start */

/*! \typedef struct son_nbr_ecgi_enb_id_hash_rec_t
 *  \brief It is the structure for storing the neighbor ECGI and eNB Id
 *  \param nbr_cell_id                      Neighbor Global Cell Id
 *  \param nbr_enb_id                       Neighbor Global eNB Id
 *  \param enb_tnl_address              IP Addresses
 */
typedef struct _son_nbr_ecgi_enb_id_hash_rec_t
{
    son_intra_rat_global_cell_id_t nbr_cell_id;
    son_global_enb_id_t nbr_enb_id;
}son_nbr_ecgi_enb_id_hash_rec_t;

/*! \typedef struct  son_nbr_ecgi_enb_id_hash_tuple_t
 *  \brief It is the structure defining the tuple for nbr_ecgi_enb_id_map
 *  \param anchor           anchor field of the node
 *  \param data             data field to contain the neighbor ECGI and eNB ID
 */
typedef struct  _son_nbr_ecgi_enb_id_hash_tuple_t
{
    SON_HASH_NODE anchor;
    son_nbr_ecgi_enb_id_hash_rec_t data;
}son_nbr_ecgi_enb_id_hash_tuple_t;
/* SPR Fix End */

/*! \fn son_return_et x2_init_nbr_enb_table(SON_HASH *H )
 *  \brief This function initializes the eNB context table
 *  \param H    - pointer to eNb hash context to initialize
 */
son_return_et
x2_init_nbr_enb_table
(
    SON_HASH *H
);

/*! \fn son_return_et x2_init_nbr_enb_ip_addr_map(SON_HASH *H )
 *  \brief  This function creates neighbor eNB IP address map
 *          present in global context
 *  \param H    - pointer to eNb IP address hash context to initialize
 */
son_return_et
x2_init_nbr_enb_ip_addr_map
(
    SON_HASH *H
);

/*! \fn son_ul32 x2_nbr_enb_hash_func(const son_void_t *p_key)
 *  \brief This is function for generating hash key for the given
 *         neighbor eNB hash record
 *  \param p_key - Pointer to the structure of type son_nbr_enb_hash_rec_t
 */
son_ul32
x2_nbr_enb_hash_func
(
    const son_void_t *p_key
);

/*! \fn son_ul32 x2_nbr_enb_ip_addr_hash_func(const son_void_t *p_key)
 *  \brief This is function for generating hash key for the given
 *         neighbor eNB IP Address hash record
 *  \param p_key - Pointer to the structure of type son_nbr_enb_ip_addr_hash_rec_t
 */
son_ul32
x2_nbr_enb_ip_addr_hash_func
(
    const son_void_t *p_key
);

/*! \fn  son_s32 x2_nbr_enb_cmp_func(const son_void_t *p_val_1,const son_void_t *p_val_2)
 *  \brief This is a hash function used for comparing the keys
 *  \param p_val_1 - Pointer to the structure of type son_nbr_enb_hash_rec_t
 *  \param p_val_2 - Pointer to the structure of type son_nbr_enb_hash_rec_t
 */
son_s32
x2_nbr_enb_cmp_func
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn  son_s32 x2_nbr_enb_ip_addr_cmp_func(const son_void_t *p_val_1,const son_void_t *p_val_2)
 *  \brief This is a hash function used for comparing the keys
 *  \param p_val_1 - Pointer to the structure of type son_nbr_enb_ip_addr_hash_rec_t
 *  \param p_val_2 - Pointer to the structure of type son_nbr_enb_ip_addr_hash_rec_t
 */
son_s32
x2_nbr_enb_ip_addr_cmp_func
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn  const son_void_t *x2_nbr_enb_key_off_nr_func( const SON_HASH_NODE *p_node)
 *  \brief This is a function used for find the data field of the tuple
 *         corresponding to the given p_node
 *  \param p_node -  Pointer to the anchor field of nbr eNB hash tuple
 */
const son_void_t *
x2_nbr_enb_key_off_nr_func
(
    const SON_HASH_NODE *p_node
);

/*! \fn  const son_void_t * x2_nbr_enb_ip_addr_key_off_nr_func( const SON_HASH_NODE *p_node)
 *  \brief This is a function used for find the data field of the tuple
 *         corresponding to the given p_node
 *  \param p_node -  Pointer to the anchor field of nbr eNB hash tuple
 */
const son_void_t *
x2_nbr_enb_ip_addr_key_off_nr_func
(
    const SON_HASH_NODE *p_node
);

/*! \fn son_nbr_enb_hash_rec_t* x2_insert_nbr_enb(SON_HASH *p_hash,
 * son_neighbor_enb_info_t *p_enb_info, son_error_et *p_error_code)
 *  \brief This function is used to insert an entry in the neighbor eNB
 *          context Table to store neighbor eNB information
 *  \param p_hash - Pointer to the nbr eNB hash context.
 *  \param p_enb_info - Pointer to a structure containing the
 *                          nbr eNB information.
 *  \param p_error_code -  Pointer to a variable for storing error code
 */
son_nbr_enb_hash_rec_t *
x2_insert_nbr_enb
(
    SON_HASH                *p_hash,
    son_neighbor_enb_info_t *p_enb_info,
    son_error_et            *p_error_code
);

/*! \fn  son_nbr_enb_hash_rec_t* x2_modify_nbr_enb(SON_HASH *p_hash, son_neighbor_enb_info_t *p_enb_info, son_error_et *p_error_code)
 *  \brief This function is used for updating selected neighbor eNB
 *         record
 *  \param p_hash - Pointer to the neighbor eNB  hash context.
 *  \param p_enb_info - Pointer to a structure for updating the
 *                           existing neighbor eNB record.
 *  \param p_error_code -  Pointer to a variable for storing error code
 */
son_nbr_enb_hash_rec_t *
x2_modify_nbr_enb
(
    SON_HASH                *p_hash,
    son_neighbor_enb_info_t *p_enb_info,
    son_error_et            *p_error_code
);

/*! \fn son_return_et x2_del_nbr_enb(SON_HASH *p_hash,
 * son_global_enb_id_t  *p_nbr_enb_id, son_error_et *p_error_code)
 *  \brief This function is used for removing selected neighbor eNB
 *         records from the nbr eNB context table
 *  \param p_hash - Pointer to the neighbor eNB hash context.
 *  \param p_nbr_enb_id   - Pointer to the neighbor enb id.
 *  \param p_error_code -  Pointer to a variable for storing error code
 */
son_return_et
x2_del_nbr_enb
(
    SON_HASH            *p_hash,
    son_global_enb_id_t *p_nbr_enb_id,
    son_error_et        *p_error_code
);

/*! \fn son_nbr_enb_hash_tuple_t* x2_find_nbr_enb(SON_HASH *p_hash,
 * son_global_enb_id_t *p_nbr_enb_id, son_error_et *p_error_code)
 *  \brief This function returns the eNB info of a specified neighbor.
 *  \param p_hash   p_hash -   Pointer to the  neighbor eNB hash context.
 *  \param p_nbr_enb_id   - Pointer to the neighbor eNB Id.
 *  \param p_error_code -  Pointer to a variable for storing error code
 */
son_nbr_enb_hash_tuple_t *
x2_find_nbr_enb
(
    SON_HASH            *p_hash,
    son_global_enb_id_t *p_nbr_enb_id,
    son_error_et        *p_error_code
);

/*! \fn son_nbr_enb_hash_tuple_t* x2_get_next_nbr_enb_from_nrt(
 * SON_HASH *p_hash, SON_HASH_NODE   *p_node)
 *  \brief Function to return the next neighbor eNB in the table
 *  \param p_hash -   Pointer to the neighbor eNB hash context.
 *  \param p_node   Pointer to the current hash node
 */
son_nbr_enb_hash_tuple_t *
x2_get_next_nbr_enb_from_nrt
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
);

/*! \fn son_nbr_enb_hash_tuple_t* x2_get_first_nbr_enb_from_nrt(SON_HASH *p_hash)
 *  \brief Function to return the first neighbor eNB record
 *         from neighbor eNB context table
 *  \param p_hash -  p_hash -   Pointer to the neighbor eNB hash context.
 */
son_nbr_enb_hash_tuple_t *
x2_get_first_nbr_enb_from_nrt
(
    SON_HASH *p_hash
);

/*! \fn son_void_t   x2_clear_enb_table(SON_HASH *p_hash)
 *  \brief  This function clears entire eNB hash context.
 *  \param p_hash -   Pointer to the neighbor eNB hash context.
 */
son_void_t
x2_clear_enb_table
(
    SON_HASH *p_hash
);

/*! \fn son_void_t   x2_clear_enb_ip_addr_table(SON_HASH *p_hash)
 *  \brief  This function clears entire eNB hash context.
 *  \param p_hash -   Pointer to the neighbor eNB hash context.
 */
son_void_t
x2_clear_enb_ip_addr_table
(
    SON_HASH *p_hash
);

/*! \fn  son_return_et x2_init_nbr_enb_db()
 *  \brief This function initialize db for neighbor eNBs.
 */
son_return_et
x2_init_nbr_enb_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t x2_clear_nbr_enb_db()
 *  \brief  This function clears db for neighbor eNBs.
 */
son_void_t
x2_clear_nbr_enb_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t x2_init_enb_neighbor_info(son_neighbor_enb_info_t *p_enb_info, son_global_enb_id_t *p_enb_id, son_u8 tnl_addr_size,, son_enb_tnl_address_t * p_tnl_add, son_u8 *p_tac)
 *  \brief This function is used to initiallize the data of the neighbor eNB
 *  while inserting node in the eNB hash table.
 *  \param p_enb_info       Pointer to the eNB info to be initialized
 *  \param p_enb_id         Pointer to the eNB Id of the peer
 *  \param tnl_addr_size    Number of IP address present
 *  \param p_tnl_add        IP address information to make SCTP association with the peer
 *  \param p_tac            TAC of eNB
 */
son_void_t
x2_init_enb_neighbor_info
(
    son_neighbor_enb_info_t *p_enb_info,
    son_global_enb_id_t     *p_enb_id,
    son_u8 tnl_addr_size,
    son_enb_tnl_address_t   *p_tnl_add,
    son_u8                  *p_tac
);

/*! \fn son_nbr_enb_ip_addr_hash_rec_t x2_insert_entry_in_nbr_enb_ip_addr_map(
 * SON_HASH *p_hash, son_nbr_enb_ip_addr_hash_rec_t * p_ip_addr_hash_rec, son_error_et *p_error_code)
 *  \brief Function to initialized the data to insert in eNB IP Address context
 *  \param p_hash               - Pointer to the neighbor eNB hash context
 *  \param p_ip_addr_hash_rec   - Pointer to structure for eNB IP record node
 *  \param p_error_code         - Error code
 */
son_nbr_enb_ip_addr_hash_rec_t *
x2_insert_entry_in_nbr_enb_ip_addr_map
(
    SON_HASH *p_hash,
    son_nbr_enb_ip_addr_hash_rec_t *p_ip_addr_hash_rec,
    son_error_et *p_error_code
);

/*! \fn son_global_enb_id_t * x2_get_enb_id_from_nbr_enb_ip_addr_map(
 * SON_HASH *p_hash, son_enb_tnl_address_t *p_nbr_ip_addr_id, son_error_et *p_error_code)
 *  \brief Function to get neighbor eNB Ids for given IP address
 *  \param p_hash               - Pointer to the neighbor eNB hash context
 *  \param p_nbr_ip_addr_id     - Pointer to the neighbor enb IP address
 *  \param p_error_code         - Error code
 */
son_global_enb_id_t *
x2_get_enb_id_from_nbr_enb_ip_addr_map
(
    SON_HASH *p_hash,
    son_enb_tnl_address_t  *p_nbr_ip_addr_id,
    son_error_et *p_error_code
);

/*! \fn son_return_et x2_delete_entry_from_nbr_enb_ip_addr_map(SON_HASH *p_hash, son_enb_tnl_address_t *p_nbr_ip_addr_id, son_error_et *p_error_code)
 *  \brief Function to remove selected neighbor eNB records from the nbr eNB IP address context table
 *  \param p_hash               - Pointer to the neighbor eNB hash context
 *  \param p_nbr_ip_addr_id     - Pointer to the neighbor enb id
 *  \param p_error_code         - Error code
 */
son_return_et
x2_delete_entry_from_nbr_enb_ip_addr_map
(
    SON_HASH *p_hash,
    son_enb_tnl_address_t  *p_nbr_ip_addr_id,
    son_error_et *p_error_code
);

/* SPR Fix Start */

/*! \fn son_ul32 x2_nbr_ecgi_enb_id_hash_func(const son_void_t *p_key)
 *  \brief This is function for generating hash key for the nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB IDs
 *  \param p_key    Pointer to the key of nbr_ecgi_enb_id_map
 */
son_ul32
x2_nbr_ecgi_enb_id_hash_func
(
    const son_void_t *p_key
);

/*! \fn const son_void_t *x2_nbr_ecgi_enb_id_key_off_nr_func(const SON_HASH_NODE *p_node)
 *  \brief This is a function used to find the data field of the tuple corresponding for the given hash node of nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB IDs
 *  \param p_node   Pointer to the anchor field of nbr_ecgi_enb_id_map hash tuple
 */
const son_void_t *
x2_nbr_ecgi_enb_id_key_off_nr_func
(
    const SON_HASH_NODE *p_node
);

/*! \fn son_nbr_ecgi_enb_id_hash_rec_t *x2_insert_entry_in_nbr_ecgi_enb_id_map(SON_HASH *p_hash, son_nbr_ecgi_enb_id_hash_rec_t *p_hash_rec)
 *  \brief This function is used to insert entry in nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB IDs
 *  \param p_hash       Pointer to nbr_ecgi_enb_id_map hash table
 *  \param p_hash_rec   Pointer to the nbr_ecgi_enb_id_map hash record
 */
son_nbr_ecgi_enb_id_hash_rec_t *
x2_insert_entry_in_nbr_ecgi_enb_id_map
(
    SON_HASH                        *p_hash,
    son_nbr_ecgi_enb_id_hash_rec_t  *p_hash_rec
);

/*! \fn son_return_et x2_init_nbr_ecgi_enb_id_map(SON_HASH *p_hash)
 *  \brief This function creates the map storing the mapping of neighbor cells ECGI and eNB Ids
 *  \param p_hash Pointer to nbr_ecgi_enb_id_map hash table
 */
son_return_et
x2_init_nbr_ecgi_enb_id_map
(
    SON_HASH *p_hash
);

/*! \fn son_void_t x2_clear_nbr_ecgi_enb_id_map(SON_HASH *p_hash)
 *  \brief This function clears entire nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB Ids
 *  \param p_hash Pointer to nbr_ecgi_enb_id_map hash table
 */
son_void_t
x2_clear_nbr_ecgi_enb_id_map
(
    SON_HASH *p_hash
);

/*! \fn son_nbr_ecgi_enb_id_hash_tuple_t *x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map(SON_HASH *p_hash)
 *  \brief Function to return the first neighhbor information from nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB Ids
 *  \param p_hash Pointer to nbr_ecgi_enb_id_map hash table
 */
son_nbr_ecgi_enb_id_hash_tuple_t *
x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map
(
    SON_HASH *p_hash
);

/*! \fn son_nbr_ecgi_enb_id_hash_tuple_t *x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map(SON_HASH *p_hash, SON_HASH_NODE *p_node)
 *  \brief Function to return the next neighhbor information from nbr_ecgi_enb_id_map storing the mapping of neighbor cells ECGI and eNB Ids
 *  \param p_hash Pointer to nbr_ecgi_enb_id_map hash table
 *  \param p_node Pointer to the nbr_ecgi_enb_id_map hash table node
 */
son_nbr_ecgi_enb_id_hash_tuple_t *
x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
);

/* SPR Fix End */

/* SPR 10331 Fix Starts */
/*! \fn son_return_et x2_delete_gu_grp_id_from_list(SON_LIST *p_list, x2ap_gu_group_id_t *p_gu_grp_id)
 *  \brief This function finds the GU Group Id in the pending GU Group Id list and if found deletes the entry
 *  \param p_list           Pointer to the pending GU Group Id list
 *  \param p_gu_grp_id      Pointer to the GU Group Id information
 */
son_return_et
x2_delete_gu_grp_id_from_list
(
    SON_LIST            *p_list,
    x2ap_gu_group_id_t  *p_gu_grp_id
);

/*! \fn x2_gu_group_id_node_t *x2_insert_gu_grp_id_in_list(SON_LIST *p_list, x2ap_gu_group_id_t *p_gu_grp_id)
 *  \brief This function inserts a node in the sent GU Group Id List
 *  \param p_list       Pointer to the list
 *  \param p_gu_grp_id  Pointer to the GU Group Id to be inserted
 */
x2_gu_group_id_node_t *
x2_insert_gu_grp_id_in_list
(
    SON_LIST            *p_list,
    x2ap_gu_group_id_t  *p_gu_grp_id
);

/*! \fn son_void_t x2_clear_ecu_gu_group_id_data(son_nbr_enb_gu_group_id_info_t *p_data)
 *  \brief Clean up the stored GU group Id data structure
 *  \param p_data   Pointer to stored eNB config update request's GU group Id data
 */
son_void_t
x2_clear_ecu_gu_group_id_data
(
    son_nbr_enb_gu_group_id_info_t  *p_data
);

/*! \fn x2_gu_group_id_node_t *x2_get_first_node_in_gu_group_id_list(x2_gu_group_id_list_t *p_list)
 *  \brief This function gets the first node of given GU Group Id list
 *  \param p_list   Pointer to pending GU Group Id add or delete list
 */
x2_gu_group_id_node_t *
x2_get_first_node_in_gu_group_id_list
(
    x2_gu_group_id_list_t *p_list
);

/*! \fn x2_gu_group_id_node_t *x2_get_next_node_in_gu_group_id_list(x2_gu_group_id_node_t *p_node)
 *  \brief This function gets the next node of given GU Group Id list
 *  \param p_node  Pointer to pending GU Group Id list node
 */
x2_gu_group_id_node_t *
x2_get_next_node_in_gu_group_id_list
(
    x2_gu_group_id_node_t *p_node
);

/* SPR 10331 Fix Ends */

#endif /* _SON_X2_ENB_TABLE_H_ */
