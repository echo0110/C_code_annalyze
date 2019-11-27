/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rach_opt_cell_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains RACH OPT cell FSM functions
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aug, 2013       SON Team    Initial         Initial
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_rach_opt_cell_fsm.h>
#include <son_rach_opt_ctxt_mgr.h>
#include <son_anr_ctxt_mgr.h>
#include <son_rach_opt_intf.h>
#include <son_anr_table.h>
#include <rrm_son_composer.h>
#include <son_anr_cell_fsm.h>
#include <x2ap_api.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern rach_opt_global_context_t g_rach_opt_gb_ctxt;
extern const son_8 *p_son_rach_opt_facility_name;
extern const son_s8 *SON_RACH_OPT_CELL_FSM_STATES_NAMES[];
extern const son_s8 *SON_RACH_OPT_CELL_FSM_EVENT_NAMES[];
extern anr_global_t  g_anr_context;
extern const son_s8 *RA_GRP_A_PREAMBLE_NAMES[];
extern const son_s8 *RA_PREAMBLE_NAMES[];

/* SPR-18735 START */
#define NZC_VAL_0_3 839
#define NZC_VAL_4 139
#define HIGH_SPEED_FLAG_UNRESTRICTED 0
#define HIGH_SPEED_FLAG_RESTRICTED 1

#define SON_NCS_NOT_APPLICABLE 0xFFFF
#define SON_MAX_ZCZC_VALUE 16
const rach_opt_ncs_mapping_format_0_3 NCS_FOR_PREAMBLE_GENERATION_FORMAT_0_3[SON_MAX_ZCZC_VALUE] =
{
    {0, 0, 15},
    {1, 13, 18},
    {2, 15, 22},
    {3, 18, 26},
    {4, 22, 32},
    {5, 26, 38},
    {6, 32, 46},
    {7, 38, 55},
    {8, 46, 68},
    {9, 59, 82},
    {10, 76, 100},
    {11, 93, 128},
    {12, 119, 158},
    {13, 167, 202},
    {14, 279, 237},
    {15, 419, SON_NCS_NOT_APPLICABLE}
};

const rach_opt_ncs_mapping_format_4 NCS_FOR_PREAMBLE_GENERATION_FORMAT_4[SON_MAX_ZCZC_VALUE] =
{
    {0, 2},
    {1, 4},
    {2, 6},
    {3, 8},
    {4, 10},
    {5, 12},
    {6, 15},
    {7, SON_NCS_NOT_APPLICABLE},
    {8, SON_NCS_NOT_APPLICABLE},
    {9, SON_NCS_NOT_APPLICABLE},
    {10, SON_NCS_NOT_APPLICABLE},
    {11, SON_NCS_NOT_APPLICABLE},
    {12, SON_NCS_NOT_APPLICABLE},
    {13, SON_NCS_NOT_APPLICABLE},
    {14, SON_NCS_NOT_APPLICABLE},
    {15, SON_NCS_NOT_APPLICABLE}
};

/* SPR-18735 END */

/* SPR-19168 START */
/* code removed */
/* SPR-19168 START */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_enable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for SONRACH_OPT_ENABLE_REQ
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_enable_req_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_enable_req_t *p_req = 
		(son_rach_opt_enable_req_t *)p_rach_opt_msg;
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
	son_feature_state_et state = SON_STATE_UNDEFINED;
	son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;
	son_rrm_rach_config_req_t req = {0};
    /* SPR 20653 Fix Start */
	anr_cell_fsm_state_et anr_cell_fsm_state = ANR_CELL_STATE_MAX;
    /* SPR 20653 Fix End */
	SON_UT_TRACE_ENTER();

	/* Check from LTE SON/ANR context whether the Cell Id is present 
	 * in ANR module cell context and ANR mode is ENABLED or not
	 * for the cell. */
	p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
			&p_cell_ctxt->cell_id);

	if (SON_PNULL != p_anr_cell_ctxt)
	{
		anr_cell_fsm_state = anr_get_anr_mode_for_cell(p_anr_cell_ctxt);
		if ((ANR_CELL_STATE_ENABLED == anr_cell_fsm_state) ||
				(ANR_CELL_STATE_ENABLING == anr_cell_fsm_state))
		{
			req.transaction_id = p_req->transaction_id;
			SON_MEMCPY(&(req.req.cell_id), &p_cell_ctxt->cell_id, sizeof(
						son_intra_rat_global_cell_id_t));
			req.req.bitmask = (L2_REPORT_PERIODICITY_PRESENT | 
					UE_REPORT_PERIODICITY_PRESENT | 
					NUM_STRONGEST_CELLS_PRESENT | RACH_SAMPLE_COUNT_PRESENT);
			req.req.l2_reports_periodicity = p_cell_ctxt->oam_config_params.
				rachopt_report_periodicity;
			req.req.ue_reports_periodicity = p_cell_ctxt->oam_config_params.
				rachopt_report_periodicity;
			req.req.rach_sample_count = p_cell_ctxt->oam_config_params.
				rachopt_min_ue_rach_report_count;
			req.req.num_of_strongest_cells = p_cell_ctxt->
				read_only_config_params.rachopt_strong_cell_count;
			/* Invoke PUP tool to send RACH config request message to RRM */
			if (RRM_SUCCESS == rrm_son_send_rrm_son_rach_config_req(&req.req,
						SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID,
						p_req->transaction_id, SON_NULL))
			{
				p_cell_ctxt->rrm_request_txid = p_req->transaction_id;
				/* Change Cell FSM state to RACH_OPT_CELL_STATE_ENABLING */
				SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						RACH_OPT_CELL_STATE_ENABLING);
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_BRIEF,
						"RRM_SON_RACH_CONFIG_REQ send from RACH OPT to"
						" RRM for Cell 0x%x",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
			}
			else
			{
				error_code = SON_ERR_COMPOSER_ERROR; 
				cause = SON_INTERNAL_FAILURE;
				state = SON_STATE_DISABLED;
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_ERROR,
						"Failed to "
						"send RRM_SON_RACH_CONFIG_REQ from RACH OPT to RRM for "
                    /*SPR 17777 +-*/
						"Cell 0x%x",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
				LTE_GCC_UNUSED_PARAM(evnt_id);
                    /*SPR 17777 +-*/
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Not sending "
					"RRM_SON_RACH_CONFIG_REQ from RACH OPT to RRM for "
					"Cell Id 0x%x since the cell in disabled at ANR",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
			error_code = SON_ERR_ANR_MODE_IS_DISABLED;    
			cause = SON_ANR_MODE_DISABLED;
			state = SON_STATE_DISABLED;
		}
	}
	else 
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Not sending "
				"RRM_SON_RACH_CONFIG_REQ from RACH OPT to RRM for "
				"Cell 0x%x since the cell in unconfigured at ANR",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		error_code = SON_ERR_CELL_UNCONFIGURED_AT_ANR;      
		cause = SON_ANR_CELL_UNCONFIGURED;
		state = SON_STATE_DISABLED;
	}

	if (SON_NO_ERROR != error_code)
	{
		cell_status_list_data_t *p_list_node = SON_PNULL;
		/* Search for the cell Id in pending response structure. */
		p_list_node = rach_opt_find_cell_in_pending_res(
				&(p_cell_ctxt->cell_id));

		/* Presence indicates that this event is triggered due to Global 
		 * Enable Request or Cell Info Indication and no immediate Enable
		 * Response must be sent from cell FSM */
		if (SON_PNULL != p_list_node)
		{
			/* Update Cell in RACH OPT Pending Response List with error 
			 *  code determined above */
			p_list_node->cell_status.error_code = error_code;
			/* Decrement expected_response_count by 1 */
			rach_opt_decrement_pending_res_expected_count();
			if (SMIF_CELL_INFO_IND == p_cspl_hdr->api)
			{
				rach_opt_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
						state, cause);
			}
		}
		/* Cell specific Enable Request is being processed */
		else
		{
			son_rach_opt_enable_res_t enable_res;
			son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);
			/* Send SONRACH_OPT_ENABLE_RES to SONMgmtIfH with faliure */
			SON_MEMSET(&enable_res, 0, enable_res_len);
			enable_res.transaction_id = p_req->transaction_id;
			enable_res.enable_res.result = SON_FAILURE;
			enable_res.enable_res.error_code = SON_NO_ERROR;
			enable_res.enable_res.cell_status_list_size = 1;
			SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			enable_res.enable_res.cell_status_list[0].error_code = error_code;
			son_create_send_buffer((son_u8 *)&enable_res,
					SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
					SONRACH_OPT_ENABLE_RES, enable_res_len);
		}
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_enable_req_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_disable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for SONRACH_OPT_DISABLE_REQ
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_disable_req_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_disable_req_t *p_req = 
		(son_rach_opt_disable_req_t *)p_rach_opt_msg;
	son_rrm_rach_config_req_t req = {0};
	cell_status_list_data_t *p_list_node = SON_PNULL;
	SON_UT_TRACE_ENTER();

	req.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&(req.req.cell_id), &p_cell_ctxt->cell_id, sizeof(
				son_intra_rat_global_cell_id_t));
	req.req.bitmask = (L2_REPORT_PERIODICITY_PRESENT |
			UE_REPORT_PERIODICITY_PRESENT);
	req.req.l2_reports_periodicity = 0;
	req.req.ue_reports_periodicity = 0;

	/* Invoke PUP tool to send RACH config request message to stop receiving
	   RACH related reports from RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_rach_config_req(&req.req,
				SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID,
				p_req->transaction_id, SON_NULL))
	{
		p_cell_ctxt->rrm_request_txid = p_req->transaction_id;
		/* Change Cell FSM state to RACH_OPT_CELL_STATE_DISABLING */
		p_cell_ctxt->dereg_invoking_api = SON_OAM_RACH_OPT_DISABLE_REQ;
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_DISABLING);
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_BRIEF,
				"Message "
				"RRM_SON_RACH_CONFIG_REQ send from RACH OPT to stop receiving"
				" RACH related reports from RRM for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Failed to send "
				"RRM_SON_RACH_CONFIG_REQ from RACH OPT to stop receiving "
                    /*SPR 17777 +-*/
				" RACH related reports from RRM for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
		/* Search for the cell Id in pending response structure. */
		p_list_node = rach_opt_find_cell_in_pending_res(
				&(p_cell_ctxt->cell_id));

		/* Presence indicates that this event is triggered due to Global 
		   Disable Request and no immediate disable Response must be sent 
		   from cell FSM */
		if (SON_PNULL != p_list_node)
		{
			/* Update Cell in RACH OPT Pending Response List with error 
			 *  code determined above */
			p_list_node->cell_status.error_code = SON_ERR_COMPOSER_ERROR;
			/* Decrement expected_response_count by 1 */
			rach_opt_decrement_pending_res_expected_count();
		}
		/* Cell specific Disable Request is being processed */
		else
		{
			/* Send failure to SONMgmtIfH */
			son_rach_opt_disable_res_t disable_res;
			son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);

			SON_MEMSET(&disable_res, 0, disable_res_len);
			disable_res.transaction_id = p_req->transaction_id;
			disable_res.disable_res.result = SON_FAILURE;
			disable_res.disable_res.error_code = SON_NO_ERROR;
			disable_res.disable_res.cell_status_list_size = 1;
			SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			disable_res.disable_res.cell_status_list[0].error_code =
				SON_ERR_COMPOSER_ERROR;

			son_create_send_buffer((son_u8 *)&disable_res,
					SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
					SONRACH_OPT_DISABLE_RES, disable_res_len);
		}
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_disable_req_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_modify_config_params
 * Inputs         : p_cell_ctxt - Pointer to the cell context
 *                  p_rach_config_params - Pointer to the new RACH data
 * Outputs        : p_error_code    - Error code value in case of error
 * Returns        : son_void_t
 * Description    : This is the helper function to modify RACH config params.
 ****************************************************************************/
static son_void_t
sonrach_opt_modify_config_params
(
 rach_opt_cell_context_t         *p_cell_ctxt,
 son_rach_opt_oam_config_param_t *p_rach_config_params,
 son_error_et                    *p_error_code
 )
{
	son_u32 bitmask = (SON_RACHOPT_START_THRESHOLD_PRESENT |
			SON_RACHOPT_STOP_THRESHOLD_PRESENT);
	SON_UT_TRACE_ENTER();

	/* Check if both start and stop threshold are to be modified, If yes then 
	   validate both of them. */
	if (bitmask == (bitmask & p_rach_config_params->bitmask))
	{
		if (p_rach_config_params->rachopt_start_threshold >
				p_rach_config_params->rachopt_stop_threshold)
		{
			p_cell_ctxt->oam_config_params.rachopt_start_threshold =
				(son_float)p_rach_config_params->rachopt_start_threshold / 100;
			p_cell_ctxt->oam_config_params.rachopt_stop_threshold =
				(son_float)p_rach_config_params->rachopt_stop_threshold / 100;
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
					SON_INFO,
					"Stop threshold %f%% received in modify request is >= "
					" start threshold %f%% received in modify request so "
					"start and stop thresholds are not modified for serving "
					"Cell 0x%x",
					(son_float)p_rach_config_params->
					rachopt_stop_threshold / 100,
					(son_float)p_rach_config_params->
					rachopt_start_threshold / 100,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			p_cell_ctxt->modify_res_bitmask |= 
				(SON_RACHOPT_START_THRESHOLD_PRESENT | 
				 SON_RACHOPT_STOP_THRESHOLD_PRESENT); 
		}
	}
	/* If only start threshold is to modified */
	else if (SON_RACHOPT_START_THRESHOLD_PRESENT &
			p_rach_config_params->bitmask)
	{
		if ((son_float)p_rach_config_params->rachopt_start_threshold / 100 
				> p_cell_ctxt->oam_config_params.
				rachopt_stop_threshold)
		{
			p_cell_ctxt->oam_config_params.rachopt_start_threshold =
				(son_float)p_rach_config_params->rachopt_start_threshold / 100;
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
					SON_INFO,
					"Start threshold %f%% received in modify "
					"request is <= current stop threshold %f%% "
					"stored in cell context so start threshold is not "
					"modified for serving Cell Id: 0x%x",
					(son_float)p_rach_config_params->
					rachopt_start_threshold / 100,
					p_cell_ctxt->oam_config_params.
					rachopt_stop_threshold,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
			p_cell_ctxt->modify_res_bitmask |= 
				SON_RACHOPT_START_THRESHOLD_PRESENT; 
			*p_error_code = SON_ERR_INVALID_PARAMS;
		}
	}
	/* If only stop threshold is to modified */
	else if (SON_RACHOPT_STOP_THRESHOLD_PRESENT & 
			p_rach_config_params->bitmask)
	{
		if ((son_float)p_rach_config_params->rachopt_stop_threshold / 100 
				< p_cell_ctxt->oam_config_params.
				rachopt_start_threshold)
		{
			p_cell_ctxt->oam_config_params.rachopt_stop_threshold =
				(son_float)p_rach_config_params->rachopt_stop_threshold / 100;
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
					SON_INFO,
					"Stop threshold %f%% received in modify "
					"request is >= current start threshold %f%% "
					"stored in cell context so stop threshold"
					"is not modified for serving Cell Id: 0x%x",
					(son_float)p_rach_config_params->
					rachopt_stop_threshold / 100,
					p_cell_ctxt->oam_config_params.
					rachopt_start_threshold,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
			p_cell_ctxt->modify_res_bitmask |= 
				SON_RACHOPT_STOP_THRESHOLD_PRESENT; 
			*p_error_code = SON_ERR_INVALID_PARAMS;
		}
	}

	if (SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT & 
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.rachopt_ue_rach_attempts_threshold = 
			p_rach_config_params->rachopt_ue_rach_attempts_threshold;
	}

	if (SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT &
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.rachopt_degrade_stop_offset =
			(son_float)p_rach_config_params->
			rachopt_degrade_stop_offset / 100;
	}

	if (SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT &
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.
			rachopt_max_iterations_to_reach_stop_threshold =
			p_rach_config_params->
			rachopt_max_iterations_to_reach_stop_threshold;
	}

	if (SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT &
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.rachopt_contention_threshold =
			p_rach_config_params->
			rachopt_contention_threshold;
	}

	if (SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT & 
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.rachopt_power_rampup_threshold = 
			p_rach_config_params->
			rachopt_power_rampup_threshold;
	}

	if (SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT &
			p_rach_config_params->bitmask)
	{
		p_cell_ctxt->oam_config_params.rachopt_min_cfra_preambles = 
			p_rach_config_params->rachopt_min_cfra_preambles;
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_modify_config_req_in_enabled_state_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for
 *                  SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ message received from 
 *                  SONMgmtIfH in RACH_OPT_CELL_STATE_ENABLED state.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_modify_config_req_in_enabled_state_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_modify_config_params_req_t *p_req = 
		(son_rach_opt_modify_config_params_req_t *)p_rach_opt_msg;
	son_rach_opt_modify_config_params_res_t modify_res;
	son_size_t modify_res_len = sizeof(son_rach_opt_modify_config_params_res_t);
	son_return_et result = SON_SUCCESS;
	son_error_et error_code = SON_NO_ERROR;
	son_rrm_rach_config_req_t req = {0};
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&modify_res, 0, modify_res_len);
	modify_res.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&modify_res.modify_res.srv_cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/*SPR-11745-Start*/

	if(SON_NULL == p_req->modify_req.rach_config_params.bitmask)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_DETAILED,
                    /*SPR 17777 +-*/
				"Message coming from OAM does not have any"
                "parameters to configure");
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
	}

	/*SPR-11745-End*/

	sonrach_opt_modify_config_params(p_cell_ctxt, 
			&p_req->modify_req.rach_config_params, &error_code);

	if (SON_RACHOPT_REPORT_PERIODICITY_PRESENT &
			p_req->modify_req.rach_config_params.bitmask)
	{
		if (p_req->modify_req.rach_config_params.
				rachopt_report_periodicity
				!= p_cell_ctxt->oam_config_params.
				rachopt_report_periodicity)
		{
			/* Store the new value in cell context temporarily. The value will
			   be used after receiving success RACH config response from RRM */ 
			p_cell_ctxt->new_rachopt_report_periodicity = 
				p_req->modify_req.rach_config_params.
				rachopt_report_periodicity;
			/* Set bitmask for new periodicity to RRM */
			req.req.bitmask |= (L2_REPORT_PERIODICITY_PRESENT |
					UE_REPORT_PERIODICITY_PRESENT);
			req.req.l2_reports_periodicity = p_cell_ctxt->new_rachopt_report_periodicity;
			req.req.ue_reports_periodicity = p_cell_ctxt->new_rachopt_report_periodicity;
		}
	}

	if (SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT &
			p_req->modify_req.rach_config_params.bitmask)
	{
		if (p_req->modify_req.rach_config_params.
				rachopt_min_ue_rach_report_count != 
				p_cell_ctxt->oam_config_params.
				rachopt_min_ue_rach_report_count)
		{
			/* Store the new value in cell context temporarily. The value will
			   be used after receiving success RACH config response from RRM */ 
			p_cell_ctxt->new_rachopt_min_ue_rach_report_count = 
				p_req->modify_req.rach_config_params.
				rachopt_min_ue_rach_report_count;
			/* Set bitmask for new RACH report count to RRM */
			req.req.bitmask |= RACH_SAMPLE_COUNT_PRESENT;
			req.req.rach_sample_count = p_cell_ctxt->new_rachopt_min_ue_rach_report_count;
		}
	}

	/* Send request to RRM to modify config params */
	if (0 != req.req.bitmask)
	{
		req.transaction_id = p_req->transaction_id;
		SON_MEMCPY(&(req.req.cell_id), &p_cell_ctxt->cell_id, sizeof(
					son_intra_rat_global_cell_id_t));
		/* Invoke PUP tool to send RACH config request message to RRM */
		if (RRM_SUCCESS == rrm_son_send_rrm_son_rach_config_req(&req.req,
					SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID,
					p_req->transaction_id, SON_NULL))
		{
			p_cell_ctxt->rrm_request_txid = p_req->transaction_id;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_BRIEF,
					"RRM_SON_RACH_CONFIG_REQ send to "
					" RRM for Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
		}
		else
		{
			if (RACH_SAMPLE_COUNT_PRESENT & req.req.bitmask)
			{
				p_cell_ctxt->modify_res_bitmask |= 
					SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT;
			}
			if ((L2_REPORT_PERIODICITY_PRESENT | 
						UE_REPORT_PERIODICITY_PRESENT)& req.req.bitmask)
			{
				p_cell_ctxt->modify_res_bitmask |= 
					SON_RACHOPT_REPORT_PERIODICITY_PRESENT;
			}
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Failed to send RRM_SON_RACH_CONFIG_REQ to "
					"RRM for Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
		}
	}
	/* If modification of at least one parameter failed */
	if  (0 != p_cell_ctxt->modify_res_bitmask)
	{
		/* If modification of all parameters failed */
		if (p_cell_ctxt->modify_res_bitmask == 
				p_req->modify_req.rach_config_params.bitmask)
		{
			result = SON_FAILURE;
		}
		else
		{
			result = SON_PARTIAL_SUCCESS;
		}
	}
	/* If no request is sent to RRM for modification of parameter
	   then send the modify response to OAM immediately otherwise the response
	   will be send after receiving the RACH config response from RRM */
	if (p_cell_ctxt->rrm_request_txid != p_req->transaction_id)
	{
		modify_res.modify_res.result = result;
		modify_res.modify_res.error_code = error_code;
		modify_res.modify_res.failed_param_bitmask = 
			p_cell_ctxt->modify_res_bitmask;
		/* Reset the modify_res_bitmask in cell context */
		p_cell_ctxt->modify_res_bitmask = 0;
		son_create_send_buffer((son_u8 *)&modify_res,
				SON_RACH_OPT_MODULE_ID,
				SON_MIF_MODULE_ID, SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES,
				modify_res_len);
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Sent SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES to "
				"SONMgmtIfH immediately for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
	}
	rach_opt_print_cell_context(
			"sonrach_opt_cell_fsm_modify_config_req_in_enabled_state_handler");

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_modify_config_req_in_enabled_state_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_modify_config_req_in_disabled_state_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for
 *                  SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ message received from 
 *                  SONMgmtIfH in RACH_OPT_CELL_STATE_DISABLED state.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_modify_config_req_in_disabled_state_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_modify_config_params_req_t *p_req = 
		(son_rach_opt_modify_config_params_req_t *)p_rach_opt_msg;
	son_rach_opt_modify_config_params_res_t modify_res;
	son_size_t modify_res_len = sizeof(son_rach_opt_modify_config_params_res_t);
	son_return_et result = SON_SUCCESS;
	son_error_et error_code = SON_NO_ERROR;
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&modify_res, 0, modify_res_len);
	modify_res.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&modify_res.modify_res.srv_cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/*SPR-11745-Start*/

	if(SON_NULL == p_req->modify_req.rach_config_params.bitmask)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_DETAILED,
                    /*SPR 17777 +-*/
				"Message coming from OAM does not have any"
                "parameters to configure event");
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
	}

	/*SPR-11745-End*/

	sonrach_opt_modify_config_params(p_cell_ctxt, 
			&p_req->modify_req.rach_config_params, &error_code);
	if (SON_RACHOPT_REPORT_PERIODICITY_PRESENT &
			p_req->modify_req.rach_config_params.bitmask)
	{
		if (p_req->modify_req.rach_config_params.
				rachopt_report_periodicity
				!= p_cell_ctxt->oam_config_params.
				rachopt_report_periodicity)
		{
			p_cell_ctxt->oam_config_params.rachopt_report_periodicity = 
				p_req->modify_req.rach_config_params.
				rachopt_report_periodicity;
		}
	}

	if (SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT &
			p_req->modify_req.rach_config_params.bitmask)
	{
		if (p_req->modify_req.rach_config_params.
				rachopt_min_ue_rach_report_count != 
				p_cell_ctxt->oam_config_params.
				rachopt_min_ue_rach_report_count)
		{
			p_cell_ctxt->oam_config_params.
				rachopt_min_ue_rach_report_count = 
				p_req->modify_req.rach_config_params.
				rachopt_min_ue_rach_report_count;
		}
	}

	/* If modification of at least one parameter failed */
	if  (0 != p_cell_ctxt->modify_res_bitmask)
	{
		/* If modification of all parameters failed */
		if (p_cell_ctxt->modify_res_bitmask == 
				p_req->modify_req.rach_config_params.bitmask)
		{
			result = SON_FAILURE;
		}
		else
		{
			result = SON_PARTIAL_SUCCESS;
		}
	}
	/* Send the modify response to OAM */
	modify_res.modify_res.result = result;
	modify_res.modify_res.error_code = error_code;
	modify_res.modify_res.failed_param_bitmask = 
		p_cell_ctxt->modify_res_bitmask;
	/* Reset the modify_res_bitmask in cell context */
	p_cell_ctxt->modify_res_bitmask = 0;
	son_create_send_buffer((son_u8 *)&modify_res,
			SON_RACH_OPT_MODULE_ID,
			SON_MIF_MODULE_ID, SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES,
			modify_res_len);
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_INFO,
			"Sent SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES to SONMgmtIfH "
			"immediately for Cell 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_cell_ctxt->cell_id.cell_identity));
	rach_opt_print_cell_context(
			"sonrach_opt_cell_fsm_modify_config_req_in_disabled_state_handler");

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_modify_config_req_in_disabled_state_handler */

/*****************************************************************************
 * Function Name  : son_rach_opt_cell_fsm_nack_enable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function to send nack for
 *                  SONRACH_OPT_ENABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_rach_opt_cell_fsm_nack_enable_req_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
     /*SPR 17777 +-*/
    (void)p_cspl_hdr;
    (void)evnt_id;
    /*SPR 17777 +-*/
	son_rach_opt_enable_req_t *p_req = 
		(son_rach_opt_enable_req_t *)p_rach_opt_msg;
	son_rach_opt_enable_res_t enable_res;
	son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&enable_res, 0, enable_res_len);
	enable_res.transaction_id = p_req->transaction_id;
	enable_res.enable_res.result = SON_FAILURE;
	enable_res.enable_res.error_code = SON_NO_ERROR;
	enable_res.enable_res.cell_status_list_size = 1;
	SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	if (RACH_OPT_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		enable_res.enable_res.cell_status_list[0].error_code = 
			SON_ERR_ENABLE_IN_PROGRESS;
	}

	if (RACH_OPT_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state)
	{
		enable_res.enable_res.cell_status_list[0].error_code = 
			SON_ERR_ALREADY_ENABLED;
	}

	son_create_send_buffer((son_u8 *)&enable_res,
			SON_RACH_OPT_MODULE_ID,
			SON_MIF_MODULE_ID, SONRACH_OPT_ENABLE_RES,
			enable_res_len);

	SON_UT_TRACE_EXIT();
} /* son_rach_opt_cell_fsm_nack_enable_req_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_nack_disable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function to send nack for
 *                  SONRACH_OPT_DISABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_nack_disable_req_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_disable_req_t *p_req = 
		(son_rach_opt_disable_req_t *)p_rach_opt_msg;
	son_rach_opt_disable_res_t disable_res;
	son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);
     /*SPR 17777 +-*/
    (void)p_cspl_hdr;
    (void)evnt_id;
    /*SPR 17777 +-*/
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&disable_res, 0, disable_res_len);
	disable_res.transaction_id = p_req->transaction_id;
	disable_res.disable_res.result = SON_FAILURE;
	disable_res.disable_res.error_code = SON_NO_ERROR;
	disable_res.disable_res.cell_status_list_size = 1;
	SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	if (RACH_OPT_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		disable_res.disable_res.cell_status_list[0].error_code = 
			SON_ERR_DISABLE_IN_PROGRESS;
	}

	if (RACH_OPT_CELL_STATE_DISABLED == p_cell_ctxt->current_cell_fsm_state)
	{
		disable_res.disable_res.cell_status_list[0].error_code =
			SON_ERR_ALREADY_DISABLED;
	}

	son_create_send_buffer((son_u8 *)&disable_res,
			SON_RACH_OPT_MODULE_ID,
			SON_MIF_MODULE_ID, SONRACH_OPT_DISABLE_RES,
			disable_res_len);

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_nack_modify_config_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function to send nack for
 *                  SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_nack_modify_config_req_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rach_opt_modify_config_params_req_t *p_req = 
		(son_rach_opt_modify_config_params_req_t *)p_rach_opt_msg;
	son_rach_opt_modify_config_params_res_t modify_res;
	son_error_et error_code = SON_NO_ERROR;
	son_size_t modify_res_len = sizeof(
			son_rach_opt_modify_config_params_res_t);

	SON_UT_TRACE_ENTER();

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case RACH_OPT_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case RACH_OPT_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}
		default:
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_ERROR,
						"Invalid Cell FSM state %u",
						p_cell_ctxt->current_cell_fsm_state);
				LTE_GCC_UNUSED_PARAM(evnt_id);
				LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
				break;
			}
	} /* switch */

	SON_MEMSET(&modify_res, 0, modify_res_len);
	modify_res.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&modify_res.modify_res.srv_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	modify_res.modify_res.result = SON_FAILURE;
	modify_res.modify_res.error_code = error_code;
	modify_res.modify_res.failed_param_bitmask = 
		p_req->modify_req.rach_config_params.bitmask;

	son_create_send_buffer((son_u8 *)&modify_res,
			SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
			SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES,
			modify_res_len);

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_nack_modify_config_req_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_invalid_event_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the messages received
 *                  in invalid states for which nothing needs to be done.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_invalid_event_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	SON_UT_TRACE_ENTER();

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
			SON_WARNING,
			"Invalid Event %s "
                    /*SPR 17777 +-*/
			"occured in %s state. [%p], [%d]",
			SON_RACH_OPT_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_RACH_OPT_CELL_FSM_STATES_NAMES[
			p_cell_ctxt->current_cell_fsm_state],p_rach_opt_msg,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_state_change_ind_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the cell state change ind
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_state_change_ind_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_cell_state_change_indication_t *p_state_ind = 
		(son_cell_state_change_indication_t *)p_rach_opt_msg;
	son_feature_state_et                state = SON_STATE_UNDEFINED;
	son_feature_state_change_cause_et   cause = 
		SON_STATE_CHANGE_CAUSE_UNDEFINED;

	SON_UT_TRACE_ENTER();
	SON_LOG(g_rach_opt_gb_ctxt.log_mode,
			p_son_rach_opt_facility_name, SON_BRIEF,
			"RRM_SON_CELL_STATE_CHANGE_IND received:"
			"Cell Id: 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_state_ind->cell_id.cell_identity));
	if (RRM_CELL_DELETED == p_state_ind->cell_state)
	{
		state = SON_STATE_DELETED;
		cause = SON_RRM_CELL_UNCONFIGURED;

		rach_opt_delete_cell_from_context((son_intra_rat_global_cell_id_t *)
				&p_state_ind->cell_id);
	}
	else if (RRM_CELL_UNOPERATIONAL == p_state_ind->cell_state)
	{
		state = SON_STATE_ENABLED;
		cause = SON_RRM_CELL_UNOPERATIONAL;
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_ENABLED);
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Invalid Cell"
                    /*SPR 17777 +-*/
				" state %u received from RRM", p_state_ind->cell_state);
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
	}   

	if (SON_STATE_UNDEFINED != state)
	{
		son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
		api = rach_opt_get_pending_res_api_id();

		if (rach_opt_get_pending_res_expected_count())
		{
			rach_opt_delete_cell_from_pending_res
				((son_intra_rat_global_cell_id_t *)&p_state_ind->cell_id);

			/* Decrement expected_response_count by 1 */
			rach_opt_decrement_pending_res_expected_count();

			if (0 == rach_opt_get_pending_res_expected_count())
			{
				g_rach_opt_gb_ctxt.current_fsm_state = RACH_OPT_STATE_ACTIVE;
				if (SONRACH_OPT_DISABLE_RES == api) 
				{
					son_rach_opt_disable_res_t res;
					son_size_t disable_res_len = sizeof(
							son_rach_opt_disable_res_t);
					SON_MEMSET(&res, 0, disable_res_len);
					res.transaction_id = 
						rach_opt_get_pending_res_transaction_id();
					res.disable_res.error_code = SON_NO_ERROR;

					/* Remove Successful cells from the
					 * RACH_OPT_Pending_Response_LIST and create cell array with
					 * entry for each cell for which RACH OPT could not be
					 * disabled. Last argument is not required
					 * as the cell FSM state is already set above */
					rach_opt_construct_cell_arr_from_pending_res_list(
							&res.disable_res.cell_status_list_size,
							res.disable_res.cell_status_list,
							RACH_OPT_CELL_NUM_OF_STATES);
					if (0 != res.disable_res.cell_status_list_size)
					{
						res.disable_res.result = SON_FAILURE;
					}
					else
					{
						res.disable_res.result = SON_SUCCESS;
					}

					son_create_send_buffer((son_u8 *)&res,
							SON_RACH_OPT_MODULE_ID,
							SON_MIF_MODULE_ID,
							SONRACH_OPT_DISABLE_RES,
							disable_res_len);
				}

				rach_opt_reset_pending_resp_list();
				son_stop_timer(g_rach_opt_gb_ctxt.
						rach_opt_pending_res_timer_id);
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name, SON_BRIEF,
						"Pending response timer with Timer Id [%p] is stopped.",
						g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id);
				g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = SON_PNULL;
			}
		}

		if(SMIF_CELL_INFO_IND == api)
		{
			rach_opt_send_feature_state_change_ind(
					(son_intra_rat_global_cell_id_t *)&p_state_ind->cell_id,
					state, cause);
		}
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_state_change_ind_handler */

/*****************************************************************************
 * Function Name  : son_rach_opt_cell_fsm_anr_disable_ind_handler
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the ANR mode change 
 *                  indication message received from ANR. ANR sends this
 *                  indication whenever ANR mode is disabled for a cell.
 ****************************************************************************/
static son_void_t
son_rach_opt_cell_fsm_anr_disable_ind_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_anr_cell_mode_ind_t *p_anr_mode_ind = 
		(son_anr_cell_mode_ind_t *)p_rach_opt_msg;
	son_rrm_rach_config_req_t req = {0};
	son_u16 tx_id = SON_MIN_TRANSACTION_ID;

	SON_UT_TRACE_ENTER();

	SON_LOG(g_rach_opt_gb_ctxt.log_mode,
			p_son_rach_opt_facility_name, SON_BRIEF,
			"SONANR_DISABLE_IND received:"
			"Cell Id: 0x%x",
			son_convert_char_cell_id_to_int_cell_id(
				p_anr_mode_ind->srv_cgi.cell_identity));

	if (ANR_MODE_DISABLED == p_anr_mode_ind->cell_mode)
	{
		req.transaction_id = son_generate_txn_id();
		SON_MEMCPY(&(req.req.cell_id), &p_anr_mode_ind->srv_cgi, sizeof(
					son_intra_rat_global_cell_id_t));
		req.req.bitmask = (L2_REPORT_PERIODICITY_PRESENT |
				UE_REPORT_PERIODICITY_PRESENT);
		req.req.l2_reports_periodicity = 0;
		req.req.ue_reports_periodicity = 0;
		tx_id = son_generate_txn_id();

		/* Invoke PUP tool to send RACH config request message to stop receiving
		   RACH related reports from RRM */
		if (RRM_FAILURE == rrm_son_send_rrm_son_rach_config_req(&req.req,
					SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID, tx_id, SON_NULL))
		{
			rach_opt_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
					SON_STATE_DISABLED, SON_ANR_MODE_DISABLED);

			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name, SON_ERROR,
					"Failed to send RACH config request on receiving "
                    /*SPR 17777 +-*/
					"SONANR_DISABLE_IND from ANR for Cell 0x%x]",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
			LTE_GCC_UNUSED_PARAM(evnt_id);
			LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
			SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					RACH_OPT_CELL_STATE_DISABLED);
		}
		else
		{    
			p_cell_ctxt->dereg_invoking_api = SONANR_DISABLE_IND;
			SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					RACH_OPT_CELL_STATE_DISABLING);
			p_cell_ctxt->rrm_request_txid = tx_id;
		}
	} 

	SON_UT_TRACE_EXIT();
} /* son_rach_opt_cell_fsm_anr_disable_ind_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_rach_config_resp_handler_in_disabling_state
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the RACH config response
 *                  message received from RRM in RACH_OPT_CELL_STATE_DISABLING 
 *                  state.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_rach_config_resp_handler_in_disabling_state
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_return_et result = SON_FAILURE;
	son_rrm_rach_config_resp_t *p_res = 
		(son_rrm_rach_config_resp_t *)p_rach_opt_msg;
	son_rach_opt_disable_res_t res;
	son_error_et error_code = SON_NO_ERROR;
	son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);

	SON_UT_TRACE_ENTER();

	if (p_res->transaction_id != p_cell_ctxt->rrm_request_txid)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"RACH config response received with transaction Id = %u while the "
                    /*SPR 17777 +-*/
				"expected response transaction Id = %u for Cell 0x%x",
				p_res->transaction_id, p_cell_ctxt->rrm_request_txid,
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
		SON_UT_TRACE_EXIT();
		return;
	}

    /* SPR-19827 START */
    p_cell_ctxt->rrm_request_txid = SON_INVALID_TXID;
    /* SPR-19827 END */

	result = ((p_res->res.result == RRM_SUCCESS) ?
			SON_SUCCESS : SON_FAILURE);
	if (SONANR_DISABLE_IND == p_cell_ctxt->dereg_invoking_api)
	{
		p_cell_ctxt->dereg_invoking_api = SON_PROCEDURE_UNDEFINED;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_BRIEF,
				"RACH config response received for RACH OPT generated"
				"RRM_SON_RACH_CONFIG_REQ with result = %u for "
				"Cell 0x%x",
				result, son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));

		rach_opt_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
				SON_STATE_DISABLED, SON_ANR_MODE_DISABLED);

		/* Change cell FSM state to RACH_OPT_CELL_STATE_DISABLED */
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_DISABLED);

		SON_UT_TRACE_EXIT();
		return;
	}

	if (SON_FAILURE == result)
	{
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_ENABLED);
		error_code = anr_map_rrm_error_codes(
				__func__,
				result, p_res->res.error_code,
				RRM_SON_RACH_CONFIG_RESP);
	}
	else
	{
		/* Change cell FSM state to RACH_OPT_CELL_STATE_DISABLED */
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_DISABLED);
	}

	if (p_res->transaction_id == rach_opt_get_pending_res_transaction_id())
	{
		cell_status_list_data_t *p_list_node = SON_PNULL;
		/* Search for the cell Id in pending response structure. */
		p_list_node = rach_opt_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

		/* Presence indicates that cell state is disabling due to Global
		 * Disable Request */
		if (SON_PNULL != p_list_node)
		{
			/* Update Cell in RACH OPT Pending Response List with error code
			 * determined above */
			p_list_node->cell_status.error_code = error_code;
			/* Decrement expected_response_count by 1 */
			rach_opt_decrement_pending_res_expected_count();
			/* Check if expected response count == 0 */
			if (0 == rach_opt_get_pending_res_expected_count())
			{
				SON_MEMSET(&res, 0, disable_res_len);
				res.transaction_id = p_res->transaction_id;
				res.disable_res.error_code = SON_NO_ERROR;

				/* Remove Successful cells from the
				 * RACH_OPT_Pending_Response_LIST and create cell array with
				 * entry for each cell for which RACH OPT could not be
				 * enabled/disabled. Last argument is not required
				 * as the cell FSM state is already set above */
				rach_opt_construct_cell_arr_from_pending_res_list(
						&res.disable_res.cell_status_list_size,
						res.disable_res.cell_status_list,
						RACH_OPT_CELL_NUM_OF_STATES);                        
				if (0 != res.disable_res.cell_status_list_size)
				{
					res.disable_res.result = SON_FAILURE;
				}
				else
				{
					res.disable_res.result = SON_SUCCESS;
				}

				son_create_send_buffer((son_u8 *)&res,
						SON_RACH_OPT_MODULE_ID,
						SON_MIF_MODULE_ID,
						SONRACH_OPT_DISABLE_RES,
						disable_res_len);
				/* Set RACH OPT global FSM state to ACTIVE */
				SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
						RACH_OPT_STATE_ACTIVE);
				/* Reset RACH_OPT_Pending_Response_LIST */
				rach_opt_reset_pending_resp_list();
				/* Stop the RACH_OPT_PENDING_RESPONSE_TIMER */
				son_stop_timer(g_rach_opt_gb_ctxt.
						rach_opt_pending_res_timer_id);
				g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = SON_PNULL;
			}
		}
	}
	else
	{
		/* Send SONRACH_OPT_DISABLE_RES to SONMgmtIfH */
		SON_MEMSET(&res, 0, disable_res_len);
		res.transaction_id = p_res->transaction_id;
		res.disable_res.result = result;
		res.disable_res.error_code = SON_NO_ERROR;
		if (SON_FAILURE == result)
		{
			res.disable_res.cell_status_list_size = 1;
			SON_MEMCPY(&res.disable_res.cell_status_list[0].cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			res.disable_res.cell_status_list[0].error_code = error_code;
		}

		son_create_send_buffer((son_u8 *)&res,
				SON_RACH_OPT_MODULE_ID,
				SON_MIF_MODULE_ID, SONRACH_OPT_DISABLE_RES,
				disable_res_len);
	}

	if ((RRM_ERR_OBJECT_NON_EXISTENT == 
				p_res->res.error_code) ||
			(RRM_ERR_CELL_UNCONFIGURED == \
			 p_res->res.error_code))
	{
		rach_opt_delete_cell_from_context(&p_cell_ctxt->cell_id);
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_rach_config_resp_handler_in_disabling_state */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabling_state
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for RRM_SON_RACH_CONFIG_RESP
 *                  message received from RRM in RACH_OPT_CELL_STATE_ENABLING 
 *                  state.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabling_state
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
	son_return_et result = SON_FAILURE;
	son_rrm_rach_config_resp_t *p_res = 
		(son_rrm_rach_config_resp_t *)p_rach_opt_msg;
	son_rach_opt_enable_res_t res;
	son_error_et error_code = SON_NO_ERROR;
	son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);
	son_feature_state_et state = SON_STATE_DISABLED;
	son_feature_state_change_cause_et cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;
	anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    /* SPR 20653 Fix Start */
	anr_cell_fsm_state_et anr_cell_fsm_state = ANR_CELL_STATE_MAX;
    /* SPR 20653 Fix End */

	SON_UT_TRACE_ENTER();

	if (p_res->transaction_id != p_cell_ctxt->rrm_request_txid)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"RACH config response received with transaction Id = %u while the "
                    /*SPR 17777 +-*/
				"expected response transaction Id = %u for Cell 0x%x",
				p_res->transaction_id, p_cell_ctxt->rrm_request_txid,
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
		SON_UT_TRACE_EXIT();
		return;
	}

    /* SPR-19827 START */
    p_cell_ctxt->rrm_request_txid = SON_INVALID_TXID;
    /* SPR-19827 END */


	SON_MEMSET(&res, 0, enable_res_len);
	result = ((p_res->res.result == RRM_SUCCESS) ? 
			SON_SUCCESS : SON_FAILURE);
	if (SON_FAILURE == result)
	{
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_DISABLED);
		error_code = anr_map_rrm_error_codes(
				__func__, 
				result, p_res->res.error_code, 
				RRM_SON_RACH_CONFIG_RESP);
	}
	else
	{
		/* Check from LTE SON/ANR context whether the Cell Id is present 
		 * in ANR module cell context and ANR mode is ENABLED or not
		 * for the cell. */
		p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
				&p_cell_ctxt->cell_id);

		if (SON_PNULL != p_anr_cell_ctxt)
		{
			anr_cell_fsm_state = anr_get_anr_mode_for_cell(p_anr_cell_ctxt);
			if (ANR_CELL_STATE_ENABLED != anr_cell_fsm_state)
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name, SON_WARNING,
						"RRM_SON_RACH_CONFIG_RESP received for "
						"Cell 0x%x but the cell is disabled at ANR",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
			}
		}
		else 
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"RRM_SON_RACH_CONFIG_RESP received for cell "
					"Cell 0x%x but the cell is unconfigured at ANR",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
		}
		/* Change cell FSM state to RACH_OPT_CELL_STATE_ENABLED */
		SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				RACH_OPT_CELL_STATE_ENABLED);
		state = SON_STATE_ENABLED;
		cause = SON_RRM_REGISTRATION_SUCCESSFUL;
	}

	if (p_res->transaction_id == rach_opt_get_pending_res_transaction_id())
	{
		cell_status_list_data_t *p_list_node = SON_PNULL;
		/* Search for the cell Id in pending response structure. */
		p_list_node = rach_opt_find_cell_in_pending_res(
				&p_cell_ctxt->cell_id);

		/* Presence indicates that cell state is enabling due to Global 
		 * Enable Request or Cell Info Indication and no immediate Enable
		 * Response must be sent from cell FSM */
		if (SON_PNULL != p_list_node)
		{
			api = rach_opt_get_pending_res_api_id();

			/* Update Cell in RACH OPT Pending Response List with error code
			 *  determined above */
			p_list_node->cell_status.error_code = error_code;
			/* Decrement expected_response_count by 1 */
			rach_opt_decrement_pending_res_expected_count();

			/* Check if expected response count == 0 */
			if (0 == rach_opt_get_pending_res_expected_count())
			{
				if (SONRACH_OPT_ENABLE_RES == api)
				{
					res.transaction_id = p_res->transaction_id;
					res.enable_res.error_code = SON_NO_ERROR;

					/* Remove Successful cells from the 
					 *  RACH_OPT_Pending_Response_LIST and create cell 
					 array with entry for each cell for which RACH 
					 OPT could not be enabled. Last argument is not 
					 required as the cell FSM state is already set 
					 above */
					rach_opt_construct_cell_arr_from_pending_res_list(
							&res.enable_res.cell_status_list_size,
							res.enable_res.cell_status_list, 
							RACH_OPT_CELL_NUM_OF_STATES);
					if (0 != res.enable_res.cell_status_list_size) 
					{
						res.enable_res.result = SON_FAILURE;
					}
					else
					{
						res.enable_res.result = SON_SUCCESS;
					}

					son_create_send_buffer((son_u8 *)&res,
							SON_RACH_OPT_MODULE_ID,
							SON_MIF_MODULE_ID,
							SONRACH_OPT_ENABLE_RES,
							enable_res_len);
				}
				else
				{
					/* Remove cell from the RACH OPT Pending Response 
					   LIST */
					rach_opt_delete_cell_from_pending_res(
							&p_cell_ctxt->cell_id);
				}

				/* Set RACH OPT global FSM state to ACTIVE */
				SONRACH_OPT_FSM_SET_STATE(
						g_rach_opt_gb_ctxt.current_fsm_state,
						RACH_OPT_STATE_ACTIVE);
				/* Reset RACH_OPT_Pending_Response_LIST */
				rach_opt_reset_pending_resp_list();
				/* Stop the RACH_OPT_PENDING_RESPONSE_TIMER */
				son_stop_timer(g_rach_opt_gb_ctxt.
						rach_opt_pending_res_timer_id);
				g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = 
					SON_PNULL;
			}
		}
	}
	else
	{
		/* Send SONRACH_OPT_ENABLE_RES to SONMgmtIfH */
		res.transaction_id = p_res->transaction_id;
		res.enable_res.result = result;
		res.enable_res.error_code = SON_NO_ERROR;
		if (SON_NO_ERROR != error_code)
		{
			res.enable_res.result = SON_FAILURE;
			res.enable_res.cell_status_list_size = 1;
			SON_MEMCPY(&res.enable_res.cell_status_list[0].cgi, 
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			res.enable_res.cell_status_list[0].error_code = error_code;
		}

		son_create_send_buffer((son_u8 *)&res,
				SON_RACH_OPT_MODULE_ID,
				SON_MIF_MODULE_ID, SONRACH_OPT_ENABLE_RES,
				enable_res_len);
	}

	if ((RRM_ERR_OBJECT_NON_EXISTENT == 
				p_res->res.error_code) ||
			(RRM_ERR_CELL_UNCONFIGURED == 
			 p_res->res.error_code))
	{
		state = SON_STATE_DELETED;
		cause = SON_RRM_CELL_UNCONFIGURED;
	}

	if (SMIF_CELL_INFO_IND == api)
	{
		/* Send feature state change indication if received 
		 * RACH config response is for the request triggered due
		 * to Cell Info Indication request received by RACH OPT */
		rach_opt_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
				state, cause);
		rach_opt_delete_cell_from_pending_res(&p_cell_ctxt->cell_id);         
	}

	if ((RRM_ERR_OBJECT_NON_EXISTENT == 
				p_res->res.error_code) ||
			(RRM_ERR_CELL_UNCONFIGURED == 
			 p_res->res.error_code))
	{
		rach_opt_delete_cell_from_context(&p_cell_ctxt->cell_id);
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabling_state */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabled_state
 * Inputs         : evnt_id - Value of the event Id internal to RACH OPT module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_rach_opt_msg - Pointer to the message data received 
 *                  (after the CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for RRM_SON_RACH_CONFIG_RESP
 *                  message received from RRM in RACH_OPT_CELL_STATE_ENABLED 
 *                  state.
 ****************************************************************************/
static son_void_t
sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabled_state
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_return_et result = SON_FAILURE;
	son_rrm_rach_config_resp_t *p_res = 
		(son_rrm_rach_config_resp_t *)p_rach_opt_msg;
	son_rach_opt_modify_config_params_res_t res;
	son_error_et error_code = SON_NO_ERROR;
	son_size_t modify_res_len = sizeof(son_rach_opt_modify_config_params_res_t);

	SON_UT_TRACE_ENTER();

	if (p_res->transaction_id != p_cell_ctxt->rrm_request_txid)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"RACH config response received with transaction Id = %u while the "
                    /*SPR 17777 +-*/
				"expected response transaction Id = %u for Cell 0x%x",
				p_res->transaction_id, p_cell_ctxt->rrm_request_txid,
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
		SON_UT_TRACE_EXIT();
		return;
	}
    /* SPR-19827 START */
    p_cell_ctxt->rrm_request_txid = SON_INVALID_TXID;
    /* SPR-19827 END */

	SON_MEMSET(&res, 0, modify_res_len);
	result = ((p_res->res.result == RRM_SUCCESS) ? 
			SON_SUCCESS : SON_FAILURE);
	if (SON_FAILURE == result)
	{
		error_code = anr_map_rrm_error_codes(
				__func__, 
				result, p_res->res.error_code, 
				RRM_SON_RACH_CONFIG_RESP);
		/* Set bitmask in cell context */
		p_cell_ctxt->modify_res_bitmask |= 
			(SON_RACHOPT_REPORT_PERIODICITY_PRESENT | 
			 SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT);
	}
	else
	{
		/* Store the new values in cell context since they are successfully 
		   updated at RRM */
		p_cell_ctxt->oam_config_params.rachopt_report_periodicity = 
			p_cell_ctxt->new_rachopt_report_periodicity;
		p_cell_ctxt->oam_config_params.
			rachopt_min_ue_rach_report_count = 
			p_cell_ctxt->new_rachopt_min_ue_rach_report_count;
	}

	/* Reset temporarily stored new values */
	p_cell_ctxt->new_rachopt_report_periodicity = 0;
	p_cell_ctxt->new_rachopt_min_ue_rach_report_count = 0;
	rach_opt_print_cell_context(
			"sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabled_state");

	/* Send SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES to SONMgmtIfH */
	res.transaction_id = p_res->transaction_id;
	SON_MEMCPY(&res.modify_res.srv_cgi, 
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	res.modify_res.result = result;
	res.modify_res.error_code = error_code;
	res.modify_res.failed_param_bitmask = 
		p_cell_ctxt->modify_res_bitmask;
	/* Reset the modify_res_bitmask in cell context */
	p_cell_ctxt->modify_res_bitmask = 0;

	son_create_send_buffer((son_u8 *)&res,
			SON_RACH_OPT_MODULE_ID,
			SON_MIF_MODULE_ID, SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES,
			modify_res_len);

	if ((RRM_ERR_OBJECT_NON_EXISTENT == p_res->res.error_code) ||
			(RRM_ERR_CELL_UNCONFIGURED == p_res->res.error_code))
	{
		rach_opt_delete_cell_from_context(&p_cell_ctxt->cell_id);
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabled_state */

/*****************************************************************************
 * Function Name  : rach_opt_send_algo_failure_notifcation_to_oam
 * Inputs         : p_cell_ctxt       - Pointer to the cell context
 *                : cause              - Rach optmization failure cause.
 *                : identified_opt_type- Optimization type for which failure
 *                                       notifcation is triggered.
 * Returns        : son_void_t 
 * Description    : This function populates and send the 
 SONRACH_OPT_FAILURE_NOTIFICATION message to MIF
 ****************************************************************************/
static son_void_t
rach_opt_send_algo_failure_notifcation_to_oam
(
 rach_opt_cell_context_t                         *p_cell_ctxt,
 son_rach_opt_failure_cause_et                   cause,
 son_rach_opt_identified_optimization_type_et    identified_opt_type
 )
{
	son_oam_rach_opt_failure_notification_t  failure_notifcation = {0};
	SON_MEMCPY(&failure_notifcation.srv_cgi,
			&p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));
	failure_notifcation.cause = cause;
	if (p_cell_ctxt->rach_algo_counters.
			percentage_of_samples_with_atmpt_more_than_thresh > 
			p_cell_ctxt->oam_config_params.rachopt_start_threshold)
	{
		failure_notifcation.rach_failure_level = 
			SON_RACH_OPT_ABOVE_START_THRESHOLD;
	}
	else
	{
		failure_notifcation.rach_failure_level = 
			SON_RACH_OPT_ABOVE_STOP_THRESHOLD;
	}
	failure_notifcation.identified_opt_type = identified_opt_type;

	son_create_send_buffer((son_u8 *)&failure_notifcation,
			SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
			SONRACH_OPT_FAILURE_NOTIFICATION,sizeof(failure_notifcation));

}

/*****************************************************************************
 * Function Name  : rach_opt_compare_pci
 * Inputs         : p_val_1     Pointer to the PCI value as void pointer
 *                : p_val_2     Pointer to the PCI value as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the PCI values
 ******************************************************************************/
son_s32
rach_opt_compare_pci
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Pointer to the PCI is NULL");
	}
	else
	{
		ret_value = (*(son_u16 *)p_val_1 != *(son_u16 *)p_val_2);
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* rach_opt_compare_pci */

/*****************************************************************************
 * Function Name  : rach_opt_compare_count
 * Inputs         : p_val_1     Pointer to the PCI value as void pointer
 *                : p_val_2     Pointer to the PCI value as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the PCI values
 ******************************************************************************/
son_s32
rach_opt_compare_count
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = 1;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Pointer to the PCI is NULL");
	}
	else
	{
		if(*(son_u16 *)p_val_1 > *(son_u16 *)p_val_2)
		{
			ret_value = -1;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* rach_opt_compare_count */

/*****************************************************************************
 * Function Name  : initialize_algo_counters_from_rrm_ue_report
 * Inputs         : p_cell_ctxt       - Pointer to the cell context
 * Inputs         : p_rach_ue_report  - Pointer to the rach information
 message from RRM.
 * Outputs        : p_rach_algo_counters - Pointer to the rach algo counters
 structure.
 * Returns        : son_return_et 
 * Description    : This function rests the algorithm counters determined from
 RRM_SON_RACH_UE_INFO_IND.
 ****************************************************************************/
static son_void_t
initialize_algo_counters_from_rrm_ue_report
(
 rach_opt_cell_context_t *p_cell_ctxt
 )
{
	son_u64 total_unassigned_sum_cfra = 
		p_cell_ctxt->rach_algo_counters.total_unassigned_sum_cfra;

	SON_MEMSET(&p_cell_ctxt->rach_algo_counters, 0, 
			sizeof(p_cell_ctxt->rach_algo_counters));
	rach_opt_clear_strongest_nbr_cells_list(p_cell_ctxt);
	p_cell_ctxt->rach_algo_counters.total_unassigned_sum_cfra =
		total_unassigned_sum_cfra;
}

/*****************************************************************************
 * Function Name  : rach_opt_pci_key_of_strong_cell_list
 * Inputs         : p_node - anchor field of the record
 * Outputs        : None
 * Returns        : Pointer to PCI as void pointer
 * Description    : This function determines the key in RACH OPT strongest 
 *                  neighbor cell list. Here we want PCI value to be used 
 *                  as the key field of the list.
 ******************************************************************************/
const son_void_t *
rach_opt_pci_key_of_strong_cell_list
(
 const SON_LIST_NODE *p_node
 )
{
	rach_opt_strongest_nbr_cell_node_t *p_data = SON_PNULL;
	son_u16 *p_pci = SON_PNULL;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Pointer to node is NULL");
	}
	else
	{
		p_data = YMEMBEROF(rach_opt_strongest_nbr_cell_node_t, sNode, p_node);
		p_pci = &(p_data->data.pci);
	}    

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_pci;
} /* rach_opt_pci_key_of_strong_cell_list */

/*****************************************************************************
 * Function Name  : rach_opt_count_key_of_strong_cell_list
 * Inputs         : p_node - anchor field of the record
 * Outputs        : None
 * Returns        : Pointer to count as void pointer
 * Description    : This function determines the key in RACH OPT strongest 
 *                  neighbor cell list. Here we want count value to be used 
 *                  as the key field of the list.
 ******************************************************************************/
const son_void_t *
rach_opt_count_key_of_strong_cell_list
(
 const SON_LIST_NODE *p_node
 )
{
	rach_opt_strongest_nbr_cell_node_t *p_data = SON_PNULL;
	son_u16 *p_count = SON_PNULL;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Pointer to node is NULL");
	}
	else
	{
		p_data = YMEMBEROF(rach_opt_strongest_nbr_cell_node_t, sNode, p_node);
		p_count = &(p_data->data.count);
	}    

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_count;
} /* rach_opt_count_key_of_strong_cell_list */
/*****************************************************************************
 * Function Name  : print_rach_algo_counters
 * Inputs         : p_cell_ctxt       - Pointer to the cell context.
 * outputs        : none
 * Returns        : son_void_t
 * Description    : This function prints the rach algo counters strcture.
 ****************************************************************************/
static son_void_t
print_rach_algo_counters
(
 rach_opt_cell_context_t             *p_cell_ctxt
 )
{
	SON_UT_TRACE_ENTER();
	if(SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"The pointer p_cell_ctxt is SON_PNULL");

		SON_UT_TRACE_EXIT();
		return;
	}

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_BRIEF,"*****************************RACH COUNTERS START***********************\n"
			"Serving Cell 0x%x have the rach couters from RRM msgs as below:\n"
			"Number of Samples with attempts more than equal to threshold:[%u]\n"
			"Number of Samples with attempts more than 1 attempt:[%u] \n"
			"Number of Samples with contention and attempts more than threshold:[%u]\n"
			"Number of Centre Samples with attempt more than threshold:[%u]\n"
			"Number of Centre Samples with attempt more than threshold and no contention:[%u]\n"
			"Number of Centre Samples:[%u]\n"
			"Number of Edge Samples:[%u]\n"
			"Percentage of Samples with attempt more than threshold:[%f]%%\n"
			"Rach failure percentage:[%f]%%\n"
			"Contention percentage:[%f]%%\n"
			"Percenage of Centre Samples with power issue:[%f]%%\n"
			"Total unassigned sum CFRA:[%llu]\n"
			"Number of group A preambles observed:[%llu]\n"
			"Number of group B preambles observed:[%llu]\n"
			"Percentage of Centre Samples:[%f]%%\n"
			"Percentage of Edge Samples:[%f]%%\n",
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cell_ctxt->rach_algo_counters.samples_with_atmpt_more_than_thresh,
			p_cell_ctxt->rach_algo_counters.samples_with_atmpt_more_than_one,
			p_cell_ctxt->rach_algo_counters.samples_with_contention_and_atmpt_more_than_thresh,
			p_cell_ctxt->rach_algo_counters.centre_samples_with_atmpt_more_than_thresh,
			p_cell_ctxt->rach_algo_counters.centre_samples_with_atmpt_more_than_thresh_and_no_contention,
			p_cell_ctxt->rach_algo_counters.centre_samples_count,
			p_cell_ctxt->rach_algo_counters.edge_samples_count,
			p_cell_ctxt->rach_algo_counters.percentage_of_samples_with_atmpt_more_than_thresh,
			p_cell_ctxt->rach_algo_counters.rach_failure_percentage,
			p_cell_ctxt->rach_algo_counters.contention_percentage,
			p_cell_ctxt->rach_algo_counters.percentage_of_centre_samples_with_power_issue,
			p_cell_ctxt->rach_algo_counters.total_unassigned_sum_cfra,
			p_cell_ctxt->rach_algo_counters.num_of_group_a_preambles_observed,
			p_cell_ctxt->rach_algo_counters.num_of_group_b_preambles_observed,
			p_cell_ctxt->rach_algo_counters.percentage_of_centre_samples,
			p_cell_ctxt->rach_algo_counters.percentage_of_edge_samples);

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : rach_opt_calculate_rach_algo_counters
 * Inputs         : p_cell_ctxt       - Pointer to the cell context
 * Inputs         : p_rach_ue_report  - Pointer to the rach information
 message from RRM.
 * Outputs        : p_rach_algo_counters - Pointer to the rach algo counters
 structure.
 * Returns        : son_return_et 
 * Description    : This function calculates the various counters specified
 rach optimization algorithm.
 ****************************************************************************/
static son_return_et
rach_opt_calculate_rach_algo_counters
(
 rach_opt_cell_context_t             *p_cell_ctxt,
 son_rrm_rach_ue_info_ind_t          *p_rach_ue_report,
 rach_opt_algo_counters_context_t    *p_rach_algo_counters
 )
{
	son_return_et  ret  = SON_SUCCESS;
	son_u16 idx = 0, idx_strongest_nbr_cell_list = 0;
	SON_LIST_NODE *p_list_node = SON_PNULL;
	rach_opt_strongest_nbr_cell_node_t *p_data = SON_PNULL;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_rach_algo_counters || SON_PNULL == p_rach_ue_report)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Atleast one of "
				"the pointer (p_rach_algo_counters,p_rach_ue_report) is "
				"SON_PNULL");

		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	for (idx = 0; idx < p_rach_ue_report->count && idx < 
			SON_MAX_NUMBER_OF_PRACH_SAMPLES; idx++)
	{
		for (idx_strongest_nbr_cell_list = 0; 
				idx_strongest_nbr_cell_list < p_rach_ue_report->rach_data[idx].
				strong_cell_info_count; 
				idx_strongest_nbr_cell_list++)
		{
			p_list_node = list_find_node(
					&p_cell_ctxt->rach_opt_strongest_nbr_cell_list,
					&(p_rach_ue_report->rach_data[idx].
						strong_cell_info[idx_strongest_nbr_cell_list].pci), 
					rach_opt_pci_key_of_strong_cell_list,
					rach_opt_compare_pci);

			/* If neighbor with received PCI does not exist then 
			   add it the strongest neighbor cell list */
			if (SON_PNULL == p_list_node)
			{
				rach_opt_strongest_nbr_cell_node_t *p_list_node = 
					SON_PNULL;
				son_size_t list_node_size = sizeof(
						rach_opt_strongest_nbr_cell_node_t);
				/* Insert the PCI value in RACH OPT strongest neighbor
				   cell list */
				p_list_node = (rach_opt_strongest_nbr_cell_node_t *)
					son_mem_get(list_node_size);
				if (SON_PNULL != p_list_node)
				{
					SON_MEMSET(p_list_node, 0, list_node_size);
					p_list_node->data.pci = p_rach_ue_report->rach_data[idx].
						strong_cell_info[idx_strongest_nbr_cell_list].pci;
					p_list_node->data.count++;
					list_insert_node(&p_cell_ctxt->
							rach_opt_strongest_nbr_cell_list, 
							&p_list_node->sNode);
				}
			}
			else
			{
				p_data = YMEMBEROF(rach_opt_strongest_nbr_cell_node_t, sNode, 
						p_list_node);
				/* One more time same neighbor PCI is reported so increment
				   the count by 1 */
				p_data->data.count++;
			}
		}

		/* prach_cause_et various values significance is as follows:
		   1)   RRC_CONNECTED_UE [Group B] - This is the RACH attempt 
		   when UE is connecting for the first time.
		   2)  RRC_RE_ESTABLISHMENT_UE [Group B] - This is the RACH 
		   attempt when UE is handed over or re-establishment is 
		   being attempted.
		   3)  IN_SYNC_IND_UE [Group A] - RACH due to sync loss.
		 */
/* SPR-18438 START */
/* code deleted */
/* SPR-18438 END */
		if (p_rach_ue_report->rach_data[idx].number_of_preambles_sent >=
				p_cell_ctxt->oam_config_params.
				rachopt_ue_rach_attempts_threshold)
		{
			p_rach_algo_counters->samples_with_atmpt_more_than_thresh++; 

			if ((son_u32)SON_TRUE ==  p_rach_ue_report->rach_data[idx].
					contention_detected)
			{
				p_rach_algo_counters->
					samples_with_contention_and_atmpt_more_than_thresh++;
			}

			if (cell_centre == p_rach_ue_report->rach_data[idx].ue_location)
			{
				p_rach_algo_counters->
					centre_samples_with_atmpt_more_than_thresh++;
				if ((son_u32)SON_FALSE ==  p_rach_ue_report->
						rach_data[idx].contention_detected)
				{
					p_rach_algo_counters->
						centre_samples_with_atmpt_more_than_thresh_and_no_contention++;
				}
			}
		}
		if (p_rach_ue_report->rach_data[idx].number_of_preambles_sent > 1)
		{
			p_rach_algo_counters->samples_with_atmpt_more_than_one++;
		}

		if (cell_centre == p_rach_ue_report->rach_data[idx].ue_location)
		{
			p_rach_algo_counters->centre_samples_count++;
		}
		else
		{
			p_rach_algo_counters->edge_samples_count++;
		}
	}

	/*Percentage determination*/
	if (p_rach_ue_report->count)
	{
		p_rach_algo_counters->percentage_of_samples_with_atmpt_more_than_thresh
			= ((son_float)p_rach_algo_counters->
					samples_with_atmpt_more_than_thresh / 
					p_rach_ue_report->count) * 100;

		p_rach_algo_counters->rach_failure_percentage  =  
			p_rach_algo_counters->percentage_of_samples_with_atmpt_more_than_thresh;

		p_rach_algo_counters->percentage_of_centre_samples =
			((son_float)p_rach_algo_counters->centre_samples_count /
			 p_rach_ue_report->count)*100;

		p_rach_algo_counters->percentage_of_edge_samples =
			((son_float)p_rach_algo_counters->edge_samples_count /
			 p_rach_ue_report->count)*100;
	}

	if (p_rach_algo_counters->samples_with_atmpt_more_than_thresh)
	{
		p_rach_algo_counters->contention_percentage =
			((son_float)p_rach_algo_counters->
			 samples_with_contention_and_atmpt_more_than_thresh /
			 p_rach_algo_counters->samples_with_atmpt_more_than_thresh) * 100;
	}

	if (p_rach_algo_counters->centre_samples_count)
	{
		p_rach_algo_counters->percentage_of_centre_samples_with_power_issue =
			((son_float)p_rach_algo_counters->
			 centre_samples_with_atmpt_more_than_thresh/
			 p_rach_algo_counters->centre_samples_count) * 100;
	}

	/* Sort the strongest neighbor cell list */
	list_sort(&p_cell_ctxt->rach_opt_strongest_nbr_cell_list,
			rach_opt_count_key_of_strong_cell_list,
			rach_opt_compare_count);
	/* Print the strongest neighbor cell list */
	rach_opt_print_strongest_nbr_cell_list(
			&p_cell_ctxt->rach_opt_strongest_nbr_cell_list);

	SON_UT_TRACE_EXIT();
	return ret;
}

/*****************************************************************************
 * Function Name  : is_nbr_rec_maps_to_the_prach_freq_offset
 * Inputs         : p_anr_cell_ctxt     - Pointer to the serving cell context
 with ANR.
 *                : p_hash_rec          - Pointer to the intra rat hash record
 *                : serv_duplex_mode    - Duplex mode of the serving cell.
 *                                          (FDD/TDD)
 * Outputs        : 
 * Returns        : son_bool_et     -(SON_TRUE/SON_FALSE) 
 * Description    : This function identifies if the intra rat neighbor is 
 to be considered for the determination of the new
 prach frequency offset for the serving cell or not.    
 ****************************************************************************/
static son_bool_et
is_nbr_rec_maps_to_the_prach_freq_offset
(
 anr_cell_context_t       *p_anr_cell_ctxt,
 anr_intra_rat_hash_rec_t *p_hash_rec,
 son_duplex_mode_et       serv_duplex_mode
 )
{
	son_bool_et ret  = SON_FALSE;
	SON_UT_TRACE_ENTER();

	if (anr_get_earfcn(p_anr_cell_ctxt) == p_hash_rec->neighbor_info.earfcn &&
			(p_hash_rec->neighbor_info.bitmask & 
			 (SON_UL_EARFCN_PRESENT|SON_PRACH_CONFIGURATION_PRESENT)))
	{
		if(SON_FDD_MODE == serv_duplex_mode)
		{
			if (p_anr_cell_ctxt->choice_eutra_mode.fdd_info.ul_earfcn 
					== p_hash_rec->neighbor_info.ul_earfcn)
			{
				ret = SON_TRUE;
			}
		}
		else if ((anr_get_earfcn(p_anr_cell_ctxt) == 
					p_hash_rec->neighbor_info.ul_earfcn) &&
				(p_hash_rec->neighbor_info.prach_config.bitmask &
				 SON_PRACH_CONFIG_INDEX_PRESENT) &&
				(p_anr_cell_ctxt->prach_configuration.prach_configuration_index
				 == p_hash_rec->neighbor_info.prach_config.
				 prach_configuration_index))
		{
			ret = SON_TRUE;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
}


/*****************************************************************************
 * Function Name  : is_nbr_rec_maps_to_the_prach_config_idx
 * Inputs         : p_anr_cell_ctxt     - Pointer to the serving cell context
 with ANR.
 *                : p_hash_rec          - Pointer to the intra rat hash record.
 *                : serv_duplex_mode    - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 * Outputs        : 
 * Returns        : son_bool_et         - (SON_TRUE/SON_FALSE) 
 * Description    : This function  identifies if the intra rat neighbor is to 
 be considered for the determination of the new prach 
 configuration index for the TDD serving cell or not.        
 ****************************************************************************/
static son_bool_et
is_nbr_rec_maps_to_the_prach_config_idx
(
 anr_cell_context_t       *p_anr_cell_ctxt,
 anr_intra_rat_hash_rec_t *p_hash_rec,
 son_duplex_mode_et       serv_duplex_mode
 )
{
	son_bool_et ret = SON_FALSE;
	SON_UT_TRACE_ENTER();

	if((SON_TDD_MODE == serv_duplex_mode) &&
			(p_hash_rec->neighbor_info.bitmask &
			 (SON_UL_EARFCN_PRESENT|SON_PRACH_CONFIGURATION_PRESENT)) &&
			(anr_get_earfcn(p_anr_cell_ctxt) ==
			 p_hash_rec->neighbor_info.ul_earfcn) &&
			(anr_get_earfcn(p_anr_cell_ctxt) ==
			 p_hash_rec->neighbor_info.earfcn)   &&
			(p_hash_rec->neighbor_info.prach_config.bitmask &
			 SON_PRACH_CONFIG_INDEX_PRESENT))
	{
		ret = SON_TRUE; 
	}

	SON_UT_TRACE_EXIT();
	return ret;
}
/*****************************************************************************
 * Function Name  : is_nbr_rec_maps_to_the_root_seq_idx
 * Inputs         : p_anr_cell_ctxt   - Pointer to the serving cell context
 with ANR.
 *                : p_hash_rec        - Pointer to the intra rat hash record
 *                : srv_duplex_mode   - Duplex mode of the serving cell.
 *                                          (FDD/TDD)
 * Outputs        : 
 * Returns        : son_bool_et  (SON_TRUE/SON_FALSE) 
 * Description    : This function identifies if the neighbor record is 
 suitable to be considered for the determination of                              new root sequence index for the serving cell or not.
 ****************************************************************************/
static son_bool_et
is_nbr_rec_maps_to_the_root_seq_idx
(
 anr_cell_context_t       *p_anr_cell_ctxt,
 anr_intra_rat_hash_rec_t *p_hash_rec,
 son_duplex_mode_et       srv_duplex_mode
 )
{
	son_bool_et ret  = SON_FALSE;

	if(anr_get_earfcn(p_anr_cell_ctxt) == p_hash_rec->neighbor_info.earfcn &&
			(p_hash_rec->neighbor_info.bitmask & 
			 (SON_UL_EARFCN_PRESENT|SON_PRACH_CONFIGURATION_PRESENT)) &&
			(p_hash_rec->neighbor_info.prach_config.
			 prach_frequency_offset == 
			 p_anr_cell_ctxt->prach_configuration.prach_frequency_offset))
	{
		if(SON_FDD_MODE == srv_duplex_mode)
		{
			if (p_anr_cell_ctxt->choice_eutra_mode.fdd_info.ul_earfcn 
					== p_hash_rec->neighbor_info.ul_earfcn)
			{
				ret = SON_TRUE;
			}
		}
		else if ((anr_get_earfcn(p_anr_cell_ctxt) == 
					p_hash_rec->neighbor_info.ul_earfcn) &&
				(p_hash_rec->neighbor_info.prach_config.bitmask &
				 SON_PRACH_CONFIG_INDEX_PRESENT) &&
				(p_anr_cell_ctxt->prach_configuration.prach_configuration_index 
				 == p_hash_rec->neighbor_info.prach_config.
				 prach_configuration_index))
		{
			ret = SON_TRUE;
		}
	}
	return ret;
}
/*****************************************************************************
 * Function Name  : get_neighbor_record_from_nrt
 * Inputs         : p_cell_ctxt         - Pointer to the serving cell context
 with ANR.
 *                : p_strongest_cell_reported - strongest cell reported
 *                : field_to_be_tuned   - Paramter for which tuning is 
 being done when interference is 
 observed.
 *                : serv_duplex_mode    - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 * Outputs        : 
 * Returns        : anr_intra_rat_hash_rec_t * -Pointer to the intra rat
 neighbor record in the NRT.
 * Description    : This is the function provides the pointer to the 
 intra rat neighbor record from the NRT.                 
 ****************************************************************************/
anr_intra_rat_hash_rec_t *
get_neighbor_record_from_nrt
(
 anr_cell_context_t              *p_anr_cell_ctxt,
 rach_opt_strongest_cell_info_t  *p_strongest_cell_reported,
 son_rach_opt_inteference_val_et  field_to_be_tuned,
 son_duplex_mode_et               serv_duplex_mode
 )
{
	anr_intra_rat_hash_rec_t        *p_hash_rec     = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl    = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_anr_cell_ctxt || SON_PNULL == p_strongest_cell_reported 
			|| RACH_OPT_INTERFERENCE_NUM_OF_UPDATES <= field_to_be_tuned ||
			serv_duplex_mode >= SON_MAX_NUM_OF_DUPLEX_MODES)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Algorithm"
				"shall not be triggered as atleast one of the pointers "
				"(p_anr_cell_ctxt or p_strongest_cell_reported) is SON_PNULL"
				"or the value of the field to be tuned is"
				"[%d]which is more than or equal to [%d] and"
				"duplex modes is [%d]",field_to_be_tuned,
				RACH_OPT_INTERFERENCE_NUM_OF_UPDATES,serv_duplex_mode);

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(
			&p_anr_cell_ctxt->intra_rat_neighbor_table);

	while(p_intra_tpl)
	{
		if (RACH_OPT_INTERFERENCE_UPDATE_ROOT_SEQ == field_to_be_tuned)
		{
			if ((p_intra_tpl->data.neighbor_info.pci == 
						p_strongest_cell_reported->pci) &&
					SON_TRUE == is_nbr_rec_maps_to_the_root_seq_idx(
						p_anr_cell_ctxt,&p_intra_tpl->data,serv_duplex_mode))
			{
				p_hash_rec = &p_intra_tpl->data;
				break;
			}
		}
		else if (RACH_OPT_INTERFERENCE_UPDATE_PRACH_FREQ_OFFSET == 
				field_to_be_tuned)
		{
			if ((p_intra_tpl->data.neighbor_info.pci ==
						p_strongest_cell_reported->pci) &&
					SON_TRUE == is_nbr_rec_maps_to_the_prach_freq_offset
					(p_anr_cell_ctxt,&p_intra_tpl->data,serv_duplex_mode))
			{
				p_hash_rec = &p_intra_tpl->data;
				break;
			}
		}
		else if (RACH_OPT_INTERFERENCE_UPDATE_PRACH_CONFIG_IDX 
				== field_to_be_tuned)
		{
			if ((p_intra_tpl->data.neighbor_info.pci ==
						p_strongest_cell_reported->pci) &&
					SON_TRUE == is_nbr_rec_maps_to_the_prach_config_idx
					(p_anr_cell_ctxt,&p_intra_tpl->data,serv_duplex_mode))
			{
				p_hash_rec = &p_intra_tpl->data;
				break;
			}
		}
		else
		{
			break;
		}
		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				&p_anr_cell_ctxt->intra_rat_neighbor_table,
				&p_intra_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
	return p_hash_rec;
}
/* SPR-18735 START */
/*****************************************************************************
 * Function Name  : rach_opt_determine_root_seq
 * Inputs         : p_anr_cell_ctxt    - Pointer to the cell context at ANR
 Module.
 *                : serv_duplex_mode   - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 *                : serv_root_seq_idx_count - Number of root sequence indexes 
 *                  for serving cell 
 * Outputs        : 
 * Returns        : son_u16 
 * Description    : This function is used to determine the root sequence 
 index for the serving cell
 ****************************************************************************/
/* SPR-18735 END */    
static son_u16
rach_opt_determine_root_seq
(
 anr_cell_context_t   *p_anr_cell_ctxt,
    son_duplex_mode_et   serv_duplex_mode, 
/* SPR-18735 START */
    son_u8               serv_root_seq_idx_count
/* SPR-18735 END */    
 )
{
	anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl        =  SON_PNULL;
/* SPR-18735 START */
    son_bool_et                     is_root_seq_used   =  SON_FALSE;
/* SPR-18735 END */    
	son_u16                         idx                 = 0;
	son_u16                         new_root_seq_idx    = 
		SON_MAX_NUM_OF_ROOT_SEQS;


/* SPR-18735 START */
    son_u8 index = SON_NULL;
    son_u16 index_1 = SON_NULL,
            outer_loop = SON_NULL,
            inner_loop = SON_NULL;
    son_u8 nbr_root_seq_idx_count = SON_NULL;
	son_u8 existing_root_seq_array[SON_MAX_NUM_OF_ROOT_SEQS] 
        =   {SON_ROOT_SEQ_NOT_USED};
    son_u16 free_index_list[SON_MAX_NUM_OF_ROOT_SEQS] = {SON_NULL};
    son_u16 free_index_list_size = SON_NULL;

	SON_UT_TRACE_ENTER();
    SON_MEMSET(existing_root_seq_array,SON_ROOT_SEQ_NOT_USED,
			sizeof(existing_root_seq_array));

	p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(
			&p_anr_cell_ctxt->intra_rat_neighbor_table);

	while(p_intra_tpl)
	{
		if (SON_TRUE == is_nbr_rec_maps_to_the_root_seq_idx(
					p_anr_cell_ctxt,&p_intra_tpl->data,
					serv_duplex_mode))
		{
            nbr_root_seq_idx_count = 
                son_rach_opt_get_number_of_root_seq_index(
                        serv_duplex_mode, p_intra_tpl->data.neighbor_info.prach_config);
            if (SON_NULL == nbr_root_seq_idx_count)
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name,SON_ERROR,
                        "Invalid number of root" 
                        " sequences received for the neighbor Cell 0x%x]."
                        "For existing_root_seq_array, assigning minimum number of" 
                        " root sequence index = %d for this neighbor",
                        son_convert_char_cell_id_to_int_cell_id(
                            p_intra_tpl->data.neighbor_info.nbr_cgi.cell_identity),
                        SON_MIN_NUM_OF_ROOT_SEQS);
                nbr_root_seq_idx_count = SON_MIN_NUM_OF_ROOT_SEQS;
            }
            is_root_seq_used = SON_TRUE;
            /* update the USED root sequence index(s) in list
             * of existing RSI */
            for(index = SON_NULL;
                    index < nbr_root_seq_idx_count; index++)
            {
                index_1 = (p_intra_tpl->data.neighbor_info.prach_config.
                        root_sequence_index + index) % SON_MAX_NUM_OF_ROOT_SEQS;
                existing_root_seq_array[index_1] = 
                    SON_ROOT_SEQ_USED;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_INFO,
					"Root Sequence in use: [%d]", index_1);
            }
		}        
		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				&p_anr_cell_ctxt->intra_rat_neighbor_table,
				&p_intra_tpl->anchor);
	}

    if (SON_TRUE  == is_root_seq_used)
    {
        /* update the free_index_list with the
         * root sequence index values from
         * existing_root_seq_array which
         * satisfy the condition of contiguous RSI
         * values >= total number of RSI of
         * serving cell */ 
        for(outer_loop = SON_NULL;  outer_loop < SON_MAX_NUM_OF_ROOT_SEQS ;outer_loop++)
        {
            for(inner_loop = SON_NULL; inner_loop< serv_root_seq_idx_count ;inner_loop++)
            {
                index_1 =  (outer_loop + inner_loop) % SON_MAX_NUM_OF_ROOT_SEQS;

                if(SON_ROOT_SEQ_USED ==  existing_root_seq_array[index_1])
                {
                    break;
                }
            }

            if(inner_loop == serv_root_seq_idx_count)
	{
                free_index_list[free_index_list_size] = outer_loop;
                free_index_list_size++;
            }
        }

        if(free_index_list_size > 0)
		{
            /* extracting random index from
             * free_index_list */
              /* SPR 17753 Fix Start */
            idx = son_anr_get_random_val(SON_NULL, free_index_list_size-1);
              /* SPR 17753 Fix End */
            new_root_seq_idx = free_index_list[idx];
            SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                    SON_INFO,
                    "Random index from free_list selected is: %d, "
                    "Root sequence selected is: %d",
                    idx, new_root_seq_idx);
        }
        else
        {
            for(index_1 = 0; index_1 < SON_MAX_NUM_OF_ROOT_SEQS; index_1++)
            {
                if(SON_ROOT_SEQ_NOT_USED == existing_root_seq_array[index_1])
			{
				new_root_seq_idx = idx;
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
						p_son_rach_opt_facility_name,SON_INFO,
						"New root"
						"sequence shall be: [%d] for the "
						"serving Cell 0x%x",
						new_root_seq_idx,
						son_convert_char_cell_id_to_int_cell_id
						(p_anr_cell_ctxt->cell_id.cell_identity));
				break;
			}
		}
            new_root_seq_idx = index_1;
        }
	}

	if (SON_MAX_NUM_OF_ROOT_SEQS == new_root_seq_idx)
	{
        if (SON_FALSE ==  is_root_seq_used)
            /* SPR-18735 END */
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_WARNING,
					"New root sequence"
					"can not be determied as for the serving cell"
					"Cell 0x%x there exists no intra rat"
					"neighbor that meets the credentials with"
					"that of the serving cell",
					son_convert_char_cell_id_to_int_cell_id(
						p_anr_cell_ctxt->cell_id.cell_identity));
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_INFO,
					"New root sequence"
					"can not be determied as for the serving cell"
					"Cell 0x%x all the root sequences"
					"all already used by neighbors",
					son_convert_char_cell_id_to_int_cell_id(
						p_anr_cell_ctxt->cell_id.cell_identity));
		}
	}

	SON_UT_TRACE_EXIT();
	return new_root_seq_idx;
}

/*****************************************************************************
 * Function Name  : get_transmission_bandwidth
 * Inputs         : p_anr_cell_ctxt  - Pointer to the ANR cell context.
 *                : p_rach_algo_counters - Pointer to the rach algo counters
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This function is used to determine the prach frequency
 offset.
 ****************************************************************************/
static son_u8
get_transmission_bandwidth
(
 son_choice_eutra_mode_t *p_duplex_mode
 )
{
	son_u8 tx_bw = SON_MAX_NUM_OF_TRANSMISSION_BW;
	if (p_duplex_mode->bitmask & SON_EUTRA_MODE_FDD)
	{
		tx_bw = p_duplex_mode->fdd_info.ul_trnsmission_bdwidth;  
	}
	else
	{
		tx_bw = p_duplex_mode->tdd_info.trnsmission_bdwidth; 
	}
	return  tx_bw; 
}
/*****************************************************************************
 * Function Name  : get_max_prach_freq_offset_available
 * Inputs         : p_anr_cell_ctxt  - Pointer to the ANR cell context.
 *                : p_rach_algo_counters - Pointer to the rach algo counters
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This function is used to determine the prach frequency
 offset.
 ****************************************************************************/
static son_u8
get_max_prach_freq_offset_available
(
 anr_cell_context_t          *p_anr_cell_ctxt,
 rach_opt_cell_context_t     *p_cell_ctxt
 )
{
    /* SPR-19168 START */
    son_u8 max_value = SON_NULL;
	son_u8 no_of_rbs = 0;  
	son_u8 transmission_bw = get_transmission_bandwidth(
			&p_anr_cell_ctxt->choice_eutra_mode);
    son_float pusch_hopping_offset_float = 0.0;
    son_u8 pusch_hopping_offset = SON_NULL;

	switch(transmission_bw)
	{
		case SON_TRANSMISSION_BW_6_RBS:
            {
                no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_6RB_CELL;
            }
			break;
		case SON_TRANSMISSION_BW_15_RBS:
			{
				no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_15RB_CELL;
			}
			break;
		case SON_TRANSMISSION_BW_25_RBS:
			{
				no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_25RB_CELL;
			}
			break;
		case SON_TRANSMISSION_BW_50_RBS:
			{
				no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_50RB_CELL;
			}
			break;
		case SON_TRANSMISSION_BW_75_RBS:
			{
				no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_75RB_CELL;
			}
			break;
		case SON_TRANSMISSION_BW_100_RBS:
			{
				no_of_rbs = SON_MAX_PRB_OFFSET_VALUE_100RB_CELL;
			}
			break;
		default:
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name,SON_ERROR,
					"Invalid bandwidth %d", transmission_bw);
			break;
	}

	/*The max value for the offset shall be greater than 1.
	  Cell of Size1 1.4 Mhz is not taken into consideration*/

    pusch_hopping_offset_float = (son_float)((son_float)p_cell_ctxt->pusch_hopping_offset / (son_float)2);
    pusch_hopping_offset = (son_u8)SON_CEIL(pusch_hopping_offset_float);
    /* maximum value = BW_IN_RBs - ceiling(PUSCH_hopping_offset / 2) - NUM_OF_CONSECUTIVE_PRBs_FOR_RACH(6) */ 
    /* For example, when BW=20MHz, PUSCH_hopping_offset = 10RB,
       Resource Block No.0,1,2,3,4 will be needed for PUCCH. So, 
       Resource Block No 5 is as the minimum Prach_frequency_offset candidate value.
       Resource Block No 95,96,97,98,99 will be needed for PUCCH. 
       Resource Block No.89 is as the maximum Prach_frequency_offset candidate value. 
       (Resource Block No 89,90,91,92,93,94 will be used for PRACH) */
	if (no_of_rbs >= 
           (pusch_hopping_offset + SON_NUM_OF_CONSECUTIVE_PRB))
	{
        max_value = no_of_rbs - pusch_hopping_offset
            - SON_NUM_OF_CONSECUTIVE_PRB;
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_INFO,
				"Upper limit offset value "
                "no_of_rbs[%d] - (pusch_hopping_offset/2[%d] + SON_NUM_OF_CONSECUTIVE_PRB[%d]) is coming out to be less than 0",
                no_of_rbs, pusch_hopping_offset, SON_NUM_OF_CONSECUTIVE_PRB);
	}
    /* SPR-19168 END */
	return max_value;
}
/*****************************************************************************
 * Function Name  : rach_opt_determine_prach_freq_offset
 * Inputs         : p_anr_cell_ctxt  - Pointer to the ANR cell context.
 *                : p_cell_ctxt       - Pointer to the RACH cell context.
 *                : serv_duplex_mode  - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 * Outputs        : 
 * Returns        : son_u8 
 * Description    : This function is used to determine the prach frequency
 offset.
 ****************************************************************************/
static son_u8
rach_opt_determine_prach_freq_offset
(
 anr_cell_context_t             *p_anr_cell_ctxt,
 rach_opt_cell_context_t        *p_cell_ctxt,
 son_duplex_mode_et             serv_duplex_mode
 )
{
	anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl            =  SON_PNULL;
	son_bool_et                     is_prach_freq_avail     =  SON_FALSE;
	son_u8 new_prach_freq_offset = SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS;
	son_u8 min_prach_freq_offset = 0;
	son_u8 max_prach_freq_offset = 0;
	son_u8 idx = 0;
    /* SPR-19168 START */
    son_float pusch_hopping_offset_float = 0.0;
    /* SPR-19168 END */

    /* SPR-19168 START */
	son_u8 existing_prach_freq_offset_array[SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS] 
        =   {SON_PRACH_FREQ_OFFSET_NOT_USED};
    /* SPR-19168 END */

	SON_UT_TRACE_ENTER();
    /* SPR-19168 START */
	SON_MEMSET(existing_prach_freq_offset_array,
                SON_PRACH_FREQ_OFFSET_NOT_USED,
			sizeof(existing_prach_freq_offset_array));
    /* SPR-19168 END */

	p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(
			&p_anr_cell_ctxt->intra_rat_neighbor_table);

	while(p_intra_tpl)
	{
		if (SON_TRUE == is_nbr_rec_maps_to_the_prach_freq_offset(
					p_anr_cell_ctxt,&p_intra_tpl->data,
					serv_duplex_mode))
		{
			is_prach_freq_avail = SON_TRUE;
            /* SPR-19168 START */
			existing_prach_freq_offset_array[p_intra_tpl->data.
				neighbor_info.prach_config.prach_frequency_offset] = 
                SON_PRACH_FREQ_OFFSET_USED; 
            /* SPR-19168 END */
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_BRIEF,
					"In use prach frequency offset: [%d]",
					p_intra_tpl->data.neighbor_info.
					prach_config.prach_frequency_offset);
		}       

		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				&p_anr_cell_ctxt->intra_rat_neighbor_table,
				&p_intra_tpl->anchor);
	}

	if (SON_TRUE == is_prach_freq_avail)
	{
        /* SPR-19168 START */
        /* minimum value = ceiling(PUSCH_hopping_offset / 2) */
        pusch_hopping_offset_float = (son_float)((son_float)p_cell_ctxt->pusch_hopping_offset / (son_float)2);
        min_prach_freq_offset = (son_u8)SON_CEIL(pusch_hopping_offset_float);        

        /* SPR-19168 END */

		max_prach_freq_offset = get_max_prach_freq_offset_available
			(p_anr_cell_ctxt,p_cell_ctxt);

		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_INFO,
				"For the serving Cell 0x%x of "
				"UL Bandwidth:[%d] and PUSCH Hopping Offset:[%d], "
				"Minimum prach frequeny offset is:[%d] & "
				"Maximum prach frequecy offset is:[%d]",
				son_convert_char_cell_id_to_int_cell_id
				(p_anr_cell_ctxt->cell_id.cell_identity),
				get_transmission_bandwidth(
					&p_anr_cell_ctxt->choice_eutra_mode),
				p_cell_ctxt->pusch_hopping_offset,
				min_prach_freq_offset,max_prach_freq_offset);

        /* SPR-19168 START */
        if (min_prach_freq_offset <  max_prach_freq_offset &&
				p_anr_cell_ctxt->prach_configuration.prach_frequency_offset
				>= min_prach_freq_offset && p_anr_cell_ctxt->prach_configuration.
				prach_frequency_offset <= max_prach_freq_offset)
        /* SPR-19168 END */
		{
			idx = (p_anr_cell_ctxt->prach_configuration.
					prach_frequency_offset + 1)
				%SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS;

			for(    ;
					idx != p_anr_cell_ctxt->prach_configuration.
					prach_frequency_offset;
					idx = (idx+1)%SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS)
			{
                /* SPR-19168 START */
                if (SON_PRACH_FREQ_OFFSET_NOT_USED ==
						existing_prach_freq_offset_array[idx] &&
						idx >= min_prach_freq_offset &&
						idx <= max_prach_freq_offset)
                /* SPR-19168 END */
				{
					new_prach_freq_offset = idx;
					SON_LOG(g_rach_opt_gb_ctxt.log_mode,
							p_son_rach_opt_facility_name,SON_INFO,
							"New prach "
							"frequecy shall be %d for the "
							"serving Cell 0x%x",
							new_prach_freq_offset,
							son_convert_char_cell_id_to_int_cell_id
							(p_anr_cell_ctxt->cell_id.cell_identity));
					break;
				}
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_INFO,
					"Since serving Cell 0x%x of "
					"UL Bandwidth:[%d] has frequency attributes as:-"
					"Prach Frequency Offset is:[%d], "
					"Minimum prach frequeny offset is:[%d] & "
					"Maximum prach frequecy offset is:[%d]."
					" The new prach Frequecy offset can not be determined",
					son_convert_char_cell_id_to_int_cell_id
					(p_anr_cell_ctxt->cell_id.cell_identity),
					get_transmission_bandwidth(&p_anr_cell_ctxt->choice_eutra_mode),
					p_anr_cell_ctxt->prach_configuration.prach_frequency_offset,       
					min_prach_freq_offset, max_prach_freq_offset);
		}
	}

	SON_UT_TRACE_EXIT();
	return new_prach_freq_offset;
}
/*****************************************************************************
 * Function Name  : get_upper_prach_config_idx_value_for_given_preamble_offset
 * Inputs         : preamble_format   - Preamble format of the serving cell. 
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This function is used to determine the maximum prach
 configuration index for the given preamble format
 ****************************************************************************/
static son_u8
get_upper_prach_config_idx_value_for_given_preamble_offset
(
 preamble_format_et preamble_format,
 son_u8      *p_lower_prach_config_idx
 )
{
	son_u8 upper_limit = 
		MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_INVALID_PREAMBLE_FORMAT;
	switch(preamble_format)
	{
		case PREAMBLE_FORMAT_0:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_0;
			*p_lower_prach_config_idx = 0;
			break;
		case PREAMBLE_FORMAT_1:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_1;
			*p_lower_prach_config_idx =
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_0 + 1;
			break;
		case PREAMBLE_FORMAT_2:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_2;
			*p_lower_prach_config_idx =
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_1 + 1;
			break;
		case PREAMBLE_FORMAT_3:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_3;
			*p_lower_prach_config_idx =
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_2 + 1;
			break;
		case PREAMBLE_FORMAT_4:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_4;
			*p_lower_prach_config_idx =
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_3 + 1;
			break;
		case PREAMBLE_FORMAT_NA:
			upper_limit = 
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_NA;
			*p_lower_prach_config_idx =
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_4 + 1;
			break;
		default:
			break;
	}
	return upper_limit;
}
/*****************************************************************************
 * Function Name  : get_lower_prach_config_idx_value_for_given_preamble_offset
 * Inputs         : lower_limit          - Lower prach configuration index
 with the preamable format similar
 to that of the serving cell.
 *                : p_serving_cell_prach_config_info - Pointer to the rach 
 configuration data for the serving
 cell.
 * Outputs        : 
 * Returns        : son_u8
 * Description    : This function determines least prach configuration index
 with the preamble format and density  
 similar to that of serving cell.
 ****************************************************************************/
static son_u8
get_lower_prach_config_idx_value_for_given_preamble_offset
(
 son_u8                                   lower_limit,
 const frame_type_2_ra_config_data_for_preamble 
 *p_serving_cell_prach_config_info
 )
{
	son_u8 index = 0;
	son_u8 return_value = SON_MAX_PRACH_CONFIG_IDX;
	SON_UT_TRACE_ENTER();
	for(index = lower_limit; index < SON_MAX_PRACH_CONFIG_IDX ;index++)
	{
		if ((p_serving_cell_prach_config_info->preamble_format ==
					RA_CONFIG_TDD_DATA_FOR_PREAMBLE[index].preamble_format) &&
				(p_serving_cell_prach_config_info->density_per_10_ms
				 == RA_CONFIG_TDD_DATA_FOR_PREAMBLE[index].
				 density_per_10_ms))
		{
			return_value = index;
			break;
		}
	}

	SON_UT_TRACE_EXIT();
	return return_value;
}
/*****************************************************************************
 * Function Name  : rach_opt_determine_prach_config_idx
 * Inputs         : p_anr_cell_ctxt  - Pointer to the ANR cell context.
 *                : p_cell_ctxt      - Pointer to the RACH cell context.
 *                : serv_duplex_mode - duplex mode of the serving cell
 *                                          (FDD/TDD)
 *                : p_serving_cell_prach_config_info -Pointer to the 
 Prach Configuration information block.
 *                : serving_cells_prach_config_idx - Serving Cell Prach
 Configuration Index.
 * Outputs        : 
 * Returns        : son_u8 
 * Description    : This function is used to determine the prach configuration
 index.
 ****************************************************************************/
static son_u8
rach_opt_determine_prach_config_idx
(
 anr_cell_context_t          *p_anr_cell_ctxt,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_duplex_mode_et         serv_duplex_mode,
 const frame_type_2_ra_config_data_for_preamble *
 p_serving_cell_prach_config_info,
 son_u8 serving_cells_prach_config_idx
 )
{
	anr_intra_rat_hash_nr_tuple_t   *p_intra_tpl                =  SON_PNULL;
	son_bool_et                     is_prach_config_idx_avail   =  SON_FALSE;
	son_u8 new_prach_config_idx = SON_MAX_PRACH_CONFIG_IDX;
	son_u8                       idx = 0;
	son_u8 lower_limit_for_prach_config_idx = 0;
	son_u8 upper_limit_for_prach_config_idx = 0;

    /* SPR-19168 START */
	son_u8 existing_prach_config_idx_array[SON_MAX_PRACH_CONFIG_IDX] 
        =   {SON_PRACH_CONFIG_IDX_NOT_USED};
    /* SPR-19168 END */

	SON_UT_TRACE_ENTER();
    /* SPR-19168 START */
	SON_MEMSET(existing_prach_config_idx_array,
                    SON_PRACH_CONFIG_IDX_NOT_USED,
			sizeof(existing_prach_config_idx_array));
    /* SPR-19168 END */

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
			p_son_rach_opt_facility_name,SON_BRIEF,
			"The Serving Cell  "
			"Cell 0x%x has the Prach Configuration index:[%d], "
			"Prach Preamble Format:[%d] & Density per 10 ms:[%f]. ",
			son_convert_char_cell_id_to_int_cell_id(
				p_cell_ctxt->cell_id.cell_identity),
			serving_cells_prach_config_idx,
			p_serving_cell_prach_config_info->preamble_format,
			p_serving_cell_prach_config_info->density_per_10_ms);

	p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(
			&p_anr_cell_ctxt->intra_rat_neighbor_table);

	while(p_intra_tpl)
	{
		if (SON_TRUE == is_nbr_rec_maps_to_the_prach_config_idx(
					p_anr_cell_ctxt,&p_intra_tpl->data,
					serv_duplex_mode))
		{
			is_prach_config_idx_avail = SON_TRUE;
            /* SPR-19168 START */
			existing_prach_config_idx_array[p_intra_tpl->data.
				neighbor_info.prach_config.prach_configuration_index] = 
                SON_PRACH_CONFIG_IDX_USED;
            /* SPR-19168 END */

			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_BRIEF,
					"In use Prach Configuration Index is: [%d]",
					p_intra_tpl->data.neighbor_info.
					prach_config.prach_configuration_index);
		}       

		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				&p_anr_cell_ctxt->intra_rat_neighbor_table,
				&p_intra_tpl->anchor);
	}

	if (SON_TRUE == is_prach_config_idx_avail)
	{
		upper_limit_for_prach_config_idx =
			get_upper_prach_config_idx_value_for_given_preamble_offset(
					p_serving_cell_prach_config_info->preamble_format,
					&lower_limit_for_prach_config_idx);

		SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
				p_son_rach_opt_facility_name,SON_INFO,
				"For Serving Cell "
				"0x%x of Prach Configuration index:[%d] & preamble format:[%d] "
				"the maximum possible Prach Configuration index is:[%d]. ",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity),
				serving_cells_prach_config_idx,
				p_serving_cell_prach_config_info->preamble_format,
				upper_limit_for_prach_config_idx);

		if (upper_limit_for_prach_config_idx !=
				MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_INVALID_PREAMBLE_FORMAT)
		{
			lower_limit_for_prach_config_idx =
				get_lower_prach_config_idx_value_for_given_preamble_offset(
						lower_limit_for_prach_config_idx,
						p_serving_cell_prach_config_info);
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_BRIEF,
					"For Serving "
					"Cell 0x%x of Prach Configuration index:[%d], "
					"preamble format:[%d] & Density per 10 ms:[%f], "
					"the minimum possible Prach Configuration index is:[%d]. ",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity),
					serving_cells_prach_config_idx,
					p_serving_cell_prach_config_info->preamble_format,
					p_serving_cell_prach_config_info->density_per_10_ms,
					lower_limit_for_prach_config_idx);

			for(idx = lower_limit_for_prach_config_idx;
					idx <= 
					upper_limit_for_prach_config_idx
					;idx++)
			{
                /* SPR-19168 START */
                if (SON_PRACH_CONFIG_IDX_NOT_USED == 
						existing_prach_config_idx_array[
						idx])
                    /* SPR-19168 END */
				{
					new_prach_config_idx = idx;
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name,SON_INFO,
							"New proposed Prach configuration index is: [%d]",
							new_prach_config_idx);
					break;
				}
			}
		} 
	}

	if (SON_MAX_PRACH_CONFIG_IDX == new_prach_config_idx)
	{
		if (SON_TRUE == is_prach_config_idx_avail)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_WARNING,
					"All possible Prach Configuration Indexes "
					"from the set [%d - %d] are taken "
					"by EUTRAN neighbors",
					lower_limit_for_prach_config_idx,
					upper_limit_for_prach_config_idx);
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,SON_WARNING,
					"No "
					"EUTRAN neighbor exists with the configuration "
					"required for the prach configuration index "
					"determination");
		}
	}

	SON_UT_TRACE_EXIT();
	return new_prach_config_idx;
}
/*****************************************************************************
 * Function Name  : update_local_counters_and_reset_msg_counters
 * Inputs         : optimization_type - Optimization type determined
 * Outputs        : p_cell_ctxt       - Pointer to the serving cell context.
 * Returns        : son_void_t 
 * Description    : This function shall update/resets the rach counters 
 maintained for each serving cell. 
 ****************************************************************************/
static son_void_t
update_local_counters_and_reset_msg_counters
(
 rach_opt_cell_context_t                         *p_cell_ctxt,
 son_rach_opt_identified_optimization_type_et    optimization_type
 )
{
	SON_UT_TRACE_ENTER();

	/*Update local counters*/
	p_cell_ctxt->is_rach_algo_started = SON_TRUE;
	p_cell_ctxt->is_cell_info_ind_awaited = SON_TRUE;
	p_cell_ctxt->rach_algo_run_count_after_started++;
	p_cell_ctxt->previous_rach_failure_percentage =
		p_cell_ctxt->rach_algo_counters.rach_failure_percentage;
	p_cell_ctxt->previous_optimization_type =
		optimization_type;

	/*reset the counters related to msgs*/
	SON_MEMSET(&p_cell_ctxt->rach_algo_counters,
			0, sizeof(p_cell_ctxt->rach_algo_counters));
	p_cell_ctxt->
		is_algo_counters_updated_from_rrm_ue_report
		= SON_FALSE;
	p_cell_ctxt->
		is_algo_counters_updated_from_rrm_l2_report
		= SON_FALSE;
	rach_opt_clear_strongest_nbr_cells_list(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
}

/* SPR-18735 START */
/*****************************************************************************
 * Function Name  : son_rach_opt_get_number_of_root_seq_index
 * Inputs         : serving_cell_duplex_mode - Duplex Mode of Serving Cell  
                    prach_configuration - prach_configuration
 * Outputs        : None
 * Returns        : num_of_root_seq_indexes - Number of root sequence indexes 
 * Description    : This function shall find the number of root sequence indexes 
                    derived using prach_configuration_index, 
                    zero_correlation_zone_config and high_speed_flag.
 ****************************************************************************/

son_u8 
son_rach_opt_get_number_of_root_seq_index
(
 son_duplex_mode_et                   serving_cell_duplex_mode,
 son_prach_configuration_t            prach_configuration 
 )
{
    son_u16 nzc_val = NZC_VAL_0_3,
            ncs_val = SON_NULL,
            no_of_seq = SON_NULL; 
    son_u8  num_of_root_seq_indexes = SON_NULL; 
    son_float  num_of_root_seq_indexes_float = 0.0; 
    son_return_et ret_val = SON_SUCCESS;
    son_u8 prach_config_idx = 
        prach_configuration.prach_configuration_index;
    son_u8 zero_correlation_zone_config = 
        prach_configuration.zero_correlation_zone_config;
    son_u8 high_speed_flag =
        prach_configuration.high_speed_flag;

    const frame_type_1_ra_config_data_for_preamble
        * p_cell_prach_config_info = SON_PNULL;
    const frame_type_2_ra_config_data_for_preamble
        * p_cell_prach_config_info_tdd = SON_PNULL;
    const rach_opt_ncs_mapping_format_4
        * p_rach_opt_ncs_mapping_f4 = SON_PNULL;
    const rach_opt_ncs_mapping_format_0_3
        * p_rach_opt_ncs_mapping_f0_3 = SON_PNULL;
    
    if(SON_FDD_MODE == serving_cell_duplex_mode)
    {
        do
        {
            if(!(prach_configuration.bitmask & SON_PRACH_CONFIG_INDEX_PRESENT))
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name,SON_WARNING,
                        "As the bitmask " 
                        "for PRACH Config index is not present, not proceeding to " 
                        "find the preamble format in case of FDD "
                        "and selecting default NZC value : %d for calculation ",
                        nzc_val);
                break;
            }

            p_cell_prach_config_info =
                &RA_CONFIG_FDD_DATA_FOR_PREAMBLE[prach_config_idx];

            if(SON_PNULL == p_cell_prach_config_info)
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_WARNING,
                        "The Cell has the Prach Configuration Index:[%d] "
                        "but corresponding Prach Configuration Information "
                        "Block is NULL. ",
                        prach_config_idx);
                ret_val = SON_FAILURE;
                break;
            }
            if(PREAMBLE_FORMAT_NA  <= p_cell_prach_config_info->preamble_format)
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_WARNING,
                        "The Cell has Prach Configuration Index:[%d] "
                        "has Preamble format is:[%d] which is > Maximun allowed "
                        "Preamble format:[%d] ",
                        prach_config_idx,
                        p_cell_prach_config_info->preamble_format,
                        PREAMBLE_FORMAT_4);
                ret_val = SON_FAILURE;
                break;
            }
        }while(0);

    }
    else if(SON_TDD_MODE == serving_cell_duplex_mode)
    {
        do
        {
            if(!(prach_configuration.bitmask & SON_PRACH_CONFIG_INDEX_PRESENT))
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name,SON_ERROR,
                        "As the bitmask" 
                        "for PRACH Config index is not present, root_seq_idx_count"
                        "cannot be calculated in case of TDD");
                ret_val = SON_FAILURE;
                break;
            }
            
            p_cell_prach_config_info_tdd =
                &RA_CONFIG_TDD_DATA_FOR_PREAMBLE[prach_config_idx];

            if(SON_PNULL == p_cell_prach_config_info_tdd)
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_WARNING,
                        "Cell has the Prach Configuration Index:[%d] "
                        "but corresponding Prach Configuration Information "
                        "Block is NULL. ",
                        prach_config_idx);
                ret_val = SON_FAILURE;
                break;
            }

            if(PREAMBLE_FORMAT_NA  <= p_cell_prach_config_info_tdd->preamble_format)
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_WARNING,
                        "Cell has Prach Configuration Index:[%d] "
                        "has Preamble format is:[%d] which is > Maximun allowed "
                        "Preamble format:[%d] ",
                        prach_config_idx, 
                        p_cell_prach_config_info_tdd->preamble_format,
                        PREAMBLE_FORMAT_4);
                ret_val = SON_FAILURE;
                break;
            }

            if(PREAMBLE_FORMAT_4 == p_cell_prach_config_info_tdd->preamble_format)
            {
                nzc_val = NZC_VAL_4;

                p_rach_opt_ncs_mapping_f4 = 
                    &NCS_FOR_PREAMBLE_GENERATION_FORMAT_4[zero_correlation_zone_config];

                ncs_val = p_rach_opt_ncs_mapping_f4->ncs_val;
            }
        }while(0);
    }
    if(SON_SUCCESS == ret_val)
    {
        if(NZC_VAL_0_3 == nzc_val)
        {
            p_rach_opt_ncs_mapping_f0_3 = 
                &NCS_FOR_PREAMBLE_GENERATION_FORMAT_0_3[zero_correlation_zone_config]; 
            if(HIGH_SPEED_FLAG_UNRESTRICTED == high_speed_flag)
            {
                ncs_val = p_rach_opt_ncs_mapping_f0_3->ncs_val_unrestricted;
            }
            else if(HIGH_SPEED_FLAG_RESTRICTED == high_speed_flag)
            {
                ncs_val = p_rach_opt_ncs_mapping_f0_3->ncs_val_restricted;
            }

        }
        if(SON_NCS_NOT_APPLICABLE == ncs_val)
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                    SON_WARNING,
                    "Ncs Value derived is"
                    "not applicable. The calculation of root_sequence_indexes cannot continue");

            ret_val = SON_FAILURE;
        }
    }
    if(SON_SUCCESS == ret_val)
    {
        if(HIGH_SPEED_FLAG_UNRESTRICTED == high_speed_flag)
        {
            if(0 == ncs_val)
            {
                num_of_root_seq_indexes = 1;
            }
            else
            {
                no_of_seq = nzc_val/ncs_val;
                num_of_root_seq_indexes_float = (son_float)((son_float)SON_MAX_PRACH_CONFIG_IDX/(son_float)no_of_seq);
                num_of_root_seq_indexes = (son_u8)SON_CEIL(num_of_root_seq_indexes_float);
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                        SON_DETAILED,
                        "num_of_root_seq_indexes calculated = %d",
                        num_of_root_seq_indexes);
            }
        }
        else if(HIGH_SPEED_FLAG_RESTRICTED == high_speed_flag)
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                    SON_WARNING,
                    "The High Speed Flag value"
                    "is set to Resticted. The calculation of root_sequence_indexes cannot continue");
        }
    }
    return num_of_root_seq_indexes;

}

/*****************************************************************************
 * Function Name  : son_rach_opt_compare_root_seq_indx
 * Inputs         : serv_root_seq_index - Starting root_sequence_index of serving cell
                    neigh_root_seq_index - Starting root_sequence_index of neighbor cell
                    num_serv_root_seq_indexes - Number of root sequence indexes for serving cell
                    num_nbr_root_seq_indexes - Number of root sequence indexes for neighboring cell
 * Outputs        : None 
 * Returns       : match_found - If any of the root_sequence_index of serving cell matches with
                                  that of neighbor cell, 
                                  value returned is SON_TRUE and SON_FALSE otherwise.    
 * Description    : This function shall compare the serving cell's root_sequence_index with 
                    that of neighbor's root_sequence_index for equality. 
 ****************************************************************************/

son_u8
son_rach_opt_compare_root_seq_indx
(
 son_u16 serv_root_seq_index_start,
 son_u16 nbr_root_seq_index_start,
 son_u8 num_serv_root_seq_indexes,
 son_u8 num_nbr_root_seq_indexes
 )
{
    son_bool_et match_found = SON_FALSE; 
    son_u16  loop1 = SON_NULL,
             loop2 = SON_NULL;
    son_u16  index1 = SON_NULL,
             index2 = SON_NULL;

    for(loop1 = 0; loop1 < num_serv_root_seq_indexes; loop1++)
    {
        index1 = (serv_root_seq_index_start + loop1) % SON_MAX_NUM_OF_ROOT_SEQS;
        for(loop2 = 0; loop2 < num_nbr_root_seq_indexes; loop2++)
        {
            index2 = (nbr_root_seq_index_start + loop2) % SON_MAX_NUM_OF_ROOT_SEQS;
            if(index1 == index2)
            {
                match_found = SON_TRUE;
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name,SON_DETAILED,
                        "Serving "
                        "prach root sequence index:[%d] matches Neighbor prach "
                        "root sequence index:[%d]", index1,index2);
                break;
            }
        }
        if(match_found == SON_TRUE){
            break;
        }

    }
    return match_found;
}

/* SPR-18735 END */

/*****************************************************************************
 * Function Name  : son_rach_opt_tune_root_sequence_index
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context.
 *                : p_rach_algo_counters - Pointer to the rach algo counters.
 *                : p_anr_cell_ctxt_node - Pointer to the serving cell context
 in ANR Module.
 *                : p_strng_nbr_cell_node - Pointer to the strongest reported
 neighbor information 
 *                : strng_cell_count      - Count of strongest neighbors 
 reported.
 *                : serv_duplex_mode      - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 * Outputs        : None 
 * Returns        : son_void_t 
 * Description    : This function determines the new value for the 
 root sequence index for the serving cell in case of 
 interference issue.
 ****************************************************************************/
static son_return_et
son_rach_opt_tune_root_sequence_index
(
 rach_opt_cell_context_t             *p_cell_ctxt,
 anr_cell_context_node_t             *p_anr_cell_ctxt_node,
 rach_opt_strongest_nbr_cell_node_t  *p_strng_nbr_cell_node,
 son_u32                              strng_cell_count,
 son_duplex_mode_et                  serv_duplex_mode
 )
{
	son_u8                       idx = 0;
	anr_intra_rat_hash_rec_t     *p_nbr_rec = SON_PNULL;
	son_u16                      new_root_seq = SON_MAX_NUM_OF_ROOT_SEQS;
	son_bool_et                  is_nbr_with_strong_pci_avail = SON_FALSE;
    /* SPR-18735 START */
    son_u8                       num_serv_root_seq_indexes = SON_NULL,
                                 num_nbr_root_seq_indexes = SON_NULL;
    /* SPR-18735 END */

	SON_UT_TRACE_ENTER();
    /* SPR-18735 START */
    /* get the number of root_sequence_indexe(s) for serving cell */
    num_serv_root_seq_indexes = 
        son_rach_opt_get_number_of_root_seq_index(serv_duplex_mode, 
                p_anr_cell_ctxt_node->data.prach_configuration);
    if (SON_NULL == num_serv_root_seq_indexes)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "Invalid number of root" 
                "sequences received for the" 
                "serving Cell 0x%x not proceeding further"
                "to tune root_sequence_index",
                son_convert_char_cell_id_to_int_cell_id(
                    p_anr_cell_ctxt_node->data.cell_id.cell_identity));
        return SON_FAILURE;
    }        
    /* SPR-18735 END */

	for(idx = 0; idx < SON_MAX_NUM_OF_PCI_FOR_INTERFERENCE && 
			idx < strng_cell_count && SON_PNULL != p_strng_nbr_cell_node;
			idx++)
	{
		p_nbr_rec = get_neighbor_record_from_nrt(
				&p_anr_cell_ctxt_node->data,
				&p_strng_nbr_cell_node->data,
				RACH_OPT_INTERFERENCE_UPDATE_ROOT_SEQ,
				serv_duplex_mode);

		if(SON_PNULL == p_nbr_rec)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_WARNING,
					"For the optmization attempt[count:%d],It is "
					"observed that For Serving Cell "
					"0x%x with Duplex mode:[%s],There is no EUTRAN neighbor "
					"in NRT with following attribute values:-"
					"PCI:[%d], "
					"UL EARFCN:[%d],DL EARFCN:[%d] and prach frequency "
					"offset:[%d].Incase of TDD, "
					"prach configuration index:[%d]",
					p_cell_ctxt->
					rach_algo_run_count_after_started +1,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity),
					DUPLEX_MODE_OF_CELL[serv_duplex_mode],
					p_strng_nbr_cell_node->data.pci,
					anr_get_ul_earfcn(&p_anr_cell_ctxt_node->data),
					anr_get_earfcn(&p_anr_cell_ctxt_node->data),
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_frequency_offset,
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index);

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
			continue;
		}

		is_nbr_with_strong_pci_avail = SON_TRUE;

		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_DETAILED,
				"For Serving "
				"Cell id:0x%x neighbor Cell 0x%x "
				"of PCI %d has been found in NRT such that:-\n"
				"Serving dulpex mode %s equal  Neighbor duplex mode %s "
				"Serving UL EARFCN %d equal  Neighbor UL EARFCN %d "
				"Serving DL EARFCN %d equal Neighbor DL EARFCN %d "
				"Serving prach freq offset %d equal Neighbor prach "
				"freq offset %d",
				son_convert_char_cell_id_to_int_cell_id(
					p_anr_cell_ctxt_node->data.cell_id.cell_identity),
				son_convert_char_cell_id_to_int_cell_id(p_nbr_rec->
					neighbor_info.nbr_cgi.cell_identity),
				p_nbr_rec->neighbor_info.pci,
				DUPLEX_MODE_OF_CELL[serv_duplex_mode],
				DUPLEX_MODE_OF_CELL[serv_duplex_mode],
				anr_get_ul_earfcn(&p_anr_cell_ctxt_node->data),
				p_nbr_rec->neighbor_info.ul_earfcn,
				anr_get_earfcn(&p_anr_cell_ctxt_node->data),
				p_nbr_rec->neighbor_info.earfcn,
				p_anr_cell_ctxt_node->data.prach_configuration.
				prach_frequency_offset,
				p_nbr_rec->neighbor_info.prach_config.
				prach_frequency_offset);

		if(SON_TDD_MODE  ==  serv_duplex_mode)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving "
					"prach configuration index %d matches Neighbor prach "
					"configuration index:[%d]",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index,
					p_nbr_rec->neighbor_info.prach_config.
					prach_configuration_index);
		}

        /* SPR-18735 START */

        /* get the number of root_sequence_indexe(s) for strongest neighbor cell */
        num_nbr_root_seq_indexes = 
            son_rach_opt_get_number_of_root_seq_index(serv_duplex_mode, 
                    p_nbr_rec->neighbor_info.prach_config);
        if (SON_NULL == num_nbr_root_seq_indexes)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name,SON_ERROR,
                    "Invalid number of root" 
                    " sequences received for the neighbor Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_nbr_rec->neighbor_info.nbr_cgi.cell_identity));
        }
        /* compare the root sequence indexe(s) for equality */
        if(son_rach_opt_compare_root_seq_indx(
                    p_anr_cell_ctxt_node->data.prach_configuration.root_sequence_index,
                    p_nbr_rec->neighbor_info.prach_config.root_sequence_index,
                    num_serv_root_seq_indexes, 
                    num_nbr_root_seq_indexes))
        {
            /* SPR-18735 END */

            /* SPR-18735 START */
			if (SON_MAX_NUM_OF_ROOT_SEQS  == (new_root_seq =  
						rach_opt_determine_root_seq(
							&p_anr_cell_ctxt_node->data,
                            serv_duplex_mode, num_serv_root_seq_indexes)))
            /* SPR-18735 END */
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,SON_WARNING,
						"For the optmization attempt[count:%d],"
						" Serving Cell 0x%x of [%s]Duplex mode "
						"no root sequence index "
						"can be calculated",
						p_cell_ctxt->
						rach_algo_run_count_after_started +1,
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity),
						DUPLEX_MODE_OF_CELL[serv_duplex_mode]);

				SON_UT_TRACE_EXIT();
				return SON_FAILURE;
			}
			else
			{
				/*Send the new proposed value to MIF 
				  and update the counters.*/
				son_rach_opt_parameter_update_req_t 
					notifcation_msg = {0};

				SON_MEMCPY(&notifcation_msg.srv_cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				notifcation_msg.update_param.root_sequence_index = new_root_seq;

				notifcation_msg.update_param.bitmask
					|= SON_UPDATE_ROOT_SEQ_IDX_PRESENT;


				SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
						&notifcation_msg.update_param,
						sizeof(son_oam_rach_opt_update_req_param_t));

				son_create_send_buffer((son_u8 *)&notifcation_msg,
						SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
						SONRACH_OPT_PARAMETER_UPDATE_REQ,
						sizeof(notifcation_msg));

				update_local_counters_and_reset_msg_counters(
						p_cell_ctxt,SON_RACH_OPT_INTERFERENCE_OPTIMIZATION);

				SON_UT_TRACE_EXIT();
				return SON_SUCCESS;
			}
		}
		else
		{
            /* SPR-18735 START */
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_INFO,
					"None of root_sequence_indexes" 
                    "for Serving cell with start root sequence index:[%d] matches Neighbor cell"
					"with start root sequence index:[%d]",
					p_anr_cell_ctxt_node->data.prach_configuration.
					root_sequence_index,
					p_nbr_rec->neighbor_info.prach_config.
					root_sequence_index);
            /* SPR-18735 END */

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
		}
	}
	if (SON_FALSE == is_nbr_with_strong_pci_avail)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"For Serving Cell 0x%x, No EUTRAN neighbor exists "
				"with PCI similar to that of first 3 maximum reported PCI " 
				"and other attribute values similar to that of Serving Cell."
                " No new root sequence index can be proposed.",
				son_convert_char_cell_id_to_int_cell_id(
					p_anr_cell_ctxt_node->data.cell_id.cell_identity));

	}
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
}

/*****************************************************************************
 * Function Name  : son_rach_opt_tune_prach_freq_offset
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context
 in RACH Module.
 *                : p_rach_algo_counters - Pointer to the rach algo counters
 *                : p_anr_cell_ctxt_node - Pointer to the serving cell context
 in ANR Module.
 *                : p_strng_nbr_cell_node - Pointer to the strongest reported
 neighbor information 
 *                : strng_cell_count      - Count of strongest neighbors 
 reported.
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This function determines the prach frequency offset
 value for the serving cell in case of interference issue.
 ****************************************************************************/
static son_return_et
son_rach_opt_tune_prach_freq_offset
(
 rach_opt_cell_context_t             *p_cell_ctxt,
                    /*SPR 17777 +-*/
 anr_cell_context_node_t             *p_anr_cell_ctxt_node,
 rach_opt_strongest_nbr_cell_node_t  *p_strng_nbr_cell_node,
 son_u32                              strng_cell_count,
 son_duplex_mode_et                  serv_duplex_mode
 )
{
	son_u8                          idx = 0;
	anr_intra_rat_hash_rec_t        *p_nbr_rec = SON_PNULL;
	son_bool_et                     is_nbr_with_strong_pci_avail = SON_FALSE;

	for(idx = 0; idx < SON_MAX_NUM_OF_PCI_FOR_INTERFERENCE && 
			idx < strng_cell_count && SON_PNULL != p_strng_nbr_cell_node;
			idx++)
	{
		son_u8 new_prach_freq_offset = SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS; 
		p_nbr_rec = get_neighbor_record_from_nrt(
				&p_anr_cell_ctxt_node->data,
				&p_strng_nbr_cell_node->data,
				RACH_OPT_INTERFERENCE_UPDATE_PRACH_FREQ_OFFSET,
				serv_duplex_mode);

		if (SON_PNULL == p_nbr_rec)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_WARNING,
					"For the optmization attempt[count:%d], "
					"Serving Cell 0x%x with Duplex mode:[%s], There is no EUTRAN "
					"neighbor with the following attributes:-"
					"Duplex Mode:[%s],PCI [%d], UL EARFCN:[%d] ,"
					"DL EARFCN:[%d].Incase of TDD, prach configuration "
					"index as [%d]",
					p_cell_ctxt->
					rach_algo_run_count_after_started +1,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity),
					DUPLEX_MODE_OF_CELL[serv_duplex_mode],
					DUPLEX_MODE_OF_CELL[serv_duplex_mode],
					p_strng_nbr_cell_node->data.pci,
					anr_get_ul_earfcn(&p_anr_cell_ctxt_node->data),
					anr_get_earfcn(&p_anr_cell_ctxt_node->data),
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index);

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
			continue;
		}

		is_nbr_with_strong_pci_avail = SON_TRUE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_DETAILED,
				"For the serving "
				"Cell 0x%x, there exists an EUTRAN neighbor "
				"of neighbor Cell id[0x%x] with PCI:[%d] such that:-",
				" Serving Duplex Mode:[%s] equal Neighbor Duplex Mode",
				" Serving DL EARFCN :[%d] equal Neighbor DL EARFCN:[%d].",
				" Serving UL EARFCN :[%d] equal Neighbor UL EARFCN:[%d].",
				son_convert_char_cell_id_to_int_cell_id(
					p_anr_cell_ctxt_node->data.cell_id.cell_identity),
				son_convert_char_cell_id_to_int_cell_id(p_nbr_rec->
					neighbor_info.nbr_cgi.cell_identity),
				p_nbr_rec->neighbor_info.pci,
				DUPLEX_MODE_OF_CELL[serv_duplex_mode],
				anr_get_earfcn(&p_anr_cell_ctxt_node->data),
				p_nbr_rec->neighbor_info.earfcn,
				anr_get_ul_earfcn(&p_anr_cell_ctxt_node->data),
				p_nbr_rec->neighbor_info.ul_earfcn);

		if(SON_TDD_MODE  ==  serv_duplex_mode)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving prach configuration index :[%d] matches Neighbor "
					"prach configuration index:[%d].",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index,
					p_nbr_rec->neighbor_info.prach_config.
					prach_configuration_index);
		}

		if (p_nbr_rec->neighbor_info.
				prach_config.prach_frequency_offset ==
				p_anr_cell_ctxt_node->data.prach_configuration.
				prach_frequency_offset)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving "
					"prach frequency offset:[%d] matches Neighbor "
					"prach frequency offset:[%d].",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_frequency_offset,
					p_nbr_rec->neighbor_info.prach_config.
					prach_frequency_offset);

			if (SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS == 
					(new_prach_freq_offset =  
					 rach_opt_determine_prach_freq_offset(
						 &p_anr_cell_ctxt_node->data,
						 p_cell_ctxt,serv_duplex_mode)))
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,SON_WARNING,
						"For the optmization attempt[count:%d], "
						"Serving Cell 0x%x of Duplex mode[%s] "
						"no prach frequency offset can be calculated ",
						p_cell_ctxt->
						rach_algo_run_count_after_started +1,
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity),
						DUPLEX_MODE_OF_CELL[serv_duplex_mode]);

				SON_UT_TRACE_EXIT();
				return SON_FAILURE;
			}
			else
			{
				/*Send the new proposed value to MIF 
				  and update the counters.*/
				son_rach_opt_parameter_update_req_t 
					notifcation_msg = {0}; 

				SON_MEMCPY(&notifcation_msg.srv_cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				notifcation_msg.update_param.prach_frequency_offset = 
					new_prach_freq_offset;

				notifcation_msg.update_param.bitmask
					|= SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT;


				SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
						&notifcation_msg.update_param,
						sizeof(son_oam_rach_opt_update_req_param_t));

				son_create_send_buffer((son_u8 *)&notifcation_msg,
						SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
						SONRACH_OPT_PARAMETER_UPDATE_REQ,
						sizeof(notifcation_msg));

				update_local_counters_and_reset_msg_counters(
						p_cell_ctxt,SON_RACH_OPT_INTERFERENCE_OPTIMIZATION);

				SON_UT_TRACE_EXIT();
				return SON_SUCCESS;
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving "
					"prach frequency offset:[%d] not matches Neighbor "
					"prach frequency offset:[%d].",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_frequency_offset,
					p_nbr_rec->neighbor_info.prach_config.
					prach_frequency_offset);

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
		}
	}
	if (SON_FALSE == is_nbr_with_strong_pci_avail)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"Serving Cell 0x%x, No EUTRAN neighbor exists "
				"with PCI similar to that of first 3 maximum reported PCI " 
				"and other attribute values similar to that of Serving Cell."
                "No new prach frequency offset can be proposed.",
				son_convert_char_cell_id_to_int_cell_id(
					p_anr_cell_ctxt_node->data.cell_id.cell_identity));
	}
	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
}

/*****************************************************************************
 * Function Name  : son_rach_opt_tune_prach_config_index
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context
 in RACH Module.
 *                : p_rach_algo_counters - Pointer to the rach algo counters
 *                : p_anr_cell_ctxt_node - Pointer to the serving cell context
 in ANR Module.
 *                : p_strng_nbr_cell_node - Pointer to the strongest reported
 neighbor information 
 *                : strng_cell_count      - Count of strongest neighbors 
 reported.
 *                : serv_duplex_mode     - Duplex mode of the serving cell
 *                                          (FDD/TDD)
 *                : p_serving_cell_prach_config_info - Prach Configuration
 Information Block.
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This function determines the prach configuration index
 for the serving cell(TDD only) in case of interference  
 issue.
 ****************************************************************************/
static son_return_et
son_rach_opt_tune_prach_config_index
(
 rach_opt_cell_context_t             *p_cell_ctxt,
                    /*SPR 17777 +-*/
 anr_cell_context_node_t             *p_anr_cell_ctxt_node,
 rach_opt_strongest_nbr_cell_node_t  *p_strng_nbr_cell_node,
 son_u32                              strng_cell_count,
 son_duplex_mode_et                  serv_duplex_mode,
 const frame_type_2_ra_config_data_for_preamble *
 p_serving_cell_prach_config_info
 )
{
	son_u8                    idx = 0;
	anr_intra_rat_hash_rec_t  *p_nbr_rec = SON_PNULL;
	son_bool_et               is_nbr_cell_with_strng_pci_avail = SON_FALSE;

	for(idx = 0; idx < SON_MAX_NUM_OF_PCI_FOR_INTERFERENCE && 
			idx < strng_cell_count && SON_PNULL != p_strng_nbr_cell_node;
			idx++)
	{
		son_u8 new_prach_config_idx  = SON_MAX_PRACH_CONFIG_IDX; 
		p_nbr_rec = get_neighbor_record_from_nrt(
				&p_anr_cell_ctxt_node->data,
				&p_strng_nbr_cell_node->data,
				RACH_OPT_INTERFERENCE_UPDATE_PRACH_CONFIG_IDX,
				serv_duplex_mode);

		if (SON_PNULL == p_nbr_rec)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_WARNING,
					"Optmization attempt[count:%d], "
					"Serving Cell 0x%x Duplex mode %s, no EUTRAN neighbor "
					"exists with the following attributes:- ",
					"Duplex mode:[%s],PCI value:[%d],EARFCN:[%d]",
					p_cell_ctxt->
					rach_algo_run_count_after_started +1,
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity),
					DUPLEX_MODE_OF_CELL[serv_duplex_mode],
					DUPLEX_MODE_OF_CELL[serv_duplex_mode],
					p_strng_nbr_cell_node->data.pci,
					anr_get_earfcn(&p_anr_cell_ctxt_node->data));

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
			continue;
		}

		is_nbr_cell_with_strng_pci_avail = SON_TRUE;

		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_DETAILED,
				"Optmization attempt[count:%d], Serving "
				"Cell 0x%x, there exists EUTRAN neighbor "
				"0x%x PCI %d such that:- "
				"Serving Duplex Mode:[%s] matches Neighbor "
				"Duplex Mode:[%s]"
				" Serving EARFCN:[%d] matches Neighbor EARFCN:[%d]",
				p_cell_ctxt->
				rach_algo_run_count_after_started +1,
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity),
				son_convert_char_cell_id_to_int_cell_id(
					p_nbr_rec->neighbor_info.nbr_cgi.cell_identity),
				p_nbr_rec->neighbor_info.pci,
				DUPLEX_MODE_OF_CELL[serv_duplex_mode],
				DUPLEX_MODE_OF_CELL[serv_duplex_mode],
				anr_get_earfcn(&p_anr_cell_ctxt_node->data),
				p_nbr_rec->neighbor_info.earfcn);


		if (p_nbr_rec->neighbor_info.
				prach_config.prach_configuration_index ==
				p_anr_cell_ctxt_node->data.prach_configuration.
				prach_configuration_index)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving Prach Configuration Index:[%d] matches "
					"Neighbor Prach Configuration Index:[%d].",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index,
					p_nbr_rec->neighbor_info.prach_config.
					prach_configuration_index);

			if (SON_MAX_PRACH_CONFIG_IDX  == 
					(new_prach_config_idx =  
					 rach_opt_determine_prach_config_idx(
						 &p_anr_cell_ctxt_node->data,
						 p_cell_ctxt,serv_duplex_mode,
						 p_serving_cell_prach_config_info,
						 p_anr_cell_ctxt_node->data.prach_configuration.
						 prach_configuration_index)))
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,SON_WARNING,
						"Optmization attempt[count:%d], Serving "
						"Cell 0x%x of TDD mode no prach "
						"configuration index can be calculated",
						p_cell_ctxt->
						rach_algo_run_count_after_started +1,
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));

				SON_UT_TRACE_EXIT();
				return SON_FAILURE;
			}
			else
			{
				/*Send the new proposed value to MIF 
				  and update the counters.*/

				son_rach_opt_parameter_update_req_t 
					notifcation_msg = {0}; 

				SON_MEMCPY(&notifcation_msg.srv_cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				notifcation_msg.update_param.prach_configuration_index = 
					new_prach_config_idx;

				notifcation_msg.update_param.bitmask
					|= SON_UPDATE_PRACH_CONFIG_IDX_PRESENT;


				SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
						&notifcation_msg.update_param,
						sizeof(son_oam_rach_opt_update_req_param_t));


				son_create_send_buffer((son_u8 *)&notifcation_msg,
						SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
						SONRACH_OPT_PARAMETER_UPDATE_REQ,
						sizeof(notifcation_msg));

				update_local_counters_and_reset_msg_counters(
						p_cell_ctxt,SON_RACH_OPT_INTERFERENCE_OPTIMIZATION);

				SON_UT_TRACE_EXIT();
				return SON_SUCCESS;
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name,SON_DETAILED,
					"Serving Prach Configuration Index:[%d] not matches "
					"Neighbor Prach Configuration Index:[%d].",
					p_anr_cell_ctxt_node->data.prach_configuration.
					prach_configuration_index,
					p_nbr_rec->neighbor_info.prach_config.
					prach_configuration_index);

			p_strng_nbr_cell_node =  
				rach_opt_get_next_strongest_nbr_cell(
						p_strng_nbr_cell_node);
		}
	}

	if (SON_FALSE == is_nbr_cell_with_strng_pci_avail)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"Serving Cell 0x%x, No EUTRAN neighbor exists "
				"with PCI similar to that of first 3 maximum reported PCI " 
				"and other attribute values similar to that of Serving Cell."
                "No new prach Configuration Index can be proposed.",
				son_convert_char_cell_id_to_int_cell_id(
					p_anr_cell_ctxt_node->data.cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
}
/*****************************************************************************
 * Function Name  : rach_optimization_algo_for_interference
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context in 
 RACH Module.
 *                : p_rach_algo_counters - Pointer to the rach algo counters.
 *                : p_anr_cell_ctxt_node - Pointer to the serving cell context
 in ANR Module.
 *                : p_strng_nbr_cell_node - Pointer to the strongest reported
 neighbor information 
 *                : strng_cell_count      - Count of strongest neighbors 
 reported.
 *                : serving_cell_duplex_mode - Duplex mode of the serving cell
 *                                          (FDD/TDD).
 *                : failure_level        - rach failure level
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This is the interference based rach optmization algorithm 
 ****************************************************************************/
static son_void_t
rach_optimization_algo_for_interference
(
 rach_opt_cell_context_t             *p_cell_ctxt,
 rach_opt_algo_counters_context_t    *p_rach_algo_counters,
 anr_cell_context_node_t             *p_anr_cell_ctxt_node,
 rach_opt_strongest_nbr_cell_node_t  *p_strng_nbr_cell_node,
 son_u32                             strng_cell_count,
 son_duplex_mode_et                  serving_cell_duplex_mode
 )
{
	son_u8 srv_cell_trans_bw = SON_MAX_NUM_OF_TRANSMISSION_BW;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_rach_algo_counters ||
			SON_PNULL == p_anr_cell_ctxt_node || 
			SON_PNULL == p_strng_nbr_cell_node || 0 == strng_cell_count ||
			serving_cell_duplex_mode >= SON_MAX_NUM_OF_DUPLEX_MODES)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Algorithm"
				"not be triggered "
				"(p_cell_ctxt or p_rach_algo_counters or p_anr_cell_ctxt_node"
				" is SON_PNULL or strng_cell_count is 0 or" 
				"serving_cell_duplex_mode is [%d] [0 - FDD and 1 - TDD] "
				" failure level is set to invalid value ",
				serving_cell_duplex_mode);

		SON_UT_TRACE_EXIT();
		return;
	}


	/*Determination of the ROOT SEQUENCE value for the 
	  serving cell*/
	if (SON_FAILURE !=  son_rach_opt_tune_root_sequence_index(
				p_cell_ctxt,
                    /*SPR 17777 +-*/
				p_anr_cell_ctxt_node,
				p_strng_nbr_cell_node,
				strng_cell_count,
				serving_cell_duplex_mode))
	{
		SON_UT_TRACE_EXIT();
		return;
	}

	/*Determination of the PRACH FREQUENCY OFFSET of the serving cell.
	  If the cell is of 6 RBs then we can not determine the 
	  frequency offset */
	srv_cell_trans_bw =  get_transmission_bandwidth(
			&p_anr_cell_ctxt_node->data.choice_eutra_mode);

	if (SON_TRANSMISSION_BW_6_RBS !=
			srv_cell_trans_bw && 
			srv_cell_trans_bw <SON_MAX_NUM_OF_TRANSMISSION_BW)
	{
		if (SON_FAILURE !=  son_rach_opt_tune_prach_freq_offset( 
					p_cell_ctxt,
                    /*SPR 17777 +-*/
					p_anr_cell_ctxt_node,
					p_strng_nbr_cell_node,
					strng_cell_count,
					serving_cell_duplex_mode))
		{
			SON_UT_TRACE_EXIT();
			return;
		}
	} 
	else
	{
		if (SON_TRANSMISSION_BW_6_RBS == srv_cell_trans_bw)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Serving Cell 0x%x, transmission "
					"bandwidth is of 6RBs so a new frequency offset "
					"can not be proposed",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
		}
	}

	/*Change the PRACH CONFIGURATION INDEX of the TDD based serving cell */
	if (SON_TDD_MODE == serving_cell_duplex_mode)
	{
		son_u8 serving_cells_prach_config_idx = p_anr_cell_ctxt_node->data.
			prach_configuration.prach_configuration_index;
		const frame_type_2_ra_config_data_for_preamble 
			* p_serving_cell_prach_config_info = SON_PNULL;
		son_return_et ret_val = SON_FAILURE;

		do
		{
			if (serving_cells_prach_config_idx >= SON_MAX_PRACH_CONFIG_IDX)
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
						SON_WARNING,
						"Serving "
						"Cell 0x%x has Prach Configuration Index:[%d] "
						"> Maximum allowed Prach Configuration index:[%d]",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity),
						serving_cells_prach_config_idx,SON_MAX_PRACH_CONFIG_IDX - 1);
				break;
			}

			p_serving_cell_prach_config_info =
				&RA_CONFIG_TDD_DATA_FOR_PREAMBLE[serving_cells_prach_config_idx];
			if(SON_PNULL == p_serving_cell_prach_config_info)
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
						SON_WARNING,
						"Serving "
						"Cell 0x%x has Prach Configuration Index:[%d] "
						"but corresponding Prach Configuration Information "
						"Block is NULL",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity),
						serving_cells_prach_config_idx);
				break;
			}

			if(PREAMBLE_FORMAT_NA  <= p_serving_cell_prach_config_info->preamble_format)
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
						SON_WARNING,
						"Serving "
						"Cell 0x%x of Prach Configuration Index:[%d] "
						"has Preamble format is:[%d] which is > Maximun allowed "
						"Preamble format:[%d] ",
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->
							cell_id.cell_identity),serving_cells_prach_config_idx, 
						p_serving_cell_prach_config_info->preamble_format,
						PREAMBLE_FORMAT_4);
				break;
			}

			ret_val = son_rach_opt_tune_prach_config_index(p_cell_ctxt,
                    /*SPR 17777 +-*/
					p_anr_cell_ctxt_node,
					p_strng_nbr_cell_node,strng_cell_count,
					serving_cell_duplex_mode,
					p_serving_cell_prach_config_info);
		}while(0);

		if (SON_FAILURE == ret_val)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Interference algorithm hard stops,counters get reset and "
					"Failure notifcation is send to MIF");
		}
		else
		{
			SON_UT_TRACE_EXIT();
			return;
		}
	}

	if (SON_FDD_MODE == serving_cell_duplex_mode)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Serving Cell 0x%x of Duplex Mode:[%s] ,"
				"in the current optmization attempt,new value for root sequence "
				"index or prach frequency offset can not be determined."
				"Interference algorithm hard stops,counters get reset and "
				"Failure notifcation is send to MIF",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity),
				DUPLEX_MODE_OF_CELL[serving_cell_duplex_mode]);
	}



	/*Populate and send the notifcation to MIF*/
	rach_opt_send_algo_failure_notifcation_to_oam(
			p_cell_ctxt,
			SON_RACH_OPT_INTERFERENCE_OPTIMIZATION_FAILURE,
			SON_RACH_OPT_INTERFERENCE_OPTIMIZATION);

	/*Reset counters*/
	rach_opt_init_rach_counters(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return;
}

/*****************************************************************************
 * Function Name  : rach_opp_correct_split
 * Inputs         : first_value_observed    The value of number of group A 
 *                  preambles found using the UE reports received from RRM
 *                  second_value_observed   The value of number of group B 
 *                  preambles found using the UE reports received from RRM
 *                  first_value_configured  The configured value of number 
 *                  of group A preambles
 *                  second_value_configured The configured value of number 
 *                  of group B preambles 
 *                  step_size               Configured parameter to increase 
 *                  or decrease the number of Contention based preambles for 
 *                  Group A in a single step.
 * Outputs        : None
 * Returns        : Change required in the number of Contention based 
 *                  preambles for Group A
 * Description    : This is used to determine if the A/B split needs 
 *                  correction and if yes then it returns the change required.
 ****************************************************************************/
son_s8
rach_opp_correct_split
(
 /* SPR-20477 START */
 son_u64 first_value_observed,
 son_u64 second_value_observed,
 /* SPR-20477 END */
 son_u8  first_value_configured,
 son_u8  second_value_configured,
 son_u8  threshold
 )
{
	son_s8 change_required = 0;
	son_float percent_first_value_observed = 0.0;
	son_float percent_first_value_configured = 0.0;
	son_float diff = 0.0;
    /* SPR-20477 START */
    son_u64 sum_of_value_observed = 0;
    son_u32 sum_of_value_configured = 0;
    /* SPR-20477 END */
	SON_UT_TRACE_ENTER();

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_INFO,
			"Split change for Group A and Group B Algo is started:"
			"Observed number of group A %llu"
			" group B %llu preambles"
			" Configured number of group A %u"
			" group B %u preambles"
			" Group A config vs group A observed difference threshold: %u",
			first_value_observed,
			second_value_observed,
			first_value_configured,
			second_value_configured,
			threshold);

	sum_of_value_observed = first_value_observed + second_value_observed;
	sum_of_value_configured = first_value_configured + second_value_configured;

    /* SPR-20477 START */
    if (0 != sum_of_value_observed)
    {
        percent_first_value_observed = (((son_double)first_value_observed) /
            sum_of_value_observed) * 100;
	}
    /* SPR-20477 END */
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING, "Sum of Group A and Group "
				"B preambles observed from UE reports is 0 so can't correct "
				"the A/B split.");
		SON_UT_TRACE_EXIT();
		return change_required;
	}

	if (0 != sum_of_value_configured)
	{
		percent_first_value_configured = (((son_float)first_value_configured) / 
				sum_of_value_configured) * 100;
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Sum of configured Group A and Group B "
				"preambles is 0 so can't correct the A/B split.");
		SON_UT_TRACE_EXIT();
		return change_required;
	}

	diff = percent_first_value_observed - percent_first_value_configured;
	SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
			p_son_rach_opt_facility_name,
			SON_INFO,
			"group A preambles observed: %f %%, "
			"group A preambles configured: %f %%",
			percent_first_value_observed,
			percent_first_value_configured);
	if (son_fabsf(diff) >= threshold)
	{
		if (diff > 0)
		{
			/* Increase the configured value */
			change_required = +1;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,
					SON_INFO,
					"Need to increase the number of RA preambles");
		}
		else if (diff < 0)
		{
			/* Decrease the configured value */
			change_required = -1;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,
					SON_INFO,
					"Need to decrease the number of RA preambles");
		}
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
				p_son_rach_opt_facility_name,
				SON_INFO,
				"Number of RA preambles not modified since the difference "
				"between Percentage group A preambles observed and "
				"configured is less than "
				"rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold: %u",
				threshold);
	}

	SON_UT_TRACE_EXIT();
	return change_required;
} /* rach_opp_correct_split */

/*******************************************************************************
 * Function Name  : rach_opt_inc_size_of_ra_group_a
 * Inputs         : p_cell_ctxt - Pointer to the serving cell context.
 *                  delta_value- Delta value by which Number of RA preambles 
 *                  Group A is Increased
 * Outputs        : p_new_size_of_ra_group_a Decremented Number of RA 
 *                  preambles Group A
 * Returns        : SON_SUCCESS - If decrement is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function increases the given number of RA preambles 
 *                  Group A value by given delta value
 ******************************************************************************/
static son_return_et
rach_opt_inc_size_of_ra_group_a
(
 rach_opt_cell_context_t                 *p_cell_ctxt,
 son_u8                                  delta_value,
 son_oam_ra_preambles_group_a_size_et    *p_new_size_of_ra_group_a
 )
{
	son_u8 temp_size_of_ra_group_a = p_cell_ctxt->size_of_ra_group_a;

	SON_UT_TRACE_ENTER();

	if (SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60 > p_cell_ctxt->size_of_ra_group_a)
	{   
		if ((SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60 - 
					p_cell_ctxt->size_of_ra_group_a) >= 
				delta_value)
		{
			*p_new_size_of_ra_group_a = (son_oam_ra_preambles_group_a_size_et)
				(temp_size_of_ra_group_a + delta_value);
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Number of RA Preamble "
					"Group A value increased. Old Number of RA Preamble Group "
					"A: %s, New Number of RA Preamble Group A: %s, "
					"Delta value: %u, Maximum valid Number of RA Preamble "
					"Group A: %s",
					RA_GRP_A_PREAMBLE_NAMES[p_cell_ctxt->
					size_of_ra_group_a],
					RA_GRP_A_PREAMBLE_NAMES[*p_new_size_of_ra_group_a], 
					delta_value, 
					RA_GRP_A_PREAMBLE_NAMES[
					SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60]);  
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_WARNING,
			"Number of RA Preamble "
			"Group A value not increased. Current Number of RA Preamble "
			"Group A: %u, Delta value: %u, Maximum valid Number of RA "
			"Preamble Group A: %u",
			p_cell_ctxt->size_of_ra_group_a, 
			delta_value, 
			SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60);  

	/*Populate and send the notifcation to MIF*/
	rach_opt_send_algo_failure_notifcation_to_oam(
			p_cell_ctxt,
			SON_RACH_OPT_RA_PREAMBLE_GROUP_A_MAX_LIMIT_REACHED,
			SON_RACH_OPT_CB_OPTIMIZATION); 

	/*Reset counters*/
	rach_opt_init_rach_counters(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* rach_opt_inc_size_of_ra_group_a */

/*******************************************************************************
 * Function Name  : rach_opt_dec_size_of_ra_group_a
 * Inputs         : p_cell_ctxt - Pointer to the serving cell context.
 *                  delta_value- Delta value by which Number of RA preambles 
 *                  Group A is decreased
 * Outputs        : p_new_size_of_ra_group_a Decremented Number of RA 
 *                  preambles Group A
 * Returns        : SON_SUCCESS - If increment is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function decreases the given number of RA preambles 
 *                  Group A value by given delta value
 ******************************************************************************/
static son_return_et
rach_opt_dec_size_of_ra_group_a
(
 rach_opt_cell_context_t                 *p_cell_ctxt,
 son_u8                                  delta_value,
 son_oam_ra_preambles_group_a_size_et    *p_new_size_of_ra_group_a
 )
{
	son_u8 temp_size_of_ra_group_a = p_cell_ctxt->size_of_ra_group_a;
	SON_UT_TRACE_ENTER();

	if (SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4 < p_cell_ctxt->size_of_ra_group_a)
	{   
		if ((p_cell_ctxt->size_of_ra_group_a - 
					SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4) >= 
				delta_value)
		{
			*p_new_size_of_ra_group_a = (son_oam_ra_preambles_group_a_size_et)
				(temp_size_of_ra_group_a - delta_value);
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Number of RA "
					"preambles Group A value decreased. Old Number of RA "
					"preambles Group A: %s, New Number of RA preambles Group "
					"A: %s, Delta value: %u, Minimum valid Number of RA "
					"preambles Group A: %s",
					RA_GRP_A_PREAMBLE_NAMES[p_cell_ctxt->size_of_ra_group_a],
					RA_GRP_A_PREAMBLE_NAMES[*p_new_size_of_ra_group_a], 
					delta_value, 
					RA_GRP_A_PREAMBLE_NAMES[
					SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4]);
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_WARNING,
			"Number of RA preambles "
			"Group A value not decreased. Current Number of RA preambles "
			"Group A: %s, Delta value: %u, Minimum valid Number of RA "
			"preambles Group A: %s",
			RA_GRP_A_PREAMBLE_NAMES[p_cell_ctxt->size_of_ra_group_a], 
			delta_value, 
			RA_GRP_A_PREAMBLE_NAMES[SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4]);  

	/*Populate and send the notifcation to MIF*/
	rach_opt_send_algo_failure_notifcation_to_oam(
			p_cell_ctxt,
			SON_RACH_OPT_RA_PREAMBLE_GROUP_A_MIN_LIMIT_REACHED,
			SON_RACH_OPT_CB_OPTIMIZATION); 

	/*Reset counters*/
	rach_opt_init_rach_counters(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* rach_opt_dec_size_of_ra_group_a */

/*******************************************************************************
 * Function Name  : rach_opt_inc_num_of_ra_preambles
 * Inputs         : p_cell_ctxt - Pointer to the serving cell context.
 *                  delta_value- Delta value by which Number of RA preambles 
 *                  is Increased
 * Outputs        : p_new_number_of_ra_preambles Decremented Number of RA 
 *                  preambles
 * Returns        : SON_SUCCESS - If decrement is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This function increases the given number of RA preambles 
 *                  value by given delta value
 ******************************************************************************/
static son_return_et
rach_opt_inc_num_of_ra_preambles
(
 rach_opt_cell_context_t        *p_cell_ctxt,
 son_u8                         delta_value,
 son_oam_ra_preamble_count_et   *p_new_number_of_ra_preambles
 )
{
	son_u8 temp_num_of_ra_preambles = p_cell_ctxt->number_of_ra_preambles;

	SON_UT_TRACE_ENTER();

	if (SON_OAM_RA_PREAMBLE_COUNT_N64 > p_cell_ctxt->number_of_ra_preambles)
	{   
		if ((SON_OAM_RA_PREAMBLE_COUNT_N64 - 
					p_cell_ctxt->number_of_ra_preambles) >= 
				delta_value)
		{
			*p_new_number_of_ra_preambles = (son_oam_ra_preamble_count_et)
				(temp_num_of_ra_preambles + delta_value);
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Number of RA Preamble "
					"value increased. Old Number of RA Preamble: %s, "
					"New Number of RA Preamble: %s, "
					"Delta value: %u, Maximum valid Number of RA Preamble: %s",
					RA_PREAMBLE_NAMES[p_cell_ctxt->number_of_ra_preambles],
					RA_PREAMBLE_NAMES[*p_new_number_of_ra_preambles], 
					delta_value, 
					RA_PREAMBLE_NAMES[SON_OAM_RA_PREAMBLE_COUNT_N64]);  
			return SON_SUCCESS;
		}
	}

	SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
			SON_WARNING,
			"Number of RA Preamble "
			"value not increased. Current Number of RA Preamble: %u, "
			"Delta value: %u, Maximum valid Number of RA Preamble: %u",
			p_cell_ctxt->number_of_ra_preambles, 
			delta_value, 
			SON_OAM_RA_PREAMBLE_COUNT_N64);  

	/*Populate and send the notifcation to MIF*/
	rach_opt_send_algo_failure_notifcation_to_oam(
			p_cell_ctxt,
			SON_RACH_OPT_RA_PREAMBLE_MIN_LIMIT_REACHED,
			SON_RACH_OPT_CB_OPTIMIZATION); 

	/*Reset counters*/
	rach_opt_init_rach_counters(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return SON_FAILURE;
} /* rach_opt_inc_num_of_ra_preambles */

/*******************************************************************************
 * Function Name  : rach_opt_update_size_of_ra_group_a
 * Inputs         : p_cell_ctxt - Pointer to the serving cell context.
 *                  change_required - Delta value by which to change size of 
 *                  group A RA preambles
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function calculates the value of size of group A RA 
 *                  preambles using given delta value and sends the update 
 *                  request to SONMgmtIfH.
 ******************************************************************************/
son_void_t
rach_opt_update_size_of_ra_group_a
(
 rach_opt_cell_context_t *p_cell_ctxt,
 son_s8                  change_required
 )
{
	son_rach_opt_parameter_update_req_t notifcation_msg = {0}; 
	son_return_et is_updated = SON_FAILURE;
	SON_UT_TRACE_ENTER();

	if (change_required > 0)
	{
		is_updated = rach_opt_inc_size_of_ra_group_a(p_cell_ctxt,
				RACH_OPT_DEFAULT_STEP_SIZE,
				&notifcation_msg.update_param.size_of_ra_group_a);
	}
	else
	{
		is_updated = rach_opt_dec_size_of_ra_group_a(p_cell_ctxt,
				RACH_OPT_DEFAULT_STEP_SIZE,
				&notifcation_msg.update_param.size_of_ra_group_a);
	}

	if (SON_SUCCESS == is_updated)
	{
		SON_MEMCPY(&notifcation_msg.srv_cgi,
				&p_cell_ctxt->cell_id,
				sizeof(son_intra_rat_global_cell_id_t));
		notifcation_msg.update_param.bitmask |= 
			SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT;

		/* Send the new proposed value to MIF and update the counters.*/
		son_create_send_buffer((son_u8 *)&notifcation_msg,
				SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
				SONRACH_OPT_PARAMETER_UPDATE_REQ,
				sizeof(notifcation_msg));
		SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
				&notifcation_msg.update_param,
				sizeof(son_oam_rach_opt_update_req_param_t));
		update_local_counters_and_reset_msg_counters(
				p_cell_ctxt,
				SON_RACH_OPT_CB_OPTIMIZATION);
	}

	SON_UT_TRACE_EXIT();
}

/*******************************************************************************
 * Function Name  : rach_opt_update_number_of_ra_preambles
 * Inputs         : p_cell_ctxt - Pointer to the serving cell context.
 *                  change_required - Delta value by which to update number 
 *                  of RA preambles
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function calculates the value of number of RA 
 *                  preambles using given delta value and sends the update 
 *                  request to SONMgmtIfH.
 ******************************************************************************/
son_void_t
rach_opt_update_number_of_ra_preambles
(
 rach_opt_cell_context_t *p_cell_ctxt,
 son_u8                  change_required
 )
{
	son_rach_opt_parameter_update_req_t notifcation_msg = {0}; 
	SON_UT_TRACE_ENTER();

	SON_MEMCPY(&notifcation_msg.srv_cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	notifcation_msg.update_param.bitmask |= 
		SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT;

	if (SON_SUCCESS == rach_opt_inc_num_of_ra_preambles(p_cell_ctxt, 
				change_required, &notifcation_msg.update_param.
				number_of_ra_preambles))
	{
		/* Send the new proposed value to MIF and update the counters.*/
		son_create_send_buffer((son_u8 *)&notifcation_msg,
				SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
				SONRACH_OPT_PARAMETER_UPDATE_REQ,
				sizeof(notifcation_msg));

		SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
				&notifcation_msg.update_param,
				sizeof(son_oam_rach_opt_update_req_param_t));

		update_local_counters_and_reset_msg_counters(
				p_cell_ctxt,
				SON_RACH_OPT_CB_OPTIMIZATION);
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_update_number_of_ra_preambles */

/*****************************************************************************
 * Function Name  : rach_opt_contention_based_optimization_algo
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context.
 *                : p_rach_algo_counters - Pointer to the rach algo counters
 *                  duplex_mode         - Duplex Mode of the serving cell 
 *                                          (FDD/TDD)
 *                  p_anr_cell_ctxt_node - Pointer to the ANR cell context
 * Outputs        : None
 * Returns        : son_void_t 
 * Description    : This is the contention based rach optimization algorithm.
 ****************************************************************************/
static son_void_t
rach_opt_contention_based_optimization_algo
(
 rach_opt_cell_context_t             *p_cell_ctxt,
 rach_opt_algo_counters_context_t    *p_rach_algo_counters,
 son_duplex_mode_et                  duplex_mode,
 anr_cell_context_node_t             *p_anr_cell_ctxt_node
 )
{
	son_u64 total_cfra_preamble = 0;
	son_s8 change_required = 0.0;
	son_double unused_cfra_percentage = 0.0;
	son_u8 number_of_cfra_preambles;
	son_u8 size_of_group_a = 4 * (p_cell_ctxt->size_of_ra_group_a + 1);
	son_u8 number_of_ra_preambles = 4 * (p_cell_ctxt->number_of_ra_preambles + 
			1);
	son_u8 size_of_group_b = number_of_ra_preambles - size_of_group_a;
    /* SPR-19170 START */
    son_float avg_used_cfra_preambles = 0.0;
    /* SPR-19170 END */
	SON_UT_TRACE_ENTER();

	/* Check if a/b split is to be corrected and if yes then increase or 
	   decrease number of group A preambles */
	change_required = rach_opp_correct_split(
			p_rach_algo_counters->num_of_group_a_preambles_observed,
			p_rach_algo_counters->num_of_group_b_preambles_observed,
			size_of_group_a,
			size_of_group_b,
			p_cell_ctxt->read_only_config_params.
			rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold);

	/* If change is required in the split */
	if (0 != change_required)
	{
		/* Need to change a/b split. Send the new proposed value to 
		   MIF and update the counters.*/
		rach_opt_update_size_of_ra_group_a(
				p_cell_ctxt, change_required);
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"A/B split is not "
				"corrected Checking for CBRA/CFRA split updation");
		number_of_cfra_preambles = SON_MAX_PRACH_CONFIG_IDX - 
			number_of_ra_preambles;
		/* Find total number of CFRA preambles in the RACH reports periodicity
		   interval i.e for L2 RACH perf reports for 1 iteration */
		total_cfra_preamble = rach_opt_get_total_cfra_preamble(
				p_anr_cell_ctxt_node->data.prach_configuration.
				prach_configuration_index,
				p_cell_ctxt->oam_config_params.rachopt_report_periodicity,
				number_of_ra_preambles, duplex_mode);
		/* Check if CBRA/CFRA split is to be corrected and if yes then 
		   increase number of contention bases preambles */
		if (0 != total_cfra_preamble)
		{
			unused_cfra_percentage = 
				((son_double)p_cell_ctxt->rach_algo_counters.
				 total_unassigned_sum_cfra / total_cfra_preamble) * 100;
			if (unused_cfra_percentage > p_cell_ctxt->read_only_config_params.
					rachopt_cfra_unused_threshold)
			{
				/* Check if CFRA is set to minimum allowed value */
    /* SPR-19170 START */
                avg_used_cfra_preambles =  (son_float)(number_of_cfra_preambles * 
                        ((100 - unused_cfra_percentage) / 100));
                if ((p_cell_ctxt->oam_config_params.rachopt_min_cfra_preambles >=
                        number_of_cfra_preambles) ||
                    (avg_used_cfra_preambles > (son_float)(number_of_cfra_preambles - RACH_OPT_DEFAULT_STEP_SIZE)))
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Number of free preambles i.e CFRA value %u is "
                            "already configured to the minimum possible CFRA "
                            "so CBRA/CFRA split can't be corrected.",
							number_of_cfra_preambles);
    /* SPR-19170 END */
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Sending failure notification to SMIF and reseting"
							" the algo.");
					rach_opt_send_algo_failure_notifcation_to_oam(
							p_cell_ctxt,
							SON_RACH_OPT_NUMBER_OF_CFRA_PREAMBLES_MIN_THRESHOLD_REACHED,
							SON_RACH_OPT_CB_OPTIMIZATION);
					/*Reset counters*/
					rach_opt_init_rach_counters(p_cell_ctxt);
				}
				else
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
							"Unused cfra observed using RRM reports: %lf %% "
							"is > rachopt_cfra_unused_threshold: %u %% so "
							"some of the CFRA preambles can be assigned to "
							"CBRA preambles",
							unused_cfra_percentage, 
							p_cell_ctxt->read_only_config_params.
							rachopt_cfra_unused_threshold);
					/* Need to change CBRA/CFRA split. Send the new proposed 
					   value to MIF and update the counters. */
					rach_opt_update_number_of_ra_preambles(p_cell_ctxt, 
							RACH_OPT_DEFAULT_STEP_SIZE);
				}
			}
			else
			{
				/* After running algo it is observed that contention
				   based tuning is required but as per the counters
				   not able to tune CBRA/CFRA or A/B split so send
				   failure notification to OAM */
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name, SON_WARNING,
						"Unused "
						"CFRA observed using RRM reports: %f %% is <= "
						"rachopt_cfra_unused_threshold: %u %% so CBRA/CFRA "
						"tuning is not possible even if contention is "
						"observed."
						" Sending failure notification to SMIF and reseting the algo",
						unused_cfra_percentage, 
						p_cell_ctxt->read_only_config_params.
						rachopt_cfra_unused_threshold);
				rach_opt_send_algo_failure_notifcation_to_oam(
						p_cell_ctxt,
						SON_RACH_OPT_INSUFFICIENT_UNUSED_CFRA_COUNTERS,
						SON_RACH_OPT_CB_OPTIMIZATION);
				/*Reset counters*/
				rach_opt_init_rach_counters(p_cell_ctxt);
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Total CFRA "
					"preamble is 0 so CBRA/CFRA tuning is not possible even if"
					" contention is observed. Sending failure notification to "
					"SMIF and reseting the algo.");
			rach_opt_send_algo_failure_notifcation_to_oam(
					p_cell_ctxt,
					SON_RACH_OPT_TOTAL_CFRA_PREAMBLE_CALCULATED_ZERO,
					SON_RACH_OPT_CB_OPTIMIZATION);
			/*Reset counters*/
			rach_opt_init_rach_counters(p_cell_ctxt);
		}
	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_contention_based_optimization_algo */

/*****************************************************************************
 * Function Name  : rach_opt_is_complete_rach_config_available
 * Inputs         : p_cell_ctxt          - Pointer to the serving cell context.
 *                : p_anr_cell_ctxt_node - Pointer to the ANR serving cell
 *                                         context.
 * Outputs        : None
 * Returns        : SON_TRUE - If complete RACH configuration is available
 *                  SON_FALSE - Otherwise
 * Description    : This function checks if complete RACH configuration is 
 *                  available or not before executing rach optmization 
 *                  algorithm.
 ****************************************************************************/
static son_bool_et
rach_opt_is_complete_rach_config_available
(
 rach_opt_cell_context_t *p_cell_context,
 anr_cell_context_node_t *p_anr_cell_ctxt_node
 )
{
	son_bool_et is_complete = SON_TRUE;
	SON_UT_TRACE_ENTER();

	if (0xFF == p_cell_context->preamble_initial_rcvd_target_power)
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config parameter "
				"preamble_initial_rcvd_target_power value is not configured "
				"for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}
	if (0xFF == p_cell_context->number_of_ra_preambles)
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config parameter "
				"number_of_ra_preambles value is not configured "
				"for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}
	if (0xFF == p_cell_context->size_of_ra_group_a)
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config parameter "
				"size_of_ra_group_a value is not configured "
				"for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}
	if (0xFF == p_cell_context->pusch_hopping_offset)
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config parameter "
				"pusch_hopping_offset value is not configured "
				"for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}
	if (SON_FALSE == 
			p_anr_cell_ctxt_node->data.is_prach_configuration_available)
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config parameter "
				"PRACH configuration is not received for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}
	if (!(SON_PRACH_CONFIG_INDEX_PRESENT & 
				p_anr_cell_ctxt_node->data.prach_configuration.bitmask))
	{
		is_complete = SON_FALSE;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Config "
				"parameter prach_configuration_index is not configured "
				"for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_context->cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return is_complete;
}

/*****************************************************************************
 * Function Name  : rach_optimization_algo
 * Inputs         : p_cell_ctxt          - Pointer to the serving cell context.
 *                : p_rach_algo_counters - Pointer to the rach algo counters.
 *                : p_anr_cell_ctxt_node - Pointer to the ANR serving cell
 *                                         context.
 *                : duplex_mode          - Duplex mode of the serving cell
 *                                          (FDD/TDD) 
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This is the entry function for the rach optmization 
 algorithm.
 ****************************************************************************/

static son_void_t
rach_optimization_algo
(
 rach_opt_cell_context_t     *p_cell_ctxt,
 rach_opt_algo_counters_context_t *p_rach_algo_counters,
 anr_cell_context_node_t *p_anr_cell_ctxt_node,
 son_duplex_mode_et      duplex_mode
 )
{
	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt ||SON_PNULL == p_rach_algo_counters ||
			SON_PNULL  == p_anr_cell_ctxt_node ||
			SON_MAX_NUM_OF_DUPLEX_MODES <= duplex_mode)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_ERROR,
				"Atleast one of the pointer "
				"(p_cell_ctxt , p_rach_algo_counters,p_anr_cell_ctxt_node )"
				" is SON_PNULL or duplex mode is set to invaild value[%d]",
				duplex_mode);

		SON_UT_TRACE_EXIT();
		return;
	}

	if (SON_FALSE == rach_opt_is_complete_rach_config_available(p_cell_ctxt,
				p_anr_cell_ctxt_node))
	{
		/*Reset the message based counters*/
		p_cell_ctxt->is_algo_counters_updated_from_rrm_ue_report = SON_FALSE;
		p_cell_ctxt->is_algo_counters_updated_from_rrm_l2_report = SON_FALSE;
		rach_opt_clear_strongest_nbr_cells_list(p_cell_ctxt);
		SON_MEMSET(&p_cell_ctxt->rach_algo_counters, 0,
				sizeof(rach_opt_algo_counters_context_t));
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"RACH OPT algorithm is not executed "
				"since Cell Info indication message received with incomplete "
				"RACH configuration data for Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		SON_UT_TRACE_EXIT();
		return;
	}

	print_rach_algo_counters(p_cell_ctxt);

	/* If the algo has not started and the condition for triggering the
	   rach algorithm is not met */
	if (SON_FALSE == p_cell_ctxt->is_rach_algo_started)
	{  
		if((p_cell_ctxt->rach_algo_counters.
					percentage_of_samples_with_atmpt_more_than_thresh <
					p_cell_ctxt->oam_config_params.
					rachopt_start_threshold))
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Algo will not "
					"be triggered since percentage of RACH samples with "
					"attempts more than  start threshold: [%f]%% is less than "
					"RACH OPT start threshold: [%f]%%.",
					p_cell_ctxt->rach_algo_counters.
					percentage_of_samples_with_atmpt_more_than_thresh,
					p_cell_ctxt->oam_config_params.
					rachopt_start_threshold);

			rach_opt_init_rach_counters(p_cell_ctxt);

			SON_UT_TRACE_EXIT();
			return;
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Algo will "
					"be triggered since percentage of RACH samples with "
					"attempts more than  start threshold: [%f]%% is >= "
					"RACH OPT start threshold: [%f]%%.",
					p_cell_ctxt->rach_algo_counters.
					percentage_of_samples_with_atmpt_more_than_thresh,
					p_cell_ctxt->oam_config_params.
					rachopt_start_threshold);
		}
	}
	else
	{
		if (p_rach_algo_counters->
				percentage_of_samples_with_atmpt_more_than_thresh <=
				p_cell_ctxt->oam_config_params.rachopt_stop_threshold)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Optimization "
					"count[attempt:%d], percentage of rach samples with "
					"attempts more than configured threshold is [%f]%% "
					"is <= stop threshold[%f]%%."
					"Algorithm stops gracefully and counters shall be reset",
					p_cell_ctxt->rach_algo_run_count_after_started + 1,
					p_rach_algo_counters->
					percentage_of_samples_with_atmpt_more_than_thresh,
					p_cell_ctxt->oam_config_params.rachopt_stop_threshold);

			/*Reset counters*/
			rach_opt_init_rach_counters(p_cell_ctxt);
			SON_UT_TRACE_EXIT();
			return;
		}

		if (p_rach_algo_counters->rach_failure_percentage >= 
				p_cell_ctxt->previous_rach_failure_percentage &&
				((p_rach_algo_counters->rach_failure_percentage - 
				  p_cell_ctxt->previous_rach_failure_percentage) >=
				 p_cell_ctxt->oam_config_params.rachopt_degrade_stop_offset))
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Optimization count"
					"[attempt:%d], Rach failure "
					"pecentage due to last optimization done is" 
					"[%f]%% and the rach failure percentage before "
					"that is [%f]%% difference >= configured degrade offset[%f]%%."
					"Algorithm hard stops,counters shall"
					"be reset and failure notifcation shall "
					"be triggered to MIF",
					p_cell_ctxt->rach_algo_run_count_after_started +1,
					p_rach_algo_counters->rach_failure_percentage,
					p_cell_ctxt->previous_rach_failure_percentage,
					p_cell_ctxt->oam_config_params.rachopt_degrade_stop_offset);

			/*Send the notification to MIF due to increase in failures
			  due to previous attempt.*/
			rach_opt_send_algo_failure_notifcation_to_oam(
					p_cell_ctxt,
					SON_RACH_OPT_RACH_DEGRADED,
					p_cell_ctxt->previous_optimization_type);

			/*Reset counters*/
			rach_opt_init_rach_counters(p_cell_ctxt);

			SON_UT_TRACE_EXIT();
			return;
		}

		if (p_cell_ctxt->rach_algo_run_count_after_started >=
				p_cell_ctxt->oam_config_params.
				rachopt_max_iterations_to_reach_stop_threshold)
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"The total number of rach "
					"optmization attempts since start of algo: [%d] >= [%d] "
					"max number of iterations allowed. The rach failure "
					"percentage is [%f]%% from last optimization attempt."
					" Since no more iterations are allowed the Algorithm stops, "
					"counters shall be reset and notifcation shall be "
					"triggered to OAM",
					p_cell_ctxt->rach_algo_run_count_after_started +1,
					p_cell_ctxt->oam_config_params.
					rachopt_max_iterations_to_reach_stop_threshold,
					p_rach_algo_counters->rach_failure_percentage);

			/*Send the failure notification to MIF
			  As the number of iteration allowed is 
			  exhaused. */
			rach_opt_send_algo_failure_notifcation_to_oam(
					p_cell_ctxt,
					SON_RACH_OPT_MAX_ITERATION_RCHD,
					p_cell_ctxt->previous_optimization_type);

			/*Reset counters*/
			rach_opt_init_rach_counters(p_cell_ctxt);

			SON_UT_TRACE_EXIT();
			return;
		}
	}/* end of if statement when Algo is already running */


	if (p_rach_algo_counters->samples_with_atmpt_more_than_thresh)
	{
		if (p_rach_algo_counters->contention_percentage < 
				p_cell_ctxt->oam_config_params.rachopt_contention_threshold)
		{

			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,
					SON_INFO,
					"Optimization count[attempt:%d] Contention [%f]%% <"
					" contention threshold [%u]%% so it is non contention situation",
					p_cell_ctxt->rach_algo_run_count_after_started +1,
					p_rach_algo_counters->contention_percentage,
					p_cell_ctxt->oam_config_params.
					rachopt_contention_threshold);

			if((p_rach_algo_counters->
						percentage_of_centre_samples_with_power_issue >= 
						p_cell_ctxt->oam_config_params.
						rachopt_power_rampup_threshold))
			{

				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,
						SON_INFO,
						"Centre rach samples with power issue [%f]%% >="
						" Configured power ramp up threshold:[%u]%%."
						" It is power issue",
						p_rach_algo_counters->
						percentage_of_centre_samples_with_power_issue,
						p_cell_ctxt->oam_config_params.
						rachopt_power_rampup_threshold);

				if (p_cell_ctxt->preamble_initial_rcvd_target_power + 1 >
						SON_OAM_PREAMBLE_POWER_DBM_90)
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode,
							p_son_rach_opt_facility_name,SON_WARNING,
							"Optimization "
							"count[attempt:%d], Initial power "
							"can not be proposed to be increased as the "
							"serving cell is already having its power set to [%d]."
							"Algorithm hard stops here, counters shall be reset"
							" and failure notfication shall be send to MIF",
							p_cell_ctxt->
							rach_algo_run_count_after_started +1,
							p_cell_ctxt->preamble_initial_rcvd_target_power);

					/*Populate and send the notifcation to MIF*/
					rach_opt_send_algo_failure_notifcation_to_oam(
							p_cell_ctxt,
							SON_RACH_OPT_INITIAL_POWER_LIMIT_REACHED,
							SON_RACH_OPT_INITIAL_POWER_OPTIMIZATION);

					/*Reset counters*/
					rach_opt_init_rach_counters(p_cell_ctxt);
				}
				else
				{
					/*Send the new proposed value to MIF 
					  and update the counters.*/
					son_rach_opt_parameter_update_req_t 
						notifcation_msg = {0}; 

					SON_MEMCPY(&notifcation_msg.srv_cgi,
							&p_cell_ctxt->cell_id,
							sizeof(son_intra_rat_global_cell_id_t));

					notifcation_msg.update_param.
						preamble_initial_rcvd_target_power
						= (son_preamble_init_rec_target_pow_et)(p_cell_ctxt->
								preamble_initial_rcvd_target_power + 1);

					notifcation_msg.update_param.bitmask
						|= SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT;


					SON_MEMCPY(&p_cell_ctxt->last_update_req_data,
							&notifcation_msg.update_param,
							sizeof(son_oam_rach_opt_update_req_param_t));


					son_create_send_buffer((son_u8 *)&notifcation_msg,
							SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID,
							SONRACH_OPT_PARAMETER_UPDATE_REQ,
							sizeof(notifcation_msg));

					update_local_counters_and_reset_msg_counters(
							p_cell_ctxt,
							SON_RACH_OPT_INITIAL_POWER_OPTIMIZATION);
				}

				SON_UT_TRACE_EXIT();
				return;
			}
			else
			{
				son_u32                 strng_cell_count      = 0;
				rach_opt_strongest_nbr_cell_node_t 
					*p_strng_nbr_cell_node = SON_PNULL;


				/*Get the PCI with the maximun occurance*/
				p_strng_nbr_cell_node = 
					rach_opt_get_first_strongest_nbr_cell(&p_cell_ctxt
							->rach_opt_strongest_nbr_cell_list);
				strng_cell_count = list_count(&p_cell_ctxt->
						rach_opt_strongest_nbr_cell_list);

				if (SON_PNULL == p_strng_nbr_cell_node)
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode,
							p_son_rach_opt_facility_name,SON_WARNING,
							"Optmization "
							"attempt[count:%d],Number of strongest cells "
							"provided by the UEs in the RACH sample is [%d]."
							" Interference algorithm can not be triggered",
							p_cell_ctxt->rach_algo_run_count_after_started +1,
							strng_cell_count);

					/*Reset the message based counters*/
					p_cell_ctxt->is_algo_counters_updated_from_rrm_ue_report 
						= SON_FALSE;
					p_cell_ctxt->is_algo_counters_updated_from_rrm_l2_report 
						= SON_FALSE;
					rach_opt_clear_strongest_nbr_cells_list(p_cell_ctxt);
					SON_MEMSET(&p_cell_ctxt->rach_algo_counters, 0,
							sizeof(rach_opt_algo_counters_context_t));

					SON_UT_TRACE_EXIT();
					return;
				}

				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,
						SON_INFO,
						"Centre rach samples with power issue is [%f]%% <"
						" Configured power ramp up threshold:[%u]%%."
						" It is Interference issue",
						p_rach_algo_counters->
						percentage_of_centre_samples_with_power_issue,
						p_cell_ctxt->oam_config_params.
						rachopt_power_rampup_threshold);

				rach_optimization_algo_for_interference(
						p_cell_ctxt,
						p_rach_algo_counters,
						p_anr_cell_ctxt_node,
						p_strng_nbr_cell_node,
						strng_cell_count,
						duplex_mode);
			}
		}
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
					p_son_rach_opt_facility_name,
					SON_DETAILED,
					"Optimization "
					"count[attempt:%d] Contention percentage: "
					"[%f]%% >= Contention threshold: [%u]%%. It is contention situation",
					p_cell_ctxt->rach_algo_run_count_after_started +1,
					p_rach_algo_counters->contention_percentage,
					p_cell_ctxt->oam_config_params.
					rachopt_contention_threshold);
			rach_opt_contention_based_optimization_algo(p_cell_ctxt, 
					p_rach_algo_counters, duplex_mode, p_anr_cell_ctxt_node);
		}
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : rach_opt_is_report_need_to_be_processed
 * Inputs         : p_son_api_name Pointer to the API name
 * p_cell_ctxt     - Pointer to the serving cell context
 * Outputs        : None
 * Returns        : TRUE If the reports needs to be processed FALSE otherwise
 * Description    : This is the function to check if RRM_SON_RACH_UE_INFO_IND
 *                  and RRM_SON_L2_RACH_PERF_REPORT report received from RRM 
 *                  can be processed or not based on the current RACH OPT algo state
 ****************************************************************************/
static son_bool_et
rach_opt_is_report_need_to_be_processed
(
 const son_8                 *p_son_api_name,
 rach_opt_cell_context_t     *p_cell_ctxt
 )
{
	son_bool_et is_report_need_to_be_processed = SON_TRUE;
	son_u16 threshold = 2 * p_cell_ctxt->read_only_config_params.
		rachopt_no_of_windows_to_skip_for_optimization;
	SON_UT_TRACE_ENTER();

	if (0 < p_cell_ctxt->no_of_windows_to_be_skipped_for_optimization)
	{
		p_cell_ctxt->no_of_windows_to_be_skipped_for_optimization--;
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
				p_son_rach_opt_facility_name, SON_INFO,
				"%s [msg number %u] received from RRM is ignored since "
				"num of messages to be ignored after receiving "
				"cell info indication msg from OAM is configured "
				"to be %u.", p_son_api_name,
				p_cell_ctxt->read_only_config_params.
				rachopt_no_of_windows_to_skip_for_optimization - 
				p_cell_ctxt->no_of_windows_to_be_skipped_for_optimization,
				p_cell_ctxt->read_only_config_params.
				rachopt_no_of_windows_to_skip_for_optimization);
		is_report_need_to_be_processed = SON_FALSE;
	}
	else if (SON_TRUE == p_cell_ctxt->is_cell_info_ind_awaited)
	{
		p_cell_ctxt->reports_ignored_in_cell_info_ind_awaited_state++; 
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Cell Info Indication is awaited to apply the RACH OPT "
				"algo previous iteration results so "
				"%s [msg number %u] received from "
				"RRM is ignored.", p_son_api_name,
				p_cell_ctxt->reports_ignored_in_cell_info_ind_awaited_state);
		if (p_cell_ctxt->reports_ignored_in_cell_info_ind_awaited_state == 
				threshold)
		{
			p_cell_ctxt->reports_ignored_in_cell_info_ind_awaited_state = 0;
			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_WARNING,
					"Cell Info Indication is not received and already "
					"threshold [%u] number of %s reports "
					"ignored from RRM so reset algo.", 
					threshold, p_son_api_name);
			rach_opt_init_rach_counters(p_cell_ctxt);
		}
		is_report_need_to_be_processed = SON_FALSE;
	}


	SON_UT_TRACE_EXIT();
	return is_report_need_to_be_processed;
} /* rach_opt_is_report_need_to_be_processed */
/* SPR-18438 START */
/* code deleted */
/* SPR-18438 END */

son_void_t
sonrach_opt_cell_fsm_ue_info_ind_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rrm_rach_ue_info_ind_t  *p_rach_ue_report = 
		(son_rrm_rach_ue_info_ind_t *)p_rach_opt_msg;
	anr_cell_context_node_t *p_anr_cell_ctxt_node = SON_PNULL;
	SON_LIST_NODE           *p_list_node          = SON_PNULL;
	son_duplex_mode_et      duplex_mode           = SON_INVALID_MODE;
	son_rach_opt_ue_info_report_t *p_ue_info_report = SON_PNULL;
	son_size_t ue_info_report_len = sizeof(son_rach_opt_ue_info_report_t);
	son_u16 idx = 0;
	son_u8 strong_cell_idx = 0;
	SON_UT_TRACE_ENTER();

	if (SON_MODE_CENTRALIZED == g_rach_opt_gb_ctxt.rachopt_mode)
	{
		/* Forward the report to OAM */
		p_ue_info_report = (son_rach_opt_ue_info_report_t *)
			son_mem_get(ue_info_report_len);
		if (SON_PNULL != p_ue_info_report)
		{
			SON_MEMSET(p_ue_info_report, 0, ue_info_report_len);
			SON_MEMCPY(&p_ue_info_report->srv_cgi, &p_rach_ue_report->cell_id, 
					sizeof(son_intra_rat_global_cell_id_t));
			if (SON_MAX_NUMBER_OF_PRACH_SAMPLES >= p_rach_ue_report->count)
			{
				p_ue_info_report->count = p_rach_ue_report->count;
				for (idx = 0; idx < p_ue_info_report->count; idx++)
				{
					switch (p_rach_ue_report->rach_data[idx].cause)
					{
						case RRC_CONNECTED_UE:
							{
								p_ue_info_report->rach_data[idx].cause = 
									SON_RRC_CONNECTED_UE;
								break;
							}
						case RRC_RE_ESTABLISHMENT_UE:
							{
								p_ue_info_report->rach_data[idx].cause =
									SON_RRC_RE_ESTABLISHMENT_UE;
								break;
							}
						case IN_SYNC_IND_UE:
							{
								p_ue_info_report->rach_data[idx].cause =
									SON_IN_SYNC_IND_UE;
								break;
							}
					}
					p_ue_info_report->rach_data[idx].number_of_preambles_sent = 
						p_rach_ue_report->rach_data[idx].
						number_of_preambles_sent;
					switch (p_rach_ue_report->rach_data[idx].
							contention_detected)
					{
						case RRM_FALSE:
							{
								p_ue_info_report->rach_data[idx].
									contention_detected = SON_FALSE;
								break;
							}
						case RRM_TRUE:
							{
								p_ue_info_report->rach_data[idx].
									contention_detected = SON_TRUE;
								break;
							}
					}
					switch (p_rach_ue_report->rach_data[idx].ue_location)
					{
						case cell_centre:
							{
								p_ue_info_report->rach_data[idx].ue_location = 
									SON_CELL_CENTRE;
								break;
							}
						case cell_edge:
							{
								p_ue_info_report->rach_data[idx].ue_location = 
									SON_CELL_EDGE;
								break;
							}
					}
					p_ue_info_report->rach_data[idx].strong_cell_info_count = 
						p_rach_ue_report->rach_data[idx].strong_cell_info_count;
					for (strong_cell_idx = 0; 
							strong_cell_idx < 
							p_ue_info_report->rach_data[idx].
							strong_cell_info_count;
							strong_cell_idx++)
					{
						p_ue_info_report->rach_data[idx].strong_cell_info[
							strong_cell_idx].pci = p_rach_ue_report->
							rach_data[idx].strong_cell_info[strong_cell_idx].
							pci;
						p_ue_info_report->rach_data[idx].strong_cell_info[
							strong_cell_idx].rsrp = p_rach_ue_report->
							rach_data[idx].strong_cell_info[strong_cell_idx].
							rsrp;
					}
				}
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,SON_INFO,
						"Sent "
						"SONRACH_OPT_UE_INFO_REPORT message to SONMgmtIfH in "
						"centralized mode for serving Cell 0x%x",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
				son_create_send_buffer((son_u8 *)p_ue_info_report,
						SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
						SONRACH_OPT_UE_INFO_REPORT, ue_info_report_len);
			}
			else
			{
				SON_LOG(g_rach_opt_gb_ctxt.log_mode,
						p_son_rach_opt_facility_name,SON_WARNING,
						"Invalid "
						"count: %u received in RRM_SON_RACH_UE_INFO_IND "
						"message from RRM so ignoring this report.", 
						p_rach_ue_report->count);
			}
			son_mem_free(p_ue_info_report);
		} 
		else
		{
			SON_LOG(g_rach_opt_gb_ctxt.log_mode,
					p_son_rach_opt_facility_name, SON_ERROR,
					"Memory allocation failed for p_ue_info_report"
					"Cannot send SONRACH_OPT_UE_INFO_REPORT message to "
					"SONMgmtIfH in centralized mode for serving Cell 0x%x",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity));
		}
		SON_UT_TRACE_EXIT();
		return;
	}

	/* Check if this report is to be ignored or processed */
	if (SON_FALSE == rach_opt_is_report_need_to_be_processed(
				"RRM_SON_RACH_UE_INFO_IND", p_cell_ctxt))
	{ 
		SON_UT_TRACE_EXIT();
		return;
	}

	p_list_node = list_find_node(&g_anr_context.cell_ctxt_list,
			&p_cell_ctxt->cell_id, son_key_of_anr,
			son_compare_cell_id);

	if (SON_PNULL == p_list_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"No Serving Cell 0x%x is configured at ANR. "
				"Optimization algo shall not "
				"trigger and counter gets reset(if any)",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
        LTE_GCC_UNUSED_PARAM(evnt_id);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}

	p_anr_cell_ctxt_node = YMEMBEROF(
			anr_cell_context_node_t,sNode, p_list_node);

	if (ANR_CELL_STATE_ENABLED != 
			p_anr_cell_ctxt_node->data.current_cell_fsm_state)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"Serving Cell 0x%x configured at ANR is not is ENABLED state."
				" Optimization algo shall not "
				"trigger and counter gets reset(if any).",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}

	/*Get the duplex mode of the serving cell*/
	duplex_mode =  anr_get_duplex_mode_of_serving_cell(
			&p_anr_cell_ctxt_node->data);

	if (SON_INVALID_MODE == duplex_mode)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"Serving Cell 0x%x is having [%d] duplex mode "
				"[0 -FDD and 1 -  TDD]."
				" Optimization algorithm can not be triggered and "
				" counters get reset(if any).",
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
				duplex_mode);

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}

	initialize_algo_counters_from_rrm_ue_report(p_cell_ctxt);
	p_cell_ctxt->is_algo_counters_updated_from_rrm_ue_report = 
		SON_FALSE;

	/* If the number of rach samples is lesser than what is expected then the 
	   collection recieved is not a quality collection. It is not an error 
	   scenario */
	if (p_rach_ue_report->count < 
			p_cell_ctxt->oam_config_params.rachopt_min_ue_rach_report_count)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Total [%d] PRACH samples recieved "
				"in the RRM_SON_RACH_UE_INFO_IND message when the minimum "
				"number of the rach samples required is [%d]. "
				"This is not a quality sample to trigger alorithm.",
				p_rach_ue_report->count,
				p_cell_ctxt->oam_config_params.
				rachopt_min_ue_rach_report_count);
	}
	else
	{
		if (SON_FAILURE == rach_opt_calculate_rach_algo_counters(
					p_cell_ctxt,p_rach_ue_report,
					&p_cell_ctxt->rach_algo_counters))
		{
			initialize_algo_counters_from_rrm_ue_report(p_cell_ctxt);

			SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
					SON_ERROR,
					"Counters are wrongly computed.");
			SON_UT_TRACE_EXIT();
			return;
		}
        /* SPR-18438 START */
        /* code deleted */
        /* SPR-18438 END */

		p_cell_ctxt->is_algo_counters_updated_from_rrm_ue_report = SON_TRUE;
		if (SON_TRUE == p_cell_ctxt->
				is_algo_counters_updated_from_rrm_l2_report)
		{
			rach_optimization_algo(p_cell_ctxt,
					&p_cell_ctxt->rach_algo_counters,
					p_anr_cell_ctxt_node,
					duplex_mode);
		}
	}

	SON_UT_TRACE_EXIT();
	return;
} /* sonrach_opt_cell_fsm_ue_info_ind_handler */

/*****************************************************************************
 * Function Name  : sonrach_opt_cell_fsm_l2_ind_handler
 * Inputs         : p_cell_ctxt       - Pointer to the serving cell context.
 * Outputs        : 
 * Returns        : son_void_t 
 * Description    : This is the handler function for RRM_SON_L2_RACH_PERF_REPORT
 from RRM.
 ****************************************************************************/
son_void_t
sonrach_opt_cell_fsm_l2_ind_handler
(
 son_rach_opt_cell_index_et  evnt_id,
 STACKAPIHDR                 *p_cspl_hdr,
 rach_opt_cell_context_t     *p_cell_ctxt,
 son_u8                      *p_rach_opt_msg
 )
{
	son_rrm_l2_rach_perf_report_t *p_l2_report  = 
		(son_rrm_l2_rach_perf_report_t *)p_rach_opt_msg;
	anr_cell_context_node_t *p_anr_cell_ctxt_node = SON_PNULL;
	SON_LIST_NODE           *p_list_node          = SON_PNULL;
	son_duplex_mode_et      duplex_mode           = SON_INVALID_MODE; 
	son_rach_opt_l2_perf_report_t l2_perf_report;
	son_size_t l2_perf_report_len = sizeof(son_rach_opt_l2_perf_report_t);
	SON_UT_TRACE_ENTER();

	if (SON_MODE_CENTRALIZED == g_rach_opt_gb_ctxt.rachopt_mode)
	{
		/* Forward the report to OAM */
		SON_MEMSET(&l2_perf_report, 0, l2_perf_report_len);
		SON_MEMCPY(&l2_perf_report.src_cgi, &p_l2_report->cell_id, 
				sizeof(son_intra_rat_global_cell_id_t));
		l2_perf_report.num_of_l2_perf_accumulated_reports = 
			p_l2_report->num_of_l2_perf_accumulated_reports;
		l2_perf_report.total_rcvd_msg3_ra_preambles_cbra = 
			p_l2_report->total_rcvd_msg3_random_access_preambles_cbra;
		l2_perf_report.total_rcvd_msg3_ra_preambles_cfra =
			p_l2_report->total_rcvd_msg3_random_access_preambles_cfra;
		l2_perf_report.total_failed_ra_preambles_msg2_cbra =
			p_l2_report-> total_failed_random_access_preambles_msg2_cbra;
		l2_perf_report.total_failed_ra_preambles_msg2_cfra =
			p_l2_report->total_failed_random_access_preambles_msg2_cfra;
		l2_perf_report.total_allocated_ra_preambles = 
			p_l2_report->total_allocated_random_access_preambles;
		SON_MEMCPY(l2_perf_report.total_succ_ra_ta, 
				p_l2_report->total_succ_random_access_ta,
				sizeof(son_u64) * MAX_TA_RANGE);
		l2_perf_report.total_unassigned_false_cfra = 
			p_l2_report->total_unassigned_false_cfra;
		l2_perf_report.total_unassigned_sum_cfra =
			p_l2_report->total_unassigned_sum_cfra;
/* SPR-18438 START */
        l2_perf_report.num_random_access_preambles_group_a = 
            p_l2_report->num_random_access_preambles_group_a;
        l2_perf_report.num_random_access_preambles_group_b = 
            p_l2_report->num_random_access_preambles_group_b;
/* SPR-18438 END */

		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_DETAILED,
				"Sent "
				"RRM_SON_L2_RACH_PERF_REPORT message to SONMgmtIfH in "
				"centralized mode for serving Cell 0x%x",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		son_create_send_buffer((son_u8 *)&l2_perf_report,
				SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
				SONRACH_OPT_L2_PERF_REPORT, l2_perf_report_len);
		SON_UT_TRACE_EXIT();
		return;
	}

	/* Check if this report is to be ignored or processed */
	if (SON_TRUE == p_cell_ctxt->is_cell_info_ind_awaited)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_INFO,
				"Cell Info Indication is awaited to apply the RACH OPT "
				"algo previous iteration results so "
				"RRM_SON_L2_RACH_PERF_REPORT received from RRM is ignored.");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_list_node = list_find_node(&g_anr_context.cell_ctxt_list,
			&p_cell_ctxt->cell_id, son_key_of_anr,
			son_compare_cell_id);

	if (SON_PNULL == p_list_node)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"No Serving Cell 0x%x is configured at ANR."
				" Optimization algo shall not"
                    /*SPR 17777 +-*/
				"trigger and counter gets reset(if any)",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}


	p_anr_cell_ctxt_node = YMEMBEROF(
			anr_cell_context_node_t,sNode, p_list_node);

	if (ANR_CELL_STATE_ENABLED !=
			p_anr_cell_ctxt_node->data.current_cell_fsm_state)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_WARNING,
				"Serving Cell 0x%x configured at ANR is not is ENABLED state."
				"Optimization algo shall not"
				"trigger and counter gets reset(if any)",
				son_convert_char_cell_id_to_int_cell_id(
					p_cell_ctxt->cell_id.cell_identity));

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}

	/*Get the duplex mode of the serving cell*/
	duplex_mode =  anr_get_duplex_mode_of_serving_cell(
			&p_anr_cell_ctxt_node->data);

	if (SON_INVALID_MODE == duplex_mode)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode,
				p_son_rach_opt_facility_name,SON_ERROR,
				"Serving Cell 0x%x is having [%d] duplex mode"
				"[0 -FDD and 1 -  TDD]."
				" Optimization algorithm can not be triggered and"
				" counters get reset(if any)",
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
				duplex_mode);

		/*Reset counters*/
		rach_opt_init_rach_counters(p_cell_ctxt);

		SON_UT_TRACE_EXIT();
		return;
	}


/* SPR-18438 START */
    p_cell_ctxt->rach_algo_counters.num_of_group_a_preambles_observed = 
        p_l2_report->num_random_access_preambles_group_a;
    p_cell_ctxt->rach_algo_counters.num_of_group_b_preambles_observed = 
        p_l2_report->num_random_access_preambles_group_b;
/* SPR-18438 END */
	p_cell_ctxt->rach_algo_counters.total_unassigned_sum_cfra 
		= p_l2_report->total_unassigned_sum_cfra;

	p_cell_ctxt->is_algo_counters_updated_from_rrm_l2_report = SON_TRUE;

	if (SON_TRUE ==  p_cell_ctxt->is_algo_counters_updated_from_rrm_ue_report)
	{
		rach_optimization_algo(p_cell_ctxt,
				&p_cell_ctxt->rach_algo_counters,
				p_anr_cell_ctxt_node,
				duplex_mode);
	}

	SON_UT_TRACE_EXIT();
} /* sonrach_opt_cell_fsm_l2_ind_handler */

/************************* RACH OPT Cell State Machine*********************/
typedef son_void_t (*rach_opt_fsm_evt_handler_t)
	(
	 son_rach_opt_cell_index_et evnt_id,
	 STACKAPIHDR *p_cspl_hdr,
	 rach_opt_cell_context_t *p_cell_ctxt,
	 son_u8 *p_rach_opt_msg
	);

	/***********************************************************************
	 * RACH OPT Cell FSM Entry Point Table
	 **********************************************************************/
	static rach_opt_fsm_evt_handler_t
	son_rach_opt_cell_fsm_table[RACH_OPT_CELL_NUM_OF_STATES]
	[RACH_OPT_CELL_NUM_OF_EVENTS] =
{
	/* State RACH_OPT_CELL_STATE_DISABLED */
	{
		sonrach_opt_cell_fsm_enable_req_handler,
		/* SONRACH_OPT_EVENT_ENABLE_REQ */
		sonrach_opt_cell_fsm_nack_disable_req_handler,
		/* SONRACH_OPT_EVENT_DISABLE_REQ */
		sonrach_opt_cell_fsm_modify_config_req_in_disabled_state_handler,
		/* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_RACH_CONFIG_RES */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_RACH_UE_INFO_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_RACH_L2_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_CELL_STATE_CHANGE_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* SONANR_EVENT_DISABLE_IND */
	},
	/* State RACH_OPT_CELL_STATE_ENABLING */
	{
		son_rach_opt_cell_fsm_nack_enable_req_handler,
		/* SONRACH_OPT_EVENT_ENABLE_REQ */
		sonrach_opt_cell_fsm_disable_req_handler,
		/* SONRACH_OPT_EVENT_DISABLE_REQ */
		sonrach_opt_cell_fsm_nack_modify_config_req_handler,
		/* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
		sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabling_state,
		/* RRM_EVENT_RACH_CONFIG_RES */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_RACH_UE_INFO_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_RACH_L2_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* RRM_EVENT_CELL_STATE_CHANGE_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* SONANR_EVENT_DISABLE_IND */
	},
	/* State RACH_OPT_CELL_STATE_ENABLED */
	{
		son_rach_opt_cell_fsm_nack_enable_req_handler,
		/* SONRACH_OPT_EVENT_ENABLE_REQ */
		sonrach_opt_cell_fsm_disable_req_handler,
		/* SONRACH_OPT_EVENT_DISABLE_REQ */
		sonrach_opt_cell_fsm_modify_config_req_in_enabled_state_handler,
		/* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
		sonrach_opt_cell_fsm_rach_config_resp_handler_in_enabled_state,
		/* RRM_EVENT_RACH_CONFIG_RES */
		sonrach_opt_cell_fsm_ue_info_ind_handler,
		/* RRM_EVENT_RACH_UE_INFO_IND */
		sonrach_opt_cell_fsm_l2_ind_handler,
		/* RRM_EVENT_RACH_L2_IND */
		sonrach_opt_cell_fsm_state_change_ind_handler,
		/* RRM_EVENT_CELL_STATE_CHANGE_IND */
		son_rach_opt_cell_fsm_anr_disable_ind_handler,
		/* SONANR_EVENT_DISABLE_IND */
	},
	/* State RACH_OPT_CELL_STATE_DISABLING */
	{
		sonrach_opt_cell_fsm_enable_req_handler,
		/* SONRACH_OPT_EVENT_ENABLE_REQ */
		sonrach_opt_cell_fsm_nack_disable_req_handler,
		/* SONRACH_OPT_EVENT_DISABLE_REQ */
		sonrach_opt_cell_fsm_nack_modify_config_req_handler,
		/* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
		sonrach_opt_cell_fsm_rach_config_resp_handler_in_disabling_state,
		/* RRM_EVENT_RACH_CONFIG_RES */
		sonrach_opt_cell_fsm_ue_info_ind_handler,
		/* RRM_EVENT_RACH_UE_INFO_IND */
		sonrach_opt_cell_fsm_l2_ind_handler,
		/* RRM_EVENT_RACH_L2_IND */
		sonrach_opt_cell_fsm_state_change_ind_handler,
		/* RRM_EVENT_CELL_STATE_CHANGE_IND */
		sonrach_opt_cell_fsm_invalid_event_handler,
		/* SONANR_EVENT_DISABLE_IND */
	}
};

/*****************************************************************************
 * Function Name  : rach_opt_cell_calculate_msg_index
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 * Outputs        : None
 * Returns        : Index in SONRACH OPT cell FSM
 * Description    : This function computes the index in the RACH OPT cell FSM for the
 *                  given procedure code
 ****************************************************************************/
static son_rach_opt_cell_index_et
rach_opt_cell_calculate_msg_index
(
 STACKAPIHDR         *p_cspl_hdr
 )
{
	son_rach_opt_cell_index_et ret = RACH_OPT_CELL_IDX_NOT_AVAILABLE;

	SON_UT_TRACE_ENTER();

	if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SMIF_CELL_INFO_IND:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
							"MIF->SONRACH:SMIF_CELL_INFO_IND");
					ret = RACH_OPT_CELL_IDX_EVENT_ENABLE_REQ;
					break;
				}
			case SONRACH_OPT_ENABLE_REQ:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
                            "MIF->SONRACH:SONRACH_OPT_ENABLE_REQ");
					ret = RACH_OPT_CELL_IDX_EVENT_ENABLE_REQ;
					break;
				}
			case SONRACH_OPT_DISABLE_REQ:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
                            "MIF->SONRACH:SONRACH_OPT_DISABLE_REQ");
					ret = RACH_OPT_CELL_IDX_EVENT_DISABLE_REQ;
					break;
				}

			case SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
                            "MIF->SONRACH:SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ");
					ret = RACH_OPT_CELL_IDX_EVENT_MODIFY_CONFIG_PARAM_REQ;
					break;
				}
			default:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Invalid API = %u received from SMIF", 
							p_cspl_hdr->api);
					break;
				}
		} /* switch */
	}
	else if (SON_RRM_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case RRM_SON_RACH_CONFIG_RESP:
				{
					ret = RACH_OPT_CELL_IDX_RRM_EVENT_RACH_CONFIG_RESP;
					break;
				}
			case RRM_SON_CELL_STATE_CHANGE_IND:
				{
					ret = RACH_OPT_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND;
					break;
				}
			case RRM_SON_RACH_UE_INFO_IND:
				{
					ret = RACH_OPT_CELL_IDX_RRM_EVENT_UE_INFO_IND;
					break;
				}
			case RRM_SON_L2_RACH_PERF_REPORT:
				{
					ret = RACH_OPT_CELL_IDX_RRM_EVENT_L2_IND;
					break;
				}
			default:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Invalid API = %u received from RRM", p_cspl_hdr->api);
					break;
				}
		} /* switch */
	}
	else if (SON_ANR_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SONANR_DISABLE_IND:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_INFO,
                            "ANR->SONRACH:SONANR_DISABLE_IND");
					ret = RACH_OPT_CELL_IDX_SONANR_CELL_ANR_MODE_IND;
					break;
				}
			default:
				{
					SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
							p_son_rach_opt_facility_name, SON_WARNING,
							"Invalid API = %u received from ANR", 
							p_cspl_hdr->api);
					break;
				}
		}
	}
	else
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Invalid api = %u received from %u", p_cspl_hdr->api, 
				p_cspl_hdr->from);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* rach_opt_cell_calculate_msg_index */

/*****************************************************************************
 * Function Name  : rach_opt_cell_process_msg
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 *                  p_cell_ctxt        Pointer to cell context
 *                  p_rach_opt_msg     Pointer to incoming message
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the entry point to the RACH OPT Cell FSM.
 *                  This function takes the API message received, CSPL header,
 *                  cell context and calls the required event handler function.
 ****************************************************************************/
son_void_t
rach_opt_cell_process_msg
(
 STACKAPIHDR             *p_cspl_hdr,
 rach_opt_cell_context_t *p_cell_ctxt,
 son_u8                  *p_rach_opt_msg
 )
{   rach_opt_cell_fsm_state_et current_state = p_cell_ctxt->current_cell_fsm_state;
	son_rach_opt_cell_index_et event_id = RACH_OPT_CELL_IDX_NOT_AVAILABLE;

	SON_UT_TRACE_ENTER();

	/* Calculate Cell FSM index */
	event_id = rach_opt_cell_calculate_msg_index(p_cspl_hdr);

	/* Validate the index of rach_opt_cell_fsm_table array*/
	if (RACH_OPT_CELL_NUM_OF_EVENTS <= event_id)
	{
		/* No need to print the log here for invalid API Id as it is
		 * already printed in rach_opt_cell_calculate_msg_index function */
		/* Do nothing */
	}
	/* Validate the cell FSM state */
	else if (RACH_OPT_CELL_NUM_OF_STATES <= p_cell_ctxt->current_cell_fsm_state)
	{
		SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
				SON_WARNING,
				"Message with API Id = %u received "
				"in invalid state = %u", p_cspl_hdr->api,
				p_cell_ctxt->current_cell_fsm_state);
	}
	else 
	{
        SON_LOG(anr_get_log_mode(), p_son_rach_opt_facility_name, SON_INFO,
               "cell_id: 0x%x Processing state %s Event %s",
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                SON_RACH_OPT_CELL_FSM_STATES_NAMES[current_state],
                SON_RACH_OPT_CELL_FSM_EVENT_NAMES[event_id]);

		/* Call the appropriate message handler based on the API Id */ 
		(*son_rach_opt_cell_fsm_table[p_cell_ctxt->current_cell_fsm_state]
		 [event_id])(event_id, p_cspl_hdr, p_cell_ctxt, p_rach_opt_msg);

        SON_LOG(anr_get_log_mode(), p_son_rach_opt_facility_name, SON_INFO,
               "cell_id: 0x%x Change state %s->%s Event %s",
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                SON_RACH_OPT_CELL_FSM_STATES_NAMES[current_state],
                SON_RACH_OPT_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
                SON_RACH_OPT_CELL_FSM_EVENT_NAMES[event_id]);

	}

	SON_UT_TRACE_EXIT();
} /* rach_opt_cell_process_msg */


