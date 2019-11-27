/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_md.c,v 1.4 2010/10/13 07:04:21 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the message handler and timer expiry 
 *                     handlers registered with cspl.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_md.c,v $
 * Revision 1.4  2010/10/13 07:04:21  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.1  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.4  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.3  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.2  2009/12/26 12:05:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.6  2009/11/22 11:50:22  gur18569
 * ut fixes
 *
 * Revision 1.1.2.5  2009/11/20 14:06:48  gur21006
 * Segmentation fault removed
 *
 * Revision 1.1.2.4  2009/11/18 11:07:53  gur20470
 * changed macro MME_MODULE_ID to RRC_MME_MODULE_ID
 *
 * Revision 1.1.2.3  2009/11/10 11:41:55  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.2  2009/11/08 11:06:47  gur20470
 * removed compilation warnings
 *
 * Revision 1.1.2.1  2009/10/23 16:10:17  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "s1ap_uecc.h"
#include "s1ap_timers.h"
#include "s1ap_oam.h"
#include "rrc_msg_mgmt.h"
#include "s1ap_logger.h" 
#include "s1ap_mme_fsm.h"
#include "s1ap_mme.h"
#include "s1ap_csc.h"
#include "s1ap_handlers.h"
#include "s1ap_enb_config_fsm.h"
#include "s1ap_enb_mgr_fsm.h"
#include "s1ap_rrm.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/


/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
extern QLOCK s1ap_lock;
#endif
/*Lockless Implementation stop*/
const S8* rrc_s1ap_log_facility_name = (const S8*)"eNodeB RRC S1AP";

extern void s1ap_process_x2ap_messages(
       void            *p_api,
       s1ap_gb_context_t    *p_s1ap_gb_context);

void s1ap_process_health_monitor_chk
(
     s1ap_gb_context_t    *p_s1ap_gb_context
);
/*Bug 9305 start*/
extern U32 s1ap_health_monitor_val[S1AP_SCTP_MAX_THREADS];
/*Bug 9305 end*/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/*****************************************************************************
 * Function Name  : s1ap_process_msg
 * Inputs         : p_api - Pointer to input API buffer.
 *                  p_s1ap_gl_ctx - Pointer to S1AP global context.
 * Outputs        : None
 * Returns        : RRC_BUFFER_SHOULD_BE_RELEASED -
 *                     Encapsulating shell should release the buffer.
 * Description    : This function handles MME initiaited RESET.
 *                  This is the main entry point of S1AP module - CSPL 
 *                  callback. It receives all the external APIs received 
 *                  from external entities and internal RRC entities and 
 *                  invokes appropriate handlers based on incoming module 
 *                  ID and API ID.This function always returns nonzero value, 
 *                  i.e. the buffer is alwaysreleased by the encapsulating 
 *                  shell.
 *****************************************************************************/
S32 s1ap_process_msg
(
 void *p_api,        /* Pointer to input API buffer */
 void *p_gl_ctx      /* Pointer to the S1AP global context data */
 )
{
    s1ap_gb_context_t *p_s1ap_gl_ctx = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* RRC_S1AP_TRACE(RRC_INFO, RRC_MSG_RECVD); */
    
    /* SPR 19273 Fix Start */
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);
    /* SPR 19273 Fix End */

    /* Precondition check */
    /* Check general message integrity too */
    if ( (PNULL == p_api) || (PNULL == p_gl_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "s1ap_process_msg: "
                "Either api or global ctx in NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }
    else if (RRC_SUCCESS != rrc_check_cspl_header((U8*)p_api))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "s1ap_process_msg: bad message header - "
                "ignore msg");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    p_s1ap_gl_ctx = (s1ap_gb_context_t *)p_gl_ctx;

    switch (rrc_get_src_module_id((U8*)p_api))
    {
        case RRC_UECC_MODULE_ID:
            s1ap_process_uecc_messages(p_api, p_s1ap_gl_ctx);
            break;

        case RRC_OAM_MODULE_ID:
            s1ap_process_oam_messages(p_api, p_s1ap_gl_ctx);
            break;

        case RRC_CSC_MODULE_ID:
            s1ap_process_csc_messages(p_api, p_s1ap_gl_ctx);
            break;

        case RRC_MME_MODULE_ID:
            s1ap_process_mme_messages(p_api,p_s1ap_gl_ctx);
            break;

        case RRC_RRM_MODULE_ID:
            s1ap_process_rrm_messages(p_api, p_s1ap_gl_ctx);
            break;

        case RRC_UECCMD_MODULE_ID:
            s1ap_process_ueccmd_messages(p_api, p_s1ap_gl_ctx);
            break;

        case RRC_X2AP_MODULE_ID:
            s1ap_process_x2ap_messages(p_api, p_s1ap_gl_ctx);
            break;

        default:
            RRC_S1AP_TRACE(RRC_WARNING, "s1ap_process_msg: API from unexpected "
                    "module received");
            break;
    }

    RRC_S1AP_UT_TRACE_EXIT();

    /* Encapsulating shell should release the buffer */
    return RRC_BUFFER_SHOULD_BE_RELEASED;
}


/*****************************************************************************
 * Function Name  : s1ap_process_timer_msg
 * Inputs         : timer_id - The id of timer expired.
 *                  p_timer_buf - Pointer to timer buffer which is allocated
 *                                when starting a timer.
 *                  p_gl_ctx - Pointer to S1AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : It handles all the timer events for S1AP module.
 *                  The timer buffer should always start with 
 *                  s1ap_timer_buffer_t
 *****************************************************************************/
void s1ap_process_timer_msg
(
 rrc_timer_t timer_id,       /* Identity of the expired timer */
 void        *p_timer_buf,   /* Pointer to data associated with the timer */
 void        *p_gl_ctx       /* Pointer to the S1AP global context data */
 )
{
    s1ap_gb_context_t *p_s1ap_gl_ctx = PNULL;
    s1ap_timer_buffer_t *p_s1ap_timer_buf = PNULL;
    mme_context_t       *p_mme_context = PNULL;
    rrc_bool_et is_auto_delete = RRC_FALSE;
    enb_config_update_invoked_et invoked_by = INVOKED_BY_INVALID;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_BRIEF, "Get timer msg.");

    /* Precondition check */
    if ( (PNULL == p_timer_buf) || (PNULL == p_gl_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "s1ap_process_timer_msg: Either timer "
                "buff or global context is NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    p_s1ap_gl_ctx = (s1ap_gb_context_t *)p_gl_ctx;
    p_s1ap_timer_buf = (s1ap_timer_buffer_t *)p_timer_buf;

    is_auto_delete = p_s1ap_timer_buf->auto_delete;

    switch (p_s1ap_timer_buf->timer_type)
    {
        case S1AP_OAM_INIT_TIMER:
            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_OAM_INIT_TIMER expired");
            s1ap_process_init_timer(
                    p_s1ap_gl_ctx);
            break;

        case S1AP_MME_S1_SETUP_RSP_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_MME_S1_SETUP_RSP_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                [p_s1ap_timer_buf->mme_id];
            mme_fsm_process_event (
                    MME_EVENT_S1_SETUP_RSP_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;

        case S1AP_MME_SCTP_CONN_REC_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_MME_SCTP_CONN_REC_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                  [p_s1ap_timer_buf->mme_id];
            mme_fsm_process_event (
                    MME_EVENT_SCTP_CONN_REC_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;

        case S1AP_MME_SCTP_CONNECT_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_MME_SCTP_CONNECT_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                  [p_s1ap_timer_buf->mme_id];
            mme_fsm_process_event (
                    MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;

        case S1AP_MME_RESET_TIMER:
            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_MME_RESET_TIMER expired");
            s1ap_process_mme_reset_timer_expiry(
                    p_timer_buf,
                    p_s1ap_gl_ctx);

            break;

        case S1AP_MME_TIME_TO_WAIT_TIMER:
            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_MME_TIME_TO_WAIT_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                  [p_s1ap_timer_buf->mme_id];
            mme_fsm_process_event (
                    MME_EVENT_S1_SETUP_TIME_TO_WAIT_TIMER_EXPIRY,
                    &timer_id,
                    p_timer_buf,
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;

        case S1AP_ENB_CONFIGURAION_UPDATE_TIMER:
            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_ENB_CONFIGURAION_UPDATE_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                [p_s1ap_timer_buf->mme_id];

            invoked_by = INVOKED_BY_FAILURE;
            /* Try to resend the message for the max re-tries */
            enb_fsm_config_update_process_event(
                    S1AP_MME_EV_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            
            break;

        case S1AP_ENB_CONFIGURATION_TIME_TO_WAIT_TIMER:
            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_ENB_CONFIGURATION_TIME_TO_WAIT_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                [p_s1ap_timer_buf->mme_id];

            invoked_by = INVOKED_BY_OAM;

            ENB_CONFIG_SET_FSM_STATE(p_mme_context, ENB_STATE_RECV_ENB_CONFIG_FROM_OAM);
            /* Try to resend the message for the max re-tries */
            enb_fsm_config_update_process_event(
                    S1AP_MME_EV_ENB_CONFIG_RESEND,/*coverity fix start*/
                    p_timer_buf,/*coverity fix stop*/
                    &invoked_by,
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;
        
       case S1AP_PWS_GUARD_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_PWS_GUARD_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                  [p_s1ap_timer_buf->mme_id];
            enb_mgr_fsm_write_replace_warning_process_event(
                    ENB_EV_PWS_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;
                    
       case S1AP_KILL_GUARD_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_KILL_GUARD_TIMER expired");
            p_mme_context = p_s1ap_gl_ctx->mme_contexts
                                  [p_s1ap_timer_buf->mme_id];
            enb_mgr_fsm_write_replace_warning_process_event(
                    ENB_EV_KILL_TIMER_EXPIRY,/*coverity fix start*/
                    p_timer_buf,
                    p_timer_buf,/*coverity fix stop*/
                    p_s1ap_gl_ctx,
                    p_mme_context);
            break;

       case S1AP_OAM_HEALTH_MONITOR_TIMER:

            RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_OAM_HEALTH_MONITOR_TIMER expired");
            s1ap_process_health_monitor_chk(p_s1ap_gl_ctx);
            break;
	    /*SPR 15570 Fix Start*/
       case S1AP_SCTP_CONN_TIMER :

	    RRC_S1AP_TRACE(RRC_INFO, "s1ap_process_timer_msg: S1AP_SCTP_CONN_TIMER expired");
	    p_mme_context = p_s1ap_gl_ctx->mme_contexts
		    [p_s1ap_timer_buf->mme_id];
	    mme_fsm_process_event (
			    MME_EVENT_SCTP_CONNECT_TIMER_EXPIRY,
			    &timer_id,
			    p_timer_buf,
			    p_s1ap_gl_ctx,
			    p_mme_context);
	    break;
	     
             /*SPR 15570 Fix Stop*/
                    
        default:
            /*SPR_16532_START*/
            RRC_S1AP_TRACE(RRC_ERROR, "s1ap_process_timer_msg: timer event for "
                    "unexpected timer type %u received", p_s1ap_timer_buf->timer_type);
            /*SPR_16532_END*/
            break;
    }

    if (RRC_TRUE == is_auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: s1ap_process_health_monitor_chk 
 *   Inputs:        p_s1ap_gb_context - Pointer to the S1AP global context data
 *   DESCRIPTION:
 *       This function copies global health monitoring variable
 *       in S1AP global context data and resets the global health
 *       monitor variable for S1AP
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void s1ap_process_health_monitor_chk
(
     s1ap_gb_context_t    *p_s1ap_gb_context
)
{
    U8 count = 0;
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    if(1 == p_s1ap_gb_context->gl_s1ap_timer_exp_counter)
    {
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
        if(PNULL != s1ap_lock)
        { 
    	    qvLock(s1ap_lock);
        }
#endif
        /*Bug 9305 start*/
        l3_memcpy_wrapper(&p_s1ap_gb_context->s1ap_health_monitor_val,&s1ap_health_monitor_val,S1AP_SCTP_MAX_THREADS * sizeof(U32));
        /*Bug 9305 end*/
        for(count = 0;count < S1AP_SCTP_MAX_THREADS; count++)
        {
        /*Bug 9305 start*/
            sync_fetch_and_and_wrapper(&s1ap_health_monitor_val[count],0,PNULL); /*Pass NULL as lock is already acquired*/
        /*Bug 9305 end*/
        }
        p_s1ap_gb_context->gl_s1ap_timer_exp_counter = 0;
#ifndef LOCKLESS_IMPL 
        if(PNULL != s1ap_lock)
        { 
	    qvUnlock(s1ap_lock);
        }
#endif
/*Lockless Implementation stop */
    }
    else
    {
        p_s1ap_gb_context->gl_s1ap_timer_exp_counter++;
        rrc_update_health_monitor_variable(S1AP_HEALTH_MONITOR_EXPIRY_EVENT);
    }
    return;
}

