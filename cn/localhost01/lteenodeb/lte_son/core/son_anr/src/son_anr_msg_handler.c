/****************************************************************************
*
* ARICENT -
*
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* $Id: son_anr_msg_handler.c $
*
****************************************************************************
*
* File Description: This file contains the definition of manifest structure for
*                   SONANR module. It also defines the init and message
*                   handler functions for the SONANR module which are used
*                   for SONANR initialization and message handling
*                   respectively.
*
****************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Feb, 2012        Komal/Shekhar               Initial
*
*
****************************************************************************/

/****************************************************************************
* Project Include
****************************************************************************/
#include <son_anr_fsm.h>
#include <son_anr_cell_fsm.h>
#include <son_utils.h>
#include <son_anr_ctxt_mgr.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_anr_table.h>
#include <son_x2_intf.h>
#include <son_x2_ctxt_mgr.h>
#include <ylib.h>

/****************************************************************************
* Global Variables
****************************************************************************/
const son_8 *p_son_anr_facility_name = "SON_ANR";
extern son_global_config_params_t g_config_params;

/* SPR 15831 Start */
extern anr_global_t  g_anr_context;
/* SPR 15831 End */

static const son_ul32 son_anr_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_RRM_MODULE_ID,
    SON_NMM_MODULE_ID,
    SON_APPS_MODULE_ID
};

/*****************************************************************************
 * Function Name  : son_anr_init
 * Inputs         : p_son_anr_init_data - Pointer to SONANR stack
 *                  initialization data (Unused)
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SONANR module.
 *                  It sets the SONANR FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_anr_init
(
    /* Pointer to SONANR stack initialization data. Unused */
    son_void_t *p_son_anr_init_data
)
{
    SON_UT_TRACE_ENTER();

    /* Init advance logger with default log file */
    SON_INIT_LOG(SON_PNULL);

    /* Rel 3.0 Multi Sector code changes Start */
    /* Validate the config params */
    if (!(SON_FALSE == g_config_params.anr_config_params.
                anr_auto_maintain_local_cell_as_nbr  ||
                SON_TRUE == 
                g_config_params.anr_config_params.
                anr_auto_maintain_local_cell_as_nbr))
    {
        g_config_params.anr_config_params.
            anr_auto_maintain_local_cell_as_nbr = SON_TRUE;
    }
    /* Rel 3.0 Multi Sector code changes Stop */

    if (SON_SUCCESS == anr_init_global_context())
    {
        anr_init_pending_res(SON_UNDEFINED, SON_PROCEDURE_UNDEFINED);

        /* Send SMIF_INIT_IND to SONMgmtIfh */
        son_create_send_buffer( SON_PNULL, SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID, SMIF_INIT_IND,
            0);
    }
    else
    {
        /*SPR 17777 +-*/
        SON_LOG(SON_OAM_LOG_ON, p_son_anr_facility_name, SON_ERROR,
            "Failed to initialize ANR module global context. "
            "Exiting p_son_anr_init_data %p!!!",p_son_anr_init_data);
        /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();

    /* Returning NULL as it is not being used in current approach */
    return SON_PNULL;
} /* son_anr_init */

/*****************************************************************************
 * Function Name  : son_anr_process_msg
 * Inputs         : p_buf - Pointer to msg buffer
 *                  p_cxt - Pointer to the SONANR global context data (NULL)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONANR module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_anr_process_msg
(
    son_void_t *p_buf,
    son_void_t *p_cxt
)
{
    STACKAPIHDR cspl_info       = {0};
    son_buf_retain_status_et ret             =  RELEASE_SON_BUFFER;
    son_u8                      *p_msg      = (son_u8 *)p_buf;
    son_oam_log_on_off_et log_mode    = anr_get_log_mode();
    anr_fsm_state_et curr_state  = ANR_STATE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_msg)
    {
        /*SPR 17777 +-*/
        SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
            "son_anr_process_msg: API"
            " pointer received is NULL p_cxt %p",p_cxt);
        /*SPR 17777 +-*/
    }
    else
    {
        /*Parse the cspl header*/
        son_parse_cspl_header(p_msg, &cspl_info);

        /* Offset by CV_HDRSIZE to skip the CSPL Header */
        p_msg = p_msg + CV_HDRSIZE;

        curr_state = anr_get_context_state();

        if (ANR_STATE_SHUTDOWN == curr_state)
        {
            SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
                "Message %d received from %d is ignored in ANR_STATE_SHUTDOWN state",
                cspl_info.api, cspl_info.from);
            if (SONX2_ADD_ENB_RES == cspl_info.api)
            {
                x2_clear_eNB_rec_list(&((son_x2_add_enb_res_t *)p_msg)->enb_list);
            }
        }
        else
        {
            SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                "Message %d received from %d",
                cspl_info.api, cspl_info.from);

            /*Remove the invalid apis*/

            if (SMIF_SHUTDOWN_REQ == cspl_info.api)
            {
                /*SPR 17777 +-*/
                ret =  anr_shutdown_handler(p_msg);
            }
            else
            {
                /*If anr_context.current_fsm_state is ANR_STATE_INIT*/
                if (ANR_STATE_INIT == curr_state)
                {
                    ret =  anr_init_state_handler(p_msg,
                        &cspl_info);
                }
                else
                {
                    ret =  anr_active_state_handler(p_msg,
                        &cspl_info);
                }
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_anr_process_msg */

/*****************************************************************************
 * Function Name  : son_anr_timer_handler
 * Inputs         : timer_id   - Identity of the expired timer
 *                  p_timer_buf - Pointer to the received message buffer
 *                  p_gl_ctx - Pointer to global context
 * Outputs        : None
 * Returns        : None
 * Description    : ANR timer expiry handler
 ****************************************************************************/
/* SPR 15831 Start */ 
static void
son_anr_timer_handler
(
    son_timer_t timer_id,     /* Identity of the expired timer */
    void         *p_timer_buf,  /* Pointer to data associated with the timer */
    void         *p_gl_ctx      /* Pointer to the global context data */
)
{
    son_oam_log_on_off_et log_mode    = anr_get_log_mode();
    /* Spr 17753 Changes Start */ 
    anr_cell_context_t * p_cell_context = SON_PNULL;
    anr_timer_expiry_flag_et expiry_flag = UNDEFINED;
    /* Spr 17753 Changes End */
    SON_UT_TRACE_ENTER();

        /*SPR 17777 +-*/
    SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF, "timer_id = %p p_timer_buf %p"
            " p_gl_ctx %p", timer_id,p_timer_buf,p_gl_ctx);
        /*SPR 17777 +-*/
    if(timer_id == g_anr_context.cgi_gaurd_timer_id)
    {
        anr_cleanup_pending_cgi_request();
    }
    /* Spr 17753 Changes Start */ 
    else if(anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired(timer_id,&p_cell_context,&expiry_flag))
    {
        if(expiry_flag == NR_ADD_TIMER_EXPIRED)
        {
            anr_handle_nr_add_timer_expiry(p_cell_context);
        }
        else if(expiry_flag == BACK_OFF_TIMER_EXPIRED)
        { 
            anr_handle_conflict_back_off_timer_expiry(p_cell_context);
        }
    }
    /* Spr 17753 Changes End */
    else
    {

        /* Clear enable disable pending response */
        if (anr_get_pending_res_expected_count() &&
                anr_get_pending_res_ts_diff_from_curr_ts() >= ANR_TIMEOUT_FOR_PENDING_RES)
        {
            anr_pending_res_timer_handler();
            anr_reset_pending_res();
            anr_set_context_state(ANR_STATE_ACTIVE);
        }

        /* Clear X2 Message Pending Response */
        anr_x2_pending_resp_timer_expiry();
        /* Autonomous neighbor Removal */
        anr_automatic_neighbor_removal();
    }

    SON_UT_TRACE_EXIT();
    return;
} /* son_anr_timer_handler */
/* SPR 15831 End */

/****************************************************************************
* Manifest Structure
****************************************************************************/
const QMANIFEST son_manifest_anr =
{
    "SON_ANR_MODULE_ID",
    SON_ANR_MODULE_ID,
    {0, son_anr_init},                  /* { early initialization, late initialization } */
    {son_anr_process_msg, son_anr_timer_handler},       /* { message processor, timer handler } */
    {0, 0, 0},                                  /* { pack, unpack, alloc} */
    son_anr_dst_list                    /* destinations */
};
