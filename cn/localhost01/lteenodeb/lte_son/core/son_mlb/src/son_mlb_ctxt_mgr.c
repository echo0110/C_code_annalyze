/******************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc . All Rights Reserved.
*
*******************************************************************************
*
* $$Id: son_mlb_ctxt_mgr.c $
*
*******************************************************************************
*
* File Description: This file contains the functions for managing MLB Module's
*        Global context and cell specific context.
*******************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Dec,2012         Mukesh                      Initial
*   Aug, 2014       Shilpi                      SPR 13251 Fix
******************************************************************************/
#include <son_mlb_ctxt_mgr.h>
#include <son_utils.h>

extern const son_8 *p_son_mlb_facility_name;

/*****************************************************************************
 *  Name  : MLB Global Pending Response.
 ****************************************************************************/
pending_res_t g_mlb_pending_response;
extern son_global_config_params_t g_config_params;

/* SPR 19015 Fix Start*/
extern son_bool_et son_anr_get_serving_enb_id (
        son_intra_rat_global_cell_id_t      cell_id,
        son_global_enb_id_t                 *src_enbid);
/* SPR 19015 Fix Stop*/

/* SON MLB FSM state names */
const son_s8 *SON_MLB_FSM_STATES_NAMES[] =
{
    (const son_s8 *)"SONMLB_STATE_INIT",
    (const son_s8 *)"SONMLB_STATE_ACTIVE",
    (const son_s8 *)"SONMLB_STATE_ENABLING",
    (const son_s8 *)"SONMLB_STATE_DISABLING",
    (const son_s8 *)"SONMLB_STATE_SHUTDOWN",
};

/* SON MLB Module FSM state names */
const son_s8 *SON_MLB_CELL_FSM_STATES_NAMES[] =
{
    (const son_s8 *)"SONMLB_CELL_STATE_DISABLING",
    (const son_s8 *)"SONMLB_CELL_STATE_DISABLED",
    (const son_s8 *)"SONMLB_CELL_STATE_ENABLING",
    (const son_s8 *)"SONMLB_CELL_STATE_ENABLED",
    (const son_s8 *)"SONMLB_CELL_STATE_UNDEFINED"
};

/* SON MLB Module Event  names */
const son_s8 *SON_MLB_CELL_FSM_EVENT_NAMES[] =
{
    (const son_s8 *)"CELL_SONMLB_ENABLE_REQ",
    (const son_s8 *)"CELL_SONMLB_DISABLE_REQ",
    (const son_s8 *)"CELL_SONMLB_MODIFY_REQ",
    (const son_s8 *)"CELL_RRM_REGISTER_RES",
    (const son_s8 *)"CELL_RRM_DEREGISTER_RES",
    (const son_s8 *)"CELL_RRM_SON_HO_REPORT",
    (const son_s8 *)"CELL_RRM_SON_LOAD_REPORT",
    (const son_s8 *)"CELL_SON_ANR_DISABLE_IND",
    (const son_s8 *)"CELL_SON_MLB_STATE_CHANGE_IND",
    (const son_s8 *)"CELL_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"CELL_MOBILITY_CHANGE_FAILURE",
    (const son_s8 *)"CELL_MOBILITY_CHANGE_ACK",
 /*Rel 3.0 Multi Sector code changes Start*/
    (const son_s8 *)"CELL_INTERNAL_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"CELL_INTERNAL_MOBILITY_CHANGE_FAILURE",
    (const son_s8 *)"CELL_INTERNAL_MOBILITY_CHANGE_ACK",
 /*Rel 3.0 Multi Sector code changes Stop*/
    (const son_s8 *)"CELL_SON_MLB_NBR_DELETE_IND",
    (const son_s8 *)"CELL_SON_MLB_OFFSET_CHANGE_REQ",
    (const son_s8 *)"CELL_SON_MLB_PEER_OFFSET_CHANGE_RESP",
    /* coverity 41591 fix start*/
    (const son_s8 *)"CELL_SON_EICIC_ENABLE_DISABLE_IND"
    /*coverity 41591 fix end*/
};

/*****************************************************************************
 * Function Name  : mlb_print_pending_res
 * Inputs         : son_method_name - A string to print
 * Outputs        : None
 * Returns        : None
 * Description    : Print pending_response structure
 ****************************************************************************/
static son_void_t
mlb_print_pending_res
(
    const son_8 *son_method_name
)
{
    SON_LIST_NODE           *p_node             = SON_PNULL;
    cell_status_list_data_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name, SON_INFO,
        "%s: Pending Response Structure:\n"
        "API Id: %d \n"
        "Transaction Id: %d \n"
        "Expected Response Count: %d\n"
        "Error Received Flag: %d",
        son_method_name,
        g_mlb_pending_response.api_id,
        g_mlb_pending_response.transaction_id,
        g_mlb_pending_response.expected_res_count,
        g_mlb_pending_response.is_error_received);
    p_node = get_first_list_node(&g_mlb_pending_response.cell_sts_list);

    while (SON_PNULL != p_node)
    {
        p_list_node =  YMEMBEROF( cell_status_list_data_t,
            cell_status_node,
            p_node);
        SON_LOG(g_mlb_context.log_enable,
            p_son_mlb_facility_name, SON_BRIEF,
            "Cell id is %x with error code = %d\n",
            son_convert_char_cell_id_to_int_cell_id(p_list_node->
                cell_status.cgi.cell_identity),
            p_list_node->cell_status.error_code);

        p_node = get_next_list_node(p_node);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_print_pending_res */

/*****************************************************************************
 * Function Name  : mlb_print_global_context
 * Inputs         : son_method_name - A string to print
 * Outputs        : None
 * Returns        : son_void_t
 * Description    :This function prints the MLB global context
 ******************************************************************************/
static son_void_t
mlb_print_global_context
(
    const son_8 *son_method_name
)
{
    mlb_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name, SON_INFO,
        "MLB Global Context Structure:\n"
        "MLB Current FSM State: %s \n"
        "MLB Global Mode: %d \n"
        "MLB Log Enable: %d\n"
        "enodeb type: %d\n"
        "Max handover sample:: %d\n"
        "Min cell offset:: %d\n"
        "Max cell offset:: %d\n"
        "threshold rsrp: %d\n"
        "small step size cell offset:: %d\n"
        "eutra_load_threshold_idle :: %d\n"
        "eutra_load_threshold_conn :: %d\n"
        "eutra_offset_min_step :: %d\n"
        "eutra_ocn_min_step :: %d\n"
        "No of Cell configured at MLB is %d\n",
        SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state],
        g_mlb_context.mlb_global_mode,
        g_mlb_context.log_enable,
        g_mlb_context.enb_type,
        g_mlb_context.mlb_config_param.max_ho_sample,
        g_mlb_context.mlb_config_param.q_offset_min,
        g_mlb_context.mlb_config_param.q_offset_max,
        g_mlb_context.mlb_config_param.eutra_threshold_rsrp,
        g_mlb_context.mlb_config_param.small_step_cell_offset,
        g_mlb_context.mlb_config_param.eutra_load_threshold_idle,
        g_mlb_context.mlb_config_param.eutra_load_threshold_conn,
        g_mlb_context.mlb_config_param.eutra_offset_min_step,
        g_mlb_context.mlb_config_param.eutra_ocn_min_step,
        g_mlb_context.cell_ctxt_list.count);
    p_node = mlb_get_first_cell_ctxt();
    while (SON_PNULL != p_node)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name, SON_INFO,
            "Cell id is %x with current state = %s son_method_name=%p\n",
            son_convert_char_cell_id_to_int_cell_id(p_node->data.cell_id.cell_identity),
            SON_MLB_CELL_FSM_STATES_NAMES[p_node->data.current_cell_fsm_state],
            son_method_name);
                    /*SPR 17777 +-*/

        p_node = mlb_get_next_node(p_node);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_print_global_context */

/*****************************************************************************
 * Function Name  : mlb_reset_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Resets pending_response structure
 ****************************************************************************/
son_void_t
mlb_reset_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    mlb_print_pending_res("mlb_reset_pending_resi: before reset");
    g_mlb_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
    g_mlb_pending_response.transaction_id = SON_UNDEFINED;
    g_mlb_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
    g_mlb_pending_response.is_error_received = SON_FALSE;
    mlb_clear_pending_res_list();
    list_init(&g_mlb_pending_response.cell_sts_list);
    g_mlb_pending_response.ts = 0;
    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
        SON_BRIEF,
        "mlb_reset_pending_res:Pending Response Reset\n");

    mlb_print_pending_res("mlb_reset_pending_resi: after reset");
    SON_UT_TRACE_EXIT();
} /* mlb_reset_pending_res */

/*****************************************************************************
 * Function Name  : mlb_init_pending_res
 * Inputs         : tid  -      Transaction Id
 *                : api  -      Procedure Code
 * Outputs        : None
 * Returns        : None
 * Description    : Initialize pending_response structure
 ****************************************************************************/
son_void_t
mlb_init_pending_res
(
    son_u16 tid,
    son_procedure_code_et api
)
{
    SON_UT_TRACE_ENTER();
    g_mlb_pending_response.api_id = api;
    g_mlb_pending_response.transaction_id = tid;
    g_mlb_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
    g_mlb_pending_response.is_error_received = SON_FALSE;
    list_init(&g_mlb_pending_response.cell_sts_list);
    son_time(&g_mlb_pending_response.ts);

    mlb_print_pending_res("mlb_init_pending_res");
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16
 * Description    : Returns transaction id of pending response
 ****************************************************************************/
son_u16
mlb_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_pending_response.transaction_id;
}

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u8
 * Description    : Returns count of awaited responses from pending response
 ****************************************************************************/
son_u8
mlb_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_pending_response.expected_res_count;
}

/*****************************************************************************
 * Function Name  : mlb_increment_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Increments expected response count in pending response
 ****************************************************************************/
son_void_t
mlb_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    if (g_mlb_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
    {
        g_mlb_pending_response.expected_res_count++;
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_increment_pending_res_expected_count:Max value of "
            "Expected Response Count in Pending Response reached\n");
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_decrement_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Decrements expected response count in pending response
 ****************************************************************************/
son_void_t
mlb_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    if (g_mlb_pending_response.expected_res_count > MIN_PENDING_RES_COUNT)
    {
        g_mlb_pending_response.expected_res_count--;
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_decrement_pending_res_expected_count:Min value of "
            "Expected Response Count in Pending Response reached\n");
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et
 * Description    : Returns API ID present in pending response
 ****************************************************************************/
son_procedure_code_et
mlb_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_pending_response.api_id;
}

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_cell_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type SON_LIST
 * Description    : Returns pointer to cell_status_list present in
 *                    pending response
 ****************************************************************************/
SON_LIST *
mlb_get_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return &g_mlb_pending_response.cell_sts_list;
}

/*****************************************************************************
 * Function Name  : mlb_set_pending_res_ts_with_current_timestamp
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set timestamp in pending response for current time
 ****************************************************************************/
son_void_t
mlb_set_pending_res_ts_with_current_timestamp
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    son_time(&g_mlb_pending_response.ts);
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_set_pending_res_error_rcvd
 * Inputs         : val - value to be updated in flag present in pending response
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set is_eror_received flag in pending response
 ****************************************************************************/
son_void_t
mlb_set_pending_res_error_rcvd
(
    son_bool_et val
)
{
    SON_UT_TRACE_ENTER();
    if (g_mlb_pending_response.is_error_received != val)
    {
        g_mlb_pending_response.is_error_received = val;
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_error_rcvd
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Returns is_eror_received flag present in pending response
 ****************************************************************************/
son_bool_et
mlb_get_pending_res_error_rcvd
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_pending_response.is_error_received;
}

/*****************************************************************************
 * Function Name  : mlb_find_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id - Pointer to Cell ID
 * Outputs        : None
 * Returns        : pointer of type cell_status_list_data_t
 * Description    : Find cell_status node in the cell_status_list
 ****************************************************************************/
cell_status_list_data_t *
mlb_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
)
{
    SON_LIST_NODE            *p_node             = SON_PNULL;
    cell_status_list_data_t *p_list_node         = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_rcvd_cell_id)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_find_cell_in_pending_res : Cell ID is NULL\n");

        SON_UT_TRACE_EXIT();
        return SON_PNULL;
    }

    p_node = list_find_node(&g_mlb_pending_response.cell_sts_list,
        (son_void_t *)p_rcvd_cell_id,
        son_key_of_mlb,
        son_compare_cell_id);

    if (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(cell_status_list_data_t,
            cell_status_node,
            p_node);
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_find_cell_in_pending_res : "
            "Cell ID %x not found in mlb context\n",
            son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* mlb_find_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : mlb_add_cell_in_pending_res
 * Inputs         : p_sts_node - Pointer to Cell Status Node
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Add cell_status node in the cell_status_list
 ****************************************************************************/
son_bool_et
mlb_add_cell_in_pending_res
(
    son_cell_status_t   *p_sts_node
)
{
    cell_status_list_data_t *p_list_node = SON_PNULL;
    son_bool_et ret_val      = SON_FALSE;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_sts_node &&
        g_mlb_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
    {
        p_list_node = son_mem_get(sizeof(cell_status_list_data_t));
        if (SON_PNULL != p_list_node)
        {
            SON_MEMCPY(&p_list_node->cell_status, p_sts_node,
                sizeof(son_cell_status_t));
            list_insert_node(&g_mlb_pending_response.cell_sts_list,
                &p_list_node->cell_status_node);
            mlb_increment_pending_res_expected_count();
            ret_val = SON_TRUE;
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_BRIEF,
                "mlb_add_cell_in_pending_res: Cell %x is added "
                "in Pending  Response\n",
                son_convert_char_cell_id_to_int_cell_id(p_list_node->
                    cell_status.cgi.cell_identity));

            mlb_print_pending_res("mlb_add_cell_in_pending_res");
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_ERROR,
                "mlb_add_cell_in_pending_res: Memory allocation "
                "failure\n");
        }
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_add_cell_in_pending_res:Invalid input parameters[p_sts_node is NULL] "
            "or pending response count reached max limit[%d] \n", MAX_PENDING_RES_COUNT);
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mlb_add_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : mlb_delete_cell_from_pending_res
 * Inputs         : p_cell_id -  Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete cell_status node from the cell_status_list
 ****************************************************************************/
son_void_t
mlb_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
)
{
    cell_status_list_data_t        *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_cell_id)
    {
        p_node = mlb_find_cell_in_pending_res(p_cell_id);
        if (SON_PNULL != p_node)
        {
            list_delete_node(&g_mlb_pending_response.cell_sts_list,
                &p_node->cell_status_node);
            son_mem_free(p_node);
            mlb_decrement_pending_res_expected_count();
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_BRIEF,
                "mlb_delete_cell_from_pending_res: "
                "Deleting Cell %x from Pending Response List\n",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

            mlb_print_pending_res("After deletion in "
                                  "mlb_delete_cell_from_pending_res");

            if (!mlb_get_pending_res_expected_count())
            {
                SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_BRIEF,
                    "mlb_delete_cell_from_pending_res: Expected Response count has reached 0"
                    " reseting Pending Response\n");
                mlb_reset_pending_res();
            }
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_ERROR,
                "mlb_delete_cell_from_pending_res:Node node found "
                "for cell %x\n",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
        }
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_delete_cell_from_pending_res: Cell ID is NULL\n");
    }

    SON_UT_TRACE_EXIT();
} /* mlb_delete_cell_from_pending_res */

/*****************************************************************************
 * Function Name  : mlb_clear_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all cell_status node from the cell_status_list
 ****************************************************************************/
son_void_t
mlb_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE            *p_node         = SON_PNULL;
    cell_status_list_data_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);

    while (SON_PNULL != p_node)
    {
        p_list_node =  YMEMBEROF(cell_status_list_data_t,
            cell_status_node, p_node);
        son_mem_free(p_list_node);
        p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);
    }

    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
        SON_DETAILED,
        "mlb_clear_pending_res_list: Pending Response list reset\n");

    mlb_print_pending_res("mlb_clear_pending_res_list");
    SON_UT_TRACE_EXIT();
} /* mlb_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : mlb_update_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id -  Pointer to Cell ID
 *                : err_code   -      error_code
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Update cell_status node in the cell_status_list
 *                    for cell id, error code and result.
 ****************************************************************************/
son_bool_et
mlb_update_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
    son_error_et err_code
)
{
    cell_status_list_data_t *p_list_node = SON_PNULL;
    son_bool_et ret_val = SON_FALSE;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_rcvd_cell_id)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_update_cell_in_pending_res:Cell ID is NULL\n");
        return ret_val;
    }

    p_list_node = mlb_find_cell_in_pending_res(p_rcvd_cell_id);

    if (p_list_node)
    {
        p_list_node->cell_status.error_code = err_code;
        ret_val = SON_TRUE;
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_BRIEF,
            "mlb_update_cell_in_pending_res: Pending Response List "
            " context updated for cell %x with error"
            " code = %d\n",
            son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity), err_code);

        mlb_print_pending_res("mlb_update_cell_in_pending_res");
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_update_cell_in_pending_res: Cell %x not found\n",
            son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mlb_update_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : mlb_construct_cell_arr_from_pending_res_list
 * Inputs         : cell_fsm_state  -    Cell FSM state to be set
 * Outputs        : p_cell_sts_arr  -    pointer to cell status array
 *                  p_cell_sts_arr_size - Pointer to cell status array size
 * Returns        : None
 * Description    : Construct cell status array from pending response list
 ****************************************************************************/
son_void_t
mlb_construct_cell_arr_from_pending_res_list
(
    son_u16                     *p_cell_sts_arr_size,
    son_cell_status_t             *p_cell_sts_arr,
    mlb_cell_fsm_state_et cell_fsm_state
)
{
    SON_LIST_NODE            *p_node             = SON_PNULL;
    cell_status_list_data_t *p_list_node     = SON_PNULL;
    mlb_cell_context_node_t *p_cell_data = SON_PNULL;
    SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_sts_arr ||
        SON_PNULL == p_cell_sts_arr_size)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_construct_cell_arr_from_pending_res_list: Could not "
            "construct cell array from pending response list, "
            "p_cell_sts_arr or p_cell_sts_arr_size is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);
    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
        SON_DETAILED,
        "mlb_construct_cell_arr_from_pending_res_list: "
        "Constructing Cell Array from pending Response");

    mlb_print_pending_res("mlb_construct_cell_arr_from_pending_res_list");

    while (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
            p_node);

        /* If no error occured for a cell then just remove its entry
         * pending list otherwise populate the error entry in the cell
         * array and then remove entry from pending list. */
        if (SON_NO_ERROR != p_list_node->cell_status.error_code)
        {
            SON_MEMCPY(&p_cell_sts_arr[*p_cell_sts_arr_size],
                &p_list_node->cell_status,
                sizeof(son_cell_status_t));
            (*p_cell_sts_arr_size)++;
            if (MLB_CELL_STATE_DISABLED == cell_fsm_state ||
                MLB_CELL_STATE_ENABLED == cell_fsm_state)
            {
                /* Change cell FSM state to cell_fsm_state
                 * (either MLB_CELL_STATE_DISABLED or MLB_CELL_STATE_ENABLED
                 * based on the registration/deregistration response awaited
                 * respectively) */
                p_cell_list_node = list_find_node(
                    &g_mlb_context.cell_ctxt_list,
                    &p_list_node->cell_status.cgi, son_key_of_mlb,
                    son_compare_cell_id);

                if (SON_PNULL != p_cell_list_node)
                {
                    p_cell_data = YMEMBEROF(mlb_cell_context_node_t, sNode,
                        p_cell_list_node);
                    if (SON_PNULL != p_cell_data)
                    {
                        SONMLB_CELL_FSM_SET_STATE(
                            (&p_cell_data->data),
                            p_cell_data->data.previous_cell_fsm_state,
                            p_cell_data->data.current_cell_fsm_state,
                            cell_fsm_state);
                    }
                }
            }
        }

        son_mem_free(p_list_node);
        p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_construct_cell_arr_from_pending_res_list */

/****************************************************************************
 * Function Name  : son_key_of_mlb
 * Inputs         : p_node -  Pointer to cell specific context node
 * Outputs        : None
 * Returns        : Pointer of type son_void_t
 * Description    : Returns cell node key
 *****************************************************************************/
const son_void_t *
son_key_of_mlb
(
    const SON_LIST_NODE *p_node
)
{
    mlb_cell_context_node_t *p_data = SON_PNULL;
    son_intra_rat_global_cell_id_t  *p_cell_id = SON_PNULL;

    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_node);

    p_data = YMEMBEROF(mlb_cell_context_node_t, sNode, p_node);
    p_cell_id = &(p_data->data.cell_id);

    SON_UT_TRACE_EXIT();
    return (son_void_t *)p_cell_id;
}
/*SPR_19279_START */
const son_void_t *
son_cell_index_key_of_mlb
(
 const SON_LIST_NODE *p_node
 )
{
	mlb_cell_context_node_t *p_data = SON_PNULL;
	son_u8  *p_cell_idx = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(mlb_cell_context_node_t, sNode, p_node);
	p_cell_idx = &(p_data->data.cell_index);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_idx;
}
/*SPR_19279_END */


/*****************************************************************************
 * Function Name  : mlb_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize MLB global Context
 ******************************************************************************/
son_void_t
mlb_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    g_mlb_context.current_fsm_state = MLB_STATE_INIT;
    g_mlb_context.mlb_global_mode = SON_DISABLED;
    g_mlb_context.log_enable = SON_OAM_LOG_ON;
    g_mlb_context.enb_type = SON_ENB_TYPE_UNDEFINED;
    g_mlb_context.mlb_prending_res_timer_id = SON_NULL;

    son_mlb_fill_mlb_param_default_values(&g_mlb_context.mlb_config_param);

    SON_MEMCPY(&g_mlb_context.mlb_config_param, &g_config_params.mlb_config_params,
        sizeof (son_mlb_config_param_t));

    /* SPR-13251 Fix Starts */
    g_mlb_context.log_level = SON_OAM_LOG_LEVEL_BRIEF;
    /* SPR-13251 Fix Ends */
    SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);
    list_init(&g_mlb_context.cell_ctxt_list);
    SON_LOG(SON_OAM_LOG_ON, p_son_mlb_facility_name, SON_BRIEF,
        "MLB Global Context Initialized\n");

    mlb_print_global_context("mlb_init_global_context");
    SON_UT_TRACE_EXIT();
} /* mlb_init_global_context */

/*****************************************************************************
 * Function Name  : mlb_init_cell_context
 * Inputs         : p_ctxt -  Pointer to cell context
 * Outputs        : p_ctxt
 * Returns        : son_void_t
 * Description    : Initialize MLB cell specific Context
 ******************************************************************************/
son_void_t
mlb_init_cell_context
(
    mlb_cell_context_t *p_ctxt
)
{
    SON_UT_TRACE_ENTER();
    if (p_ctxt != SON_PNULL)
    {
        SON_MEMSET(&p_ctxt->cell_id, 0,
            sizeof(son_intra_rat_global_cell_id_t));
        p_ctxt->current_cell_fsm_state = MLB_CELL_STATE_DISABLED;
        p_ctxt->previous_cell_fsm_state = MLB_CELL_STATE_DISABLED;
        p_ctxt->current_cell_state = RRM_CELL_OPERATIONAL;
        p_ctxt->eutra_q_offset_config.q_offset_min = g_mlb_context.
                                                     mlb_config_param.q_offset_min;
        p_ctxt->eutra_q_offset_config.q_offset_max =
            g_mlb_context.mlb_config_param.q_offset_max;
        p_ctxt->strongest_nbr_rsrp = 0;
        p_ctxt->eutra_threshold_rsrp = g_mlb_context.mlb_config_param.
                                       eutra_threshold_rsrp;
        p_ctxt->small_step_cell_offset = g_mlb_context.mlb_config_param.
                                         small_step_cell_offset;
        p_ctxt->eutra_threshold_idle = g_mlb_context.mlb_config_param.
                                       eutra_load_threshold_idle;
        p_ctxt->eutra_threshold_connected = g_mlb_context.mlb_config_param.
                                            eutra_load_threshold_conn;
        p_ctxt->dereg_invoking_api = SON_PROCEDURE_UNDEFINED;
        p_ctxt->count_for_neigh_cell = 0;
        SON_MEMSET(p_ctxt->csg_identity, 0, 
                SON_CSG_ID_OCTET_SIZE * sizeof(son_u8));
        p_ctxt->src_csg_id_present = 0;
        p_ctxt->delta_offset_idle = 0;
        p_ctxt->delta_offset_connected = 0;
        p_ctxt->eutra_threshold = 0;
        p_ctxt->serving_rsrp = 0;
        /* SPR 22195 Fix Start */
        /* Code Removed */
        /* SPR 22195 Fix Stop */
        p_ctxt->min_abs_usage_threshold_victim = 0;
        p_ctxt->max_abs_usage_threshold_victim = 0;
        /* bug_13866_end */
        /* Valgrind fix 04 Dec 2014 start*/
        p_ctxt->mlb_start_flag = SON_FALSE;
        /* Valgrind fix 04 Dec 2014 end*/

        SON_MEMSET(&p_ctxt->serv_cell_load_info, 0,
            sizeof(rrm_son_cell_load_info_t));

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
            "Cell Context Initialized \n");
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_init_cell_context: Input Cell Ctxt Param is NULL !!!\n");
    }

    SON_UT_TRACE_EXIT();
} /* mlb_init_cell_context */

/*****************************************************************************
 * Function Name  : mlb_get_context_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32
 * Description    : Returns the count of the cell ctxt node present in
 *                  cell ctxt list
 ******************************************************************************/
son_u32
mlb_get_context_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return list_count(&g_mlb_context.cell_ctxt_list);
}

/*****************************************************************************
 * Function Name  : mlb_get_cell_ctxt_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type SON_LIST
 * Description    : Returns cell context list
 ******************************************************************************/
SON_LIST *
mlb_get_cell_ctxt_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return &g_mlb_context.cell_ctxt_list;
}

/*****************************************************************************
 * Function Name  : mlb_get_cell_context_from_global_context
 * Inputs         : p_cell_id -  Pointer to the cell Id
 * Outputs        : None
 * Returns        : pointer of type mlb_cell_context_t
 * Description    : Returns cell context for cell id passed as an argument
 ******************************************************************************/
mlb_cell_context_t *
mlb_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
)
{
    mlb_cell_context_node_t *p_data      = SON_PNULL;
    SON_LIST_NODE         *p_list_node     = SON_PNULL;
    mlb_cell_context_t    *p_retval        = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_id)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_get_cell_context_from_global_context: Cell ID is NULL\n");

        SON_UT_TRACE_EXIT();
        return p_retval;
    }

    p_list_node = list_find_node(&g_mlb_context.cell_ctxt_list,
        p_cell_id, son_key_of_mlb, son_compare_cell_id);

    if (SON_NULL != p_list_node)
    {
        p_data = (mlb_cell_context_node_t *) YMEMBEROF
                     (mlb_cell_context_node_t, sNode, p_list_node);
        p_retval = &p_data->data;
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_get_cell_context_from_global_context: "
            "Cell %x not found\n",
            son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return p_retval;
} /* mlb_get_cell_context_from_global_context */

/*****************************************************************************
 * Function Name  : mlb_get_context_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : mlb_fsm_state_et
 * Description    : Returns MLB module's Global state
 ******************************************************************************/
mlb_fsm_state_et
mlb_get_context_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();

    return g_mlb_context.current_fsm_state;
}

/*****************************************************************************
 * Function Name  : mlb_set_context_state
 * Inputs         : state -   MLB module's Global state
 * Outputs        : None
 * Returns        : None
 * Description    : Set MLB module's Global state
 ******************************************************************************/
son_void_t
mlb_set_context_state
(
    mlb_fsm_state_et state
)
{
    SON_UT_TRACE_ENTER();
    if (g_mlb_context.current_fsm_state != state)
    {
        SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state, state);
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_set_log_mode
 * Inputs         : log_mode -  log mode for MLB module
 * Outputs        : None
 * Returns        : None
 * Description    : set log mode
 ******************************************************************************/
son_void_t
mlb_set_log_mode
(
    son_oam_log_on_off_et log_mode
)
{
    SON_UT_TRACE_ENTER();
    g_mlb_context.log_enable = log_mode;
    SON_UT_TRACE_EXIT();
}

/*SPR-11554-Start-Functions modified*/

/*****************************************************************************
 * Function Name  : mlb_validate_cell_data
 * Inputs         : p_cell    - Pointer to the cell data
 * Outputs        : None
 * Returns        : SON_TRUE: If valid
 *                  SON_FALSE:If not valid
 * Description    : This function validates the data of cell received in cell
 *                  info indication
 ******************************************************************************/
static son_bool_et
mlb_validate_cell_data
(
    son_cell_t  *p_cell
)
{
    son_bool_et is_valid = SON_TRUE;
    /* SPR 19015 Fix Start */
    son_global_enb_id_t       src_enbid;
    /* SPR 19015 Fix Stop */
    son_u16                      temp_open_pci_end = 0;
    son_u16                      temp_closed_pci_end = 0;
    son_u16                      temp_hybrid_pci_end = 0;
    son_bool_et is_home_eNB = SON_FALSE;

    SON_UT_TRACE_ENTER();

    /*Check if the pci range received is valid or not.Also check whether eNB is to be configured as Home or Macro*/

    if(p_cell->bitmask & SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT)
    {
        temp_open_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_open_pci_range.pci_range);
        if((temp_open_pci_end == 0) || ((p_cell->intra_freq_open_pci_range.pci_start + temp_open_pci_end -1) > 503))
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_ERROR,
                    "mlb_validate_cell_data:"
                    "Invalid pci range received. open_pci_start[%d], open_pci_end[%d]\n",
                    p_cell->intra_freq_open_pci_range.pci_start, temp_open_pci_end);

            is_valid = SON_FALSE;
            SON_UT_TRACE_EXIT();
            return is_valid;
        }

        if((p_cell->pci >=  p_cell->intra_freq_open_pci_range.pci_start) &&
                (p_cell->pci <= (p_cell->intra_freq_open_pci_range.pci_start + temp_open_pci_end -1)))
        {
            is_home_eNB = SON_TRUE;
        }
    }

    if(p_cell->bitmask & SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT)
    {
        temp_closed_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_closed_pci_range.pci_range);
        if((temp_closed_pci_end == 0) || ((p_cell->intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1) > 503))
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_ERROR,
                    "mlb_validate_cell_data:"
                    "Invalid pci range received. closed_pci_start[%d], closed_pci_end[%d]\n",
                    p_cell->intra_freq_closed_pci_range.pci_start, temp_closed_pci_end);

            is_valid = SON_FALSE;
            SON_UT_TRACE_EXIT();
            return is_valid;
        }

        if((p_cell->pci >=  p_cell->intra_freq_closed_pci_range.pci_start) &&
                (p_cell->pci <= (p_cell->intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1)))
        {
            is_home_eNB = SON_TRUE;
        }
    }

    if(p_cell->bitmask & SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT)
    {
        temp_hybrid_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_hybrid_pci_range.pci_range);
        if((temp_hybrid_pci_end == 0) || ((p_cell->intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1) > 503))
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_ERROR,
                    "mlb_validate_cell_data:"
                    "Invalid pci range received. hybrid_pci_start[%d], hybrid_pci_end[%d]\n",
                    p_cell->intra_freq_hybrid_pci_range.pci_start, temp_hybrid_pci_end);

            is_valid = SON_FALSE;
            SON_UT_TRACE_EXIT();
            return is_valid;
        }

        if((p_cell->pci >=  p_cell->intra_freq_hybrid_pci_range.pci_start) &&
                (p_cell->pci <= (p_cell->intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1)))
        {
            is_home_eNB = SON_TRUE;
        }
    }

    /* Check if it is the first cell being configured at MLB */
    if (0 == g_mlb_context.cell_ctxt_list.count)
    {
        /* If serving cell's csg identity is present then it
         * represents that the solution is running on a
         * home eNB, otherwise it is Macro eNB */
        /* SPR 19015 Fix Start */
        son_anr_get_serving_enb_id(p_cell->src_cgi,&src_enbid);
        g_mlb_context.enb_type = src_enbid.enb_type;

            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_DETAILED,
                "mlb_validate_cell_data: The serving eNB "
                "is 1:Home 2:Macro g_mlb_context.enb_type [%d]\n",g_mlb_context.enb_type);
        /* SPR 19015 Fix Stop */
    }

    /* Check if 1 cell is already configured and if the eNB is
     * home eNB do not configure any other cell as home eNB can
     * have only one serving cell */
    else if (SON_HOME_ENB == g_mlb_context.enb_type)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_BRIEF,
            "mlb_validate_cell_data: Cell [Cell Id: 0x%x]"
            " is not added to MLB Global Context - home eNB is"
            " already configured with 1 serving cell",
            son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
        is_valid = SON_FALSE;
    }
    else if ((SON_MACRO_ENB == g_mlb_context.enb_type) &&
             (is_home_eNB == SON_TRUE))
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_validate_cell_data: Cell [Cell Id: 0x%x]"
            " is not added to MLB Global Context - macro eNB "
            " eNB is already configured as macro,cannot configure femto cell",
            son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
        is_valid = SON_FALSE;
    }

    SON_UT_TRACE_EXIT();
    return is_valid;
} /* mlb_validate_cell_data */

/*****************************************************************************
 * Function Name  :mlb_add_new_cell_in_context
 * Inputs         : p_cell   - Pointer to the cell data
 * Outputs        : p_cause - Cause of failure if failure occurs in addition
 *                  p_state - MLB feature state for the cell
 * Returns        : Pointer to the new node added in the list, NULL if node
 *                  already exists or creation of node fails
 * Description    : This is used to add a new context in MLB global
 *                  context if not already exists.
 ******************************************************************************/
mlb_cell_context_node_t *
mlb_add_new_cell_in_context
(
    son_cell_t                          *p_cell,
    son_feature_state_et                *p_state,
    /*SPR_19279_START */
    son_feature_state_change_cause_et   *p_cause,
    son_u8 cell_index
    /*SPR_19279_END */
)
{
    mlb_cell_context_node_t *p_new_node     = SON_PNULL;
    SON_LIST_NODE             *p_list_node     = SON_PNULL;
    mlb_cell_context_node_t *p_data = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell || SON_PNULL == p_cause ||
        SON_PNULL == p_state)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_add_new_cell_in_context: Cell Data or pointer to cause "
            "or state  is is NULL\n");
    }
    else
    {
        /*SPR_19279_START */
        p_list_node = list_find_node(&g_mlb_context.cell_ctxt_list,
            &cell_index, son_cell_index_key_of_mlb, son_compare_cell_index);
        /*SPR_19279_END */

        if (SON_PNULL == p_list_node)
        {
            if (g_mlb_context.cell_ctxt_list.count <
                SON_MAX_CELL_SUPPORTED_AT_ENB)
            {
                if (SON_FALSE == mlb_validate_cell_data(p_cell))
                {
                    *p_state = SON_STATE_DELETED;
                    *p_cause = SON_INTERNAL_FAILURE;
                    SON_UT_TRACE_EXIT();
                    return p_new_node;
                }

                /* Create a new node */
                p_new_node = (mlb_cell_context_node_t *)son_mem_get(
                    sizeof(mlb_cell_context_node_t));

                if (SON_PNULL != p_new_node)
                {
                    /*Populate the node*/
                    mlb_init_cell_context(&p_new_node->data);
                    /*SPR_19279_START */
                    p_new_node->data.cell_index = cell_index;
                    /*SPR_19279_END */
                    SON_MEMCPY(&p_new_node->data.cell_id, &p_cell->src_cgi,
                        sizeof(son_intra_rat_global_cell_id_t));
                    /** eICIC changes start */
                    if(p_cell->bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT)
                    {
                        /*+ eICIC review commnents incorporated +*/
                        if(p_cell->abs_usage_threshold.bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT)
                        /*- eICIC review commnents incorporated -*/
                        {
                           /* bug_13866_start */
                            p_new_node->data.min_abs_usage_threshold_victim = 
                                p_cell->abs_usage_threshold.min_abs_usage_threshold;
                        }
                        else
                        {
                            p_new_node->data.min_abs_usage_threshold_victim = SON_CELL_DEFAULT_MIN_ABS_THRESHOLD;
                        }

                        /*+ eICIC review commnents incorporated +*/
                        if(p_cell->abs_usage_threshold.bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT)
                        /*- eICIC review commnents incorporated -*/
                        {
                            p_new_node->data.max_abs_usage_threshold_victim = 
                                p_cell->abs_usage_threshold.max_abs_usage_threshold;
                        }
                        else
                        {
                            p_new_node->data.max_abs_usage_threshold_victim = SON_CELL_DEFAULT_MAX_ABS_THRESHOLD;
                        }
                    	SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                        SON_BRIEF,
                        "Cell Id %x added in"
                        " MLB Cell Context List\n"
			"min_abs_usage_threshold_victim[%d]"
			"max_abs_usage_threshold_victim[%d]",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
			p_new_node->data.min_abs_usage_threshold_victim,
			p_new_node->data.max_abs_usage_threshold_victim);
                      /* bug_13866_start */
                    }
                    /** eICIC changes end */
                    MLB_NBR_Q_INIT(&p_new_node->data.neigh_cell_info_head);
                    /*Insert it in context list */
                    list_insert_node(&g_mlb_context.cell_ctxt_list,
                        &p_new_node->sNode);
                    *p_state = SON_STATE_DISABLED;
                    *p_cause = SON_MLB_MODE_DISABLED;
                    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                        SON_BRIEF,
                        "Cell Id %x added in"
                        " MLB Cell Context List\n",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
                    mlb_print_global_context("mlb_add_new_cell_in_context");
                }
                else
                {
                    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                        SON_ERROR,
                        "mlb_add_new_cell_in_context:Memory "
                        "allocation failure\n");
                    *p_state = SON_STATE_DELETED;
                    *p_cause = SON_MEM_ALLOC_FAILURE;
                }
            }
            else
            {
                SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_ERROR,
                    "mlb_add_new_cell_in_context: Cell [Cell Id: %d]"
                    " could not be added to MLB Global Context - "
                    "Maximum(%d)) number of supported cells limit "
                    "reached\n",
                    son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
                    g_mlb_context.cell_ctxt_list.count);
                *p_state = SON_STATE_DELETED;
                *p_cause = SON_MAX_SUPPORTED_CELL_LIMIT_REACHED;
            }
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_ERROR,
                "mlb_add_new_cell_in_context:Cell %x already exists"
                " in MLB Cell Context List\n",
                son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

            p_data = YMEMBEROF(mlb_cell_context_node_t, sNode, p_list_node);
            /*SPR_19279_START*/
            if (SON_MEMCMP(&p_data->data.cell_id,
                        &p_cell->src_cgi, sizeof(p_new_node->data.cell_id)))
            {
                SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                        SON_BRIEF,
                        "ECGI for cell [cell "
                        "id: 0x%x] updated\n",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

                SON_MEMCPY(&p_data->data.cell_id,
                        &p_cell->src_cgi,
                        sizeof(p_data->data.cell_id));
            }
            /*SPR_19279_END*/

	    /* 14386 SPR fix start */
	    /** eICIC changes start */
                    if(p_cell->bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT)
                    {
                        /*+ eICIC review commnents incorporated +*/
                        if(p_cell->abs_usage_threshold.bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT)
                        /*- eICIC review commnents incorporated -*/
                        {
                           /* bug_13866_start */
                            p_data->data.min_abs_usage_threshold_victim =
                                p_cell->abs_usage_threshold.min_abs_usage_threshold;
                        }

                        /*+ eICIC review commnents incorporated +*/
                        if(p_cell->abs_usage_threshold.bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT)
                        /*- eICIC review commnents incorporated -*/
                        {
                            p_data->data.max_abs_usage_threshold_victim =
                                p_cell->abs_usage_threshold.max_abs_usage_threshold;
                        }
                      /* bug_13866_start */
                    	SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                        SON_BRIEF,
                        "mlb_add_new_cell_in_context: Cell %x added in"
                        " MLB Cell Context List\n"
			"min_abs_usage_threshold_victim[%d]"
			"max_abs_usage_threshold_victim[%d]",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
			p_data->data.min_abs_usage_threshold_victim,
			p_data->data.max_abs_usage_threshold_victim);
                    }
                    /** eICIC changes end */
	    /* 14386 SPR fix end */


            if (MLB_CELL_STATE_DISABLED == p_data->data.current_cell_fsm_state)
            {
                *p_state = SON_STATE_DISABLED;
            }
            else if (MLB_CELL_STATE_ENABLED ==
                     p_data->data.current_cell_fsm_state)
            {
                *p_state = SON_STATE_ENABLED;
            }
            else if (MLB_CELL_STATE_ENABLING ==
                     p_data->data.current_cell_fsm_state)
            {
                *p_state = SON_STATE_DISABLED;
            }
            else if (MLB_CELL_STATE_DISABLING ==
                     p_data->data.current_cell_fsm_state)
            {
                *p_state = SON_STATE_ENABLED;
            }

            *p_cause = SON_CELL_ALREADY_EXISTS;
        }
    }

    SON_UT_TRACE_EXIT();
    return p_new_node;
} /* mlb_add_new_cell_in_context */

/*SPR-11554-End*/

/*****************************************************************************
 * Function Name  : mlb_clear_context_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Clear MLB context list
 ******************************************************************************/
son_void_t
mlb_clear_context_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE             *p_node     = SON_PNULL;
    mlb_cell_context_node_t *p_data     = SON_PNULL;
    son_mlb_neigh_cell_info_record_t *p_neigh_cell_record = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_node = list_pop_node(&g_mlb_context.cell_ctxt_list);
    while (SON_PNULL != p_node)
    {
        p_data = (mlb_cell_context_node_t *)
                 YMEMBEROF(mlb_cell_context_node_t, sNode, p_node);
        /* Free the memory occupied by the node */
        if (p_data != SON_PNULL)
        {
            while (MLB_NBR_Q_GET_COUNT(&p_data->data.neigh_cell_info_head))
            {
                MLB_NBR_Q_DEQUEUE(&p_data->data.neigh_cell_info_head, p_neigh_cell_record);
                if (p_neigh_cell_record != SON_PNULL)
                {
                    son_mem_free(p_neigh_cell_record);
                    p_neigh_cell_record = SON_PNULL;
                }
            }

            son_mem_free(p_data);
            p_node = list_pop_node(&g_mlb_context.cell_ctxt_list);
        }
    }

    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
        SON_BRIEF,
        "mlb_clear_context_list: MLB Context List Cleared\n");

    SON_UT_TRACE_EXIT();
} /* mlb_clear_context_list */

/*****************************************************************************
 * Function Name  : mlb_delete_cell_from_context
 * Inputs         : p_cell_id  - Pointer to the cell Id
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : delete node from context list based on cell id passed as
 *                    argument
 ******************************************************************************/
son_bool_et
mlb_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
)
{
    son_bool_et is_node_deleted = SON_FALSE;
    SON_LIST_NODE             *p_list_node     = SON_PNULL;
    mlb_cell_context_node_t *p_data         = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_cell_id)
    {
        p_list_node = list_find_node(&g_mlb_context.cell_ctxt_list,
            p_cell_id,
            son_key_of_mlb, son_compare_cell_id);
        if (SON_PNULL != p_list_node)
        {
            p_data = (mlb_cell_context_node_t *)YMEMBEROF
                         (mlb_cell_context_node_t, sNode, p_list_node);
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_BRIEF,
                "mlb_delete_cell_from_context: Cell %x  deleted"
                " from MLB Cell Context List\n",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
            list_delete_node(&g_mlb_context.cell_ctxt_list,
                p_list_node);

            /* Free the memory occupied by the node */
            son_mem_free(p_data);
            is_node_deleted = SON_TRUE;
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_ERROR,
                "mlb_delete_cell_from_context: Cell %x not found\n",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
        }

        mlb_print_global_context("mlb_delete_cell_from_context");
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_delete_cell_from_context: Cell ID is NULL\n");
    }

    SON_UT_TRACE_EXIT();
    return is_node_deleted;
} /* mlb_delete_cell_from_context */

/*****************************************************************************
 * Function Name  : mlb_get_global_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_app_mode_et
 * Description    : Returns mlb global mode(enabled/disabled)
 ******************************************************************************/
son_app_mode_et
mlb_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_context.mlb_global_mode;
}

/*****************************************************************************
 * Function Name  : mlb_set_global_mode
 * Inputs         : mode -  son mlb mode
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set mlb global mode(enabled/disabled)
 ******************************************************************************/
son_void_t
mlb_set_global_mode
(
    son_app_mode_et mode
)
{
    SON_UT_TRACE_ENTER();
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
        "mlb_set_global_mode: MLB global mode set to %u\n", mode);
    g_mlb_context.mlb_global_mode = mode;
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mlb_get_log_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_oam_log_on_off_et
 * Description    : Returns log mode(on/off)
 ******************************************************************************/
son_oam_log_on_off_et
mlb_get_log_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_context.log_enable;
}

/*****************************************************************************
 * Function Name  : mlb_get_first_cell_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type mlb_cell_context_node_t
 * Description    : Returns first context node from MLB context list
 ******************************************************************************/
mlb_cell_context_node_t *
mlb_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE    *p_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (list_count(&g_mlb_context.cell_ctxt_list) > 0)
    {
        p_node = get_first_list_node(&g_mlb_context.cell_ctxt_list);

        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (mlb_cell_context_node_t *)
                   YMEMBEROF(mlb_cell_context_node_t, sNode, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* mlb_get_first_cell_ctxt */

/*****************************************************************************
 * Function Name  : mlb_get_next_node
 * Inputs         : p_node - Pointer to context node
 * Outputs        : None
 * Returns        : pointer of type mlb_cell_context_node_t
 * Description    : Return next context node in context list of the node passed
 *                  as an argument
 ******************************************************************************/
mlb_cell_context_node_t *
mlb_get_next_node
(
    mlb_cell_context_node_t *p_node
)
{
    SON_LIST_NODE    *p_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->sNode);
        if (SON_PNULL != p_list_node)
        {
            return (mlb_cell_context_node_t *)YMEMBEROF
                       (mlb_cell_context_node_t, sNode, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* mlb_get_next_node */

/*****************************************************************************
 * Function Name  : mlb_get_pending_res_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32
 * Description    : Returns the count of ctxt nodes present in pending response
 ******************************************************************************/
son_u32
mlb_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return list_count(&g_mlb_pending_response.cell_sts_list);
}

/*****************************************************************************
 * Function Name  : mlb_delete_succesful_cells_from_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete those cell_status nodes from cell_status_list
 *                  which are succefully registered
 ****************************************************************************/
son_void_t
mlb_delete_succesful_cells_from_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE            *p_node         = SON_PNULL;
    SON_LIST_NODE           *p_next_node = SON_PNULL;
    cell_status_list_data_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    p_node = get_first_list_node(&g_mlb_pending_response.cell_sts_list);

    while (SON_PNULL != p_node)
    {
        p_list_node =  YMEMBEROF( cell_status_list_data_t,
            cell_status_node,
            p_node);
        if (SON_NO_ERROR == p_list_node->cell_status.error_code)
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_BRIEF,
                "mlb_delete_succesful_cells_from_pending_res: "
                "Cell id %x  with result=SON_SUCCESS is succesfully"
                " deleted from mlb pending response\n",
                son_convert_char_cell_id_to_int_cell_id(p_list_node->
                    cell_status.cgi.cell_identity));
            p_next_node = get_next_list_node(p_node);
            list_delete_node(&g_mlb_pending_response.cell_sts_list, p_node);
            son_mem_free(p_list_node);
            p_node = p_next_node;
            p_next_node = SON_PNULL;
        }
        else
        {
            p_node = get_next_list_node(p_node);
        }
    }

    mlb_print_pending_res("after mlb_delete_succesful_cells_from_pending_res");
    SON_UT_TRACE_EXIT();
} /* mlb_delete_succesful_cells_from_pending_res */

/*****************************************************************************
 * Function Name  : mlb_cell_info_ind_timer_handler
 * Outputs        : None
 * Returns        : None
 * Description    : This the funciton to clean up pending response strucutre
 *                  and update cell state for the cells on expirty of expected
 *                  registeration response timer. This is used when the
 *                  response awaited were trigerred due to cell info indication
 *                  requrest received from OAM.
 ****************************************************************************/
static son_void_t
mlb_cell_info_ind_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE            *p_node             = SON_PNULL;
    cell_status_list_data_t *p_list_node     = SON_PNULL;
    mlb_cell_context_node_t *p_cell_data = SON_PNULL;
    SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    mlb_print_pending_res("mlb_cell_info_ind_timer_handler");
    p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);

    while (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
            p_node);
        /* Find the pointer to MLB cell context for this node */
        p_cell_list_node = list_find_node(&g_mlb_context.cell_ctxt_list,
            &p_list_node->cell_status.cgi, son_key_of_mlb,
            son_compare_cell_id);

        if (SON_PNULL != p_cell_list_node)
        {
            p_cell_data = YMEMBEROF(mlb_cell_context_node_t, sNode,
                p_cell_list_node);
            if (SON_PNULL != p_cell_data)
            {
                /* Change cell FSM state to MLB_CELL_STATE_DISABLED */
                SONMLB_CELL_FSM_SET_STATE(
                    (&p_cell_data->data),
                    p_cell_data->data.previous_cell_fsm_state,
                    p_cell_data->data.current_cell_fsm_state,
                    MLB_CELL_STATE_DISABLED);
            }
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_WARNING,
                "mlb_cell_info_ind_timer_handler: Could not find MLB"
                " cell context for the cell [Cell Id: 0x%x] present in"
                " pending response list",
                son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_status.cgi.
                    cell_identity));
        }

        /* Send feature state change indication to SMIF */
        mlb_send_feature_state_change_ind(&p_list_node->cell_status.cgi,
            SON_STATE_DISABLED, SON_RRM_REGISTRATION_UNSUCCESSFUL);
        son_mem_free(p_list_node);
        p_node = list_pop_node(&g_mlb_pending_response.cell_sts_list);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_cell_info_ind_timer_handler */

/*****************************************************************************
 * Function Name  : mlb_pending_res_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This function cleans up Pending Response Structure and sends
 *                : pending response to OAM
 ****************************************************************************/
son_void_t
mlb_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    if (MIN_PENDING_RES_COUNT < mlb_get_pending_res_cell_count())
    {
        api = mlb_get_pending_res_api_id();
        if (SONMLB_ENABLE_RES == api)
        {
            son_mlb_enable_res_t res;
            son_size_t enable_res_len = sizeof(son_mlb_enable_res_t);
            SON_MEMSET(&res, 0, enable_res_len);
            res.transaction_id = mlb_get_pending_res_transaction_id();
            res.enable_res.error_code = SON_NO_ERROR;

            /* Remove Successful cells from the MLB_Pending_Response_LIST and
             *  create cell array with entry for each cell for which MLB could
             *  not be enabled. */
            mlb_construct_cell_arr_from_pending_res_list(
                &res.enable_res.cell_status_list_size,
                res.enable_res.cell_status_list, MLB_CELL_STATE_DISABLED);
            if (0 != res.enable_res.cell_status_list_size)
            {
                res.enable_res.result = SON_FAILURE;
                son_create_send_buffer((son_u8 *)&res,
                    SON_MLB_MODULE_ID,
                    SON_MIF_MODULE_ID,
                    SONMLB_ENABLE_RES,
                    enable_res_len);
            }
            else
            {
                SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_WARNING,
                    "mlb_pending_res_timer_handler: The pending response "
                    "list is either empty or all pending registration "
                    "response nodes are present with error code as "
                    "SON_NO_ERROR on pending response timer expiry");
            }
        }
        else if (SONMLB_DISABLE_RES == api)
        {
            son_mlb_disable_res_t res;
            son_size_t disable_res_len = sizeof(son_mlb_disable_res_t);
            SON_MEMSET(&res, 0, disable_res_len);
            res.transaction_id = mlb_get_pending_res_transaction_id();
            res.disable_res.error_code = SON_NO_ERROR;

            /* Remove Successful cells from the MLB_Pending_Response_LIST and
             *  create cell array with entry for each cell for which MLB could
             *  not be disabled. */
            mlb_construct_cell_arr_from_pending_res_list(
                &res.disable_res.cell_status_list_size,
                res.disable_res.cell_status_list, MLB_CELL_STATE_ENABLED);
            if (0 != res.disable_res.cell_status_list_size)
            {
                res.disable_res.result = SON_FAILURE;
                son_create_send_buffer((son_u8 *)&res,
                    SON_MLB_MODULE_ID,
                    SON_MIF_MODULE_ID,
                    SONMLB_DISABLE_RES,
                    disable_res_len);
            }
            else
            {
                SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                    SON_WARNING,
                    "mlb_pending_res_timer_handler: The pending response "
                    "list is either empty or all pending deregistration "
                    "response nodes are present with error code as "
                    "SON_NO_ERROR on pending response timer expiry");
            }
        }
        else if (SMIF_CELL_INFO_IND == api)
        {
            mlb_cell_info_ind_timer_handler();
        }
        else
        {
            SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                SON_WARNING,
                "mlb_pending_res_timer_handler: Invalid API Id %u found in "
                "pending response list", api);
        }

        /* Set MLB global FSM state to ACTIVE */
        SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
            MLB_STATE_ACTIVE);

        /* Clear/Reset Pending Response ListMLB_Pending_Response_LIST. */
        mlb_reset_pending_resp_list();
    }
    else
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "mlb_pending_res_timer_handler: MLB pending response timer"
            " expired with no data in pending reponse list.");
    }

    SON_UT_TRACE_EXIT();
} /* mlb_pending_res_timer_handler */

/*****************************************************************************
 * Function Name  : mlb_get_cell_mode
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : mlb cell FSM state for the cell
 * Description    : This function find the cell FSM state for the given cell Id
 ****************************************************************************/
mlb_cell_fsm_state_et
mlb_get_cell_mode
(
    son_intra_rat_global_cell_id_t     *p_cell_id
)
{
    mlb_cell_fsm_state_et fsm_state = MLB_CELL_STATE_UNDEFINED;
    mlb_cell_context_t *p_mlb_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_mlb_cell_ctxt = mlb_get_cell_context_from_global_context(p_cell_id);
    if (SON_PNULL != p_mlb_cell_ctxt)
    {
        fsm_state = p_mlb_cell_ctxt->current_cell_fsm_state;
    }

    SON_UT_TRACE_EXIT();
    return fsm_state;
}

/*****************************************************************************
 * Function Name  : mlb_send_feature_state_change_ind
 * Inputs         : p_cell_id - Pointer to the cell Id
 *                  state - MLB state for the cell
 *                  cause - Cause for the present state of MLB for the cell
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and sends the feature state
 *                  change indication message to SONMgmtIfH.
 ****************************************************************************/
son_void_t
mlb_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et state,
    son_feature_state_change_cause_et cause
)
{
    son_mlb_feature_state_change_ind_t state_change_ind;
    son_size_t state_change_ind_len =
        sizeof(son_mlb_feature_state_change_ind_t);

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&state_change_ind, 0,
        state_change_ind_len);
    /* Copy the CGI */
    SON_MEMCPY(&state_change_ind.srv_cgi, p_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    /* Set the Module ID */
    state_change_ind.module_id = SON_MLB_MODULE_ID;
    state_change_ind.state = state;
    state_change_ind.cause = cause;

    son_create_send_buffer((son_u8 *)&state_change_ind,
        SON_MLB_MODULE_ID,
        SON_MIF_MODULE_ID,
        SONMLB_FEATURE_STATE_CHANGE_IND,
        state_change_ind_len);

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "Feature state change "
        "indication sent for cell[Cell Id: %d]\n",
        son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

    SON_UT_TRACE_EXIT();
} /* mlb_send_feature_state_change_ind */

/*****************************************************************************
 * Function Name  : mlb_reset_pending_resp_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Resets MLB pending_response structure
 ****************************************************************************/
son_void_t
mlb_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    g_mlb_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
    g_mlb_pending_response.transaction_id = 0;
    g_mlb_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
    g_mlb_pending_response.is_error_received = SON_FALSE;
    if (g_mlb_pending_response.cell_sts_list.count)
    {
        mlb_clear_pending_res_list();
    }

    list_init(&g_mlb_pending_response.cell_sts_list);
    SON_MEMSET(&g_mlb_pending_response.ts, 0, sizeof(time_stamp_t));
    SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
        SON_DETAILED,
        "mlb_init_pending_resp_list: MLB Pending Response has been reset");
    mlb_print_pending_res("mlb_reset_pending_resp_list");

    SON_UT_TRACE_EXIT();
} /* mlb_reset_pending_resp_list */

/*****************************************************************************
 * Function Name  : mlb_is_any_cell_disabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in MLB_CELL_STATE_DISABLED
 *                  or MLB_CELL_STATE_DISABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in
 *                  MLB_CELL_STATE_DISABLED or MLB_CELL_STATE_DISABLING state.
 *                  If such cell is found it indicates that global enable
 *                  request can be processed as some cells needs to be enabled.
 ****************************************************************************/
son_bool_et
mlb_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_bool_et cell_exists = SON_FALSE;
    mlb_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_node = mlb_get_first_cell_ctxt();
    while (SON_PNULL != p_node)
    {
        if (MLB_CELL_STATE_DISABLED == p_node->data.current_cell_fsm_state)
        {
            cell_exists = SON_TRUE;
            break;
        }

        p_node = mlb_get_next_node(p_node);
    }

    SON_UT_TRACE_EXIT();
    return cell_exists;
} /* mlb_is_any_cell_disabled */

/*****************************************************************************
 * Function Name  : mlb_is_any_cell_enabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in MLB_CELL_STATE_ENABLED
 *                  or MLB_CELL_STATE_ENABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in
 *                  MLB_CELL_STATE_ENABLED or MLB_CELL_STATE_ENABLING state.
 *                  If such cell is found it indicates that global disable
 *                  request can be processed as some cells needs to be disabled.
 ****************************************************************************/
son_bool_et
mlb_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_bool_et cell_exists = SON_FALSE;
    mlb_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_node = mlb_get_first_cell_ctxt();
    while (SON_PNULL != p_node)
    {
        if (MLB_CELL_STATE_ENABLED == p_node->data.current_cell_fsm_state)
        {
            cell_exists = SON_TRUE;
            break;
        }

        p_node = mlb_get_next_node(p_node);
    }

    SON_UT_TRACE_EXIT();
    return cell_exists;
} /* mlb_is_any_cell_enabled */

/*****************************************************************************
 * Function Name  : mlb_populate_enable_disable_res_failure_data_from_pending_list
 * Inputs         : None
 * Outputs        : p_cell_status_list_size - Number of cells to be send in
 *                  response
 *                  p_cell_status_list  - Pointer to list containg cell Ids and
 *                  error code indicating the reason for failure
 * Returns        : None
 * Description    : This function populates enable or disable response data
 *                  struture with the cell Ids and the error codes of the cell
 *                  for which (de)registration request could not be sent to
 *                  RRM due to some SONMLB error or may be because ANR mode is
 *                  not enabled for the cell or cell context is not found at
 *                  ANR.
 ****************************************************************************/
son_void_t
mlb_populate_enable_disable_res_failure_data_from_pending_list
(
    son_u16            *p_cell_status_list_size,
    son_cell_status_t  *p_cell_status_list
)
{
    SON_LIST_NODE           *p_node      = SON_PNULL;
    cell_status_list_data_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_status_list_size || SON_PNULL == p_cell_status_list)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_ERROR,
            "mlb_populate_enable_disable_res_failure_data_from_pending_"
            "list: p_cell_status_list_size or p_cell_status_list is NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    p_node = get_first_list_node(&g_mlb_pending_response.cell_sts_list);
    while (SON_PNULL != p_node)
    {
        p_list_node =  YMEMBEROF(cell_status_list_data_t,
            cell_status_node,
            p_node);
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name, SON_BRIEF,
            "mlb_populate_enable_disable_res_failure_data_from_pending_"
            "list: Status for Cell [Cell Id: 0x%x] in Pending Response is "
            "error code = %u\n",
            son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_status.cgi.cell_identity),
            p_list_node->cell_status.error_code);

        SON_MEMCPY(&p_cell_status_list[*p_cell_status_list_size].cgi,
            &p_list_node->cell_status.cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        p_cell_status_list[*p_cell_status_list_size].error_code =
            p_list_node->cell_status.error_code;
        (*p_cell_status_list_size)++;
        p_node = get_next_list_node(p_node);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_populate_enable_disable_res_failure_data_from_pending_list */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : mlb_get_log_level
 * Inputs         : None
 * Outputs        : None
 * Returns        : Log Level of the MLB
 * Description    : This function gets the log level
 ******************************************************************************/
son_u8
mlb_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_UT_TRACE_ENTER();
    SON_UT_TRACE_EXIT();
    return g_mlb_context.log_level;
}

/*****************************************************************************
 * Function Name  : mlb_set_log_level
 * Inputs         : log_level - Log Level to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log level
 ******************************************************************************/
son_void_t
mlb_set_log_level
(
    son_u8 log_level
)
{
    SON_UT_TRACE_ENTER();
    g_mlb_context.log_level = log_level;
    SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */
