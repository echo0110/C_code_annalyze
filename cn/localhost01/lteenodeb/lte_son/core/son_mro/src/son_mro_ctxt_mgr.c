/******************************************************************************G
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_mro_ctxt_mgr.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for managing MRO Module's
 *        Global context and cell specific context.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Dec, 2012       Shilpi      Initial         Initial
 *   June, 2014      Shilpi                      SPR 11639 Fix
 *   Aug, 2014       Shilpi                      SPR 13251 Fix
 ******************************************************************************/
/* Includes */
#include <son_utils.h>
#include <son_mro_ctxt_mgr.h>
#include <son_mro_cell_fsm.h>
#include <son_anr_utils.h>
#include <son_rrm_composer.h>

extern const son_8 *p_son_mro_facility_name;

/*****************************************************************************
 *  Name  : MRO Global Context.
 ****************************************************************************/
mro_global_context_t  g_mro_gb_ctxt;

/*****************************************************************************
 *  Name  : Global Pending Response.
 ****************************************************************************/
pending_res_t g_mro_pending_response;

/* SONMRO FSM state names - for MRO Global FSM*/
const son_s8 *SON_MRO_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONMRO_STATE_INIT",
	(const son_s8 *)"SONMRO_STATE_ACTIVE",
	(const son_s8 *)"SONMRO_STATE_ENABLING",
	(const son_s8 *)"SONMRO_STATE_DISABLING",
	(const son_s8 *)"SONMRO_STATE_SHUTDOWN",
	(const son_s8 *)"SONMRO_STATE_UNDEFINED"
};

/* SONMRO FSM state names - for Cell level FSM*/
const son_s8 *SON_MRO_CELL_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONMRO_CELL_STATE_DISABLED",
	(const son_s8 *)"SONMRO_CELL_STATE_ENABLING",
	(const son_s8 *)"SONMRO_CELL_STATE_ENABLED",
	(const son_s8 *)"SONMRO_CELL_STATE_DISABLING",
	(const son_s8 *)"SONMRO_CELL_STATE_UNDEFINED"
};

/* Expected SONMRO Global FSM Event names */
const son_s8 *SON_MRO_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"SMIF_INIT_CONFIG_REQ",
	(const son_s8 *)"SMIF_SET_LOG_LEVEL_CMD",
	(const son_s8 *)"SMIF_LOG_ENABLE_DISABLE_CMD",
	(const son_s8 *)"SMIF_CELL_INFO_IND",
	(const son_s8 *)"SMIF_SHUTDOWN_REQ",
	(const son_s8 *)"SONMRO_ENABLE_REQ",
	(const son_s8 *)"SONMRO_DISABLE_REQ",
	(const son_s8 *)"SONMRO_MODIFY_CONFIG_PARAM_REQ",
	(const son_s8 *)"RRM_SON_REGISTER_RESP",
	(const son_s8 *)"RRM_SON_DEREGISTER_RESP",
	(const son_s8 *)"RRM_SON_HO_FAILURE_REPORT",
	(const son_s8 *)"RRM_SON_HO_ATTEMPT_IND",
	(const son_s8 *)"RRM_SON_CELL_STATE_CHANGE_IND",
	(const son_s8 *)"SONANR_DISABLE_IND",
	(const son_s8 *)"SONX2_HO_REPORT_IND",
	(const son_s8 *)"SONMLB_NBR_CELL_STATUS_IND",
	/* Rel 3.0 Cell delete support changes Start */
	(const son_s8 *)"SMIF_CELL_DELETE_REQ",
	/* Rel 3.0 Cell delete support changes Stop */
	/* SPR-13251 Fix Starts */
	(const son_s8 *)"SMIF_GET_LOG_LEVEL_REQ",
	/* SPR-13251 Fix Ends */
	(const son_s8 *)"NOT_AVAILABLE"
};

/* Expected SONMRO Cell FSM Event names */
const son_s8 *SON_MRO_CELL_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"MRO_CELL_SONMRO_EVENT_ENABLE_REQ",
	(const son_s8 *)"MRO_CELL_SONRMO_EVENT_DISABLE_REQ",
	(const son_s8 *)"MRO_CELL_SONMRO_EVENT_MODIFY_CONFIG_REQ",
	(const son_s8 *)"MRO_CELL_RRM_EVENT_REGISTER_RES",
	(const son_s8 *)"MRO_CELL_RRM_EVENT_DEREGISTER_RES",
	(const son_s8 *)"MRO_CELL_RRM_EVENT_HO_FAILURE_REPORT",
	(const son_s8 *)"MRO_CELL_RRM_EVENT_HO_ATTEMPT_IND",
	(const son_s8 *)"MRO_CELL_RRM_EVENT_CELL_STATE_CHANGE_IND",
	(const son_s8 *)"MRO_CELL_SONANR_EVENT_CELL_ANR_MODE_IND",
	(const son_s8 *)"MRO_CELL_X2AP_EVENT_HO_REPORT_IND",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* Possible TTT Enum value names */
const son_s8 *TTT_NAMES[] =
{
	(const son_s8 *)"SON_TTT_MS_0",
	(const son_s8 *)"SON_TTT_MS_40",
	(const son_s8 *)"SON_TTT_MS_64",
	(const son_s8 *)"SON_TTT_MS_80",
	(const son_s8 *)"SON_TTT_MS_100",
	(const son_s8 *)"SON_TTT_MS_128",
	(const son_s8 *)"SON_TTT_MS_160",
	(const son_s8 *)"SON_TTT_MS_256",
	(const son_s8 *)"SON_TTT_MS_320",
	(const son_s8 *)"SON_TTT_MS_480",
	(const son_s8 *)"SON_TTT_MS_512",
	(const son_s8 *)"SON_TTT_MS_640",
	(const son_s8 *)"SON_TTT_MS_1024",
	(const son_s8 *)"SON_TTT_MS_1280",
	(const son_s8 *)"SON_TTT_MS_2560",
	(const son_s8 *)"SON_TTT_MS_5120",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* SPR-11639 Fix Starts */
/* CIO_NAMES definiton deleted from here */
extern const son_s8 *CIO_NAMES[];
/* SPR-11639 Fix Ends */

const son_s8 *MRO_ALGO_DIR[] =
{
	(const son_s8 *)"SON_TOO_EARLY",
	(const son_s8 *)"SON_TOO_LATE",
	(const son_s8 *)"SON_NO_OPTIMIZATION_DIR",
	(const son_s8 *)"NOT_AVAILABLE"
};

/*  SON_MRO_PARAM_NAMES- Stores son_mro_param_et enum possible values as 
 *  strings */
const son_s8 *SON_MRO_PARAM_NAMES[] =
{
	(const son_s8 *)"SON_MRO_CIO",
	(const son_s8 *)"SON_MRO_TTT",
	(const son_s8 *)"SON_MRO_CIO_TTT",
	(const son_s8 *)"NOT_AVAILABLE"
};

/*****************************************************************************
 * Function Name  : mro_set_log_mode
 * Inputs         : log_mode
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function sets the log mode for MRO module
 ******************************************************************************/
son_void_t
mro_set_log_mode
(
 son_oam_log_on_off_et log_mode
 )
{
	SON_UT_TRACE_ENTER();
	g_mro_gb_ctxt.log_mode = log_mode;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function name  : mro_get_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16 - Transaction Id
 * Description    : Provides transaction id present in MRO pending response
 ****************************************************************************/
son_u16
mro_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_mro_pending_response.transaction_id;
}

/*****************************************************************************
 * Function Name  : mro_print_pending_res
 * Inputs         : son_method_name - Invoking method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Prints MRO pending_response structure
 ****************************************************************************/
son_void_t 
mro_print_pending_res
(
 const son_8 *son_method_name
 )
{
	SON_LIST_NODE           *p_node      = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
			SON_BRIEF,
			"%s: Pending Response Structure: API Id: %u Transaction Id: %u "
			"Expected Response Count: %d Error Received Flag: %d Number of nodes in pending response list %u",
			son_method_name,
			g_mro_pending_response.api_id,
			g_mro_pending_response.transaction_id,
			g_mro_pending_response.expected_res_count,
			g_mro_pending_response.is_error_received,
			list_count(&g_mro_pending_response.cell_sts_list));

	p_node = get_first_list_node(&g_mro_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_WARNING,
				"%s: Status for Cell 0x%x in Pending Response is "
				"error code = %u",
				son_method_name,
				son_convert_char_cell_id_to_int_cell_id(
					p_list_node->cell_status.cgi.cell_identity),
				p_list_node->cell_status.error_code);
		p_node = get_next_list_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* mro_print_pending_res */

/*****************************************************************************
 * Function Name  : mro_init_pending_resp_list
 * Inputs         : tid - Transaction Id
 *                : api - Procedure Code
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize MRO pending_response structure
 ****************************************************************************/
son_void_t
mro_init_pending_resp_list
(
 son_u16                  tid,
 son_procedure_code_et    api
 )
{
	SON_UT_TRACE_ENTER();
	g_mro_pending_response.api_id = api;
	g_mro_pending_response.transaction_id = tid;
	g_mro_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_mro_pending_response.is_error_received = SON_FALSE;
	list_init(&g_mro_pending_response.cell_sts_list);
	SON_MEMSET(&g_mro_pending_response.ts, 0, sizeof(time_stamp_t));
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_DETAILED,
			"MRO Pending Response Initialized");

	mro_print_pending_res("mro_init_pending_resp_list"); 

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : mro_reset_pending_resp_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Resets MRO pending_response structure
 ****************************************************************************/
son_void_t
mro_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	g_mro_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
	g_mro_pending_response.transaction_id = 0;
	g_mro_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_mro_pending_response.is_error_received = SON_FALSE;
	if (g_mro_pending_response.cell_sts_list.count)
	{
		mro_clear_pending_res_list();
	}

	list_init(&g_mro_pending_response.cell_sts_list);
	SON_MEMSET(&g_mro_pending_response.ts, 0, sizeof(time_stamp_t));
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_DETAILED,
			"MRO Pending Response has been reset");
	mro_print_pending_res("mro_reset_pending_resp_list"); 

	SON_UT_TRACE_EXIT();
} /* mro_reset_pending_resp_list */

/* Rel 3.0 Multi Sector code changes Start */

/*SPR-11554-Start-Function Modified*/

/*****************************************************************************
 * Function Name  : mro_validate_cell_data
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : None
 * Returns        : SON_TRUE: If valid
 *                  SON_FALSE:If not valid
 * Description    : This function validates the data of cell received in cell
 *                  info indication
 ******************************************************************************/
static son_bool_et
mro_validate_cell_data
(
 son_cell_t                          *p_cell
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
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"Invalid pci range[open_pci_start=%d, open_pci_end=%d] received",
                    p_cell->intra_freq_open_pci_range.pci_start,
                    temp_open_pci_end);

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
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"Invalid pci range[closed_pci_start=%d, closed_pci_end=%d] received",
                    p_cell->intra_freq_closed_pci_range.pci_start,
                    temp_closed_pci_end);

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
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"Invalid pci range[hybrid__pci_start=%d, hybridpci_end=%d] received",
                    p_cell->intra_freq_hybrid_pci_range.pci_start,
                    temp_hybrid_pci_end);

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

	/* Check if it is the first cell being configured at MRO */
	if (0 == g_mro_gb_ctxt.mro_cell_ctxt_list.count)
	{
		/*Check if the serving cell is home or macro*/
		if (is_home_eNB == SON_TRUE)
		{
			g_mro_gb_ctxt.enb_type = SON_HOME_ENB;
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
					SON_INFO,
					"The serving eNB is a home eNB");
		}
		else
		{
			g_mro_gb_ctxt.enb_type = SON_MACRO_ENB;
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
					SON_INFO,
					"The serving eNB is a macro eNB");
		}
	}
	/* Check if 1 cell is already configured and if the eNB is
	 * home eNB do not configure any other cell as home eNB can
	 * have only one serving cell */
	else if (SON_HOME_ENB == g_mro_gb_ctxt.enb_type)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"Cell 0x%x"
				" is not added to ANR Global Context - home eNB is"
				" already configured with 1 serving cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}
	else if ((SON_MACRO_ENB ==  g_mro_gb_ctxt.enb_type) &&
			(is_home_eNB == SON_TRUE)) /*Coverity CID 64969 Fix Start-End*/
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"Cell 0x%x"
				" is not added to ANR Global Context - macro eNB "
				" eNB is already configured as macro,cannot configure femto cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_valid;
} /* mro_validate_cell_data */

/*SPR-11554-End*/

/* Rel 3.0 Multi Sector code changes Stop */

/*****************************************************************************
 * Function Name  : mro_add_new_cell_in_context
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : p_cause - Cause of failure if failure occurs in addition
 *                  p_state - MRO feature state for the cell
 * Returns        : Pointer to the new node added in the list, NULL if node
 *                  already exists or creation of node fails
 * Description    : This function adds new cell in the MRO cell context
 ******************************************************************************/
mro_cell_context_node_t *
mro_add_new_cell_in_context
(
 son_cell_t                          *p_cell,
 son_feature_state_et                *p_state,
 /*SPR_19279_START */
 son_feature_state_change_cause_et   *p_cause,
 son_u8 cell_index
 /*SPR_19279_END */
 )
{
	mro_cell_context_node_t *p_new_node = SON_PNULL;
	son_size_t cell_context_node_size = sizeof(mro_cell_context_node_t);
	SON_LIST_NODE *p_list_node = SON_PNULL;
	mro_cell_context_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell || SON_PNULL == p_cause || 
			SON_PNULL == p_state)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
				SON_ERROR,
				"Cell ID or pointer to cause is NULL");
	}
	else 
	{
		/* Check if Cell is already present in MRO cell configuration list */
		p_list_node = list_find_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
                /*SPR_19279_START */
				&cell_index, son_cell_index_key_of_mro,
				son_compare_cell_index);
                /*SPR_19279_END */

		if (SON_PNULL == p_list_node)
		{
			if (g_mro_gb_ctxt.mro_cell_ctxt_list.count < 
					SON_MAX_CELL_SUPPORTED_AT_ENB)
			{
				/* Rel 3.0 Multi Sector code changes Start */
				if (SON_FALSE == mro_validate_cell_data(p_cell))
				{
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_INTERNAL_FAILURE;
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}
				/* Rel 3.0 Multi Sector code changes Stop */

				/* Create a new node */       
				p_new_node = (mro_cell_context_node_t *)son_mem_get(
						cell_context_node_size);

				if (SON_PNULL != p_new_node)
				{
					/* Populate the node*/ 
					SON_MEMSET(p_new_node, 0, cell_context_node_size);
                    /*SPR_19279_START */
                    p_new_node->data.cell_index = cell_index;
                    /*SPR_19279_END */
					SON_MEMCPY(&p_new_node->data.cell_id, &p_cell->src_cgi,
							sizeof(son_intra_rat_global_cell_id_t));
					p_new_node->data.previous_cell_fsm_state = 
						MRO_CELL_STATE_DISABLED;
					p_new_node->data.current_cell_fsm_state = 
						MRO_CELL_STATE_DISABLED;
					SON_MEMCPY(&p_new_node->data.mro_config_param, 
							&g_mro_gb_ctxt.mro_config_param, 
							sizeof(son_mro_config_param_t));
					p_new_node->data.dereg_invoking_api = 
						SON_PROCEDURE_UNDEFINED;
					p_new_node->data.ho_failure_accumulation_duration_timer_id
						= SON_PNULL;
					p_new_node->data.srvd_cell_algo_direction =
						SON_NO_OPTIMIZATION_DIR;
					p_new_node->data.is_ttt_algo_running = SON_FALSE;
					p_new_node->data.previous_ttt = SON_TTT_MS_512;
					p_new_node->data.retry_count = 0;
					p_new_node->data.previous_ho_fail_rate = 0;
					p_new_node->data.is_ttt_update_reqd = SON_FALSE;
					/* Insert it in context list */
					list_insert_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
							&p_new_node->sNode);
					*p_state = SON_STATE_DISABLED;
					*p_cause = SON_MRO_MODE_DISABLED;
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_INFO,
							"Added Cell 0x%x in MRO Global Context",
							son_convert_char_cell_id_to_int_cell_id(
								p_cell->src_cgi.cell_identity));
					mro_print_cell_context("mro_add_new_cell_in_context");
				}
				else
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_ERROR,
							"Memory allocation failure");
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_MEM_ALLOC_FAILURE;
				}
			}
			else
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_INFO,
						"Cell 0x%x could not be added to MRO Global Context "
						"Maximum(%u) number of supported cells limit reached",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell->src_cgi.cell_identity),
						list_count(&g_mro_gb_ctxt.mro_cell_ctxt_list));
				*p_state = SON_STATE_DELETED;
				*p_cause = SON_MAX_SUPPORTED_CELL_LIMIT_REACHED;
			}
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"Cell 0x%x already exists in MRO Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell->src_cgi.cell_identity));
			p_data = YMEMBEROF(mro_cell_context_node_t, sNode, p_list_node);
            /*SPR_19279_START*/
            if (SON_MEMCMP(&p_data->data.cell_id,
                        &p_cell->src_cgi, sizeof(p_new_node->data.cell_id)))
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_INFO,
                        "ECGI for cell Cell 0x%x updated",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

                SON_MEMCPY(&p_data->data.cell_id,
                        &p_cell->src_cgi,
                        sizeof(p_data->data.cell_id));
            }
            /*SPR_19279_END*/


			if (MRO_CELL_STATE_DISABLED == p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_DISABLED;
			}
			else if (MRO_CELL_STATE_ENABLED == 
					p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_ENABLED;
			}
			else if (MRO_CELL_STATE_ENABLING == 
					p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_DISABLED;
			}
			else if (MRO_CELL_STATE_DISABLING == 
					p_data->data.current_cell_fsm_state)
			{
				*p_state = SON_STATE_ENABLED;
			}

			*p_cause = SON_CELL_ALREADY_EXISTS;
		}
	}

	SON_UT_TRACE_EXIT();
	return p_new_node;
} /* mro_add_new_cell_in_context */

/*****************************************************************************
 * Function Name  : son_key_of_mro
 * Inputs         : p_node - anchor field of the record
 * Outputs        : None
 * Returns        : pointer to cell id as void pointer
 * Description    : This function determines the key in MRO
 ******************************************************************************/
const son_void_t *
son_key_of_mro
(
 const SON_LIST_NODE *p_node
 )
{
	mro_cell_context_node_t *p_data = SON_PNULL;
	son_intra_rat_global_cell_id_t  *p_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_node)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Pointer to node is NULL");
	}
	else
	{
		p_data = YMEMBEROF(mro_cell_context_node_t, sNode, p_node);
		p_cell_id = &(p_data->data.cell_id);
	}    

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_id;
} /* son_key_of_mro */
/*SPR_19279_START*/

const son_void_t *
son_cell_index_key_of_mro
(
 const SON_LIST_NODE *p_node
 )
{
	mro_cell_context_node_t *p_data = SON_PNULL;
	son_u8  *p_cell_idx = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(mro_cell_context_node_t, sNode, p_node);
	p_cell_idx = &(p_data->data.cell_index);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_idx;
}
/*SPR_19279_END*/


/*****************************************************************************
 * Function Name  : mro_print_global_context
 * Inputs         : son_method_name - Invoking Method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the MRO global context
 ******************************************************************************/
son_void_t
mro_print_global_context
(
 const son_8 *son_method_name
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_BRIEF,
			"%s: MRO Global Context Structure:\n"
			"Current FSM State %s MRO Global Mode %u MRO Log Mode %u MRO pending response timer Id 0x%x Mode of SON MRO feature %s\n"
			"MRO Parameter for optimization %s HO Failure accmulation duration %u Min Threshold HO attempts %u MRO Start Threshold %f"
			"MRO Stop Threshold %f HO Fail Threshold Rate for optimization %f\n"
			"Delta CIO %u Time to Trigger %u Delta TTT %u MRO Expected HO improvement for single step %f Minimum valid TTT %s"
			" Maximum TTT %s Minimum CIO %s Maximum CIO %s No of Cell configured at MRO is %u",
			son_method_name,
			SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
			g_mro_gb_ctxt.mro_global_mode,
			g_mro_gb_ctxt.log_mode,
			g_mro_gb_ctxt.mro_pending_res_timer_id,
			SON_MODE_NAMES[g_mro_gb_ctxt.mro_config_param.mro_mode],
			SON_MRO_PARAM_NAMES[g_mro_gb_ctxt.mro_config_param.mro_param],
			(son_u16)(g_mro_gb_ctxt.mro_config_param.mro_ho_failure_accumulation_duration),
			(son_u16)(g_mro_gb_ctxt.mro_config_param.mro_ho_attempts_threshold),
			g_mro_gb_ctxt.mro_config_param.mro_ho_optimization_start_threshold,
			g_mro_gb_ctxt.mro_config_param.mro_ho_optimization_stop_threshold,
			g_mro_gb_ctxt.mro_config_param.mro_ho_fail_rate_for_optimization,
			g_mro_gb_ctxt.mro_config_param.delta_cio,
			g_mro_gb_ctxt.mro_config_param.ttt,
			g_mro_gb_ctxt.mro_config_param.delta_ttt,
			g_mro_gb_ctxt.mro_config_param.
			mro_expected_ho_improvement_for_single_step,
			TTT_NAMES[g_mro_gb_ctxt.mro_config_param.min_ttt],
			TTT_NAMES[g_mro_gb_ctxt.mro_config_param.max_ttt],
			CIO_NAMES[g_mro_gb_ctxt.mro_config_param.min_cio],
			CIO_NAMES[g_mro_gb_ctxt.mro_config_param.max_cio],
			g_mro_gb_ctxt.mro_cell_ctxt_list.count);


	mro_print_cell_context(son_method_name);
	SON_UT_TRACE_EXIT();
} /* mro_print_global_context */

/*****************************************************************************
 * Function Name  : mro_get_first_cell_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to cell context of first cell in list
 * Description    : This function gets the cell context of the first cell 
 *                : in the MRO cell context list
 ******************************************************************************/
mro_cell_context_node_t *
mro_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (0 != list_count(&g_mro_gb_ctxt.mro_cell_ctxt_list))
	{
		p_node = get_first_list_node(&g_mro_gb_ctxt.mro_cell_ctxt_list);
		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (mro_cell_context_node_t *)
				YMEMBEROF(mro_cell_context_node_t, sNode, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* mro_get_first_cell_ctxt */

/*****************************************************************************
 * Function Name  : mro_print_cell_context
 * Inputs         : son_method_name - Invoking Method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the MRO Cell contexts list
 ******************************************************************************/
son_void_t
mro_print_cell_context
(
 const son_8 *son_method_name
 )
{
	mro_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = mro_get_first_cell_ctxt();
	if (SON_PNULL == p_node)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_DETAILED,
				"%s Number of cells configured at MRO = 0.",
				son_method_name);
		SON_UT_TRACE_EXIT();
		return;
	}

	while (SON_PNULL != p_node)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_INFO,
				"Cell id 0x%x previous cell fsm state %s current cell fsm state %s"
				"De-registration invoking API %u HO Failure accumulation duration timer Id %lu"
				"Mode of SON MRO feature %s MRO parameter selected for optimization %s"
				" Ho failure accmulation duration %u min"
				" Threshold ho attempts %u MRO start threshold %f MRO stop threshold %f"
				" HO fail threshold rate for optimization %f "
				"Time to Trigger %u delta cio %u delta ttt %u"
				" MRO Expected HO improvement for single step %f\n"
				"Minimum valid TTT %s Maximum valid TTT %s Minimum valid CIO %s Maximum valid CIO %s Served Cell algorithm direction %s"
				" Is TTT Algo running %u Previous TTT %s Retry Count %u Previous HO Failure rate %f Is TTT update required %u",
				son_convert_char_cell_id_to_int_cell_id(
					p_node->data.cell_id.cell_identity),
				SON_MRO_CELL_FSM_STATES_NAMES[p_node->data.
				previous_cell_fsm_state],
				SON_MRO_CELL_FSM_STATES_NAMES[p_node->data.
				current_cell_fsm_state],
				p_node->data.dereg_invoking_api,
				((son_ul32)p_node->data.ho_failure_accumulation_duration_timer_id),
				SON_MODE_NAMES[p_node->data.mro_config_param.mro_mode],
				SON_MRO_PARAM_NAMES[p_node->data.mro_config_param.mro_param],
				(son_u16)(p_node->data.mro_config_param.
					mro_ho_failure_accumulation_duration),
				p_node->data.mro_config_param.
				mro_ho_attempts_threshold,
				p_node->data.mro_config_param.
				mro_ho_optimization_start_threshold,
				p_node->data.mro_config_param.
				mro_ho_optimization_stop_threshold,
				p_node->data.mro_config_param.mro_ho_fail_rate_for_optimization,
				p_node->data.mro_config_param.ttt,
				p_node->data.mro_config_param.delta_cio,
				p_node->data.mro_config_param.delta_ttt,
				p_node->data.mro_config_param.
				mro_expected_ho_improvement_for_single_step,
				TTT_NAMES[g_mro_gb_ctxt.mro_config_param.min_ttt],
				TTT_NAMES[g_mro_gb_ctxt.mro_config_param.max_ttt],
				CIO_NAMES[g_mro_gb_ctxt.mro_config_param.min_cio],
				CIO_NAMES[g_mro_gb_ctxt.mro_config_param.max_cio],
				MRO_ALGO_DIR[p_node->data.srvd_cell_algo_direction],
				p_node->data.is_ttt_algo_running,
				TTT_NAMES[p_node->data.previous_ttt],
				p_node->data.retry_count,
				p_node->data.previous_ho_fail_rate,
				p_node->data.is_ttt_update_reqd);


		p_node = mro_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* mro_print_cell_context */

/*****************************************************************************
 * Function Name  : mro_get_next_node
 * Inputs         : p_node   Pointer to the current cell context
 * Outputs        : None
 * Returns        : cell context pointer of the next cell
 * Description    : This function gets the  next MRO cell context in the list
 ******************************************************************************/
mro_cell_context_node_t *
mro_get_next_node
(
 mro_cell_context_node_t *p_node
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->sNode);
		if (SON_PNULL != p_list_node)
		{
			return (mro_cell_context_node_t *)
				YMEMBEROF(mro_cell_context_node_t, sNode, p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* mro_get_next_node */

/*****************************************************************************
 * Function Name  : mro_add_cell_in_pending_res
 * Inputs         : p_sts_node : Pointer to Cell Status Node
 * Outputs        : None
 * Returns        : SON_SUCCESS - In case of successful addition of Node,
 *                  SON_FAILURE - Otherwise
 * Description    : Add cell_status node in the MRO pending response list 
 ****************************************************************************/
son_return_et
mro_add_cell_in_pending_res
(
 son_cell_status_t *p_sts_node
 )
{
	cell_status_list_data_t *p_list_node = SON_PNULL;
	son_return_et ret_val = SON_FAILURE;
	son_size_t list_node_size = sizeof(cell_status_list_data_t);

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_sts_node && 
			g_mro_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		p_list_node = (cell_status_list_data_t *)son_mem_get(list_node_size);
		if (SON_PNULL != p_list_node)
		{
			SON_MEMSET(p_list_node, 0, list_node_size);
			SON_MEMCPY(&p_list_node->cell_status, p_sts_node,
					sizeof(son_cell_status_t));
			list_insert_node(&g_mro_pending_response.cell_sts_list,
					&p_list_node->cell_status_node);

			mro_increment_pending_res_expected_count();
			ret_val = SON_SUCCESS;

			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_DETAILED,
					"Added cell "
					"Cell 0x%x in Pending Response list." 
					" Number of cells in Pending Response list: %u",
					son_convert_char_cell_id_to_int_cell_id(
						p_sts_node->cgi.cell_identity),
					g_mro_pending_response.expected_res_count);
			mro_print_pending_res("mro_add_cell_in_pending_res");
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"Memory allocation failure");
		}
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Invalid input parameter[p_sts_node is NULL] or "
				"pending response count(%u) reached max limit(%u)",
				g_mro_pending_response.expected_res_count,
				MAX_PENDING_RES_COUNT);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* mro_add_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : mro_increment_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et- Success/failure
 * Description    : Increment Expected Response Count present in pending 
 *                  response by 1
 ****************************************************************************/
son_return_et
mro_increment_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();
	g_mro_pending_response.expected_res_count++;
	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
}

/*****************************************************************************
 * Function Name  : mro_decrement_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et- Success/failure
 * Description    : Decrement Expected Response Count present in pending 
 *                  response by 1
 ****************************************************************************/
son_return_et
mro_decrement_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();

	if (MIN_PENDING_RES_COUNT < g_mro_pending_response.expected_res_count)
	{
		g_mro_pending_response.expected_res_count--;
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Min value"
				" of Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* mro_decrement_pending_res_expected_count */

/*****************************************************************************
 * Function Name  : mro_get_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : Number of expected response count
 * Description    : Provides the expected response count present in MRO pending 
 *                  response list
 ****************************************************************************/
son_u8
mro_get_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_mro_pending_response.expected_res_count;
}

/*****************************************************************************
 * Function Name  : mro_is_any_cell_disabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in MRO_CELL_STATE_DISABLED 
 *                  or MRO_CELL_STATE_DISABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in 
 *                  MRO_CELL_STATE_DISABLED or MRO_CELL_STATE_DISABLING state.
 *                  If such cell is found it indicates that global enable 
 *                  request can be processed as some cells needs to be enabled.
 ****************************************************************************/
son_bool_et
mro_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_bool_et cell_exists = SON_FALSE;
	mro_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = mro_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		if (MRO_CELL_STATE_DISABLED == p_node->data.current_cell_fsm_state
				|| MRO_CELL_STATE_DISABLING == 
				p_node->data.current_cell_fsm_state)
		{
			cell_exists = SON_TRUE;
			break;
		}

		p_node = mro_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
	return cell_exists;
} /* mro_is_any_cell_disabled */

/*****************************************************************************
 * Function Name  : mro_is_any_cell_enabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in MRO_CELL_STATE_ENABLED 
 *                  or MRO_CELL_STATE_ENABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in 
 *                  MRO_CELL_STATE_ENABLED or MRO_CELL_STATE_ENABLING state.
 *                  If such cell is found it indicates that global disable 
 *                  request can be processed as some cells needs to be disabled.
 ****************************************************************************/
son_bool_et
mro_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_bool_et cell_exists = SON_FALSE;
	mro_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = mro_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		if (MRO_CELL_STATE_ENABLED == p_node->data.current_cell_fsm_state
				|| MRO_CELL_STATE_ENABLING == 
				p_node->data.current_cell_fsm_state)    
		{
			cell_exists = SON_TRUE;
			break;
		}

		p_node = mro_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
	return cell_exists;
} /* mro_is_any_cell_enabled */

/*****************************************************************************
 * Function Name  : mro_populate_enable_disable_res_failure_data_from_pending_list
 * Inputs         : None
 * Outputs        : p_cell_status_list_size - Number of cells to be send in 
 *                  response
 *                  p_cell_status_list  - Pointer to list containg cell Ids and 
 *                  error code indicating the reason for failure
 * Returns        : son_void_t
 * Description    : This function populates enable or disable response data 
 *                  struture with the cell Ids and the error codes of the cell 
 *                  for which (de)registration request could not be sent to 
 *                  RRM due to some SONMRO error or may be because ANR mode is 
 *                  not enabled for the cell or cell context is not found at 
 *                  ANR.
 ****************************************************************************/
son_void_t
mro_populate_enable_disable_res_failure_data_from_pending_list
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
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"p_cell_status_list_size or p_cell_status_list is NULL");

		SON_UT_TRACE_EXIT();
		return;
	}

	p_node = get_first_list_node(&g_mro_pending_response.cell_sts_list);
	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_DETAILED,
				"Status for Cell 0x%x in Pending Response is " 
				"error code = %u",
				son_convert_char_cell_id_to_int_cell_id(
					p_list_node->cell_status.cgi.cell_identity),
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
} /* mro_populate_enable_disable_res_failure_data_from_pending_list */

/*****************************************************************************
 * Function Name  : mro_get_cell_context_from_global_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : Pointer to the cell context 
 * Description    : This function gets the cell context of the specified cell 
 *                  from MRO's global context
 ******************************************************************************/
mro_cell_context_t *
mro_get_cell_context_from_global_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	mro_cell_context_node_t *p_data = SON_PNULL;
	SON_LIST_NODE *p_list_node = SON_PNULL;
	mro_cell_context_t *p_ret_val = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	p_list_node = list_find_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
			p_cell_id, son_key_of_mro, son_compare_cell_id);

	if (SON_PNULL != p_list_node)
	{
		p_data = YMEMBEROF(mro_cell_context_node_t, sNode, p_list_node);
		p_ret_val = &p_data->data;
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"Cell 0x%x not found",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* mro_get_cell_context_from_global_context */

/*****************************************************************************
 * Function Name  : mro_clear_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all nodes from MRO pending response list
 ****************************************************************************/
son_void_t
mro_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);
	}

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_DETAILED,
			"Cleared pending response list");
	mro_print_pending_res("mro_clear_pending_res_list"); 

	SON_UT_TRACE_EXIT();
} /* mro_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : mro_reset_ho_counters
 * Inputs         : p_intra_rat_hash_rec Pointer to the NRT record
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Reset HO counters in NRT record
 ****************************************************************************/
son_void_t
mro_reset_ho_counters
(
 anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec
 )
{
	SON_UT_TRACE_ENTER();

	son_mro_ho_counter_t *p_mro_ho_counter =
		&p_intra_rat_hash_rec->mro_data.mro_ho_counter;

	p_mro_ho_counter->num_too_late_ho = 0;
	p_mro_ho_counter->num_too_early_ho = 0;
	p_mro_ho_counter->num_ho_to_wrong_cell_for_failure_cell = 0;
	p_mro_ho_counter->num_ho_to_wrong_cell_for_reestablishment_cell = 0;
	p_mro_ho_counter->total_ho_attempted = 0;
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"MRO HO counters has been reset in NRT for "
			"neighbor Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_intra_rat_hash_rec->neighbor_info.nbr_cgi.cell_identity));
	SON_UT_TRACE_EXIT();
} /* mro_reset_ho_counters */

/*****************************************************************************
 * Function Name  : mro_reset_ttt_algo_params
 * Inputs         : p_mro_cell_ctxt Pointer to the MRO cell context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Reset parameters used by MRO TTT algorithm 
 ****************************************************************************/
son_void_t
mro_reset_ttt_algo_params
(
 mro_cell_context_t *p_mro_cell_ctxt
 )
{
	SON_UT_TRACE_ENTER();

	p_mro_cell_ctxt->is_ttt_algo_running = SON_FALSE;
	p_mro_cell_ctxt->previous_ttt = SON_TTT_MS_512;
	p_mro_cell_ctxt->retry_count = 0;
	p_mro_cell_ctxt->srvd_cell_algo_direction = SON_NO_OPTIMIZATION_DIR;
	p_mro_cell_ctxt->previous_ho_fail_rate = 0;
	p_mro_cell_ctxt->is_ttt_update_reqd = SON_FALSE;
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"MRO data(TTT) has been reset in cell context for "
			"serving Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_mro_cell_ctxt->cell_id.cell_identity));
	SON_UT_TRACE_EXIT();
} /* mro_reset_ttt_algo_params */

/*****************************************************************************
 * Function Name  : mro_reset_cio_algo_params
 * Inputs         : p_intra_rat_hash_rec Pointer to the NRT record
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Reset parameters used by MRO CIO algorithm 
 ****************************************************************************/
son_void_t
mro_reset_cio_algo_params
(
 anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec
 )
{
	SON_UT_TRACE_ENTER();

	p_intra_rat_hash_rec->mro_data.is_cio_algo_running = SON_FALSE;
	p_intra_rat_hash_rec->mro_data.retry_count = 0;
	p_intra_rat_hash_rec->mro_data.prev_cio = SON_Q_OFFSET_RANGE_DB0;
	p_intra_rat_hash_rec->mro_data.prev_ho_fail_rate = 0;
	p_intra_rat_hash_rec->mro_data.optimizaton_for = SON_NO_OPTIMIZATION_DIR;
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"MRO data(CIO) has been reset in NRT for "
			"neighbor Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_intra_rat_hash_rec->neighbor_info.nbr_cgi.cell_identity));
	SON_UT_TRACE_EXIT();
} /* mro_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : mro_clear_cell_contexts_list
 * Inputs         : transaction_id : Transaction Identifier
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all the cell contexts from MRO cell context list
 ****************************************************************************/
son_void_t
mro_clear_cell_contexts_list
(
 son_u16 transaction_id
 )
{
	mro_cell_context_node_t *p_ctxt_node = SON_PNULL;
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t *p_hash_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_ctxt_node = mro_get_first_cell_ctxt();
	while (SON_PNULL != p_ctxt_node)
	{
		if (MRO_CELL_STATE_ENABLING ==
				p_ctxt_node->data.current_cell_fsm_state ||
				MRO_CELL_STATE_ENABLED ==
				p_ctxt_node->data.current_cell_fsm_state)
		{
			if (SON_FAILURE == mro_construct_send_rrm_deregister_req(
						transaction_id,
						&p_ctxt_node->data.cell_id))
			{
				SON_LOG(g_mro_gb_ctxt.log_mode,
						p_son_mro_facility_name,
						SON_ERROR,
						"Could not send deregister request to RRM "
						"for Cell 0x%x]",
						son_convert_char_cell_id_to_int_cell_id(
							p_ctxt_node->data.cell_id.cell_identity));
			}
		}

		if (SON_TRUE == son_is_timer_running(
					p_ctxt_node->data.
					ho_failure_accumulation_duration_timer_id))
		{
			son_stop_timer(p_ctxt_node->data.
					ho_failure_accumulation_duration_timer_id);
			p_ctxt_node->data.ho_failure_accumulation_duration_timer_id =
				SON_PNULL;
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_DETAILED,
					"Stopped HO counters acculmulation timer for Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(
						p_ctxt_node->data.cell_id.cell_identity));
		}

		/* Find the cell neighbors and reset their HO counters to 0 in NRT */
		p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
				&p_ctxt_node->data.cell_id);
		if (SON_PNULL != p_anr_cell_ctxt)
		{
			p_hash_tpl = anr_get_first_nr_from_intra_rat_hash_table(
					&p_anr_cell_ctxt->intra_rat_neighbor_table);

			while (SON_PNULL != p_hash_tpl)
			{
				mro_reset_cio_algo_params(&p_hash_tpl->data);
				mro_reset_ho_counters(&p_hash_tpl->data);
				anr_print_eutran_nbr_mro_info(&p_hash_tpl->data);
				p_hash_tpl = anr_get_next_nr_from_intra_rat_hash_table(
						&p_anr_cell_ctxt->intra_rat_neighbor_table,
						&p_hash_tpl->anchor);
			}
		}

		mro_reset_ttt_algo_params(&p_ctxt_node->data);
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"Deleted Cell 0x%x from MRO Global Context",
				son_convert_char_cell_id_to_int_cell_id(
					p_ctxt_node->data.cell_id.cell_identity));
		list_delete_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
				&p_ctxt_node->sNode);

		p_ctxt_node = mro_get_next_node(p_ctxt_node);
	}

	mro_print_cell_context("mro_fsm_shutdown_req_handler");
	SON_UT_TRACE_EXIT();
} /* mro_clear_cell_contexts_list */

/*****************************************************************************
 * Function Name  : mro_get_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et
 * Description    : Get API ID present in pending response
 ****************************************************************************/
son_procedure_code_et
mro_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_mro_pending_response.api_id;
}

/*****************************************************************************
 * Function Name  : mro_construct_cell_arr_from_pending_res_list
 * Inputs         : cell_fsm_state      Cell FSM state to be set
 * Outputs        : p_cell_sts_arr      pointer to cell status array
 *                  p_cell_sts_arr_size Pointer to cell status array size
 * Returns        : son_void_t
 * Description    : Construct cell status array from pending response list
 ****************************************************************************/
son_void_t 
mro_construct_cell_arr_from_pending_res_list
(
 son_u16                     *p_cell_sts_arr_size,
 son_cell_status_t           *p_cell_sts_arr,
 mro_cell_fsm_state_et       cell_fsm_state
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;
	mro_cell_context_node_t *p_cell_data = SON_PNULL;
	SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_sts_arr || 
			SON_PNULL == p_cell_sts_arr_size)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Could not construct cell array from pending response list, "
				"p_cell_sts_arr or p_cell_sts_arr_size is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_DETAILED,
			"Constructing Cell Array from pending Response");

	mro_print_pending_res("mro_construct_cell_arr_from_pending_res_list");

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
			if (MRO_CELL_STATE_DISABLED == cell_fsm_state ||
					MRO_CELL_STATE_ENABLED == cell_fsm_state)
			{
				/* Change cell FSM state to cell_fsm_state 
				 * (either MRO_CELL_STATE_DISABLED or MRO_CELL_STATE_ENABLED
				 * based on the registration/deregistration response awaited
				 * respectively) */
				p_cell_list_node = list_find_node(
						&g_mro_gb_ctxt.mro_cell_ctxt_list,
						&p_list_node->cell_status.cgi, son_key_of_mro,
						son_compare_cell_id);

				if (SON_PNULL != p_cell_list_node)
				{
					p_cell_data = YMEMBEROF(mro_cell_context_node_t, sNode, 
							p_cell_list_node);
					if (SON_PNULL != p_cell_data)
					{    
						SONMRO_CELL_FSM_SET_STATE(
								(&p_cell_data->data),
								p_cell_data->data.previous_cell_fsm_state,
								p_cell_data->data.current_cell_fsm_state,
								cell_fsm_state);
					}
				}
			}
		}

		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);
	}

	SON_UT_TRACE_EXIT();
} /* mro_construct_cell_arr_from_pending_res_list */

/*******************************************************************************
 * Function Name  : mro_get_pending_res_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : number of cells available in pending response
 * Description    : This function gets number of cells available in pending response
 ******************************************************************************/
son_u32 
mro_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_mro_pending_response.cell_sts_list);
}

/*******************************************************************************
 * Function Name  : mro_decrease_cm_cell_specific_offset
 * Inputs         : cio Current cell individual offset
 *                  delta_cio Delta value by which CIO is Increased or decreased
 * Outputs        : p_cm_cell_specific_offset Incremented cell individual offset
 * Returns        : SON_SUCCESS - If increment is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function decreases the given cell individual offset
 *                  value by given delta value
 ******************************************************************************/
static son_return_et
mro_decrease_cm_cell_specific_offset
(
 son_q_offset_range_et   cio,    
 son_u8                  delta_cio,
 son_q_offset_range_et   *p_cm_cell_specific_offset
 )
{
	son_u8 temp_cio = cio;

	SON_UT_TRACE_ENTER();
	if (g_mro_gb_ctxt.mro_config_param.min_cio < cio)
	{   
		if ((cio - g_mro_gb_ctxt.mro_config_param.min_cio) >= delta_cio)
		{
			*p_cm_cell_specific_offset = (son_q_offset_range_et)
				(temp_cio - delta_cio);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"CIO value decreased Old CIO %s New CIO %s Delta CIO %u Minimum CIO %s",
					CIO_NAMES[cio],
					CIO_NAMES[*p_cm_cell_specific_offset], delta_cio, 
					CIO_NAMES[g_mro_gb_ctxt.mro_config_param.min_cio]);
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"CIO value not decreased"
			" Current CIO: %s, Delta CIO: %u, Minimum CIO: %s",
			CIO_NAMES[cio], delta_cio, 
			CIO_NAMES[g_mro_gb_ctxt.mro_config_param.min_cio]);  
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* mro_decrease_cm_cell_specific_offset */

/*******************************************************************************
 * Function Name  : mro_increase_cm_cell_specific_offset
 * Inputs         : cio- Current cell individual offset
 *                  delta_cio- Delta value by which CIO is Increased
 * Outputs        : p_cm_cell_specific_offset Decremented cell individual offset
 * Returns        : SON_SUCCESS - If decrement is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function increases the given cell individual offset
 *                  value by given delta value
 ******************************************************************************/
static son_return_et
mro_increase_cm_cell_specific_offset
(
 son_q_offset_range_et   cio,    
 son_u8                  delta_cio,
 son_q_offset_range_et   *p_cm_cell_specific_offset
 )
{
	son_u8 temp_cio = cio;

	SON_UT_TRACE_ENTER();

	if (g_mro_gb_ctxt.mro_config_param.max_cio > cio)
	{   
		if ((g_mro_gb_ctxt.mro_config_param.max_cio - cio) >= delta_cio)
		{
			*p_cm_cell_specific_offset = (son_q_offset_range_et)
				(temp_cio + delta_cio);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"CIO value increased. Old CIO %s, "
					"New CIO %s, "
					"Delta CIO %u, Maximum CIO %s",
					CIO_NAMES[cio],
					CIO_NAMES[*p_cm_cell_specific_offset], delta_cio, 
					CIO_NAMES[g_mro_gb_ctxt.mro_config_param.max_cio]);  
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"CIO value not increased. "
			"Current CIO %s, "
			"Delta CIO %u, Maximum CIO %s",
			CIO_NAMES[cio], delta_cio, 
			CIO_NAMES[g_mro_gb_ctxt.mro_config_param.max_cio]);  
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* mro_increase_cm_cell_specific_offset */

/*******************************************************************************
 * Function Name  : mro_decrease_ttt
 * Inputs         : ttt-       Time to trigger value
 *                  delta_ttt- Delta value by which TTT is decreased
 * Outputs        : p_ttt- Incremented Time to trigger value
 * Returns        : SON_SUCCESS - If increment is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function decreases the given Time to trigger
 *                  value by given delta value
 ******************************************************************************/
static son_return_et
mro_decrease_ttt
(
 son_time_to_trigger_range_et    ttt,    
 son_u8                          delta_ttt,
 son_time_to_trigger_range_et    *p_ttt
 )
{
	son_u8 temp_ttt = ttt;

	SON_UT_TRACE_ENTER();
	if (g_mro_gb_ctxt.mro_config_param.min_ttt < ttt)
	{   
		if ((ttt - g_mro_gb_ctxt.mro_config_param.min_ttt) >= delta_ttt)
		{
			*p_ttt = (son_time_to_trigger_range_et)(temp_ttt - delta_ttt);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"TTT value decreased Old TTT %s New TTT %s"
                    " Delta TTT %u, Minimum TTT %s",
					TTT_NAMES[ttt], TTT_NAMES[*p_ttt], delta_ttt, 
					TTT_NAMES[g_mro_gb_ctxt.mro_config_param.min_ttt]);  
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"TTT value not decreased Current TTT %s "
			"Delta TTT %u Minimum TTT %s",
			TTT_NAMES[ttt], delta_ttt, 
			TTT_NAMES[g_mro_gb_ctxt.mro_config_param.min_ttt]);  
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* mro_decrease_ttt */

/*******************************************************************************
 * Function Name  : mro_increase_ttt
 * Inputs         : ttt- Current Time to trigger value
 *                  delta_ttt- Delta value by which TTT is Increased
 * Outputs        : p_ttt- Incremented Time to trigger value
 * Returns        : SON_SUCCESS - If decrement is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function increases the given Time to trigger
 *                  value by given delta value
 ******************************************************************************/
static son_return_et
mro_increase_ttt
(
 son_time_to_trigger_range_et    ttt,    
 son_u8                          delta_ttt,
 son_time_to_trigger_range_et    *p_ttt
 )
{
	son_u8 temp_ttt = ttt;

	SON_UT_TRACE_ENTER();
	if (g_mro_gb_ctxt.mro_config_param.max_ttt > ttt)
	{   
		if ((g_mro_gb_ctxt.mro_config_param.max_ttt - ttt) >= delta_ttt)
		{
			*p_ttt = (son_time_to_trigger_range_et)(temp_ttt + delta_ttt);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"TTT value increased Old TTT %s, New "
					"TTT %s Delta TTT %u Maximum TTT %s",
					TTT_NAMES[ttt], TTT_NAMES[*p_ttt], delta_ttt,
					TTT_NAMES[g_mro_gb_ctxt.mro_config_param.max_ttt]);
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"TTT value not increased "
			"Current TTT %s, "
			"Delta TTT %u Maximum TTT %s",
			TTT_NAMES[ttt], delta_ttt, 
			TTT_NAMES[g_mro_gb_ctxt.mro_config_param.max_ttt]);  
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* mro_increase_ttt */

/*******************************************************************************
 * Function Name  : mro_stop_algo_notify_oam
 * Inputs         : p_mro_cell_ctxt Pointer to MRO cell context
 *                  p_hash_tpl          Pointer to the intra RAT neighbor tuple
 *                  cause               Cause value to stop the algo
 *                  ho_failure_rate     HO failure rate
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send
 *                  SONMRO_OPTIMIZATION_NOTIFICATION to OAM whenever MRO is not
 *                  able to update CIO/TTT.
 ******************************************************************************/
static son_void_t
mro_stop_algo_notify_oam
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl,
 son_mro_notification_cause_et   cause,
 son_float                       ho_failure_rate
 )
{
	son_mro_optimization_notification_t mro_optimization_notification;
	son_size_t len = sizeof(son_mro_optimization_notification_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&mro_optimization_notification, 0, len);
	/* Send SON_OAM_MRO_OPTIMIZATION_NOTIFICATION to OAM */
	SON_MEMCPY(&mro_optimization_notification.srv_cgi,
			&p_mro_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	if (SON_PNULL != p_intra_tpl)
	{
		if (SON_MRO_CIO == p_mro_cell_ctxt->mro_config_param.mro_param &&
				SON_TOO_EARLY == p_intra_tpl->data.mro_data.optimizaton_for)
		{
			/* To be done in future
			 * PCI confusion possible
			 * Report Neighbor PCI as Suspicious to ANR. */
		}

		mro_optimization_notification.bitmask |=
			SON_NBR_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
		SON_MEMCPY(&mro_optimization_notification.nbr_cgi,
				&p_intra_tpl->data.neighbor_info.nbr_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
	}

	mro_optimization_notification.cause = cause;
	if (p_mro_cell_ctxt->mro_config_param.mro_ho_optimization_start_threshold
			> ho_failure_rate)
	{
		mro_optimization_notification.ho_failure_level =
			SON_MRO_ABOVE_STOP_THRESHOLD;
	}
	else
	{
		mro_optimization_notification.ho_failure_level =
			SON_MRO_ABOVE_START_THRESHOLD;
	}

	mro_optimization_notification.ho_failure_rate = 
		son_nearbyintf(ho_failure_rate * 100);

	son_create_send_buffer((son_u8 *)&mro_optimization_notification,
			SON_MRO_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONMRO_OPTIMIZATION_NOTIFICATION,
			len);

	SON_UT_TRACE_EXIT();
} /* mro_stop_algo_notify_oam */

/*******************************************************************************
 * Function Name  : mro_parameter_tuning
 * Inputs         : p_mro_cell_ctxt     Pointer to MRO cell context
 *                  p_mro_ho_counter    Pointer to HO counters info of the
 *                                      neighbor
 *                  p_hash_tpl          Pointer to the intra RAT neighbor tuple
 *                  calling_for         Parameter to be updated after tuning(CIO/TTT)
 *                  p_anr_cell_ctxt     Pointer to ANR cell context
 *                  p_oam_updated_nrt_info          Pointer to the updated NRT
 *                                                  info structure to be send
 *                                                  to OAM
 *                  p_rrm_updated_nrt_info          Pointer to the updated NRT
 *                                                  info structure to be send
 *                                                  to RRM
 * Outputs        : None
 * Returns        : SON_TRUE    If CIO is updated successfully in case
 *                              mro_param is SON_MRO_CIO
 *                              If TTT update is required in case mro_param
 *                              is SON_MRO_TTT
 *                  SON_FALSE   Otherwise
 * Description    : This function is used to update CIO value of a neighbor if
 *                  mro_param is SON_MRO_CIO or to determine if TTT update is
 *                  required in case mro_param is SON_MRO_TTT.
 ******************************************************************************/
static son_bool_et
mro_parameter_tuning
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 son_mro_ho_counter_t            *p_mro_ho_counter,
 anr_intra_rat_hash_nr_tuple_t   *p_hash_tpl,
 anr_cell_context_t              *p_anr_cell_ctxt,
 son_mro_param_et calling_for,
 son_anr_updated_nrt_info_t      *p_oam_updated_nrt_info,
 son_anr_rrm_updated_nrt_info_t  *p_rrm_updated_nrt_info
 )
{
	son_float too_late_ho_rate = 0.0, too_early_ho_rate = 0.0, 
	ho_to_wrong_fail_cell_rate = 0.0, 
	ho_to_wrong_reest_cell_rate = 0.0,
	total_of_too_late_wrong_reest_cell_ho_rate = 0.0,
	total_of_too_early_wrong_fail_cell_ho_rate = 0.0;
	son_bool_et is_updated = SON_FALSE;
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	son_error_et error_code = SON_NO_ERROR;
	son_intra_rat_neighbor_info_t   *p_nbr_info = SON_PNULL;
	son_float ho_failure_rate = 0;

	SON_UT_TRACE_ENTER();

	p_nbr_info = &(p_hash_tpl->data.neighbor_info);
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_DETAILED,
			"MRO HO optimization function called for "
			"the neighbor Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_nbr_info->nbr_cgi.cell_identity));

	/* find various HO rates */
	too_late_ho_rate = (son_float)(p_mro_ho_counter->num_too_late_ho) /
		p_mro_ho_counter->total_ho_attempted;
	too_early_ho_rate = (son_float)(p_mro_ho_counter->num_too_early_ho) /
		p_mro_ho_counter->total_ho_attempted;
	ho_to_wrong_fail_cell_rate = 
		(son_float)(p_mro_ho_counter->num_ho_to_wrong_cell_for_failure_cell) /
		p_mro_ho_counter->total_ho_attempted;
	ho_to_wrong_reest_cell_rate = 
		(son_float)(p_mro_ho_counter->
				num_ho_to_wrong_cell_for_reestablishment_cell) /
		p_mro_ho_counter->total_ho_attempted;

	total_of_too_late_wrong_reest_cell_ho_rate = 
		too_late_ho_rate + ho_to_wrong_reest_cell_rate;

	total_of_too_early_wrong_fail_cell_ho_rate =
		too_early_ho_rate + ho_to_wrong_fail_cell_rate;

	/* HO optimization algorithm */
	if (total_of_too_late_wrong_reest_cell_ho_rate >=
			p_mro_cell_ctxt->mro_config_param.mro_ho_fail_rate_for_optimization)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"total_of_too_late_wrong_reest_cell_ho_rate %f >= configured "
				"mro_ho_fail_rate_for_optimization %f for neighbor "
				"Cell 0x%x",
				total_of_too_late_wrong_reest_cell_ho_rate,
				p_mro_cell_ctxt->mro_config_param.
				mro_ho_fail_rate_for_optimization,
				son_convert_char_cell_id_to_int_cell_id(
					p_nbr_info->nbr_cgi.cell_identity));

		if (total_of_too_late_wrong_reest_cell_ho_rate >
				total_of_too_early_wrong_fail_cell_ho_rate)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"total_of_too_late_wrong_reest_cell_ho_rate %f >"
                    "total_of_too_early_wrong_fail_cell_ho_rate %f for neighbor Cell 0x%x",
					total_of_too_late_wrong_reest_cell_ho_rate,
					total_of_too_early_wrong_fail_cell_ho_rate,
					son_convert_char_cell_id_to_int_cell_id(
						p_nbr_info->nbr_cgi.cell_identity));
			if (SON_MRO_TTT == calling_for)
			{
				/* Algorithem requires Decrease TTT for MRO */
				is_updated = SON_TRUE;
			}
			else if (SON_MRO_CIO == calling_for)
			{
				/* Algorithem requires Increase CIO for MRO */
				SON_MEMSET(&intra_rat_hash_rec, 0,
						sizeof(anr_intra_rat_hash_rec_t));
				/* Store old CIO value in the NRT */
				p_hash_tpl->data.mro_data.prev_cio =
					p_nbr_info->cm_cell_specific_offset;
				if (SON_SUCCESS == mro_increase_cm_cell_specific_offset(
							p_nbr_info->cm_cell_specific_offset,
							p_mro_cell_ctxt->mro_config_param.delta_cio,
							&(intra_rat_hash_rec.neighbor_info.
								cm_cell_specific_offset)))
				{
					p_hash_tpl->data.mro_data.optimizaton_for = SON_TOO_LATE;
					is_updated = SON_TRUE;
					intra_rat_hash_rec.neighbor_info.bitmask |=
						SON_CM_CELL_OFFSET_PRESENT;
					SON_MEMCPY(&intra_rat_hash_rec.neighbor_info.nbr_cgi,
							&p_nbr_info->nbr_cgi,
							sizeof(son_intra_rat_global_cell_id_t));
					anr_intra_rat_modify_nr(p_anr_cell_ctxt,
							&intra_rat_hash_rec,
							SON_FALSE, &error_code, SON_PNULL,
							p_oam_updated_nrt_info,
							p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);
				}
				else
				{
					ho_failure_rate = (son_float)
						(p_mro_ho_counter->num_too_late_ho +
						 p_mro_ho_counter->num_too_early_ho +
						 p_mro_ho_counter->
						 num_ho_to_wrong_cell_for_failure_cell +
						 p_mro_ho_counter->
						 num_ho_to_wrong_cell_for_reestablishment_cell) /
						p_mro_ho_counter->total_ho_attempted;
					mro_stop_algo_notify_oam(
							p_mro_cell_ctxt,
							p_hash_tpl,
							SON_MRO_CIO_MAX_LIMIT_REACHED,
							ho_failure_rate);
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_INFO,
							"Failed to increase value of "
							"cm_cell_specific_offset. Current "
							"cm_cell_specific_offset = %s",
							CIO_NAMES[intra_rat_hash_rec.neighbor_info.
							cm_cell_specific_offset]);
				}
			}

			SON_UT_TRACE_EXIT();
			return is_updated;
		}
	}

	if (total_of_too_early_wrong_fail_cell_ho_rate >=
			p_mro_cell_ctxt->mro_config_param.
			mro_ho_fail_rate_for_optimization)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"total_of_too_early_wrong_fail_cell_ho_rate %f >= "
				"mro_ho_fail_rate_for_optimization %f for neighbor "
				"Cell 0x%x",
				total_of_too_early_wrong_fail_cell_ho_rate,
				p_mro_cell_ctxt->mro_config_param.
				mro_ho_fail_rate_for_optimization,
				son_convert_char_cell_id_to_int_cell_id(
					p_nbr_info->nbr_cgi.cell_identity));
		if (total_of_too_early_wrong_fail_cell_ho_rate >
				total_of_too_late_wrong_reest_cell_ho_rate)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"total_of_too_early_wrong_fail_cell_ho_rate %f >"
                    " total_of_too_late_wrong_reest_cell_ho_rate %f for "
					"neighbor Cell 0x%x",
					total_of_too_early_wrong_fail_cell_ho_rate,
					total_of_too_late_wrong_reest_cell_ho_rate,
					son_convert_char_cell_id_to_int_cell_id(
						p_nbr_info->nbr_cgi.cell_identity));
			if (SON_MRO_TTT == calling_for)
			{
				/* Algorithem requires Increase TTT for MRO */
				is_updated = SON_TRUE;
			}
			else if (SON_MRO_CIO == calling_for)
			{
				/* Algorithem requires Decrease CIO for MRO */
				SON_MEMSET(&intra_rat_hash_rec, 0,
						sizeof(anr_intra_rat_hash_rec_t));
				/* Store old CIO value in the NRT */
				p_hash_tpl->data.mro_data.prev_cio =
					p_nbr_info->cm_cell_specific_offset;
				if (SON_SUCCESS == mro_decrease_cm_cell_specific_offset(
							p_nbr_info->cm_cell_specific_offset,
							p_mro_cell_ctxt->mro_config_param.delta_cio,
							&(intra_rat_hash_rec.neighbor_info.
								cm_cell_specific_offset)))
				{
					p_hash_tpl->data.mro_data.optimizaton_for =
						SON_TOO_EARLY;
					is_updated = SON_TRUE;
					intra_rat_hash_rec.neighbor_info.bitmask |=
						SON_CM_CELL_OFFSET_PRESENT;
					SON_MEMCPY(&intra_rat_hash_rec.neighbor_info.nbr_cgi,
							&p_nbr_info->nbr_cgi,
							sizeof(son_intra_rat_global_cell_id_t));
					anr_intra_rat_modify_nr(p_anr_cell_ctxt,
							&intra_rat_hash_rec,
							SON_FALSE, &error_code, SON_PNULL,
							p_oam_updated_nrt_info,
							p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);
				}
				else
				{
					ho_failure_rate = (son_float)
						(p_mro_ho_counter->num_too_late_ho +
						 p_mro_ho_counter->num_too_early_ho +
						 p_mro_ho_counter->
						 num_ho_to_wrong_cell_for_failure_cell +
						 p_mro_ho_counter->
						 num_ho_to_wrong_cell_for_reestablishment_cell) /
						p_mro_ho_counter->total_ho_attempted;
					mro_stop_algo_notify_oam(
							p_mro_cell_ctxt,
							p_hash_tpl,
							SON_MRO_CIO_MIN_LIMIT_REACHED,
							ho_failure_rate);
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_INFO,
							"Failed to decrease "
							"cm_cell_specific_offset for neighbor"
							" Cell 0x%x Current cm_cell_specific_offset %u",
							son_convert_char_cell_id_to_int_cell_id(
								p_hash_tpl->data.neighbor_info.
								nbr_cgi.cell_identity),
							intra_rat_hash_rec.neighbor_info.
							cm_cell_specific_offset);
				}
			}
		}
		else
		{
			/* No optimization is required */
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_BRIEF,
					"No optimization is required for "
					"the neighbor cell Cell 0x%x since "
					"total_of_too_early_wrong_fail_cell_ho_rate %f <= "
					"total_of_too_late_wrong_reest_cell_ho_rate %f",
					son_convert_char_cell_id_to_int_cell_id(
						p_nbr_info->nbr_cgi.cell_identity),
					total_of_too_early_wrong_fail_cell_ho_rate,
					total_of_too_late_wrong_reest_cell_ho_rate);
		}
	}
	else
	{
		/* No optimization is required */
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_BRIEF,
				"No optimization is required for "
				"the neighbor Cell Id 0x%x since"
				" total_of_too_late_wrong_reest_cell_ho_rate %f and "
				"total_of_too_early_wrong_fail_cell_ho_rate %f is less"
				"than HO fail rate for optimization %f",
				son_convert_char_cell_id_to_int_cell_id(
					p_nbr_info->nbr_cgi.cell_identity),
				total_of_too_late_wrong_reest_cell_ho_rate,
				total_of_too_early_wrong_fail_cell_ho_rate,
				p_mro_cell_ctxt->mro_config_param.
				mro_ho_fail_rate_for_optimization);
	}

	SON_UT_TRACE_EXIT();
	return is_updated;
} /* mro_parameter_tuning */

/*******************************************************************************
 * Function Name  : mro_find_ho_failure_rate
 * Inputs         : p_intra_tpl     Pointer to the intra RAT neighbor tuple
 * Outputs        : None
 * Returns        : Total HO fail rate
 * Description    : This function is used to find the total HO failure rate
 *                  between the source cell and a given neighbor cell.
 ******************************************************************************/
static son_float
mro_find_ho_failure_rate
(
 anr_intra_rat_hash_nr_tuple_t *p_intra_tpl
 )
{
	son_float ho_failure_rate = 0;
	son_u32 num_failed_ho = 0;

	SON_UT_TRACE_ENTER();

	/* Calculate HO failure rate as per the following calculation.
	 * ho_failure_rate = (accumulated_too_late_ho +
	 * accumulated_too_early_ho +
	 * accumulated_ho_to_wrong_cell_for_failure_cell +
	 * accumulated_ho_to_wrong_cell_for_reestablishment_cell) /
	 * accumulated_ho_attempts. */

	num_failed_ho = p_intra_tpl->data.mro_data.mro_ho_counter.num_too_late_ho +
		p_intra_tpl->data.mro_data.mro_ho_counter.num_too_early_ho +
		p_intra_tpl->data.mro_data.mro_ho_counter.
		num_ho_to_wrong_cell_for_failure_cell +
		p_intra_tpl->data.mro_data.mro_ho_counter.
		num_ho_to_wrong_cell_for_reestablishment_cell;
	if (0 != p_intra_tpl->data.mro_data.mro_ho_counter.total_ho_attempted)
	{
		ho_failure_rate =
			(son_float)(num_failed_ho) / p_intra_tpl->data.mro_data.
			mro_ho_counter.total_ho_attempted;
	}
	else if (0 != num_failed_ho)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_WARNING,
				"Neighbor Cell 0x%x total number of HO attempted is 0 while the "
				"number of failed HO = %u",
				son_convert_char_cell_id_to_int_cell_id(
					p_intra_tpl->data.neighbor_info.nbr_cgi.cell_identity),
				num_failed_ho);
	}

	SON_UT_TRACE_EXIT();
	return ho_failure_rate;
} /* mro_find_ho_failure_rate */

/*******************************************************************************
 * Function Name  : mro_send_ttt_update
 * Inputs         : p_mro_cell_context  Pointer to MRO cell context
 *                  new_ttt             New value of TTT
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send updated value of TTT to both
 *                  OAM and RRM.
 ******************************************************************************/
static son_void_t
mro_send_ttt_update
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 son_time_to_trigger_range_et new_ttt
 )
{
	son_rrm_ttt_update_ind_t rrm_ttt_update_ind;
	son_mro_eutran_ttt_update_ind_t oam_ttt_update_ind;
	son_size_t oam_ttt_update_ind_len =
		sizeof(son_mro_eutran_ttt_update_ind_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&rrm_ttt_update_ind, 0,
			sizeof(son_rrm_ttt_update_ind_t));
	SON_MEMCPY(&rrm_ttt_update_ind.srv_cgi,
			&p_mro_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	rrm_ttt_update_ind.updated_ttt = new_ttt;
	/* Invoke pup tool to send update ind to RRM */
	if (SON_FAILURE == son_rrm_send_son_rrm_ttt_update_ind(
				&rrm_ttt_update_ind,
				SON_MRO_MODULE_ID,
				SON_RRM_MODULE_ID,
				SON_NULL,
				SON_NULL))
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_WARNING,
				"Failed to "
				"send SONMRO_EUTRAN_TTT_UPDATE_IND from MRO to "
				"RRM for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_mro_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_INFO,
				"MRO->RRM:SONMRO_EUTRAN_TTT_UPDATE_IND "
				" for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_mro_cell_ctxt->cell_id.cell_identity));
	}

	/* Send SONMRO_EUTRAN_TTT_UPDATE_IND to SMIF */
	SON_MEMSET(&oam_ttt_update_ind, 0, oam_ttt_update_ind_len);
	SON_MEMCPY(&oam_ttt_update_ind.srv_cgi,
			&p_mro_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	oam_ttt_update_ind.updated_ttt = new_ttt;

	son_create_send_buffer((son_u8 *)&oam_ttt_update_ind,
			SON_MRO_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONMRO_EUTRAN_TTT_UPDATE_IND,
			oam_ttt_update_ind_len);
	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_INFO,
			"MRO->MIF:SONMRO_EUTRAN_TTT_UPDATE_IND "
			"for Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_mro_cell_ctxt->cell_id.cell_identity));
	SON_UT_TRACE_EXIT();
} /* mro_send_ttt_update */

/*******************************************************************************
 * Function Name  : mro_continue_ttt_optimization
 * Inputs         : p_mro_cell_context              Pointer to MRO cell context
 *                  srvd_cell_total_ho_failure_rate Served cell HO failure rate
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to continue TTT optimization in case
 *                  the last update resulted in decreased HO failure rate.
 ******************************************************************************/
son_void_t
mro_continue_ttt_optimization
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 son_float srvd_cell_total_ho_failure_rate
 )
{
	son_return_et error_code = SON_SUCCESS;

	SON_UT_TRACE_ENTER();

	p_mro_cell_ctxt->previous_ttt =
		p_mro_cell_ctxt->mro_config_param.ttt;
	p_mro_cell_ctxt->is_ttt_algo_running = SON_TRUE;
	if (SON_TOO_EARLY == p_mro_cell_ctxt->srvd_cell_algo_direction)
	{
		/* Increase TTT*/
		error_code = mro_increase_ttt(p_mro_cell_ctxt->
				mro_config_param.ttt,
				p_mro_cell_ctxt->mro_config_param.delta_ttt,
				&p_mro_cell_ctxt->mro_config_param.ttt);
		if (SON_SUCCESS == error_code)
		{
			p_mro_cell_ctxt->previous_ho_fail_rate =
				srvd_cell_total_ho_failure_rate;
			p_mro_cell_ctxt->srvd_cell_algo_direction = SON_TOO_EARLY;
			mro_send_ttt_update(p_mro_cell_ctxt,
					p_mro_cell_ctxt->mro_config_param.ttt);
		}
		else
		{
			mro_stop_algo_notify_oam(
					p_mro_cell_ctxt,
					SON_PNULL,
					SON_MRO_TTT_MAX_LIMIT_REACHED,
					srvd_cell_total_ho_failure_rate);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_WARNING,
					"Failed to increase value "
					"of time to trigger. Current "
					"TTT = %s",
					TTT_NAMES[p_mro_cell_ctxt->mro_config_param.ttt]);
			mro_reset_ttt_algo_params(p_mro_cell_ctxt);
		}
	}
	else
	{
		/* Decrease TTT */
		error_code = mro_decrease_ttt(p_mro_cell_ctxt->
				mro_config_param.ttt,
				p_mro_cell_ctxt->mro_config_param.delta_ttt,
				&p_mro_cell_ctxt->mro_config_param.ttt);
		if (SON_SUCCESS == error_code)
		{
			p_mro_cell_ctxt->previous_ho_fail_rate =
				srvd_cell_total_ho_failure_rate;
			p_mro_cell_ctxt->srvd_cell_algo_direction = SON_TOO_LATE;
			mro_send_ttt_update(p_mro_cell_ctxt,
					p_mro_cell_ctxt->mro_config_param.ttt);
		}
		else
		{
			mro_stop_algo_notify_oam(
					p_mro_cell_ctxt,
					SON_PNULL,
					SON_MRO_TTT_MIN_LIMIT_REACHED,
					srvd_cell_total_ho_failure_rate);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_DETAILED,
					"Failed to decrease value "
					"of time to trigger. Current "
					"TTT = %s",
					TTT_NAMES[p_mro_cell_ctxt->mro_config_param.ttt]);
			mro_reset_ttt_algo_params(p_mro_cell_ctxt);
		}
	}

	SON_UT_TRACE_EXIT();
} /* mro_continue_ttt_optimization */

/*******************************************************************************
 * Function Name  : mro_continue_cio_optimization
 * Inputs         : p_mro_cell_context              Pointer to MRO cell context
 *                  p_anr_cell_ctxt                 Pointer to ANR cell context
 *                  ho_failure_rate                 HO failure rate
 *                  p_intra_tpl                     Pointer to the neighbor
 *                                                  tuple in intra RAT NRT
 *                  p_oam_updated_nrt_info          Pointer to the updated NRT
 *                                                  info structure to be send
 *                                                  to OAM
 *                  p_rrm_updated_nrt_info          Pointer to the updated NRT
 *                                                  info structure to be send
 *                                                  to RRM
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to continue CIO optimization in case
 *                  the last update resulted in decreased HO failure rate.
 ******************************************************************************/
static son_void_t
mro_continue_cio_optimization
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 anr_cell_context_t              *p_anr_cell_ctxt,
 son_float ho_failure_rate,
 anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl,
 son_anr_updated_nrt_info_t      *p_oam_updated_nrt_info,
 son_anr_rrm_updated_nrt_info_t  *p_rrm_updated_nrt_info
 )
{
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	son_error_et error_code = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	p_intra_tpl->data.mro_data.prev_cio = p_intra_tpl->data.neighbor_info.
		cm_cell_specific_offset;
	SON_MEMSET(&intra_rat_hash_rec, 0,
			sizeof(anr_intra_rat_hash_rec_t));
	if (SON_TOO_LATE == p_intra_tpl->data.mro_data.optimizaton_for)
	{
		if (SON_SUCCESS == mro_increase_cm_cell_specific_offset(
					p_intra_tpl->data.neighbor_info.cm_cell_specific_offset,
					p_mro_cell_ctxt->mro_config_param.delta_cio,
					&(intra_rat_hash_rec.neighbor_info.
						cm_cell_specific_offset)))
		{
			p_intra_tpl->data.mro_data.prev_ho_fail_rate = ho_failure_rate;
			intra_rat_hash_rec.neighbor_info.bitmask |=
				SON_CM_CELL_OFFSET_PRESENT;
			SON_MEMCPY(&intra_rat_hash_rec.neighbor_info.nbr_cgi,
					&p_intra_tpl->data.neighbor_info.nbr_cgi,
					sizeof(son_intra_rat_global_cell_id_t));
			anr_intra_rat_modify_nr(p_anr_cell_ctxt,
					&intra_rat_hash_rec,
					SON_FALSE, &error_code, SON_PNULL,
					p_oam_updated_nrt_info,
					p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);
		}
		else
		{
			mro_stop_algo_notify_oam(
					p_mro_cell_ctxt,
					p_intra_tpl,
					SON_MRO_CIO_MAX_LIMIT_REACHED,
					ho_failure_rate);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_WARNING,
					"Failed to increase value "
					"of cm_cell_specific_offset. Current "
					"cm_cell_specific_offset = %s",
					CIO_NAMES[intra_rat_hash_rec.neighbor_info.
					cm_cell_specific_offset]);
			mro_reset_cio_algo_params(&p_intra_tpl->data);
		}
	}
	else
	{
		if (SON_SUCCESS == mro_decrease_cm_cell_specific_offset(
					p_intra_tpl->data.neighbor_info.cm_cell_specific_offset,
					p_mro_cell_ctxt->mro_config_param.delta_cio,
					&(intra_rat_hash_rec.neighbor_info.
						cm_cell_specific_offset)))
		{
			p_intra_tpl->data.mro_data.prev_ho_fail_rate = ho_failure_rate;
			intra_rat_hash_rec.neighbor_info.bitmask |=
				SON_CM_CELL_OFFSET_PRESENT;
			SON_MEMCPY(&intra_rat_hash_rec.neighbor_info.nbr_cgi,
					&p_intra_tpl->data.neighbor_info.nbr_cgi,
					sizeof(son_intra_rat_global_cell_id_t));
			anr_intra_rat_modify_nr(p_anr_cell_ctxt,
					&intra_rat_hash_rec,
					SON_FALSE, &error_code, SON_PNULL,
					p_oam_updated_nrt_info,
					p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);
		}
		else
		{
			mro_stop_algo_notify_oam(
					p_mro_cell_ctxt,
					p_intra_tpl,
					SON_MRO_CIO_MIN_LIMIT_REACHED,
					ho_failure_rate);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_WARNING,
					"Failed to decrease value "
					"of cm_cell_specific_offset. Current "
					"cm_cell_specific_offset = %u",
					intra_rat_hash_rec.neighbor_info.
					cm_cell_specific_offset);
			mro_reset_cio_algo_params(&p_intra_tpl->data);
		}
	}

	if (SON_MRO_CIO ==
			p_mro_cell_ctxt->mro_config_param.mro_param)
	{
		mro_reset_ho_counters(&p_intra_tpl->data);
	}

	SON_UT_TRACE_EXIT();
} /* mro_continue_cio_optimization */

/*******************************************************************************
 * Function Name  : mro_start_ttt_optimization
 * Inputs         : p_mro_cell_context              Pointer to MRO cell context
 *                  srvd_cell_total_ho_attempts     Total number of HO attempts
 *                                                  for the served cell
 *                  srvd_cell_too_late_failures     Total number of too late
 *                                                  HO for the served cell
 *                  srvd_cell_too_early_failures    Total number of too early
 *                                                  HO for the served cell
 *                  new_ttt                         Updated value of TTT
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to set the MRO context's TTT algo
 *                  specific variables whenever TTT OPTIMIZATION is started
 *                  for the first time and send the TTT value update to OAM
 *                  and RRM.
 ******************************************************************************/
static son_void_t
mro_start_ttt_optimization
(
 mro_cell_context_t              *p_mro_cell_ctxt,
 son_u32 srvd_cell_total_ho_attempts,
 son_u32 srvd_cell_too_late_failures,
 son_u32 srvd_cell_too_early_failures,
 son_time_to_trigger_range_et new_ttt
 )
{
	SON_UT_TRACE_ENTER();

	p_mro_cell_ctxt->is_ttt_algo_running = SON_TRUE;
	/*CID 30724 FIX START*/
	if (0 != srvd_cell_total_ho_attempts)
	{
		/*CID 30724 FIX END*/
		p_mro_cell_ctxt->previous_ho_fail_rate =
			(son_float)(srvd_cell_too_early_failures +
					srvd_cell_too_late_failures) /
			srvd_cell_total_ho_attempts;
	}
	mro_send_ttt_update(p_mro_cell_ctxt, new_ttt);

	SON_UT_TRACE_EXIT();
}

/*******************************************************************************
 * Function Name  : mro_execute_ttt_optimization_algo
 * Inputs         : p_mro_cell_context Pointer to MRO cell context for which HO
 *                  optimization to be done(i.e the cell for which HO counters
 *                  accumulation duraion expired)
 *                  p_anr_cell_ctxt    Pointer to ANR cell context for which HO
 *                  optimization to be done
 * Outputs        : None
 * Returns        : None
 * Description    : This function runs TTT OPTIMIZATION ALGO on HO counters
 *                  collected during the HO accumulation duration period.
 *                  This algorithm tunes the TTT of the serving cell which
 *                  impacts the Handovers for all the neighbor cells.
 ******************************************************************************/
static son_void_t
mro_execute_ttt_optimization_algo
(
 mro_cell_context_t *p_mro_cell_ctxt,
 anr_cell_context_t *p_anr_cell_ctxt
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;
	son_bool_et ttt_optimization_required = SON_FALSE;
	son_u32 srvd_cell_total_ho_attempts = 0;
	son_u32 srvd_cell_too_early_failures = 0;
	son_float srvd_cell_total_ho_failure_rate = 0;
	son_float srvd_cell_ho_failure_improvement = 0;
	son_u32 srvd_cell_too_late_failures = 0;
	SON_HASH *p_hash = SON_PNULL;
	son_float ho_failure_rate = 0;
	son_return_et error_code = SON_SUCCESS;
	son_rrm_ttt_update_ind_t rrm_ttt_update_ind;
	son_mro_eutran_ttt_update_ind_t oam_ttt_update_ind;
	son_size_t oam_ttt_update_ind_len =
		sizeof(son_mro_eutran_ttt_update_ind_t);

	SON_UT_TRACE_ENTER();

	p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;
	if (SON_FALSE == p_mro_cell_ctxt->is_ttt_algo_running)
	{
		p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

		while (SON_PNULL != p_intra_tpl)
		{
			if ((SON_X2_ALLOWED_CONNECTED ==
						p_intra_tpl->data.neighbor_info.x2_status ||
						SON_X2_PROHIBITED_CONNECTED ==
						p_intra_tpl->data.neighbor_info.x2_status))
			{
				/* 1.  Calculate total_ho_fail_rate (i.e. total_ho_failures/
				 * total_ho_attempts) */
				ho_failure_rate = mro_find_ho_failure_rate(p_intra_tpl);

				/* 2.  If total_ho_attempts >=
				 * mro_ho_attempts_threshold and total_ho_fail_rate
				 * >= mro_ho_optimization_start_threshold then */
				if ((p_intra_tpl->data.mro_data.mro_ho_counter.
							total_ho_attempted >=
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_attempts_threshold) &&
						((SON_TRUE == p_mro_cell_ctxt->is_ttt_update_reqd) ||
						 (ho_failure_rate >=
						  p_mro_cell_ctxt->mro_config_param.
						  mro_ho_optimization_start_threshold)))
				{
					/* Run the algo and see if TTT updation is required */
					if (SON_FALSE == ttt_optimization_required)
					{
						ttt_optimization_required =
							mro_parameter_tuning(p_mro_cell_ctxt,
									&p_intra_tpl->data.mro_data.mro_ho_counter,
									p_intra_tpl,
									p_anr_cell_ctxt,
									SON_MRO_TTT,
									SON_PNULL,
									SON_PNULL);
					}
				}

				srvd_cell_too_early_failures +=
					(p_intra_tpl->data.mro_data.mro_ho_counter.
					 num_too_early_ho +
					 p_intra_tpl->data.mro_data.mro_ho_counter.
					 num_ho_to_wrong_cell_for_failure_cell);
				srvd_cell_too_late_failures +=
					(p_intra_tpl->data.mro_data.mro_ho_counter.
					 num_too_late_ho +
					 p_intra_tpl->data.mro_data.mro_ho_counter.
					 num_ho_to_wrong_cell_for_reestablishment_cell);
				srvd_cell_total_ho_attempts +=
					p_intra_tpl->data.mro_data.mro_ho_counter.
					total_ho_attempted;
			}

			mro_reset_ho_counters(&p_intra_tpl->data);
			p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
					p_hash, &p_intra_tpl->anchor);
		}

		if (SON_TRUE == ttt_optimization_required)
		{
            /*Coverity 30724 Fix Start - If loop extended*/
			if (0 != srvd_cell_total_ho_attempts)
			{
				srvd_cell_total_ho_failure_rate =
					(son_float)(srvd_cell_too_early_failures +
							srvd_cell_too_late_failures) /
					srvd_cell_total_ho_attempts;


			p_mro_cell_ctxt->previous_ttt =
				p_mro_cell_ctxt->mro_config_param.ttt;
			if (srvd_cell_too_late_failures > srvd_cell_too_early_failures)
			{
				/* Decrease TTT*/
				error_code = mro_decrease_ttt(p_mro_cell_ctxt->
						mro_config_param.ttt,
						p_mro_cell_ctxt->mro_config_param.delta_ttt,
						&p_mro_cell_ctxt->mro_config_param.ttt);
				if (SON_SUCCESS == error_code)
				{
					mro_start_ttt_optimization(p_mro_cell_ctxt,
							srvd_cell_total_ho_attempts,
							srvd_cell_too_late_failures,
							srvd_cell_too_early_failures,
							p_mro_cell_ctxt->mro_config_param.ttt);
					p_mro_cell_ctxt->srvd_cell_algo_direction = SON_TOO_LATE;
				}
				else
				{
					mro_stop_algo_notify_oam(
							p_mro_cell_ctxt,
							p_intra_tpl,
							SON_MRO_TTT_MIN_LIMIT_REACHED,
							srvd_cell_total_ho_failure_rate);
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
                                "Failed to decrease value "
							"of time to trigger. Current "
							"TTT = %s",
							TTT_NAMES[p_mro_cell_ctxt->mro_config_param.ttt]);
					mro_reset_ttt_algo_params(p_mro_cell_ctxt);
				}
			}
			else if (srvd_cell_too_late_failures < srvd_cell_too_early_failures)
			{
				/* Increase TTT*/
				error_code = mro_increase_ttt(p_mro_cell_ctxt->
						mro_config_param.ttt,
						p_mro_cell_ctxt->mro_config_param.delta_ttt,
						&p_mro_cell_ctxt->mro_config_param.ttt);
				if (SON_SUCCESS == error_code)
				{
					mro_start_ttt_optimization(p_mro_cell_ctxt,
							srvd_cell_total_ho_attempts,
							srvd_cell_too_late_failures,
							srvd_cell_too_early_failures,
							p_mro_cell_ctxt->mro_config_param.ttt);
					p_mro_cell_ctxt->srvd_cell_algo_direction = SON_TOO_EARLY;
				}
				else
				{
					mro_stop_algo_notify_oam(
							p_mro_cell_ctxt,
							p_intra_tpl,
							SON_MRO_TTT_MAX_LIMIT_REACHED,
							srvd_cell_total_ho_failure_rate);
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
                                "Failed to "
							"increase value of time to trigger. Current "
							"TTT = %s",
							TTT_NAMES[p_mro_cell_ctxt->mro_config_param.ttt]);
					mro_reset_ttt_algo_params(p_mro_cell_ctxt);
				}
			}
			else
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                            SON_INFO,
                            "Optimization "
                            "not performed for serving Cell 0x%x as "
                            "srvd_cell_too_early_failures is %u and  "
                            "srvd_cell_too_late_failures is %u ",
						son_convert_char_cell_id_to_int_cell_id(
							p_mro_cell_ctxt->cell_id.cell_identity),
                            srvd_cell_too_early_failures, srvd_cell_too_late_failures);
			}
            }/*Coverity 30724 Fix End - extended if loop ends here*/
            /*Coverity  30724 Fix Start*/
            else
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_INFO,
                        "Optimization not "
                        "done for serving Cell 0x%x] as "
                        "srvd_cell_total_ho_attempts counter is zero",
                        son_convert_char_cell_id_to_int_cell_id(
                            p_mro_cell_ctxt->cell_id.cell_identity));
            }
            /*Coverity 30724 Fix End*/
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"Optimization not "
					"done for serving Cell 0x%x since the pre-conditions"
					" to trigger TTT optimization are not met even for a single "
					"neighbor cell",
					son_convert_char_cell_id_to_int_cell_id(
						p_mro_cell_ctxt->cell_id.cell_identity));
		}
	}
	else
	{
		son_bool_et algo_to_be_run_next_time = SON_FALSE;
		son_bool_et ttt_all_pairs_reached_stop_threshold = SON_TRUE;

		p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

		while (SON_PNULL != p_intra_tpl)
		{
			if ((SON_X2_ALLOWED_CONNECTED ==
						p_intra_tpl->data.neighbor_info.x2_status ||
						SON_X2_PROHIBITED_CONNECTED ==
						p_intra_tpl->data.neighbor_info.x2_status))
			{
				/* 1.  Calculate total_ho_fail_rate (i.e. total_ho_failures/
				 * total_ho_attempts) */
				ho_failure_rate = mro_find_ho_failure_rate(p_intra_tpl);

				if (p_intra_tpl->data.mro_data.mro_ho_counter.
						total_ho_attempted >=
						p_mro_cell_ctxt->mro_config_param.
						mro_ho_attempts_threshold)
				{
					/* 2.  If total_ho_fail_rate
					 * > mro_ho_optimization_stop_threshold then */
					if (ho_failure_rate >
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_optimization_stop_threshold)
					{
						ttt_all_pairs_reached_stop_threshold = SON_FALSE;
					}

					srvd_cell_too_early_failures +=
						(p_intra_tpl->data.mro_data.mro_ho_counter.
						 num_too_early_ho +
						 p_intra_tpl->data.mro_data.mro_ho_counter.
						 num_ho_to_wrong_cell_for_failure_cell);
					srvd_cell_too_late_failures +=
						(p_intra_tpl->data.mro_data.mro_ho_counter.
						 num_too_late_ho +
						 p_intra_tpl->data.mro_data.mro_ho_counter.
						 num_ho_to_wrong_cell_for_reestablishment_cell);
					srvd_cell_total_ho_attempts +=
						p_intra_tpl->data.mro_data.mro_ho_counter.
						total_ho_attempted;
				}
				else if (p_mro_cell_ctxt->previous_ho_fail_rate >
						p_mro_cell_ctxt->mro_config_param.
						mro_ho_optimization_stop_threshold)
				{
					algo_to_be_run_next_time = SON_TRUE;
				}
			}

			mro_reset_ho_counters(&p_intra_tpl->data);
			p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
					p_hash, &p_intra_tpl->anchor);
		}

		if (0 != srvd_cell_total_ho_attempts)
		{
			srvd_cell_total_ho_failure_rate =
				(son_float)(srvd_cell_too_early_failures +
						srvd_cell_too_late_failures) /
				srvd_cell_total_ho_attempts;
		}

		if (SON_FALSE == ttt_all_pairs_reached_stop_threshold)
		{
			srvd_cell_ho_failure_improvement =
				p_mro_cell_ctxt->previous_ho_fail_rate -
				srvd_cell_total_ho_failure_rate;

			/* If srvd_cell_ho_failure_improvement is Negative */
			if (0 > srvd_cell_ho_failure_improvement)
			{
				SON_LOG(g_mro_gb_ctxt.log_mode,
						p_son_mro_facility_name, SON_INFO,
						"Optimization stopped as the HO "
						"failure rate is increased for serving "
						"Cell 0x%x. Previous HO failure"
						" rate %f, Current HO failure rate %f",
						son_convert_char_cell_id_to_int_cell_id(
							p_mro_cell_ctxt->cell_id.cell_identity),
						p_mro_cell_ctxt->previous_ho_fail_rate,
						srvd_cell_total_ho_failure_rate);
				/* ttt = prev_ttt */
				p_mro_cell_ctxt->mro_config_param.ttt =
					p_mro_cell_ctxt->previous_ttt;
				SON_MEMSET(&rrm_ttt_update_ind, 0,
						sizeof(son_rrm_ttt_update_ind_t));
				SON_MEMCPY(&rrm_ttt_update_ind.srv_cgi,
						&p_mro_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				rrm_ttt_update_ind.updated_ttt =
					p_mro_cell_ctxt->previous_ttt;
				/* Invoke pup tool to send update ind to RRM */
				if (SON_FAILURE == son_rrm_send_son_rrm_ttt_update_ind(
							&rrm_ttt_update_ind,
							SON_MRO_MODULE_ID,
							SON_RRM_MODULE_ID,
							SON_NULL,
							SON_NULL))
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_BRIEF,
							"Failed to "
							"send SONMRO_EUTRAN_TTT_UPDATE_IND from MRO to "
							"RRM for Cell 0x%x",
							son_convert_char_cell_id_to_int_cell_id(
								p_mro_cell_ctxt->cell_id.cell_identity));
				}
				else
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_INFO,
							"MRO->RRM:SONMRO_EUTRAN_TTT_UPDATE_IND for Cell 0x%x",
							son_convert_char_cell_id_to_int_cell_id(
								p_mro_cell_ctxt->cell_id.cell_identity));
				}

				/* Send SONMRO_EUTRAN_TTT_UPDATE_IND to SMIF */
				SON_MEMSET(&oam_ttt_update_ind, 0, oam_ttt_update_ind_len);
				SON_MEMCPY(&oam_ttt_update_ind.srv_cgi,
						&p_mro_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				oam_ttt_update_ind.updated_ttt =
					rrm_ttt_update_ind.updated_ttt;

				son_create_send_buffer((son_u8 *)&oam_ttt_update_ind,
						SON_MRO_MODULE_ID,
						SON_MIF_MODULE_ID,
						SONMRO_EUTRAN_TTT_UPDATE_IND,
						oam_ttt_update_ind_len);
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_INFO,
						"MRO->MIF:SONMRO_EUTRAN_TTT_UPDATE_IND for Cell 0x%x",
						son_convert_char_cell_id_to_int_cell_id(
							p_mro_cell_ctxt->cell_id.cell_identity));
				/* Stop Algo, nofity OAM and reset algo params */
				mro_stop_algo_notify_oam(
						p_mro_cell_ctxt,
						SON_PNULL,
						SON_MRO_OPTIMIZATION_NO_GAIN,
						p_mro_cell_ctxt->previous_ho_fail_rate);
				mro_reset_ttt_algo_params(p_mro_cell_ctxt);
			}
			else if (srvd_cell_ho_failure_improvement <
					p_mro_cell_ctxt->mro_config_param.
					mro_expected_ho_improvement_for_single_step)
			{
				p_mro_cell_ctxt->retry_count++;
				if (MRO_MAX_RETRY_CNT <=
						p_mro_cell_ctxt->retry_count)
				{
					SON_LOG(g_mro_gb_ctxt.log_mode,
							p_son_mro_facility_name, SON_INFO,
							"HO imporvement below expected improvement rate "
							"for serving Cell 0x%x. Maximum retry"
							" limit %u reached. Previous HO failure"
							" rate %f, Current HO failure rate %f",
							son_convert_char_cell_id_to_int_cell_id(
								p_mro_cell_ctxt->cell_id.
								cell_identity),
							p_mro_cell_ctxt->retry_count,
							p_mro_cell_ctxt->previous_ho_fail_rate,
							srvd_cell_total_ho_failure_rate);
					mro_stop_algo_notify_oam(
							p_mro_cell_ctxt,
							SON_PNULL,
							SON_MRO_OPTIMIZATION_LIMITED_GAIN,
							srvd_cell_total_ho_failure_rate);
					mro_reset_ttt_algo_params(p_mro_cell_ctxt);
				}
				else
				{
					SON_LOG(g_mro_gb_ctxt.log_mode,
							p_son_mro_facility_name, SON_INFO,
							"HO imporvement is below "
							"expected improvement rate for serving "
							"Cell 0x%x Retry Count %u, Previous "
							"HO failure rate %f, Current HO failure rate %f",
							son_convert_char_cell_id_to_int_cell_id(
								p_mro_cell_ctxt->cell_id.cell_identity),
							p_mro_cell_ctxt->retry_count,
							p_mro_cell_ctxt->previous_ho_fail_rate,
							srvd_cell_total_ho_failure_rate);
					mro_continue_ttt_optimization(
							p_mro_cell_ctxt, srvd_cell_total_ho_failure_rate);
				}
			}
			else
			{
				p_mro_cell_ctxt->retry_count = 0;
				mro_continue_ttt_optimization(
						p_mro_cell_ctxt, srvd_cell_total_ho_failure_rate);
			}
		}
		else if (SON_FALSE == algo_to_be_run_next_time &&
				SON_TRUE == ttt_all_pairs_reached_stop_threshold)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"ALGO execution stopped for serving cell "
					"Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(p_mro_cell_ctxt->
						cell_id.cell_identity));
			mro_reset_ttt_algo_params(p_mro_cell_ctxt);
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_BRIEF,
					"ALGO not executed for serving Cell 0x%x as "
					"HO attempts are less than mro_ho_attempts_threshold "
					"for all the neighbor cells",
					son_convert_char_cell_id_to_int_cell_id(p_mro_cell_ctxt->
						cell_id.cell_identity));
		}
	}

	SON_UT_TRACE_EXIT();
} /* mro_execute_ttt_optimization_algo */

/*SPR 13795 Changes - Start - Function optimized*/

/*******************************************************************************
 * Function Name  : mro_execute_cio_optimization_algo
 * Inputs         : p_mro_cell_context Pointer to MRO cell context for which HO
 *                  optimization to be done(i.e the cell for which HO counters
 *                  accumulation duraion expired)
 *                  p_anr_cell_ctxt    Pointer to ANR cell context for which HO
 *                  optimization to be done
 * Outputs        : None
 * Returns        : None
 * Description    : This function runs CIO OPTIMIZATION ALGO on HO counters
 *                  collected during the HO accumulation duration period.
 *                  This algorithm tunes the CIO between each Source Cell
 *                  and Neighbor Cell Pair.
 ******************************************************************************/
static son_void_t
mro_execute_cio_optimization_algo
(
 mro_cell_context_t *p_mro_cell_ctxt,
 anr_cell_context_t *p_anr_cell_ctxt
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;
	SON_HASH *p_hash = SON_PNULL;
	son_float ho_failure_rate = 0;
	son_float ho_improvement = 0;
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	son_anr_updated_nrt_info_t *p_oam_updated_nrt_info = SON_PNULL;
	son_size_t oam_updated_nrt_info_len = sizeof(son_anr_updated_nrt_info_t);
	son_anr_rrm_updated_nrt_info_t *p_rrm_updated_nrt_info = SON_PNULL;
	son_size_t rrm_updated_nrt_info_len =
		sizeof(son_anr_rrm_updated_nrt_info_t);
	son_error_et error_code = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	p_oam_updated_nrt_info = (son_anr_updated_nrt_info_t *)son_mem_get(
			oam_updated_nrt_info_len);
	if (SON_PNULL == p_oam_updated_nrt_info)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Could not allocate memory "
				"for SONANR_UPDATED_NRT_INFO message");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_rrm_updated_nrt_info = (son_anr_rrm_updated_nrt_info_t *)son_mem_get(
			rrm_updated_nrt_info_len);
	if (SON_PNULL == p_rrm_updated_nrt_info)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Could not allocate "
				"memory for SON_ANR_RRM_UPDATED_NRT_INFO message");
		son_mem_free(p_oam_updated_nrt_info);
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_oam_updated_nrt_info, 0, oam_updated_nrt_info_len);
	SON_MEMSET(p_rrm_updated_nrt_info, 0, rrm_updated_nrt_info_len);

	p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;
	p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

	while (SON_PNULL != p_intra_tpl)
	{
		if ((SON_X2_ALLOWED_CONNECTED ==
					p_intra_tpl->data.neighbor_info.x2_status ||
					SON_X2_PROHIBITED_CONNECTED ==
					p_intra_tpl->data.neighbor_info.x2_status) &&
				SON_FALSE == p_intra_tpl->data.mro_data.in_use_by_mlb)
		{
			/* Algo is not already running */
			if (SON_FALSE == p_intra_tpl->data.mro_data.is_cio_algo_running)
			{
				/* 1.  Calculate total_ho_fail_rate (i.e. total_ho_failures/
				 * total_ho_attempts) */
				ho_failure_rate = mro_find_ho_failure_rate(p_intra_tpl);

				/* 2.  If total_ho_attempts >=
				 * mro_ho_attempts_threshold and total_ho_fail_rate
				 * >= mro_ho_optimization_start_threshold then */
				if ((ho_failure_rate >=
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_optimization_start_threshold) &&
						(p_intra_tpl->data.mro_data.mro_ho_counter.
						 total_ho_attempted >=
						 p_mro_cell_ctxt->mro_config_param.
						 mro_ho_attempts_threshold))
				{
					/* Run the algo and see if CIO is updated */
					if (SON_TRUE == mro_parameter_tuning(p_mro_cell_ctxt,
								&p_intra_tpl->data.mro_data.mro_ho_counter,
								p_intra_tpl,
								p_anr_cell_ctxt,
								SON_MRO_CIO,
								p_oam_updated_nrt_info,
								p_rrm_updated_nrt_info))
					{
						p_intra_tpl->data.mro_data.prev_ho_fail_rate =
							ho_failure_rate;
						p_intra_tpl->data.mro_data.is_cio_algo_running =
							SON_TRUE;
					}
				}
				else if (p_intra_tpl->data.mro_data.mro_ho_counter.
						total_ho_attempted <
						p_mro_cell_ctxt->mro_config_param.
						mro_ho_attempts_threshold)
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
							"MRO algo is "
							"not executed for neighbor Cell 0x%x "
							"since the total_ho_attempted %u is "
							"less than mro_ho_attempts_threshold %u",
							son_convert_char_cell_id_to_int_cell_id(
								p_intra_tpl->data.neighbor_info.nbr_cgi.
								cell_identity),
							p_intra_tpl->data.mro_data.mro_ho_counter.
							total_ho_attempted,
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_attempts_threshold);
				}
				else if (ho_failure_rate <
						p_mro_cell_ctxt->mro_config_param.
						mro_ho_optimization_start_threshold)
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
							"MRO algo is "
							"not executed for neighbor Cell 0x%x "
							"since the total HO failure rate %f "
							"is less than mro_ho_optimization_start_threshold "
							"%f",
							son_convert_char_cell_id_to_int_cell_id(
								p_intra_tpl->data.neighbor_info.nbr_cgi.
								cell_identity),
							ho_failure_rate,
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_optimization_start_threshold);
				}
				else
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
							"MRO algo is "
							"not executed for neighbor Cell 0x%x "
							"since the total HO failure rate %f "
							"is less than mro_ho_optimization_start_threshold "
							"%f and total_ho_attempted %u is less than "
							"mro_ho_attempts_threshold %u",
							son_convert_char_cell_id_to_int_cell_id(
								p_intra_tpl->data.neighbor_info.nbr_cgi.
								cell_identity),
							ho_failure_rate,
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_optimization_start_threshold,
							p_intra_tpl->data.mro_data.mro_ho_counter.total_ho_attempted,
							p_mro_cell_ctxt->mro_config_param.mro_ho_attempts_threshold);
				}

				if (SON_MRO_CIO ==
						p_mro_cell_ctxt->mro_config_param.mro_param)
				{
					/* 3. Reset HO counters */
					mro_reset_ho_counters(&p_intra_tpl->data);
				}
			}
			/* Algo is already running */
			else
			{
				/* If number of HO attempts exceeds the threshold value */
				if (p_intra_tpl->data.mro_data.mro_ho_counter.
						total_ho_attempted <
						p_mro_cell_ctxt->mro_config_param.
						mro_ho_attempts_threshold)
				{
					SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
							SON_DETAILED,
							"Number of HO attempts %u is less than configured"
							"mro_ho_attempts_threshold %u for"
							"neighbor Cell 0x%x",
							p_intra_tpl->data.mro_data.mro_ho_counter.
							total_ho_attempted,
							p_mro_cell_ctxt->mro_config_param.
							mro_ho_attempts_threshold,
							son_convert_char_cell_id_to_int_cell_id(
								p_intra_tpl->data.neighbor_info.nbr_cgi.
								cell_identity));
					if (SON_MRO_CIO ==
							p_mro_cell_ctxt->mro_config_param.mro_param)
					{
						mro_reset_ho_counters(&p_intra_tpl->data);
					}
				}
				else
				{
					/* 1.  Calculate total_ho_fail_rate (i.e. total_ho_failures/
					 * total_ho_attempts) */
					ho_failure_rate = mro_find_ho_failure_rate(p_intra_tpl);

					/* 2.  If total_ho_fail_rate >
					 * mro_ho_optimization_stop_threshold then */
					if (p_mro_cell_ctxt->mro_config_param.
							mro_ho_optimization_stop_threshold <
							ho_failure_rate)
					{
						/* Calculate HO improvement */
						ho_improvement = p_intra_tpl->data.
							mro_data.prev_ho_fail_rate - 
							ho_failure_rate;
						/* If ho_improvement is Negative */
						if (0 > ho_improvement)
						{
							SON_LOG(g_mro_gb_ctxt.log_mode,
									p_son_mro_facility_name, SON_INFO,
									"Optimization stopped as the HO "
									"failure rate is increased for neighbor "
									"Cell 0x%x Previous HO failure"
									" rate %f Current HO failure rate %f",
									son_convert_char_cell_id_to_int_cell_id(
										p_intra_tpl->data.neighbor_info.
										nbr_cgi.cell_identity),
									p_intra_tpl->data.mro_data.
									prev_ho_fail_rate,
									ho_failure_rate);

							/* Set the value of CIO in NRT to the previous
							 * value */
							SON_MEMSET(&intra_rat_hash_rec, 0,
									sizeof(anr_intra_rat_hash_rec_t));
							intra_rat_hash_rec.neighbor_info.
								cm_cell_specific_offset = p_intra_tpl->data.
								mro_data.prev_cio;
							intra_rat_hash_rec.neighbor_info.bitmask |=
								SON_CM_CELL_OFFSET_PRESENT;
							SON_MEMCPY(&intra_rat_hash_rec.neighbor_info.
									nbr_cgi,
									&p_intra_tpl->data.neighbor_info.nbr_cgi,
									sizeof(son_intra_rat_global_cell_id_t));
							anr_intra_rat_modify_nr(p_anr_cell_ctxt,
									&intra_rat_hash_rec,
									SON_FALSE, &error_code, SON_PNULL,
									p_oam_updated_nrt_info,
									p_rrm_updated_nrt_info, SON_TRUE,
									SON_FALSE);
							if (SON_MRO_CIO ==
									p_mro_cell_ctxt->mro_config_param.
									mro_param)
							{
								mro_stop_algo_notify_oam(
										p_mro_cell_ctxt,
										p_intra_tpl,
										SON_MRO_OPTIMIZATION_NO_GAIN,
										p_intra_tpl->data.mro_data.
										prev_ho_fail_rate);
								mro_reset_ho_counters(&p_intra_tpl->data);
							}
							else if (SON_MRO_CIO_TTT ==
									p_mro_cell_ctxt->mro_config_param.mro_param)
							{
								p_mro_cell_ctxt->is_ttt_update_reqd = SON_TRUE;
							}

							mro_reset_cio_algo_params(&p_intra_tpl->data);
						}

						/* If the ho_imporvement is there but is below expected
						 * improvement */
						else if (ho_improvement < p_mro_cell_ctxt->
								mro_config_param.
								mro_expected_ho_improvement_for_single_step)
						{
							p_intra_tpl->data.mro_data.retry_count++;
							if (MRO_MAX_RETRY_CNT <=
									p_intra_tpl->data.mro_data.retry_count)
							{
								SON_LOG(g_mro_gb_ctxt.log_mode,
										p_son_mro_facility_name, SON_INFO,
										"HO imporvement below expected "
										"improvement for neighbor Cell 0x%x"
                                        " Maximum retry limit %u reached"
                                        " Previous HO failure"
										" rate %f Current HO failure rate %f",
										son_convert_char_cell_id_to_int_cell_id
										(p_intra_tpl->data.neighbor_info.
										 nbr_cgi.cell_identity),
										p_intra_tpl->data.mro_data.retry_count,
										p_intra_tpl->data.mro_data.
										prev_ho_fail_rate,
										ho_failure_rate);
								if (SON_MRO_CIO ==
										p_mro_cell_ctxt->mro_config_param.
										mro_param)
								{
									mro_stop_algo_notify_oam(
											p_mro_cell_ctxt,
											p_intra_tpl,
											SON_MRO_OPTIMIZATION_LIMITED_GAIN,
											ho_failure_rate);
									mro_reset_ho_counters(&p_intra_tpl->data);
								}
								else if (SON_MRO_CIO_TTT ==
										p_mro_cell_ctxt->mro_config_param.
										mro_param)
								{
									p_mro_cell_ctxt->is_ttt_update_reqd =
										SON_TRUE;
								}

								mro_reset_cio_algo_params(&p_intra_tpl->data);
							}
							else
							{
                                SON_LOG(g_mro_gb_ctxt.log_mode,
                                        p_son_mro_facility_name, SON_INFO,
                                        "HO imporvement below expected improvement"
                                        " for neighbor Cell 0x%x "
                                        "Retry Count %u, Previous HO failure"
                                        " rate %f Current HO failure rate: %f",
                                        son_convert_char_cell_id_to_int_cell_id(
                                            p_intra_tpl->data.
                                            neighbor_info.nbr_cgi.cell_identity),
                                        p_intra_tpl->data.mro_data.retry_count,
                                        p_intra_tpl->data.mro_data.
                                        prev_ho_fail_rate,
                                        ho_failure_rate);
								mro_continue_cio_optimization(
										p_mro_cell_ctxt, p_anr_cell_ctxt,
										ho_failure_rate,
										p_intra_tpl,
										p_oam_updated_nrt_info,
										p_rrm_updated_nrt_info);
							}
						}

						/* If the ho_imporvement is equal to or above expected
						 *  improvement */
						else
						{
							SON_LOG(g_mro_gb_ctxt.log_mode,
									p_son_mro_facility_name, SON_DETAILED,
									"HO imporvement is equal to expected "
									"improvement rate for neighbor Cell 0x%x"
                                    " Previous HO failure"
									" rate %f Current HO failure rate %f",
									son_convert_char_cell_id_to_int_cell_id(
										p_intra_tpl->data.
										neighbor_info.nbr_cgi.cell_identity),
									p_intra_tpl->data.mro_data.
									prev_ho_fail_rate,
									ho_failure_rate);
							p_intra_tpl->data.mro_data.retry_count = 0;
							mro_continue_cio_optimization(
									p_mro_cell_ctxt, p_anr_cell_ctxt,
									ho_failure_rate,
									p_intra_tpl,
									p_oam_updated_nrt_info,
									p_rrm_updated_nrt_info);
						}
					}

					/* If total_ho_fail_rate <=
					 * mro_ho_optimization_stop_threshold */
					else
					{
						SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
								SON_INFO,
								"ALGO execution stopped for neighbor Cell 0x%x"
								" as total_ho_fail_rate %f "
								"<= mro_ho_optimization_stop_threshold %f",
								son_convert_char_cell_id_to_int_cell_id(
									p_intra_tpl->data.neighbor_info.
									nbr_cgi.cell_identity),
								ho_failure_rate,
								p_mro_cell_ctxt->mro_config_param.
								mro_ho_optimization_stop_threshold);
						mro_reset_cio_algo_params(&p_intra_tpl->data);
						if (SON_MRO_CIO == p_mro_cell_ctxt->mro_config_param.
								mro_param)
						{
							mro_reset_ho_counters(&p_intra_tpl->data);
						}
					}
				}
			}
		}
		else
		{
			if (SON_TRUE == p_intra_tpl->data.mro_data.in_use_by_mlb)
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_INFO,
						"ALGO not executed for neighbor Cell 0x%x"
						" as the neighbor CIO is being updated"
						" by MLB",
						son_convert_char_cell_id_to_int_cell_id(
							p_intra_tpl->data.neighbor_info.nbr_cgi.
							cell_identity));
			}
			else
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_INFO,
						"ALGO not executed for neighbor Cell 0x%x"
						" as the X2 connection does not exist"
						" with this neighbor cell",
						son_convert_char_cell_id_to_int_cell_id(
							p_intra_tpl->data.neighbor_info.
							nbr_cgi.cell_identity));
			}

			mro_reset_cio_algo_params(&p_intra_tpl->data);
			mro_reset_ho_counters(&p_intra_tpl->data);
		}

		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				p_hash, &p_intra_tpl->anchor);
	}

	if (0 != p_oam_updated_nrt_info->nr_list.
			intra_rat_nr_list_size)
	{
		/* Send information of updated CIO to OAM */
		SON_MEMCPY(&p_oam_updated_nrt_info->src_cgi,
				&p_mro_cell_ctxt->cell_id,
				sizeof(son_intra_rat_global_cell_id_t));
		p_oam_updated_nrt_info->nrt_op = SON_NRT_OP_UPDATE;
		anr_send_updated_ncl_to_oam(p_oam_updated_nrt_info);

		/* Send information of updated CIO to RRM */
		SON_MEMCPY(&p_rrm_updated_nrt_info->src_cgi,
				&p_mro_cell_ctxt->cell_id,
				sizeof(son_rrm_intra_rat_global_cell_id_t));
		anr_send_updated_ncl_to_rrm(p_rrm_updated_nrt_info,
				son_convert_char_cell_id_to_int_cell_id(
					(p_rrm_updated_nrt_info->
					 src_cgi.cell_identity)));
	}

	son_mem_free(p_oam_updated_nrt_info);
	son_mem_free(p_rrm_updated_nrt_info);

	SON_UT_TRACE_EXIT();
} /* mro_execute_cio_optimization_algo */

/*SPR 13795 Changes - End*/

/*******************************************************************************
 * Function Name  : mro_execute_cio_ttt_optimization_algo
 * Inputs         : p_mro_cell_context Pointer to MRO cell context for which HO
 *                  optimization to be done(i.e the cell for which HO counters
 *                  accumulation duraion expired)
 *                  p_anr_cell_ctxt    Pointer to ANR cell context for which HO
 *                  optimization to be done
 * Outputs        : None
 * Returns        : None
 * Description    : This function runs CIO AND TTT OPTIMIZATION ALGO on
 *                  HO counters collected during the HO accumulation duration
 *                  period. This algorithm takes into account the tuning of
 *                  CIO (for each neighbor cell) as well as TTT (for serving
 *                  cell) parameter. This algo uses the CIO OPTIMIZATION
 *                  ALGO and TTT OPTIMIZATION ALGO.
 ******************************************************************************/
static son_void_t
mro_execute_cio_ttt_optimization_algo
(
 mro_cell_context_t *p_mro_cell_ctxt,
 anr_cell_context_t *p_anr_cell_ctxt
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;
	SON_HASH *p_hash = SON_PNULL;
	son_bool_et is_cio_update_reqd_for_any_nbr = SON_FALSE;

	SON_UT_TRACE_ENTER();

	p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;

	/* Run CIO optimization algo first */
	if (SON_FALSE == p_mro_cell_ctxt->is_ttt_update_reqd)
	{
		mro_execute_cio_optimization_algo(p_mro_cell_ctxt, p_anr_cell_ctxt);
		p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);
		while (SON_PNULL != p_intra_tpl)
		{
			if (SON_TRUE == p_intra_tpl->data.mro_data.is_cio_algo_running)
			{
				/* If CIO is to be run then no need to do TTT optimization */
				is_cio_update_reqd_for_any_nbr = SON_TRUE;
				break;
			}

			p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
					p_hash, &p_intra_tpl->anchor);
		}
	}

	/* Check if TTT updation is required */
	if (SON_FALSE == is_cio_update_reqd_for_any_nbr &&
			SON_TRUE == p_mro_cell_ctxt->is_ttt_update_reqd)
	{
		mro_execute_ttt_optimization_algo(p_mro_cell_ctxt, p_anr_cell_ctxt);
	}

	p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);
	while (SON_PNULL != p_intra_tpl)
	{
		mro_reset_ho_counters(&p_intra_tpl->data);
		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				p_hash, &p_intra_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
} /* mro_execute_cio_ttt_optimization_algo */

/*******************************************************************************
 * Function Name  : mro_execute_optimization_algo
 * Inputs         : p_mro_cell_context Pointer to MRO cell context for which HO
 *                  optimization to be done(i.e the cell for which HO counters
 *                  accumulation duraion expired)
 * Outputs        : None
 * Returns        : None
 * Description    : This function runs algorithm on HO counters collected
 *                  during the HO accumulation duration period and optimizes
 *                  CIO or TTT or both based on the MRO configuration
 *                  paramameter setting.
 ******************************************************************************/
son_void_t
mro_execute_optimization_algo
(
 mro_cell_context_t *p_mro_cell_ctxt
 )
{
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_mro_cell_ctxt)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Could not execute "
				"optimization algorithm, p_mro_cell_ctxt is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
			&p_mro_cell_ctxt->cell_id);
	if (SON_PNULL != p_anr_cell_ctxt)
	{
		if (SON_MRO_CIO == p_mro_cell_ctxt->mro_config_param.mro_param)
		{
			mro_execute_cio_optimization_algo(p_mro_cell_ctxt, p_anr_cell_ctxt);
		}
		else if (SON_MRO_TTT == p_mro_cell_ctxt->mro_config_param.mro_param)
		{
			mro_execute_ttt_optimization_algo(p_mro_cell_ctxt, p_anr_cell_ctxt);
		}
		else
		{
			mro_execute_cio_ttt_optimization_algo(p_mro_cell_ctxt,
					p_anr_cell_ctxt);
		}
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_WARNING,
				"Cell 0x%x not "
				"found in anr cell context list, so can't execute"
				"optimization algorithm",
				son_convert_char_cell_id_to_int_cell_id(
					p_mro_cell_ctxt->cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();
} /* mro_execute_optimization_algo */

/*******************************************************************************
 * Function Name  : mro_build_and_send_mro_ho_stats
 * Inputs         : p_mro_cell_context- Pointer to MRO cell context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function builds the HO counters report and send it 
 *                  to OAM.
 ******************************************************************************/
son_void_t
mro_build_and_send_mro_ho_stats
(
 mro_cell_context_t *p_mro_cell_ctxt
 )
{
	son_mro_ho_counter_report_t *p_ho_counter_report;
	son_size_t report_len = sizeof(son_mro_ho_counter_report_t);
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
	son_u16 index = SON_NULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_mro_cell_ctxt)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"p_mro_cell_ctxt is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}
        /* Coverity ID 72719 Starts */
        p_ho_counter_report = (son_mro_ho_counter_report_t *)son_mem_get(
                report_len);
        /* Coverity ID 72719 Ends */

	if (SON_PNULL == p_ho_counter_report)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_ho_counter_report, 0, report_len);
	p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
			&p_mro_cell_ctxt->cell_id);
	SON_MEMCPY(&p_ho_counter_report->srv_cgi,
			&p_mro_cell_ctxt->cell_id,
			sizeof (son_intra_rat_global_cell_id_t));
	if (SON_PNULL != p_anr_cell_ctxt)
	{
		anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;
		SON_HASH *p_hash = SON_PNULL; 

		p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table; 

		p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

		while (SON_PNULL != p_intra_tpl)
		{
			/* Populating HO counters */
			SON_MEMCPY(&p_ho_counter_report->ho_counter_list[index].nbr_cgi,
					&p_intra_tpl->data.neighbor_info.nbr_cgi,
					sizeof (son_intra_rat_global_cell_id_t));
			p_ho_counter_report->ho_counter_list[index].num_too_early_ho =
				p_intra_tpl->data.mro_data.mro_ho_counter.num_too_early_ho;
			p_ho_counter_report->ho_counter_list[index].num_too_late_ho =
				p_intra_tpl->data.mro_data.mro_ho_counter.num_too_late_ho;
			p_ho_counter_report->ho_counter_list[index].num_ho_to_wrong_cell_f =
				p_intra_tpl->data.mro_data.mro_ho_counter.
				num_ho_to_wrong_cell_for_failure_cell;
			p_ho_counter_report->ho_counter_list[index].num_ho_to_wrong_cell_r =
				p_intra_tpl->data.mro_data.mro_ho_counter.
				num_ho_to_wrong_cell_for_reestablishment_cell;
			p_ho_counter_report->ho_counter_list[index].total_ho_attempted =
				p_intra_tpl->data.mro_data.mro_ho_counter.total_ho_attempted;
			mro_reset_ho_counters(&p_intra_tpl->data);
			p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
					p_hash, &p_intra_tpl->anchor);
			index++;
		}

		p_ho_counter_report->neighbour_list_size = index;
		if (SON_NULL == index)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_DETAILED,
					"NO neighbour present"
					"for serving Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(
						p_mro_cell_ctxt->cell_id.cell_identity));
		}    
	}    
	else
	{    
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_WARNING,
				"Cell 0x%x not"
				"found in anr cell context list",
				son_convert_char_cell_id_to_int_cell_id(
					p_mro_cell_ctxt->cell_id.cell_identity));
	}

	son_create_send_buffer((son_u8 *)p_ho_counter_report,
			SON_MRO_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONMRO_HO_COUNTER_REPORT,
			report_len);

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_BRIEF,
			"MRO->MIF:SONMRO_HO_COUNTER_REPORT for Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_ho_counter_report->srv_cgi.cell_identity));

	son_mem_free(p_ho_counter_report);

	SON_UT_TRACE_EXIT();
} /* mro_build_and_send_mro_ho_stats */

/*****************************************************************************
 * Function Name  : mro_find_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id : Pointer to the cell Id
 * Outputs        : None
 * Returns        : Pointer to the node of the pending response list if cell Id 
 *                  is found, NULL otherwise
 * Description    : Find node of the pendign response list with given cell Id
 ****************************************************************************/
cell_status_list_data_t *
mro_find_cell_in_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_node = list_find_node(&g_mro_pending_response.cell_sts_list,
			(son_void_t *)p_rcvd_cell_id, son_key_of_mro,
			son_compare_cell_id);
	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
				p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* mro_find_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : mro_delete_cell_from_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : SON_TRUE  :  if node deleted 
 *                : SON_FALSE :  if node not deleted
 * Description    : This function deletes the specified cell context from the 
 *                  cell context list
 ******************************************************************************/
son_bool_et
mro_delete_cell_from_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	son_bool_et is_node_deleted = SON_FALSE;
	SON_LIST_NODE *p_list_node = SON_PNULL;
	mro_cell_context_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell_id)
	{
		p_list_node = list_find_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
				p_cell_id,
				son_key_of_mro, son_compare_cell_id);

		if (SON_PNULL != p_list_node)
		{
			p_data = (mro_cell_context_node_t *)
				YMEMBEROF(mro_cell_context_node_t, sNode, p_list_node);

			if (SON_TRUE == son_is_timer_running(p_data->data.
						ho_failure_accumulation_duration_timer_id))
			{
				son_stop_timer(p_data->data.
						ho_failure_accumulation_duration_timer_id);
				p_data->data.ho_failure_accumulation_duration_timer_id =
					SON_PNULL;
			}

			list_delete_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
					p_list_node);

			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"Deleted Cell 0x%x from MRO Global Context",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
			mro_print_global_context("mro_delete_cell_from_context");

			/* Free the memory occupied by the node */
			son_mem_free(p_data);
			is_node_deleted = SON_TRUE;
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_INFO,
					"Cell 0x%x could "
					"not be deleted as cell not found in MRO Cell Context "
					"List",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
	return is_node_deleted;
} /* mro_delete_cell_from_context */

/*****************************************************************************
 * Function Name  : mro_send_feature_state_change_ind
 * Inputs         : p_cell_id - Pointer to the cell Id
 *                  state - MRO state for the cell
 *                  cause - Cause for the present state of MRO for the cell
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates and sends the feature state 
 *                  change indication message to SONMgmtIfH.
 ****************************************************************************/
son_void_t
mro_send_feature_state_change_ind
(
 son_intra_rat_global_cell_id_t      *p_cell_id,
 son_feature_state_et                state,
 son_feature_state_change_cause_et   cause
 )
{
	son_mro_feature_state_change_ind_t state_change_ind;
	son_size_t state_change_ind_len = 
		sizeof(son_mro_feature_state_change_ind_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&state_change_ind, 0, 
			state_change_ind_len);
	/* Copy the CGI */
	SON_MEMCPY(&state_change_ind.srv_cgi, p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Set the Module ID */
	state_change_ind.module_id = SON_MRO_MODULE_ID;
	state_change_ind.state = state;
	state_change_ind.cause = cause;

	son_create_send_buffer((son_u8 *)&state_change_ind,
			SON_MRO_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONMRO_FEATURE_STATE_CHANGE_IND,
			state_change_ind_len);

	SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
			SON_BRIEF,
			"Feature state change "
			"indication sent for Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				state_change_ind.srv_cgi.cell_identity));

	SON_UT_TRACE_EXIT();
} /* mro_send_feature_state_change_ind */

/*****************************************************************************
 * Function Name  : mro_get_cell_mode
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : MRO cell FSM state for the cell
 * Description    : This function find the cell FSM state for the given cell Id
 ****************************************************************************/
mro_cell_fsm_state_et
mro_get_cell_mode
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	mro_cell_fsm_state_et fsm_state = MRO_CELL_NUM_OF_STATES;
	mro_cell_context_t *p_mro_cell_ctxt = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_mro_cell_ctxt = mro_get_cell_context_from_global_context(p_cell_id);
	if (SON_PNULL != p_mro_cell_ctxt)
	{
		fsm_state = p_mro_cell_ctxt->current_cell_fsm_state;
	}

	SON_UT_TRACE_EXIT();
	return fsm_state;
}

/*****************************************************************************
 * Function Name  : mro_delete_cell_from_pending_res
 * Inputs         : p_cell_id : Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete node from the pendig response list
 ****************************************************************************/
son_void_t
mro_delete_cell_from_pending_res
(
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	cell_status_list_data_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_id)
	{
		p_node = mro_find_cell_in_pending_res(p_cell_id);

		if (SON_PNULL != p_node)
		{
			list_delete_node(&g_mro_pending_response.cell_sts_list,
					&p_node->cell_status_node);

			son_mem_free(p_node);
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_DETAILED,
					"Deleted cell 0x%x from Pending Response list",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
			mro_print_pending_res("mro_delete_cell_from_pending_res");
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"Node for Cell 0x%x not found in pending response list",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* mro_delete_cell_from_pending_res */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : mro_get_log_level
 * Inputs         : None
 * Outputs        : None
 * Returns        : Log Level of the MRO 
 * Description    : This function gets the log level
 ******************************************************************************/
son_u8
mro_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_mro_gb_ctxt.log_level;
}

/*****************************************************************************
 * Function Name  : mro_set_log_level
 * Inputs         : log_level - Log Level to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log level
 ******************************************************************************/
son_void_t
mro_set_log_level
(
 son_u8 log_level
 )
{
	SON_UT_TRACE_ENTER();
	g_mro_gb_ctxt.log_level = log_level;
	SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */
