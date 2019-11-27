/*******************************************************************************
*
*  FILE NAME   : x2ap_evthdlr.h
*
*  DESCRIPTION : This file function declaration of diffrent event handler.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _EVENT_HDL_H_
#define _EVENT_HDL_H_

#include "commoninc.h"
#include "x2setup_sm.h"

X2AP_EVENT(x2ap_unexpected_msg);
X2AP_EVENT(x2setup_request_msg_init);
X2AP_EVENT(csc_cell_setup_ind_hdl);
X2AP_EVENT(cell_setup_ind_tmr_exp_hdl);
X2AP_EVENT(sctp_conn_down_evt_hdl);
X2AP_EVENT(csc_ind_in_x2Setup_hdl);
X2AP_EVENT(time_to_wait_exp_hdl);
X2AP_EVENT(x2setup_guard_timer_exp_hdl);
X2AP_EVENT(x2_setup_req_evt_hdl);
X2AP_EVENT(x2setup_req_idle_hdl);
X2AP_EVENT(x2_setup_resp_evt_hdl);
X2AP_EVENT(x2_setup_fail_resp_hdl);
X2AP_EVENT(x2_setup_active_sctp_conn_down_evt_hdl);
X2AP_EVENT(csc_cell_status_resp_hdl);
/* SPR 13707 Fix Start */
X2AP_EVENT(x2_setup_active_x2_setup_req_evt_hdl);
/* SPR 13707 Fix Stop */
/*SPR_18154_START*/
X2AP_EVENT(x2_setup_req_w_x2_setup_resp_evt_hdl);
X2AP_EVENT(x2_setup_active_fail_resp_hdl);
/*SPR_18154_END*/


x2ap_return_et
enb_sctp_fsm_process_event
(
 U16                api,
 x2ap_gb_context_t *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t *p_enb_cntxt_arr
);

x2ap_return_et x2ap_send_link_up_ind_to_oam
(
     x2ap_peer_enb_context_t *p_enb_ctx
);

#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
X2AP_EVENT(endc_x2_setup_active_x2_setup_req_evt_hdl);
X2AP_EVENT(endc_x2_setup_active_fail_resp_hdl);
X2AP_EVENT(endc_x2_setup_req_w_x2_setup_resp_evt_hdl);
X2AP_EVENT(endc_x2_setup_req_evt_hdl);
X2AP_EVENT(endc_x2_setup_fail_resp_hdl);
X2AP_EVENT(endc_x2setup_req_idle_hdl);
/* MENB_CHANGES_END */
#endif
#endif
