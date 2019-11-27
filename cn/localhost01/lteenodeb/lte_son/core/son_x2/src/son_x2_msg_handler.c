/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* File Details
* ------------
*  $Id: son_x2_msg_handler.c
*
****************************************************************************
*
* File Description:  This file is the main entry point handler for
*                    SON X2 Module. It handles all the X2 APIs from
*                    L3 X2AP module, processes the message and sends the
*                    responses to the L3 X2AP module
****************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*   Sep, 2012       Shilpi      Initial         Initial
*   July, 2014      Shilpi                      SPR 10331 Fix
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_utils.h>
#include <son_anr_ctxt_mgr.h>
#include <son_x2_ctxt_mgr.h>
#include <son_x2_fsm.h>
#include <son_x2_enb_fsm.h>
#include <rrc_x2apCommon_composer.h>

/****************************************************************************
* Global Variables
****************************************************************************/
const son_8 *p_son_x2_facility_name = "SON_X2";
extern x2_global_context_t g_x2_gb_ctxt;
extern const QWAIT son_waitstruct;

static const son_ul32 son_x2_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_ANR_MODULE_ID,
    SON_ES_MODULE_ID,
    SON_RRC_MODULE_ID
};

/*****************************************************************************
 * Function Name  : son_x2_init
 * Inputs         : p_son_x2_init_data - Pointer to the global context of SONX2
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SON X2 module.
 *                  It sets the SONX2 FSM state to active and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_x2_init
(
    /* Pointer to SONX2 stack initialization data*/
    son_void_t *p_son_x2_init_data
)
{
    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS == x2_init_global_context())
    {
        /* Init advance logger with default log file */
        SON_INIT_LOG(SON_PNULL);

        /* Start timer for pending messages handling */
        if (SON_PNULL == son_start_timer(SON_X2_DEFAULT_TIMER, SON_PNULL,
                0, SON_TRUE))
        {
            SON_LOG(SON_OAM_LOG_ON, p_son_x2_facility_name, SON_ERROR,
                "X2 module failed to start the timer. "
                "Exiting!!!");
        }
        else
        {
            /* Send SMIF_INIT_IND to SONMgmtIfh */
            son_create_send_buffer(SON_PNULL, SON_X2_MODULE_ID,
                SON_MIF_MODULE_ID, SMIF_INIT_IND,
                0);
        }
    }
    else
    {
                /*SPR 17777 +-*/
        SON_LOG(SON_OAM_LOG_ON, p_son_x2_facility_name, SON_ERROR,
            "failed to initialize X2 module global context."
            "p_son_x2_init_data [%p]"
            "Exiting!!! ",p_son_x2_init_data);
                /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();

    /* Returning pointer to the global context of SONX2-unused
     * in current approach */
    return SON_PNULL;
} /* son_x2_init */

/*****************************************************************************
 * Function Name  : son_x2_process_msg
 * Inputs         : p_msg - The message received
 *                  p_cxt - Pointer to the SONX2 global context data (Unused)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONX2 module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_x2_process_msg
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
        /*Coverity 96719 fix*/
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
                "API pointer received is NULL,p_cxt 0x%x",p_cxt);
                /*SPR 17777 +-*/

        /*Coverity 96719 fix*/
    }
    else
    {
        buf_retain_status = son_x2_process_event(p_buf);
    }

    SON_UT_TRACE_EXIT();
    return buf_retain_status;
} /* son_x2_process_msg */

/* SPR 11340 Fix Starts */
/* Line deleted */
/* SPR 11340 Fix Ends */

/*****************************************************************************
 * Function Name  : son_x2_process_time_d_wait_timer
 * Inputs         : p_nbr_enb_hash_tpl - Pointer to the eNB entry in neighbor
 *                  eNB table
 * Outputs        : None
 * Returns        : None
 * Description    : This is the X2 timer handler function which checks the
 *                  expiry of time d wait timer running and takes appropriate
 *                  actions.
 ****************************************************************************/
son_void_t
son_x2_process_time_d_wait_timer
(
    son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl
)
{
    son_time_t current_time, result_time;
    son_s32 time_diff = 0;

    SON_UT_TRACE_ENTER();

    /* Get current time */
    son_waitstruct.walltime(&current_time);
    time_diff = qvTimeDiff(&current_time,
        &p_nbr_enb_hash_tpl->data.time_d_wait_expiry,
        &result_time);
    /* If eNB config update time_d_wait timer is expired. */
    if (0 <= time_diff)
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
            SON_BRIEF,
            "Time to wait timer expired "
            "for eNB 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_nbr_enb_hash_tpl->data.enb_id.enb_id));

        /* If pending eNB config update request is present. */
        /* SPR-10331 Fix Starts */
        if (list_count(&p_nbr_enb_hash_tpl->data.enb_config_cell_info) ||
                list_count(&p_nbr_enb_hash_tpl->data.
                    pending_gu_group_id_data.add_gu_id_list) ||
                list_count(&p_nbr_enb_hash_tpl->data.
                    pending_gu_group_id_data.delete_gu_id_list))
            /* SPR-10331 Fix Ends */
        {
            /* Send the eNB config update request */
            x2_create_send_enb_config_update_req_msg(
                &p_nbr_enb_hash_tpl->data.enb_config_cell_info,
                &p_nbr_enb_hash_tpl->data);
            x2_print_pending_enb_config_update_req_data(
                    &p_nbr_enb_hash_tpl->data.enb_config_cell_info);
            /* SPR-10331 Fix Starts */
            x2_print_pending_gu_group_id_data(&p_nbr_enb_hash_tpl->data);
            x2_print_sent_gu_group_id_data(&p_nbr_enb_hash_tpl->data);
            /* SPR-10331 Fix Ends */
        }

        /* Reset time_d_wait expiry timer to 0 */
        p_nbr_enb_hash_tpl->data.time_d_wait_expiry.s = 0;
        p_nbr_enb_hash_tpl->data.time_d_wait_expiry.us = 0;
    }

    SON_UT_TRACE_EXIT();
} /* son_x2_process_time_d_wait_timer */

/*****************************************************************************
 * Function Name  : son_x2_process_timer
 * Inputs         : p_msg - The message received
 *                  p_cxt - Pointer to the SONX2 global context data (Unused)
 * Outputs        : None
 * Returns        : None
 * Description    : This is the X2 timer handler function which checks the
 *                  expiry of various timers running and takes appropriate
 *                  actions.
 ****************************************************************************/
son_void_t
son_x2_process_timer
(
    son_timer_t timer_id,
    son_void_t *p_timer_data,
    son_void_t *p_cxt_data
)
{
    /*SPR 17777 +-*/
    (void)p_cxt_data;
    (void)p_timer_data;
    (void)timer_id;
    /*SPR 17777 +-*/
    son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /* Iterate through all the eNB entires in neighbor eNB table */
    p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
        &g_x2_gb_ctxt.nbr_enb_context_table);

    while (SON_PNULL != p_nbr_enb_hash_tpl)
    {
        /* SPR 11340 Fix Starts */
        /* Lines deleted */
        /* SPR 11340 Fix Ends */

        /* If time_d_wait value was received for previous eNB config update
         *  request */
        if (p_nbr_enb_hash_tpl->data.time_d_wait_expiry.s ||
                 p_nbr_enb_hash_tpl->data.time_d_wait_expiry.us)
        {
            son_x2_process_time_d_wait_timer(p_nbr_enb_hash_tpl);
        }

        p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
            &g_x2_gb_ctxt.nbr_enb_context_table, &p_nbr_enb_hash_tpl->anchor);
    }

    SON_UT_TRACE_EXIT();
} /* son_x2_process_timer */

/****************************************************************************
* Manifest Structure
****************************************************************************/
const QMANIFEST son_manifest_x2 =
{
    "SON_X2_MODULE",
    SON_X2_MODULE_ID,
    {0, son_x2_init},   /* { early initialization, late initialization } */
    {son_x2_process_msg, son_x2_process_timer}, /* { message processor,
                                                 *  timer handler } */
    {0, 0, 0},          /* { pack, unpack, alloc} */
    son_x2_dst_list     /* destinations */
};
