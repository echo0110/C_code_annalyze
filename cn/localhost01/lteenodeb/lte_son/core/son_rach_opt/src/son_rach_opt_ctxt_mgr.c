/******************************************************************************G
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rach_opt_ctxt_mgr.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for managing SON RACH OPT
 *                   Module's  Global context and cell specific context.
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aug, 2013       SON Team    Initial         Initial
 *  Aug, 2014        Shilpi                      SPR 13251 Fix
 ******************************************************************************/
/* Includes */
#include <son_utils.h>
#include <son_rach_opt_ctxt_mgr.h>
#include <son_rach_opt_cell_fsm.h>
#include <rrm_son_composer.h>
#include <son_anr_table.h>

extern const son_8 *p_son_rach_opt_facility_name;
extern son_global_config_params_t g_config_params;
extern anr_global_t  g_anr_context;

/*****************************************************************************
 *  Name  : RACH OPT Global Context.
 ****************************************************************************/
rach_opt_global_context_t g_rach_opt_gb_ctxt;

/*****************************************************************************
 *  Name  : Global Pending Response.
 ****************************************************************************/
pending_res_t g_rach_opt_pending_response;

/* SON RACH OPT FSM state names */
const son_s8 *SON_RACH_OPT_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONRACH_OPT_STATE_INIT",
	(const son_s8 *)"SONRACH_OPT_STATE_ACTIVE",
	(const son_s8 *)"SONRACH_OPT_STATE_ENABLING",
	(const son_s8 *)"SONRACH_OPT_STATE_DISABLING",
	(const son_s8 *)"SONRACH_OPT_STATE_SHUTDOWN",
	(const son_s8 *)"SONRACH_OPT_STATE_UNDEFINED"
};

/* SON RACH OPT Global FSM Event names */
const son_s8 *SON_RACH_OPT_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"SMIF_INIT_CONFIG_REQ",
	(const son_s8 *)"SMIF_SET_LOG_LEVEL_CMD",
	(const son_s8 *)"SMIF_LOG_ENABLE_DISABLE_CMD",
	(const son_s8 *)"SMIF_CELL_INFO_IND",
	(const son_s8 *)"SMIF_SHUTDOWN_REQ",
	(const son_s8 *)"SONRACH_OPT_EVENT_ENABLE_REQ", 
	(const son_s8 *)"SONRACH_OPT_EVENT_DISABLE_REQ",
	(const son_s8 *)"SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ",
	(const son_s8 *)"SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ",
	(const son_s8 *)"RRM_EVENT_RACH_CONFIG_RES",
	(const son_s8 *)"RRM_EVENT_RACH_UE_INFO_IND",
	(const son_s8 *)"RRM_EVENT_RACH_L2_IND",
	(const son_s8 *)"RRM_EVENT_CELL_STATE_CHANGE_IND",
	(const son_s8 *)"SONANR_DISABLE_IND",
	/* Rel 3.0 Cell delete support changes Start */
	(const son_s8 *)"SMIF_CELL_DELETE_REQ",
	/* Rel 3.0 Cell delete support changes Stop */
	/* SPR-13251 Fix Starts */
	(const son_s8 *)"SMIF_GET_LOG_LEVEL_REQ",
	/* SPR-13251 Fix Ends */
	(const son_s8 *)"NOT_AVAILABLE"
};

/* SON RACH OPT FSM state names - for Cell level FSM */
const son_s8 *SON_RACH_OPT_CELL_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"RACH_OPT_CELL_STATE_DISABLED",
	(const son_s8 *)"RACH_OPT_CELL_STATE_ENABLING",
	(const son_s8 *)"SONRACH_OPT_CELL_STATE_ENABLED",
	(const son_s8 *)"SONRACH_OPT_CELL_STATE_DISABLING",
	(const son_s8 *)"SONRACH_OPT_CELL_STATE_UNDEFINED"
};

/* SON RACH OPT Cell FSM Event names */
const son_s8 *SON_RACH_OPT_CELL_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"SONRACH_OPT_EVENT_ENABLE_REQ",
	(const son_s8 *)"SONRACH_OPT_EVENT_DISABLE_REQ",
	(const son_s8 *)"SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ",
	(const son_s8 *)"RRM_EVENT_RACH_CONFIG_RES",
	(const son_s8 *)"RRM_EVENT_RACH_UE_INFO_IND",
	(const son_s8 *)"RRM_EVENT_RACH_L2_IND",
	(const son_s8 *)"RRM_EVENT_CELL_STATE_CHANGE_IND",
	(const son_s8 *)"SONANR_EVENT_DISABLE_IND",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* SON RACH OPT Initial preamble target power possible values as strings */
const son_s8 *SON_RACH_OPT_PREAMBLE_INIT_RCVD_TARGET_POWER_NAMES[] =
{
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_120",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_118",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_116",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_114",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_112",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_110",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_108",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_106",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_104",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_102",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_100",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_98",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_96",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_94",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_92",
	(const son_s8 *)"SON_OAM_PREAMBLE_POWER_DBM_90",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* SON RACH OPT Number of RA perambles possible values as strings */
const son_s8 *SON_RACH_OPT_NO_OF_RA_PREAMBLES_NAMES[] =
{
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N4",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N8",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N12",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N16",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N20",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N24",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N28",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N32",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N36",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N40",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N44",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N48",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N52",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N56",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N60",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N64",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* SON RACH OPT Size of RA group A preambles possible values as strings */
const son_s8 *SON_RACH_OPT_SIZE_OF_RA_GRP_A_NAMES[] =
{
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N8",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N12",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N16",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N20",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N24",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N28",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N32",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N36",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N40",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N44",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N48",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N52",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N56",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* Possible values of number of Group A RA preambles stored as strings */
const son_s8 *RA_GRP_A_PREAMBLE_NAMES[] = 
{
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N8",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N12",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N16",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N20",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N24",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N28",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N32",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N36",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N40",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N44",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N48",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N52",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N56",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60",
	(const son_s8 *)"NOT_AVAILABLE"
};

/* Possible values of number of contention based RA preambles stored as 
   strings */
const son_s8 *RA_PREAMBLE_NAMES[] = 
{
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N4",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N8",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N12",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N16",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N20",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N24",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N28",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N32",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N36",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N40",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N44",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N48",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N52",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N56",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N60",
	(const son_s8 *)"SON_OAM_RA_PREAMBLE_COUNT_N64",
	(const son_s8 *)"NOT_AVAILABLE"
};

/*****************************************************************************
 * Function Name  : rach_opt_set_log_mode
 * Inputs         : log_mode
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the log mode for RACH OPT module
 ******************************************************************************/
son_void_t
rach_opt_set_log_mode
(
 son_oam_log_on_off_et log_mode
 )
{
	SON_UT_TRACE_ENTER();
	g_rach_opt_gb_ctxt.log_mode = log_mode;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : rach_opt_print_parameter_update_request_data
 * Inputs         : p_update_req_param -    Pointer to the data sent in update 
 *                                          request if any
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the data sent in update request if any
 ******************************************************************************/
static son_void_t
rach_opt_print_parameter_update_request_data
(
 son_oam_rach_opt_update_req_param_t *p_update_req_param
 )
{
	SON_UT_TRACE_ENTER();

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_INFO,
			"Bitmask: %u "
			"Preamble initial received target power: %s\n"
			"Number of RA preambles: %s"
			"Size of RA group A: %s"
			"Root sequence index: %u"
			"PRACH frequency offset: %u"
			"PRACH configuration index: %u",
			p_update_req_param->bitmask,
			SON_RACH_OPT_PREAMBLE_INIT_RCVD_TARGET_POWER_NAMES[
			p_update_req_param->preamble_initial_rcvd_target_power],
			RA_PREAMBLE_NAMES[p_update_req_param->number_of_ra_preambles],
			RA_GRP_A_PREAMBLE_NAMES[p_update_req_param->size_of_ra_group_a],
			p_update_req_param->root_sequence_index,
			p_update_req_param->prach_frequency_offset,
			p_update_req_param->prach_configuration_index);

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : rach_opt_print_cell_context
 * Inputs         : son_method_name - Invoking Method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the RACH OPT Cell contexts list
 ******************************************************************************/
son_void_t
rach_opt_print_cell_context
(
 const son_8 *son_method_name
 )
{
	rach_opt_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = rach_opt_get_first_cell_ctxt();
	if (SON_PNULL == p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_DETAILED,
				"%s: Number of cells configured at RACH OPT = 0.",
				son_method_name);
		SON_UT_TRACE_EXIT();
		return;
	}

	while (SON_PNULL != p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_BRIEF,
				"Cell id: 0x%x\n"
				"previous cell fsm state %s\n"
				"current cell fsm state %s\n"
				"De-registration invoking API: %u\n"
				"RACH Reports periodicity: %u\n"
				"Min UE RACH report count: %u\n"
				"UE RACH attempts threshold: %u\n"
				"RACH start threshold: %f\n"
				"RACH stop threshold: %f\n"
				"Degrade stop offset: %f\n"
				"Max iterations to reach stop threshold: %u\n"
				"Contention threshold: %u\n"
				"Power ramp up threshold: %u\n"
				"Minimum CFRA preambles: %u\n"
				"Strong cell count: %u\n"
				"PUSCH hopping offset: %u\n"
				"Transaction Id of the RACH config request triggered "
				"towards RRM: %u\n"
				"New RACH Reports periodicity(Used only during parameters update): %u\n"
				"New Min UE RACH report count(Used only during parameters update): %u\n",
				son_convert_char_cell_id_to_int_cell_id(
					p_node->data.cell_id.cell_identity),
				SON_RACH_OPT_CELL_FSM_STATES_NAMES[p_node->data.
				previous_cell_fsm_state],
				SON_RACH_OPT_CELL_FSM_STATES_NAMES[p_node->data.
				current_cell_fsm_state],
				p_node->data.dereg_invoking_api,
				p_node->data.oam_config_params.rachopt_report_periodicity,
				p_node->data.oam_config_params.
				rachopt_min_ue_rach_report_count,
				p_node->data.oam_config_params.rachopt_ue_rach_attempts_threshold,
				p_node->data.oam_config_params.rachopt_start_threshold,
				p_node->data.oam_config_params.rachopt_stop_threshold,
				p_node->data.oam_config_params.rachopt_degrade_stop_offset,
				p_node->data.oam_config_params.
				rachopt_max_iterations_to_reach_stop_threshold,
				p_node->data.oam_config_params.rachopt_contention_threshold,
				p_node->data.oam_config_params.rachopt_power_rampup_threshold,
				p_node->data.oam_config_params.rachopt_min_cfra_preambles,
				p_node->data.read_only_config_params.rachopt_strong_cell_count,
				p_node->data.pusch_hopping_offset,
				p_node->data.rrm_request_txid,
				p_node->data.new_rachopt_report_periodicity,
				p_node->data.new_rachopt_min_ue_rach_report_count);

		/* SPR 15748 Fixed Start */
		/*Coverity 86729 Fix Start*/
		if((SON_OAM_PREAMBLE_POWER_UNDEFINED > p_node->data.preamble_initial_rcvd_target_power)&&
				(SON_OAM_RA_PREAMBLE_COUNT_UNDEFINED > p_node->data.number_of_ra_preambles) &&
				(SON_OAM_RA_PREAMBLE_GROUP_A_UNDEFINED > p_node->data.size_of_ra_group_a)) 
		{
			/*Coverity 86729 Fix End*/
			/* SPR 15748 Fixed End */
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
					SON_INFO,
					"Preamble initial received target power: %s"
					" Number of RA preambles: %s"
					" Size of RA group A: %s",
					SON_RACH_OPT_PREAMBLE_INIT_RCVD_TARGET_POWER_NAMES[
					p_node->data.preamble_initial_rcvd_target_power],
					RA_PREAMBLE_NAMES[p_node->data.number_of_ra_preambles],
					RA_GRP_A_PREAMBLE_NAMES[p_node->data.size_of_ra_group_a]);
			/* SPR 15748 Fixed Start */
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Preamble initial received target power: %d"
					" Number of RA preambles: %d"
					" Size of RA group A: %d",
					p_node->data.preamble_initial_rcvd_target_power,
					p_node->data.number_of_ra_preambles,
					p_node->data.size_of_ra_group_a);
		}
		/* SPR 15748 Fixed End */
/* coverity_94823 start */
		rach_opt_print_parameter_update_request_data(&p_node->data.last_update_req_data);
		p_node = rach_opt_get_next_node(p_node);
/* coverity_94823 stop */
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_print_cell_context */

/*****************************************************************************
 * Function Name  : rach_opt_print_global_context
 * Inputs         : p_son_method_name
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the RACH OPT global context
 ******************************************************************************/
son_void_t
rach_opt_print_global_context
(
 const son_8* p_son_method_name
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
			SON_BRIEF,
			"%s: RACH OPT Global Context Structure:\n"
			"Current FSM State: %s "
			"RACH OPT Global Mode: %u "
			"RACH OPT Log Mode: %u "
			"Mode of SON RACH OPT feature: %s "
			"RACH OPT pending response timer Id: 0x%x",
			p_son_method_name,
			SON_RACH_OPT_FSM_STATES_NAMES[g_rach_opt_gb_ctxt.current_fsm_state],
			g_rach_opt_gb_ctxt.rach_opt_global_mode,
			g_rach_opt_gb_ctxt.log_mode,
			SON_MODE_NAMES[g_rach_opt_gb_ctxt.rachopt_mode],
			g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id);

	rach_opt_print_cell_context(p_son_method_name);

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : rach_opt_print_pending_res
 * Inputs         : son_method_name - Invoking method name
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Prints RACH OPT pending response structure
 ****************************************************************************/
son_void_t 
rach_opt_print_pending_res
(
 const son_8 *son_method_name
 )
{
	SON_LIST_NODE           *p_node      = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;
	SON_UT_TRACE_ENTER();

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
			SON_DETAILED,
			"%s: Pending Response Structure:"
			"API Id: %d "
			"Transaction Id: %d "
			"Expected Response Count: %d"
			"Error Received Flag: %d"
			"Number of nodes in pending response list %u",
			son_method_name,
			g_rach_opt_pending_response.api_id,
			g_rach_opt_pending_response.transaction_id,
			g_rach_opt_pending_response.expected_res_count,
			g_rach_opt_pending_response.is_error_received,
			list_count(&g_rach_opt_pending_response.cell_sts_list));

	p_node = get_first_list_node(&g_rach_opt_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_INFO,
				"%s: Status for Cell 0x%x in Pending Response is "
				"error code = %u",
				son_method_name,
				son_convert_char_cell_id_to_int_cell_id(
					p_list_node->cell_status.cgi.cell_identity),
				p_list_node->cell_status.error_code);
		p_node = get_next_list_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_print_pending_res */

/*****************************************************************************
 * Function Name  : rach_opt_init_pending_resp_list
 * Inputs         : tid - Transaction Id
 *                : api - Procedure Code
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Initialize RACH OPT pending response structure
 ****************************************************************************/
son_void_t
rach_opt_init_pending_resp_list
(
 son_u16                  tid,
 son_procedure_code_et    api
 )
{
	SON_UT_TRACE_ENTER();
	g_rach_opt_pending_response.api_id = api;
	g_rach_opt_pending_response.transaction_id = tid;
	g_rach_opt_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_rach_opt_pending_response.is_error_received = SON_FALSE;
	list_init(&g_rach_opt_pending_response.cell_sts_list);
	SON_MEMSET(&g_rach_opt_pending_response.ts, 0, sizeof(time_stamp_t));
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
			SON_DETAILED,
			"RACH_OPT Pending Response "
			"Initialized");
	rach_opt_print_pending_res("rach_opt_init_pending_resp_list"); 

	SON_UT_TRACE_EXIT();
} /* rach_opt_init_pending_resp_list */

/*****************************************************************************
 * Function Name  : rach_opt_add_cell_in_pending_res
 * Inputs         : p_sts_node : Pointer to Cell Status Node
 * Outputs        : None
 * Returns        : SON_SUCCESS - In case of successful addition of Node,
 *                  SON_FAILURE - Otherwise
 * Description    : Add cell_status node in the RACH OPT pending response list 
 ****************************************************************************/
son_return_et
rach_opt_add_cell_in_pending_res
(
 son_cell_status_t *p_sts_node
 )
{
	cell_status_list_data_t *p_list_node = SON_PNULL;
	son_return_et ret_val = SON_FAILURE;
	son_size_t list_node_size = sizeof(cell_status_list_data_t);

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_sts_node && 
			g_rach_opt_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		p_list_node = (cell_status_list_data_t *)son_mem_get(list_node_size);
		if (SON_PNULL != p_list_node)
		{
			SON_MEMSET(p_list_node, 0, list_node_size);
			SON_MEMCPY(&p_list_node->cell_status, p_sts_node,
					sizeof(son_cell_status_t));
			list_insert_node(&g_rach_opt_pending_response.cell_sts_list,
					&p_list_node->cell_status_node);

			rach_opt_increment_pending_res_expected_count();
			ret_val = SON_SUCCESS;

			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Added "
					"Cell 0x%x in Pending Response list." 
					" Number of cells in Pending Response list: %u",
					son_convert_char_cell_id_to_int_cell_id(
						p_sts_node->cgi.cell_identity),
					g_rach_opt_pending_response.expected_res_count);
			rach_opt_print_pending_res("rach_opt_add_cell_in_pending_res");
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Memory allocation failure for p_list_node");
		}
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Invalid input parameter [p_sts_node is NULL] or "
				"pending response count(%u) reached max limit(%u)",
				g_rach_opt_pending_response.expected_res_count,
				MAX_PENDING_RES_COUNT);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* rach_opt_add_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : son_key_of_rach_opt
 * Inputs         : p_node - anchor field of the record
 * Outputs        : None
 * Returns        : Pointer to cell id as void pointer
 * Description    : This function determines the key in RACH OPT cell context 
 *                  list
 ******************************************************************************/
const son_void_t *
son_key_of_rach_opt
(
 const SON_LIST_NODE *p_node
 )
{
	rach_opt_cell_context_node_t *p_data = SON_PNULL;
	son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Pointer to node is NULL");
	}
	else
	{
		p_data = YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_node);
		p_cell_id = &(p_data->data.cell_id);
	}    

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_id;
} /* son_key_of_rach_opt */
 /*SPR_19279_START */
const son_void_t *
son_cell_index_key_of_rach_opt
(
 const SON_LIST_NODE *p_node
 )
{
	rach_opt_cell_context_node_t *p_data = SON_PNULL;
	son_u8  *p_cell_idx = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_node);
	p_cell_idx = &(p_data->data.cell_index);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_idx;
}
 /*SPR_19279_END */


/*****************************************************************************
 * Function Name  : rach_opt_compare_last_updated_req_data_with_rcvd_config_data
 * Inputs         : p_cell_data     Pointer to the cell context data receieved
 *                                  in cell info indication from OAM.
 *                  p_cell_context  Pointer to the last updated request data sent to OAM as a result of RACH OPT algo execution
 * Outputs        : None
 * Returns        : TRUE if all parameters are equal FALSE otherwise
 * Description    : This function is used to compare the configuration received
 *                  in cell info indication from OAM with the configuration
 *                  sent in update request to OAM as a result of RACH OPT algo 
 *                  execution
 ******************************************************************************/
static son_bool_et
rach_opt_compare_last_updated_req_data_with_rcvd_config_data
(
 son_cell_t                          *p_cell_data,
 son_oam_rach_opt_update_req_param_t *p_last_update_req_data
 )
{
	son_bool_et is_equal = SON_TRUE;
	SON_UT_TRACE_ENTER();

	do 
	{
		/* Check if preamble_initial_rcvd_target_power was requested to be 
		   updated */
		if (SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT & 
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			if (p_cell_data->preamble_initial_rcvd_target_power != 
					p_last_update_req_data->preamble_initial_rcvd_target_power)
			{
				/* Expected value is revceived but not same as expected value 
				   in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
		}
		/* Check if root_sequence_index was requested to be updated */
		if (SON_UPDATE_ROOT_SEQ_IDX_PRESENT &
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_PRACH_CONFIG_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			/* Expected value is revceived but not same as expected value 
			   in cell info indicaion */
			if (p_cell_data->prach_config.root_sequence_index != 
					p_last_update_req_data->root_sequence_index)
			{
				is_equal = SON_FALSE;
				break;
			}
		}
		/* Check if prach_frequency_offset was requested to be updated */
		if (SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT &
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_PRACH_CONFIG_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			/* Expected value is revceived but not same as expected value 
			   in cell info indicaion */
			if (p_cell_data->prach_config.prach_frequency_offset != 
					p_last_update_req_data->prach_frequency_offset)
			{
				is_equal = SON_FALSE;
				break;
			}
		}
		/* Check if prach_configuration_index was requested to be updated */
		if (SON_UPDATE_PRACH_CONFIG_IDX_PRESENT &
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_PRACH_CONFIG_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			/* Expected value is revceived but not same as expected value 
			   in cell info indicaion */
			if (p_cell_data->prach_config.prach_configuration_index != 
					p_last_update_req_data->prach_configuration_index)
			{
				is_equal = SON_FALSE;
				break;
			}
		}
		/* Check if number_of_ra_preambles was requested to be updated */
		if (SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT &
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			/* Expected value is revceived but not same as expected value 
			   in cell info indicaion */
			if (p_cell_data->number_of_ra_preambles != 
					p_last_update_req_data->number_of_ra_preambles)
			{
				is_equal = SON_FALSE;
				break;
			}
		}
		/* Check if size_of_ra_group_a was requested to be updated */
		if (SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT &
				p_last_update_req_data->bitmask)
		{
			/* Check if this requested parameter value is present in 
			   received message */
			if (!(SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT &
						p_cell_data->bitmask))
			{
				/* Expected value is not revceived in cell info indicaion */
				is_equal = SON_FALSE;
				break;
			}
			/* Expected value is revceived but not same as expected value 
			   in cell info indicaion */
			if (p_cell_data->size_of_ra_group_a != 
					p_last_update_req_data->size_of_ra_group_a)
			{
				is_equal = SON_FALSE;
				break;
			}
		}
	}while(0);

	SON_UT_TRACE_EXIT();
	return is_equal;
} /* rach_opt_compare_last_updated_req_data_with_rcvd_config_data */

/*****************************************************************************
 * Function Name  : rach_opt_compare_cell_context_data_with_rcvd_config_data
 * Inputs         : p_cell_data     Pointer to the cell context data receieved
 *                                  in cell info indication from OAM.
 *                  p_cell_context  Pointer to the cell context
 *                  p_prev_prach_data_at_anr    Previous RACH data stored at ANR
 * Outputs        : None
 * Returns        : bimask set for the parameters which are different
 * Description    : This function is used to compare the configuration received
 *                  in cell info indication from OAM with the configuration
 *                  already stored in cell context.
 ******************************************************************************/
static son_u32
rach_opt_compare_cell_context_data_with_rcvd_config_data
(
 son_cell_t                      *p_cell_data,
 rach_opt_cell_context_t         *p_cell_context,
 son_prach_configuration_t       *p_prev_prach_data_at_anr
 )
{
	son_u32 bitmask = 0;
	SON_UT_TRACE_ENTER();

	if (SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT & 
			p_cell_data->bitmask)
	{
		if (p_cell_data->preamble_initial_rcvd_target_power !=  
				p_cell_context->preamble_initial_rcvd_target_power)
		{
			bitmask |= SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT;
		}
	}
	if (SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT &
			p_cell_data->bitmask)
	{
		if (p_cell_data->number_of_ra_preambles != p_cell_context->
				number_of_ra_preambles)
		{
			bitmask |= SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT;
		}
	}
	if (SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT &
			p_cell_data->bitmask)
	{
		if (p_cell_data->size_of_ra_group_a != p_cell_context->
				size_of_ra_group_a)
		{
			bitmask |= SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT;
		}
	}

	if (SON_CELL_PRACH_CONFIG_PRESENT & p_cell_data->bitmask)
	{
		if (p_cell_data->prach_config.root_sequence_index !=
				p_prev_prach_data_at_anr->root_sequence_index)
		{
			bitmask |= SON_UPDATE_ROOT_SEQ_IDX_PRESENT;
		}
		if (p_cell_data->prach_config.prach_frequency_offset !=
				p_prev_prach_data_at_anr->prach_frequency_offset)
		{
			bitmask |= SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT;
		}
		if ((SON_PRACH_CONFIG_INDEX_PRESENT & p_cell_data->prach_config.bitmask) &&
				(p_cell_data->prach_config.prach_configuration_index != 
				 p_prev_prach_data_at_anr->prach_configuration_index))
		{
			bitmask |= SON_UPDATE_PRACH_CONFIG_IDX_PRESENT;
		}
	}
	if (SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT &
			p_cell_data->bitmask)
	{
		if (p_cell_data->pusch_hopping_offset != p_cell_context-> 
				pusch_hopping_offset)
		{
			bitmask |= SON_UPDATE_PUSCH_HOPPING_OFFSET_PRESENT;;
		}
	}

	SON_UT_TRACE_EXIT();
	return bitmask;
} /* rach_opt_compare_cell_context_data_with_rcvd_config_data */

/*****************************************************************************
 * Function Name  : rach_opt_modify_cell_context_with_config_rcvd
 * Inputs         : p_cell_data     Pointer to the cell context data receieved
 *                                  in cell info indication from OAM.
 * Outputs        : p_cell_context  Pointer to the updated cell context
 * Returns        : son_void_t
 * Description    : This function is used to modify the cell context of a 
 *                  RACH OPT cell with the data received in cell info 
 *                  indication from OAM 
 ******************************************************************************/
static son_void_t
rach_opt_modify_cell_context_with_config_rcvd
(
 son_cell_t                      *p_cell_data,
 rach_opt_cell_context_t         *p_cell_context
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_BRIEF,
			"Cell 0x%x is updated with new configuration received "
			"in cell info indication message.",
			son_convert_char_cell_id_to_int_cell_id(
				p_cell_data->src_cgi.cell_identity));
	/* Modify the parameters stored in RACH cell context. Rest would have 
	   already been updated by ANR in its cell context */
	if (SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT & 
			p_cell_data->bitmask)
	{
		p_cell_context->preamble_initial_rcvd_target_power =
			p_cell_data->preamble_initial_rcvd_target_power;
	}
	if (SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->number_of_ra_preambles =
			p_cell_data->number_of_ra_preambles;
	}
	if (SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->size_of_ra_group_a =
			p_cell_data->size_of_ra_group_a;
	}
	if (SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->pusch_hopping_offset = 
			p_cell_data->pusch_hopping_offset;
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_modify_cell_context_with_config_rcvd */

/*****************************************************************************
 * Function Name  : rach_opt_modify_cell_context_for_rach_enabled_cell
 * Inputs         : p_cell_data     Pointer to the cell context data receieved
 *                                  in cell info indication from OAM.
 *                  p_prev_prach_data_at_anr    Previous RACH data stored at ANR
 * Outputs        : p_cell_context  Pointer to the updated cell context
 * Returns        : son_void_t
 * Description    : This function is used to modify the cell context of a 
 *                  RACH OPT enabled cell with the data received in cell 
 *                  info indication from OAM 
 ******************************************************************************/
static son_void_t
rach_opt_modify_cell_context_for_rach_enabled_cell
(
 son_cell_t                      *p_cell_data,
 rach_opt_cell_context_t         *p_cell_context,
 son_prach_configuration_t       *p_prev_prach_data_at_anr
 )
{
	son_u32 bitmask = 0;
	SON_UT_TRACE_ENTER();

	/* Check if the some new data is received in cell info indication message */
	if (0 == (bitmask = 
				rach_opt_compare_cell_context_data_with_rcvd_config_data(
					p_cell_data, p_cell_context, 
					p_prev_prach_data_at_anr)))
	{
		/* If cell info inidication is awaited but now the cell info indication
		   is received with no new configuration then stop algo and notify 
		   OAM else do nothing */
		if (SON_TRUE == p_cell_context->is_cell_info_ind_awaited)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Cell Info indication message receievd with no new "
					"configuration for Cell 0x%x so RACH OPT algo "
					"is reset.",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_data->src_cgi.cell_identity));
			rach_opt_init_rach_counters(p_cell_context);
		}
	}
	else
	{
		/* Cell Info indication is received so need to skip RRM reports
		   for configured number of times to wait for network stability */
		p_cell_context->no_of_windows_to_be_skipped_for_optimization = 
			p_cell_context->read_only_config_params.
			rachopt_no_of_windows_to_skip_for_optimization;
		/* Store the configuration received in cell info ind 
		   in the cell context. */
		rach_opt_modify_cell_context_with_config_rcvd(
				p_cell_data, p_cell_context);
		/* If cell info inidication is awaited */
		if (SON_TRUE == p_cell_context->is_cell_info_ind_awaited)
		{
			/* awaited cell info inidication is received so reset the flag */
			p_cell_context->is_cell_info_ind_awaited = SON_FALSE;
			/* The cell info indication is received with configuration data 
			   as suggested by algo then update the context with the new 
			   parameters and continue algo */
			if (bitmask == p_cell_context->last_update_req_data.bitmask)
			{
				if (SON_FALSE == 
						rach_opt_compare_last_updated_req_data_with_rcvd_config_data(
							p_cell_data, 
							&p_cell_context->last_update_req_data))
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Cell Info indication message receievd with the "
							"data different from the data suggested by RACH "
							"OPT algorithm for Cell 0x%x so RACH "
							"OPT algo is reset.",
							son_convert_char_cell_id_to_int_cell_id(
								p_cell_data->src_cgi.cell_identity));
					rach_opt_init_rach_counters(p_cell_context);
				}
				else
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_DETAILED,
							"Cell Info indication message receievd with the "
							"data as suggested by RACH OPT algorithm for "
							"Cell 0x%x]",
							son_convert_char_cell_id_to_int_cell_id(
								p_cell_data->src_cgi.cell_identity));
				}
			}
			else
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_WARNING,
						"Cell Info indication message receievd with the data "
						"different from the data suggested by RACH OPT "
						"algorithm for Cell 0x%x so RACH OPT algo "
						"is reset.",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_data->src_cgi.cell_identity));
				rach_opt_init_rach_counters(p_cell_context);
			}
			SON_MEMSET(&p_cell_context->last_update_req_data, 0, 
					sizeof(son_oam_rach_opt_update_req_param_t));
		}
		/* Cell info indication received with new data but it was not
		   awaited so operator has taken decision to modify cell 
		   configuration on its own not on the basis of RACH OPT algo output 
		   so apply the new configuration and restart the algo */
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Cell Info indication message receievd for Cell 0x%x "
					"with new data but it was not awaited."
					"Operator has taken decision to modify cell configuration "
					"on its own not on the basis of RACH OPT algo output so "
					"apply the new configuration and restart the algo.",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_data->src_cgi.cell_identity));

			rach_opt_init_rach_counters(p_cell_context);
		}
	}
	SON_UT_TRACE_EXIT();

} /* rach_opt_modify_cell_context_for_rach_enabled_cell */

/*****************************************************************************
 * Function Name  : rach_opt_modify_cell_context
 * Inputs         : p_cell_data     Pointer to the cell context data receieved
 *                                  in cell info indication from OAM.
 * Outputs        : p_cell_context  Pointer to the updated cell context
 * Returns        : son_void_t
 * Description    : This function is used to modify the cell context with the
 *                  data received in cell info indication from OAM
 ******************************************************************************/
son_void_t
rach_opt_modify_cell_context
(
 son_cell_t                      *p_cell_data, 
 rach_opt_cell_context_t         *p_cell_context
 )
{
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
	SON_UT_TRACE_ENTER();

	/* Get LTE SON/ANR cell context */
	p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
			&p_cell_context->cell_id);

	if (SON_PNULL == p_anr_cell_ctxt)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Failed to find ANR cell "
				"context for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_data->src_cgi.cell_identity));

		SON_UT_TRACE_EXIT();
		return; 
	}

	if (RACH_OPT_CELL_STATE_ENABLED == 
			p_cell_context->current_cell_fsm_state)
	{
		rach_opt_modify_cell_context_for_rach_enabled_cell(p_cell_data, 
				p_cell_context, &p_anr_cell_ctxt->rach_related_info.
				old_prach_configuration);
	}
	else
	{
		rach_opt_modify_cell_context_with_config_rcvd(p_cell_data,
				p_cell_context);
	}
	/* Reset the RACH data of previous cell info indicaiton message at ANR */
	SON_MEMSET(&p_anr_cell_ctxt->rach_related_info, 0, 
			sizeof(anr_rach_related_info_t));

	SON_UT_TRACE_EXIT();
} /* rach_opt_modify_cell_context */

/*****************************************************************************
 * Function Name  : rach_opt_init_rach_counters
 * Inputs         : None
 * Outputs        : p_cell_context     - Pointer to the cell context structure
 * Returns        : son_void_t
 * Description    : This function initialize the rach algo counters 
 available  at the cell context
 ****************************************************************************/
son_void_t
rach_opt_init_rach_counters
(
 rach_opt_cell_context_t         *p_cell_context
 )
{
	p_cell_context->is_rach_algo_started = SON_FALSE;
	p_cell_context->rach_algo_run_count_after_started = 0;
	p_cell_context->previous_rach_failure_percentage = 0.0;
	SON_MEMSET(&p_cell_context->rach_algo_counters, 0, 
			sizeof(rach_opt_algo_counters_context_t));
	p_cell_context->is_algo_counters_updated_from_rrm_ue_report = SON_FALSE;
	p_cell_context->is_algo_counters_updated_from_rrm_l2_report = SON_FALSE;
	p_cell_context->previous_optimization_type = SON_RACH_OPT_TYPE_UNDEFINED;
	/* Clear strongest neighbor cells List */
	rach_opt_clear_strongest_nbr_cells_list(p_cell_context);
	p_cell_context->is_cell_info_ind_awaited = SON_FALSE;
	p_cell_context->no_of_windows_to_be_skipped_for_optimization = 0;
	p_cell_context->reports_ignored_in_cell_info_ind_awaited_state = 0;
	SON_MEMSET(&p_cell_context->last_update_req_data, 0, 
			sizeof(son_oam_rach_opt_update_req_param_t));
}

/*****************************************************************************
 * Function Name  : rach_opt_init_cell_context
 * Inputs           p_cell_data    - Pointer to the cell data received in cell 
 *                  info indication message
 * Outputs        : p_cell_context - Pointer to the RACH OPT cell context
 * Returns        : son_void_t
 * Description    : This function initialize the newly created RACH OPT cell 
 *                  context
 ******************************************************************************/
static son_void_t
rach_opt_init_cell_context
(
 rach_opt_cell_context_t         *p_cell_context,
 son_cell_t                      *p_cell_data     
 )
{
	SON_UT_TRACE_ENTER();
	SON_MEMSET(p_cell_context, 0, sizeof(rach_opt_cell_context_t));
	p_cell_context->preamble_initial_rcvd_target_power = 0xFF;
	p_cell_context->pusch_hopping_offset = 0xFF;
	p_cell_context->number_of_ra_preambles = 0xFF;
	p_cell_context->size_of_ra_group_a = 0xFF;

	SON_MEMCPY(&p_cell_context->cell_id, &p_cell_data->src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));
	p_cell_context->previous_cell_fsm_state = 
		RACH_OPT_CELL_STATE_DISABLED;
	p_cell_context->current_cell_fsm_state = 
		RACH_OPT_CELL_STATE_DISABLED;
	p_cell_context->oam_config_params.rachopt_report_periodicity = 
		g_config_params.rach_config_params.rach_cell_level_config_params.
		rachopt_report_periodicity;
	p_cell_context->oam_config_params.rachopt_min_ue_rach_report_count =
		g_config_params.rach_config_params.rach_cell_level_config_params.
		rachopt_min_ue_rach_report_count;
	p_cell_context->oam_config_params.rachopt_ue_rach_attempts_threshold =
		g_config_params.rach_config_params.rach_cell_level_config_params.
		rachopt_ue_rach_attempts_threshold;
	p_cell_context->oam_config_params.rachopt_start_threshold =
		(son_float)g_config_params.rach_config_params.
		rach_cell_level_config_params.rachopt_start_threshold / 100;
	p_cell_context->oam_config_params.rachopt_stop_threshold =
		(son_float)g_config_params.rach_config_params.
		rach_cell_level_config_params.rachopt_stop_threshold / 100;
	p_cell_context->oam_config_params.rachopt_degrade_stop_offset =
		(son_float)g_config_params.rach_config_params.
		rach_cell_level_config_params.rachopt_degrade_stop_offset / 100;
	p_cell_context->oam_config_params.
		rachopt_max_iterations_to_reach_stop_threshold =
		g_config_params.rach_config_params.rach_cell_level_config_params.
		rachopt_max_iterations_to_reach_stop_threshold;
	p_cell_context->oam_config_params.rachopt_contention_threshold =
		g_config_params.rach_config_params.
		rach_cell_level_config_params.rachopt_contention_threshold;
	p_cell_context->oam_config_params.rachopt_power_rampup_threshold =
		g_config_params.rach_config_params.
		rach_cell_level_config_params.rachopt_power_rampup_threshold;
	p_cell_context->oam_config_params.rachopt_min_cfra_preambles =
		g_config_params.rach_config_params.rach_cell_level_config_params.
		rachopt_min_cfra_preambles;
	SON_MEMCPY(&p_cell_context->read_only_config_params,
			&g_config_params.rach_config_params.read_only_config_params,
			sizeof(son_rach_opt_read_only_config_param_t));
	if (SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->preamble_initial_rcvd_target_power = 
			p_cell_data->preamble_initial_rcvd_target_power;
	}
	if (SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT & 
			p_cell_data->bitmask)
	{
		p_cell_context->number_of_ra_preambles = 
			p_cell_data->number_of_ra_preambles;
	}
	if (SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->size_of_ra_group_a = 
			p_cell_data->size_of_ra_group_a;
	}
	if (SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT &
			p_cell_data->bitmask)
	{
		p_cell_context->pusch_hopping_offset = 
			p_cell_data->pusch_hopping_offset;
	}
	p_cell_context->dereg_invoking_api = 
		SON_PROCEDURE_UNDEFINED;
	list_init(&p_cell_context->rach_opt_strongest_nbr_cell_list);

	/* RACH Alogrithm specific fields initialization */
	rach_opt_init_rach_counters(p_cell_context);

	SON_UT_TRACE_EXIT();
}

/* Rel 3.0 Multi Sector code changes Start */

/*SPR-11554-Start-Function Modified*/

/*****************************************************************************
 * Function Name  : rach_opt_validate_cell_data
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : None
 * Returns        : SON_TRUE: If valid
 *                  SON_FALSE:If not valid
 * Description    : This function validates the data of cell received in cell
 *                  info indication
 ******************************************************************************/
static son_bool_et
rach_opt_validate_cell_data
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
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Invalid pci range[open pci_start=%d, pci_end=%d] received",
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
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Invalid pci range[closed pci_start=%d, pci_end=%d] received",
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
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Invalid pci range[hybrid pci_start=%d, pci_end=%d] received",
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

	/* Check if it is the first cell being configured at Rach_opt */
	if (0 == g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list.count)
	{
		/*Check if the serving cell is home or macro*/
		if(is_home_eNB == SON_TRUE)
		{
			g_rach_opt_gb_ctxt.enb_type = SON_HOME_ENB;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,
					SON_INFO,
					"The serving eNB is a home eNB");
		}
		else
		{
			g_rach_opt_gb_ctxt.enb_type = SON_MACRO_ENB;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,
					SON_INFO,
					"The serving eNB is a macro eNB");
		}

	}

	/* Check if 1 cell is already configured and if the eNB is
	 * home eNB do not configure any other cell as home eNB can
	 * have only one serving cell */

	else if (SON_HOME_ENB == g_rach_opt_gb_ctxt.enb_type)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_INFO,
				"Cell 0x%x"
				" is not added to RACH-OPT Global Context - Home eNB is"
				" already configured with 1 serving cell",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}
	else if ((SON_MACRO_ENB == g_rach_opt_gb_ctxt.enb_type) &&
			(is_home_eNB == SON_TRUE))
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_INFO,
				"Cell 0x%x"
				" is not added to RACH-OPT Global Context - macro eNB "
				" eNB is already configured as macro,cannot configure femto cell",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_valid;

} /* rach_opt_validate_cell_data */

/*SPR-11554-End*/

/* Rel 3.0 Multi Sector code changes Stop */

/*****************************************************************************
 * Function Name  : rach_opt_add_mod_cell_context
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : p_cause - Cause of failure if failure occurs in addition
 *                  p_state - RACH OPT feature state for the cell
 * Returns        : Pointer to the new node added in the list, NULL if node
 *                  already existing node is modified or creation of node fails
 * Description    : This function adds new cell in the RACH OPT cell context or  *                  modify the existing data of already configured cell
 ******************************************************************************/
rach_opt_cell_context_node_t *
rach_opt_add_mod_cell_context
(
 son_cell_t                          *p_cell,
 son_feature_state_et                *p_state,
 /*SPR_19279_START */
 son_feature_state_change_cause_et   *p_cause,
 son_u8 cell_index
 /*SPR_19279_END */
 )
{
	rach_opt_cell_context_node_t *p_new_node = SON_PNULL;
	son_size_t cell_context_node_size = sizeof(rach_opt_cell_context_node_t);
	SON_LIST_NODE *p_list_node = SON_PNULL;
	rach_opt_cell_context_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell || SON_PNULL == p_cause || 
			SON_PNULL == p_state)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_ERROR,
				"Cell ID or pointer to cause is NULL");
	}
	else 
	{
		/* Check if Cell is already present in RACH OPT cell configuration 
		   list */
        /*SPR_19279_START */
		p_list_node = list_find_node(
				&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
				&cell_index, son_cell_index_key_of_rach_opt,
				son_compare_cell_index);
        /*SPR_19279_END */

		if (SON_PNULL == p_list_node)
		{
			if (g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list.count < 
					SON_MAX_CELL_SUPPORTED_AT_ENB)
			{
				/* Rel 3.0 Multi Sector code changes Start */
				if (SON_FALSE == rach_opt_validate_cell_data(p_cell))
				{
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_INTERNAL_FAILURE;
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}
				/* Rel 3.0 Multi Sector code changes Stop */

				/* Create a new node */       
				p_new_node = (rach_opt_cell_context_node_t *)son_mem_get(
						cell_context_node_size);

				if (SON_PNULL != p_new_node)
				{
					/* Populate the node*/ 
					rach_opt_init_cell_context(&p_new_node->data, p_cell);
					/* Insert it in context list */
                    /*SPR_19279_START */
                    p_new_node->data.cell_index = cell_index;
                    /*SPR_19279_END */
					list_insert_node(
							&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
							&p_new_node->sNode);
					*p_state = SON_STATE_DISABLED;
					*p_cause = SON_RACH_OPT_MODE_DISABLED;
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
							"Added Cell 0x%x in RACH OPT Global Context",
							son_convert_char_cell_id_to_int_cell_id(
								p_cell->src_cgi.cell_identity));
					rach_opt_print_cell_context(
							"rach_opt_add_mod_cell_context");
				}
				else
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_ERROR,
							"Memory allocation failed for p_new_node");
					*p_state = SON_STATE_DELETED;
					*p_cause = SON_MEM_ALLOC_FAILURE;
				}
			}
			else
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_ERROR,
						"Cell 0x%x could not be added to RACH OPT Global Context - "
						"Maximum(%u) number of supported cells limit reached",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell->src_cgi.cell_identity),
						list_count(&g_rach_opt_gb_ctxt.
							rach_opt_cell_ctxt_list));
				*p_state = SON_STATE_DELETED;
				*p_cause = SON_MAX_SUPPORTED_CELL_LIMIT_REACHED;
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Cell 0x%x "
					"already exists in RACH OPT Cell Context List.",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell->src_cgi.cell_identity));
			p_data = YMEMBEROF(rach_opt_cell_context_node_t, sNode, 
					p_list_node);
            /*SPR_19279_START*/
            if (SON_MEMCMP(&p_data->data.cell_id,
                        &p_cell->src_cgi, sizeof(p_new_node->data.cell_id)))
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_INFO,
                        "ECGI for Cell 0x%x updated",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

                SON_MEMCPY(&p_data->data.cell_id,
                        &p_cell->src_cgi,
                        sizeof(p_data->data.cell_id));
            }
            /*SPR_19279_END*/

			rach_opt_modify_cell_context(p_cell, &p_data->data);
			rach_opt_print_cell_context(
					"rach_opt_add_mod_cell_context");
		}
	}

	SON_UT_TRACE_EXIT();
	return p_new_node;
} /* rach_opt_add_mod_cell_context */

/*****************************************************************************
 * Function Name  : rach_opt_reset_pending_resp_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Resets RACH OPT pending_response structure
 ****************************************************************************/
son_void_t
rach_opt_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	g_rach_opt_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
	g_rach_opt_pending_response.transaction_id = 0;
	g_rach_opt_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_rach_opt_pending_response.is_error_received = SON_FALSE;
	if (g_rach_opt_pending_response.cell_sts_list.count)
	{
		rach_opt_clear_pending_res_list();
	}

	list_init(&g_rach_opt_pending_response.cell_sts_list);
	SON_MEMSET(&g_rach_opt_pending_response.ts, 0, sizeof(time_stamp_t));
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_DETAILED,
			"RACH OPT Pending Response "
			"has been reset");
	rach_opt_print_pending_res("rach_opt_reset_pending_resp_list"); 

	SON_UT_TRACE_EXIT();
} /* rach_opt_reset_pending_resp_list */

/*****************************************************************************
 * Function Name  : rach_opt_send_feature_state_change_ind
 * Inputs         : p_cell_id - Pointer to the cell Id
 *                  state - RACH OPT state for the cell
 *                  cause - Cause for the present state of RACH OPT for the cell
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates and sends the feature state 
 *                  change indication message to SONMgmtIfH.
 ****************************************************************************/
son_void_t
rach_opt_send_feature_state_change_ind
(
 son_intra_rat_global_cell_id_t      *p_cell_id,
 son_feature_state_et                state,
 son_feature_state_change_cause_et   cause
 )
{
	son_rach_opt_feature_state_change_ind_t state_change_ind;
	son_size_t state_change_ind_len = 
		sizeof(son_rach_opt_feature_state_change_ind_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&state_change_ind, 0, 
			state_change_ind_len);
	/* Copy the CGI */
	SON_MEMCPY(&state_change_ind.srv_cgi, p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Set the Module ID */
	state_change_ind.module_id = SON_RACH_OPT_MODULE_ID;
	state_change_ind.state = state;
	state_change_ind.cause = cause;

	son_create_send_buffer((son_u8 *)&state_change_ind,
			SON_RACH_OPT_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONRACH_OPT_FEATURE_STATE_CHANGE_IND,
			state_change_ind_len);

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_BRIEF,
			"Feature state change "
			"indication sent for Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				state_change_ind.srv_cgi.cell_identity));

	SON_UT_TRACE_EXIT();
} /* rach_opt_send_feature_state_change_ind */

/*****************************************************************************
 * Function name  : rach_opt_get_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16 - Transaction Id
 * Description    : Provides transaction id present in RACH OPT pending response
 ****************************************************************************/
son_u16
rach_opt_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_rach_opt_pending_response.transaction_id;
}

/*****************************************************************************
 * Function Name  : rach_opt_get_cell_context_from_global_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : Pointer to the cell context 
 * Description    : This function gets the cell context of the specified cell 
 *                  from RACH OPT's global context
 ******************************************************************************/
rach_opt_cell_context_t *
rach_opt_get_cell_context_from_global_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	rach_opt_cell_context_node_t *p_data = SON_PNULL;
	SON_LIST_NODE *p_list_node = SON_PNULL;
	rach_opt_cell_context_t *p_ret_val = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	p_list_node = list_find_node(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
			p_cell_id, son_key_of_rach_opt, son_compare_cell_id);

	if (SON_PNULL != p_list_node)
	{
		p_data = YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_list_node);
		p_ret_val = &p_data->data;
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Cell 0x%x not found",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* rach_opt_get_cell_context_from_global_context */

/*****************************************************************************
 * Function Name  : rach_opt_is_any_cell_disabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in 
 *                  RACH_OPT_CELL_STATE_DISABLED or
 *                  RACH_OPT_CELL_STATE_DISABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in 
 *                  RACH_OPT_CELL_STATE_DISABLED or 
 *                  RACH_OPT_CELL_STATE_DISABLING state.
 *                  If such cell is found it indicates that global enable 
 *                  request can be processed as some cells needs to be enabled.
 ****************************************************************************/
son_bool_et
rach_opt_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_bool_et cell_exists = SON_FALSE;
	rach_opt_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = rach_opt_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		if (RACH_OPT_CELL_STATE_DISABLED == p_node->data.current_cell_fsm_state
				|| RACH_OPT_CELL_STATE_DISABLING == 
				p_node->data.current_cell_fsm_state)
		{
			cell_exists = SON_TRUE;
			break;
		}

		p_node = rach_opt_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
	return cell_exists;
} /* rach_opt_is_any_cell_disabled */

/*****************************************************************************
 * Function Name  : rach_opt_get_first_cell_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to cell context of first cell in list
 * Description    : This function gets the cell context of the first cell 
 *                : in the RACH OPT cell context list
 ******************************************************************************/
rach_opt_cell_context_node_t *
rach_opt_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (0 != list_count(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list))
	{
		p_node = get_first_list_node(&g_rach_opt_gb_ctxt.
				rach_opt_cell_ctxt_list);
		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (rach_opt_cell_context_node_t *)
				YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* rach_opt_get_first_cell_ctxt */

/*****************************************************************************
 * Function Name  : rach_opt_get_next_node
 * Inputs         : p_node   Pointer to the current cell context
 * Outputs        : None
 * Returns        : cell context pointer of the next cell
 * Description    : This function gets the  next RACH OPT cell context in the list
 ******************************************************************************/
rach_opt_cell_context_node_t *
rach_opt_get_next_node
(
 rach_opt_cell_context_node_t *p_node
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->sNode);
		if (SON_PNULL != p_list_node)
		{
			return (rach_opt_cell_context_node_t *)
				YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* rach_opt_get_next_node */

/*****************************************************************************
 * Function Name  : rach_opt_populate_enable_disable_res_data_from_pending_list
 * Inputs         : None
 * Outputs        : p_cell_status_list_size - Number of cells to be send in 
 *                  response
 *                  p_cell_status_list  - Pointer to list containg cell Ids and 
 *                  error code indicating the reason for failure
 * Returns        : son_void_t
 * Description    : This function populates enable or disable response data 
 *                  struture with the cell Ids and the error codes of the cell 
 *                  for which (de)registration request could not be sent to 
 *                  RRM due to some RACH OPT error or may be because ANR mode 
 *                  is not enabled for the cell or cell context is not found 
 *                  at ANR.
 ****************************************************************************/
son_void_t
rach_opt_populate_enable_disable_res_data_from_pending_list
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
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"p_cell_status_list_size or p_cell_status_list"
				"is NULL");

		SON_UT_TRACE_EXIT();
		return;
	}

	p_node = get_first_list_node(&g_rach_opt_pending_response.cell_sts_list);
	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_BRIEF,
				"Status for Cell 0x%x in Pending "
				"Response is error code = %u",
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
} /* rach_opt_populate_enable_disable_res_data_from_pending_list */

/*****************************************************************************
 * Function Name  : rach_opt_is_any_cell_enabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE - If a cell is found in 
 *                  RACH_OPT_CELL_STATE_ENABLED or 
 *                  RACH_OPT_CELL_STATE_ENABLING state
 *                  SON_FALSE - Otherwise
 * Description    : Checks if at least one cell exists which is in 
 *                  RACH_OPT_CELL_STATE_ENABLED or 
 *                  RACH_OPT_CELL_STATE_ENABLING state.
 *                  If such cell is found it indicates that global disable 
 *                  request can be processed as some cells needs to be disabled.
 ****************************************************************************/
son_bool_et
rach_opt_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_bool_et cell_exists = SON_FALSE;
	rach_opt_cell_context_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = rach_opt_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		if (RACH_OPT_CELL_STATE_ENABLED == p_node->data.current_cell_fsm_state
				|| RACH_OPT_CELL_STATE_ENABLING == 
				p_node->data.current_cell_fsm_state)    
		{
			cell_exists = SON_TRUE;
			break;
		}

		p_node = rach_opt_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
	return cell_exists;
} /* rach_opt_is_any_cell_enabled */

/*****************************************************************************
 * Function Name  : rach_opt_get_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : Number of expected response count
 * Description    : Provides the expected response count present in RACH OPT 
 *                  pending response list
 ****************************************************************************/
son_u8
rach_opt_get_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_rach_opt_pending_response.expected_res_count;
} /* rach_opt_get_pending_res_expected_count */

/*****************************************************************************
 * Function Name  : rach_opt_decrement_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et- Success/failure
 * Description    : Decrement Expected Response Count present in pending 
 *                  response by 1
 ****************************************************************************/
son_return_et
rach_opt_decrement_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();

	if (MIN_PENDING_RES_COUNT < g_rach_opt_pending_response.expected_res_count)
	{
		g_rach_opt_pending_response.expected_res_count--;
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Min value"
				" of Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* rach_opt_decrement_pending_res_expected_count */

/*****************************************************************************
 * Function Name  : rach_opt_increment_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et- Success/failure
 * Description    : Increment Expected Response Count present in pending 
 *                  response by 1
 ****************************************************************************/
son_return_et
rach_opt_increment_pending_res_expected_count
(  
)
{
	SON_UT_TRACE_ENTER();
	g_rach_opt_pending_response.expected_res_count++;
	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* rach_opt_increment_pending_res_expected_count */

/*****************************************************************************
 * Function Name  : rach_opt_delete_cell_from_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : SON_TRUE  :  if node deleted 
 *                : SON_FALSE :  if node not deleted
 * Description    : This function deletes the specified cell context from the 
 *                  cell context list
 ******************************************************************************/
son_bool_et
rach_opt_delete_cell_from_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	son_bool_et is_node_deleted = SON_FALSE;
	SON_LIST_NODE *p_list_node = SON_PNULL;
	rach_opt_cell_context_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell_id)
	{
		p_list_node = list_find_node(&g_rach_opt_gb_ctxt.
				rach_opt_cell_ctxt_list, p_cell_id, son_key_of_rach_opt, 
				son_compare_cell_id);

		if (SON_PNULL != p_list_node)
		{
			p_data = (rach_opt_cell_context_node_t *)
				YMEMBEROF(rach_opt_cell_context_node_t, sNode, p_list_node);

			list_delete_node(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
					p_list_node);

			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Deleted Cell 0x%x from RACH OPT Global Context",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
			rach_opt_print_global_context("rach_opt_delete_cell_from_context");

			/* Free the memory occupied by the node */
			son_mem_free(p_data);
			is_node_deleted = SON_TRUE;
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Cell 0x%x "
					"could not be deleted as cell not found in RACH OPT Cell"
					" Context List",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
	return is_node_deleted;
} /* rach_opt_delete_cell_from_context */

/*****************************************************************************
 * Function Name  : rach_opt_delete_cell_from_pending_res
 * Inputs         : p_cell_id : Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete node from the pendig response list
 ****************************************************************************/
son_void_t
rach_opt_delete_cell_from_pending_res
(
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	cell_status_list_data_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_id)
	{
		p_node = rach_opt_find_cell_in_pending_res(p_cell_id);

		if (SON_PNULL != p_node)
		{
			list_delete_node(&g_rach_opt_pending_response.cell_sts_list,
					&p_node->cell_status_node);

			son_mem_free(p_node);
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Deleted Cell 0x%x from Pending Response list",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
			rach_opt_print_pending_res("rach_opt_delete_cell_from_pending_res");
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Node for "
					"Cell 0x%x not found in pending response list",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_delete_cell_from_pending_res */


/*****************************************************************************
 * Function Name  : rach_opt_get_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et
 * Description    : Get API ID present in pending response
 ****************************************************************************/
son_procedure_code_et
rach_opt_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_rach_opt_pending_response.api_id;
} /* rach_opt_get_pending_res_api_id */

/*****************************************************************************
 * Function Name  : rach_opt_construct_cell_arr_from_pending_res_list
 * Inputs         : cell_fsm_state      Cell FSM state to be set
 * Outputs        : p_cell_sts_arr      pointer to cell status array
 *                  p_cell_sts_arr_size Pointer to cell status array size
 * Returns        : son_void_t
 * Description    : Construct cell status array from pending response list
 ****************************************************************************/
son_void_t 
rach_opt_construct_cell_arr_from_pending_res_list
(
 son_u16                     *p_cell_sts_arr_size,
 son_cell_status_t           *p_cell_sts_arr,
 rach_opt_cell_fsm_state_et  cell_fsm_state
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;
	rach_opt_cell_context_node_t *p_cell_data = SON_PNULL;
	SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_sts_arr || 
			SON_PNULL == p_cell_sts_arr_size)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Could not "
				"construct cell array from pending response list, "
				"p_cell_sts_arr or p_cell_sts_arr_size is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_DETAILED,
			"Constructing Cell Array from pending Response");

	rach_opt_print_pending_res(
			"rach_opt_construct_cell_arr_from_pending_res_list");

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
			if (RACH_OPT_CELL_STATE_DISABLED == cell_fsm_state ||
					RACH_OPT_CELL_STATE_ENABLED == cell_fsm_state)
			{
				/* Change cell FSM state to cell_fsm_state 
				 * (either RACH_OPT_CELL_STATE_DISABLED or 
				 RACH_OPT_CELL_STATE_ENABLED based on the 
				 registration/deregistration response awaited respectively) */
				p_cell_list_node = list_find_node(
						&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
						&p_list_node->cell_status.cgi, son_key_of_rach_opt,
						son_compare_cell_id);

				if (SON_PNULL != p_cell_list_node)
				{
					p_cell_data = YMEMBEROF(rach_opt_cell_context_node_t, 
							sNode, p_cell_list_node);
					if (SON_PNULL != p_cell_data)
					{    
						SONRACH_OPT_CELL_FSM_SET_STATE(
								(&p_cell_data->data),
								p_cell_data->data.previous_cell_fsm_state,
								p_cell_data->data.current_cell_fsm_state,
								cell_fsm_state);
					}
				}
			}
		}

		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_construct_cell_arr_from_pending_res_list */

/*****************************************************************************
 * Function Name  : rach_opt_find_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id : Pointer to the cell Id
 * Outputs        : None
 * Returns        : Pointer to the node of the pending response list if cell Id 
 *                  is found, NULL otherwise
 * Description    : Find node of the pendign response list with given cell Id
 ****************************************************************************/
cell_status_list_data_t *
rach_opt_find_cell_in_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_node = list_find_node(&g_rach_opt_pending_response.cell_sts_list,
			(son_void_t *)p_rcvd_cell_id, son_key_of_rach_opt,
			son_compare_cell_id);
	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
				p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* rach_opt_find_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : rach_opt_clear_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all nodes from RACH OPT pending response list
 ****************************************************************************/
son_void_t
rach_opt_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);
	}

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_DETAILED,
			"Cleared pending response list");
	rach_opt_print_pending_res("rach_opt_clear_pending_res_list"); 

	SON_UT_TRACE_EXIT();
} /* rach_opt_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : rach_opt_get_first_strongest_nbr_cell
 * Inputs         : p_list Pointer to the strongest neighbor cell list
 * Outputs        : Pointer to the first node of strongest neighbor cell list
 * Returns        : Pointer to the first node of strongest neighbor cell list
 * Description    : This function gets the first node of strongest neighbor 
 *                  cell list
 ******************************************************************************/
rach_opt_strongest_nbr_cell_node_t *
rach_opt_get_first_strongest_nbr_cell
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (0 != list_count(p_list))
	{
		p_node = get_first_list_node(p_list);
		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (rach_opt_strongest_nbr_cell_node_t *)
				YMEMBEROF(rach_opt_strongest_nbr_cell_node_t, sNode, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* rach_opt_get_first_strongest_nbr_cell */

/*****************************************************************************
 * Function Name  : rach_opt_get_next_strongest_nbr_cell
 * Inputs         : p_node   Pointer to the current node of list
 * Outputs        : None
 * Returns        : Pointer to the next node of strongest neighbor cell list
 * Description    : This function gets the next node in the strongest neighbor 
 *                  cell list
 ******************************************************************************/
rach_opt_strongest_nbr_cell_node_t *
rach_opt_get_next_strongest_nbr_cell
(
 rach_opt_strongest_nbr_cell_node_t *p_node
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->sNode);
		if (SON_PNULL != p_list_node)
		{
			return (rach_opt_strongest_nbr_cell_node_t *)
				YMEMBEROF(rach_opt_strongest_nbr_cell_node_t, sNode, 
						p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* rach_opt_get_next_strongest_nbr_cell */

/*****************************************************************************
 * Function Name  : rach_opt_clear_strongest_nbr_cells_list
 * Inputs         : p_cell_context Pointer to the RACH OPT cell context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all the nodes from strongest neighbor cells list
 ****************************************************************************/
son_void_t
rach_opt_clear_strongest_nbr_cells_list
(
 rach_opt_cell_context_t *p_cell_context    
 )
{
	rach_opt_strongest_nbr_cell_node_t *p_nbr_cell_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_nbr_cell_node = rach_opt_get_first_strongest_nbr_cell(
			&p_cell_context->rach_opt_strongest_nbr_cell_list);
	while (SON_PNULL != p_nbr_cell_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Deleted strongest "
				"cell entry with PCI: %u from strongest neighbor cell list for "
				" Cell 0x%x",
				p_nbr_cell_node->data.pci,
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
		list_delete_node(&p_cell_context->rach_opt_strongest_nbr_cell_list,
				&p_nbr_cell_node->sNode);
		p_nbr_cell_node = rach_opt_get_next_strongest_nbr_cell(p_nbr_cell_node);
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_clear_strongest_nbr_cells_list */

/*****************************************************************************
 * Function Name  : rach_opt_clear_cell_contexts_list
 * Inputs         : transaction_id : Transaction Identifier
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Delete all the cell contexts from RACH OPT cell context list
 ****************************************************************************/
son_void_t
rach_opt_clear_cell_contexts_list
(
void /*SPR 17777 +-*/
 )
{
	rach_opt_cell_context_node_t *p_ctxt_node = SON_PNULL;
	son_rrm_rach_config_req_t req = {0};

	SON_UT_TRACE_ENTER();

	p_ctxt_node = rach_opt_get_first_cell_ctxt();
	while (SON_PNULL != p_ctxt_node)
	{
		if (RACH_OPT_CELL_STATE_ENABLING ==
				p_ctxt_node->data.current_cell_fsm_state ||
				RACH_OPT_CELL_STATE_ENABLED ==
				p_ctxt_node->data.current_cell_fsm_state)
		{

			req.transaction_id = son_generate_txn_id();
			SON_MEMCPY(&(req.req.cell_id), &p_ctxt_node->data.cell_id, 
					sizeof(son_intra_rat_global_cell_id_t));
			req.req.bitmask = (L2_REPORT_PERIODICITY_PRESENT |
					UE_REPORT_PERIODICITY_PRESENT);
			req.req.l2_reports_periodicity = 0;
			req.req.ue_reports_periodicity = 0;

			/* Invoke PUP tool to send RACH config request message to stop 
			   receiving RACH related reports from RRM */
			if (RRM_FAILURE == rrm_son_send_rrm_son_rach_config_req(&req.req,
						SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID,
						req.transaction_id, SON_NULL))
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,
						SON_ERROR,
						"Could not send deregister request to RRM "
						"for Cell Id 0x%x",
						son_convert_char_cell_id_to_int_cell_id(
							p_ctxt_node->data.cell_id.cell_identity));
			}
		}

		/* Clear the strongest neighbor cells list for this serving cell */
		rach_opt_clear_strongest_nbr_cells_list(&p_ctxt_node->data);

		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Deleted Cell 0x%x from RACH OPT Global Context",
				son_convert_char_cell_id_to_int_cell_id(
					p_ctxt_node->data.cell_id.cell_identity));
		list_delete_node(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list,
				&p_ctxt_node->sNode);

		p_ctxt_node = rach_opt_get_next_node(p_ctxt_node);
	}
	list_init(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list);

	rach_opt_print_cell_context("rach_opt_fsm_shutdown_req_handler");
	SON_UT_TRACE_EXIT();
} /* rach_opt_clear_cell_contexts_list */

/*****************************************************************************
 * Function Name  : rach_opt_get_total_cfra_preamble
 * Inputs         : prach_config_index      PRACH configuration index
 *                  l2_perf_report_duration duration for which L2 RACH 
 *                                          performance reports are accumulated
 *                  number_of_ra_preambles  Number of non dedicated random 
 *                                          access preambles (Contention based)
 *                  duplex_mode             Duplex mode of the serving cell 
 *                                          (FDD/TDD)
 * Outputs        : None
 * Returns        : total of free RACH CFRA Preambles
 * Description    : This function is used to find the total of free RACH 
 *                  CFRA Preambles in the l2_perf_report_duration (duration for 
 *                  which we have received the accumulated L2 perf reports).
 ****************************************************************************/
son_u64
rach_opt_get_total_cfra_preamble
(
 son_u8                          prach_config_index,
 son_u16                         l2_perf_report_duration,
 son_u8                          number_of_ra_preambles,
 son_duplex_mode_et              duplex_mode
 )
{
	son_u64 total_cfra_preamble = 0;
	son_float density_per_10_ms = 0.0;
	son_u32 total_rach_opportunities = 0;
	SON_UT_TRACE_ENTER();

	if (SON_FDD_MODE == duplex_mode && (PREAMBLE_FORMAT_NA != 
				RA_CONFIG_FDD_DATA_FOR_PREAMBLE[prach_config_index].
				preamble_format))
	{
		density_per_10_ms = RA_CONFIG_FDD_DATA_FOR_PREAMBLE[prach_config_index].
			density_per_10_ms;
	}
	else if (SON_TDD_MODE == duplex_mode && (PREAMBLE_FORMAT_NA !=
				RA_CONFIG_TDD_DATA_FOR_PREAMBLE[prach_config_index].
				preamble_format))
	{
		density_per_10_ms = RA_CONFIG_TDD_DATA_FOR_PREAMBLE[prach_config_index].
			density_per_10_ms;
	}
	SON_LOG(g_rach_opt_gb_ctxt.log_mode,
			p_son_rach_opt_facility_name,
			SON_INFO,
			"PRACH configuration "
			"index %u and mode: %s, density per 10 ms: %f",
			prach_config_index, DUPLEX_MODE_OF_CELL[duplex_mode], density_per_10_ms);

	if (0 != density_per_10_ms)
	{
		/* density_per_10_ms indicates number of RACH opporunities in 10 ms. 
		   We have collected reports for l2_perf_report_duration (in sec) so 
		   using this duration and the density we can find total number of 
		   RACH opportunities. 
		   total_rach_opportunities = density_per_1_ms * time in ms to collect the reports,
		   here density_per_1_ms = density_per_10_ms / 10 */
		total_rach_opportunities = (son_u32)son_roundf((density_per_10_ms / 10) 
				* (l2_perf_report_duration * SON_MILLISECONDS_IN_A_SECOND));

		/* In each opportunity there can be total of 64 preambles(both CBRA and CFRA),
		   so in total cfra preamble will be total CFRA configured multiply by the 
		   number of RACH opportunity where total CFRA configured is 
		   (64 - total CBRA configured) */
		total_cfra_preamble = total_rach_opportunities * 
			(SON_MAX_PRACH_CONFIG_IDX - number_of_ra_preambles); 

		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,
				SON_INFO,
				"Total RACH opportunities "
				"in RACH report periodicity: %u sec is %u and the total "
				"CFRA preamble is %llu",
				l2_perf_report_duration, total_rach_opportunities,
				total_cfra_preamble);


	}
	SON_UT_TRACE_EXIT();
	return total_cfra_preamble; 
}

/*****************************************************************************
 * Function Name  : rach_opt_print_strongest_nbr_cell_list
 * Inputs         : p_list  Pointer to the strongest neighbor cell list 
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function prints the data of strongest neighbor cell 
 *                  list
 ******************************************************************************/
son_void_t
rach_opt_print_strongest_nbr_cell_list
(
 SON_LIST *p_list
 )
{
	rach_opt_strongest_nbr_cell_node_t *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = rach_opt_get_first_strongest_nbr_cell(p_list);
	if (SON_PNULL == p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_DETAILED,
				"Number of strongest "
				"cells reported by UEs = 0.");
		SON_UT_TRACE_EXIT();
		return;
	}

	while (SON_PNULL != p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
				SON_DETAILED,
				"PCI: %u Count: %u",
				p_node->data.pci,
				p_node->data.count);

		p_node = rach_opt_get_next_strongest_nbr_cell(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_print_strongest_nbr_cell_list */

/*****************************************************************************
 * Function Name  : rach_opt_get_cell_mode
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : RACH OPT cell FSM state for the cell
 * Description    : This function find the cell FSM state for the given cell Id
 ****************************************************************************/
rach_opt_cell_fsm_state_et
rach_opt_get_cell_mode
(
 son_intra_rat_global_cell_id_t *p_cell_id
 )
{
	rach_opt_cell_fsm_state_et fsm_state = RACH_OPT_CELL_NUM_OF_STATES;
	rach_opt_cell_context_t *p_rach_opt_cell_ctxt = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_rach_opt_cell_ctxt = rach_opt_get_cell_context_from_global_context(
			p_cell_id);
	if (SON_PNULL != p_rach_opt_cell_ctxt)
	{
		fsm_state = p_rach_opt_cell_ctxt->current_cell_fsm_state;
	}

	SON_UT_TRACE_EXIT();
	return fsm_state;
} /* rach_opt_get_cell_mode */

/*******************************************************************************
 * Function Name  : rach_opt_get_pending_res_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : number of cells available in pending response
 * Description    : This function gets number of cells available in 
 *                   pending response list
 ******************************************************************************/
son_u32
rach_opt_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_rach_opt_pending_response.cell_sts_list);
}

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : rach_opt_get_log_level
 * Inputs         : None
 * Outputs        : None
 * Returns        : Log Level of the RACH-OPT
 * Description    : This function gets the log level
 ******************************************************************************/
son_u8
rach_opt_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_rach_opt_gb_ctxt.log_level;
}

/*****************************************************************************
 * Function Name  : rach_opt_set_log_level
 * Inputs         : log_level - Log Level to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log level
 ******************************************************************************/
son_void_t
rach_opt_set_log_level
(
 son_u8 log_level
 )
{
	SON_UT_TRACE_ENTER();
	g_rach_opt_gb_ctxt.log_level = log_level;
	SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */
