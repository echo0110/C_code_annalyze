/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_es_ctxt_mgr.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for managing ES Module's
 *        Global context and cell specific context.
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Feb, 2012     Atul/Reetesh                  Initial
 *  Sep,2012         Atul                   Changes Done to support Release 1.2
 *  Jun, 2013      gur25767                 ES Enhancement for Autonomous Swict Off
 *  Aug, 2014      Shilpi                   SPR 13251 Fix
 ******************************************************************************/
#include <son_es_ctxt_mgr.h>
#include <son_utils.h>
#include <son_es_cell_fsm.h>

extern const son_8 *p_son_es_facility_name;

/*****************************************************************************
 *  Name  : ES Global Context List.
 ****************************************************************************/
es_global_t g_es_context;

/*****************************************************************************
 *  Name  : ES Global Pending Response structure to store information related
 *  to the cells with pending response messages.
 ****************************************************************************/
pending_res_t g_es_pending_response;

/*****************************************************************************
 *  Name  : ES Global Pending Response Structure to store information
 *  related to the cells with pending Switch off response messages for
 *  Global Cell Switch Off Request received from ANR or Cell Activation
 *  request received over X2.
 ****************************************************************************/
switch_on_pending_res_t g_es_switch_on_pending_res;

/* SONES FSM state names - for ES Global FSM*/
const son_s8 *SON_ES_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONES_STATE_INIT",
	(const son_s8 *)"SONES_STATE_ACTIVE",
	(const son_s8 *)"SONES_STATE_ENABLING",
	(const son_s8 *)"SONES_STATE_DISABLING",
	(const son_s8 *)"SONES_STATE_SHUTDOWN",
	(const son_s8 *)"SONES_STATE_UNDEFINED"
};

/* SONES FSM state names - for Cell Level FSM*/
const son_s8 *SON_ES_CELL_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONES_CELL_STATE_DISABLING",
	(const son_s8 *)"SONES_CELL_STATE_DISABLED",
	(const son_s8 *)"SONES_CELL_STATE_ENABLING",
	(const son_s8 *)"SONES_CELL_STATE_ENABLED",
	(const son_s8 *)"SONES_CELL_STATE_SWITCHING_OFF",
	(const son_s8 *)"SONES_CELL_STATE_SWITCHING_ON",
	(const son_s8 *)"SONES_CELL_STATE_SWITCHED_OFF",
	(const son_s8 *)"SONES_CELL_STATE_UNDEFINED"
};
/* Expected SON ES Module Event names from other modules*/
const son_s8 *SON_ES_CELL_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"SONES_ENABLE_REQ",
	(const son_s8 *)"SONES_DISABLE_REQ",
	(const son_s8 *)"SONES_CELL_SWITCH_ON_REQ",
	(const son_s8 *)"SONES_CELL_SWITCH_OFF_REQ",
	(const son_s8 *)"SONES_MODIFY_UE_COUNT_INTERVAL_REQ",
	(const son_s8 *)"SONES_MODIFY_UE_COUNT_THRESHOLD_REQ",
	(const son_s8 *)"RRM_SON_REGISTER_RESP",
	(const son_s8 *)"RRM_SON_DEREGISTER_RESP",
	(const son_s8 *)"RRM_SON_SET_ATTRIBUTE_VALUE_RES",
	(const son_s8 *)"RRM_SON_CELL_SWITCH_ON_RES",
	(const son_s8 *)"RRM_SON_CELL_SWITCH_OFF_RES",
	(const son_s8 *)"RRM_SON_ACTIVE_UE_COUNT_REPORT",
	(const son_s8 *)"RRM_SON_COUNT_THRESHOLD_HIT_IND",
	(const son_s8 *)"RRM_SON_S1_MSG_IND",
	(const son_s8 *)"RRM_SON_CELL_CHNAGE_IND",
	(const son_s8 *)"RRM_SON_LOAD_REPORT_IND",
	(const son_s8 *)"NOT_AVAILABLE",
    /* SPR_19619 start */
	(const son_s8 *)"SONES_PEER_CELL_ACTIVATION_REQ",
    /* SPR_19619 stop */
};

/*****************************************************************************
 * Function Name  : es_print_pending_res
 * Inputs         : son_method_name - name of the invoking method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Print pending_response structure
 ****************************************************************************/
static son_void_t
es_print_pending_res
(
 const son_8 *son_method_name
 )
{
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_LOG(g_es_context.log_enable, p_son_es_facility_name, SON_INFO,
			"%s: Pending Response Structure:"
			"API Id: %d "
			"Transaction Id: %d "
			"Expected Response Count: %d"
			"Error Received Flag: %d",
			son_method_name,
			g_es_pending_response.api_id,
			g_es_pending_response.transaction_id,
			g_es_pending_response.expected_res_count,
			g_es_pending_response.is_error_received);
	p_node = get_first_list_node(&g_es_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF( cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_es_context.log_enable,
				p_son_es_facility_name, SON_WARNING,
				"Cell id is [0x%x] with error code = %d",
				son_convert_char_cell_id_to_int_cell_id(p_list_node->
					cell_status.cgi.cell_identity),
				p_list_node->cell_status.error_code);

		p_node = get_next_list_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* es_print_pending_res */

/*****************************************************************************
 * Function Name  : es_print_global_context
 * Inputs         : son_method_name - name of the invoking method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the existing ES global context
 *                  structure
 ******************************************************************************/
son_void_t
es_print_global_context
(
 const son_8 *son_method_name
 )
{
	es_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	/* AUTONOMOUS SWITCH OFF START */
	SON_LOG(g_es_context.log_enable, p_son_es_facility_name, SON_INFO,
			"\n==== ES Global Context Structure:====\n"
			"ES Current FSM State: %s "
			"ES Global Mode: %d "
			"ES Log Enable: %d"
			"Autonomous Swicth Off config: %d"
			"Load Level Threshold(Valid if Load level check set): %d"
			"No of Cell configured at ES %d",
			SON_ES_FSM_STATES_NAMES[g_es_context.current_fsm_state],
			g_es_context.es_global_mode,
			g_es_context.log_enable,
			g_es_context.g_autonomous_switch_off_info,
			g_es_context.g_load_level_threshold,
			g_es_context.cell_ctxt_list.count);
	/* AUTONOMOUS SWITCH OFF END */
	p_node = es_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
        /*SPR 17777 +-*/
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name, SON_BRIEF,
				"Cell id is [0x%x] with current state = %s; son_method_name %s",
				son_convert_char_cell_id_to_int_cell_id(p_node->data.cell_id.cell_identity),
				SON_ES_CELL_FSM_STATES_NAMES[p_node->data.current_cell_fsm_state],son_method_name);
        /*SPR 17777 +-*/

		p_node = es_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* es_print_global_context */

/*****************************************************************************
 * Function Name  : es_reset_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Resets global pending_response structure
 ****************************************************************************/
son_void_t
es_reset_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	es_print_pending_res("before reset");
	g_es_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
	g_es_pending_response.transaction_id = SON_UNDEFINED;
	g_es_pending_response.expected_res_count = 0;
	g_es_pending_response.is_error_received = SON_FALSE;
	es_clear_pending_res_list();
	list_init(&g_es_pending_response.cell_sts_list);
	g_es_pending_response.ts = 0;
	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_BRIEF,
			"Pending Response Reset");

	es_print_pending_res("after reset");
	SON_UT_TRACE_EXIT();
} /* es_reset_pending_res */

/*****************************************************************************
 * Function Name  : es_init_pending_res
 * Inputs         : tid		Transaction Id
 *                : api		Procedure Code
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize global pending_response structure
 ****************************************************************************/
son_void_t
es_init_pending_res
(
 son_u16 tid,
 son_procedure_code_et api
 )
{
	SON_UT_TRACE_ENTER();
	g_es_pending_response.api_id = api;
	g_es_pending_response.transaction_id = tid;
	g_es_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_es_pending_response.is_error_received = SON_FALSE;
	list_init(&g_es_pending_response.cell_sts_list);
	son_time(&g_es_pending_response.ts);

	es_print_pending_res("es_init_pending_res");
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16 - pending response Transaction ID
 * Description    : Returns transaction id of pending response
 ****************************************************************************/
son_u16
es_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_pending_response.transaction_id;
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u8
 * Description    : Returns count of awaited responses from pending response
 ****************************************************************************/
son_u8
es_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_pending_response.expected_res_count;
}

/*****************************************************************************
 * Function Name  : es_increment_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Increments expected response count in pending
 *                  response structure
 ****************************************************************************/
son_void_t
es_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_es_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		g_es_pending_response.expected_res_count++;
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Max value of "
				"Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_decrement_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Decrements expected response count in pending
 *                  response structure
 ****************************************************************************/
son_void_t
es_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_es_pending_response.expected_res_count > MIN_PENDING_RES_COUNT)
	{
		g_es_pending_response.expected_res_count--;
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Min value of "
				"Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et
 * Description    : Returns API ID present in pending response
 ****************************************************************************/
son_procedure_code_et
es_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_pending_response.api_id;
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_cell_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type SON_LIST
 * Description    : Returns pointer to cell_status_list present in
 *					pending response
 ****************************************************************************/
SON_LIST *
es_get_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_es_pending_response.cell_sts_list;
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_ts_diff_from_curr_ts
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32
 * Description    : Returns difference between current timestamp and timestamp
 *                : present in pending response
 ****************************************************************************/
son_u32
es_get_pending_res_ts_diff_from_curr_ts
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	time_stamp_t curr_ts;

	SON_UT_TRACE_ENTER();
	son_time(&curr_ts);
	SON_UT_TRACE_EXIT();
	return ((son_u32)son_difftime(curr_ts, g_es_pending_response.ts));
}

/*****************************************************************************
 * Function Name  : es_set_pending_res_ts_with_current_timestamp
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set timestamp in pending response for current time
 ****************************************************************************/
son_void_t
es_set_pending_res_ts_with_current_timestamp
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	son_time(&g_es_pending_response.ts);
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_set_pending_res_error_rcvd
 * Inputs         : val: value to be updated in flag present in pending response
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set is_eror_received flag in pending response
 ****************************************************************************/
son_void_t
es_set_pending_res_error_rcvd
(
 son_bool_et val
 )
{
	SON_UT_TRACE_ENTER();
	if (g_es_pending_response.is_error_received != val)
	{
		g_es_pending_response.is_error_received = val;
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_pending_res_error_rcvd
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Returns is_eror_received flag, present in pending response
 ****************************************************************************/
son_bool_et
es_get_pending_res_error_rcvd
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_pending_response.is_error_received;
}

/*****************************************************************************
 * Function Name  : es_find_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id : Pointer to Cell ID
 * Outputs        : None
 * Returns        : pointer of type cell_status_list_data_t
 * Description    : Find cell_status node in the cell_status_list for the
 *                  given Cell ID
 ****************************************************************************/
cell_status_list_data_t *
es_find_cell_in_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
 )
{
	SON_LIST_NODE                       *p_node                         = SON_PNULL;
	cell_status_list_data_t *p_list_node                = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_node = list_find_node(&g_es_pending_response.cell_sts_list,
			(son_void_t *)p_rcvd_cell_id,
			son_key_of_es,
			son_compare_cell_id);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_status_list_data_t,
				cell_status_node,
				p_node);
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID [0x%x] not found in es context",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* es_find_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : es_delete_succesful_cells_from_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete those cell_status nodes from pending response list
 *                  which are succefully registered
 ****************************************************************************/
son_void_t
es_delete_succesful_cells_from_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE                       *p_node          = SON_PNULL;
	SON_LIST_NODE           *p_next_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	p_node = get_first_list_node(&g_es_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF( cell_status_list_data_t,
				cell_status_node,
				p_node);
		if (SON_NO_ERROR == p_list_node->cell_status.error_code)
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Cell id [0x%x]  with result=SON_SUCCESS is succesfully"
					" deleted from es pending response",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->
						cell_status.cgi.cell_identity));
			p_next_node = get_next_list_node(p_node);
			list_delete_node(&g_es_pending_response.cell_sts_list, p_node);
			son_mem_free(p_list_node);
			p_node = p_next_node;
			p_next_node = SON_PNULL;
		}
		else
		{
			p_node = get_next_list_node(p_node);
		}
	}

	es_print_pending_res("after es_delete_succesful_cells_from_pending_res");
	SON_UT_TRACE_EXIT();
} /* es_delete_succesful_cells_from_pending_res */

/*****************************************************************************
 * Function Name  : es_add_cell_in_pending_res
 * Inputs         : p_sts_node : Pointer to Cell Status Node
 * Outputs        : None
 * Returns        : son_bool_et, node added succesfully or not.
 * Description    : Add cell_status node in the pending response structure.
 ****************************************************************************/
son_bool_et
es_add_cell_in_pending_res
(
 son_cell_status_t   *p_sts_node
 )
{
	cell_status_list_data_t *p_list_node = SON_PNULL;
	son_bool_et ret_val      = SON_FALSE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_sts_node &&
			g_es_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		p_list_node = son_mem_get(sizeof(cell_status_list_data_t));
		if (SON_PNULL != p_list_node)
		{
			SON_MEMCPY(&p_list_node->cell_status, p_sts_node,
					sizeof(son_cell_status_t));
			list_insert_node(&g_es_pending_response.cell_sts_list,
					&p_list_node->cell_status_node);
			es_increment_pending_res_expected_count();
			ret_val = SON_TRUE;
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Cell [0x%x] is added in Pending  Response",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->
						cell_status.cgi.cell_identity));

			es_print_pending_res("es_add_cell_in_pending_res");
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Memory allocation failure");
		}
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Invalid input parameters [p_sts_node is NULL] "
				"or pending response count reached max limit [%d]", MAX_PENDING_RES_COUNT);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_add_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : es_pending_res_handler
 * Inputs         : p_cell_id   Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function cleans up Pending Response Structure and sends
 *                : pending response to OAM
 ****************************************************************************/
son_void_t
es_pending_res_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
	son_es_feature_state_change_ind_t state_change_ind;
	SON_MEMSET(&state_change_ind,0,sizeof(son_oam_feature_state_change_ind_t));

	SON_UT_TRACE_ENTER();
	if((api = es_get_pending_res_api_id()) != SMIF_CELL_INFO_IND)
	{
		son_es_enable_res_t res;
		SON_MEMSET(&res,0,sizeof(son_es_enable_res_t));

		/* send response for request
		   maintained in pending_response */
		res.transaction_id = es_get_pending_res_transaction_id();
		es_delete_succesful_cells_from_pending_res();

		if(es_get_pending_res_cell_count() > MIN_PENDING_RES_COUNT)
		{
			res.enable_res.cell_status_list_size =
				es_get_pending_res_cell_count();

			es_construct_cell_arr_from_pending_res_list(
					res.enable_res.cell_status_list);
		}
		else
		{
			res.enable_res.result = SON_SUCCESS;
			res.enable_res.error_code = SON_NO_ERROR;
		}

		son_create_send_buffer((son_u8 *)&res,
				SON_ES_MODULE_ID,
				SON_MIF_MODULE_ID,
				api,
				sizeof(res));
	}
	if( es_get_pending_res_ts_diff_from_curr_ts() >
			ES_TIMEOUT_FOR_PENDING_RES)
	{
		es_cell_context_node_t *p_ctxt_node = SON_PNULL;
		es_cell_context_t  * p_cell_ctxt = SON_PNULL;
		es_cell_fsm_state_et new_state = ES_CELL_STATE_UNDEFINED;
		p_ctxt_node = es_get_first_cell_ctxt();
		while(SON_PNULL != p_ctxt_node)
		{
			p_cell_ctxt = &p_ctxt_node->data;
			if(es_find_cell_in_pending_res(&p_cell_ctxt->cell_id))
			{
				new_state = p_cell_ctxt->previous_cell_fsm_state;
				SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						new_state);
			}
			if (SMIF_CELL_INFO_IND == api)
			{
				/* Send feature state change indication to SMIF */
				es_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
						SON_STATE_DISABLED,
						SON_RRM_REGISTRATION_UNSUCCESSFUL);
			}
			p_ctxt_node = es_get_next_node(p_ctxt_node);
		}
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_delete_cell_from_pending_res
 * Inputs         : p_cell_id   Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete cell_status node from the pending response's
 *                  cell_status_list using cell ID.
 ****************************************************************************/
son_void_t
es_delete_cell_from_pending_res
(
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	cell_status_list_data_t             *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_id)
	{
		p_node = es_find_cell_in_pending_res(p_cell_id);
		if (SON_PNULL != p_node)
		{
			list_delete_node(&g_es_pending_response.cell_sts_list,
					&p_node->cell_status_node);
			son_mem_free(p_node);
			es_decrement_pending_res_expected_count();
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Deleting Cell [0x%x] from Pending Response List",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

			es_print_pending_res("After deletion in "
					"es_delete_cell_from_pending_res");

			if (!es_get_pending_res_expected_count())
			{
				SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
						SON_BRIEF,
						"Expected Response count has reached 0"
						" reseting Pending Response");
				es_set_context_state(ES_STATE_ACTIVE);
			}
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Node not found for cell [0x%x]",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* es_delete_cell_from_pending_res */

/*****************************************************************************
 * Function Name  : es_clear_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all cell_status node from the pending response's
 *                  cell_status_list
 ****************************************************************************/
son_void_t
es_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE                       *p_node          = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	p_node = list_pop_node(&g_es_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_es_pending_response.cell_sts_list);
	}

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_DETAILED,
			"Pending Response list reset");

	es_print_pending_res("es_clear_pending_res_list");
	SON_UT_TRACE_EXIT();
} /* es_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : es_update_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id   Pointer to Cell ID
 *                : err_code		 error_code
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Update cell_status node in the cell_status_list
 *                    for cell id and error code.
 ****************************************************************************/
son_bool_et
es_update_cell_in_pending_res
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
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
		return ret_val;
	}

	p_list_node = es_find_cell_in_pending_res(p_rcvd_cell_id);

	if (p_list_node)
	{
		p_list_node->cell_status.error_code = err_code;
		ret_val = SON_TRUE;
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_BRIEF,
				"Pending Response List "
				" context updated for cell [0x%x] with error"
				" code = %d",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity), err_code);

		es_print_pending_res("es_update_cell_in_pending_res");
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell [0x%x] not found",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_update_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : es_construct_cell_arr_from_pending_res_list
 * Inputs         : None
 * Outputs        : p_cell_sts_arr - Pointer of type son_cell_status_t
 * Returns        : son_void_t
 * Description    : Construct cell_status_arr from cell_status_list
 ****************************************************************************/
son_void_t
es_construct_cell_arr_from_pending_res_list
(
 son_cell_status_t                       *p_cell_sts_arr
 )
{
	SON_LIST_NODE                       *p_node                 = SON_PNULL;
	cell_status_list_data_t *p_list_node        = SON_PNULL;
	son_u8 idx = 0;
	son_u8 count = list_count(&g_es_pending_response.cell_sts_list);

	SON_UT_TRACE_ENTER();

	if (count > MIN_PENDING_RES_COUNT)
	{
		p_node = list_pop_node(&g_es_pending_response.cell_sts_list);
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_DETAILED,
				"Constructing Cell Array from pending Response");

		es_print_pending_res("es_construct_cell_arr_from_pending_res_list");

		while (SON_PNULL != p_node)
		{
			p_list_node = YMEMBEROF(cell_status_list_data_t,
					cell_status_node, p_node);
			SON_MEMCPY(&p_cell_sts_arr[idx++], &p_list_node->cell_status,
					sizeof(son_cell_status_t));
			son_mem_free(p_list_node);
			p_node = list_pop_node(&g_es_pending_response.cell_sts_list);
		}
	}

	SON_UT_TRACE_EXIT();
} /* es_construct_cell_arr_from_pending_res_list */

/*****************************************************************************
 * Function Name  : son_key_of_es
 * Inputs         : p_node   Pointer to cell specific context node
 * Outputs        : None
 * Returns        : Pointer of type son_void_t
 * Description    : Returns cell node key
 ******************************************************************************/
const son_void_t *
son_key_of_es
(
 const SON_LIST_NODE *p_node
 )
{
	es_cell_context_node_t *p_data = SON_PNULL;
	son_intra_rat_global_cell_id_t  *p_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(es_cell_context_node_t, sNode, p_node);
	p_cell_id = &(p_data->data.cell_id);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_id;
}
/*SPR_19279_START*/
const son_void_t *
son_cell_index_key_of_es
(
 const SON_LIST_NODE *p_node
 )
{
	es_cell_context_node_t *p_data = SON_PNULL;
	son_u8  *p_cell_idx = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(es_cell_context_node_t, sNode, p_node);
	p_cell_idx = &(p_data->data.cell_index);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_idx;
}
/*SPR_19279_END*/

/*****************************************************************************
 * Function Name  : es_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize ES global Context
 ******************************************************************************/
son_void_t
es_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	g_es_context.current_fsm_state = ES_STATE_INIT;
	g_es_context.es_global_mode = SON_DISABLED;
	g_es_context.log_enable = SON_OAM_LOG_ON;
	/* SPR-13251 fix starts */
	g_es_context.log_level = SON_OAM_LOG_LEVEL_BRIEF;
	/* SPR-13251 fix ends */
	SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);
	list_init(&g_es_context.cell_ctxt_list);
	/* AUTONOMOUS SWITCH OFF START */
	g_es_context.g_autonomous_switch_off_info = SON_NULL;
	g_es_context.g_load_level_threshold = SON_NULL;
	/* Fix 875 Start */
	g_es_context.g_es_timer_id = SON_NULL;
	/* Fix 875 End */
	/* AUTONOMOUS SWITCH OFF END */
	/* Fix 867 Start*/
	g_es_context.ue_report_interval = SON_NULL;
	g_es_context.ue_count_threshold = SON_NULL;
	/* Fix 867 End */
	/*SPR11689 changes start */
	/*by deafult peer cell activation is enabled*/
	g_es_context.peer_cell_activation_enabled = SON_TRUE;
	/*SPR11689 changes end */

	es_print_global_context("es_init_global_context");
	SON_UT_TRACE_EXIT();
} /* es_init_global_context */

/*****************************************************************************
 * Function Name  : es_init_cell_context
 * Inputs         : None
 * Outputs        : p_ctxt
 * Returns        : son_void_t
 * Description    : Initialize ES cell specific Context
 ******************************************************************************/
son_void_t
es_init_cell_context
(
 es_cell_context_t *p_ctxt
 )
{
	SON_UT_TRACE_ENTER();
	if (p_ctxt != SON_PNULL)
	{
		SON_MEMSET(&p_ctxt->cell_id, 0,
				sizeof(son_intra_rat_global_cell_id_t));
		p_ctxt->current_cell_fsm_state = ES_CELL_STATE_DISABLED;
		p_ctxt->previous_cell_fsm_state = ES_CELL_STATE_DISABLED;
		p_ctxt->current_cell_state = RRM_CELL_OPERATIONAL;
		/* AUTONOMOUS SWITCH OFF START */
		p_ctxt->autonomous_switch_off_info = g_es_context.g_autonomous_switch_off_info;
		/* Fix 875 Start */
		if (SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL & g_es_context.g_autonomous_switch_off_info)
		{
			p_ctxt->load_level_threshold = g_es_context.g_load_level_threshold;
		}
		else
		{
			p_ctxt->load_level_threshold = SON_NULL;
		}
		p_ctxt->ue_report_interval = g_es_context.ue_report_interval;
		p_ctxt->modify_ue_report_interval = SON_NULL;
		p_ctxt->ue_count_threshold = g_es_context.ue_count_threshold;
		p_ctxt->modify_ue_count_threshold_val = SON_NULL;
		p_ctxt->es_timer_id = SON_PNULL;
		p_ctxt->switch_off_cond_hit_status = SON_NULL;
		/* Fix 875 End */

		/* AUTONOMOUS SWITCH OFF END */

		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"Cell Context Initialized");
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Input Cell Ctxt Param is NULL !!!");
	}

	SON_UT_TRACE_EXIT();
} /* es_init_cell_context */

/*****************************************************************************
 * Function Name  : es_get_context_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : Number of Cell ctxt nodes in Cell ctxl list
 * Description    : Returns the count of the cell ctxt node present in
 *                  cell ctxt list
 ******************************************************************************/
son_u32
es_get_context_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_es_context.cell_ctxt_list);
}

/*****************************************************************************
 * Function Name  : es_get_cell_ctxt_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type SON_LIST
 * Description    : Returns cell context list
 ******************************************************************************/
SON_LIST *
es_get_cell_ctxt_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_es_context.cell_ctxt_list;
}
/*****************************************************************************
 * Function Name  : es_get_cell_context_from_global_context
 * Inputs         : p_cell_id   Pointer to the cell Id
 * Outputs        : None
 * Returns        : pointer of type es_cell_context_t
 * Description    : Returns cell context for cell id passed as an argument
 ******************************************************************************/
es_cell_context_t *
es_get_cell_context_from_global_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	es_cell_context_node_t *p_data      = SON_PNULL;
	SON_LIST_NODE               *p_list_node    = SON_PNULL;
	es_cell_context_t   *p_retval               = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return p_retval;
	}

        SET_CELL_INDEX(son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	p_list_node = list_find_node(&g_es_context.cell_ctxt_list,
			p_cell_id, son_key_of_es, son_compare_cell_id);

	if (SON_NULL != p_list_node)
	{
		p_data = (es_cell_context_node_t *) YMEMBEROF
			(es_cell_context_node_t, sNode, p_list_node);
		p_retval = &p_data->data;
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell [0x%x] not found",
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_retval;
} /* es_get_cell_context_from_global_context */

/*****************************************************************************
 * Function Name  : es_get_context_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : es_fsm_state_et
 * Description    : Returns ES module's Global state
 ******************************************************************************/
es_fsm_state_et
es_get_context_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();

	return g_es_context.current_fsm_state;
}

/*****************************************************************************
 * Function Name  : es_set_context_state
 * Inputs         : state    ES module's Global state
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set ES module's Global state
 ******************************************************************************/
son_void_t
es_set_context_state
(
 es_fsm_state_et state
 )
{
	SON_UT_TRACE_ENTER();
	if (g_es_context.current_fsm_state != state)
	{
		SONES_FSM_SET_STATE(g_es_context.current_fsm_state, state);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_set_log_mode
 * Inputs         : log_mode   log mode for ES module
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : set log mode
 ******************************************************************************/
son_void_t
es_set_log_mode
(
 son_oam_log_on_off_et log_mode
 )
{
	SON_UT_TRACE_ENTER();
	g_es_context.log_enable = log_mode;
	SON_UT_TRACE_EXIT();
}

/*SPR-11554-Start-Functions modified*/

/*****************************************************************************
 * Function Name  : es_validate_cell_data
 * Inputs         : p_cell - Pointer to the cell data
 * Outputs        : None
 * Returns        : SON_TRUE: If valid
 *                  SON_FALSE:If not valid
 * Description    : This function validates the data of cell received in cell
 *                  info indication
 ******************************************************************************/
static son_bool_et
es_validate_cell_data
(
 son_cell_t        *p_cell
 )
{
	son_bool_et is_valid = SON_TRUE;
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
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Invalid pci range received. open_pci_start[%d], open_pci_end[%d]",
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
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Invalid pci range received. closed_pci_start[%d], closed_pci_end[%d]",
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
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Invalid pci range received. hybrid_pci_start[%d], hybrid_pci_end[%d]",
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

	/* Check if it is the first cell being configured at ES */
	if (0 == g_es_context.cell_ctxt_list.count)
	{
		if (is_home_eNB == SON_TRUE)
		{
			g_es_context.enb_type = SON_HOME_ENB;
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_DETAILED,
					"The serving eNB is a home eNB");
		}
		else
		{
			g_es_context.enb_type = SON_MACRO_ENB;
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_INFO,
					"The serving eNB is a macro eNB");
		}
	}

	/* Check if 1 cell is already configured and if the eNB is
	 * home eNB do not configure any other cell as home eNB can
	 * have only one serving cell */
	else if (SON_HOME_ENB == g_es_context.enb_type)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_INFO,
				"Cell [0x%x]"
				" is not added to ES Global Context - home eNB is"
				" already configured with 1 serving cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}
	else if ((SON_MACRO_ENB == g_es_context.enb_type) &&
			(is_home_eNB == SON_TRUE))
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_INFO,
				"Cell [0x%x]"
				" is not added to ES Global Context - macro eNB "
				" eNB is already configured as macro,cannot configure femto cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_valid;
} /* es_validate_cell_data */


/*****************************************************************************
 * Function Name  : es_add_new_cell_in_context
 * Inputs         : p_cell - Pointer to the cell data
 * Outputs        : p_cause - Cause of failure if failure occurs in addition
 *                  p_state - ES feature state for the cell
 * Returns        : Pointer to the new node added in the list, NULL if node
 *                  already exists or creation of node fails
 * Description    : This is used to add a new context in ES global
 *                  context if not already exists.
 ******************************************************************************/
es_cell_context_node_t *
es_add_new_cell_in_context
(
 son_cell_t                          *p_cell,
 son_feature_state_et                *p_state,
 /*SPR_19279_START */
 son_feature_state_change_cause_et   *p_cause,
 son_u8 cell_index
 /*SPR_19279_END */
 )
{
	es_cell_context_node_t *p_new_node  = SON_PNULL;
	SON_LIST_NODE                  *p_list_node         = SON_PNULL;
	es_cell_context_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell || SON_PNULL == p_cause ||
			SON_PNULL == p_state)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell Data or p_state or p_cause is "
				"NULL");
	}
	else
	{
		p_list_node = list_find_node(&g_es_context.cell_ctxt_list,
 /*SPR_19279_START */
				&cell_index, son_cell_index_key_of_es, son_compare_cell_index);
 /*SPR_19279_END */

		if (SON_PNULL == p_list_node)
		{
			if (g_es_context.cell_ctxt_list.count <
					SON_MAX_CELL_SUPPORTED_AT_ENB)
			{
				if (SON_FALSE == es_validate_cell_data(p_cell))
				{
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_INTERNAL_FAILURE;
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}

				/* Create a new node */
				p_new_node = (es_cell_context_node_t *)son_mem_get(
						sizeof(es_cell_context_node_t));

				if (SON_PNULL != p_new_node)
				{
					/*Populate the node*/
					es_init_cell_context(&p_new_node->data);
                    /*SPR_19279_START */
                    p_new_node->data.cell_index = cell_index;
                    /*SPR_19279_END */
					SON_MEMCPY(&p_new_node->data.cell_id, &p_cell->src_cgi,
							sizeof(son_intra_rat_global_cell_id_t));

					/*Insert it in context list */
					list_insert_node(&g_es_context.cell_ctxt_list,
							&p_new_node->sNode);
					*p_state = SON_STATE_DISABLED;
					*p_cause = SON_ES_MODE_DISABLED;
					SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
							SON_BRIEF,
							"Cell Id[0x%x] added in"
							" ES Cell Context List",
							son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
					es_print_global_context("es_add_new_cell_in_context");
				}
				else
				{
					SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
							SON_ERROR,
							"Memory allocation failure");
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_MEM_ALLOC_FAILURE;
				}
			}
			else
			{
				SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
						SON_ERROR,
						"Cell [0x%x]"
						" could not be added to ES Global Context - "
						"Maximum(%d) number of supported cells limit reached",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
						g_es_context.cell_ctxt_list.count);
				*p_state = SON_STATE_DELETED;
				*p_cause = SON_MAX_SUPPORTED_CELL_LIMIT_REACHED;
			}
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Cell 0x%x already exists"
					" in ES Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
			p_data = YMEMBEROF(es_cell_context_node_t, sNode, p_list_node);
            /*SPR_19279_START*/
            if (SON_MEMCMP(&p_data->data.cell_id,
                        &p_cell->src_cgi, sizeof(p_new_node->data.cell_id)))
            {
                SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
                        SON_BRIEF,
                        "ECGI for cell [0x%x] updated",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

                SON_MEMCPY(&p_data->data.cell_id,
                        &p_cell->src_cgi,
                        sizeof(p_data->data.cell_id));
            }
            /*SPR_19279_END*/

			if (ES_CELL_STATE_DISABLED == p_data->data.current_cell_fsm_state
					|| ES_CELL_STATE_ENABLING ==
					p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_DISABLED;
			}
			else if (ES_CELL_STATE_ENABLED ==
					p_data->data.current_cell_fsm_state ||
					ES_CELL_STATE_SWITCHED_OFF ==
					p_data->data.current_cell_fsm_state ||
					ES_CELL_STATE_SWITCHING_OFF ==
					p_data->data.current_cell_fsm_state ||
					ES_CELL_STATE_SWITCHING_ON ==
					p_data->data.current_cell_fsm_state ||
					ES_CELL_STATE_DISABLING ==
					p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_ENABLED;
			}

			*p_cause = SON_CELL_ALREADY_EXISTS;
		}
	}

	SON_UT_TRACE_EXIT();
	return p_new_node;
} /* es_add_new_cell_in_context */

/*SPR-11554-End*/

/*****************************************************************************
 * Function Name  : es_clear_context_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Clear ES context list
 ******************************************************************************/
son_void_t
es_clear_context_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE                       *p_node         = SON_PNULL;
	es_cell_context_node_t *p_data      = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&g_es_context.cell_ctxt_list);
	while (SON_PNULL != p_node)
	{
		p_data = (es_cell_context_node_t *)
			YMEMBEROF(es_cell_context_node_t, sNode, p_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);
		p_node = list_pop_node(&g_es_context.cell_ctxt_list);
	}

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_BRIEF,
			"ES Context List Cleared");

	SON_UT_TRACE_EXIT();
} /* es_clear_context_list */

/*****************************************************************************
 * Function Name  : es_delete_cell_from_context
 * Inputs         : p_cell_id   Pointer to the cell Id
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : delete node from context list based on cell id passed as
 *					argument
 ******************************************************************************/
son_bool_et
es_delete_cell_from_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	son_bool_et is_node_deleted = SON_FALSE;
	SON_LIST_NODE                       *p_list_node    = SON_PNULL;
	es_cell_context_node_t *p_data              = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell_id)
	{
		p_list_node = list_find_node(&g_es_context.cell_ctxt_list,
				p_cell_id,
				son_key_of_es, son_compare_cell_id);
		if (SON_PNULL != p_list_node)
		{
			p_data = (es_cell_context_node_t *)YMEMBEROF
				(es_cell_context_node_t, sNode, p_list_node);
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Cell [0x%x]  deleted"
					" from ES Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
			list_delete_node(&g_es_context.cell_ctxt_list,
					p_list_node);

			/* Free the memory occupied by the node */
			son_mem_free(p_data);
			is_node_deleted = SON_TRUE;
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Cell [0x%x] not found",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		}

		es_print_global_context("es_delete_cell_from_context");
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
	return is_node_deleted;
} /* es_delete_cell_from_context */

/*****************************************************************************
 * Function Name  : es_get_global_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_app_mode_et
 * Description    : Returns es global mode(enabled/disabled)
 ******************************************************************************/
son_app_mode_et
es_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_context.es_global_mode;
}

/*****************************************************************************
 * Function Name  : es_set_global_mode
 * Inputs         : mode   son_app_mode_et
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set es global mode(enabled/disabled)
 ******************************************************************************/
son_void_t
es_set_global_mode
(
 son_app_mode_et mode
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
			"ES global mode set to %u", mode);
	g_es_context.es_global_mode = mode;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_log_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_oam_log_on_off_et
 * Description    : Returns log mode(on/off)
 ******************************************************************************/
son_oam_log_on_off_et
es_get_log_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_context.log_enable;
}

/*****************************************************************************
 * Function Name  : es_get_first_cell_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type es_cell_context_node_t
 * Description    : Returns first context node from ES context list
 ******************************************************************************/
es_cell_context_node_t *
es_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE       *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (list_count(&g_es_context.cell_ctxt_list) > 0)
	{
		p_node = get_first_list_node(&g_es_context.cell_ctxt_list);

		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (es_cell_context_node_t *)
				YMEMBEROF(es_cell_context_node_t, sNode, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* es_get_first_cell_ctxt */

/*****************************************************************************
 * Function Name  : es_get_next_node
 * Inputs         : p_node  Pointer to context node
 * Outputs        : None
 * Returns        : pointer of type es_cell_context_node_t
 * Description    : Return next context node in context list of the node passed
 *                  as an argument
 ******************************************************************************/
es_cell_context_node_t *
es_get_next_node
(
 es_cell_context_node_t *p_node
 )
{
	SON_LIST_NODE       *p_list_node    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->sNode);
		if (SON_PNULL != p_list_node)
		{
			return (es_cell_context_node_t *)YMEMBEROF
				(es_cell_context_node_t, sNode, p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* es_get_next_node */

/*****************************************************************************
 * Function Name  : es_get_pending_res_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32
 * Description    : Returns the count of ctxt nodes present in pending response
 ******************************************************************************/
son_u32
es_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_es_pending_response.cell_sts_list);
}

/* -------------------ES 1.2 Changes Start ------------------------*/

/*****************************************************************************
 * Function Name  : es_print_switchon_pending_res
 * Inputs         : son_method_name - name of the invoking method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Print switchon_pending_res structure
 ****************************************************************************/
static son_void_t
es_print_switchon_pending_res
(
 const son_8 *son_method_name
 )
{
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_LOG(g_es_context.log_enable, p_son_es_facility_name, SON_DETAILED,
			"Pending Switch on Response Structure:"
			"API Id: %d Trans Id: %d Expected Response Count: %d Expiry time(Seconds): %lu"
             /*SPR 17777 +-*/
			"Expiry time(Micro Seconds): %lu"
            "son_method_name %s",
			g_es_switch_on_pending_res.api_id,
			g_es_switch_on_pending_res.transaction_id,
			g_es_switch_on_pending_res.expected_res_count,
			g_es_switch_on_pending_res.expiry_time.s,
			g_es_switch_on_pending_res.expiry_time.us,
            son_method_name);
        /*SPR 17777 +-*/

	p_node = get_first_list_node(
			&g_es_switch_on_pending_res.cell_switchon_status_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(
				cell_switchon_status_list_data_t,
				cell_switchon_status_node,
				p_node
				);
		SON_LOG(g_es_context.log_enable,
				p_son_es_facility_name, SON_BRIEF,
				"Cell [0x%x], with cell power state = %d, "
				"OAM Indication required = %d, "
				"Deactivation Indication = %d, "
				"Result = %d and Error code = %d",
				son_convert_char_cell_id_to_int_cell_id(p_list_node->
					cell_switchon_status.cgi.cell_identity),
				p_list_node->cell_switchon_status.cell_power_state,
				p_list_node->cell_switchon_status.oam_ind_required,
				p_list_node->cell_switchon_status.deactivation_indication,
				p_list_node->cell_switchon_status.result,
				p_list_node->cell_switchon_status.error_code);

		p_node = get_next_list_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* es_print_switchon_pending_res */

/*****************************************************************************
 * Function Name  : es_reset_switch_on_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Resets switchon_pending_res structure
 ****************************************************************************/
son_void_t
es_reset_switch_on_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	es_print_switchon_pending_res(
			"before reset");
	g_es_switch_on_pending_res.api_id = SON_PROCEDURE_UNDEFINED;
	g_es_switch_on_pending_res.transaction_id = SON_UNDEFINED;
	g_es_switch_on_pending_res.expected_res_count = MIN_PENDING_RES_COUNT;
	g_es_switch_on_pending_res.expiry_time.s = 0;
	g_es_switch_on_pending_res.expiry_time.us = 0;

	es_clear_switchon_pending_res_list();

	list_init(&g_es_switch_on_pending_res.cell_switchon_status_list);

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_BRIEF,
			"Pending Response Reset");

	es_print_switchon_pending_res(
			"after reset");
	SON_UT_TRACE_EXIT();
} /* es_reset_switch_on_pending_res */

/*****************************************************************************
 * Function Name  : es_init_switch_on_pending_res
 * Inputs         : tid     Transaction Id
 *                : api     Procedure Code
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize switchon_pending_res structure
 ****************************************************************************/
son_void_t
es_init_switch_on_pending_res
(
 son_u16 tid,
 son_procedure_code_et api
 )
{
	SON_UT_TRACE_ENTER();

	es_reset_switch_on_pending_res();
	g_es_switch_on_pending_res.api_id = api;
	g_es_switch_on_pending_res.transaction_id = tid;

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
			"Pending Response Initialized");

	es_print_switchon_pending_res("es_init_switch_on_pending_res");

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_switchon_pending_res_has_entry
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Returns true if entry found, otherwise false.
 ****************************************************************************/
son_bool_et
es_switchon_pending_res_has_entry
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_bool_et res = SON_FALSE;

	SON_UT_TRACE_ENTER();

	if (g_es_switch_on_pending_res.cell_switchon_status_list.count)
	{
		res = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return res;
}

/*****************************************************************************
 * Function Name  : es_get_switchon_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16
 * Description    : Returns transaction id of pending response
 ****************************************************************************/
son_u16
es_get_switchon_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_switch_on_pending_res.transaction_id;
}

/*****************************************************************************
 * Function Name  : es_get_switchon_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u8
 * Description    : Returns count of awaited responses from pending response
 ****************************************************************************/
son_u8
es_get_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_switch_on_pending_res.expected_res_count;
}

/*****************************************************************************
 * Function Name  : es_increment_switchon_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Increments expected response count in pending response
 ****************************************************************************/
son_void_t
es_increment_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_es_switch_on_pending_res.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		g_es_switch_on_pending_res.expected_res_count++;
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Max value of "
				"Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_decrement_switchon_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Decrements expected response count in Switch ON pending
 *                  response structure
 ****************************************************************************/
son_void_t
es_decrement_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_es_switch_on_pending_res.expected_res_count > MIN_PENDING_RES_COUNT)
	{
		g_es_switch_on_pending_res.expected_res_count--;
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Min value of "
				"Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_switchon_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et
 * Description    : Returns API ID present in Switch ON response structure
 ****************************************************************************/
son_procedure_code_et
es_get_switchon_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_switch_on_pending_res.api_id;
}

/*****************************************************************************
 * Function Name  : es_get_switchon_expiry_time
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_time_t
 * Description    : Returns the expiry time of pending switch on response struct
 ****************************************************************************/
son_time_t
es_get_switchon_expiry_time
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();

	return g_es_switch_on_pending_res.expiry_time;

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_set_switchon_expiry_time
 * Inputs         : son_time_t   current_time
 *                  son_u16      number of seconds
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Set timestamp in pending switch on response structure
 ****************************************************************************/
son_void_t
es_set_switchon_expiry_time
(
 son_time_t set_time,
 son_u16 sec
 )
{
	SON_UT_TRACE_ENTER();

	g_es_switch_on_pending_res.expiry_time.s = set_time.s + sec;
	g_es_switch_on_pending_res.expiry_time.us = set_time.us;

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_get_switchon_pending_res_cell_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer of type SON_LIST
 * Description    : Returns pointer to cell_status_list present in switch on
 *                  pending response
 ****************************************************************************/
SON_LIST *
es_get_switchon_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_es_switch_on_pending_res.cell_switchon_status_list;
}

/*****************************************************************************
 * Function Name  : es_find_cell_in_switchon_pending_res
 * Inputs         : p_rcvd_cell_id : Pointer to Cell ID
 * Outputs        : None
 * Returns        : pointer of type cell_switchon_status_list_data_t
 * Description    : Find cell_status node in the cell_status_list present in
 *                  switch on pending response struct
 ****************************************************************************/
cell_switchon_status_list_data_t *
es_find_cell_in_switchon_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
 )
{
	SON_LIST_NODE           *p_node                 = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node   = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_node = list_find_node(
			&g_es_switch_on_pending_res.cell_switchon_status_list,
			(son_void_t *)p_rcvd_cell_id,
			son_key_of_es, son_compare_cell_id);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_switchon_status_list_data_t,
				cell_switchon_status_node, p_node);
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell [0x%x] not found in switch on pending response list",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* es_find_cell_in_switchon_pending_res */

/*****************************************************************************
 * Function Name  : es_add_cell_in_switchon_pending_res
 * Inputs         : p_sts_node : Pointer to Cell switch on status Node
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Add cell_status node in the cell_status_list present in
 *                  switch on pending response
 ****************************************************************************/
son_bool_et
es_add_cell_in_switchon_pending_res
(
 son_cell_switchon_status_t   *p_sts_node
 )
{
	cell_switchon_status_list_data_t *p_list_node   = SON_PNULL;
	son_bool_et ret_val                 = SON_FALSE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_sts_node &&
			g_es_switch_on_pending_res.expected_res_count <
			MAX_PENDING_RES_COUNT)
	{
		p_list_node = son_mem_get(sizeof(cell_switchon_status_list_data_t));

		if (SON_PNULL != p_list_node)
		{
			SON_MEMCPY(&p_list_node->cell_switchon_status,
					p_sts_node,
					sizeof(son_cell_switchon_status_t));
			list_insert_node(&
					g_es_switch_on_pending_res.cell_switchon_status_list,
					&p_list_node->cell_switchon_status_node);

			es_increment_switchon_pending_res_expected_count();

			ret_val = SON_TRUE;
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Cell %x is added "
					"in Pending  Response",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->
						cell_switchon_status.cgi.cell_identity));

			es_print_switchon_pending_res(
					"es_add_cell_in_switchon_pending_res");
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Memory allocation failure");
		}
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Invalid input parameters[p_sts_node is NULL] "
				"or pending response count reached max limit[%d] ", MAX_PENDING_RES_COUNT);
	}

	SON_UT_TRACE_EXIT();

	return ret_val;
} /* es_add_cell_in_switchon_pending_res */

/*****************************************************************************
 * Function Name  : es_delete_cell_from_switchon_pending_res
 * Inputs         : p_cell_id   Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete cell_status node from the cell_status_list present
 *                  in switch on pending response
 ****************************************************************************/
son_void_t
es_delete_cell_from_switchon_pending_res
(
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	cell_switchon_status_list_data_t     *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_id)
	{
		p_node = es_find_cell_in_switchon_pending_res(p_cell_id);

		if (SON_PNULL != p_node)
		{
			list_delete_node(
					&g_es_switch_on_pending_res.cell_switchon_status_list,
					&p_node->cell_switchon_status_node);
			son_mem_free(p_node);

			es_decrement_switchon_pending_res_expected_count();

			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_BRIEF,
					"Deleting Cell [0x%x] from Pending Response List",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

			es_print_switchon_pending_res("After deletion in "
					"es_delete_cell_from_switchon_pending_res");

			if (!es_get_switchon_pending_res_expected_count())
			{
				SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
						SON_BRIEF,
						"Expected Response count has reached 0"
						" reseting Pending Response");
			}
		}
		else
		{
			SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
					SON_ERROR,
					"Node not found for cell [0x%x]",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* es_delete_cell_from_switchon_pending_res */

/*****************************************************************************
 * Function Name  : es_clear_switchon_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all cell_status node from the cell_status_list
 *                  present in switch on pending response
 ****************************************************************************/
son_void_t
es_clear_switchon_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE           *p_node      = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	p_node = list_pop_node(
			&g_es_switch_on_pending_res.cell_switchon_status_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_switchon_status_list_data_t,
				cell_switchon_status_node, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(
				&g_es_switch_on_pending_res.cell_switchon_status_list);
	}

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_DETAILED,
			"Pending Response list reset");

	es_print_switchon_pending_res("es_clear_switchon_pending_res_list");

	SON_UT_TRACE_EXIT();
} /* es_clear_switchon_pending_res_list */

/*****************************************************************************
 * Function Name  : es_update_cell_in_switchon_pending_res
 * Inputs         : p_rcvd_cell_id      Pointer to Cell ID
 *                : power_state         Cell Power State
 *                : oam_ind             Whetehr oam indication needs to be sent
 *                : deactivation_flag   Value of deactivation flag
 *                : res                 result
 *                : err_code            error_code
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : Update cell_status node in the cell_status_list present
 *                  in switch on pending response, for cell id, error code
 *                  and result.
 ****************************************************************************/
son_bool_et
es_update_cell_in_switchon_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
 es_cell_power_state_et power_state,
 son_bool_et oam_ind,
 x2_deactivation_flag_et deactivation_flag,
 son_return_et res,
 son_error_et err_code
 )
{
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;
	son_bool_et ret_val = SON_FALSE;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
		return ret_val;
	}

	p_list_node = es_find_cell_in_switchon_pending_res(p_rcvd_cell_id);

	if (p_list_node)
	{
		p_list_node->cell_switchon_status.cell_power_state = power_state;
		p_list_node->cell_switchon_status.oam_ind_required = oam_ind;
		p_list_node->cell_switchon_status.
			deactivation_indication = deactivation_flag;
		p_list_node->cell_switchon_status.result = res;
		p_list_node->cell_switchon_status.error_code = err_code;
		ret_val = SON_TRUE;

		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_BRIEF,
				"Pending Response List "
				" context updated for cell [0x%x] with result = %d and error"
				" code = %d",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity), res, err_code);
		es_print_switchon_pending_res("es_update_cell_in_switchon_pending_res");
	}
	else
	{
		SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
				SON_ERROR,
				"Cell [0x%x] not found",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_update_cell_in_switchon_pending_res */

/*****************************************************************************
 * Function Name  : es_send_feature_state_change_ind
 * Inputs         : p_cell_id - Pointer to the cell Id
 *                  state - ES state for the cell
 *                  cause - Cause for the present state of ES for the cell
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates and sends the feature state
 *                  change indication message to SONMgmtIfH.
 ****************************************************************************/
son_void_t
es_send_feature_state_change_ind
(
 son_intra_rat_global_cell_id_t      *p_cell_id,
 son_feature_state_et state,
 son_feature_state_change_cause_et cause
 )
{
	son_es_feature_state_change_ind_t state_change_ind;
	son_size_t state_change_ind_len =
		sizeof(son_es_feature_state_change_ind_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&state_change_ind, 0,
			state_change_ind_len);
	/* Copy the CGI */
	SON_MEMCPY(&state_change_ind.srv_cgi, p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Set the Module ID */
	state_change_ind.module_id = SON_ES_MODULE_ID;
	state_change_ind.state = state;
	state_change_ind.cause = cause;

	son_create_send_buffer((son_u8 *)&state_change_ind,
			SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONES_FEATURE_STATE_CHANGE_IND,
			state_change_ind_len);

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_BRIEF,
			"Feature state change "
			"indication sent for cell[0x%x]",
			son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

	SON_UT_TRACE_EXIT();
} /* es_send_feature_state_change_ind */

/* Rel 3.0 Multi Sector code changes Start */
/*****************************************************************************
 * Function Name  : es_send_sonanr_cell_switch_on_off_info_ind
 * Inputs         : p_cell_id - Pointer to the cell Id
 *                  cell_on_off_status - Cell switched ON/OFF status
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates and sends the cell switch ON/OFF 
 *                  indication message to SON ANR.
 ****************************************************************************/
son_void_t
es_send_sonanr_cell_switch_on_off_info_ind
(
 son_intra_rat_global_cell_id_t  *p_cell_id,
 es_cell_switch_on_off_status_et cell_on_off_status
 )
{
	sonanr_cell_switch_on_off_ind_t switch_on_off_ind;
	son_size_t switch_on_off_ind_len =
		sizeof(sonanr_cell_switch_on_off_ind_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&switch_on_off_ind, 0, switch_on_off_ind_len);
	/* Copy the CGI */
	SON_MEMCPY(&switch_on_off_ind.src_cgi, p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	switch_on_off_ind.cell_on_off_status = cell_on_off_status;

	son_create_send_buffer((son_u8 *)&switch_on_off_ind,
			SON_ES_MODULE_ID,
			SON_ANR_MODULE_ID,
			SONANR_CELL_SWITCH_ON_OFF_IND,
			switch_on_off_ind_len);

	SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
			SON_BRIEF,
			"Cell switch ON/OFF "
			"indication sent for cell[0x%x]",
			son_convert_char_cell_id_to_int_cell_id(switch_on_off_ind.src_cgi.
				cell_identity));

	SON_UT_TRACE_EXIT();
} /* es_send_sonanr_cell_switch_on_off_info_ind */
/* Rel 3.0 Multi Sector code changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : es_get_log_level
 * Inputs         : None
 * Outputs        : None
 * Returns        : Log Level of the ES
 * Description    : This function gets the log level
 ******************************************************************************/
son_u8
es_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_es_context.log_level;
}

/*****************************************************************************
 * Function Name  : es_set_log_level
 * Inputs         : log_level - Log Level to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log level
 ******************************************************************************/
son_void_t
es_set_log_level
(
 son_u8 log_level
 )
{
	SON_UT_TRACE_ENTER();
	g_es_context.log_level = log_level;
	SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */
