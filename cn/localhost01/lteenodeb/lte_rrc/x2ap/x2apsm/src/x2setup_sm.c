/*******************************************************************************
*
*  FILE NAME   : x2ap_fsm.c
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function definition. 
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_api.h"
#include "x2setup_sm.h"
#include "x2ap_evthdlr.h"
#include "x2ap_tracing.h"
#include "sm_entry.h"
#include "rrc_intrl_api.h"

/*****************************************************************************
 * DATA
 *      DESCRIPTION: This is used for the logging of X2AP States.
 ****************************************************************************/
U8 *X2SETUP_FSM_STATES_NAMES[] =
{
    (U8 *)"X2_SETUP_IDLE_ST",
    (U8 *)"X2_SETUP_W_X2_SETUP_RESP_ST",
    (U8 *)"X2_SETUP_ACTIVE_ST",
    (U8 *)"X2_SETUP_TIME_TO_WAIT_EXP_ST",
    (U8 *)"X2_SETUP_MAX_STATE"
};

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the logging of X2AP Events.
 ******************************************************************************/
U8 *X2SETUP_FSM_EVENTS_NAMES[] =
{
    (U8 *)"X2_SETUP_SCTP_X2_CONN_DOWN_EV",
    (U8 *)"X2_SETUP_SCTP_X2_CONN_UP_EV",
    (U8 *)"CSC_CELL_SETUP_IND",
    (U8 *)"CELL_SETUP_IND_TIMER_EXP",
    (U8 *)"X2_SETUP_REQ_EV",
    (U8 *)"X2_SETUP_RESP_EV",
    (U8 *)"X2_SETUP_FAIL_EV",
    (U8 *)"X2_SETUP_TIME_WAIT_TMR_EXP_EV",
    (U8 *)"X2_SETUP_RESP_TMR_EXP_EV",
    (U8 *)"CSC_CELL_STATUS_RESP",
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    (U8 *)"ENDC_X2_SETUP_REQ_EV",
    (U8 *)"ENDC_X2_SETUP_FAIL_EV",
    /* MENB CHANGES - END */
    #endif
    (U8 *)"X2_SETUP_MAX_EVENT"    
};

x2ap_return_et x2_setup_sm_map_event(U32               api,
                                     en_x2setup_event *p_event);

/****************************************************************************
 * Data Description    : This is Initialization of X2 setup SMc.
 ****************************************************************************/
static x2setup_action_routine x2setup_sm[X2_SETUP_MAX_STATE][X2_SETUP_MAX_EVENT] =
{
    /* X2_SETUP_IDLE_ST */
    {
       sctp_conn_down_evt_hdl,    /* X2_SETUP_SCTP_X2_CONN_DOWN_EV */
       x2setup_request_msg_init,  /* X2_SETUP_SCTP_X2_CONN_UP_EV */
       csc_cell_setup_ind_hdl,    /* CSC_CELL_SETUP_IND */
       cell_setup_ind_tmr_exp_hdl,/* CELL_SETUP_IND_TIMER_EXP */
       x2setup_req_idle_hdl,      /* X2_SETUP_REQ_EV */
       x2ap_unexpected_msg,       /* X2_SETUP_RESP_EV */
       x2ap_unexpected_msg,       /* X2_SETUP_FAIL_EV */
       x2ap_unexpected_msg,       /* X2_SETUP_TIME_WAIT_TMR_EXP_EV */
       x2ap_unexpected_msg,       /* X2_SETUP_RESP_TMR_EXP_EV */
       csc_cell_status_resp_hdl  /* CSC_CELL_STATUS_RESP */
       #ifdef ENDC_ENABLED
       /* MENB CHANGES - START */
       ,endc_x2setup_req_idle_hdl,      /* ENDC_X2_SETUP_REQ_EV */
       x2ap_unexpected_msg,       /* ENDC_X2SETUP_FAIL_EV */
       /* MENB CHANGES - END */
       #endif
    },
    /* X2_SETUP_W_X2_SETUP_RESP_ST */
    {
       sctp_conn_down_evt_hdl,       /* X2_SETUP_SCTP_X2_CONN_DOWN_EV */
       x2ap_unexpected_msg,          /* X2_SETUP_SCTP_X2_CONN_UP_EV */
       csc_ind_in_x2Setup_hdl,       /* CSC_CELL_SETUP_IND */
       x2ap_unexpected_msg,          /* CELL_SETUP_IND_TIMER_EXP */
       /*SPR_18154_START*/
       x2_setup_req_w_x2_setup_resp_evt_hdl,    /* X2_SETUP_REQ_EV */
       /*SPR_18154_END*/
       x2_setup_resp_evt_hdl,        /* X2_SETUP_RESP_EV */
       x2_setup_fail_resp_hdl,       /* X2_SETUP_FAIL_EV */
       x2ap_unexpected_msg,          /* X2_SETUP_TIME_WAIT_TMR_EXP_EV */
       x2setup_guard_timer_exp_hdl,  /* X2_SETUP_RESP_TMR_EXP_EV */
       x2ap_unexpected_msg          /* CSC_CELL_STATUS_RESP */
       #ifdef ENDC_ENABLED
       /* MENB CHANGES - START */
       ,endc_x2_setup_req_w_x2_setup_resp_evt_hdl,    /* ENDC_X2_SETUP_REQ_EV */
       endc_x2_setup_fail_resp_hdl,       /* ENDC_X2SETUP_FAIL_EV */
       /* MENB CHANGES - END */
       #endif
    },
    /* X2_SETUP_ACTIVE_ST */
    {
       x2_setup_active_sctp_conn_down_evt_hdl, /* X2_SETUP_SCTP_X2_CONN_DOWN_EV */
       x2ap_unexpected_msg,    /* X2_SETUP_SCTP_X2_CONN_UP_EV */
       x2ap_unexpected_msg,    /* CSC_CELL_SETUP_IND */
       x2ap_unexpected_msg,    /* CELL_SETUP_IND_TIMER_EXP */ /* SPR 13707 Fix Start */
       x2_setup_active_x2_setup_req_evt_hdl,    /* X2_SETUP_REQ_EV */	/* SPR 13707 Fix Stop */
       x2ap_unexpected_msg,    /* X2_SETUP_RESP_EV */
       /*SPR_18154_START*/
       x2_setup_active_fail_resp_hdl,    /* X2_SETUP_FAIL_EV */
       /*SPR_18154_END*/
       x2ap_unexpected_msg,    /* X2_SETUP_TIME_WAIT_TMR_EXP_EV */
       x2ap_unexpected_msg,     /* X2_SETUP_RESP_TMR_EXP_EV */
       x2ap_unexpected_msg      /* CSC_CELL_STATUS_RESP */
       #ifdef ENDC_ENABLED
       /* MENB CHANGES - START */
       ,endc_x2_setup_active_x2_setup_req_evt_hdl,    /* ENDC_X2_SETUP_REQ_EV */
       endc_x2_setup_active_fail_resp_hdl,    /* ENDC_X2_SETUP_FAIL_EV */
       /* MENB CHANGES - END */
       #endif
    },
    /* X2_SETUP_TIME_TO_WAIT_EXP_ST */
    {
       sctp_conn_down_evt_hdl, /* X2_SETUP_SCTP_X2_CONN_DOWN_EV */
       x2ap_unexpected_msg,    /* X2_SETUP_SCTP_X2_CONN_UP_EV */
       csc_ind_in_x2Setup_hdl, /* CSC_CELL_SETUP_IND */
       x2ap_unexpected_msg,    /* CELL_SETUP_IND_TIMER_EXP */
       x2_setup_req_evt_hdl,   /* X2_SETUP_REQ_EV */
       x2ap_unexpected_msg,    /* X2_SETUP_RESP_EV */
       x2ap_unexpected_msg,    /* X2_SETUP_FAIL_EV */
       time_to_wait_exp_hdl,   /* X2_SETUP_TIME_WAIT_TMR_EXP_EV */
       x2ap_unexpected_msg,     /* X2_SETUP_RESP_TMR_EXP_EV */
       x2ap_unexpected_msg      /* CSC_CELL_STATUS_RESP */
       #ifdef ENDC_ENABLED
       /* MENB CHANGES - START */
       ,endc_x2_setup_req_evt_hdl,   /* ENDC_X2_SETUP_REQ_EV */
       x2ap_unexpected_msg,    /* ENDC_X2_SETUP_FAIL_EV */
       /* MENB CHANGES - END */
       #endif
    },
};

/****************************************************************************
 * Function Name  : x2setup_sm_entry
 * Inputs         : event      -    event_id
 *                : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     - Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to invoke the x2 setup SM. 
 ****************************************************************************/
x2ap_return_et x2setup_sm_entry (U16                api,
                                 U8                *p_msg_ptr,
                                 x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                 x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et    retVal = X2AP_FAILURE;
    en_x2setup_state  state = X2_SETUP_MAX_STATE;
    en_x2setup_event  event = X2_SETUP_MAX_EVENT;
 
    X2AP_UT_TRACE_ENTER();

    state = (en_x2setup_state)p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state;
    /* klocwork Fix */
    /*coverity_fix*/
    if (api < X2_SETUP_MAX_EVENT)
    {
    /*coverity_fix*/
         event = (en_x2setup_event)api ;
         retVal = X2AP_SUCCESS;
    }
    else if ((api >= X2AP_CSC_IF_API_BASE) && (api <=X2AP_CSC_MAX_API))
    {
       retVal = x2_setup_sm_map_event(api,
                                   &event);
    }
    else
    {
       X2AP_TRACE(X2AP_ERROR, "Invalid Event or API ID received %d \n", api); 
    }

    /* klocwork Fix */
    if (X2AP_SUCCESS == retVal)
    {                              
        if ( (state < X2_SETUP_MAX_STATE) && (event < X2_SETUP_MAX_EVENT) )
        {
            X2AP_TRACE(X2AP_INFO, "### X2SETUP_FSM Processing State %s Event %s ###",
                    X2SETUP_FSM_STATES_NAMES[state], X2SETUP_FSM_EVENTS_NAMES[event]);
            
            retVal = (x2setup_sm [state][event])(p_msg_ptr,
                    p_x2ap_gl_ctx,
                    p_enb_ctx); 
           /* SPR -17891 CHANGES START */ 
            X2AP_TRACE(X2AP_INFO, "### X2SETUP_FSM Event %s Change State %s -> %s ###",
                    X2SETUP_FSM_EVENTS_NAMES[event], X2SETUP_FSM_STATES_NAMES[state],
                    X2SETUP_FSM_STATES_NAMES[(en_x2setup_state)p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state]);
	  /* SPR -17891 CHANGES END */ 
        }
        else
        {
           X2AP_TRACE(X2AP_WARNING,"[%s]::Invalid Event :"
                      "unexpected state or event!",
                      __FUNCTION__);
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/****************************************************************************
 * Function Name  : x2_setup_sm_map_event
 * Inputs         : api       -     api ID
 *                  p_event        event ID
 * Outputs        : p_event        event ID
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to map api ID with SM event IDs
 ****************************************************************************/
x2ap_return_et x2_setup_sm_map_event(U32               api,
                                     en_x2setup_event *p_event)
{
    x2ap_return_et    retVal = X2AP_SUCCESS;
    switch (api)
    {
        case X2AP_CSC_CELL_ADD_IND:
            *p_event = CSC_CELL_SETUP_IND;
            break; 
        case X2AP_CSC_CELL_STATUS_RESP:
            *p_event = CSC_CELL_STATUS_RESP;
            break; 
        default :
           retVal = X2AP_FAILURE;
           X2AP_TRACE(X2AP_INFO, "Wrong API ID received %d \n", api); 
           break;
    }
    return retVal;
}
