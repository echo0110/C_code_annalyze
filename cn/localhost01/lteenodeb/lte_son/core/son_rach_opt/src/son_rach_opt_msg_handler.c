/****************************************************************************
 *
 * ARICENT -
 *
 *
 * Copyright (C) 2013 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $$Id: son_rach_opt_msg_handler.c $
 *
 ****************************************************************************
 *
 * File Description: This file contains the manifest structure definition for
 *                   SON RACH OPT module.It also defines the
 *                   init and message  handler functions for the 
 *                   SON RACH OPT module which are used for 
 *                   module initialization and message handling
 *                   respectively.
 *
 ****************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aug, 2013      SON Team    Initial         Initial
 *   Aug, 2014      Shilpi                       SPR 13251 Fix
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_utils.h>
#include <son_defines.h>
#include <son_rach_opt_fsm.h>
#include <son_rach_opt_ctxt_mgr.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern  pending_res_t g_rach_opt_pending_response;
const son_8 *p_son_rach_opt_facility_name = "SON_RACH_OPT";
extern rach_opt_global_context_t g_rach_opt_gb_ctxt;
extern son_global_config_params_t g_config_params;
static const son_ul32 son_rach_opt_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_RRM_MODULE_ID
};

/*****************************************************************************
 * Function Name  : rach_opt_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function initializes the rach opt global context.
 ******************************************************************************/
son_return_et
rach_opt_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_return_et ret_val = SON_SUCCESS;
    SON_UT_TRACE_ENTER();
    g_rach_opt_gb_ctxt.current_fsm_state = RACH_OPT_STATE_INIT;
    /* SPR-13251 Fix Starts */
    g_rach_opt_gb_ctxt.log_level = SON_OAM_LOG_LEVEL_BRIEF;
    /* SPR-13251 Fix Ends */
    SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);

    g_rach_opt_gb_ctxt.log_mode = SON_OAM_LOG_ON;
    g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = SON_PNULL;
    g_rach_opt_gb_ctxt.rach_opt_global_mode = SON_DISABLED;
    g_rach_opt_gb_ctxt.rachopt_mode = g_config_params.rach_config_params.
        rachopt_mode;

    /*SPR-11554-Start*/  
    g_rach_opt_gb_ctxt.enb_type = SON_ENB_TYPE_UNDEFINED;
    /*SPR-11554-End*/

    /*Validate the RACH based parameteres*/
    if (!(SON_MODE_DISTRIBUTED == g_rach_opt_gb_ctxt.rachopt_mode || 
            SON_MODE_CENTRALIZED == g_rach_opt_gb_ctxt.rachopt_mode))
    {
        g_rach_opt_gb_ctxt.rachopt_mode = RACH_OPT_DEFAULT_MODE;
    }

    if(!(SON_RACHOPT_REPORT_PERIODICITY_PRESENT & g_config_params.
                rach_config_params.rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_report_periodicity < RACH_OPT_MIN_REPORT_PERIOIDICITY)||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_report_periodicity > RACH_OPT_MAX_REPORT_PERIOIDICITY))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_report_periodicity =
            RACH_OPT_DEFAULT_REPORT_PERIOIDICITY;
    }

    if(!(SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT & g_config_params.
                rach_config_params.rach_cell_level_config_params.bitmask)
            ||(g_config_params.rach_config_params.
                rach_cell_level_config_params.rachopt_min_ue_rach_report_count 
                < RACH_OPT_MIN_POSSIBLE_MIN_UE_RACH_REPORT_COUNT) || 
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_min_ue_rach_report_count > 
             RACH_OPT_MAX_POSSIBLE_MIN_UE_RACH_REPORT_COUNT))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_min_ue_rach_report_count = 
            RACH_OPT_DEFAULT_MIN_UE_RACH_REPORT_COUNT;
    }

    if(!(SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT & 
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_ue_rach_attempts_threshold < 
             RACH_OPT_MIN_UE_RACH_ATTEMPTS_THRESHOLD)||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_ue_rach_attempts_threshold > 
             RACH_OPT_MAX_UE_RACH_ATTEMPTS_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_ue_rach_attempts_threshold =
            RACH_OPT_DEFAULT_UE_RACH_ATTEMPTS_THRESHOLD;
    }

    if(!(SON_RACHOPT_START_THRESHOLD_PRESENT & 
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask)||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_start_threshold < RACH_OPT_MIN_RACH_START_THRESHOLD)||
            ( g_config_params.rach_config_params.rach_cell_level_config_params.
              rachopt_start_threshold > RACH_OPT_MAX_RACH_START_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_start_threshold = 
            RACH_OPT_DEFAULT_RACH_START_THRESHOLD;
    }

    if(!(SON_RACHOPT_STOP_THRESHOLD_PRESENT &
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_stop_threshold < RACH_OPT_MIN_RACH_STOP_THRESHOLD) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_stop_threshold > RACH_OPT_MAX_RACH_STOP_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_stop_threshold =
            RACH_OPT_DEFAULT_RACH_STOP_THRESHOLD;
    }

    if(!(SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT  &
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask ) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_degrade_stop_offset < 
             RACH_OPT_MIN_RACH_DEGRADE_STOP_OFFSET) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_degrade_stop_offset > 
             RACH_OPT_MAX_RACH_DEGRADE_STOP_OFFSET))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_degrade_stop_offset =
            RACH_OPT_DEFAULT_RACH_DEGRADE_STOP_OFFSET;
    }

    if(!(SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT &
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_max_iterations_to_reach_stop_threshold < 
             RACH_OPT_MIN_POSSIBLE_MAX_ITERATION_TO_REACH_STOP_THRESHOLD) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_max_iterations_to_reach_stop_threshold > 
             RACH_OPT_MAX_POSSIBLE_MAX_ITERATION_TO_REACH_STOP_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_max_iterations_to_reach_stop_threshold = 
            RACH_OPT_DEFAULT_MAX_ITERATION_TO_REACH_STOP_THRESHOLD;
    }

    if(!(SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT & 
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_contention_threshold < RACH_OPT_MIN_CONTENTION_THRESHOLD)||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_contention_threshold > RACH_OPT_MAX_CONTENTION_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_contention_threshold = 
            RACH_OPT_DEFAULT_CONTENTION_THRESHOLD;
    }

    if(!(SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT &
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_power_rampup_threshold < 
             RACH_OPT_MIN_POWER_RAMPUP_THRESHOLD) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_power_rampup_threshold > 
             RACH_OPT_MAX_POWER_RAMPUP_THRESHOLD))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_power_rampup_threshold =
            RACH_OPT_DEFAULT_POWER_RAMPUP_THRESHOLD;
    } 

    if(!(SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT &
                g_config_params.rach_config_params.
                rach_cell_level_config_params.bitmask) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_min_cfra_preambles < 
             RACH_OPT_MIN_POSSIBLE_MIN_CFRA_PREAMBLES) ||
            (g_config_params.rach_config_params.rach_cell_level_config_params.
             rachopt_min_cfra_preambles >
             RACH_OPT_MAX_POSSIBLE_MIN_CFRA_PREAMBLES))
    {
        g_config_params.rach_config_params.rach_cell_level_config_params.
            rachopt_min_cfra_preambles = 
            RACH_OPT_DEFAULT_MIN_CFRA_PREAMBLES;
    }

    g_config_params.rach_config_params.rach_cell_level_config_params.bitmask = 
        SON_RACHOPT_REPORT_PERIODICITY_PRESENT|
        SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT|
        SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT|
        SON_RACHOPT_START_THRESHOLD_PRESENT|
        SON_RACHOPT_STOP_THRESHOLD_PRESENT|
        SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT|
        SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT|
        SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT|
        SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT|
        SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT;

    if((0xFF == g_config_params.rach_config_params.read_only_config_params.
                rachopt_strong_cell_count) ||
            (g_config_params.rach_config_params.read_only_config_params.
             rachopt_strong_cell_count > RACH_OPT_MAX_STRONG_CELL_COUNT))
    {
        g_config_params.rach_config_params.read_only_config_params.
            rachopt_strong_cell_count = 
            RACH_OPT_DEFAULT_STRONG_CELL_COUNT;
    }

    if((0xFF == g_config_params.rach_config_params.read_only_config_params.
                rachopt_no_of_windows_to_skip_for_optimization) ||
            (g_config_params.rach_config_params.read_only_config_params.
             rachopt_no_of_windows_to_skip_for_optimization  > 
             RACH_OPT_MAX_NO_OF_WINDOWS_TO_SKIP_FOR_OPTIMIZATION))
    {
        g_config_params.rach_config_params.read_only_config_params.
            rachopt_no_of_windows_to_skip_for_optimization =
            RACH_OPT_DEFAULT_NO_OF_WINDOWS_TO_SKIP_FOR_OPTIMIZATION;
    }

    if ((0xFF == g_config_params.rach_config_params.read_only_config_params.
                rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold) ||
            (g_config_params.rach_config_params.read_only_config_params.
             rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold  < 
             RACH_OPT_MIN_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD) || 
             (g_config_params.rach_config_params.read_only_config_params.
             rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold  > 
             RACH_OPT_MAX_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD))
    {
        g_config_params.rach_config_params.read_only_config_params.
            rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold =
            RACH_OPT_DEFAULT_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD;
    }

    if((0xFF == g_config_params.rach_config_params.read_only_config_params.
                rachopt_cfra_unused_threshold) ||
            (g_config_params.rach_config_params.read_only_config_params.
             rachopt_cfra_unused_threshold < 
             RACH_OPT_MIN_CFRA_UNUSED_THRESHOLD) ||
             (g_config_params.rach_config_params.read_only_config_params.
              rachopt_cfra_unused_threshold  > 
              RACH_OPT_MAX_CFRA_UNUSED_THRESHOLD))
    {
        g_config_params.rach_config_params.read_only_config_params.
            rachopt_cfra_unused_threshold = 
            RACH_OPT_DEFAULT_CFRA_UNUSED_THRESHOLD;
    }

    rach_opt_print_global_context("rach_opt_init_global_context"); 
    SON_UT_TRACE_EXIT();
    return ret_val;
} /* rach_opt_init_global_context */

/*****************************************************************************
 * Function Name  : son_rach_opt_process_msg
 * Inputs         : p_buf - Pointer to the message received
 *                  p_cxt - Pointer to the SON RACH OPT global context data (Unused)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SON RACH OPT module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_rach_opt_process_msg
(
 son_void_t *p_buf,
 son_void_t *p_cxt
 )
{
    SON_UT_TRACE_ENTER();

    /* Precondition check */
    if (SON_PNULL == p_buf)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
                p_son_rach_opt_facility_name, SON_ERROR,
                /*SPR 17777 +-*/
                "API pointer received is NULL");
        LTE_GCC_UNUSED_PARAM(p_cxt);
    }
    else
    {
        son_rach_opt_process_event(p_buf);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* son_rach_opt_process_msg */

/*****************************************************************************
 * Function Name  : son_rach_opt_init
 * Inputs         : p_son_rach_opt_init_data - Pointer to SON RACH OPT stack
 *                  initialization data (Unused)
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SON RACH OPT module.
 *                  It sets the SON RACH OPT FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_rach_opt_init
(
 /* Pointer to SON RACH OPT stack initialization data. Unused */
 son_void_t *p_son_rach_opt_init_data
 )
{
    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS == rach_opt_init_global_context())
    {
        /* Init advance logger with default log file */
        SON_INIT_LOG(SON_PNULL);

        /* Send SMIF_INIT_IND to SONMgmtIfh */
        son_create_send_buffer(SON_PNULL, SON_RACH_OPT_MODULE_ID,
                SON_MIF_MODULE_ID, SMIF_INIT_IND,
                0);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_rach_opt_facility_name, SON_ERROR,
                "Failed to initialize RACH OPT module global context."
                /*SPR 17777 +-*/
                "Exiting");
        LTE_GCC_UNUSED_PARAM(p_son_rach_opt_init_data);
    }

    SON_UT_TRACE_EXIT();
    /* Returning pointer to the global context of SON RACH OPT-unused
       in current approach */
    return SON_PNULL;
} /* son_rach_opt_init */

/*****************************************************************************
 * Function Name  : rach_opt_cell_info_ind_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This the funciton to clean up pending response strucutre 
 *                  and update cell state for the cells on expiry of expected 
 *                  RACH config response timer. This is used when the 
 *                  response awaited were trigerred due to cell info indication
 *                  requrest received from OAM.
 ****************************************************************************/
static son_void_t 
rach_opt_cell_info_ind_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE *p_node = SON_PNULL;
    cell_status_list_data_t *p_list_node = SON_PNULL;
    rach_opt_cell_context_node_t *p_cell_data = SON_PNULL;
    SON_LIST_NODE           *p_cell_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    rach_opt_print_pending_res("rach_opt_cell_info_ind_timer_handler");
    p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);

    while (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
                p_node);
        /* Find the pointer to RACH OPT cell context for this node */
        p_cell_list_node = list_find_node(&g_rach_opt_gb_ctxt.
                rach_opt_cell_ctxt_list, &p_list_node->cell_status.cgi, 
                son_key_of_rach_opt, son_compare_cell_id);

        if (SON_PNULL != p_cell_list_node)
        {
            p_cell_data = YMEMBEROF(rach_opt_cell_context_node_t, sNode, 
                    p_cell_list_node);
            if (SON_PNULL != p_cell_data)
            {    
                /* Change cell FSM state to RACH_OPT_CELL_STATE_DISABLED */
                SONRACH_OPT_CELL_FSM_SET_STATE(
                        (&p_cell_data->data),
                        p_cell_data->data.previous_cell_fsm_state,
                        p_cell_data->data.current_cell_fsm_state,
                        RACH_OPT_CELL_STATE_DISABLED);
            }
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                    SON_WARNING,
                    "Could not find "
                    "RACH OPT cell context for the Cell 0x%x "
                    "present in pending response list",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_list_node->cell_status.cgi.cell_identity));
        }

        /* Send feature state change indication to SMIF */
        rach_opt_send_feature_state_change_ind(&p_list_node->cell_status.cgi,
                SON_STATE_DISABLED, SON_RRM_REGISTRATION_UNSUCCESSFUL);
        son_mem_free(p_list_node);
        p_node = list_pop_node(&g_rach_opt_pending_response.cell_sts_list);
    }

    SON_UT_TRACE_EXIT();
} /* rach_opt_cell_info_ind_timer_handler */

/*****************************************************************************
 * Function Name  : rach_opt_pending_res_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function cleans up Pending Response Structure and sends
 *                  pending response to OAM on pending response timer expiry.
 ****************************************************************************/
son_void_t
rach_opt_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    if (MIN_PENDING_RES_COUNT < rach_opt_get_pending_res_cell_count())
    {
        api = rach_opt_get_pending_res_api_id();
        if (SONRACH_OPT_ENABLE_RES == api)
        {
            son_rach_opt_enable_res_t res;
            son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);
            SON_MEMSET(&res, 0, enable_res_len);
            res.transaction_id = rach_opt_get_pending_res_transaction_id();
            res.enable_res.error_code = SON_NO_ERROR;

            /* Remove Successful cells from the RACH OPT Pending Response LIST
               and create cell array with entry for each cell for which 
               RACH OPT could not be enabled. */
            rach_opt_construct_cell_arr_from_pending_res_list(
                    &res.enable_res.cell_status_list_size, 
                    res.enable_res.cell_status_list, 
                    RACH_OPT_CELL_STATE_DISABLED);
            if (0 != res.enable_res.cell_status_list_size)
            {
                res.enable_res.result = SON_FAILURE;
                son_create_send_buffer((son_u8 *)&res,
                        SON_RACH_OPT_MODULE_ID,
                        SON_MIF_MODULE_ID,
                        SONRACH_OPT_ENABLE_RES,
                        enable_res_len);
            }
            else
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
                        p_son_rach_opt_facility_name, SON_WARNING,
                        "Pending "
                        "response list is either empty or all pending "
                        "response nodes to start RACH reports are present "
                        "with error code as SON_NO_ERROR on pending "
                        "response timer expiry");
            }
        }
        else if (SONRACH_OPT_DISABLE_RES == api)
        {
            son_rach_opt_disable_res_t res;
            son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);
            SON_MEMSET(&res, 0, disable_res_len);
            res.transaction_id = rach_opt_get_pending_res_transaction_id();
            res.disable_res.error_code = SON_NO_ERROR;

            /* Remove Successful cells from the RACH OPT Pending Response LIST 
               and create cell array with entry for each cell for which RACH 
               OPT could not be disabled. */
            rach_opt_construct_cell_arr_from_pending_res_list(
                    &res.disable_res.cell_status_list_size,
                    res.disable_res.cell_status_list, 
                    RACH_OPT_CELL_STATE_ENABLED);
            if (0 != res.disable_res.cell_status_list_size)
            {
                res.disable_res.result = SON_FAILURE;
                son_create_send_buffer((son_u8 *)&res,
                        SON_RACH_OPT_MODULE_ID,
                        SON_MIF_MODULE_ID,
                        SONRACH_OPT_DISABLE_RES,
                        disable_res_len);
            }
            else
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode, 
                        p_son_rach_opt_facility_name, SON_WARNING,
                        "Pending "
                        "response list is either empty or all pending "
                        "response nodes to stop RACH reports are present with "
                        "error code as SON_NO_ERROR on pending response "
                        "timer expiry");
            }
        }
        else if (SMIF_CELL_INFO_IND == api)
        {
            rach_opt_cell_info_ind_timer_handler();
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                    SON_WARNING,
                    "Invalid API Id %u "
                    "found in pending response list", api);
        }

        /* Set RACH OPT global FSM state to ACTIVE */
        SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
                RACH_OPT_STATE_ACTIVE);

        /* Clear/Reset Pending Response ListRACH_OPT_Pending_Response_LIST. */
        rach_opt_reset_pending_resp_list();
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_WARNING,
                "RACH OPT pending response"
                "timer expired with no data in pending reponse list.");
    }

    SON_UT_TRACE_EXIT();
} /* rach_opt_pending_res_timer_handler */

/*****************************************************************************
 * Function Name  : son_rach_opt_process_timer
 * Inputs         : timer_id
 *                  p_timer_data - Timer data
 *                  p_cxt_data - Pointer to the SON RACH OPT global context data 
 *                  (Unused) 
 * Outputs        : None
 * Returns        : None
 * Description    : This is the RACH OPT timer handler function which checks the
 *                  expiry of various timers running and takes appropriate
 *                  actions.
 ****************************************************************************/
son_void_t
son_rach_opt_process_timer
(
    son_timer_t timer_id,
    son_void_t  *p_timer_data,
    son_void_t  *p_cxt_data
)
{
    SON_UT_TRACE_ENTER();

    /* Check if RACH_OPT_PENDING_RESPONSE_TIMER timer expired */
    if (g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id == timer_id)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_INFO,
                "RACH OPT pending response timer 0x%x"
                /*SPR 17777 +-*/
                " expired",timer_id);
        LTE_GCC_UNUSED_PARAM(p_timer_data);
        LTE_GCC_UNUSED_PARAM(p_cxt_data);
        rach_opt_pending_res_timer_handler();
        g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = SON_PNULL;
    }

    SON_UT_TRACE_EXIT();
} /* son_rach_opt_process_timer */

const QMANIFEST son_manifest_rach_opt =
{
    "SON_RACH_OPT_MODULE",
    SON_RACH_OPT_MODULE_ID,
    {0, son_rach_opt_init},             /*{ early initialization, late initialization }*/
    {son_rach_opt_process_msg, son_rach_opt_process_timer},  /* { message processor, timer handler }*/
    {0, 0, 0},                            /* { pack, unpack, alloc}*/
    son_rach_opt_dst_list               /* destinations*/
};
