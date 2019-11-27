/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_enb_mgr_fsm.h,v 1.6 2011/02/13 07:04:23 gur22047 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file comtains the function prototypes for S1AP 
                       Write Replace Warning FSM
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_enb_mgr_fsm.h,v $
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#ifndef INCLUDED_ENB_MGR_FSM_H
#define INCLUDED_ENB_MGR_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "sctp_wrappers.h"
#include "s1ap_timers.h"
#include "rrc_common_utils.h"
#include "rrc_s1apOam_intf.h"
#include "s1ap_utils.h"
#include "rrc_s1ap_uecc_intf.h"
#include "s1ap_asn_codec.h"
#include "s1ap_api.h"
#include "s1ap_mme.h"
#include "s1ap_oam.h"
#include "rrc_s1ap_asn_intrl_enc_3gpp.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/
#define ENB_MGR_GET_FSM_STATE(gb_s1ap_ctx) gb_s1ap_ctx->current_enb_mgr_state

/* SPR 15641 Fix Start */
#define ENB_MGR_SET_FSM_STATE(gb_s1ap_ctx,new_state) do \
{ \
    gb_s1ap_ctx->current_enb_mgr_state=new_state; \
    if (new_state == ENB_MGR_IDLE) \
    { \
        s1ap_kill_req_dequeue(gb_s1ap_ctx); \
    } \
}while(0)

/* SPR 15641 Fix Stop */

#define ENB_MGR_FSM_EVENT_TRACE(state, event) \
  RRC_S1AP_TRACE(RRC_BRIEF, "[STATE:%s] [EVENT:%s]", \
      ENB_MGR_FSM_STATES_NAMES[(state)], \
      ENB_MGR_FSM_EVENTS_NAMES[(event)])



/*****************************************************************************
 * ENB MGR PWS  - START
 ****************************************************************************/
typedef enum 
{
  ENB_MGR_IDLE, /* initial state of eNB Manager FSM */
  ENB_MGR_W_RRM_PWS_RESP, /* eNB manager is waiting for write-replace warning 
                           * response from RRM */
  ENB_MGR_W_RRC_PWS_RESP, /* eNB manager is waiting for PWS
                           * response from all the CSCs */
  ENB_MGR_W_RRC_PWS_FAILURE_RESP, /* eNB manager is waiting for PWS Failure
                           * response from all the CSCs */
  ENB_MGR_STATE_MAX_STATE_VAL
}enb_mgr_pws_state_et;

typedef enum {
    ENB_EV_WRITE_REPLACE_WARNING_REQ, 
  /*on receiving the write-replace warning request from MME to broadcast the PWS notification. */

  ENB_EV_PWS_TIMER_EXPIRY, 
  /*on expiry of PWS procedural timer on eNB manager*/

  ENB_EV_RRM_PWS_RESP, 
  /* on receiving the response from RRM for write-replace warning request. */

  ENB_EV_RRC_PWS_RESP, 
  /* on receiving the write-replace warning response from any of the CSC */

  ENB_EV_CELL_DREG_REQ,
  ENB_EV_KILL_REQ,
  ENB_EV_RRM_KILL_RESP,
  ENB_EV_KILL_TIMER_EXPIRY,
  ENB_EV_OAMH_CLEANUP_RESP,
  ENB_EV_RRC_KILL_RESP,
  ENB_MGR_EV_MAX_EVENT_VAL
}enb_mgr_wrw_event_et;

typedef void (*enb_mgr_pws_fsm_ret)(
    void *p_api,
    void *p_msg,
    s1ap_gb_context_t *p_s1ap_gb_ctx,
    mme_context_t *p_mme_ctx);

/*****************************************************************************
 * ENB MGR PWS - END
 ****************************************************************************/

/*****************************************************************************
  Private Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/


void enb_mgr_fsm_write_replace_warning_process_event(
    U8 event_id,
    void                *p_api,            
    void                *p_msg,            
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_invalid_event_handler(
    void                * p_api,                     
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_ignore_event_handler(
    void                * p_api,                     
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_write_replace_warning_req_handler(
    void                * p_api,             
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

rrc_return_t s1ap_rrm_build_and_send_write_replace_warning_req(
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_wrw_req
    );

rrc_return_t enb_mgr_fill_warning_area_list(
    warning_area_list_info_t *p_warning_area_list_info,
    s1ap_WarningAreaList     *p_s1ap_WarningAreaList
    );

void enb_mgr_prepare_tai_info(
    s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
    );

void enb_mgr_prepare_emergency_area_info(
    s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
    /*SPR 17777 +-*/
    );

void enb_mgr_prepare_cgi_info(
    s1ap_gb_context_t     * p_s1ap_gb_ctx, /* s1ap context */
    warning_area_list_info_t *p_warning_area_list /* warning area list*/
    );

void enb_mgr_fsm_rrm_wrw_response_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx, /* s1ap context */
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_csc_wrw_response_handler (
    void                * p_api,    /* p_api */ 
    void                *msg,
    s1ap_gb_context_t *p_s1ap_gb_ctx, /* s1ap context */
    mme_context_t* p_mme_ctx
    );

rrc_return_t enb_mgr_compose_s1ap_write_replace_warning_resp(
    s1ap_gb_context_t *p_s1ap_gb_ctx,
    U8 *asn_enc_msg_buf,
    U16 *asn_enc_buf_len,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_w_rrm_wrw_resp_wrw_request_handler(
    void                * p_api,             
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_etws_timer_expiry_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx/* mme context */
    );

void enb_mgr_fsm_rrm_wrw_failure_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx/* mme context */
    );

void enb_mgr_fsm_csc_etws_timer_expiry_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx/* mme context */
    );

void enb_mgr_fsm_csc_wrw_failure_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx/* mme context */
    );

void enb_mgr_fsm_w_rrc_wrw_resp_wrw_req_handler(
    void                * p_api,             
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
    );

void enb_mgr_fsm_cell_dreg_req_handler(
    void                * p_api,    /* p_api */ 
    void                 *p_msg,
    s1ap_gb_context_t   *p_s1ap_gb_ctx, /* s1ap context */
    mme_context_t       *p_mme_ctx
    );


void enb_mgr_prepare_kill_tai_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
);


void enb_mgr_prepare_kill_emergency_area_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
);

/* SPR 15641 Fix Start */
void s1ap_kill_req_dequeue 
(
    s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
); 
/* SPR 15641 Fix Stop */

#endif /* INCLUDED_ENB_MGR_FSM_H */ 
/* s1ap_enb_mgr_fsm.h */


