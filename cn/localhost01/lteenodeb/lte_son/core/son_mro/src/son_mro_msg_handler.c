/****************************************************************************
 *
 * ARICENT -
 *
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $$Id: son_mro_msg_handler.c $
 *
 ****************************************************************************
 *
 * File Description: This file contains the manifest structure definition for
 *                   SONMRO module. It also defines the init and message
 *                   handler functions for the SONMRO module which are used
 *                   for SONMRO initialization and message handling
 *                   respectively.
 *
 ****************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Dec , 2012       Ravi/Shilpi Initial         Initial
 *  May, 2014        Shilpi                      Coverity Fixes  
 *  Aug, 2014        Shilpi                      SPR 13251 Fix
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_utils.h>
#include <son_defines.h>
#include <son_mro_fsm.h>
#include <son_mro_ctxt_mgr.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern  pending_res_t g_mro_pending_response;
const son_8 *p_son_mro_facility_name = "SON_MRO";
mro_global_context_t g_mro_gb_ctxt;
extern son_global_config_params_t g_config_params;
extern const QWAIT son_waitstruct;
extern const son_s8 *TTT_NAMES[];
extern const son_s8 *CIO_NAMES[];

static const son_ul32 son_mro_dst_list[] =
{
	SON_MIF_MODULE_ID,
	SON_RRM_MODULE_ID,
	SON_X2_MODULE_ID
};

/*****************************************************************************
 * Function Name  : son_mro_fill_mro_config_param
 * Inputs         : p_mro_config_param: Pointer to son_mro_config_param_t
 * Outputs        : None
 * Returns        : son_return_et
 * Returns        : None
 * Description    : This function initializes the mro global context 
 *                  configuration parameters with the values read from file.
 ******************************************************************************/
static son_void_t
son_mro_fill_mro_config_param
(
 son_mro_config_param_t *p_mro_config_param
 )
{
	SON_UT_TRACE_ENTER();
	/* Update the data with the data read from config file by main() function */
	p_mro_config_param->mro_mode =
		g_config_params.mro_config_params.mro_mode;
	p_mro_config_param->mro_param =
		g_config_params.mro_config_params.mro_param;
	p_mro_config_param->mro_ho_failure_accumulation_duration = 
		g_config_params.mro_config_params.mro_ho_failure_accumulation_duration;
	p_mro_config_param->mro_ho_attempts_threshold =
		g_config_params.mro_config_params.mro_ho_attempts_threshold;
	p_mro_config_param->ttt =
		g_config_params.mro_config_params.ttt;
	p_mro_config_param->delta_cio =
		g_config_params.mro_config_params.delta_cio;
	p_mro_config_param->delta_ttt =
		g_config_params.mro_config_params.delta_ttt;
	p_mro_config_param->min_ttt =
		g_config_params.mro_config_params.min_ttt;
	p_mro_config_param->max_ttt =
		g_config_params.mro_config_params.max_ttt;
	p_mro_config_param->min_cio =
		g_config_params.mro_config_params.min_cio;
	p_mro_config_param->max_cio =
		g_config_params.mro_config_params.max_cio;
	p_mro_config_param->mro_ho_optimization_start_threshold =
		(son_float)g_config_params.mro_config_params.
		mro_ho_optimization_start_threshold / MRO_SCALE_FACTOR;
	p_mro_config_param->mro_ho_optimization_stop_threshold =
		(son_float)g_config_params.mro_config_params.
		mro_ho_optimization_stop_threshold / MRO_SCALE_FACTOR;
	p_mro_config_param->mro_ho_fail_rate_for_optimization =
		(son_float)g_config_params.mro_config_params.
		mro_ho_fail_rate_for_optimization / MRO_SCALE_FACTOR;
	p_mro_config_param->mro_expected_ho_improvement_for_single_step =
		(son_float)g_config_params.mro_config_params.
		mro_expected_ho_improvement_for_single_step / MRO_SCALE_FACTOR;

	SON_UT_TRACE_EXIT();
} /* son_mro_fill_mro_config_param */

/*****************************************************************************
 * Function Name  : mro_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function initializes the mro global context.
 ******************************************************************************/
son_return_et
mro_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_return_et ret_val = SON_SUCCESS;

	SON_UT_TRACE_ENTER();
	g_mro_gb_ctxt.current_fsm_state = MRO_STATE_INIT;
	/* SPR-13251 Fix Starts */
	g_mro_gb_ctxt.log_level = SON_OAM_LOG_LEVEL_BRIEF;
	/* SPR-13251 Fix Ends */
	SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);

	g_mro_gb_ctxt.log_mode = SON_OAM_LOG_ON;
	g_mro_gb_ctxt.mro_pending_res_timer_id = SON_PNULL;

	son_mro_fill_mro_config_param(&g_mro_gb_ctxt.mro_config_param);

	/* Validate the config params */
	/* Coverity CID-55070 Fix Starts */
	if (SON_MODE_DISTRIBUTED != g_mro_gb_ctxt.mro_config_param.mro_mode &&
			SON_MODE_CENTRALIZED != 
			g_mro_gb_ctxt.mro_config_param.mro_mode)
		/* Coverity CID-55070 Fix Ends */
	{
		g_mro_gb_ctxt.mro_config_param.mro_mode = SON_MODE_DISTRIBUTED;
	}

	/* Coverity CID-55070 Fix Starts */
	if (SON_MRO_CIO != g_mro_gb_ctxt.mro_config_param.mro_param &&
			SON_MRO_TTT != g_mro_gb_ctxt.mro_config_param.mro_param &&
			SON_MRO_CIO_TTT != g_mro_gb_ctxt.mro_config_param.mro_param)
		/* Coverity CID-55070 Fix Ends */
	{
		g_mro_gb_ctxt.mro_config_param.mro_param = SON_MRO_CIO;
	}

	if (!(5 <= g_mro_gb_ctxt.mro_config_param.
				mro_ho_failure_accumulation_duration && 
				60 >= g_mro_gb_ctxt.mro_config_param.
				mro_ho_failure_accumulation_duration))
	{
		g_mro_gb_ctxt.mro_config_param.
			mro_ho_failure_accumulation_duration = 
			SON_MRO_DEFAULT_HO_FAILURE_ACCUMULATION_DURATION;
	}

	if (!(1 <= g_mro_gb_ctxt.mro_config_param.delta_cio &&
				5 >= g_mro_gb_ctxt.mro_config_param.delta_cio))
	{
		g_mro_gb_ctxt.mro_config_param.delta_cio = 
			SON_MRO_DEFAULT_DELTA_CIO;
	}

	if (!(1 <= g_mro_gb_ctxt.mro_config_param.
				mro_ho_attempts_threshold))
	{
		g_mro_gb_ctxt.mro_config_param.
			mro_ho_attempts_threshold =
			SON_MRO_DEFAULT_HO_ATTEMPTS_THRESHOLD;
	}

	if (!(0.01 <= g_mro_gb_ctxt.mro_config_param.
				mro_ho_optimization_start_threshold &&
				10 >= g_mro_gb_ctxt.mro_config_param.
				mro_ho_optimization_start_threshold))
	{
		g_mro_gb_ctxt.mro_config_param.
			mro_ho_optimization_start_threshold =
			SON_MRO_DEFAULT_HO_OPTIMIZATION_START_THRESHOLD;
	}

	if ((!(0.01 <= g_mro_gb_ctxt.mro_config_param.
					mro_ho_optimization_stop_threshold &&
					10 >= g_mro_gb_ctxt.mro_config_param.
					mro_ho_optimization_stop_threshold)) &&
			(g_mro_gb_ctxt.mro_config_param.
			 mro_ho_optimization_stop_threshold >= 
			 g_mro_gb_ctxt.mro_config_param.
			 mro_ho_optimization_start_threshold))
	{
		g_mro_gb_ctxt.mro_config_param.
			mro_ho_optimization_stop_threshold =
			SON_MRO_DEFAULT_HO_OPTIMIZATION_STOP_THRESHOLD;
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
				SON_DETAILED,
				"mro_init_global_context: MRO HO "
				"Optimization Stop threshold %f read from config file is less"
				" than or equal to start threshold %f so stop threshold is "
				"set to its default value %f",
				g_mro_gb_ctxt.mro_config_param.
				mro_ho_optimization_stop_threshold,
				g_mro_gb_ctxt.mro_config_param.
				mro_ho_optimization_start_threshold,
				SON_MRO_DEFAULT_HO_OPTIMIZATION_STOP_THRESHOLD);
	}

	if (!(0.01 <= g_mro_gb_ctxt.mro_config_param.
				mro_ho_fail_rate_for_optimization &&
				10 >= g_mro_gb_ctxt.mro_config_param.
				mro_ho_fail_rate_for_optimization))
	{
		g_mro_gb_ctxt.mro_config_param.mro_ho_fail_rate_for_optimization =
			SON_MRO_DEFAULT_HO_FAIL_RATE_FOR_OPTIMIZATION;
	}

	if (!(SON_TTT_MS_5120 >= g_mro_gb_ctxt.mro_config_param.ttt))
	{
		g_mro_gb_ctxt.mro_config_param.ttt = SON_TTT_MS_512;
	}

	if (!(1 <= g_mro_gb_ctxt.mro_config_param.delta_ttt &&
				5 >= g_mro_gb_ctxt.mro_config_param.delta_ttt))
	{
		g_mro_gb_ctxt.mro_config_param.delta_ttt = 
			SON_MRO_DEFAULT_DELTA_TTT;
	}

	if (!(0.01 <= g_mro_gb_ctxt.mro_config_param.
				mro_expected_ho_improvement_for_single_step &&
				10 >= g_mro_gb_ctxt.mro_config_param.
				mro_expected_ho_improvement_for_single_step))
	{
		g_mro_gb_ctxt.mro_config_param.
			mro_expected_ho_improvement_for_single_step =
			SON_MRO_DEFAULT_EXPECTED_HO_IMPROVEMENT_FOR_SINGLE_STEP;
	}

	if (!(SON_TTT_MS_512 >= g_mro_gb_ctxt.mro_config_param.min_ttt))
	{
		SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name, SON_WARNING,
				"Failed to set min_ttt value to "
				"%s as default value is SON_TTT_MS_512",
				TTT_NAMES[g_mro_gb_ctxt.mro_config_param.min_ttt]);
		g_mro_gb_ctxt.mro_config_param.min_ttt = SON_TTT_MS_0;
	}

	if (!(SON_TTT_MS_512 <= g_mro_gb_ctxt.mro_config_param.max_ttt &&
				SON_TTT_MS_5120 >= g_mro_gb_ctxt.mro_config_param.max_ttt))
	{
		SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name, SON_WARNING,
				"Failed to set max_ttt value to "
				"%s as default value is SON_TTT_MS_512",
				TTT_NAMES[g_mro_gb_ctxt.mro_config_param.max_ttt]);

		g_mro_gb_ctxt.mro_config_param.max_ttt = SON_TTT_MS_5120;
	}

	if (!(SON_Q_OFFSET_RANGE_DB0 >= 
				g_mro_gb_ctxt.mro_config_param.min_cio))
	{
		SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name, SON_WARNING,
				"Failed to set min_cio value to "
				"%s as default value is SON_Q_OFFSET_RANGE_DB0",
				CIO_NAMES[g_mro_gb_ctxt.mro_config_param.min_cio]);

		g_mro_gb_ctxt.mro_config_param.min_cio = SON_Q_OFFSET_RANGE_DB_24;
	}

	if (!(SON_Q_OFFSET_RANGE_DB0 <= g_mro_gb_ctxt.mro_config_param.max_cio
				&& SON_Q_OFFSET_RANGE_DB24 >= 
				g_mro_gb_ctxt.mro_config_param.max_cio))
	{
		SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name, SON_WARNING,
				"Failed to set max_cio value to "
				"%s as default value is SON_Q_OFFSET_RANGE_DB0",
				CIO_NAMES[g_mro_gb_ctxt.mro_config_param.max_cio]);
		g_mro_gb_ctxt.mro_config_param.max_cio = SON_Q_OFFSET_RANGE_DB24;
	}

	g_mro_gb_ctxt.mro_global_mode = SON_DISABLED;
	/*SPR-11554-Start*/
	g_mro_gb_ctxt.enb_type = SON_ENB_TYPE_UNDEFINED;
	/*SPR-11554-End*/
	list_init(&g_mro_gb_ctxt.mro_cell_ctxt_list);
	mro_print_global_context("mro init global context");
	SON_UT_TRACE_EXIT();
	return ret_val;
} /* mro_init_global_context */

/*****************************************************************************
 * Function Name  : son_mro_init
 * Inputs         : p_son_mro_init_data - Pointer to SONMRO stack
 *                  initialization data (Unused, but required as part of CSPL
 *                  signature)
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SONMRO module.
 *                  It sets the SONMRO FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_mro_init
(
 /* Pointer to SONMRO stack initialization data. Unused */
 son_void_t *p_son_mro_init_data
 )
{
	SON_UT_TRACE_ENTER();

	if (SON_SUCCESS == mro_init_global_context())
	{
		/* Init advance logger with default log file */
		SON_INIT_LOG(SON_PNULL);

		/* Send SMIF_INIT_IND to SONMgmtIfh */
		son_create_send_buffer(SON_PNULL, SON_MRO_MODULE_ID,
				SON_MIF_MODULE_ID, SMIF_INIT_IND,
				0);
	}
	else
	{
        /*SPR 17777 +-*/
		SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name, SON_ERROR,
				"Failed to initialize MRO module global context. "
				"Exiting!!! p_son_mro_init_data=%p",p_son_mro_init_data);
        /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();

	/* Returning pointer to the global context of SONMRO-unused
	 * in current approach */
	return SON_PNULL;
} /* son_mro_init */

/*****************************************************************************
 * Function Name  : son_mro_process_msg
 * Inputs         : p_buf - Pointer to the message received
 *                  p_cxt - Pointer to the SONMRO global context data (Unused, 
 *                  but required as part of CSPL signature)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONMRO module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_mro_process_msg
(
 son_void_t *p_buf,
 son_void_t *p_cxt
 )
{
	son_buf_retain_status_et buf_retain_status =  RELEASE_SON_BUFFER;

	SON_UT_TRACE_ENTER();

	/* Precondition check */
	if (SON_PNULL == p_buf)
	{
        /*SPR 17777 +-*/
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_ERROR,
				"API pointer received is NULL");
        LTE_GCC_UNUSED_PARAM(p_cxt);
        /*SPR 17777 +-*/
	}
	else
	{
		buf_retain_status = son_mro_process_event(p_buf);
	}

	SON_UT_TRACE_EXIT();
	return buf_retain_status;
} /* son_mro_process_msg */

/*****************************************************************************
 * Function Name  : mro_cell_info_ind_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This the funciton to clean up pending response strucutre 
 *                  and update cell state for the cells on expirty of expected 
 *                  registeration response timer. This is used when the 
 *                  response awaited were trigerred due to cell info indication
 *                  requrest received from OAM.
 ****************************************************************************/
static son_void_t 
mro_cell_info_ind_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_LIST_NODE *p_node = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;
	mro_cell_context_node_t *p_cell_data = SON_PNULL;
	SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	mro_print_pending_res("mro_cell_info_ind_timer_handler");
	p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
				p_node);
		/* Find the pointer to MRO cell context for this node */
		p_cell_list_node = list_find_node(&g_mro_gb_ctxt.mro_cell_ctxt_list,
				&p_list_node->cell_status.cgi, son_key_of_mro,
				son_compare_cell_id);

		if (SON_PNULL != p_cell_list_node)
		{
			p_cell_data = YMEMBEROF(mro_cell_context_node_t, sNode, 
					p_cell_list_node);
			if (SON_PNULL != p_cell_data)
			{    
				/* Change cell FSM state to MRO_CELL_STATE_DISABLED */
				SONMRO_CELL_FSM_SET_STATE(
						(&p_cell_data->data),
						p_cell_data->data.previous_cell_fsm_state,
						p_cell_data->data.current_cell_fsm_state,
						MRO_CELL_STATE_DISABLED);
			}
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_WARNING,
					"mro_cell_info_ind_timer_handler: Could not find MRO"
					" cell context for the cell [Cell Id: 0x%x] present in"
					" pending response list",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_status.cgi.
						cell_identity));
		}

		/* Send feature state change indication to SMIF */
		mro_send_feature_state_change_ind(&p_list_node->cell_status.cgi,
				SON_STATE_DISABLED, SON_RRM_REGISTRATION_UNSUCCESSFUL);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_mro_pending_response.cell_sts_list);
	}

	SON_UT_TRACE_EXIT();
} /* mro_cell_info_ind_timer_handler */

/*****************************************************************************
 * Function Name  : mro_pending_res_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function cleans up Pending Response Structure and sends
 *                  pending response to OAM on pending response timer expiry.
 ****************************************************************************/
son_void_t
mro_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;

	SON_UT_TRACE_ENTER();

	if (MIN_PENDING_RES_COUNT < mro_get_pending_res_cell_count())
	{
		api = mro_get_pending_res_api_id();
		if (SONMRO_ENABLE_RES == api)
		{
			son_mro_enable_res_t res;
			son_size_t enable_res_len = sizeof(son_mro_enable_res_t);
			SON_MEMSET(&res, 0, enable_res_len);
			res.transaction_id = mro_get_pending_res_transaction_id();
			res.enable_res.error_code = SON_NO_ERROR;

			/* Remove Successful cells from the MRO_Pending_Response_LIST and 
			 *  create cell array with entry for each cell for which MRO could
			 *  not be enabled. */
			mro_construct_cell_arr_from_pending_res_list(
					&res.enable_res.cell_status_list_size, 
					res.enable_res.cell_status_list, MRO_CELL_STATE_DISABLED);
			if (0 != res.enable_res.cell_status_list_size)
			{
				res.enable_res.result = SON_FAILURE;
				son_create_send_buffer((son_u8 *)&res,
						SON_MRO_MODULE_ID,
						SON_MIF_MODULE_ID,
						SONMRO_ENABLE_RES,
						enable_res_len);
			}
			else
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_WARNING,
						"mro_pending_res_timer_handler: The pending response "
						"list is either empty or all pending registration "
						"response nodes are present with error code as "
						"SON_NO_ERROR on pending response timer expiry");
			}
		}
		else if (SONMRO_DISABLE_RES == api)
		{
			son_mro_disable_res_t res;
			son_size_t disable_res_len = sizeof(son_mro_disable_res_t);
			SON_MEMSET(&res, 0, disable_res_len);
			res.transaction_id = mro_get_pending_res_transaction_id();
			res.disable_res.error_code = SON_NO_ERROR;

			/* Remove Successful cells from the MRO_Pending_Response_LIST and 
			 *  create cell array with entry for each cell for which MRO could
			 *  not be disabled. */
			mro_construct_cell_arr_from_pending_res_list(
					&res.disable_res.cell_status_list_size,
					res.disable_res.cell_status_list, MRO_CELL_STATE_ENABLED);
			if (0 != res.disable_res.cell_status_list_size)
			{
				res.disable_res.result = SON_FAILURE;
				son_create_send_buffer((son_u8 *)&res,
						SON_MRO_MODULE_ID,
						SON_MIF_MODULE_ID,
						SONMRO_DISABLE_RES,
						disable_res_len);
			}
			else
			{
				SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
						SON_WARNING,
						"mro_pending_res_timer_handler: The pending response "
						"list is either empty or all pending deregistration "
						"response nodes are present with error code as "
						"SON_NO_ERROR on pending response timer expiry");
			}
		}
		else if (SMIF_CELL_INFO_IND == api)
		{
			mro_cell_info_ind_timer_handler();
		}
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_WARNING,
					"mro_pending_res_timer_handler: Invalid API Id %u found in "
					"pending response list", api);
		}

		/* Set MRO global FSM state to ACTIVE */
		SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
				MRO_STATE_ACTIVE);

		/* Clear/Reset Pending Response ListMRO_Pending_Response_LIST. */
		mro_reset_pending_resp_list();
		g_mro_gb_ctxt.mro_pending_res_timer_id = SON_PNULL;
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_WARNING,
				"mro_pending_res_timer_handler: MRO pending response timer"
				" expired with no data in pending reponse list.");
	}

	SON_UT_TRACE_EXIT();
} /* mro_pending_res_timer_handler */

/*****************************************************************************
 * Function Name  : mro_ho_failure_accumulatioin_duraion_handler
 * Inputs         : p_cell_id    Pointer to Cell ID
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles the expiry of HO failure analysis 
 *                  timer for a cell. 
 ****************************************************************************/
static son_void_t
mro_ho_failure_accumulatioin_duraion_handler
(
 son_intra_rat_global_cell_id_t *p_cell_id
 )
{
	mro_cell_context_t *p_mro_cell_ctxt = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell_id)
	{ 
		p_mro_cell_ctxt = mro_get_cell_context_from_global_context(
				p_cell_id);
		if (SON_PNULL == p_mro_cell_ctxt)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"mro_ho_failure_accumulatioin_duraion_handler: cell [cell "
					"id: 0x%x] not found in MRO cell"
					" context list, so can't perform ho analysis",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity));
		}
		else if (MRO_CELL_STATE_ENABLED == 
				p_mro_cell_ctxt->current_cell_fsm_state)
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_BRIEF,
					"HO Failure "
					"accumulation duration timer expired for cell "
					"[cell id: 0x%x] in state %s",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->
						cell_identity),
					SON_MRO_CELL_FSM_STATES_NAMES[p_mro_cell_ctxt->
					current_cell_fsm_state]);
			/* PR 860 Fix Start */
			if (SON_MODE_DISTRIBUTED == 
					p_mro_cell_ctxt->mro_config_param.mro_mode)
			{    
				mro_execute_optimization_algo(p_mro_cell_ctxt);
			}
			else if (SON_MODE_CENTRALIZED == 
					p_mro_cell_ctxt->mro_config_param.mro_mode)
				/* PR 860 Fix End */
			{
				mro_build_and_send_mro_ho_stats(p_mro_cell_ctxt);
			}
		}  
		else
		{
			SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
					SON_ERROR,
					"mro_ho_failure_accumulatioin_duraion_handler: HO Failure"
					" accumulation duration timer expired for cell "
					"[cell id: 0x%x] in state %s, so can't perform ho analysis",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_id->cell_identity),
					SON_MRO_CELL_FSM_STATES_NAMES[p_mro_cell_ctxt->
					current_cell_fsm_state]);
		} 
	}
	else
	{
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_ERROR,
				"mro_ho_failure_accumulatioin_duraion_handler: Pointer to Cell"
				"Id is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* mro_ho_failure_accumulatioin_duraion_handler */

/*****************************************************************************
 * Function Name  : son_mro_process_timer
 * Inputs         : timer_id   
 *                  p_timer_data - Pointer to the timer data ctxt managed by
 *                                 CSPL
 *                  p_cxt - Pointer to the SONMRO global context data(Unused,
 *                  but required as part of CSPL signature)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the MRO timer handler function which checks the
 *                  expiry of various timers running and takes appropriate
 *                  actions.
 ****************************************************************************/
son_void_t
son_mro_process_timer
(
 son_timer_t timer_id,
 son_void_t  *p_timer_data,
 son_void_t  *p_cxt_data
 )
{
	SON_UT_TRACE_ENTER();

	/* Check if MRO_PENDING_RESPONSE_TIMER timer expired */
	if (g_mro_gb_ctxt.mro_pending_res_timer_id == timer_id)
	{
        /*SPR 17777 +-*/
		SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
				SON_DETAILED,
				"son_mro_process_timer: MRO pending response timer"
				" expired; p_cxt_data=%p",p_cxt_data);
        /*SPR 17777 +-*/
		mro_pending_res_timer_handler();
	}
	/* HO_FAILURE_ACCUMULATION_DURATION timer expired for any of the cell */
	else
	{
		mro_ho_failure_accumulatioin_duraion_handler(
				(son_intra_rat_global_cell_id_t *)p_timer_data); 
	}

	SON_UT_TRACE_EXIT();
} /* son_mro_process_timer */

/****************************************************************************
 * Initialization of QManifest Structure for SON MRO Module
 ****************************************************************************/
const QMANIFEST son_manifest_mro =
{
	"SON_MRO_MODULE",
	SON_MRO_MODULE_ID,
	{0, son_mro_init},             /*{ early initialization, late initialization }*/
	{son_mro_process_msg, son_mro_process_timer},  /* { message processor, timer handler }*/
	{0, 0, 0},                            /* { pack, unpack, alloc}*/
	son_mro_dst_list               /* destinations*/
};
