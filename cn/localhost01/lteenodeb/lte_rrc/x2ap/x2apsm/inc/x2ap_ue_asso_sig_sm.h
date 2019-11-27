/*******************************************************************************
*
*  FILE NAME   : x2ap_ue_asso_sig_sm.h
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function declaration.
*
*  DATE 15-Sep-2011  NAME Hari Oum Sharan REFERENCE  36.423_960(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
/*#ifndef _X2_AP_FSM_H_
#define _X2_AP_FSM_H_*/

#include "x2ap_types.h"
#include "commoninc.h"
#include "x2ap_db.h"

extern U8 *X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[];
extern U8 *X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[];

/*****************************************************************************
 * DATA
 *      DESCRIPTION: This enum is used for the enumeration of X2AP States.
 ****************************************************************************/
typedef enum 
{
    X2AP_UE_ASSO_SIG_IDLE_ST = 0,
    X2AP_UE_ASSO_SIG_ACTIVE_ST,
    X2AP_UE_ASSO_SIG_MAX_STATE
} en_x2ap_ue_asso_sig_state;

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the enumeration of X2AP Events.
 ******************************************************************************/
typedef enum 
{
    X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
    X2AP_UE_ASSO_SIG_MSG_IND_EV,
    X2AP_UE_ASSO_SIG_MSG_REQ_EV,
    X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV,
    X2AP_UE_ASSO_SIG_MAX_EVENT
} en_x2ap_ue_asso_sig_event;

x2ap_return_et x2ap_ue_asso_sig_sm_entry (U32             api,
                                 U8                       *p_msg_ptr,
                                 x2ap_gb_context_t       *p_x2ap_gl_ctx,
                                 x2ap_peer_enb_context_t *p_enb_ctx,
                                 x2ap_ProcedureCode       proc_code);


/* Macro for signature of all UE Associated Sig evenet handlers
*/
#define X2AP_UE_ASSO_SIG_SM_EVENT(ue_asso_sig_event) \
x2ap_return_et  ue_asso_sig_event( U8                  *p_msg_ptr,\
                                x2ap_gb_context_t         *p_x2ap_gl_ctx,\
                                x2ap_peer_enb_context_t   *p_enb_ctx,\
                                x2ap_ProcedureCode       proc_code)


/* Function pointer which represents the type of all UE associated Sig evenet
 * handlers in SM
*/
typedef x2ap_return_et (*x2ap_ue_asso_sig_sm_action_routine)(
                               U8                  *p_msg_ptr,
                               x2ap_gb_context_t  *p_x2ap_gl_ctx,
                               x2ap_peer_enb_context_t *p_enb_ctx,
                               x2ap_ProcedureCode       proc_code);

#define X2AP_UE_ASSO_SIG_FSM_EVENT_TRACE(state, event) \
              X2AP_TRACE(X2AP_INFO, "### Processing State:%s Event:%s ###", \
                              X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[(state)], \
                              X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[(event)])



//#endif

