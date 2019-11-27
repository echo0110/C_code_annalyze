/****************************************************************************
*
* ARICENT -
*
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* $$Id: son_es_msg_handler.c $
*
****************************************************************************
*
* File Description: This file contains the definition manifest structure for
*                   SONES module. It also defines the init and message
*                   handler functions for the SONES module which are used
*                   used for SONES initialization and message handling
*                   respectively.
*
****************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Feb, 2012        Atul	                    Initial
*  Sep,2012         Atul                   Changes Done to support Release 1.2
*
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_es_cell_fsm.h>
#include <son_utils.h>
#include <son_es_fsm.h>
#include <son_es_ctxt_mgr.h>

/****************************************************************************
* Global Variables
****************************************************************************/
const son_8 *p_son_es_facility_name = "SON_ES";
extern const QWAIT son_waitstruct;
extern switch_on_pending_res_t g_es_switch_on_pending_res;
/* Fix 875 Start */
extern es_global_t g_es_context;
/* Fix 875 End */

static const son_ul32 son_es_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_RRM_MODULE_ID,
    SON_ANR_MODULE_ID,
    SON_X2_MODULE_ID
};

/*****************************************************************************
 * Function Name  : son_es_init
 * Inputs         : p_son_es_init_data - Pointer to SONES stack
 *                  initialization data (Unused, but required as part of CSPL
 *                  signature)
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SONES module.
 *                  It sets the SONES FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.It also reset all the ES data structures.
 ****************************************************************************/
son_void_t *
son_es_init
(
    /* Pointer to SONANR stack initialization data. Unused */
    son_void_t *p_son_es_init_data
)
{
    SON_UT_TRACE_ENTER();

    /* Init advance logger with default log file */
    SON_INIT_LOG(SON_PNULL);

    /*
    ** Reset the ES module level structures
    */
    es_init_global_context();
    es_init_pending_res(SON_UNDEFINED, SON_PROCEDURE_UNDEFINED);

    /* -------------------ES 1.2 Changes Start ------------------------*/
    es_init_switch_on_pending_res(SON_UNDEFINED, SON_PROCEDURE_UNDEFINED);

    /* Start timer for pending messages handling */

    /* Fix 875 Start */
    g_es_context.g_es_timer_id = son_start_timer(DEFAULT_SON_ES_TIMER, SON_PNULL,
            0, SON_TRUE);
    if (SON_PNULL == g_es_context.g_es_timer_id)
    {
    /* Fix 875 End */
            /*SPR 17777 +-*/
        SON_LOG(SON_OAM_LOG_ON, p_son_es_facility_name, SON_ERROR,
            "ES module failed to start the timer. Exiting!!!");
        LTE_GCC_UNUSED_PARAM(p_son_es_init_data);
            /*SPR 17777 +-*/
    }
    /* -------------------ES 1.2 Changes Finish ------------------------*/
    else
    {
        /* Send SMIF_INIT_IND to SONMgmtIfh */
        son_create_send_buffer( SON_PNULL,
            SON_ES_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_INIT_IND,
            0);
    }

    SON_UT_TRACE_EXIT();

    /* Returning NULL as it is not being used in current approach */
    return SON_PNULL;
} /* son_es_init */

/*****************************************************************************
 * Function Name  : son_es_process_msg
 * Inputs         : p_buf - Pointer to the message received
 *                  p_cxt - Pointer to the SONES global context data(Unused,
 *                  but required as part of CSPL signature)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONES module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_es_process_msg
(
    son_void_t *p_buf,
    son_void_t *p_cxt
)
{
    son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;
    STACKAPIHDR cspl_hdr    = {0};
    son_u8                  *p_msg      = (son_u8 *)p_buf;
    son_oam_log_on_off_et log_mode    = es_get_log_mode();
    es_fsm_state_et curr_state  = ES_STATE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    /* Precondition check */
    if (SON_PNULL == p_msg)
    {
        SON_LOG(log_mode,
            p_son_es_facility_name,
            SON_ERROR,
            "API pointer received is NULL");
    }
    else
    {
        /* Parse the CSPL Header */
        son_parse_cspl_header(p_msg, &cspl_hdr);
        curr_state = es_get_context_state();

        if (ES_STATE_SHUTDOWN == curr_state)
        {
            /*SPR 17777 +-*/
            SON_LOG(log_mode,
                p_son_es_facility_name,
                SON_ERROR,
                "Message %d ignored in "
                "ES_STATE_SHUTDOWN state received from %d",
                cspl_hdr.api, cspl_hdr.from);
            LTE_GCC_UNUSED_PARAM(p_cxt);
            /*SPR 17777 +-*/
        }
        else
        {
            /* If API value is SHUTDOWN*/
            if (SMIF_SHUTDOWN_REQ == cspl_hdr.api)
            {
                buf_retain_status =
                    es_shutdown_handler((p_msg + CV_HDRSIZE), &cspl_hdr);
            }
            else
            {
                /* If es_context.current_fsm_state is ES_STATE_INIT */
                if (ES_STATE_INIT == curr_state)
                {
                    buf_retain_status =
                        es_init_state_handler((p_msg + CV_HDRSIZE), &cspl_hdr);
                }
                else
                {
                    buf_retain_status =
                        es_active_state_handler((p_msg + CV_HDRSIZE), &cspl_hdr);
                }
            }
        }
    }

    SON_UT_TRACE_EXIT();

    return buf_retain_status;
} /* son_es_process_msg */

/*****************************************************************************
 * Function Name  : son_es_process_timer
 * Inputs         : timer_id - Timer ID, useful in case of multiple timers
 *                  are running
 *                  p_timer_data - pointer to the timer ctxt stored by CSPL
 *                  p_cxt_data - Pointer to the SONES global context data(Unused,
 *                  but required as part of CSPL signature)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the SONES module Timer function - CSPL
 *                  callback. It is automatically called .
 ****************************************************************************/
son_void_t
son_es_process_timer
(
    son_timer_t timer_id,
    son_void_t *p_timer_data,
    son_void_t *p_cxt_data
)
{
    son_time_t current_time, pending_swithcon_xpry_time, result_time;
    son_s32 time_diff = 0;
    son_es_cell_switchon_for_x2_setup_res_t cell_switchon_x2_res = {0};
    SON_LIST_NODE           *p_node                 = SON_PNULL;
    cell_switchon_status_list_data_t *p_list_node   = SON_PNULL;
    es_cell_context_t       *p_cell_ctxt            = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /*
     * Earlier thought to have X2 Enbale check and only if it is true than
     * to check the pending response for switch on structure. But then removed
     * this check as during run time if x2 enabling is disbled in that case
     * also this pending structure shall be cleaned and ANR must received the
     * response.
     */
    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
            "Timer 0x%x expired", timer_id);

    /* Fix 875 Start */
    if(timer_id == g_es_context.g_es_timer_id)
    {
    /* Fix 875 End */
    if (es_switchon_pending_res_has_entry())
    {
        /* Get current time */
        son_waitstruct.walltime(&current_time);

        pending_swithcon_xpry_time = es_get_switchon_expiry_time();

        time_diff = qvTimeDiff(&current_time,
            &pending_swithcon_xpry_time,
            &result_time);

        /* If reset response message timer is expired */
        if (0 <= time_diff)
        {
            SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
                        "Pending switch on response timer"
                        " expired");

            /*
             *   1. Send response to ANR for its requestu with Failure and error code as SON_ERR_TIMER_EXPIRED.
             */

            cell_switchon_x2_res.transaction_id = es_get_switchon_pending_res_transaction_id();
            cell_switchon_x2_res.result = SON_FAILURE;
            cell_switchon_x2_res.error_code = SON_ERR_TIMER_EXPIRED;

            son_create_send_buffer((son_u8 *)&cell_switchon_x2_res,
                SON_ES_MODULE_ID,
                SON_ANR_MODULE_ID,
                SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES,
                sizeof(cell_switchon_x2_res));

            /*
             *   2.  Send Switch on indication to X2 module as X2
             *       support is enabled
             */
            son_create_and_send_switch_on_ind_to_x2(
                SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ, NULL);

            /*
             *   3. Send Switch off indication to OAM module
             *      for all the other cells whose oam_ind = true
             */
             /* + 17439 */
            son_create_and_send_cell_switch_on_off_ind_to_oam();
             /* - 17439 */

            /*
             *   4. Move Transit state to Previous state as no response is received , hence state shall be in previous stable state
             */
            p_node = get_first_list_node(
                &g_es_switch_on_pending_res.cell_switchon_status_list);

            while (SON_PNULL != p_node)
            {
                p_list_node =  YMEMBEROF(
                    cell_switchon_status_list_data_t,
                    cell_switchon_status_node,
                    p_node);

                if ((p_list_node->cell_switchon_status.result ==
                     SON_FAILURE) &&
                    (p_list_node->cell_switchon_status.error_code ==
                     SON_NO_ERROR))
                {
                    p_cell_ctxt = SON_PNULL;
                    p_cell_ctxt = es_get_cell_context_from_global_context(
                        &p_list_node->cell_switchon_status.cgi);

                    if (SON_PNULL != p_cell_ctxt)
                    {
                        if (ES_CELL_STATE_SWITCHING_ON ==
                            p_cell_ctxt->current_cell_fsm_state)
                        {
                            SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
                                p_cell_ctxt->previous_cell_fsm_state,
                                p_cell_ctxt->current_cell_fsm_state,
                                ES_CELL_STATE_SWITCHED_OFF);
                        }
                        else if (ES_CELL_STATE_SWITCHING_OFF ==
                                 p_cell_ctxt->current_cell_fsm_state)
                        {
                            SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
                                p_cell_ctxt->previous_cell_fsm_state,
                                p_cell_ctxt->current_cell_fsm_state,
                                ES_CELL_STATE_ENABLED);
                        }
                    }
                }

                p_node = get_next_list_node(p_node);
            }

            /*
             *   5. Clear the pending response structure
             */
            es_reset_switch_on_pending_res();
        }
    }
    }
    /* Fix 875 Start */
    else
    {
        son_buf_retain_status_et ret_val = RETAIN_SON_BUFFER;

        son_intra_rat_global_cell_id_t *p_cell_id = 
            (son_intra_rat_global_cell_id_t *)p_timer_data;

        /* Find the cell context for which timer has expired */
        if (SON_PNULL != p_cell_id)
        {
            p_cell_ctxt = es_get_cell_context_from_global_context(
                    p_cell_id);
            if (SON_PNULL == p_cell_ctxt)
            {
                /*SPR 17777 +-*/
                SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
                    "Cell %x not found",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
                LTE_GCC_UNUSED_PARAM(p_cxt_data);                            
                /*SPR 17777 +-*/
            }
            else if((ES_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state) ||
                    (ES_CELL_STATE_SWITCHING_ON == p_cell_ctxt->current_cell_fsm_state)) 
            {
                son_es_cell_switch_off_req_t cell_switch_off_req = {0};
                SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                        "Autonomous Cell Switch OFF to be triggered");
                cell_switch_off_req.transaction_id = son_generate_txn_id();
                cell_switch_off_req.switch_off_req.switch_off_mode = SO_GRACEFUL;

                /* Send Cell Switch OFF Req to RRM Autonomously */
                ret_val = es_fsm_cell_switch_off_req_handler(IDX_NOT_AVAILABLE,
                        p_cell_ctxt,
                        SON_PNULL,
                        (son_u8 *)&cell_switch_off_req);

                if (RELEASE_SON_BUFFER == ret_val)
                {
                    /* Update the Req Sent status and the cause for Switch of Req */
                    p_cell_ctxt->autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM;
                }
                p_cell_ctxt->es_timer_id = SON_PNULL;
            }
            else
            {
                SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_WARNING,
                        "Autonomous Cell Switch OFF can NOT be triggered in cell state = %s",
                        SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state]);

                p_cell_ctxt->es_timer_id = SON_PNULL;
            }
        }
    }
    /* Fix 875 End */
} /* son_es_process_timer */

/****************************************************************************
* Initialization of QManifest Structure for SON ES Module
****************************************************************************/
const QMANIFEST son_manifest_es =
{
    "SON_ES_MODULE_ID",
    SON_ES_MODULE_ID,
    {0, son_es_init},             /*{ early initialization, late initialization }*/
    {son_es_process_msg, son_es_process_timer},  /* { message processor, timer handler }*/
    {0, 0, 0},                            /* { pack, unpack, alloc}*/
    son_es_dst_list               /* destinations*/
};
