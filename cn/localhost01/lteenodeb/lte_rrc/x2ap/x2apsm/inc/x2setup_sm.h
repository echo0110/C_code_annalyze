/*******************************************************************************
*
*  FILE NAME   : x2ap_fsm.h
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function declaration.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2_AP_FSM_H_
#define _X2_AP_FSM_H_

#include "x2ap_types.h"
#include "commoninc.h"

#include "x2ap_db.h"

extern U8 *X2SETUP_FSM_STATES_NAMES[];
extern U8 *X2SETUP_FSM_EVENTS_NAMES[];

/*****************************************************************************
 * DATA
 *      DESCRIPTION: This enum is used for the enumeration of X2AP States.
 ****************************************************************************/
typedef enum 
{
    X2_SETUP_IDLE_ST = 0,
    X2_SETUP_W_X2_SETUP_RESP_ST,
    X2_SETUP_ACTIVE_ST,
    X2_SETUP_TIME_TO_WAIT_EXP_ST,
    X2_SETUP_MAX_STATE
} en_x2setup_state;

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the enumeration of X2AP Events.
 ******************************************************************************/
typedef enum 
{
    X2_SETUP_SCTP_X2_CONN_DOWN_EV = 0,
    X2_SETUP_SCTP_X2_CONN_UP_EV,
    CSC_CELL_SETUP_IND,
    CELL_SETUP_IND_TIMER_EXP,
    X2_SETUP_REQ_EV,
    X2_SETUP_RESP_EV,
    X2_SETUP_FAIL_EV,
    X2_SETUP_TIME_WAIT_TMR_EXP_EV,
    X2_SETUP_RESP_TMR_EXP_EV,
    CSC_CELL_STATUS_RESP,
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    ENDC_X2_SETUP_REQ_EV,
    ENDC_X2_SETUP_FAIL_EV,
    /* MENB CHANGES - END */
    #endif
    X2_SETUP_MAX_EVENT
} en_x2setup_event;



#define X2AP_EVENT(x2ap_event) \
x2ap_return_et  x2ap_event( U8                    *p_msg_ptr,\
                            x2ap_gb_context_t    *p_x2ap_gl_ctx,\
                            x2ap_peer_enb_context_t   *p_enb_ctx)
 

typedef x2ap_return_et (*x2setup_action_routine)(
                                          U8                  *p_msg_ptr,
                                          x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                          x2ap_peer_enb_context_t *p_enb_ctx);

#define X2SETUP_FSM_EVENT_TRACE(state, event) \
              X2AP_TRACE(X2AP_INFO, "[STATE:%s] [EVENT:%s]", \
                              X2SETUP_FSM_STATES_NAMES[(state)], \
                              X2SETUP_FSM_EVENTS_NAMES[(event)])

#define X2AP_SCTP_SM_EVENT(x2ap_event) \
x2ap_return_et  x2ap_event( U8                   *p_msg_ptr,\
                         x2ap_gb_context_t   *p_x2ap_gl_ctx,\
                         x2ap_peer_enb_context_t*p_enb_cntxt_arr) 



typedef x2ap_return_et ( *x2ap_sctp_sm_action_routine)(
                                    U8                *p_msg_ptr,
                                    x2ap_gb_context_t *p_x2ap_gl_ctx,
                                    x2ap_peer_enb_context_t *p_enb_cntxt_arr
                                    );


/* Macro for signature of all enbconf update evenet handlers
*/
#define X2AP_ENBCONF_SM_EVENT(enbconf_event) \
x2ap_return_et  enbconf_event( U8                    *p_msg_ptr,\
                            x2ap_gb_context_t    *p_x2ap_gl_ctx,\
                            x2ap_peer_enb_context_t   *p_enb_ctx)


/* Function pointer which represents the type of all enbconf update evenet
 * handlers in SM
*/
typedef x2ap_return_et (*x2ap_enbconf_sm_action_routine)(
                                          U8                  *p_msg_ptr,
                                          x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                          x2ap_peer_enb_context_t *p_enb_ctx);

#define ENBCONF_FSM_EVENT_TRACE(state, event) \
              X2AP_TRACE(X2AP_INFO, "[STATE:%s] [EVENT:%s]", \
                              ENBCONFUPD_FSM_STATES_NAMES[(state)], \
                              ENBCONFUPD_FSM_EVENTS_NAMES[(event)])



#endif

