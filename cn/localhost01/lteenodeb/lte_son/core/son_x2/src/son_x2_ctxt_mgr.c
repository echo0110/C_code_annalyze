/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_x2_ctxt_mgr.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for managing X2 Module's
 *        Global context and eNB specific context.
 *
 *******************************************************************************
* Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Sep, 2012     Komal/Shekhar/Shilpi/Atul                  Initial
 *  May, 2014        Shilpi                      Coverity Fixes  
 *  June,2014     Shilpi                         SPR-11877 Fix
*   July, 2014    Shilpi                         SPR 10331 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_utils.h>
#include <son_anr_ctxt_mgr.h>
#include <son_anr_table.h>
#include <son_x2_ctxt_mgr.h>
#include <son_enb_table.h>
#include <son_defines.h>
#include <rrc_x2apCommon_composer.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_x2_facility_name;
extern x2_global_context_t g_x2_gb_ctxt;
extern anr_global_t  g_anr_context;
extern const QWAIT  son_waitstruct;

/* SON X2 FSM state names */
const son_s8 *SON_X2_FSM_STATES_NAMES[] =
{
    (const son_s8 *)"SONX2_STATE_INIT",
    (const son_s8 *)"SONX2_STATE_ACTIVE",
    (const son_s8 *)"SONX2_STATE_CONNECTING",
    (const son_s8 *)"SONX2_STATE_CONNECTED",
    (const son_s8 *)"SONX2_STATE_SHUTDOWN",
    (const son_s8 *)"SONX2_STATE_UNDEFINED"
};

/* SON X2 ENB FSM state names */
const son_s8 *SON_X2_ENB_FSM_STATES_NAMES[] =
{
    (const son_s8 *)"SONX2_ENB_STATE_DISCONNECTED",
    (const son_s8 *)"SONX2_ENB_STATE_CONNECTING",
    (const son_s8 *)"SONX2_ENB_STATE_CONNECTED",
    (const son_s8 *)"SONX2_ENB_STATE_DISCONNECTING",
    (const son_s8 *)"SONX2_ENB_STATE_RESETTING",
    (const son_s8 *)"SONX2_ENB_STATE_UNDEFINED"
};

/* SON X2 Module Event  names */
const son_s8 *SON_X2_FSM_EVENT_NAMES[] =
{
    (const son_s8 *)"X2_SMIF_EVENT_INIT_CONFIG_REQ",
    (const son_s8 *)"X2_SMIF_EVENT_SET_LOG_LEVEL_CMD",
    (const son_s8 *)"X2_SMIF_EVENT_SHUTDOWN_REQ",
    (const son_s8 *)"X2_SONANR_EVENT_ADD_ENB_REQ",
    (const son_s8 *)"X2_SONANR_EVENT_DELETE_ENB_REQ",
    (const son_s8 *)"X2_SONANR_EVENT_ENB_UPDATE_IND",
    (const son_s8 *)"X2_SONES_EVENT_CELL_SWITCH_ON_OFF_IND",
    (const son_s8 *)"X2_SONES_EVENT_CELL_ACTIVATION_RES",
    (const son_s8 *)"X2_RRC_EVENT_ADD_ENB_RES",
    (const son_s8 *)"X2_RRC_EVENT_LINK_UP_IND",
    (const son_s8 *)"X2_RRC_EVENT_LINK_DOWN_IND",
    (const son_s8 *)"X2_RRC_EVENT_DELETE_ENB_RES",
    (const son_s8 *)"X2_RRC_EVENT_CELL_ACTIVATION_REQ",
    (const son_s8 *)"X2_RRC_EVENT_ENB_CONFIG_UPDATE_RES",
    (const son_s8 *)"X2_RRC_EVENT_ENB_CONFIG_UPDATE_IND",
    (const son_s8 *)"X2_RRC_EVENT_ENB_RESET_RES",
    (const son_s8 *)"X2_RRC_EVENT_HO_REPORT_IND",
    (const son_s8 *)"X2_RRC_EVENT_MOBILITY_SETTING_CHANGE_REQ",
    (const son_s8 *)"X2_RRC_EVENT_MOBILITY_CHANGE_RESP",
    (const son_s8 *)"X2_RRC_EVENT_MLB_MOBILTY_CHANGE_REQ",
    (const son_s8 *)"X2_RRC_EVENT_MLB_MOBILTY_CHANGE_ACK",
    (const son_s8 *)"X2_RRC_EVENT_MLB_MOBILTY_CHANGE_FAILURE",
    /* SPR-10331 Fix Starts */
    (const son_s8 *)"X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ",
    /* SPR-10331 Fix Ends */
    /* SPR_19619 start */
    (const son_s8 *)"X2_SMIF_EVENT_PEER_CELL_ACTIVATION_REQ",
    /* SPR_19619 stop */
    (const son_s8 *)"X2_EVENT_UNDEFINED"
};

/* SON X2 Module ENB FSM Event  names */
const son_s8 *SON_X2_ENB_FSM_EVENT_NAMES[] =
{
    (const son_s8 *)"X2_ENB_EVENT_SONANR_ADD_ENB_REQ",
    (const son_s8 *)"X2_ENB_EVENT_RRC_ADD_ENB_RES",
    (const son_s8 *)"X2_ENB_EVENT_RRC_LINK_UP_IND",
    (const son_s8 *)"X2_ENB_EVENT_RRC_LINK_DOWN_IND",
    (const son_s8 *)"X2_ENB_EVENT_SONANR_DELETE_ENB_REQ",
    (const son_s8 *)"X2_ENB_EVENT_RRC_DELETE_ENB_RES",
    (const son_s8 *)"X2_ENB_EVENT_MSG_ENB_CONFIG_UPDATE",
    (const son_s8 *)"X2_ENB_EVENT_RRC_ENB_CONFIG_UPDATE_RES",
    (const son_s8 *)"X2_ENB_EVENT_RRC_RESET_RES",
    (const son_s8 *)"X2_ENB_EVENT_SON_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"X2_ENB_EVENT_SON_MOBILITY_CHANGE_ACK",
    (const son_s8 *)"X2_ENB_EVENT_SON_MOBILITY_CHANGE_FAILURE",
    /* SPR-10331 Fix Starts */
    (const son_s8 *)"X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ",
    /* SPR-10331 Fix Ends */
    /* SPR_19619 start */
    (const son_s8 *)"X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ",
    /* SPR_19619 stop */
    (const son_s8 *)"X2_ENB_EVENT_UNDEFINED"
};

/*******************************************************************************
 * Function Name  : x2_get_nbr_enb_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the eNB context pool
 * Description    : This function gets pointer to the eNB context pool 
 ******************************************************************************/
x2_hash_pool_t *
x2_get_nbr_enb_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
        SON_UT_TRACE_ENTER();
        SON_UT_TRACE_EXIT();
        return &g_x2_gb_ctxt.nbr_enb_hash_rec_pool;
}

/*******************************************************************************
 * Function Name  : x2_get_nbr_enb_ip_addr_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the eNB IP address context pool
 * Description    : This function gets pointer to the eNB IP address context pool 
 ******************************************************************************/
x2_hash_pool_t *
x2_get_nbr_enb_ip_addr_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
        SON_UT_TRACE_ENTER();
        SON_UT_TRACE_EXIT();
        return &g_x2_gb_ctxt.nbr_enb_ip_addr_hash_rec_pool;
}

/* SPR Fix Start */

/*******************************************************************************
 * Function Name  : x2_get_nbr_ecgi_enb_id_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the Neighbor ECGI and eNB Id map context pool
 * Description    : This function gets pointer to the Neighbor ECGI and eNB 
 *                  Id map context pool
 ******************************************************************************/
x2_hash_pool_t *
x2_get_nbr_ecgi_enb_id_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
        SON_UT_TRACE_ENTER();
        SON_UT_TRACE_EXIT();
        return &g_x2_gb_ctxt.nbr_ecgi_enb_id_hash_rec_pool;
}

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_get_enb_hash_node_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the eNB table pool
 * Description    : This function gets pointer to the eNB table pool
 ****************************************************************************/
x2_hash_pool_t *
x2_get_enb_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return &g_x2_gb_ctxt.hash_node_pool;
}

/******************************************************************************
 * Function Name  : x2_print_plmn_id
 * Inputs         : p_plmn_id    Pointer to the plmn Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the value of Plmn Id
 ****************************************************************************/
son_void_t
x2_print_plmn_id
(
    son_cell_plmn_info_t *p_plmn_id
)
{
    son_u8 index = 0;

    SON_UT_TRACE_ENTER();
    
    if (SON_PNULL == p_plmn_id)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "print PLMN Id, p_plmn_id is NULL");
    }
    else
    {
        for (index = 0; index < SON_MCC_OCTET_SIZE; index++)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                    "mcc[%u]: %u", 
                    index,
                    (son_u16)p_plmn_id->mcc[index]);
        }

        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Number of mnc digits: %u", 
                (son_u16)p_plmn_id->num_mnc_digit);
        for (index = 0; index < p_plmn_id->num_mnc_digit; index++)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                    "mnc[%u]: %u", 
                    index,
                    (son_u16)p_plmn_id->mnc[index]);
        }
    }

    SON_UT_TRACE_EXIT();
} /* x2_print_plmn_id */

/******************************************************************************
 * Function Name  : x2_print_enb_id
 * Inputs         : p_enb_id    Pointer to the eNB Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the value of eNB Identifier
 ****************************************************************************/
son_void_t
x2_print_enb_id
(
    son_global_enb_id_t *p_enb_id
)
{
    son_u8 index = 0;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_id)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "print eNB Id, p_enb_id is NULL");
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                "eNB Type: %u", p_enb_id->enb_type);
        x2_print_plmn_id(&p_enb_id->plmn_id);
        for (index = 0; index < SON_ENB_ID_OCTET_SIZE; index++)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                    "eNB Id 0x%x", 
                    (son_u16)p_enb_id->enb_id[index]);
        }
    }

    SON_UT_TRACE_EXIT();
} /* x2_print_enb_id */

/******************************************************************************
 * Function Name  : print_tnl_address_list
 * Inputs         : list_size       Size of the TNL address list
 *                  p_tnl_add_list  Pointer to the TNL address list
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the TNL addresses for a peer eNB
 ****************************************************************************/
son_void_t
print_tnl_address_list
(
    son_u8                  list_size,
    son_enb_tnl_address_t   *p_tnl_add_list
)
{
    son_u8 index = 0, index1 = 0; 

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_tnl_add_list)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "print TNL address list, p_tnl_add_list is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
            "TNL address list size: %u",
            (son_u16)list_size);
    for (index = 0; index < list_size; index++)
    {
        if (IPV4_ADD_PRESENT & p_tnl_add_list[index].bitmask)
        {
            for (index1 = 0; index1 < SON_IPV4_SIZE; index1++)
            {
                SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                        "IPv4 addr: %u",
                        (son_u16)(p_tnl_add_list[index].ipv4_add[index1]));
            }
        }
        else if (IPV6_ADD_PRESENT & p_tnl_add_list[index].bitmask)
        {
            for (index1 = 0; index1 < SON_IPV6_SIZE; index1++)
            {
                SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                        "IPv6 addr %u",
                        (son_u16)(p_tnl_add_list[index].ipv6_add[index1]));
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* print_tnl_address_list */

/******************************************************************************
 * Function Name  : x2_print_enb_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the peer eNBs hash table data
 ****************************************************************************/
son_void_t
x2_print_enb_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_nbr_enb_hash_tuple_t *p_enb_tuple = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
        "\n--- Neighbor eNB hash table contents ---");
    /* Fetching the first record from neighbor enb NRT */
    p_enb_tuple = x2_get_first_nbr_enb_from_nrt(
        &g_x2_gb_ctxt.nbr_enb_context_table);

    /* Checking if the neighbor enb NRT is empty */
    while (SON_PNULL != p_enb_tuple)
    {
        x2_print_enb_id(&p_enb_tuple->data.enb_id);

        print_tnl_address_list(
                p_enb_tuple->data.enb_tnl_address_list_size,
                p_enb_tuple->data.enb_tnl_address_list);

        /* SPR 11340 Fix Starts */
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                "X2 Status: %u NR Status: %u Previous eNB FSM state: %u Current eNB FSM state: %u"
                " Expiry time for Time to wait timer: %lu sec, %lu usec"
                " is_enb_config_update_res_awaited Flag for eNB config update "
                " response message: %u"
                "Delete eNB request triggered due to reset response"
                " expiry/failure: %u"
                " Number of cells in pending eNB config update list: %u",
                p_enb_tuple->data.x2_status,
                p_enb_tuple->data.nr_status,
                p_enb_tuple->data.enb_prev_fsm_state,
                p_enb_tuple->data.enb_curr_fsm_state,
                p_enb_tuple->data.time_d_wait_expiry.s,
                p_enb_tuple->data.time_d_wait_expiry.us,
                p_enb_tuple->data.is_enb_config_update_res_awaited,
                p_enb_tuple->data.is_self_del_enb_req_triggered,
                list_count(&p_enb_tuple->data.enb_config_cell_info));

        /* SPR 11340 Fix Ends */

            p_enb_tuple =  x2_get_next_nbr_enb_from_nrt(
                &g_x2_gb_ctxt.nbr_enb_context_table,
                &p_enb_tuple->anchor);
    }

    SON_UT_TRACE_EXIT();
} /* x2_print_enb_table */

/*****************************************************************************
 * Function Name  : x2_print_global_context
 * Inputs         : son_method_name         Calling function name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the X2 global context
 ******************************************************************************/
static son_void_t
x2_print_global_context
(
    const son_8 *son_method_name
)
{
    SON_UT_TRACE_ENTER();
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
            "%s:\n=== X2 Global Context Structure:===\n"
            "Current FSM State: %s "
            "X2 Log Enable: %u"
            "Number of peer eNB configured: %u",
            son_method_name,
            SON_X2_FSM_STATES_NAMES[g_x2_gb_ctxt.x2_global_fsm_state], 
            X2_GET_LOG_MODE,
            hash_count(&g_x2_gb_ctxt.nbr_enb_context_table));
    x2_print_enb_table();
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function initialzes the X2 global context
 ******************************************************************************/
son_return_et
x2_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_return_et ret_val = SON_SUCCESS;

    SON_UT_TRACE_ENTER();
    g_x2_gb_ctxt.x2_global_fsm_state = X2_STATE_INIT;
    ret_val = x2_init_nbr_enb_db();
    if (SON_SUCCESS == ret_val)
    {
        ret_val = x2_init_nbr_enb_table(&g_x2_gb_ctxt.nbr_enb_context_table);
    }

    if (SON_SUCCESS == ret_val)
    {
        ret_val = x2_init_nbr_enb_ip_addr_map(
                &g_x2_gb_ctxt.nbr_enb_ip_addr_map);    
    }

    /* SPR Fix Start */
    if (SON_SUCCESS == ret_val)
    {
        ret_val = x2_init_nbr_ecgi_enb_id_map(
                &g_x2_gb_ctxt.nbr_ecgi_enb_id_map);    
    }

/* SPR Fix End */

    SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);
    x2_print_global_context("x2_init_global_context");

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_init_global_context */

/*****************************************************************************
 * Function Name  : x2_clear_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function does the clean up of X2 global context
 ******************************************************************************/
son_void_t
x2_clear_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    x2_clear_enb_ip_addr_table(&g_x2_gb_ctxt.nbr_enb_ip_addr_map);    
/* SPR Fix Start */    
    x2_clear_nbr_ecgi_enb_id_map(&g_x2_gb_ctxt.nbr_ecgi_enb_id_map);
    /* SPR Fix End */    
    /* Coverity CID-48280 Fix Starts */
    ANR_FREE_TABLE(g_x2_gb_ctxt.nbr_ecgi_enb_id_map.table);
    /* Coverity CID-48280 Fix Ends */
    x2_clear_enb_table(&g_x2_gb_ctxt.nbr_enb_context_table);
    x2_clear_nbr_enb_db();
    SON_UT_TRACE_EXIT();
}

/*******************************************************************************
 * Function Name  : x2_get_enb_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the context pool
 * Description    : This function gets pointer to the context pool
 ******************************************************************************/
x2_hash_pool_t *
x2_get_enb_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
       SON_UT_TRACE_ENTER();
       SON_UT_TRACE_EXIT();
       return &g_x2_gb_ctxt.nbr_enb_hash_rec_pool;
}

/*******************************************************************************
 * Function Name  : x2_get_enb_ip_addr_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the context pool
 * Description    : This function gets pointer to the context pool
 ******************************************************************************/
x2_hash_pool_t *
x2_get_enb_ip_addr_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
       SON_UT_TRACE_ENTER();
       SON_UT_TRACE_EXIT();
       return &g_x2_gb_ctxt.nbr_enb_ip_addr_hash_rec_pool;
}

/******************************************************************************
 * Function Name  : x2_get_hash_node_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the table pool
 * Description    : This function gets pointer to the table pool
 ****************************************************************************/
x2_hash_pool_t *
x2_get_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return &g_x2_gb_ctxt.hash_node_pool;
}

/******************************************************************************
 * Function Name  : x2_get_enb_nrt
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to the eNB hash table
 * Description    : This function gets pointer to the X2 eNB hash table
 ***************************************************************************/
SON_HASH *
x2_get_enb_nrt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return &g_x2_gb_ctxt.nbr_enb_context_table;
}

/*****************************************************************************
 * Function Name  : x2_key_of_enb_config_cell_list
 * Inputs         : p_node - anchor field of the enb config cell list record
 * Outputs        : None
 * Returns        : pointer to enb cell data as void pointer
 * Description    : This function determines the key in ENB config cell list
 ******************************************************************************/
const son_void_t *
x2_key_of_enb_config_cell_list
(
 const SON_LIST_NODE *p_node
 )
{
    enb_config_cell_data_t *p_data = SON_PNULL;

    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_node);

    p_data = YMEMBEROF(enb_config_cell_data_t, enb_cell_node, p_node);

    return (son_void_t *)(&p_data->enb_cell_data.cell_id);
}

/* SPR-10331 Fix Starts */
/* Lines Deleted */
/* SPR-10331 Fix Ends */

/*****************************************************************************
 * Function Name  : x2_modify_cell_in_enb_cfg_pending_list
 * Inputs         : p_enb_config_cell_list : ENB config cell list
 *                : cell_id : Intra rat global cell id
 *                : last_op : Last operation performed on NRT 
 *                : deac_flag : Deactivation flag 
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : This function modifies cell in ENB Config Pending List 
 *****************************************************************************/
son_bool_et
x2_modify_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t  *cell_id,
    son_nrt_operation_performed_et  last_op,
    x2_deactivation_flag_et         deac_flag
 )
{
    enb_config_cell_data_t  *p_list_node = SON_PNULL;
    son_bool_et             ret_val      = SON_FALSE;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_enb_config_cell_list && SON_PNULL != cell_id)
    {
        /* Getting the node from enb config pending list */
        p_list_node = x2_find_cell_in_enb_cfg_pending_list(
                        p_enb_config_cell_list,
                        cell_id);

        if (SON_PNULL == p_list_node)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_INFO,
                    "Cell 0x%x is not present in eNB "
                    "Config Pending list",
                    son_convert_char_cell_id_to_int_cell_id(cell_id->cell_identity));
        }
        else
        {
            p_list_node->enb_cell_data.last_op = last_op;
    
            p_list_node->enb_cell_data.deacivation_flag = deac_flag;
            
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_BRIEF,
                    "Modified Cell 0x%x in enb "
                    "config Pending list",
                    son_convert_char_cell_id_to_int_cell_id(cell_id->cell_identity));
                
            ret_val = SON_TRUE;
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Invalid input "
                "parameters [p_enb_config_cell_list or cell_id is NULL]");
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_modify_cell_in_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_delete_cell_in_enb_cfg_pending_list
 * Inputs         : p_enb_config_cell_list : ENB config cell list
 *                : cell_id : Intra rat global cell id
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : This function deletes cell in ENB Config Pending List 
 *****************************************************************************/
son_bool_et
x2_delete_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t  *cell_id
)
{
    enb_config_cell_data_t  *p_list_node = SON_PNULL;
    son_bool_et             ret_val      = SON_FALSE;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_enb_config_cell_list && SON_PNULL != cell_id)
    {
        /* Getting the node from enb config pending list */
        p_list_node = x2_find_cell_in_enb_cfg_pending_list(
                                p_enb_config_cell_list,
                                cell_id);

        if (SON_PNULL == p_list_node)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_INFO,
                    "Cell 0x%x is not present in eNB Config "
                    "Pending list",
                    son_convert_char_cell_id_to_int_cell_id(cell_id->cell_identity));
        }
        else
        {
            list_delete_node(p_enb_config_cell_list,
                             &p_list_node->enb_cell_node);
              
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_BRIEF,
                    "Deleted Cell 0x%x "
                    "from enb config Pending list",
                    son_convert_char_cell_id_to_int_cell_id(cell_id->cell_identity));
                
            ret_val = SON_TRUE;
            son_mem_free(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Invalid input "
                "parameters [p_enb_config_cell_list or cell_id is NULL]");
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_delete_cell_in_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_clear_enb_cfg_pending_list
 * Inputs         : p_enb_config_cell_list : ENB config cell list
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : This function clears ENB Config Pending List 
 *****************************************************************************/
son_bool_et
x2_clear_enb_cfg_pending_list
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list
)
{
    enb_config_cell_data_t  *p_list_node = SON_PNULL;
    SON_LIST_NODE           *p_node      = SON_PNULL;
    son_bool_et             ret_val      = SON_FALSE;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_enb_config_cell_list)
    {
        p_node = list_pop_node(p_enb_config_cell_list);

        while (SON_PNULL != p_node)
        {
            p_list_node = YMEMBEROF(enb_config_cell_data_t,
                                    enb_cell_node, p_node);

            son_mem_free(p_list_node);
            
            p_node = list_pop_node(p_enb_config_cell_list);
        }
                
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_DETAILED,
                "Cleared enb config Pending list");
                
        ret_val = SON_TRUE;
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Invalid input "
                "parameters [p_enb_config_cell_list is NULL]");
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_clear_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_find_cell_in_enb_cfg_pending_list
 * Inputs         : p_enb_config_cell_list : ENB config cell list
 *                : cell_id : Intra rat global cell id
 * Outputs        : None
 * Returns        : enb_config_cell_data_t*
 * Description    : This function finds a cell in ENB Config Pending List 
 *****************************************************************************/
enb_config_cell_data_t *
x2_find_cell_in_enb_cfg_pending_list
(
    son_enb_config_cell_list_t      *p_enb_config_cell_list,
    son_intra_rat_global_cell_id_t  *cell_id
)
{
    enb_config_cell_data_t  *p_list_node = SON_PNULL;
    SON_LIST_NODE           *p_node      = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_enb_config_cell_list && SON_PNULL != cell_id)
    {
        p_node = list_find_node(p_enb_config_cell_list,
                                (const son_void_t *)cell_id,
                                x2_key_of_enb_config_cell_list,
                                son_compare_cell_id);

        if (SON_PNULL == p_node)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_INFO,
                    "Cell 0x%x is not present in ENB Config "
                    "Pending list",
                    son_convert_char_cell_id_to_int_cell_id(cell_id->cell_identity));
            
            SON_UT_TRACE_EXIT();
            return SON_PNULL;
        }
        else
        {
            p_list_node = YMEMBEROF(enb_config_cell_data_t,
                                    enb_cell_node, p_node);

            SON_UT_TRACE_EXIT();
            return p_list_node;
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Invalid input "
                "parameters [p_enb_config_cell_list or cell_id is NULL]");

        SON_UT_TRACE_EXIT();
        return SON_PNULL;
    }
} /* x2_find_cell_in_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_convert_son_plmn_id_to_l3_plmn_id
 * Inputs         : p_son_plmn_id - Pointer to the PLMN value defined by SON
 * Outputs        : p_l3_plmn_id  - Pointer to the PLMN value defined by L3 X2AP
 * Returns        : None
 * Description    : This is the function which is used to convert PLMN value
 *                  stored by SON to the PLMN value required by L3 X2AP
 ****************************************************************************/
son_void_t
x2_convert_son_plmn_id_to_l3_plmn_id
(
    son_cell_plmn_info_t *p_son_plmn_id,
    x2ap_plmn_identity_t *p_l3_plmn_id
)
{
    /* As per TS 36.423 :
     * PLMN Id consists of 3 bit string where
     * - digits 0 to 9, encoded 0000 to 1001,
     * - 1111 used as filler digit, two digits per octet,
     * - bits 4 to 1 of octet n encoding digit 2n-1
     * - bits 8 to 5 of octet n encoding digit 2n
     *
     * -The Selected PLMN identity consists of 3 digits from MCC followed by either
     *  -a filler digit plus 2 digits from MNC (in case of 2 digit MNC) or
     *  -3 digits from MNC (in case of a 3 digit MNC). */
   
        SON_UT_TRACE_ENTER();

        p_l3_plmn_id->plmn_id[0] = p_son_plmn_id->mcc[0] |
                (p_son_plmn_id->mcc[1] << 4);

        if (2 == p_son_plmn_id->num_mnc_digit)
        {
                /* Add the filler digit as only 2 mnc digits are there */
                p_l3_plmn_id->plmn_id[1] = (p_son_plmn_id->mcc[2]) | 0xF0;
                p_l3_plmn_id->plmn_id[2] = p_son_plmn_id->mnc[0] |
                        (p_son_plmn_id->mnc[1] << 4);
        }
        else
        {
                p_l3_plmn_id->plmn_id[1] = (p_son_plmn_id->mcc[2]) |
                                   (p_son_plmn_id->mnc[0] << 4);
                p_l3_plmn_id->plmn_id[2] = (p_son_plmn_id->mnc[2] << 4) |
                        p_son_plmn_id->mnc[1];
        }

        SON_UT_TRACE_EXIT();
} /* x2_convert_son_plmn_id_to_l3_plmn_id */

/*****************************************************************************
 * Function Name  : x2_convert_son_ecgi_to_l3_ecgi
 * Inputs         : p_son_cgi - Pointer to the CGI value defined by SON
 * Outputs        : p_x2_cgi - Pointer to the CGI value defined by L3 X2AP
 * Returns        : None
 * Description    : This is the function which is used to convert CGI value
 *                  stored by SON to the CGI value required by L3 X2AP
 ****************************************************************************/
son_void_t
x2_convert_son_ecgi_to_l3_ecgi
(
    son_intra_rat_global_cell_id_t  *p_son_cgi, /* Input parameter */
    x2ap_ecgi_t                     *p_x2_cgi   /* Output parameter */
)
{
    SON_UT_TRACE_ENTER();
    
    x2_convert_son_plmn_id_to_l3_plmn_id(&p_son_cgi->plmn_id, 
        &p_x2_cgi->plmn_identity); 
    SON_MEMCPY(p_x2_cgi->eutran_cell_id, 
        p_son_cgi->cell_identity,
        CELL_ID_OCTET_SIZE);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2_populate_srv_cell_nbrs_info
 * Inputs         : p_intra_rat_nrt - Pointer to the intra RAT neighbors table
 *                  of a serving cell
 * Outputs        : p_nbr_info - Pointer to the serving cell's neighbors 
 *                  information
 * Returns        : None
 * Description    : This is the function which is used to find and populate 
 *                  the neighbor information of a particular serving cell
 ****************************************************************************/
son_void_t
x2_populate_srv_cell_nbrs_info
(
    SON_HASH                *p_intra_rat_nrt,
    x2ap_nbour_cell_info_t  *p_nbr_info
)   
{
    anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    if (SON_PNULL == p_intra_rat_nrt ||
            SON_PNULL == p_nbr_info)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "populate serving cell's neighbor information, "
                "p_intra_rat_nrt or p_nbr_info is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }
    
    p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(p_intra_rat_nrt);

    if (SON_PNULL ==  p_intra_tpl)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
                " First NR record returned from intra rat hash table is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    while (SON_PNULL != p_intra_tpl)
    {
        if (SON_TRUE == anr_validate_eutran_neighbor_for_l3(&p_intra_tpl->data))
        {
            p_nbr_info->neighbour_info_arr[p_nbr_info->num_neighbour_cell].
                earfcn.earfcn =
            p_intra_tpl->data.neighbor_info.earfcn;
        p_nbr_info->neighbour_info_arr[p_nbr_info->num_neighbour_cell].pci = 
            p_intra_tpl->data.neighbor_info.pci;
        x2_convert_son_ecgi_to_l3_ecgi(
            &p_intra_tpl->data.neighbor_info.nbr_cgi, 
                    &p_nbr_info->neighbour_info_arr[p_nbr_info->
                    num_neighbour_cell].ecgi);
        p_nbr_info->num_neighbour_cell++;
        }
        p_intra_tpl =  anr_get_next_nr_from_intra_rat_hash_table(
                            p_intra_rat_nrt, &p_intra_tpl->anchor);
     }

     SON_UT_TRACE_EXIT();
} /* x2_populate_srv_cell_nbrs_info */

/*****************************************************************************
 * Function Name  : x2_populate_choice_eutra_mode
 * Inputs           p_in_choice_eutra_mode  - Son eutra mode information 
 *                  p_out_choice_eutra_mode - X2 eutra mode information
 * Returns        : None
 * Description    : This is the function which is used to find and populate
 *                  the neighbor information of a particular serving cell
 ****************************************************************************/
static son_void_t
x2_populate_choice_eutra_mode
(
    son_choice_eutra_mode_t     *p_in_choice_eutra_mode,
    x2ap_choice_eutra_mode_t    *p_out_choice_eutra_mode
)
{
    SON_UT_TRACE_ENTER();
    
    if (SON_PNULL == p_in_choice_eutra_mode ||
            SON_PNULL == p_out_choice_eutra_mode)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "populate choice eutra mode field value, "
                "p_in_choice_eutra_mode or "
                "p_out_choice_eutra_mode is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    if (SON_EUTRA_MODE_FDD & p_in_choice_eutra_mode->bitmask)
    {
        p_out_choice_eutra_mode->bitmask |= X2AP_EUTRA_MODE_FDD;
        
        p_out_choice_eutra_mode->fdd_info.ul_earfcn.earfcn = 
            p_in_choice_eutra_mode->fdd_info.ul_earfcn;
        p_out_choice_eutra_mode->fdd_info.dl_earfcn.earfcn =
            p_in_choice_eutra_mode->fdd_info.dl_earfcn;
        p_out_choice_eutra_mode->fdd_info.ul_trnsmission_bdwidth = 
            p_in_choice_eutra_mode->fdd_info.ul_trnsmission_bdwidth;
        p_out_choice_eutra_mode->fdd_info.dl_trnsmission_bdwidth = 
            p_in_choice_eutra_mode->fdd_info.dl_trnsmission_bdwidth;
    }
    else if (SON_EUTRA_MODE_TDD & p_in_choice_eutra_mode->bitmask)
    {
        p_out_choice_eutra_mode->bitmask |= X2AP_EUTRA_MODE_TDD;
        p_out_choice_eutra_mode->tdd_info.earfcn.earfcn =
            p_in_choice_eutra_mode->tdd_info.earfcn;
        p_out_choice_eutra_mode->tdd_info.trnsmission_bdwidth =
            p_in_choice_eutra_mode->tdd_info.trnsmission_bdwidth;
        p_out_choice_eutra_mode->tdd_info.subframe_assn = 
            p_in_choice_eutra_mode->tdd_info.subframe_assn;
        p_out_choice_eutra_mode->tdd_info.special_subframe_info.
            special_subframe_patterns 
            = p_in_choice_eutra_mode->tdd_info.special_subframe_info.
            special_subframe_patterns;
        p_out_choice_eutra_mode->tdd_info.special_subframe_info.dl_cp =
            p_in_choice_eutra_mode->tdd_info.
            special_subframe_info.dl_cp;
        p_out_choice_eutra_mode->tdd_info.special_subframe_info.ul_cp =
            p_in_choice_eutra_mode->tdd_info.
            special_subframe_info.ul_cp;
    }

    SON_UT_TRACE_EXIT();
} /* x2_populate_choice_eutra_mode */

/*****************************************************************************
 * Function Name  : x2_populate_prach_config_info
 * Inputs         : p_input_prach_config   - Pointer to rach configuration
 *                                           data of serving cell
 *                : is_fdd_mode            - Is mode FDD or TDD of the
                                             serving cell
 * Outputs        :  p_cell_info           - Pointer to rach configuration
 *                                           data to L3
 * Returns        : None
 * Description    : This is the function which is used to populate 
 *                  the prach configuration of the serving cell
 ****************************************************************************/
static son_void_t
x2_populate_prach_config_info
(
 son_prach_configuration_t *p_input_prach_config,
 x2ap_served_cell_info_arr_t     *p_cell_info,
 son_bool_et               is_fdd_mode
 )
{
    SON_UT_TRACE_ENTER();
    x2ap_prach_configuration_info_t *p_output_prach_config = SON_PNULL;
    if(SON_PNULL == p_input_prach_config ||SON_PNULL == p_cell_info)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "populate prach config field value, "
                "p_input_prach_config or "
                "p_cell_info is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    if(SON_FALSE == is_fdd_mode && !
            (p_input_prach_config->bitmask & SON_PRACH_CONFIG_INDEX_PRESENT))
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_WARNING,
                "Could not "
                "populate prach config field value, "
                "as serving cell is TDD but prach configuration index "
                "is not available ");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_output_prach_config = &p_cell_info->cell_info.prach_config;

    p_output_prach_config->root_sequence_index = p_input_prach_config->root_sequence_index;
    p_output_prach_config->zero_correlation_zone_config = p_input_prach_config->zero_correlation_zone_config;
    p_output_prach_config->high_speed_flag = p_input_prach_config->high_speed_flag;
    p_output_prach_config->prach_frequency_offset = p_input_prach_config->prach_frequency_offset;
    /*If the mode is TDD */
    if((SON_FALSE == is_fdd_mode) &&  (p_input_prach_config->bitmask & SON_PRACH_CONFIG_INDEX_PRESENT))
    {
        p_output_prach_config->prach_config_index = p_input_prach_config->prach_configuration_index;
        p_output_prach_config->bitmask |= X2AP_PRACH_CONFIG_INDEX_PRESENT;
    }

    p_cell_info->cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
    SON_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : x2_populate_srv_cell_info
 * Inputs         : p_cell_id   - Pointer to intra rat global cell id
 *                  p_cell_info - Pointer to x2 served cells
 * Outputs        : 
 * Returns        : None
 * Description    : This is the function which is used to find and populate 
 *                  the serving cells information of the serving eNB.
 ****************************************************************************/
son_void_t
x2_populate_srv_cell_info
(
    son_intra_rat_global_cell_id_t  *p_cell_id,
    x2ap_served_cell_info_arr_t     *p_cell_info
)
{
    anr_cell_context_t      *p_cell_ctxt        = SON_PNULL;
    son_bool_et              is_fdd_mode        = SON_TRUE;
    son_u8 index = 0;

    SON_UT_TRACE_ENTER();
    
    if (SON_PNULL == p_cell_id || SON_PNULL == p_cell_info)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "populate serving cell information, "
                "p_cell_id or p_cell_info is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Search for the given cell Id in the cell contexts 
     *  maintained by ANR */
    p_cell_ctxt = anr_get_cell_context_from_global_context(p_cell_id);

    if (SON_PNULL != p_cell_ctxt) 
    {
        SON_MEMSET(p_cell_info, 0, sizeof(x2ap_served_cell_info_arr_t));
        p_cell_info->cell_info.pci = 
            p_cell_ctxt->pci_value;

        /*SPR 21554 Start*/       
        p_cell_info->cell_info.cell_id=p_cell_ctxt->cell_index;       
        /*SPR 21554 End*/       
        SON_MEMCPY(p_cell_info->cell_info.tac, 
                p_cell_ctxt->tac,
                MAX_TAC_BYTES);    

        p_cell_info->cell_info.bcast_plmn_info.num_bcast_plmn_id = 
            p_cell_ctxt->num_plmn_id;
        for (index = 0; 
                index < p_cell_ctxt->num_plmn_id;
                index++)
        {
            x2_convert_son_plmn_id_to_l3_plmn_id(
                    &p_cell_ctxt->plmn_id[index],
                    &p_cell_info->cell_info.bcast_plmn_info.
                    plmn_identity[index]);    
        }

        x2_populate_choice_eutra_mode(&p_cell_ctxt->choice_eutra_mode,
                &p_cell_info->cell_info.choice_eutra_mode.choice_eutra_mode);
        /*Determine if the mode is FDD or TDD*/
        if(p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_TDD)
        {
            is_fdd_mode = SON_FALSE;
        }

        if(SON_TRUE == p_cell_ctxt->is_prach_configuration_available)
        {
            x2_populate_prach_config_info(&p_cell_ctxt->prach_configuration,
                    p_cell_info,is_fdd_mode);
        }
        else
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_ERROR,
                    "Could not "
                    "populate Prach Configuration as it is not "
                    "present in the serving cell ");
        }

        if(SON_TRUE == p_cell_ctxt->is_csg_id_available)
        {
            SON_MEMCPY(p_cell_info->cell_info.csg_identity,
                    p_cell_ctxt->csg_identity,
                    sizeof(p_cell_info->cell_info.csg_identity));
            p_cell_info->cell_info.bitmask |= X2AP_CSG_ID_INFO_PRESENT; 
        }
#ifdef LTE_EMBMS_SUPPORTED
        if(SON_TRUE == p_cell_ctxt->is_srv_mbms_available)
        {
            p_cell_info->cell_info.mbsfn_subframe_info.num_mbsfn =
            p_cell_ctxt->mbms_config.num_mbsfn;

            for (index = 0; index < p_cell_info->cell_info.mbsfn_subframe_info.num_mbsfn;
                index ++)
            {
                p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_period =
                    p_cell_ctxt->mbms_config.mbsfn_info[index].radioframe_alloc_period;
                p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_offset =
                    p_cell_ctxt->mbms_config.mbsfn_info[index].radioframe_alloc_offset;
                p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask =
                    p_cell_ctxt->mbms_config.mbsfn_info[index].subframe_alloc.bitmask;
                if (SON_ONE == p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask)
                {
                p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.oneframe[0] =  
                    p_cell_ctxt->mbms_config.mbsfn_info[index].subframe_alloc.oneframe[0];
                }
                else
                {
                    SON_MEMCPY(p_cell_info->cell_info.mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.fourframe,
                        p_cell_ctxt->mbms_config.mbsfn_info[index].subframe_alloc.fourframe,
                        SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT);
                }
            }
            p_cell_info->cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT; 
        }
#endif
        x2_populate_srv_cell_nbrs_info(&p_cell_ctxt->intra_rat_neighbor_table,
                &(p_cell_info->nbour_cell_info));
    }

    SON_UT_TRACE_EXIT();
} /* x2_populate_srv_cell_info */

/*****************************************************************************
 * Function Name  : x2_convert_l3_plmn_id_to_son_plmn_id
 * Inputs         : p_l3_plmn_id    - Pointer to plmn id for l3
 *                  p_son_plmn_id   - Pointer to plmn id for son
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function which is used to convert PLMN value
 *                  provided by L3 X2AP to the PLMN value required by SON
 ****************************************************************************/
son_void_t
x2_convert_l3_plmn_id_to_son_plmn_id
(
    x2ap_plmn_identity_t *p_l3_plmn_id,
    son_cell_plmn_info_t *p_son_plmn_id
)
{
    /* As per TS 36.423 :
     * PLMN Id consists of 3 bit string where
     * - digits 0 to 9, encoded 0000 to 1001,
     * - 1111 used as filler digit, two digits per octet,
     * - bits 4 to 1 of octet n encoding digit 2n-1
     * - bits 8 to 5 of octet n encoding digit 2n
     *
     * -The Selected PLMN identity consists of 3 digits from MCC followed by either
     *  -a filler digit plus 2 digits from MNC (in case of 2 digit MNC) or
     *  -3 digits from MNC (in case of a 3 digit MNC). */

    son_u8 temp =  p_l3_plmn_id->plmn_id[1] >> 4;

    SON_UT_TRACE_ENTER();

    p_son_plmn_id->mcc[0] = 0x0F & p_l3_plmn_id->plmn_id[0];
    p_son_plmn_id->mcc[1] = p_l3_plmn_id->plmn_id[0] >> 4;
    p_son_plmn_id->mcc[2] = 0x0F & p_l3_plmn_id->plmn_id[1];

    /* Check if filler digit is present */
    if (0x0F == temp)
    {
        p_son_plmn_id->num_mnc_digit = 2;
        p_son_plmn_id->mnc[0] = 0x0F & p_l3_plmn_id->plmn_id[2];
        p_son_plmn_id->mnc[1] = p_l3_plmn_id->plmn_id[2] >> 4;
    }
    else
    {
        p_son_plmn_id->num_mnc_digit = 3;
        p_son_plmn_id->mnc[0] = temp;
        p_son_plmn_id->mnc[1] = 0x0F & p_l3_plmn_id->plmn_id[2];
        p_son_plmn_id->mnc[2] = p_l3_plmn_id->plmn_id[2] >> 4;
    }
} /* x2_convert_l3_plmn_id_to_son_plmn_id */

/*****************************************************************************
 * Function Name  : x2_convert_l3_enbid_to_son_enbid
 * Inputs         : p_l3_enbid - Pointer to the eNB Id value defined by L3 X2AP
 * Outputs        : p_son_enbid - Pointer to the eNB Id value defined by SON
 * Returns        : None
 * Description    : This is the function which is used to convert eNB Id value
 *                  provided by L3 X2AP to the eNB Id value required by SON
 ****************************************************************************/
son_void_t
x2_convert_l3_enbid_to_son_enbid
(
    x2_gb_enb_id_t      *p_l3_enbid, 
    son_global_enb_id_t *p_son_enbid
)
{
    SON_UT_TRACE_ENTER();
    x2_convert_l3_plmn_id_to_son_plmn_id(
        &p_l3_enbid->plmn_identity, &p_son_enbid->plmn_id);

    if (X2AP_OAM_MACRO_ENB_ID_PRESENT & p_l3_enbid->enb_id.presence_bitmask) 
    {
        p_son_enbid->enb_type = SON_MACRO_ENB;
        SON_MEMCPY(p_son_enbid->enb_id,
            p_l3_enbid->enb_id.macro_enb_id.eNB_id,
            MACRO_ENB_ID_OCTET_SIZE);
    }
    else if (X2AP_OAM_HOME_ENB_ID_PRESENT & p_l3_enbid->enb_id.presence_bitmask)
    {
        p_son_enbid->enb_type = SON_HOME_ENB;
        SON_MEMCPY(p_son_enbid->enb_id, 
            p_l3_enbid->enb_id.home_enb_id.eNB_id,
            HOME_ENB_ID_OCTET_SIZE);
    }

    SON_UT_TRACE_EXIT();
} /* x2_convert_l3_enbid_to_son_enbid */

/*****************************************************************************
 * Function Name  : x2_convert_son_enbid_to_l3_enbid
 * Inputs         : p_son_enbid - Pointer to the eNB Id value defined by SON
 * Outputs        : p_l3_enbid - Pointer to the eNB Id value defined by L3 X2AP
 * Returns        : None
 * Description    : This is the function which is used to convert eNB Id value
 *                  provided by SON to the eNB Id value required by L3 X2AP
 ****************************************************************************/
son_void_t
x2_convert_son_enbid_to_l3_enbid
(
    son_global_enb_id_t *p_son_enbid,
    x2_gb_enb_id_t      *p_l3_enbid 
)
{
    SON_UT_TRACE_ENTER();
    x2_convert_son_plmn_id_to_l3_plmn_id(
        &p_son_enbid->plmn_id, &p_l3_enbid->plmn_identity);

    if (SON_MACRO_ENB == p_son_enbid->enb_type)
    {
        p_l3_enbid->enb_id.presence_bitmask |= X2AP_OAM_MACRO_ENB_ID_PRESENT;
        SON_MEMCPY(&p_l3_enbid->enb_id.macro_enb_id.eNB_id,
            p_son_enbid->enb_id,     
            MACRO_ENB_ID_OCTET_SIZE);
    }
    else if (SON_HOME_ENB == p_son_enbid->enb_type)
    {
        p_l3_enbid->enb_id.presence_bitmask |= X2AP_OAM_HOME_ENB_ID_PRESENT;
        SON_MEMCPY(&p_l3_enbid->enb_id.home_enb_id.eNB_id,
            p_son_enbid->enb_id, 
            HOME_ENB_ID_OCTET_SIZE);
    }

    SON_UT_TRACE_EXIT();
} /* x2_convert_son_enbid_to_l3_enbid */

/*****************************************************************************
 * Function Name  : x2_insert_eNB_in_list
 * Inputs         : p_list      - Pointer to the list
 *                : p_enb_rec   - Pointer to eNB to inserted into the list
 * Outputs        : None
 * Returns        : Pointer to newly inserted eNb node hash record
 * Description    : This function inserts a eNB into the list
 ****************************************************************************/
son_nbr_enb_hash_node_t *
x2_insert_eNB_in_list
(
 SON_LIST                *p_list,
 son_nbr_enb_hash_rec_t   *p_enb_rec
)
{
    son_nbr_enb_hash_node_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_rec || SON_PNULL == p_list)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "eNB could not be added as "
                " p_enb_rec or p_list is NULL");
        SON_UT_TRACE_EXIT();
        return p_list_node;
    }

    p_list_node = son_mem_get(sizeof(son_nbr_enb_hash_node_t));
    if (SON_PNULL != p_list_node)
    {
        p_list_node->data = p_enb_rec;

        list_insert_node(p_list,
                &p_list_node->anchor);

        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_INFO,
                "Added eNB 0x%x in X2 List", son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Memory allocation failed for p_list_node. Cannot add eNB 0x%x in the X2 list",
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* x2_insert_eNB_in_list */

/*****************************************************************************
 * Function Name  : x2_get_first_eNB_rec
 * Inputs         : p_list      - Pointer to the list
 * Outputs        : None
 * Returns        : son_nbr_enb_hash_node_t
 * Description    : Function to get first eNB record in X2
 ****************************************************************************/
son_nbr_enb_hash_node_t *
x2_get_first_eNB_rec
(
SON_LIST *p_list
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (son_nbr_enb_hash_node_t *)
                YMEMBEROF(son_nbr_enb_hash_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* x2_get_first_eNB_rec */

/*****************************************************************************
 * Function Name  : x2_get_next_eNB_rec
 * Inputs         : p_node  - Pointer to the eNb node
 * Outputs        : None
 * Returns        : Pointer to next eNb node in the list
 * Description    : This function returns pointer to next eNb node in the list
 ****************************************************************************/
son_nbr_enb_hash_node_t *
x2_get_next_eNB_rec
(
son_nbr_enb_hash_node_t *p_node
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            return (son_nbr_enb_hash_node_t *)
                YMEMBEROF(son_nbr_enb_hash_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* x2_get_next_eNB_rec */

/*****************************************************************************
 * Function Name  : x2_clear_eNB_rec_list
 * Inputs         : p_node  - Pointer to the eNb node
 * Outputs        : None
 * Returns        : None
 * Description    : This function clears all the eNb records in the list
 ****************************************************************************/
son_void_t
x2_clear_eNB_rec_list
(
    SON_LIST *p_list
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_list_node = list_pop_node(p_list);

    while (p_list_node)
    {
        son_nbr_enb_hash_node_t *p_data = YMEMBEROF(son_nbr_enb_hash_node_t,
                anchor, p_list_node);

        /* Free the memory occupied by the node */
        son_mem_free(p_data);

        p_list_node = list_pop_node(p_list);
     }

    SON_UT_TRACE_EXIT();
} /* x2_clear_eNB_rec_list */

/*****************************************************************************
 * Function Name  : x2_insert_add_res_node
 * Inputs         : p_list           - Pointer to the list
 *                : p_enb_add_status - Pointer to the structure containing
 *                : data stored in the eNB add response list
 * Outputs        : None
 * Returns        : Pointer to add eNB response list nodes data
 * Description    : This Function adds eNB in X2 response list
 ****************************************************************************/
son_enb_add_status_node_t *
x2_insert_add_res_node
(
    SON_LIST                *p_list,
    son_enb_add_status_t    *p_enb_add_status
)
{
    son_enb_add_status_node_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_add_status || SON_PNULL == p_list)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_DETAILED,
                "Response node could not be added as "
                " x2_insert_add_res_node or p_list is NULL");
        SON_UT_TRACE_EXIT();
        return p_list_node;
    }

    p_list_node = son_mem_get(sizeof(son_enb_add_status_node_t));
    if (SON_PNULL != p_list_node)
    {
        p_list_node->data.p_enb_hash_rec = p_enb_add_status->p_enb_hash_rec;
        p_list_node->data.result = p_enb_add_status->result;

        list_insert_node(p_list,
                &p_list_node->anchor);

        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_DETAILED,
                "Added eNB  0x%x in X2 list",
                son_convert_char_cell_id_to_int_cell_id(p_enb_add_status->p_enb_hash_rec->enb_id.enb_id));
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Memory allocation failed for p_list_node. Cannot add eNB 0x%x in X2 list",
                son_convert_char_cell_id_to_int_cell_id(p_enb_add_status->p_enb_hash_rec->enb_id.enb_id));
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* x2_insert_add_res_node */

/*****************************************************************************
 * Function Name  : x2_clean_up_enb_cfg_pending_data
 * Inputs         : p_enb_ctxt  - Pointer to enb hash record
 * Outputs        : None
 * Returns        : enum error code
 * Description    : This function cleans up the enb pending list for X2
 ****************************************************************************/
son_bool_et
x2_clean_up_enb_cfg_pending_data
(
    son_nbr_enb_hash_rec_t      *p_enb_ctxt
)
{
    son_bool_et ret = SON_FALSE;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_ctxt)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_WARNING,
                "Failed to do the clean"
                "up for the given eNB, p_enb_ctxt is NULL");
    }
    else
    {
        p_enb_ctxt->time_d_wait_expiry.s = 0;
        p_enb_ctxt->time_d_wait_expiry.us = 0;
        /* SPR 11340 Fix Starts */
        /* Lines deleted */
        p_enb_ctxt->is_enb_config_update_res_awaited = SON_FALSE;
        /* SPR 11340 Fix Ends */
        p_enb_ctxt->is_self_del_enb_req_triggered = SON_FALSE;
        x2_clear_enb_cfg_pending_list(&p_enb_ctxt->enb_config_cell_info);
        ret = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* x2_clean_up_enb_cfg_pending_data */

/*****************************************************************************
 * Function Name  : x2_insert_cell_in_enb_cfg
 * Inputs         : p_enb_config_cell_list - ENB config cell list
 *                  cell_id -   Intra rat global cell id
 *                  last_op - Last operation performed on NRT
 *                  deac_flag - Deactivation flag
 * Outputs        : None
 * Returns        : None
 * Description    : This function inserts new cell in ENB Config Pending List
 ******************************************************************************/
son_void_t
x2_insert_cell_in_enb_cfg
(
    son_nbr_enb_hash_rec_t *p_enb_rec,
    son_intra_rat_global_cell_id_t *p_cell_id,
    son_nrt_operation_performed_et last_op,
    x2_deactivation_flag_et deac_flag
)
{
    enb_config_cell_data_t  *p_list_node     = SON_PNULL;
    son_module_id_et        module_id        = SON_ES_MODULE_ID;
    enb_cell_data_t         *p_enb_cell_data = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /*Get the Serving cell context from the pending res*/
    p_list_node = x2_find_cell_in_enb_cfg_pending_list(
                         &p_enb_rec->enb_config_cell_info,
                         p_cell_id);

    /*Determining the mdoule id */ 
    if (X2_CELL_DEACTIVATION_IND_NULL ==  deac_flag)
    {
        module_id = SON_ANR_MODULE_ID;
    }

    /*Serving Cell is not available in pending resp */ 
    if (SON_PNULL == p_list_node)
    {
        /*Create the serving cell context to be inserted in pending resp*/  
        p_list_node = son_mem_get(sizeof(enb_config_cell_data_t));

        if (SON_PNULL != p_list_node)
        {
            /*Populate the the serving cell context appropriatly */

            /*Assign the serving cell id*/ 
            SON_MEMCPY(&p_list_node->enb_cell_data.cell_id,
                    p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

            /*Assign the operation*/
            p_list_node->enb_cell_data.last_op = last_op;

            /*Assign the deactivation flag*/
            p_list_node->enb_cell_data.deacivation_flag = deac_flag;

            /*Assign the module id*/
            p_list_node->enb_cell_data.module_id = module_id;

            /*Insert the context in the list*/
            list_insert_node(&p_enb_rec->enb_config_cell_info, 
                    &p_list_node->enb_cell_node);

            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_INFO,
                    "Added Cell 0x%x"
                    " in enb config Pending list",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
        }
        else
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_ERROR,
                    "Memory allocation failure for p_list_node."
                    " Can not add Cell 0x%x in pending list",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
        }
    }
    else
    {
        p_enb_cell_data = &p_list_node->enb_cell_data;
        if (SON_ANR_MODULE_ID == module_id)
        {
            p_enb_cell_data->module_id = SON_ANR_MODULE_ID;

            if (SON_NRT_OP_ADD == last_op)
            {
                if (SON_NRT_OP_DELETE == p_enb_cell_data->last_op)
                {
                    p_enb_cell_data->last_op          = SON_NRT_OP_UPDATE;
                    p_enb_cell_data->deacivation_flag = 
                        X2_CELL_DEACTIVATION_IND_NULL;
                }
            }
            else if (SON_NRT_OP_DELETE == last_op)
            {
                if (SON_NRT_OP_ADD == p_enb_cell_data->last_op)
                {
                    /*Remove the entry from the pending res.*/
                    list_delete_node(&p_enb_rec->enb_config_cell_info,
                                     &p_list_node->enb_cell_node);
                    son_mem_free(p_list_node);
                }
                else if (SON_NRT_OP_UPDATE == p_enb_cell_data->last_op)
                {
                    p_enb_cell_data->last_op = SON_NRT_OP_DELETE;
                    p_enb_cell_data->deacivation_flag = 
                        X2_CELL_DEACTIVATION_IND_NULL;
                }
            }
        }
        else
        {
            /*When Add is present then there is Module Id as ANR always*/  
            if (SON_NRT_OP_ADD == p_enb_cell_data->last_op)
            {
                if (X2_CELL_DEACTIVATION_IND_NULL == 
                    p_enb_cell_data->deacivation_flag)
                {
                    p_enb_cell_data->deacivation_flag = deac_flag;    
                } 
                else if (deac_flag != p_enb_cell_data->deacivation_flag)
                {
                    p_enb_cell_data->deacivation_flag = 
                        X2_CELL_DEACTIVATION_IND_NULL;    
                }
            }
            else if (SON_NRT_OP_UPDATE == p_enb_cell_data->last_op)
            {
                if (SON_ANR_MODULE_ID == p_enb_cell_data->module_id)
                {
                    if (X2_CELL_DEACTIVATION_IND_NULL == 
                        p_enb_cell_data->deacivation_flag)
                    {
                        p_enb_cell_data->deacivation_flag = deac_flag;    
                    } 
                    else if (deac_flag != p_enb_cell_data->deacivation_flag)
                    {
                        p_enb_cell_data->deacivation_flag = 
                            X2_CELL_DEACTIVATION_IND_NULL;    
                    }
                }
                else
                {   /*Remove the entry form the pending response*/
                    list_delete_node(&p_enb_rec->enb_config_cell_info,
                            &p_list_node->enb_cell_node);
                    son_mem_free(p_list_node);
                }
            }  
        }
    }

    SON_UT_TRACE_EXIT();
} /* x2_insert_cell_in_enb_cfg */

/*****************************************************************************
 * Function Name  : x2_get_first_cell_in_enb_cfg_pending_list
 * Inputs         : p_pnding_cell_list - Pointer to pending cells list
 * Outputs        : None
 * Returns        : enb_config_cell_data_t*  - pointer to pending cell of
 *                                            first cell in enb config pending
 *                                            list.
 * Description    : This function gets the first  pending cell in the enb config 
 *                  pending list
 ******************************************************************************/
enb_config_cell_data_t *
x2_get_first_cell_in_enb_cfg_pending_list
(
   son_enb_config_cell_list_t  *p_pnding_cell_list
)
{
    SON_LIST_NODE   *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (list_count(p_pnding_cell_list) > 0)
    {
        p_node = get_first_list_node(p_pnding_cell_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (enb_config_cell_data_t *)
                   YMEMBEROF(enb_config_cell_data_t, enb_cell_node, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* x2_get_first_cell_in_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_get_next_cell_in_enb_cfg_pending_list
 * Inputs         : p_pnding_cell - Pointer to pending cell in enb config pending
 *                                  list.
 * Outputs        : None
 * Returns        : enb_config_cell_data_t*  - pointer to pending cell of
 *                                            next cell in enb config pending
 *                                            list.
 * Description    : This function gets the next  pending cell in the enb config 
 *                  pending list
 ******************************************************************************/
enb_config_cell_data_t *
x2_get_next_cell_in_enb_cfg_pending_list
(
   enb_config_cell_data_t  *p_pnding_cell
)
{
    SON_LIST_NODE   *p_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_pnding_cell)
    {
        p_list_node = get_next_list_node(&p_pnding_cell->enb_cell_node);
        if (SON_PNULL != p_list_node)
        {
            return (enb_config_cell_data_t *)
                   YMEMBEROF(enb_config_cell_data_t, enb_cell_node, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* x2_get_next_cell_in_enb_cfg_pending_list */

/*****************************************************************************
 * Function Name  : x2_create_send_enb_config_update_req_msg
 * Inputs         : p_pnding_cell_list  - Pointer containing eNB config update
 *                                        request data
 *                  p_enb_ctxt          - Pointer to enb context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates and sends eNB Config Update request
 *                  message from the data in pending list.
 ******************************************************************************/
son_void_t
x2_create_send_enb_config_update_req_msg
(
   son_enb_config_cell_list_t      *p_pnding_cell_list,
   son_nbr_enb_hash_rec_t           *p_enb_ctxt
)
{
    enb_config_cell_data_t  *p_list_node     = SON_PNULL;
    enb_cell_data_t         *p_enb_cell_data = SON_PNULL;
    SON_LIST_NODE           *p_node          = SON_PNULL;
    x2_deactivation_flag_et deac_ind = X2_CELL_DEACTIVATION_IND_NULL;
    son_rrc_enb_config_update_req_t  *p_update_req = SON_PNULL;
    x2ap_enb_config_update_req_t *p_enb_config_update_req = SON_PNULL;
    son_u32 list_size = 0, index = 0;                        
    son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);
    /* SPR-10331 Fix Starts */
    x2_gu_group_id_node_t *p_gu_grp_id_list_node = SON_PNULL;
    son_size_t gu_grp_id_len = sizeof(x2ap_gu_group_id_t);
    /* SPR-10331 Fix Ends */
    SON_UT_TRACE_ENTER();
            
    p_update_req = (son_rrc_enb_config_update_req_t *)
        son_mem_get(update_req_size);

    if (SON_PNULL == p_update_req)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed to p_update_req. Can not send eNB Config Update"
                " request message to eNB 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
    }
    else
    {
        SON_MEMSET(p_update_req, 0, update_req_size);
        p_update_req->transaction_id = son_generate_txn_id();
        p_enb_config_update_req = &p_update_req->enb_config_update_req;
        p_enb_config_update_req->num_peer_enb = 1;
        x2_convert_son_enbid_to_l3_enbid(&p_enb_ctxt->enb_id, 
            &p_enb_config_update_req->enb_info[0]);

        list_size = list_count(p_pnding_cell_list);
        for (index = 0; index < list_size; index++)
        {
            /*Get the node from the pending data */
            p_node = list_pop_node(p_pnding_cell_list);
            p_list_node = YMEMBEROF(enb_config_cell_data_t, 
                enb_cell_node, p_node);
            if (SON_PNULL != p_list_node)
            {
                p_enb_cell_data = &p_list_node->enb_cell_data;

                if (SON_NRT_OP_ADD  == p_enb_cell_data->last_op)
                {
                    p_enb_config_update_req->bitmask |= 
                        X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT;
                    x2_populate_srv_cell_info(&p_enb_cell_data->cell_id,
                            &p_enb_config_update_req->add_served_cell.cell_list
                            [p_enb_config_update_req->add_served_cell.
                            num_served_cell]);
                    p_enb_config_update_req->add_served_cell.num_served_cell++;

                    if (X2_CELL_DEACTIVATION_TRUE == 
                        p_enb_cell_data->deacivation_flag)
                    {  
                        /*Construct the modify list*/ 
                        p_enb_config_update_req->bitmask |= 
                            X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT; 

                        x2_populate_cells_to_modify_list_info(
                                &p_enb_cell_data->cell_id,
                                X2_CELL_DEACTIVATION_TRUE,
                                &p_enb_config_update_req->mod_served_cell.
                                cell_list[p_enb_config_update_req->
                                mod_served_cell.num_served_cell]);
                        p_enb_config_update_req->mod_served_cell.
                            num_served_cell++;
                    }    
                }
                else if (SON_NRT_OP_UPDATE == p_enb_cell_data->last_op)
                {
                    deac_ind = X2_CELL_DEACTIVATION_FALSE; 
                    p_enb_config_update_req->bitmask |= 
                        X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;

                    if (X2_CELL_DEACTIVATION_TRUE == 
                        p_enb_cell_data->deacivation_flag)
                    {
                        deac_ind = X2_CELL_DEACTIVATION_TRUE;                 
                    }

                    x2_populate_cells_to_modify_list_info(
                            &p_enb_cell_data->cell_id,
                            deac_ind, 
                            &p_enb_config_update_req->mod_served_cell.cell_list[
                            p_enb_config_update_req->mod_served_cell.
                            num_served_cell]);
                    p_enb_config_update_req->mod_served_cell.num_served_cell++;
                }
                else
                {
                    p_enb_config_update_req->bitmask |= 
                        X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT;

                    x2_populate_cell_id_for_l3(
                            p_enb_cell_data->cell_id.cell_identity,
                            &p_enb_config_update_req->del_served_cell.
                            cell_id_list[
                            p_enb_config_update_req->del_served_cell.
                            num_served_cell]);

                    p_enb_config_update_req->del_served_cell.num_served_cell++;
                }

                son_mem_free(p_list_node);
                p_list_node = SON_PNULL;
            }
        }

        /* SPR-10331 Fix Starts */
        /* Check if data is present in sent GU group Id list. If it is present
           then send the consolidated data of sent and pending GU Group Id 
           to L3 X2AP else just send the pending GU Group Id to L3 X2AP */
        if (list_count(&p_enb_ctxt->sent_gu_group_id_data.add_gu_id_list) ||
                list_count(&p_enb_ctxt->sent_gu_group_id_data.
                    delete_gu_id_list))
        {
            p_node = list_pop_node(&p_enb_ctxt->sent_gu_group_id_data.
                    add_gu_id_list);
            while (SON_PNULL != p_node && MAX_ENB_POOLS > 
                    p_enb_config_update_req->add_gu_id.num_gu_group_id)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);
                if (SON_FAILURE == x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
                            pending_gu_group_id_data.delete_gu_id_list, 
                            &p_gu_grp_id_list_node->gu_group_id))
                {
                    SON_MEMCPY(&p_enb_config_update_req->add_gu_id.gu_id_list[
                            p_enb_config_update_req->add_gu_id.num_gu_group_id],
                            &p_gu_grp_id_list_node->gu_group_id,
                            gu_grp_id_len);
                    p_enb_config_update_req->add_gu_id.num_gu_group_id++;
                }
                son_mem_free(p_gu_grp_id_list_node);
                p_gu_grp_id_list_node = SON_PNULL;
                p_node = list_pop_node(&p_enb_ctxt->sent_gu_group_id_data.
                        add_gu_id_list);
            }

            p_node = list_pop_node(&p_enb_ctxt->sent_gu_group_id_data.
                    delete_gu_id_list);
            while (SON_PNULL != p_node && MAX_ENB_POOLS > 
                    p_enb_config_update_req->delete_gu_id.num_gu_group_id)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);

                if (SON_FAILURE == x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
                            pending_gu_group_id_data.add_gu_id_list, 
                            &p_gu_grp_id_list_node->gu_group_id))
                {
                    SON_MEMCPY(&p_enb_config_update_req->
                            delete_gu_id.gu_id_list[p_enb_config_update_req->
                            delete_gu_id.num_gu_group_id],
                            &p_gu_grp_id_list_node->gu_group_id,
                            gu_grp_id_len);
                    p_enb_config_update_req->delete_gu_id.num_gu_group_id++;
                }
                son_mem_free(p_gu_grp_id_list_node);
                p_gu_grp_id_list_node = SON_PNULL;
                p_node = list_pop_node(&p_enb_ctxt->sent_gu_group_id_data.
                        delete_gu_id_list);
            }

            p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                    add_gu_id_list);
            while (SON_PNULL != p_node && MAX_ENB_POOLS > 
                    p_enb_config_update_req->add_gu_id.num_gu_group_id)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);

                if (SON_FAILURE == x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
                            sent_gu_group_id_data.delete_gu_id_list, 
                            &p_gu_grp_id_list_node->gu_group_id))
                {
                    SON_MEMCPY(&p_enb_config_update_req->add_gu_id.gu_id_list[
                            p_enb_config_update_req->add_gu_id.num_gu_group_id],
                            &p_gu_grp_id_list_node->gu_group_id,
                            gu_grp_id_len);
                    p_enb_config_update_req->add_gu_id.num_gu_group_id++;
                }
                son_mem_free(p_gu_grp_id_list_node);
                p_gu_grp_id_list_node = SON_PNULL;
                p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                        add_gu_id_list);
            }

            p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                    delete_gu_id_list);
            while (SON_PNULL != p_node && MAX_ENB_POOLS > 
                    p_enb_config_update_req->delete_gu_id.num_gu_group_id)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);

                if (SON_FAILURE == x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
                            sent_gu_group_id_data.add_gu_id_list, 
                            &p_gu_grp_id_list_node->gu_group_id))
                {
                    SON_MEMCPY(&p_enb_config_update_req->delete_gu_id.
                            gu_id_list[p_enb_config_update_req->
                            delete_gu_id.num_gu_group_id],
                            &p_gu_grp_id_list_node->gu_group_id,
                            gu_grp_id_len);
                    p_enb_config_update_req->delete_gu_id.num_gu_group_id++;
                }
                son_mem_free(p_gu_grp_id_list_node);
                p_gu_grp_id_list_node = SON_PNULL;
                p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                        delete_gu_id_list);
            }
            
            if (p_enb_config_update_req->add_gu_id.num_gu_group_id)
            {
                p_enb_config_update_req->bitmask |= 
                    X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
            }

            if (p_enb_config_update_req->delete_gu_id.num_gu_group_id)
            {
                p_enb_config_update_req->bitmask |=
                    X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
            }

        }
        else 
        {
            /* Get the node from the pending GU Group Id add list */
            p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                    add_gu_id_list);
            while (SON_PNULL != p_node)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);
                p_enb_config_update_req->bitmask |= 
                    X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
                SON_MEMCPY(&p_enb_config_update_req->add_gu_id.gu_id_list[
                        p_enb_config_update_req->add_gu_id.num_gu_group_id],
                        &p_gu_grp_id_list_node->gu_group_id,
                        gu_grp_id_len);
                p_enb_config_update_req->add_gu_id.num_gu_group_id++;
                p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                        add_gu_id_list);
            }

            /* Get the node from the pending GU Group Id delete list */
            p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                    delete_gu_id_list);
            while (SON_PNULL != p_node)
            {
                p_gu_grp_id_list_node = YMEMBEROF(x2_gu_group_id_node_t, 
                        anchor, p_node);
                p_enb_config_update_req->bitmask |= 
                    X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
                SON_MEMCPY(&p_enb_config_update_req->delete_gu_id.gu_id_list[
                        p_enb_config_update_req->delete_gu_id.num_gu_group_id],
                        &p_gu_grp_id_list_node->gu_group_id,
                        gu_grp_id_len);
                p_enb_config_update_req->delete_gu_id.num_gu_group_id++;
                p_node = list_pop_node(&p_enb_ctxt->pending_gu_group_id_data.
                        delete_gu_id_list);
            }
        }
        /* SPR-10331 Fix Ends */

        /* Call pup tool to send update req to RRC */
        if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_enb_config_update_req(
                    &p_update_req->enb_config_update_req,
                    SON_MODULE_ID,
                    SON_RRC_MODULE_ID,
                    p_update_req->transaction_id,
                    SON_NULL))
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                    SON_ERROR,
                    "eNB config update request could not be sent to RRC "
                    "for eNB 0x%x", 
                    son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
        }
        else
        {
            /* SPR 11340 Fix Starts */
            /* Line deleted */
            /* Lines deleted */
            p_enb_ctxt->is_enb_config_update_res_awaited = SON_TRUE;
            /* SPR 11340 Fix Ends */
            /* SPR-10331 Fix Starts */
            /* Store the data in last sent eNB config update request
               data which shall be used to re-initiate the
               X2AP_ENB_CONFIG_UPDATE_REQ in case eNB Config
               Update failure Response is received from peer ENB. */
            for (index = 0; 
                    index < p_enb_config_update_req->add_gu_id.num_gu_group_id;
                    index++)
            {
                x2_insert_gu_grp_id_in_list(
                        &p_enb_ctxt->sent_gu_group_id_data.add_gu_id_list,
                        &p_enb_config_update_req->add_gu_id.gu_id_list[index]);
            }
            for (index = 0; index < 
                    p_enb_config_update_req->delete_gu_id.num_gu_group_id;
                    index++)
            {
                x2_insert_gu_grp_id_in_list(
                        &p_enb_ctxt->sent_gu_group_id_data.delete_gu_id_list,
                        &p_enb_config_update_req->delete_gu_id.
                        gu_id_list[index]);
            }
            /* SPR-10331 Fix Ends */
        }

        /* Free the memory allocated on heap for the update request */
        son_mem_free(p_update_req);
    }

    SON_UT_TRACE_EXIT();
} /* x2_create_send_enb_config_update_req_msg */

/******************************************************************************
 * Function Name  : x2_populate_cells_to_modify_list_info 
 * Inputs         : p_src_cgi             Pointer to the source cell Id
 *                  deacitvation_ind    Deactivation Indication bit    
 * Outputs        : p_mod_info          Pointer to the cells to modify list 
 * Returns        : None
 * Description    : This function populates the cells to modify list of eNB 
 *                  config update request message.
 *****************************************************************************/
son_void_t
x2_populate_cells_to_modify_list_info
(
    son_intra_rat_global_cell_id_t      *p_src_cgi,
    x2_deactivation_flag_et             deacitvation_ind,
    x2ap_served_cells_to_modify_info_t  *p_mod_info
)
{
    SON_UT_TRACE_ENTER();
    /* If deactivation activation is present */
    if (X2_CELL_DEACTIVATION_TRUE == deacitvation_ind)
    {   
        p_mod_info->bitmask |= X2AP_DEACTIVATION_IND_PRESENT;
        p_mod_info->deactivation_ind = 0;
    }
   /*This is not used in X2AP_ENB_CONFIG_UPDATE_REQ*/ 
    /*SPR 21554 Start*/
    p_mod_info->old_cell_id=0;
    /*SPR 21554 End*/
    
    x2_populate_srv_cell_info(p_src_cgi, &p_mod_info->served_cell_info);

    SON_UT_TRACE_EXIT();
}

/* SPR_19619 start */

/******************************************************************************
 * Function Name  : x2_populate_cells_for_peer_cell_activation
 * Inputs         : p_src_cgi             Pointer to the source cell Id
 *                  deacitvation_ind    Deactivation Indication bit    
 * Outputs        : p_mod_info          Pointer to the cells to modify list 
 * Returns        : None
 * Description    : This function populates the info of serving cell 
 *                  config update request message.
 *****************************************************************************/
son_void_t
x2_populate_cells_for_peer_cell_activation
(
    son_intra_rat_global_cell_id_t          *p_src_cgi,
    x2ap_served_cells_to_modify_info_t      *p_cell_activation_info
)
{
    SON_UT_TRACE_ENTER();
    
    x2_populate_cell_id_for_l3(p_src_cgi->cell_identity,
        &p_cell_activation_info->old_cell_id);
    
    x2_populate_srv_cell_info(p_src_cgi, &p_cell_activation_info->served_cell_info);

    SON_UT_TRACE_EXIT();
}


/* SPR_19619 stop */


/* SPR 7528 IPV6 Support Start */
/******************************************************************************
 * Function Name  : x2_convert_son_ipv6_addr_to_l3_ipv6_addr
 * Inputs         : p_son_ipv6_addr - Pointer to IP address in the notation used by SON.
 * Outputs        : p_l3_ipv6_addr - Pointer to the IP address in the notation
 *                  used by L3 X2AP
 * Returns        : SON_SUCCESS/SON_FAILURE
 * Description    : This function converts the IP address from the notation used by
 *                  SON to the notation used by L3 X2AP.
 *****************************************************************************/
son_return_et
x2_convert_son_ipv6_addr_to_l3_ipv6_addr
(
    son_u8  *p_son_ipv6_addr,
    son_u8  *p_l3_ipv6_addr
)
{
    son_return_et   ret_val = SON_SUCCESS;
    son_u8          index   = SON_NULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_son_ipv6_addr ||
        SON_PNULL == p_l3_ipv6_addr)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
            SON_ERROR,
            "Could not "
            "convert IP address, p_son_ip_addr or "
            "p_l3_ipv6_addr is NULL");
        ret_val = SON_FAILURE;
    }
    else
    {
        for (index = 0; index < SON_IPV6_SIZE; index++)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                    "SON IPv6 addr %u", p_son_ipv6_addr[index]);
        }

        SON_MEMSET(p_l3_ipv6_addr, '\0', MAX_X2AP_IPV6_ADDRESS_LENGTH);

        if(SON_PNULL == son_inet_ntop(AF_INET6, (const son_void_t *)p_son_ipv6_addr, 
                             (son_8 *)p_l3_ipv6_addr, MAX_X2AP_IPV6_ADDRESS_LENGTH))
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
                    "Failed to convert "
                    "SON IPv6 address to L3 IPv6 address errno = %d", errno);
            ret_val = SON_FAILURE;
        }
        else
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
                    "Converted "
                    "to L3 IPv6 address = %s", p_l3_ipv6_addr);
            ret_val = SON_SUCCESS;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_convert_son_ipv6_addr_to_l3_ipv6_addr */

/******************************************************************************
 * Function Name  : x2_convert_l3_ipv6_addr_to_son_ipv6_addr
 * Inputs         : p_l3_ipv6_addr - Pointer to IP address received from L3 X2AP
 * Outputs        : p_son_ipv6_addr - Pointer to the IP address in the notation
 *                  used by SON.
 * Returns        : SON_SUCCESS/SON_FAILURE
 * Description    : This function converts the IP address from the notation used by
 *                  L3 X2AP to the notation used by SON.
 *****************************************************************************/
son_return_et
x2_convert_l3_ipv6_addr_to_son_ipv6_addr
(
    son_u8  *p_l3_ipv6_addr,
    son_u8  *p_son_ipv6_addr
)
{
    son_return_et   ret_val         = SON_SUCCESS;
    son_s8          convert_ret_val = SON_NULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_son_ipv6_addr ||
        SON_PNULL == p_l3_ipv6_addr)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
            "Could not "
            "convert IP address, p_son_ip_addr or p_l3_ipv6_addr is NULL");

        ret_val = SON_FAILURE;
    }
    else if(SON_SUCCESS != (convert_ret_val = son_inet_pton(AF_INET6, (const son_8 *)p_l3_ipv6_addr, p_son_ipv6_addr)))
    {
        if(SON_NULL == convert_ret_val)
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
                    "Failed to convert "
                    "L3 IPv6 address = %s to SON format.Invalid presentation format"
                    "received from L3", p_l3_ipv6_addr);
        }
        else
        {
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
                "Failed to convert "
                "L3 IPv6 address = %s to SON format errno = %d", p_l3_ipv6_addr, errno);
        }
        ret_val = SON_FAILURE;
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                "Converted "
                "L3 IPv6 address 0x%x%x%x%x:%x%x%x%x:%x%x%x%x:%x%x%x%x"
                " to SON IPv6 address format",
                p_l3_ipv6_addr[SON_NULL],
                p_l3_ipv6_addr[SON_ONE],
                p_l3_ipv6_addr[SON_TWO],
                p_l3_ipv6_addr[SON_THREE],
                p_l3_ipv6_addr[SON_FOUR],
                p_l3_ipv6_addr[SON_FIVE],
                p_l3_ipv6_addr[SON_SIX],
                p_l3_ipv6_addr[SON_SEVEN],
                p_l3_ipv6_addr[SON_EIGHT],
                p_l3_ipv6_addr[SON_NINE],
                p_l3_ipv6_addr[SON_TEN],
                p_l3_ipv6_addr[SON_ELEVEN],
                p_l3_ipv6_addr[SON_TWELVE],
                p_l3_ipv6_addr[SON_THIRTEEN],
                p_l3_ipv6_addr[SON_FOURTEEN],
                p_l3_ipv6_addr[SON_FIFTEEN]);

        ret_val = SON_SUCCESS;
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* x2_convert_l3_ipv6_addr_to_son_ipv6_addr */
/* SPR 7528 IPV6 Support End */

/******************************************************************************
 * Function Name  : x2_populate_cell_id_for_l3
 * Inputs         : p_son_cell_identity Pointer to the input cell identity array
 * Outputs        : p_l3_cell_identity  Pointer to the cell Id value for L3
 * Returns        : None
 * Description    : This function populates the cells Id to be sent to L3.
 *****************************************************************************/
son_void_t
x2_populate_cell_id_for_l3
(
    son_u8  *p_son_cell_identity,
    son_u8  *p_l3_cell_identity
)
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_son_cell_identity || 
        SON_PNULL == p_l3_cell_identity)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "populate cell identity, p_son_cell_identity or "
                "p_l3_cell_identity is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* SPR-11655 Fix Starts */
    /* Lines deleted */
    *p_l3_cell_identity =
        (p_son_cell_identity[2] << 4) |
        (p_son_cell_identity[3] >> 4);
    /* Lines deleted */
    /* SPR-11655 Fix Ends*/

    SON_UT_TRACE_EXIT();
} /* x2_populate_cell_id_for_l3 */

/******************************************************************************
 * Function Name  : x2_print_pending_enb_config_update_req_data
 * Inputs         : p_son_cell_identity Pointer to the input cell identity array
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the pending data stored for eNB config
 *                  update request to peer eNB.
 *****************************************************************************/
son_void_t
x2_print_pending_enb_config_update_req_data
(
    son_enb_config_cell_list_t  *p_enb_config_cell_list
)
{
    enb_config_cell_data_t  *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_list_node = x2_get_first_cell_in_enb_cfg_pending_list(
                        p_enb_config_cell_list);
    if (SON_PNULL != p_list_node)
    {
        while (SON_NULL != p_list_node)
        {
            /* SPR-11877 Fix Starts */
            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                    "Data pending to send in eNB Config Update Request"
                    " message Cell 0x%x"
                    " Last Operation %u Deactivation bit %u Module Id %u",
                    son_convert_char_cell_id_to_int_cell_id(p_list_node->enb_cell_data.cell_id.cell_identity),
                    p_list_node->enb_cell_data.last_op,
                    p_list_node->enb_cell_data.deacivation_flag,
                    p_list_node->enb_cell_data.module_id);
            /* SPR-11877 Fix Ends */

            p_list_node = x2_get_next_cell_in_enb_cfg_pending_list(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Data pending to send in eNB Config Update Request None");
    }

    SON_UT_TRACE_EXIT();
} /* x2_print_pending_enb_config_update_req_data */

/* SPR-10331 Fix Starts */
/******************************************************************************
 * Function Name  : x2_print_pending_gu_group_id_data
 * Inputs         : p_enb_rec Pointer to the eNB context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the pending GU Group Id data stored 
 *                  for eNB config update request to peer eNB.
 *****************************************************************************/
son_void_t
x2_print_pending_gu_group_id_data
(
    son_nbr_enb_hash_rec_t *p_enb_rec
)
{
    x2_gu_group_id_node_t *p_list_node = SON_PNULL;
    son_cell_plmn_info_t son_plmn_id;
    SON_UT_TRACE_ENTER();

    p_list_node = x2_get_first_node_in_gu_group_id_list(
            &p_enb_rec->pending_gu_group_id_data.add_gu_id_list);

    if (SON_PNULL != p_list_node)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                "Add list data pending to send in eNB Config Update Request"
                " message to peer eNB 0x%x", 
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

        while (SON_NULL != p_list_node)
        {
            SON_MEMSET(&son_plmn_id, 0, sizeof(son_plmn_id));
            x2_convert_l3_plmn_id_to_son_plmn_id(
                    &p_list_node->gu_group_id.plmn_id, &son_plmn_id);
            x2_print_plmn_id(&son_plmn_id);

            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                    "MME Group Id 0x%x%x",
                    p_list_node->gu_group_id.mme_group_id[0],
                    p_list_node->gu_group_id.mme_group_id[1]);

            p_list_node = x2_get_next_node_in_gu_group_id_list(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Add list data pending "
                "to send in eNB Config Update Request message to peer eNB "
                "0x%x is empty",
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

    }

    p_list_node = x2_get_first_node_in_gu_group_id_list(
            &p_enb_rec->pending_gu_group_id_data.delete_gu_id_list);

    if (SON_PNULL != p_list_node)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                "Delete list data pending to send in eNB Config Update Request"
                " message to peer eNB 0x%x", 
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

        while (SON_NULL != p_list_node)
        {
            SON_MEMSET(&son_plmn_id, 0, sizeof(son_plmn_id));
            x2_convert_l3_plmn_id_to_son_plmn_id(
                    &p_list_node->gu_group_id.plmn_id, &son_plmn_id);
            x2_print_plmn_id(&son_plmn_id);

            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                    "MME Group Id 0x%x%x",
                    p_list_node->gu_group_id.mme_group_id[0],
                    p_list_node->gu_group_id.mme_group_id[1]);

            p_list_node = x2_get_next_node_in_gu_group_id_list(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Delete list data pending "
                "to send in eNB Config Update Request message to peer eNB "
                "0x%x is empty",
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

    }

    SON_UT_TRACE_EXIT();
} /* x2_print_pending_gu_group_id_data */

/******************************************************************************
 * Function Name  : x2_print_sent_gu_group_id_data
 * Inputs         : p_enb_rec Pointer to the eNB context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the sent GU group Id data in eNB config
 *                  update request to peer eNB whose response is awaited.
 *****************************************************************************/
son_void_t
x2_print_sent_gu_group_id_data
(
    son_nbr_enb_hash_rec_t *p_enb_rec
)
{
    x2_gu_group_id_node_t *p_list_node = SON_PNULL;
    son_cell_plmn_info_t son_plmn_id;
    SON_UT_TRACE_ENTER();

    p_list_node = x2_get_first_node_in_gu_group_id_list(
            &p_enb_rec->sent_gu_group_id_data.add_gu_id_list);

    if (SON_PNULL != p_list_node)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Add list data last sent in eNB Config Update Request"
                " message to peer eNB 0x%x", 
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

        while (SON_NULL != p_list_node)
        {
            SON_MEMSET(&son_plmn_id, 0, sizeof(son_plmn_id));
            x2_convert_l3_plmn_id_to_son_plmn_id(
                    &p_list_node->gu_group_id.plmn_id, &son_plmn_id);
            x2_print_plmn_id(&son_plmn_id);

            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                    "MME Group Id 0x%x%x",
                    p_list_node->gu_group_id.mme_group_id[0],
                    p_list_node->gu_group_id.mme_group_id[1]);

            p_list_node = x2_get_next_node_in_gu_group_id_list(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Add list data last "
                "sent to peer eNB 0x%x is empty",
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

    }

    p_list_node = x2_get_first_node_in_gu_group_id_list(
            &p_enb_rec->sent_gu_group_id_data.delete_gu_id_list);

    if (SON_PNULL != p_list_node)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Delete list data last sent in eNB Config Update Request"
                " message to peer eNB 0x%x", 
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

        while (SON_NULL != p_list_node)
        {
            SON_MEMSET(&son_plmn_id, 0, sizeof(son_plmn_id));
            x2_convert_l3_plmn_id_to_son_plmn_id(
                    &p_list_node->gu_group_id.plmn_id, &son_plmn_id);
            x2_print_plmn_id(&son_plmn_id);

            SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
                    "MME Group Id 0x%x%x",
                    p_list_node->gu_group_id.mme_group_id[0],
                    p_list_node->gu_group_id.mme_group_id[1]);

            p_list_node = x2_get_next_node_in_gu_group_id_list(p_list_node);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
                "Delete list data last "
                "sent to peer eNB 0x%x is empty",
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.
                    enb_id));

    }

    SON_UT_TRACE_EXIT();
} /* x2_print_sent_gu_group_id_data */
/* SPR-10331 Fix Starts */

/******************************************************************************
 * Function Name  : x2_convert_son_ip_addr_to_l3_ip_addr
 * Inputs         : p_son_ip_addr - Pointer to IP address in the notation used by SON.
 * Outputs        : p_l3_ip_addr - Pointer to the IP address in the notation 
 *                  used by L3 X2AP
 * Returns        : None
 * Description    : This function converts the IP address from the notation used by 
 *                  SON to the notation used by L3 X2AP.
 *****************************************************************************/
son_void_t
x2_convert_son_ip_addr_to_l3_ip_addr
(
    son_u8  *p_son_ip_addr,
    son_u8  *p_l3_ip_addr
)
{
    son_u8 index = 0, num_of_digits_read = 0;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_son_ip_addr || 
        SON_PNULL == p_l3_ip_addr)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "convert IP address, p_son_ip_addr or "
                "p_l3_ip_addr is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(p_l3_ip_addr, 0, MAX_X2AP_IP_ADDRESS_LENGTH);
    for (index = 0; index < SON_IPV4_SIZE && 
        num_of_digits_read < MAX_X2AP_IP_ADDRESS_LENGTH; index++)
    {
        /* Use 4 as the size for snprintf as IP address octet can have
         *  maximum of 3 digits + 1 for storing NULL character */
        if (0 < SON_SNPRINTF((son_8 *)p_l3_ip_addr + num_of_digits_read, 4,
            "%u", (son_u16)(p_son_ip_addr[index])))
        {
            num_of_digits_read = SON_STRLEN((const son_8 *)p_l3_ip_addr);
            p_l3_ip_addr[num_of_digits_read] = '.';
            num_of_digits_read++;
            p_l3_ip_addr[num_of_digits_read] = '\0';
        }
    }

    p_l3_ip_addr[num_of_digits_read - 1] = '\0';
    
    SON_UT_TRACE_EXIT();
} /* x2_convert_son_ip_addr_to_l3_ip_addr */

/******************************************************************************
 * Function Name  : x2_convert_l3_ip_addr_to_son_ip_addr
 * Inputs         : p_l3_ip_addr - Pointer to IP address received from L3 X2AP
 * Outputs        : p_son_ip_addr - Pointer to the IP address in the notation 
 *                  used by SON.
 * Returns        : None
 * Description    : This function converts the IP address from the notation used by 
 *                  L3 X2AP to the notation used by SON.
 *****************************************************************************/
son_void_t
x2_convert_l3_ip_addr_to_son_ip_addr
(
    son_u8  *p_l3_ip_addr,
    son_u8  *p_son_ip_addr
)
{
    son_u8 *p_ip_addr_octet = SON_PNULL;
    son_u8 index = 0;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_son_ip_addr || 
        SON_PNULL == p_l3_ip_addr)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Could not "
                "convert IP address, p_son_ip_addr or "
                "p_l3_ip_addr is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_ip_addr_octet = (son_u8 *)SON_STRTOK((son_8 *)p_l3_ip_addr, ".");
    while (SON_PNULL != p_ip_addr_octet)
    {
        p_son_ip_addr[index] = son_atoi((const son_8 *)p_ip_addr_octet);
        index++;
        p_ip_addr_octet = (son_u8 *)SON_STRTOK(NULL, ".");
    }

    SON_UT_TRACE_EXIT();
} /* x2_convert_l3_ip_addr_to_son_ip_addr */

/*****************************************************************************
 * Function Name  : x2_convert_l3_ecgi_to_son_ecgi
 * Inputs         : p_x2_cgi - Pointer to the CGI value defined by L3 X2AP
 * Outputs        : p_son_cgi - Pointer to the CGI value defined by SON
 * Returns        : None
 * Description    : This is the function which is used to convert CGI value
 *                  received from L3 to the CGI value required by SON
 ****************************************************************************/
son_void_t
x2_convert_l3_ecgi_to_son_ecgi
(
    x2ap_ecgi_t                     *p_x2_cgi,   /* Input parameter */
    son_intra_rat_global_cell_id_t  *p_son_cgi /* Output parameter */
)
{
    SON_UT_TRACE_ENTER();
    
    x2_convert_l3_plmn_id_to_son_plmn_id(&p_x2_cgi->plmn_identity,
            &p_son_cgi->plmn_id);
    SON_MEMCPY(p_son_cgi->cell_identity,
            p_x2_cgi->eutran_cell_id, 
            SON_CELL_ID_OCTET_SIZE * sizeof(son_u8));

    SON_UT_TRACE_EXIT();
}

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : x2_map_ho_report_type_enum
 * Inputs         : p_l3_ho_report_type - Pointer to the HO report type.
 * Outputs        : p_rrm_ho_report_type - Pointer to the HO report type enum
 *                  value defined by RRM
 * Returns        : None
 * Description    : This is the function which is used to map the HO report 
 *                  type value recieved from L3 X2AP to the HO report enum value
 *                  defined by RRM.
 ****************************************************************************/
son_void_t
x2_map_ho_report_type_enum
(
    son_ul32                 *p_l3_ho_report_type,
    rrm_ho_cause_et         *p_rrm_ho_report_type
)
{
    SON_UT_TRACE_ENTER();

    switch (*p_l3_ho_report_type)
    {
        case HO_TOO_EARLY:
        {
            *p_rrm_ho_report_type = RRM_HO_TOO_EARLY;
            break;
        }

        case HO_TO_WRONG_CELL:
        {
            *p_rrm_ho_report_type = RRM_HO_TO_WRONG_CELL;
            break;
        }

        default:
        {
            SON_LOG(X2_GET_LOG_MODE,
                    p_son_x2_facility_name, SON_WARNING,
                        "Invalid enum value = [%lu] received from RRC", 
                    *p_l3_ho_report_type);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* x2_map_ho_report_type_enum */

#endif /* ifdef SON_MRO_FEATURE */

/* SPR Fix Start */

/*****************************************************************************
 * Function Name  : x2_get_active_nbr_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : Total number of active X2 connections
 * Description    : This function is used to calculate the total number of 
 *                  active X2 connections.
 ****************************************************************************/
son_u32 
x2_get_active_nbr_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;
    son_u32 num_of_active_nbr_count = 0;

    SON_UT_TRACE_ENTER();
    
    p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
            &g_x2_gb_ctxt.nbr_enb_context_table);

    while (SON_PNULL != p_nbr_enb_hash_tpl)
    {
        if (X2_ENB_STATE_CONNECTED == 
                p_nbr_enb_hash_tpl->data.enb_curr_fsm_state)
        {
            num_of_active_nbr_count++;
        }

        p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
                &g_x2_gb_ctxt.nbr_enb_context_table, 
                &p_nbr_enb_hash_tpl->anchor);     
    }

    SON_UT_TRACE_EXIT();
    return num_of_active_nbr_count;
} /* x2_get_active_nbr_count */

/* SPR Fix End */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : x2_convert_son_gu_group_id_to_l3_gu_group_id
 * Inputs         : p_son_gu_grp_id - Pointer to the GU Group Id value 
 *                                    defined by SON
 * Outputs        : p_x2_gu_grp_id  - Pointer to the GU Group Id value 
 *                                    defined by L3 X2AP
 * Returns        : None
 * Description    : This is the function which is used to convert GU Group 
 *                  Id value stored by SON to the GU Group Id value required 
 *                  by L3 X2AP
 ****************************************************************************/
son_void_t
x2_convert_son_gu_group_id_to_l3_gu_group_id
(
    son_gu_group_id_t   *p_son_gu_grp_id,   /* Input parameter */
    x2ap_gu_group_id_t  *p_x2_gu_grp_id     /* Output parameter */
)
{
    SON_UT_TRACE_ENTER();
    
    x2_convert_son_plmn_id_to_l3_plmn_id(&p_son_gu_grp_id->plmn_id, 
            &p_x2_gu_grp_id->plmn_id); 
    SON_MEMCPY(p_x2_gu_grp_id->mme_group_id, 
            p_son_gu_grp_id->mme_group_id,
            sizeof(son_u8) * MAX_MME_GRP_BYTES);

    SON_UT_TRACE_EXIT();
}
/* SPR-10331 Fix Ends */
/*SPR_16835_START*/
/*****************************************************************************
 * Function Name  : x2_delete_enb_config_cell_list
 * Inputs         : p_pnding_cell_list:- eNB Config Pending list pointer.  
 *
 * Outputs
 * Returns        : None
 * Description    : Function shall delete the pending eNB list.  
 ****************************************************************************/
son_void_t
x2_delete_enb_config_cell_list(
son_enb_config_cell_list_t      *p_pnding_cell_list
)
{
	son_u32 index = 0;
	son_u32 list_size; 
	enb_config_cell_data_t  *p_list_node     = SON_PNULL;
	SON_LIST_NODE           *p_node          = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if( SON_PNULL == p_pnding_cell_list )
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Pointer is NULL");

	}
	else if(0 ==  list_count(p_pnding_cell_list))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
				"List Size is 0");

	}
	else
	{
		list_size = list_count(p_pnding_cell_list);
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILED,
				"List Size Before deletion %d",list_size);

		for (index = 0; index < list_size; index++)
		{
			p_list_node = SON_PNULL;
			p_node      = SON_PNULL;

			p_node = list_pop_node(p_pnding_cell_list);
			if(SON_PNULL !=  p_node)
			{
				p_list_node = YMEMBEROF(enb_config_cell_data_t,
						enb_cell_node, p_node);
				son_mem_free(p_list_node);
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return;
}
/*SPR_16835_END*/
