/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 * $Id: s1ap_enb_mgr_fsm.c,v 1.6 2011/02/13 07:04:23 gur22047 Exp $
 ****************************************************************************
 *
 *  File Description : This file comtains the S1AP enb manager FSM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_enb_mgr_fsm.c,v $
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_enb_mgr_fsm.h"
#include "s1ap_asn_codec.h"
#include "list_wrap.h"
#include "rtxDList.h"
#include "s1ap_timers.h"
#include "uecc_rrm.h"
#include "rrc_defines.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_s1apRrm_il_composer.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_s1apOam_intf.h"
#include "rrc_rrm_intf.h"
#include "rrc_s1ap_csc_intf.h"
#include "s1ap_utils.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define CELL_ID_NUM_BITS 28

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static rrc_return_et s1ap_rrm_build_and_send_pws_cnf
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx,
 U8                    pws_resp
 );
rrc_return_t s1ap_rrm_build_and_send_pws_req
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx,
 rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_pws_req
);
static rrc_return_t enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t     *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 );

static rrc_return_t enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure
(
 rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_wrw_req,
 mme_context_t     *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 );


void enb_mgr_fsm_idle_kill_request_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx);
rrc_return_t enb_mgr_compose_s1ap_kill_resp
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len,
 mme_context_t       *p_mme_ctx
 );
rrc_return_t enb_mgr_build_and_send_s1ap_kill_resp_failure
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t     *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 );
rrc_return_et s1ap_rrm_build_and_send_kill_cnf
(
 s1ap_gb_context_t     *p_s1ap_gb_ctx,
 U8                     kill_resp
 );
rrc_return_et s1ap_rrm_build_and_send_kill_request (
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    rrc_s1ap_kill_request_t *p_rrc_s1ap_kill_req );
void enb_mgr_fsm_w_rrc_resp_timer_expiry_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx);

void enb_mgr_fsm_w_rrm_resp_kill_request_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx);
void enb_mgr_fsm_w_rrm_resp_kill_rrm_resp(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx);

void enb_mgr_fsm_w_rrm_pws_resp_pws_request_handler
(
        void                * p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx
);
void enb_mgr_fsm_pws_timer_expiry_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */;
void enb_mgr_fsm_rrm_pws_response_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */;
void enb_mgr_fsm_w_rrm_resp_timer_expiry_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */;
void enb_mgr_fsm_csc_pws_response_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */ ;

void enb_mgr_fsm_w_rrc_pws_resp_pws_req_handler(
    void                * p_api,
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx
);

void enb_mgr_fsm_csc_pws_timer_expiry_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
);
void enb_mgr_fsm_w_rrc_resp_csc_kill_resp
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */;
void enb_mgr_fsm_cleanup_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
)/* mme context */;
void enb_mgr_clean_pws_info 
(
 s1ap_gb_context_t *p_s1ap_gb_ctx /* s1ap context */
);
void enb_mgr_clean_kill_data 
(
 s1ap_gb_context_t *p_s1ap_gb_ctx /* s1ap context */
);

void prepare_and_send_wrw_and_pws_cnf
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t       *p_mme_ctx
);

void prepare_and_send_kill_resp_and_kill_cnf
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t       *p_mme_ctx
);
void enb_mgr_fsm_csc_pws_failure_timer_expiry_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
);
void enb_mgr_fsm_csc_pws_failure_response_handler
(
    void                * p_api,    /* p_api */
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx
);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

const S8* ENB_MGR_FSM_STATES_NAMES[] =
{
  (const S8*)"ENB_MGR_IDLE",
  (const S8*)"ENB_MGR_W_RRM_PWS_RESP",
  (const S8*)"ENB_MGR_W_RRC_PWS_RESP",
  (const S8*)"ENB_MGR_W_RRC_PWS_FAILURE_RESP"
};

const S8* ENB_MGR_FSM_EVENTS_NAMES[] =
{
  (const S8*)"ENB_EV_WRITE_REPLACE_WARNING_REQ",
  (const S8*)"ENB_EV_PWS_TIMER_EXPIRY",
  (const S8*)"ENB_EV_RRM_PWS_RESP",
  (const S8*)"ENB_EV_RRC_PWS_RESP",
  (const S8*)"ENB_EV_CELL_DREG_REQ",
  (const S8*)"ENB_EV_KILL_REQ",
  (const S8*)"ENB_EV_RRM_KILL_RESP",
  (const S8*)"ENB_EV_KILL_TIMER_EXPIRY",
  (const S8*)"ENB_EV_OAMH_CLEANUP_RESP",
  (const S8*)"ENB_EV_RRC_KILL_RESP"
  
};

/* Handlers for events occuring during Initial state */
static enb_mgr_pws_fsm_ret 
enb_mgr_pws_fsm[ENB_MGR_STATE_MAX_STATE_VAL][ENB_MGR_EV_MAX_EVENT_VAL] =
{
  /* State ENB_MGR_IDLE */
  {
    enb_mgr_fsm_write_replace_warning_req_handler, 
                                /* ENB_EV_WRITE_REPLACE_WARNING_REQ */
    enb_mgr_fsm_invalid_event_handler, 
                                /* ENB_EV_PWS_TIMER_EXPIRY */
    enb_mgr_fsm_invalid_event_handler, 
                                /* ENB_EV_RRM_PWS_RESP*/
    enb_mgr_fsm_invalid_event_handler,  
                                /* ENB_EV_RRC_PWS_RESP*/ 
    enb_mgr_fsm_ignore_event_handler, 
                                /* ENB_EV_CELL_DREG_REQ */ 
    enb_mgr_fsm_idle_kill_request_handler,
                               /* ENB_EV_KILL_REQ */
    enb_mgr_fsm_invalid_event_handler,
                           /* ENB_EV_RRM_KILL_RESP */ 
    enb_mgr_fsm_invalid_event_handler,
                               /* ENB_EV_KILL_TIMER_EXPIRY */
    enb_mgr_fsm_cleanup_handler,
                               /* ENB_EV_OAMH_CLEANUP_RESP */
    enb_mgr_fsm_invalid_event_handler
                              /* ENB_EV_RRC_KILL_RESP */
  },
  /* State ENB_MGR_W_RRM_PWS_RESP */
  {
    enb_mgr_fsm_w_rrm_pws_resp_pws_request_handler, 
                                /* ENB_EV_WRITE_REPLACE_WARNING_REQ */
    enb_mgr_fsm_pws_timer_expiry_handler, 
                                /* ENB_EV_PWS_TIMER_EXPIRY */
    enb_mgr_fsm_rrm_pws_response_handler,  
                                /* ENB_EV_RRM_PWS_RESP*/
    enb_mgr_fsm_invalid_event_handler,      
                                /*ENB_EV_RRC_PWS_RESP*/ 
    enb_mgr_fsm_invalid_event_handler , 
                                /* ENB_EV_CELL_DREG_REQ*/ 
    enb_mgr_fsm_w_rrm_resp_kill_request_handler,
                               /* ENB_EV_KILL_REQ */
    enb_mgr_fsm_w_rrm_resp_kill_rrm_resp ,
                             /*  ENB_EV_RRM_KILL_RESP */
    enb_mgr_fsm_w_rrm_resp_timer_expiry_handler,
                               /* ENB_EV_KILL_TIMER_EXPIRY */
    enb_mgr_fsm_cleanup_handler,
                               /* ENB_EV_OAMH_CLEANUP_RESP */
    enb_mgr_fsm_invalid_event_handler
                              /* ENB_EV_RRC_KILL_RESP */
  },
  /* State ENB_MGR_W_RRC_PWS_RESP */
  {
    enb_mgr_fsm_w_rrc_pws_resp_pws_req_handler, 
                                /* ENB_EV_WRITE_REPLACE_WARNING_REQ */
    enb_mgr_fsm_csc_pws_timer_expiry_handler,  
                                /* ENB_EV_PWS_TIMER_EXPIRY */
    enb_mgr_fsm_invalid_event_handler,  
                                /* ENB_EV_RRM_PWS_RESP*/
    enb_mgr_fsm_csc_pws_response_handler, 
                                /*ENB_EV_RRC_PWS_RESP*/
    enb_mgr_fsm_cell_dreg_req_handler,
                                /* ENB_EV_CELL_DREG_REQ*/
    enb_mgr_fsm_w_rrm_resp_kill_request_handler,
                               /* ENB_EV_KILL_REQ */
    enb_mgr_fsm_invalid_event_handler,
                              /* ENB_EV_RRM_KILL_RESP */
    enb_mgr_fsm_w_rrc_resp_timer_expiry_handler,
                               /* ENB_EV_KILL_TIMER_EXPIRY */
    enb_mgr_fsm_cleanup_handler,
                               /* ENB_EV_OAMH_CLEANUP_RESP */
    enb_mgr_fsm_w_rrc_resp_csc_kill_resp,
                                /* ENB_EV_RRC_KILL_RESP */
  },
  /* State ENB_MGR_W_RRC_PWS_FAILURE_RESP */
  {
    enb_mgr_fsm_w_rrc_pws_resp_pws_req_handler, 
                                /* ENB_EV_WRITE_REPLACE_WARNING_REQ */
    enb_mgr_fsm_csc_pws_failure_timer_expiry_handler,  
                                /* ENB_EV_PWS_TIMER_EXPIRY */
    enb_mgr_fsm_invalid_event_handler,  
                                /* ENB_EV_RRM_PWS_RESP*/
    enb_mgr_fsm_csc_pws_failure_response_handler, 
                                /*ENB_EV_RRC_PWS_RESP*/
    enb_mgr_fsm_cell_dreg_req_handler,
                                /* ENB_EV_CELL_DREG_REQ*/
    enb_mgr_fsm_w_rrm_resp_kill_request_handler,
                               /* ENB_EV_KILL_REQ */
    enb_mgr_fsm_invalid_event_handler,
                              /* ENB_EV_RRM_KILL_RESP */
    enb_mgr_fsm_invalid_event_handler,  
                               /* ENB_EV_KILL_TIMER_EXPIRY */
    enb_mgr_fsm_cleanup_handler,
                               /* ENB_EV_OAMH_CLEANUP_RESP */
    enb_mgr_fsm_invalid_event_handler,  
                                /* ENB_EV_RRC_KILL_RESP */
  }
};

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_write_replace_warning_process_event
 * Inputs         : event_id      - event id
 *                  p_api         - pointer to input API data 
 *                  p_msg         - pointer to msg 
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx     - pointer to MME context
 * Outputs        : asn_enc_msg_buf - ASN.1 Buffer to be encoded 
 *                  asn_enc_buf_len - ASN.1 Buffer length 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function compose the ASN.1 write replace warning 
 *                  message
 ******************************************************************************/
void enb_mgr_fsm_write_replace_warning_process_event(
    U8 event_id,
    void                *p_api,            
    void                *p_msg,            
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
  enb_mgr_pws_state_et state = ENB_MGR_STATE_MAX_STATE_VAL;

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  RRC_S1AP_UT_TRACE_ENTER();

  state = (enb_mgr_pws_state_et)ENB_MGR_GET_FSM_STATE (p_s1ap_gb_ctx);

  if ((state < ENB_MGR_STATE_MAX_STATE_VAL) && 
      (event_id < ENB_MGR_EV_MAX_EVENT_VAL))
  {
    ENB_MGR_FSM_EVENT_TRACE (state, event_id);

    /* Get appropriate event handler */
    (*enb_mgr_pws_fsm[state][event_id])(
                                        p_api, 
                                        p_msg, 
                                        p_s1ap_gb_ctx,
                                        p_mme_ctx);
  
    RRC_S1AP_TRACE(RRC_BRIEF, "mme_index: %u OldState: %s Event: %s NewState: %s ",
                p_mme_ctx->mme_id,
                ENB_MGR_FSM_STATES_NAMES[state],
                ENB_MGR_FSM_EVENTS_NAMES[event_id],
                ENB_MGR_FSM_STATES_NAMES[p_mme_ctx->current_enb_config_update_state]);
  }
  else
  {
    RRC_S1AP_TRACE(RRC_ERROR,"::Invalid Event : [%u]"
        "unexpected state :[%u]",
        event_id, state);
  }

  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_invalid_event_handler 
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is for handling of invalid event.
 ******************************************************************************/
void enb_mgr_fsm_invalid_event_handler(
    void                * p_api,                     
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  RRC_S1AP_TRACE(RRC_DETAILED, "Unused variable in FSM:p_api=%p,"
          " p_msg=%p and p_mme_ctx=%p", p_api, p_msg, p_mme_ctx);

  RRC_S1AP_TRACE(RRC_WARNING,"[%s]::Invalid Event, state=%s",
      __FUNCTION__,
      ENB_MGR_FSM_STATES_NAMES[(ENB_MGR_GET_FSM_STATE(p_s1ap_gb_ctx))]);

  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_ignore_event_handler 
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is to ignore events.
 ******************************************************************************/
void enb_mgr_fsm_ignore_event_handler(
    void                * p_api,                     
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  RRC_S1AP_TRACE(RRC_DETAILED, "Unused variable in FSM:p_api=%p,"
          " p_msg=%p and p_mme_ctx=%p", p_api, p_msg, p_mme_ctx);

  RRC_S1AP_TRACE(RRC_INFO,"[%s]::Ignore Event, state=%s",
      __FUNCTION__,
      ENB_MGR_FSM_STATES_NAMES[(ENB_MGR_GET_FSM_STATE(p_s1ap_gb_ctx))]);

  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_write_replace_warning_req_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM and
 *                  build and send the write replace warning message to RRM.
 ******************************************************************************/
void enb_mgr_fsm_write_replace_warning_req_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
    rrc_s1ap_write_replace_warning_request_t
        *p_rrc_s1ap_pws_req = PNULL;

    s1ap_timer_buffer_t s1ap_pws_timer_buf;
    U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_enc_buf_len = RRC_NULL;
    U16 *asn_msg = (U16 *)p_msg; 
    OSCTXT  asn1_ctx;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    p_rrc_s1ap_pws_req =
        (rrc_s1ap_write_replace_warning_request_t *)
        rrc_mem_get(sizeof(rrc_s1ap_write_replace_warning_request_t));

    if(PNULL == p_rrc_s1ap_pws_req)
    {
        RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
        return;
    }

    memset_wrapper(p_rrc_s1ap_pws_req,0,
            sizeof(rrc_s1ap_write_replace_warning_request_t));

    memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

    memset_wrapper(&s1ap_pws_timer_buf, 0,
            sizeof(s1ap_timer_buffer_t));

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        if (PNULL != p_rrc_s1ap_pws_req)
        {
            rrc_mem_free(p_rrc_s1ap_pws_req);
        }
        return;
    }

    /* Parse the message */
    if (RRC_FAILURE == parse_asn_write_replace_warning_req(
                p_api,
                &asn1_ctx,
                *asn_msg,
                p_rrc_s1ap_pws_req,
                p_mme_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "parse_asn_write_replace_warning_req returnd Failure\n"
                );
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);

        if (PNULL != p_rrc_s1ap_pws_req)
        {
            rrc_mem_free(p_rrc_s1ap_pws_req);
        }
        return;
    }

    /* Start the Guard Timer */
    s1ap_pws_timer_buf.timer_type = 
        S1AP_PWS_GUARD_TIMER;

    s1ap_pws_timer_buf.auto_delete = RRC_TRUE;
    s1ap_pws_timer_buf.mme_id = p_mme_ctx->mme_id;

    /* Start the guard timer */
    p_s1ap_gb_ctx->pws_timer_id =  rrc_start_timer(
            p_s1ap_gb_ctx->s1_pws_duration,
            &s1ap_pws_timer_buf,
            sizeof(s1ap_pws_timer_buf),
            RRC_FALSE); 

    if (PNULL == p_s1ap_gb_ctx->pws_timer_id)
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "Unable to start PWS guard timer\n");
        if (PNULL != p_rrc_s1ap_pws_req)
        {
            rrc_mem_free(p_rrc_s1ap_pws_req);
        }
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
       
        return;
    } 
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[PWS guard timer] Started\n");
    }

    /*Allocate pws_info in s1ap global ctx*/
    p_s1ap_gb_ctx->p_pws_info = (pws_info_t*)rrc_mem_get(sizeof(pws_info_t));

    if(PNULL == p_s1ap_gb_ctx->p_pws_info)
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                ": Memory allocation failed!");
        if (PNULL != p_rrc_s1ap_pws_req)
        {
            rrc_mem_free(p_rrc_s1ap_pws_req);
        }
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
        return;
    }

    /*Initializing pws_info*/
    memset_wrapper(p_s1ap_gb_ctx->p_pws_info,0,sizeof(pws_info_t));

    
    /*Allocating warning_message_contents*/
    if(RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT &
            p_rrc_s1ap_pws_req->bitmask)
    {
        p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents = 
            (U8 *)rrc_mem_get(
                WARNING_MSG_SEGMENT_OCTET_SIZE);
    }
    
    /* assigning mme_id */
    p_s1ap_gb_ctx->p_pws_info->mme_id = p_mme_ctx->mme_id; 

    if ((RRC_NULL == (p_rrc_s1ap_pws_req->bitmask & 
        RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_TYPE_PRESENT)) &&
      ((p_rrc_s1ap_pws_req->repetition_period > 0 &&
        p_rrc_s1ap_pws_req->numberof_broadcast_request == 0 &&
        (RRC_NULL == (p_rrc_s1ap_pws_req->bitmask &
         RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT ))) ||
       ((RRC_NULL == (p_rrc_s1ap_pws_req->bitmask & 
       RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT)) &&
       ((p_rrc_s1ap_pws_req->repetition_period == 0 &&
        p_rrc_s1ap_pws_req->numberof_broadcast_request == 0) ||
       (p_rrc_s1ap_pws_req->repetition_period == 0 &&
        p_rrc_s1ap_pws_req->numberof_broadcast_request > 1 ))) ||
       (RRC_NULL == (p_rrc_s1ap_pws_req->bitmask &
         RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT))))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "Neither sending the CMAS nor the ETWS warning\n");
        /* Prepare write-replace warning response as failure (not containing
         * Broadcast completed area list) to MME*/ 
        /* compose the message which is send to mme as a response */
        if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure (
              p_rrc_s1ap_pws_req,
              p_mme_ctx,
              asn_enc_msg_buf,
              &asn_enc_buf_len))
        {
           RRC_S1AP_TRACE(RRC_ERROR,
               "%s:enb_mgr_compose_s1ap_write_replace_warning_resp_failure failed.",
               __FUNCTION__);
        }
        /* Stop the pws timer.*/ 
        if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
        { 
            rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
            p_s1ap_gb_ctx->pws_timer_id = PNULL;
        }

        /* Clean pws_info fron Global Context */
        enb_mgr_clean_pws_info (p_s1ap_gb_ctx);

        /*Change the state to ENB_MGR_ENB_MGR_IDLE*/
        ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_IDLE);
        if (PNULL != p_rrc_s1ap_pws_req)
        {
            rrc_mem_free(p_rrc_s1ap_pws_req);
        }
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
        return;
    }
    /*builds and sends S1AP_RRM_PWS_REQ message to RRM*/
    if (RRC_FAILURE == s1ap_rrm_build_and_send_pws_req(
                p_s1ap_gb_ctx,
                p_rrc_s1ap_pws_req))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "builds and sends S1AP_RRM_PWS_REQ message"
                " returnd Failure\n"); 
        /* Stop the pws timer.*/  
        if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
        { 
            rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
            p_s1ap_gb_ctx->pws_timer_id = PNULL;
        }

        /* Clean pws_info fron Global Context */
        enb_mgr_clean_pws_info (p_s1ap_gb_ctx);

        /*Change the state to ENB_MGR_ENB_MGR_IDLE*/
        ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_IDLE);
        RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_IDLE]");
    }
    else
    {
        /*Change the state to ENB_MGR_W_RRM_PWS_RESP*/
        ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRM_PWS_RESP);
        RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_W_RRM_PWS_RESP]");
    }
    
    if (PNULL != p_rrc_s1ap_pws_req)
    {
        rrc_mem_free(p_rrc_s1ap_pws_req);
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}


/*****************************************************************************
 * Function Name  : s1ap_rrm_build_and_send_pws_req 
 * Inputs         : p_s1ap_gb_ctx       - poiner to S1AP global context 
 *                  p_rrc_s1ap_pws_req  -  pointer to p_rrc_s1ap_pws_req 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function builds and sends S1AP_RRM_PWS_REQ message
 *                  to RRM module. 
 ********************************************************************************/
rrc_return_t s1ap_rrm_build_and_send_pws_req
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx,
 rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_pws_req
 )
{
  rrc_return_et result = RRC_FAILURE;
  s1ap_rrm_pws_request_t *rrm_pws_req = PNULL;
  rrc_transaction_id_t trans_id = RRC_NULL;  

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx); 
  RRC_ASSERT(PNULL != p_rrc_s1ap_pws_req);

  /* Allocating memory */
  rrm_pws_req = rrc_mem_get(sizeof(s1ap_rrm_pws_request_t));
  if(PNULL == rrm_pws_req)
  {
      RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
      return RRC_FAILURE;
  }

  /* Clean up message */
  memset_wrapper(rrm_pws_req, 0,
      sizeof(s1ap_rrm_pws_request_t));
   trans_id = s1ap_enb_mgr_get_next_transaction_id(p_s1ap_gb_ctx);
  do
  {
    if(PNULL != p_rrc_s1ap_pws_req)
    {
      /* Fill Message */
      /* disable masks by default */
      rrm_pws_req->bitmask = 0;

      /* message identifier */
      l3_memcpy_wrapper(rrm_pws_req->message_identifier,
        p_rrc_s1ap_pws_req->message_identifier.data,
        sizeof(U16));

      /* serial_number */ 
      l3_memcpy_wrapper(rrm_pws_req->serial_number,
        p_rrc_s1ap_pws_req->serial_number.data,
        sizeof(U16));

      /* warning_area_list */
      if (p_rrc_s1ap_pws_req->bitmask &
        RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_AREA_LIST_PRESENT)
      {
        rrm_pws_req->bitmask |=
          S1AP_RRM_PWS_WARNING_AREA_LIST;

        enb_mgr_fill_warning_area_list(
            &(rrm_pws_req->warning_area_list),
            &p_rrc_s1ap_pws_req->warning_area_list);
      }
      /* repetition_period */ 

      l3_memcpy_wrapper(&(rrm_pws_req->repetition_period),
        &(p_rrc_s1ap_pws_req->repetition_period),
        sizeof(p_rrc_s1ap_pws_req->repetition_period));

      /* numberof_broadcast_request */
      l3_memcpy_wrapper(&(rrm_pws_req->number_of_broadcast_requested),
        &(p_rrc_s1ap_pws_req->numberof_broadcast_request),
        sizeof(p_rrc_s1ap_pws_req->numberof_broadcast_request));

      /* warning_type */
      if (p_rrc_s1ap_pws_req->bitmask & 
        RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_TYPE_PRESENT)
      {
        rrm_pws_req->bitmask |=
          S1AP_RRM_PWS_WARNING_TYPE;

        l3_memcpy_wrapper(rrm_pws_req->warning_type,
          p_rrc_s1ap_pws_req->warning_type.data,
          p_rrc_s1ap_pws_req->warning_type.numocts);
      }

      /* warning_security_info */
      if (p_rrc_s1ap_pws_req->bitmask &
        RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_SECURITY_INFO_PRESENT)
      {
        rrm_pws_req->bitmask |=
          S1AP_RRM_PWS_WARNING_SECURITY_INFO;

        l3_memcpy_wrapper(rrm_pws_req->warning_security_info,
          p_rrc_s1ap_pws_req->warning_security_info.data,
          p_rrc_s1ap_pws_req->warning_security_info.numocts);
      }

      /* data_coding_scheme */
      if (p_rrc_s1ap_pws_req->bitmask &
         RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_DATA_CODING_SCHEME_PRESENT)
      {
        rrm_pws_req->bitmask |=
          S1AP_RRM_PWS_DATA_CODING_SCHEME;

        l3_memcpy_wrapper(rrm_pws_req->data_coding_scheme,
          p_rrc_s1ap_pws_req->data_coding_scheme.data,
          p_rrc_s1ap_pws_req->data_coding_scheme.numbits);
      }

      /* Extended Repetion Period */
      if (p_rrc_s1ap_pws_req->bitmask &
            RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT)
      {
        rrm_pws_req->bitmask |=     
             S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD;
        rrm_pws_req->extended_repetition_period =   
           p_rrc_s1ap_pws_req->extended_repetition_period ;

      }

      /* Concurrent Warning Request */
      if (p_rrc_s1ap_pws_req->bitmask &
             RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT)
      {
        rrm_pws_req->bitmask |=
          S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR;
        rrm_pws_req->concurrent_warning_msg_indicator =
           (U8)(p_rrc_s1ap_pws_req->concurrent_warning_message_indicator);
      }

      /* warning_message_contents */
      if (p_rrc_s1ap_pws_req->bitmask &
        RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT)
      {
        if (!((p_rrc_s1ap_pws_req->repetition_period > 0 &&
              p_rrc_s1ap_pws_req->numberof_broadcast_request == 0 &&
             (RRC_NULL == (p_rrc_s1ap_pws_req->bitmask &
              RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT ))) ||
            ((RRC_NULL == (p_rrc_s1ap_pws_req->bitmask & 
              RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT)) &&
             ((p_rrc_s1ap_pws_req->repetition_period == 0 &&
               p_rrc_s1ap_pws_req->numberof_broadcast_request == 0) ||
              (p_rrc_s1ap_pws_req->repetition_period == 0 &&
               p_rrc_s1ap_pws_req->numberof_broadcast_request > 1 )))))
        {
          rrm_pws_req->bitmask |=
            S1AP_RRM_PWS_WARNING_MESSAGE_SIZE;

          rrm_pws_req->warning_message_size = 
            (U16)(p_rrc_s1ap_pws_req->warning_message_contents.numocts);

          l3_memcpy_wrapper(p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents,
            p_rrc_s1ap_pws_req->warning_message_contents.data,
            p_rrc_s1ap_pws_req->warning_message_contents.numocts);
        }
      }
      /* store in pws_info in s1ap global context*/
      p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request = rrm_pws_req;

      /* PUP encode and send message to RRM */
      result = rrc_s1apRrm_il_send_s1ap_rrm_pws_request(
          rrm_pws_req,
          RRC_S1AP_MODULE_ID,
          RRC_RRM_MODULE_ID,
          trans_id,
          RRC_NULL);
      if (RRC_SUCCESS == result)
      {
        RRC_S1AP_TRACE(RRC_INFO,
          "PWS "
          " Message sent to RRM");
      }
      else
      {
        RRC_S1AP_TRACE(RRC_INFO,
          "Unable to send message to RRM");
        break;
      }
      result = RRC_SUCCESS;
    }
  }while(0);   
  return result;
}

/*****************************************************************************
 * Function Name  : s1ap_rrm_build_and_send_pws_cnf 
 * Inputs         : p_s1ap_gb_ctx  - pointer to S1AP global context 
 *                  pws_resp       - pws responce
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function builds and sends S1AP_RRM_PWS_CNF message
 *                  to RRM module
 ********************************************************************************/
rrc_return_et s1ap_rrm_build_and_send_pws_cnf
(
 s1ap_gb_context_t     *p_s1ap_gb_ctx,
 U8                     pws_resp
 )
{
  rrc_return_et result = RRC_FAILURE;
  s1ap_rrm_pws_cnf_t rrm_pws_cnf;
  U8 pws_cell_counter = RRC_NULL;
  
  rrc_transaction_id_t trans_id = RRC_NULL;
  
  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx); 

  /* Clean up message */
  memset_wrapper(&rrm_pws_cnf, 0,
      sizeof(rrm_pws_cnf));

  trans_id = s1ap_enb_mgr_get_next_transaction_id(p_s1ap_gb_ctx);

  do
  {
    /* message identifier */
    l3_memcpy_wrapper(rrm_pws_cnf.message_identifier,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->message_identifier,
        sizeof(p_s1ap_gb_ctx->p_pws_info->
                s1ap_rrm_pws_request->message_identifier));

    /* serial number */
    l3_memcpy_wrapper(rrm_pws_cnf.serial_number,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number));

    if(RRC_FALSE != pws_resp)
    {

      /* Fill pws response */
      rrm_pws_cnf.pws_resp = RRC_SUCCESS;
      /* Fill Broadcast info list bitmask */
      rrm_pws_cnf.bitmask
        |=S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG;

      /* Fill Broadcast Message list */
      for (pws_cell_counter = 0; pws_cell_counter <
          p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++) 
      {
        /* fill pws_broadcast list */
        (rrm_pws_cnf.pws_broadcast_list.count)++;

        /* fill bitmask */
        rrm_pws_cnf.pws_broadcast_list.
            pws_broadcast_cell_resp[pws_cell_counter].bitmask = 0;

        /* fill cell_index */
        rrm_pws_cnf.pws_broadcast_list.
            pws_broadcast_cell_resp[pws_cell_counter].cell_index = 
          p_s1ap_gb_ctx->p_pws_info->pws_resp->
            pws_resp[pws_cell_counter].cell_index;

        /* response */ 
        rrm_pws_cnf.pws_broadcast_list.
            pws_broadcast_cell_resp[pws_cell_counter].response =
          (U8)(p_s1ap_gb_ctx->p_pws_info->pws_resp->
            pws_resp[pws_cell_counter].resp); 

        /* fail_cause */
        if (p_s1ap_gb_ctx->p_pws_info->pws_resp->
                pws_resp[pws_cell_counter].resp == RRC_FAILURE)
        {
          if (S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT &
              p_s1ap_gb_ctx->p_pws_info->pws_resp->
                pws_resp[pws_cell_counter].bitmask)
          {
            rrm_pws_cnf.pws_broadcast_list.
                pws_broadcast_cell_resp[pws_cell_counter].bitmask |= 
              S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG;
            rrm_pws_cnf.pws_broadcast_list.
                pws_broadcast_cell_resp[pws_cell_counter].fail_cause =
              p_s1ap_gb_ctx->p_pws_info->pws_resp->
                pws_resp[pws_cell_counter].fail_cause; 
          }
        }     
      } /* end of for loop*/
    }
    else
    {
      /* Fill pws response */
      rrm_pws_cnf.pws_resp = RRC_FAILURE;
    }
    /* PUP encode and send message to RRM */
    result = rrc_s1apRrm_il_send_s1ap_rrm_pws_cnf(
        &rrm_pws_cnf,
        RRC_S1AP_MODULE_ID,
        RRC_RRM_MODULE_ID,
        trans_id,
        RRC_NULL);
    if (RRC_SUCCESS == result)
    {
      RRC_S1AP_TRACE(RRC_INFO,
          "PWS CNF"
          " Message sent to RRM");
    }
    else
    {
      RRC_S1AP_TRACE(RRC_INFO,"%s: "
          "Unable to send message to RRM", __FUNCTION__);
      break;
    }
    result = RRC_SUCCESS;
  }while(0);   

  /* Clean pws_info fron Global Context */
  enb_mgr_clean_pws_info (p_s1ap_gb_ctx);

  return result;
}

/*****************************************************************************
 * Function Name  : enb_mgr_fill_warning_area_list
 * Inputs         : p_warning_area_list_info - rrm warning area list info
 *                  p_s1ap_WarningAreaList - s1ap warning area list info
 * Outputs        : None 
 * Returns        : rrc_return_t
 * Description    : This function is filling warning area list info, which has
 *                  to be sent to RRM.
 ******************************************************************************/

rrc_return_t enb_mgr_fill_warning_area_list(
    warning_area_list_info_t *p_warning_area_list_info,
    s1ap_WarningAreaList     *p_s1ap_WarningAreaList
    )
{
  OSRTDListNode*      p_node = PNULL;
  U16 count = 0;
  rrc_return_et result = RRC_FAILURE;

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_warning_area_list_info);
  RRC_ASSERT(PNULL != p_s1ap_WarningAreaList);

  do 
  {

    if (T_s1ap_WarningAreaList_cellIDList == p_s1ap_WarningAreaList->t)
    {
      if (PNULL != p_s1ap_WarningAreaList->u.cellIDList)
      {
        p_warning_area_list_info->bitmask |=
          WARNING_AREA_LIST_INFO_CELL_ID_LIST;   
        /* Bug_15874 Fix Starts */ 
        p_warning_area_list_info->cell_id_list.count = 
          (U16)p_s1ap_WarningAreaList->u.cellIDList->count;
        /* Bug_15874 Fix ends */
        p_node = p_s1ap_WarningAreaList->u.cellIDList->head;        

        for (count =0; count <
            p_s1ap_WarningAreaList->u.cellIDList->count;
            count++)
        {
          RRC_ASSERT(PNULL != p_node);
          RRC_ASSERT(PNULL != p_node->data);
          
          l3_memcpy_wrapper(p_warning_area_list_info->cell_id_list.
              eutran_cgi[count].plmn_identity.plmn_id,
              ((s1ap_EUTRAN_CGI*)p_node->data)->
              pLMNidentity.data,
              ((s1ap_EUTRAN_CGI*)p_node->data)->
              pLMNidentity.numocts);

          l3_memcpy_wrapper(p_warning_area_list_info->cell_id_list.
              eutran_cgi[count].cell_identity,
              ((s1ap_EUTRAN_CGI*)p_node->data)->
              cell_ID.data,
              sizeof(((s1ap_EUTRAN_CGI*)p_node->data)->
              cell_ID.data));

          p_node = p_node->next;
        }
      }
    }

    if (T_s1ap_WarningAreaList_trackingAreaListforWarning == 
            p_s1ap_WarningAreaList->t)
    {
      if (PNULL != p_s1ap_WarningAreaList->u.trackingAreaListforWarning)
      {
        p_warning_area_list_info->bitmask |=
          WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST;   
        /* Bug_15874 Fix Starts */ 
        p_warning_area_list_info->tracking_area_id_list.count = 
          (U16)p_s1ap_WarningAreaList->u.trackingAreaListforWarning->count;
        /* Bug_15874 Fix ends */

        p_node = 
            p_s1ap_WarningAreaList->u.trackingAreaListforWarning->head;        

        for (count =0; count <
            p_s1ap_WarningAreaList->u.trackingAreaListforWarning->count;
            count++)
        {
          RRC_ASSERT(PNULL != p_node);
          RRC_ASSERT(PNULL != p_node->data);

          l3_memcpy_wrapper(p_warning_area_list_info->tracking_area_id_list.
              tracking_area_identifier[count].plmn_identity.plmn_id,
              ((s1ap_TAI*)p_node->data)->
              pLMNidentity.data,
              ((s1ap_TAI*)p_node->data)->
              pLMNidentity.numocts);

          l3_memcpy_wrapper(p_warning_area_list_info->tracking_area_id_list.
              tracking_area_identifier[count].tracking_area_code,
              ((s1ap_TAI*)p_node->data)->
              tAC.data,
              ((s1ap_TAI*)p_node->data)->
              tAC.numocts);

          p_node = p_node->next;
        }
      }
    }

    if (T_s1ap_WarningAreaList_emergencyAreaIDList == p_s1ap_WarningAreaList->t)
    {
      if (PNULL != p_s1ap_WarningAreaList->u.emergencyAreaIDList)
      {
        p_warning_area_list_info->bitmask |=
          WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST;   
        /* Bug_15874 Fix Starts */ 
        p_warning_area_list_info->emergency_area_id_list.count = 
          (U16)p_s1ap_WarningAreaList->u.emergencyAreaIDList->count;
        /* Bug_15874 Fix ends */
        p_node = p_s1ap_WarningAreaList->u.emergencyAreaIDList->head;        

        for (count =0; count <
            p_s1ap_WarningAreaList->u.emergencyAreaIDList->count;
            count++)
        {
          RRC_ASSERT(PNULL != p_node);
          RRC_ASSERT(PNULL != p_node->data);

          l3_memcpy_wrapper(p_warning_area_list_info->emergency_area_id_list.
              emer_id[count].emergency_area_id,
              ((s1ap_EmergencyAreaID*)p_node->data)->
              data,
              ((s1ap_EmergencyAreaID*)p_node->data)->
              numocts);

          p_node = p_node->next;
        }
      }
    }
    result = RRC_SUCCESS;
  }while(0);

  RRC_S1AP_UT_TRACE_EXIT();
  return result;
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_rrm_pws_response_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - mme Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM 
 *                  handle the write replace warning resp(success) received 
 *                  from RRM.
 ******************************************************************************/
void enb_mgr_fsm_rrm_pws_response_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t* p_mme_ctx /* mme Context */
    )
{
	/* coverity_fix_54703_start */
	s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_resp =
		(s1ap_rrm_pws_response_t *)p_msg;

	void *p_intrl_msg = PNULL;
	s1ap_pws_req_t *p_s1ap_pws_req = PNULL;
	U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
	U16 asn_enc_buf_len = RRC_NULL;
	U8 count = RRC_NULL;
	U8 cell_index = RRC_NULL;
	U16 pws_cell_index = RRC_NULL;
	rrc_bool_et  is_cell_found = RRC_FALSE;

	RRC_S1AP_UT_TRACE_ENTER();

  RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);
	memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

	/* handling of response code */
	if ((RRC_SUCCESS == p_s1ap_rrm_pws_resp->response) &&
			(S1AP_RRM_PWS_CONTENTS_LIST_PRESENT & p_s1ap_rrm_pws_resp->bitmask))
	{
		p_s1ap_gb_ctx->p_pws_info->pws_resp = rrc_mem_get(
				sizeof(pws_resp_t));
		if (PNULL != p_s1ap_gb_ctx->p_pws_info->pws_resp)
		{
			memset_wrapper(p_s1ap_gb_ctx->p_pws_info->pws_resp, RRC_NULL,
					sizeof(pws_resp_t));

			for (count = 0; count <
					p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.cell_count; count++)
			{
				/****** Sending S1AP_WRITE_REPLACE_WARNING_REQ ******/
				p_intrl_msg = s1ap_alloc_intrl_msg(
						RRC_CSC_MODULE_ID,
						S1AP_PWS_REQ,
						sizeof (s1ap_pws_req_t));

				if (PNULL != p_intrl_msg)
				{
					/* Fill and send message */
					p_s1ap_pws_req =
						(s1ap_pws_req_t *)((U8 *)p_intrl_msg +
								RRC_API_HEADER_SIZE);
					/* To get the respective cell_index */
					cell_index = p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
						rrc_rrm_pws_contents[count].cell_index;

					if ( PNULL != p_s1ap_gb_ctx->
							cell_s1ap_contexts[cell_index])
					{
						if ( p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
								rrc_rrm_pws_contents[count].cell_index == p_s1ap_gb_ctx->
								cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.cell_index)
						{
							/* Store the cells info received from RRM in s1ap_context */

							/* --Increment the cell count*/
							p_s1ap_gb_ctx->p_pws_info->pws_resp->count++;
							/* --Store the cell index*/
							p_s1ap_gb_ctx->p_pws_info->pws_resp->pws_resp[count].cell_index=
								p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
								rrc_rrm_pws_contents[count].cell_index;
							is_cell_found = RRC_TRUE;
						}
					}

					/* If Cell Index not found in the global context */
					if ( RRC_FALSE == is_cell_found )
					{
						/*fill response*/
						p_s1ap_gb_ctx->p_pws_info->pws_resp->
							pws_resp[count].resp = RRC_FAILURE;

						/*fill cause*/
						p_s1ap_gb_ctx->p_pws_info->pws_resp->
							pws_resp[count].fail_cause =
							RRM_RRC_INTERNAL_FAILURE;

						/*set bitmask*/
						p_s1ap_gb_ctx->p_pws_info->pws_resp->
							pws_resp[count].bitmask = 
							S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT;

						if (p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
								cell_count == 1)
						{
							RRC_S1AP_TRACE(RRC_ERROR,
									"Single Cell and Not found in S1AP Global Context\n");
							/* Prepare write-replace warning response as failure 
							 * (not containing Broadcast completed area list) to MME*/ 
							/* compose the message which is send to mme as a response */
							if (RRC_FAILURE == 
									enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure (
										p_s1ap_gb_ctx,
										p_mme_ctx, 
										asn_enc_msg_buf,
										&asn_enc_buf_len))
							{
								RRC_S1AP_TRACE(RRC_ERROR,
										"enb_mgr_compose_s1ap_write_replace_warning_resp_failue"
										" failed.");
                                /* coverity_fix_48261_start */
                                rrc_msg_mem_free(p_intrl_msg);
                                /* coverity_fix_48261_stop */
                                return;
							}
							/*builds and sends S1AP_RRM_PWS_CNF 
							 * (FAILURE ) message to RRM*/
							if (RRC_FAILURE == 
									s1ap_rrm_build_and_send_pws_cnf(
										p_s1ap_gb_ctx, RRC_FAILURE))
							{
								RRC_S1AP_TRACE(RRC_ERROR,
										"builds and sends S1AP_RRM_PWS_CNF "
										"message returnd Failure\n");
                                /* coverity_fix_48261_start */
                                rrc_msg_mem_free(p_intrl_msg);
                                /* coverity_fix_48261_stop */
								return;
							}
							/* Set the state to ENB_MGR_IDLE */
                            ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE);
                            /* coverity_fix_48261_start */
                            rrc_msg_mem_free(p_intrl_msg);
                            /* coverity_fix_48261_stop */
                            return;
                        }
                    }
                    else
					{
						/* --Store the eCGI id */
						is_cell_found = RRC_FALSE;
						p_s1ap_gb_ctx->p_pws_info->pws_resp->
							pws_resp[count].eCGI.plmn_identity =
							p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.plmn_id;
						/* --Store the cell id ,here first need to check cell index then only
						 * store the cell id of that cell*/
						l3_memcpy_wrapper(p_s1ap_gb_ctx->p_pws_info->pws_resp->
								pws_resp[count].eCGI.cell_identity,
								p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.
								cell_area_info.cell_id, CELL_ID_OCTET_SIZE);


						/* Filling the API to be sent to CSC */

						p_s1ap_pws_req->bitmask = 0;
						/*cell_index*/
						p_s1ap_pws_req->cell_index =
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
							rrc_rrm_pws_contents[count].cell_index;

						p_s1ap_pws_req->transaction_id =
							s1ap_enb_mgr_get_next_transaction_id (p_s1ap_gb_ctx);   

						/* Message Id */
						l3_memcpy_wrapper(p_s1ap_pws_req->message_identifier,
								p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->message_identifier,
								MSG_ID_OCTET_SIZE);

						/* Serial No */
						l3_memcpy_wrapper(p_s1ap_pws_req->serial_number,
								p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number,
								SERIAL_NUMBER_OCTET_SIZE);

						/*repetition period*/
						p_s1ap_pws_req->repetition_period =
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->repetition_period;

						/*number of broadcast requested*/
						p_s1ap_pws_req->number_of_broadcasts_requested =
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
							number_of_broadcast_requested;

						/*scheduling info*/
						p_s1ap_pws_req->scheduling_info_list =
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
							rrc_rrm_pws_contents[count].scheduling_info_list;

						if(RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT &
								p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.rrc_rrm_pws_contents[count].bitmask)
						{
							p_s1ap_pws_req->bitmask |= PWS_SI_TRANSMISSION_INFO_LIST;
							p_s1ap_pws_req->si_transmission_info =
								p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
								rrc_rrm_pws_contents[count].si_transmission_info;
						}


						/*warning type*/ 
						if (S1AP_RRM_PWS_WARNING_TYPE & 
								p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
						{
							l3_memcpy_wrapper (p_s1ap_pws_req->warning_type,
									p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->warning_type,
									sizeof(p_s1ap_gb_ctx->p_pws_info->
										s1ap_rrm_pws_request->warning_type));
							p_s1ap_pws_req->bitmask |= 
								PWS_WARNING_TYPE_PRESENT;
						}

						/*warning security info*/
						if (S1AP_RRM_PWS_WARNING_SECURITY_INFO & 
								p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
						{
							l3_memcpy_wrapper (p_s1ap_pws_req->warning_security_info,
									p_s1ap_gb_ctx->p_pws_info->
									s1ap_rrm_pws_request->warning_security_info,
									sizeof(p_s1ap_gb_ctx->p_pws_info->
										s1ap_rrm_pws_request->warning_security_info));
							p_s1ap_pws_req->bitmask |=
								PWS_WARNING_SECURITY_INFO_PRESENT;
						}

						/*Data coding scheme*/
						if (S1AP_RRM_PWS_DATA_CODING_SCHEME & 
								p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
						{
							l3_memcpy_wrapper (p_s1ap_pws_req->data_coding_scheme,
									p_s1ap_gb_ctx->p_pws_info->
									s1ap_rrm_pws_request->data_coding_scheme,
									sizeof(p_s1ap_gb_ctx->p_pws_info->
										s1ap_rrm_pws_request->data_coding_scheme));
							p_s1ap_pws_req->bitmask |=
								PWS_DATA_CODING_SCHEME_PRESENT;
						}
					}

					/*warning msg size*/
					if (S1AP_RRM_PWS_WARNING_MESSAGE_SIZE & 
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
					{
						p_s1ap_pws_req->warning_msg_size =
							p_s1ap_gb_ctx->p_pws_info->
							s1ap_rrm_pws_request->warning_message_size;

						p_s1ap_pws_req->bitmask |=
							PWS_WARNING_MESSAGE_SIZE_PRESENT;

						/*warning msg contents*/
						l3_memcpy_wrapper (p_s1ap_pws_req->wrw_message_contents,
								p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents,
								p_s1ap_pws_req->warning_msg_size);
						p_s1ap_pws_req->bitmask |=
							PWS_WARNING_MESSAGE_CONTENTS_PRESENT;
					}

					if(RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT &
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.rrc_rrm_pws_contents[count].bitmask)
					{
						p_s1ap_pws_req->bitmask |= PWS_ETWS_SCHEDULING_INFO;
						p_s1ap_pws_req->etws_scheduling_info =
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.rrc_rrm_pws_contents[count].etws_scheduling_info;
					}

					if(RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT &
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.rrc_rrm_pws_contents[count].bitmask)
					{
						p_s1ap_pws_req->bitmask |= PWS_CMAS_SCHEDULING_INFO_LIST;
						p_s1ap_pws_req->cmas_scheduling_info_list =
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.rrc_rrm_pws_contents[count].cmas_scheduling_info_list ;
					}
					/* fill ecgi info for all csc */

					if (S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD &
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
					{
						p_s1ap_pws_req->bitmask |= PWS_EXTENDED_REPETITION_PERIOD_PRESENT;
						p_s1ap_pws_req->extended_repetition_period =
							p_s1ap_gb_ctx->p_pws_info->
							s1ap_rrm_pws_request->extended_repetition_period ;                
					}

					if (S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR  &
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask)
					{
						p_s1ap_pws_req->bitmask |= PWS_CONCURRENT_WARNING_MSG_INDICATOR;
						p_s1ap_pws_req->concurrent_warning_msg_indicator =
							p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->concurrent_warning_msg_indicator ;
					}

					/* Send response */
					rrc_send_message(
							p_intrl_msg,
							RRC_CSC_MODULE_ID);

					/* Storing the pws bitmask for this cell*/
					pws_cell_index = (U16)(1 <<
							p_s1ap_rrm_pws_resp->rrc_rrm_pws_contents_list.
							rrc_rrm_pws_contents[count].cell_index);
					p_s1ap_gb_ctx->p_pws_info->pws_cells |= pws_cell_index;
				}
			}/* end of if condition */
		} /* end of for loop*/


		/*Change the state to ENB_MGR_W_RRC_PWS_RESP*/
		ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRC_PWS_RESP);
		RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [ENB_MGR_W_RRC_PWS_RESP]",
				__FUNCTION__);
	}
	else if (p_s1ap_rrm_pws_resp->response == RRC_FAILURE)
	{
		/* Prepare write-replace warning response as failure (not containing
		 * Broadcast completed area list) to MME*/ 
		/* compose the message which is send to mme as a response */
		if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure (
					p_s1ap_gb_ctx,
					p_mme_ctx,
					asn_enc_msg_buf,
					&asn_enc_buf_len))
		{

			RRC_S1AP_TRACE(RRC_ERROR,
					"%s:enb_mgr_compose_s1ap_write_replace_warning_resp_failue failed.",
					__FUNCTION__);
			RRC_S1AP_UT_TRACE_EXIT();
			return;
		}
		/* Clean pws_info fron Global Context */
		enb_mgr_clean_pws_info (p_s1ap_gb_ctx);
		/* Stop the pws timer.*/  
		if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
		{ 
			rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
			p_s1ap_gb_ctx->pws_timer_id = PNULL;
		}

		/*Change the state to ENB_MGR_ENB_MGR_IDLE*/
		ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_IDLE);
		RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_IDLE]");
	}
	else
	{
		RRC_S1AP_TRACE(RRC_INFO,"Invalid response received from RRM");
		return;
	}
	RRC_S1AP_UT_TRACE_EXIT();
}
/* coverity_fix_54703_stop */

/*****************************************************************************
 * Function Name  : enb_mgr_prepare_tai_info
 *                  p_s1ap_gb_ctx - S1AP Global Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is praparing tracking area info. 
 ******************************************************************************/
void enb_mgr_prepare_tai_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
 )
{
  tai_cell_index_list_t *p_tai_info_list = PNULL;
  pws_resp_t *p_pws_resp = PNULL;
  U8 tai_count = RRC_NULL;
  U8 cell_count = RRC_NULL;
  U8 s1ap_cell_count = RRC_NULL;
  U8 cell_id_found = RRC_FALSE;
  U8 tai_index = RRC_NULL;
  U8 tai_found = RRC_FALSE;
  U8 count = RRC_NULL;
  U8 tai_cell_count = RRC_NULL;
  tracking_area_identifier_t  tai_local ;
  tracking_area_identifier_t  tai_elem  ;

  p_s1ap_gb_ctx->p_pws_info->tai_list = rrc_mem_get(
          sizeof (tai_cell_index_list_t));
  if(PNULL == p_s1ap_gb_ctx->p_pws_info->tai_list)
  {
      RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
      return;
  }

  memset_wrapper (p_s1ap_gb_ctx->p_pws_info->tai_list, RRC_NULL,
                  sizeof (tai_cell_index_list_t));

  p_tai_info_list = p_s1ap_gb_ctx->p_pws_info->tai_list;
  p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);


  for(cell_count = 0; cell_count < p_pws_resp->count;
      cell_count++)
  {
    /*Match cell index*/
    for(s1ap_cell_count = 0; s1ap_cell_count < MAX_NUM_CELLS;
        s1ap_cell_count++)
    {
      if( ( (RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp )|| 
                (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                      pws_resp[cell_count].fail_cause )) &&
           (p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]!= PNULL) && 
           ( p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
            s1ap_cell_area_info.cell_index == 
            p_pws_resp->pws_resp[cell_count].cell_index))
      {
        cell_id_found = RRC_TRUE;
        break;
      }

    }
    if (RRC_FALSE == cell_id_found)
    {
      RRC_S1AP_TRACE(RRC_ERROR,"CELL id sent by RRM "\
          "does not exist at RRC or response is failure");
      return ;
    }
    else
    {
        if (MAX_NUM_CELLS > s1ap_cell_count)
        {
            l3_memcpy_wrapper(tai_local.plmn_identity.plmn_id, 
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.plmn_id.plmn_id,
                    MAX_PLMN_ID_BYTES);

            l3_memcpy_wrapper(tai_local.tracking_area_code,
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_area_info.tac,
                    TAC_OCTET_SIZE);

            /* Traverse and check TAI in the existing TAI (TAC and PLMN id) DB*/
            tai_count = p_tai_info_list->tai_count;
            for(tai_index = 0; tai_index < tai_count; tai_index++)
            {
                tai_elem = p_tai_info_list->tai_info[tai_index].tai; 
                if(!(memcmp_wrapper(tai_elem.tracking_area_code,tai_local.tracking_area_code,TAC_OCTET_SIZE)))
                {
                    if(!(memcmp_wrapper(tai_elem.plmn_identity.plmn_id,tai_local.plmn_identity.plmn_id,MAX_PLMN_ID_BYTES)))
                    {
                        tai_found = RRC_TRUE;

                        /* Add this cell count in current tai_info */
                        p_tai_info_list->tai_info[tai_index].cell_count++;
                        count = p_tai_info_list->tai_info[tai_index].cell_count;

                        if ((count > RRC_NULL) && (count <= MAX_NUM_CELLS))
                        {
                            p_tai_info_list->tai_info[tai_index].cell_index[count - 1] = 
                                p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                                s1ap_cell_area_info.cell_index;
                        }
                        break;

                    }
                }
            }
            if(RRC_FALSE == tai_found)
            {
                tai_index = p_tai_info_list->tai_count;
                /*Add TAI (TAC and PLMN id) to DB*/
                p_tai_info_list->tai_info[tai_index].tai =
                    tai_local  ;

                p_tai_info_list->tai_count++;

                /* Add cell id in current tai_info DB*/
                tai_cell_count =
                    p_tai_info_list->tai_info[tai_index].cell_count;

                p_tai_info_list->tai_info[tai_index].cell_index[tai_cell_count]
                    = p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_index;
                p_tai_info_list->tai_info[tai_index].cell_count++;

            } 
        }
    }
  }

  RRC_S1AP_UT_TRACE_EXIT();

}



/*****************************************************************************
 * Function Name  : enb_mgr_prepare_emergency_area_info
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_warning_area_list - pointer to  warning area list
 * Outputs        : None
 * Returns        : None
 * Description    : This function is preparing emergency area info.
 ******************************************************************************/
void enb_mgr_prepare_emergency_area_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
 /*SPR 17777 +-*/
 )
{
  U8 emergency_count = RRC_NULL;
  U8 cell_count = RRC_NULL;
  U8 s1ap_cell_count = RRC_NULL;
  U8 cell_id_found = RRC_NULL;
  U8 emergency_id_cell_count = RRC_NULL;
  U8 emergency_id_found = RRC_NULL;
  U8 emergency_area_id_local[EMERGENCY_AREA_SIZE];
  pws_resp_t *p_pws_resp = PNULL;

  p_s1ap_gb_ctx->p_pws_info->emergency_list = rrc_mem_get(
          sizeof(emergency_cell_index_list_t));
  if(PNULL == p_s1ap_gb_ctx->p_pws_info->emergency_list)
  {
      RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
      return;
  }

  p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;


  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  /*SPR 17777 +-*/

  /* cleaninmg the tai_with_cell_index_t */
  memset_wrapper (p_s1ap_gb_ctx->p_pws_info->emergency_list, 0,
      sizeof(emergency_cell_index_list_t));

  for(cell_count = 0; cell_count < p_pws_resp->count;
      cell_count++)
  {
    /*Check for the cell id*/
    /*Match cell index*/
    for(s1ap_cell_count = 0; s1ap_cell_count < MAX_NUM_CELLS;
        s1ap_cell_count++)
    {
      if( ( (RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp) ||
                (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                      pws_resp[cell_count].fail_cause ))&&
         (PNULL != p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]) &&
          (p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
           s1ap_cell_area_info.cell_index == 
           p_pws_resp->pws_resp[cell_count].cell_index ))


      {
        cell_id_found = RRC_TRUE;
        break;
      }

    }
    if (RRC_FALSE == cell_id_found)
    {
      RRC_S1AP_TRACE(RRC_ERROR,"CELL id sent by RRM "\
          "does not exist at RRC or response is failure");
      return ;
    }
    else
    {
        if (MAX_NUM_CELLS > s1ap_cell_count)
        {
            l3_memcpy_wrapper(emergency_area_id_local,
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->s1ap_cell_area_info.cell_area_info.emergency_id, EMERGENCY_AREA_SIZE);
            for(emergency_count = 0; emergency_count <
                    p_s1ap_gb_ctx->p_pws_info->emergency_list->emer_area_count;
                    emergency_count++)
            {

                if (memcmp_wrapper(emergency_area_id_local,
                            p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                            s1ap_cell_area_info.cell_area_info.emergency_id,
                            EMERGENCY_AREA_SIZE))
                {
                    emergency_id_found = RRC_TRUE;
                    /* store cell index*/
                    emergency_id_cell_count = 
                        p_s1ap_gb_ctx->p_pws_info->emergency_list->
                        emer_area_info[emergency_count].cell_count;

                    p_s1ap_gb_ctx->p_pws_info->emergency_list->
                        emer_area_info[emergency_count].
                        cell_index[emergency_id_cell_count] =
                        p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                        s1ap_cell_area_info.cell_index;

                    p_s1ap_gb_ctx->p_pws_info->emergency_list->
                        emer_area_info[emergency_count].cell_count++;

                }
            }
            if (RRC_FALSE == emergency_id_found)
            {

                p_s1ap_gb_ctx->p_pws_info->emergency_list->emer_area_count++;

                /* store cell index*/
                emergency_id_cell_count = p_s1ap_gb_ctx->p_pws_info->emergency_list->
                    emer_area_info[emergency_count].cell_count;

                p_s1ap_gb_ctx->p_pws_info->emergency_list->
                    emer_area_info[emergency_count].
                    cell_index[emergency_id_cell_count] =
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_index;

                /*store this EMERGENCY ID */
                l3_memcpy_wrapper(p_s1ap_gb_ctx->p_pws_info->emergency_list->emer_area_info[emergency_count].emergency_area_id,
                        emergency_area_id_local,
                        EMERGENCY_AREA_SIZE);

                p_s1ap_gb_ctx->p_pws_info->emergency_list->
                    emer_area_info[emergency_count].cell_count ++;
            }
        }
    }
  }
  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_prepare_kill_tai_info
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is praparing tracking area info. 
 ******************************************************************************/
void enb_mgr_prepare_kill_tai_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
)
{
  tai_cell_index_list_t *p_tai_info_list = PNULL;
  kill_resp_t *p_kill_resp = PNULL;
  U8 tai_count = RRC_NULL;
  U8 cell_count = RRC_NULL;
  U8 s1ap_cell_count = RRC_NULL;
  U8 cell_id_found = RRC_FALSE;
  U8 tai_index = RRC_NULL;
  U8 tai_found = RRC_FALSE;
  U8 count = RRC_NULL;
  U8 tai_cell_count = RRC_NULL;
  tracking_area_identifier_t  tai_local ;
  tracking_area_identifier_t  tai_elem  ;

  p_s1ap_gb_ctx->p_kill_data->tai_list = rrc_mem_get (
          sizeof(tai_cell_index_list_t));
  if(PNULL == p_s1ap_gb_ctx->p_kill_data->tai_list)
  {
      RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
      return;
  }
  memset_wrapper (p_s1ap_gb_ctx->p_kill_data->tai_list, RRC_NULL,
          sizeof(tai_cell_index_list_t));
  p_tai_info_list = p_s1ap_gb_ctx->p_kill_data->tai_list;
  p_kill_resp = p_s1ap_gb_ctx->p_kill_data->kill_resp;

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  for(cell_count = 0; cell_count < p_kill_resp->count;
          cell_count++)
  {
      /*Match cell index*/
      for(s1ap_cell_count = 0; s1ap_cell_count < MAX_NUM_CELLS; s1ap_cell_count++)
      {
          if( (RRC_SUCCESS == p_kill_resp->kill_resp_data[cell_count].resp ) &&
                  ( PNULL != p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]) &&
                  ( p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_index == 
                    p_kill_resp->kill_resp_data[cell_count].cell_index))
          {
              cell_id_found = RRC_TRUE;
              break;
          }

      }
      if (RRC_FALSE == cell_id_found)
      {
          RRC_S1AP_TRACE(RRC_ERROR,"CELL id sent by RRM "\
                  "does not exist at RRC or response is failure");
          return ;
      }
      else
      {
          if (MAX_NUM_CELLS > s1ap_cell_count)
          {
              l3_memcpy_wrapper(tai_local.plmn_identity.plmn_id, 
                      p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                      s1ap_cell_area_info.plmn_id.plmn_id,
                      MAX_PLMN_ID_BYTES);

              l3_memcpy_wrapper(tai_local.tracking_area_code,
                      p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                      s1ap_cell_area_info.cell_area_info.tac,
                      TAC_OCTET_SIZE);

              /* Traverse and check TAI in the existing TAI (TAC and PLMN id) DB*/
              tai_count = p_tai_info_list->tai_count;
              for(tai_index = 0; tai_index < tai_count; tai_index++)
              {
                  tai_elem = p_tai_info_list->tai_info[tai_index].tai; 
                  if(!(memcmp_wrapper(tai_elem.tracking_area_code,tai_local.tracking_area_code,TAC_OCTET_SIZE)))
                  {
                      if(!(memcmp_wrapper(tai_elem.plmn_identity.plmn_id,tai_local.plmn_identity.plmn_id,MAX_PLMN_ID_BYTES)))
                      {
                          tai_found = RRC_TRUE;

                          /* Add this cell count in current tai_info */
                          p_tai_info_list->tai_info[tai_index].cell_count++;
                          count = p_tai_info_list->tai_info[tai_index].cell_count;

                          if ((count > RRC_NULL) && (count <= MAX_NUM_CELLS))
                          {
                              p_tai_info_list->tai_info[tai_index].cell_index[count - 1] = 
                                  p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                                  s1ap_cell_area_info.cell_index;
                          }
                          break;

                      }
                  }
              }
              if(RRC_FALSE == tai_found)
              {
                  tai_index = p_tai_info_list->tai_count;
                  /*Add TAI (TAC and PLMN id) to DB*/
                  p_tai_info_list->tai_info[tai_index].tai =
                      tai_local  ;

                  p_tai_info_list->tai_count++;

                  /* Add cell id in current tai_info DB*/
                  tai_cell_count =
                      p_tai_info_list->tai_info[tai_index].cell_count;

                  p_tai_info_list->tai_info[tai_index].cell_index[tai_cell_count]
                      = p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                      s1ap_cell_area_info.cell_index;
                  p_tai_info_list->tai_info[tai_index].cell_count++;

              } 
          }
      }
  }

  RRC_S1AP_UT_TRACE_EXIT();

}



/*****************************************************************************
 * Function Name  : enb_mgr_prepare_kill_emergency_area_info
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is preparing emergency area info.
 ******************************************************************************/
void enb_mgr_prepare_kill_emergency_area_info
(
 s1ap_gb_context_t     * p_s1ap_gb_ctx /* s1ap context */
 )
{
  U8 emergency_count = RRC_NULL;
  U8 cell_count = RRC_NULL;
  U8 s1ap_cell_count = RRC_NULL;
  U8 cell_id_found = RRC_NULL;
  U8 emergency_id_cell_count = RRC_NULL;
  U8 emergency_id_found = RRC_NULL;
  U8 emergency_area_id_local[EMERGENCY_AREA_SIZE];
  kill_resp_t *p_kill_resp = PNULL;

  p_s1ap_gb_ctx->p_kill_data->emergency_list = rrc_mem_get (
          sizeof(emergency_cell_index_list_t));
  if(PNULL == p_s1ap_gb_ctx->p_kill_data->emergency_list)
  {
      RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
      return;
  }
  p_kill_resp = p_s1ap_gb_ctx->p_kill_data->kill_resp;


  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  /* cleaninmg the tai_with_cell_index_t */
  memset_wrapper (p_s1ap_gb_ctx->p_kill_data->emergency_list, 0,
      sizeof(emergency_cell_index_list_t));

  for(cell_count = 0; cell_count < p_kill_resp->count;
      cell_count++)
  {
    /*Check for the cell id*/
    /*Match cell index*/
    for(s1ap_cell_count = 0; s1ap_cell_count < MAX_NUM_CELLS;
        s1ap_cell_count++)
    {
      if((RRC_SUCCESS == p_kill_resp->kill_resp_data[cell_count].resp)  && 
          (PNULL != p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]) &&
          (p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
           s1ap_cell_area_info.cell_index == 
           p_kill_resp->kill_resp_data[cell_count].cell_index ))


      {
        cell_id_found = RRC_TRUE;
        break;
      }

    }
    if (RRC_FALSE == cell_id_found)
    {
      RRC_S1AP_TRACE(RRC_ERROR,"CELL id sent by RRM "\
          "does not exist at RRC or response is failure");
      return ;
    }
    else
    {
        if (MAX_NUM_CELLS > s1ap_cell_count)
        {
            l3_memcpy_wrapper(emergency_area_id_local,
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_area_info.emergency_id, 
                    EMERGENCY_AREA_SIZE);
            for(emergency_count = 0; emergency_count <
                    p_s1ap_gb_ctx->p_kill_data->emergency_list->emer_area_count;
                    emergency_count++)
            {

                if (memcmp_wrapper(emergency_area_id_local,
                            p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                            s1ap_cell_area_info.cell_area_info.emergency_id,
                            EMERGENCY_AREA_SIZE))
                {
                    emergency_id_found = RRC_TRUE;
                    /* store cell index*/
                    emergency_id_cell_count = 
                        p_s1ap_gb_ctx->p_kill_data->emergency_list->
                        emer_area_info[emergency_count].cell_count;

                    p_s1ap_gb_ctx->p_kill_data->emergency_list->
                        emer_area_info[emergency_count].
                        cell_index[emergency_id_cell_count] =
                        p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                        s1ap_cell_area_info.cell_index;

                    p_s1ap_gb_ctx->p_kill_data->emergency_list->
                        emer_area_info[emergency_count].cell_count++;

                }
            }
            if (RRC_FALSE == emergency_id_found)
            {

                p_s1ap_gb_ctx->p_kill_data->emergency_list->emer_area_count++;

                /* store cell index*/
                emergency_id_cell_count = p_s1ap_gb_ctx->p_kill_data->emergency_list->
                    emer_area_info[emergency_count].cell_count;

                p_s1ap_gb_ctx->p_kill_data->emergency_list->
                    emer_area_info[emergency_count].
                    cell_index[emergency_id_cell_count] =
                    p_s1ap_gb_ctx->cell_s1ap_contexts[s1ap_cell_count]->
                    s1ap_cell_area_info.cell_index;

                /*store this EMERGENCY ID */
                l3_memcpy_wrapper(p_s1ap_gb_ctx->p_kill_data->emergency_list->emer_area_info[emergency_count].emergency_area_id,
                        emergency_area_id_local,
                        EMERGENCY_AREA_SIZE);

                p_s1ap_gb_ctx->p_kill_data->emergency_list->
                    emer_area_info[emergency_count].cell_count ++;
            }
        }
    }
  }
  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_csc_pws_response_handler
 * Inputs         : p_api - pointer to input api buffer
 *                  p_msg - ppinter to filled message
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is handling csc write replace warning success
 *                  handler 
 ******************************************************************************/
void enb_mgr_fsm_csc_pws_response_handler
(
 void                * p_api,    /* p_api */ 
 void                 *p_msg,
 s1ap_gb_context_t   *p_s1ap_gb_ctx, /* s1ap context */
 mme_context_t       *p_mme_ctx
 )
{
  s1ap_pws_resp_t *p_s1ap_pws_resp = 
    (s1ap_pws_resp_t *)(p_msg);

  rrc_cell_index_t pws_cell_index = RRC_NULL;
  U8 pws_cell_counter = RRC_NULL;
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];

  RRC_S1AP_UT_TRACE_ENTER();


  RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT (PNULL != p_mme_ctx);
  RRC_ASSERT (PNULL != p_s1ap_pws_resp);

  RRC_S1AP_TRACE(RRC_DETAILED,
          "Unused variable in FSM:p_api=%p", p_api);
  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
  pws_cell_index = p_s1ap_pws_resp->cell_index;

  do {

      /*Check for which CSC the reply has come*/
      for (pws_cell_counter = 0; pws_cell_counter <
              p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++) 
      {
          if (pws_cell_index ==
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                  pws_resp[pws_cell_counter].cell_index)
          {
              if(p_s1ap_gb_ctx->p_pws_info->pws_cells & (1 << pws_cell_index))
              {
                  /*fill response*/
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].resp = p_s1ap_pws_resp->pws_response;

                  if (RRC_FAILURE == p_s1ap_gb_ctx->p_pws_info->
                          pws_resp->pws_resp[pws_cell_counter].resp) 
                  {
                      /*fill cause*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].fail_cause =
                          (rrm_fail_cause_et)p_s1ap_pws_resp->pws_fail_cause;

                      /*set bitmask*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].bitmask |= 
                          S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT;
                  }
                  p_s1ap_gb_ctx->p_pws_info->pws_cells &= (rrc_bitmask_t)(~(1 << pws_cell_index));  

                  RRC_S1AP_TRACE(RRC_INFO,
                          "%s:s1ap_pws_resp recvd for cell index [%d].",
                          __FUNCTION__,pws_cell_index);
              }
              else
              { 
                  RRC_S1AP_TRACE(RRC_INFO,
                          "s1ap_pws_resp recvd for cell index [%d] again!."
                          ,pws_cell_index);

              }
              break;
          }
      }

      /*Check if All CSCs has sent the response or not*/
      if (p_s1ap_gb_ctx->p_pws_info->pws_cells == RRC_NULL)
      {
          prepare_and_send_wrw_and_pws_cnf (p_s1ap_gb_ctx, p_mme_ctx);
          /* stop the pws timer */
          if (p_s1ap_gb_ctx->pws_timer_id != PNULL)
          {
              rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
              p_s1ap_gb_ctx->pws_timer_id = PNULL;
          }

          /* set the state to ENB_MGR_IDLE */
          ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 
          break;
      }
  }
  while(0);
  if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
  { 
      rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
      p_s1ap_gb_ctx->pws_timer_id = PNULL;
  }

  RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : enb_mgr_fsm_csc_pws_failure_response_handler
 * Inputs         : p_api - pointer to input API buffre 
 *                  p_msg - pointer to filled msg 
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context
 * Outputs        : None
 * Returns        : void
 * Description    : This function is handling csc pws failure response
 *                  handler 
 ******************************************************************************/
void enb_mgr_fsm_csc_pws_failure_response_handler
(
 void                * p_api,    /* p_api */ 
 void                 *p_msg,
 s1ap_gb_context_t   *p_s1ap_gb_ctx, /* s1ap context */
 mme_context_t       *p_mme_ctx
 )
{
  s1ap_pws_failure_resp_t *p_s1ap_pws_failure_resp = 
    (s1ap_pws_failure_resp_t *)(p_msg);

  rrc_cell_index_t pws_cell_index = RRC_NULL;
  U8 pws_cell_counter = RRC_NULL;
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];

  RRC_S1AP_UT_TRACE_ENTER();


  RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT (PNULL != p_mme_ctx);
  RRC_ASSERT (PNULL != p_s1ap_pws_failure_resp);

  RRC_S1AP_TRACE(RRC_DETAILED,
          "Unused variable in FSM:p_api=%p ", p_api);
  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
  pws_cell_index = p_s1ap_pws_failure_resp->cell_index;

  do {

      /*Check for which CSC the reply has come*/
      for (pws_cell_counter = 0; pws_cell_counter <
              p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++) 
      {
          if (pws_cell_index ==
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                  pws_resp[pws_cell_counter].cell_index)
          {
              if(p_s1ap_gb_ctx->p_pws_info->pws_cells & (1 << pws_cell_index))
              {
                  /*fill response*/
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].resp = p_s1ap_pws_failure_resp->response;

                  if (RRC_FAILURE == p_s1ap_gb_ctx->p_pws_info->
                          pws_resp->pws_resp[pws_cell_counter].resp) 
                  {
                      /*fill cause*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].fail_cause =
                                 RRM_RRC_UNKNOWN_FAILURE;

                      /*set bitmask*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].bitmask |= 
                          S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT;
                  }
                  p_s1ap_gb_ctx->p_pws_info->pws_cells &= (rrc_bitmask_t)(~(1 << pws_cell_index));  

                  RRC_S1AP_TRACE(RRC_INFO,
                          "%s:s1ap_pws_resp recvd for cell index [%d].",
                          __FUNCTION__,pws_cell_index);
              }
              else
              { 
                  RRC_S1AP_TRACE(RRC_INFO,
                          "s1ap_pws_resp recvd for cell index [%d] again!."
                          ,pws_cell_index);

              }
              break;
          }
      }

      /*Check if All CSCs has sent the response or not*/
      if (p_s1ap_gb_ctx->p_pws_info->pws_cells == RRC_NULL)
      {
          prepare_and_send_wrw_and_pws_cnf (p_s1ap_gb_ctx, p_mme_ctx);
          break;
      }
  }
  while(0);
  /* stop the pws timer */
  if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
  { 
      rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
      p_s1ap_gb_ctx->pws_timer_id = PNULL;
  }

  /* set the state to ENB_MGR_IDLE */
  ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 

  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fill_emergency_area_broadcast
 * Inputs         : p_asn1_ctx
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_pws_resp
 *                  p_s1ap_pws_response
 *                 
 * Outputs        : p_broadcast_completed_area_list - message to be filled
 * Returns        : RRC_SUCCESS
 *                  RRC_FAILURE
 * Description    : This function fills broadcast_compl_area_list
 ******************************************************************************/
static rrc_return_t enb_mgr_fill_emergency_area_broadcast(
        OSCTXT                           *p_asn1_ctx,
        s1ap_BroadcastCompletedAreaList  *p_broadcast_completed_area_list,
        s1ap_EmergencyAreaID_Broadcast   *p_emer_area_broadcast,
        s1ap_CompletedCellinEAI          *p_completed_cell_eai,
        s1ap_gb_context_t                *p_s1ap_gb_ctx,
        pws_resp_t                       *p_pws_resp,
        U8                               *broadcast_list_present,
        U8                               rel_of_mme)
{
    U8 emer_count = RRC_NULL;
    U8 cell_count = RRC_NULL;
    U8 pws_cell_counter = RRC_NULL;
    U8 pws_cell_index = RRC_NULL;
    OSRTDListNode *p_node5 = PNULL;
    OSRTDListNode *p_node6 = PNULL;
    s1ap_CompletedCellinEAI_Item * p_s1ap_CompletedCellinEAI_Item = PNULL;
    s1ap_EmergencyAreaID_Broadcast_Item *
        p_s1ap_EmergencyAreaID_Broadcast_Item = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    if (p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
            warning_area_list.bitmask
            & WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST)
    {
        RRC_S1AP_TRACE(RRC_INFO,"Emergency Area Broadcast");

        p_broadcast_completed_area_list->t =
            T_s1ap_BroadcastCompletedAreaList_emergencyAreaID_Broadcast; 

        /* Initialise the Broadcast list*/    
        rtxDListInit(p_emer_area_broadcast);

        p_broadcast_completed_area_list->u.cellID_Broadcast =
            p_emer_area_broadcast;

        /* Traverse the Emergency list(stored in gb context) and fetch the cell
         * ids corresponding to emergency id element */
        for (emer_count = 0; emer_count <
                p_s1ap_gb_ctx->p_pws_info->emergency_list->emer_area_count;
                emer_count++ )
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    s1ap_EmergencyAreaID_Broadcast_Item,
                    &p_node5,
                    &p_s1ap_EmergencyAreaID_Broadcast_Item);

            if (PNULL == p_node5)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return RRC_FAILURE;
            }

            /* Initialise the TAI item*/
            asn1Init_s1ap_EmergencyAreaID_Broadcast_Item(
                    p_s1ap_EmergencyAreaID_Broadcast_Item);

            /*(1)Fill the emergencyAreaID*/
            p_s1ap_EmergencyAreaID_Broadcast_Item->emergencyAreaID.numocts =
                EMERGENCY_AREA_SIZE;

            l3_memcpy_wrapper(p_s1ap_EmergencyAreaID_Broadcast_Item->
                    emergencyAreaID.data,
                    p_s1ap_gb_ctx->p_pws_info->emergency_list->
                    emer_area_info[emer_count].emergency_area_id,
                    EMERGENCY_AREA_SIZE);

            /* (1)Filll the completedCellinEAI list*/
            rtxDListInit(p_completed_cell_eai);

            for(cell_count = 0; cell_count < p_s1ap_gb_ctx->p_pws_info->
                    emergency_list->emer_area_info[emer_count].cell_count; cell_count++)
            {
                for (pws_cell_counter = 0; 
                        pws_cell_counter < p_s1ap_gb_ctx->p_pws_info->pws_resp->count; 
                        pws_cell_counter++)
                {

                    p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;
                    pws_cell_index = p_pws_resp->pws_resp[pws_cell_counter].cell_index;

                    if(pws_cell_index == p_s1ap_gb_ctx->p_pws_info->emergency_list->
                            emer_area_info[emer_count].cell_index[cell_count])
                    {
                      if((RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp) ||
                          ((RRC_FAILURE == p_pws_resp->pws_resp[cell_count].resp) &&
                           (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                            pws_resp[cell_count].fail_cause ) && (rel_of_mme >= MME_REL10)))
                      {
                         *broadcast_list_present = RRC_TRUE;

                        rtxDListAllocNodeAndData(p_asn1_ctx,
                                s1ap_CompletedCellinEAI_Item,
                                &p_node6,
                                &p_s1ap_CompletedCellinEAI_Item);

                        if (PNULL == p_node6)
                        {
                            /* Not enough memory */
                            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                            return RRC_FAILURE;
                        }
                        asn1Init_s1ap_CompletedCellinEAI_Item(
                                p_s1ap_CompletedCellinEAI_Item);

                        p_s1ap_CompletedCellinEAI_Item->eCGI.pLMNidentity.numocts
                            = MAX_PLMN_ID_BYTES;

                        l3_memcpy_wrapper(p_s1ap_CompletedCellinEAI_Item->
                                eCGI.pLMNidentity.data,
                                p_pws_resp->pws_resp[pws_cell_counter].eCGI.plmn_identity.plmn_id,MAX_PLMN_ID_BYTES);

                        p_s1ap_CompletedCellinEAI_Item->eCGI.cell_ID.numbits =
                            CELL_ID_NUM_BITS;

                        /* BUG 10973 start */
                        l3_memcpy_wrapper(p_s1ap_CompletedCellinEAI_Item->eCGI.cell_ID.data,
                                p_pws_resp->pws_resp[pws_cell_counter].eCGI.cell_identity,
                                sizeof(p_s1ap_CompletedCellinEAI_Item->eCGI.cell_ID.data));
                        /* BUG 10973 stop */
                        


                        rtxDListAppendNode(p_completed_cell_eai, p_node6);
                      }
                   }
                }
            }
            p_s1ap_EmergencyAreaID_Broadcast_Item->completedCellinEAI = *p_completed_cell_eai;
            rtxDListAppendNode(p_emer_area_broadcast, p_node5);
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : enb_mgr_fill_broadcast_compl_area_list
 * Inputs         : p_asn1_ctx
 *                  p_broadcast_completed_area_list
 *                  p_tai_broadcast
 *                  p_completed_cell_tai
 *                  p_cell_id_broadcast
 *                  p_emer_area_broadcast
 *                  p_completed_cell_eai
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_pws_resp
 *                  p_s1ap_pws_response
 *                  broadcast_list_present
 *                  rel_of_mme
 *                 
 * Outputs        : p_broadcast_completed_area_list - message to be filled
 * Returns        : RRC_SUCCESS
 *                  RRC_FAILURE
 * Description    : This function fills broadcast_compl_area_list
 ******************************************************************************/
static rrc_return_t enb_mgr_fill_broadcast_compl_area_list(
        OSCTXT                           *p_asn1_ctx,
        s1ap_BroadcastCompletedAreaList  *p_broadcast_completed_area_list,
        s1ap_TAI_Broadcast               *p_tai_broadcast,
        s1ap_CompletedCellinTAI          *p_completed_cell_tai,
        s1ap_CellID_Broadcast            *p_cell_id_broadcast,
        s1ap_EmergencyAreaID_Broadcast   *p_emer_area_broadcast,
        s1ap_CompletedCellinEAI          *p_completed_cell_eai,
        s1ap_gb_context_t                *p_s1ap_gb_ctx,
        pws_resp_t                       *p_pws_resp,
        /*SPR 17777 +-*/
        U8                               *broadcast_list_present,
        U8                               rel_of_mme)
{
    U8 cell_count = RRC_NULL;
    U8 tai_count = RRC_NULL;
    U8 pws_cell_counter = RRC_NULL;
    U8 pws_cell_index = RRC_NULL;
    OSRTDListNode *p_node2 = PNULL;
    OSRTDListNode *p_node3 = PNULL;
    OSRTDListNode *p_node4 = PNULL;

    s1ap_CellID_Broadcast_Item * p_s1ap_CellID_Broadcast_Item     =PNULL; 
    s1ap_TAI_Broadcast_Item * p_s1ap_TAI_Broadcast_Item           = PNULL;
    s1ap_CompletedCellinTAI_Item * p_s1ap_CompletedCellinTAI_Item = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_INFO,"Filling broadcast completed area list");

    asn1Init_s1ap_BroadcastCompletedAreaList(p_broadcast_completed_area_list); 

    /*****Fill CellId Broadcast*****/
    if (p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
            warning_area_list.bitmask
            & WARNING_AREA_LIST_INFO_CELL_ID_LIST)
    {
        p_broadcast_completed_area_list->t =
            T_s1ap_BroadcastCompletedAreaList_cellID_Broadcast; 


        /* Start filling up the pws resp by fetching all the cell index */
        rtxDListInit(p_cell_id_broadcast);
        p_broadcast_completed_area_list->u.cellID_Broadcast = 
            p_cell_id_broadcast;

        for (cell_count = 0; cell_count < 
                p_s1ap_gb_ctx->p_pws_info->pws_resp->count;
                cell_count++ )
        {
            /* Check the response of each cell*/
            /* if success, then sends its eCGI in broadcast info list*/
            p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;

            if((RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp) ||
                    ((RRC_FAILURE == p_pws_resp->pws_resp[cell_count].resp) &&
                     (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                      pws_resp[cell_count].fail_cause )  && (rel_of_mme >= MME_REL10)))
            {
                *broadcast_list_present = RRC_TRUE;
                rtxDListAllocNodeAndData(p_asn1_ctx,
                        s1ap_CellID_Broadcast_Item,
                        &p_node2,
                        &p_s1ap_CellID_Broadcast_Item);

                if (PNULL == p_node2)
                {
                    /* Not enough memory */
                    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    return RRC_FAILURE;
                }

                asn1Init_s1ap_CellID_Broadcast_Item (p_s1ap_CellID_Broadcast_Item);

                asn1Init_s1ap_CellID_Broadcast_Item (p_s1ap_CellID_Broadcast_Item);

                p_s1ap_CellID_Broadcast_Item->eCGI.pLMNidentity.numocts
                    = MAX_PLMN_ID_BYTES;

                l3_memcpy_wrapper(p_s1ap_CellID_Broadcast_Item->eCGI.pLMNidentity.data,
                        p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id,
                        sizeof(p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id));

                p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.numbits = CELL_ID_NUM_BITS;

                /* BUG 10973 start */
                l3_memcpy_wrapper(p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.data,
                        p_pws_resp->pws_resp[cell_count].eCGI.cell_identity,
                        sizeof(p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.data));
                /* BUG 10973 stop */

                rtxDListAppendNode(p_cell_id_broadcast, p_node2);
            }
        }
    }

    /*****Fill Tracking Area Id Broadcast*****/
    if (p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
            warning_area_list.bitmask
            & WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST)
    {
        p_broadcast_completed_area_list->t =
            T_s1ap_BroadcastCompletedAreaList_tAI_Broadcast; 

        /* Initialise the Broadcast list*/    
        rtxDListInit(p_tai_broadcast);
        p_broadcast_completed_area_list->u.tAI_Broadcast = 
            p_tai_broadcast;


        /* Traverse the TAI list(stored in gb context) and fetch the cell
         * ids corresponding to TAI element */
        for (tai_count = 0; tai_count <
                p_s1ap_gb_ctx->p_pws_info->tai_list->tai_count;
                tai_count++ )
        {
            /* Check the response of each cell*/
            /* if success, then sends its eCGI in broadcast info list*/

            rtxDListAllocNodeAndData(p_asn1_ctx,
                    s1ap_TAI_Broadcast_Item,
                    &p_node3,
                    &p_s1ap_TAI_Broadcast_Item);

            if (PNULL == p_node3)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return RRC_FAILURE;
            }
            /* Initialise the TAI item*/
            asn1Init_s1ap_TAI_Broadcast_Item(p_s1ap_TAI_Broadcast_Item);
            /* (1)fill the TAI */

            /*-- Fill the TAC*/
            p_s1ap_TAI_Broadcast_Item->tAI.tAC.numocts = TAC_OCTET_SIZE;
            l3_memcpy_wrapper(p_s1ap_TAI_Broadcast_Item->tAI.tAC.data,
                    p_s1ap_gb_ctx->p_pws_info->tai_list->tai_info[tai_count].
                    tai.tracking_area_code,TAC_OCTET_SIZE);

            /* --Fill the PLMN ID*/
            p_s1ap_TAI_Broadcast_Item->tAI.pLMNidentity.numocts =
                MAX_PLMN_ID_BYTES;

            l3_memcpy_wrapper(p_s1ap_TAI_Broadcast_Item->tAI.pLMNidentity.data,
                    p_s1ap_gb_ctx->p_pws_info->tai_list->tai_info[tai_count].
                    tai.plmn_identity.plmn_id,MAX_PLMN_ID_BYTES);


            /* (2)Fill the completedCellinTAI */
            /* Fill all the eCGIs corresponding to TAI*/

            /* Initilise the completed cell tai list*/
            rtxDListInit(p_completed_cell_tai);

            for(cell_count = 0; cell_count < p_s1ap_gb_ctx->
                    p_pws_info->tai_list->tai_info[tai_count].cell_count; 
                    cell_count++ )
            {
                /* First check the cell index, if found in pws_resp list then
                 * add eCGI corresponding to that cell index*/
                for (pws_cell_counter = 0; pws_cell_counter < 
                        p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++)
                {
                    p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;

                    pws_cell_index = p_pws_resp->pws_resp[pws_cell_counter].cell_index;

                    if(pws_cell_index == p_s1ap_gb_ctx->p_pws_info->tai_list->
                            tai_info[tai_count].cell_index[cell_count])
                    {
                       if((RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp) ||
                          ((RRC_FAILURE == p_pws_resp->pws_resp[cell_count].resp) &&
                           (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                            pws_resp[cell_count].fail_cause ) && (rel_of_mme >= MME_REL10)))
                      {
                         *broadcast_list_present = RRC_TRUE;

                        rtxDListAllocNodeAndData(p_asn1_ctx,
                                s1ap_CompletedCellinTAI_Item,
                                &p_node4,
                                &p_s1ap_CompletedCellinTAI_Item);

                        if (PNULL == p_node4)
                        {
                            /* Not enough memory */
                            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                            return RRC_FAILURE;
                        }
                        asn1Init_s1ap_CompletedCellinTAI_Item(p_s1ap_CompletedCellinTAI_Item);
                        p_s1ap_CompletedCellinTAI_Item->eCGI.pLMNidentity.numocts
                            = MAX_PLMN_ID_BYTES;

                        l3_memcpy_wrapper(p_s1ap_CompletedCellinTAI_Item->eCGI.pLMNidentity.data,
                                p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id,sizeof(
                                    p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id));


                        p_s1ap_CompletedCellinTAI_Item->eCGI.cell_ID.numbits =
                            CELL_ID_NUM_BITS;

                        /* BUG 10973 start */
                        l3_memcpy_wrapper(p_s1ap_CompletedCellinTAI_Item->eCGI.cell_ID.data,
                                p_pws_resp->pws_resp[pws_cell_counter].eCGI.cell_identity,
                                sizeof(p_s1ap_CompletedCellinTAI_Item->eCGI.cell_ID.data));
                        /* BUG 10973 start */

                        rtxDListAppendNode(p_completed_cell_tai, p_node4);
                    }
                  }
                }
            }
            /* Assigning the completed cell_tai_list to TAI_broadcast_item*/
            p_s1ap_TAI_Broadcast_Item->completedCellinTAI = *p_completed_cell_tai;
            rtxDListAppendNode(p_tai_broadcast, p_node3);
        }
    }

    /*****Fi1l Emergency Area Broadcast*****/
    if (RRC_SUCCESS != enb_mgr_fill_emergency_area_broadcast(
                          p_asn1_ctx, p_broadcast_completed_area_list, 
                          p_emer_area_broadcast, p_completed_cell_eai,  
                          p_s1ap_gb_ctx, p_pws_resp, 
                          broadcast_list_present, rel_of_mme))
    {
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return SUCCESS;
}

/*****************************************************************************
 * Function Name  : enb_mgr_compose_s1ap_write_replace_warning_resp
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 *                  asn_enc_msg_buf - pointer to ASN encoded msg buf
 *                  asn_enc_buf_len - pointer to ASN encoded msg length 
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is composing the write replace warning resp 
 ******************************************************************************/
rrc_return_t enb_mgr_compose_s1ap_write_replace_warning_resp
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len,
 mme_context_t       *p_mme_ctx
 )
{
  rrc_return_et response = RRC_FAILURE;
  OSCTXT asn1_ctx;
  S1AP_PDU s1ap_pdu;
  OSRTDListNode *p_node = PNULL;
  OSRTDListNode *p_node2 = PNULL;
  pws_resp_t*  p_pws_resp = PNULL;

  U8 cell_count = RRC_NULL;
  s1ap_WriteReplaceWarningResponse *p_s1ap_pws_response = PNULL;
  s1ap_CellID_Broadcast cell_id_broadcast;
  s1ap_TAI_Broadcast tai_broadcast;
    s1ap_CompletedCellinTAI  completed_cell_tai;
  s1ap_EmergencyAreaID_Broadcast emer_area_broadcast;
  s1ap_CompletedCellinEAI       completed_cell_eai;

  s1ap_CellID_Broadcast_Item * p_s1ap_CellID_Broadcast_Item     =PNULL;
  s1ap_MessageIdentifier message_identifier; 
  s1ap_SerialNumber serial_number;
  s1ap_BroadcastCompletedAreaList broadcast_completed_area_list;
  U8                  rel_of_mme = RRC_NULL;
  U8                  broadcast_list_present = RRC_FALSE;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT(PNULL != asn_enc_msg_buf);
  RRC_ASSERT(PNULL != p_mme_ctx);

  rel_of_mme = p_mme_ctx->connection_info.rel_of_mme;
  /* Init ASN1 context */
  if ( 0 != rtInitContext(&asn1_ctx) )
  {
    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
        __FUNCTION__);
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
  }

  do
  {
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /* Fill the values in the ASN structures that shall be encoded by
     *  ASN Encoder */
    /* Set Pdu type to successfuloutcome message */
    s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

    s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx, 
        SuccessfulOutcome);
    if (PNULL == s1ap_pdu.u.successfulOutcome)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      break;
    }
    asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

    s1ap_pdu.u.successfulOutcome->procedureCode = 
        ASN1V_s1ap_id_WriteReplaceWarning;
    s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

    /* Set the initiating message type to Write Replace Warning */
    s1ap_pdu.u.successfulOutcome->value.t = T1_writeReplaceWarning; 

    p_s1ap_pws_response = 
        rtxMemAllocType(&asn1_ctx, s1ap_WriteReplaceWarningResponse);

    if (PNULL == p_s1ap_pws_response)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      response = RRC_FAILURE;
      break;
    }
    memset_wrapper (p_s1ap_pws_response, 0, 
            sizeof (s1ap_WriteReplaceWarningResponse));

    s1ap_pdu.u.successfulOutcome->value.u.writeReplaceWarning =
      p_s1ap_pws_response;

    rtxDListInit(&p_s1ap_pws_response->protocolIEs);

    /****** compose message identifier ********/
    memset_wrapper(&message_identifier,0,sizeof(s1ap_MessageIdentifier));

    message_identifier.numbits =
     (MSG_ID_OCTET_SIZE*8);

    l3_memcpy_wrapper(message_identifier.data,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->message_identifier, 
        sizeof(p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
            message_identifier));

    if (0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_1(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &message_identifier))
    {
        RRC_S1AP_TRACE(RRC_ERROR, 
                "s1ap_WriteReplaceWarningResponse_protocolIEs_1 append "
                "failed.");
        break;
    }

    /***Serial number****/
    memset_wrapper (&serial_number,0,sizeof(s1ap_SerialNumber));

    serial_number.numbits =
      SERIAL_NUMBER_OCTET_SIZE*8;

    l3_memcpy_wrapper(serial_number.data,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_pws_info->
            s1ap_rrm_pws_request->serial_number));

    if(0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_2(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &serial_number))
    {
        RRC_S1AP_TRACE(RRC_ERROR, 
                "s1ap_WriteReplaceWarningResponse_protocolIEs_2 append "
                "failed.");
        break;
    }


    /****broadcast completed area list ******/
    if (p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->bitmask
        & S1AP_RRM_PWS_WARNING_AREA_LIST)
    {
            if( RRC_SUCCESS != enb_mgr_fill_broadcast_compl_area_list(&asn1_ctx, 
                                &broadcast_completed_area_list, &tai_broadcast, 
                                &completed_cell_tai, &cell_id_broadcast, 
                                &emer_area_broadcast, &completed_cell_eai, 
                                p_s1ap_gb_ctx,p_pws_resp,/*SPR 17777 +-*/
                                &broadcast_list_present, rel_of_mme))
            {
                RRC_S1AP_TRACE(RRC_WARNING, 
                        "s1ap_WriteReplaceWarningResponse_protocolIEs_3 append "
                        "failed.");
                response = RRC_FAILURE;
                break;
            }

      rtxDListAppendNode(&p_s1ap_pws_response->protocolIEs, p_node);
      if ( broadcast_list_present == RRC_TRUE)
      {
	      if(0 !=
			      asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_3(&asn1_ctx,
				      &p_s1ap_pws_response->protocolIEs,
				      &broadcast_completed_area_list))
	      {
		      RRC_S1AP_TRACE(RRC_ERROR, 
				      "s1ap_WriteReplaceWarningResponse_protocolIEs_3 append "
				      "failed.");
        
                      response = RRC_FAILURE;
		      break;
	      }
      }
    }
    else
    {
        /* If empty Warning Area List is received, then fill Cell Id List in 
           Broadcast completed area list. */
        RRC_S1AP_TRACE(RRC_INFO,"Filling broadcast completed area list");

        asn1Init_s1ap_BroadcastCompletedAreaList(&broadcast_completed_area_list); 

        broadcast_completed_area_list.t =
            T_s1ap_BroadcastCompletedAreaList_cellID_Broadcast; 


        /* Start filling up the pws resp by fetching all the cell index */
        rtxDListInit(&cell_id_broadcast);
        broadcast_completed_area_list.u.cellID_Broadcast = 
            &cell_id_broadcast;

        for (cell_count = 0; cell_count < 
                p_s1ap_gb_ctx->p_pws_info->pws_resp->count;
                cell_count++ )
        {
            /* Check the response of each cell*/
            /* if success, then sends its eCGI in broadcast info list*/
            p_pws_resp = p_s1ap_gb_ctx->p_pws_info->pws_resp;

            if((RRC_SUCCESS == p_pws_resp->pws_resp[cell_count].resp) ||
                ((RRC_FAILURE == p_pws_resp->pws_resp[cell_count].resp) &&
                (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_pws_resp->
                      pws_resp[cell_count].fail_cause ) && (rel_of_mme >= MME_REL10)))
            {
                broadcast_list_present = RRC_TRUE;

                rtxDListAllocNodeAndData(&asn1_ctx,
                        s1ap_CellID_Broadcast_Item,
                        &p_node2,
                        &p_s1ap_CellID_Broadcast_Item);

                if (PNULL == p_node2)
                {
                    /* Not enough memory */
                    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    response = RRC_FAILURE;
                    break;
                }

                asn1Init_s1ap_CellID_Broadcast_Item (p_s1ap_CellID_Broadcast_Item);

                p_s1ap_CellID_Broadcast_Item->eCGI.pLMNidentity.numocts
                    = MAX_PLMN_ID_BYTES;

                l3_memcpy_wrapper(p_s1ap_CellID_Broadcast_Item->eCGI.pLMNidentity.data,
                        p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id,
                        sizeof(p_pws_resp->pws_resp[cell_count].eCGI.plmn_identity.plmn_id));

                p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.numbits = CELL_ID_NUM_BITS;

                /* BUG 10973 start */
                l3_memcpy_wrapper(p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.data,
                        p_pws_resp->pws_resp[cell_count].eCGI.cell_identity,
                        sizeof(p_s1ap_CellID_Broadcast_Item->eCGI.cell_ID.data));

                /* BUG 10973 stop */
                
                 rtxDListAppendNode(&cell_id_broadcast, p_node2);

            }
        }


	if ( broadcast_list_present == RRC_TRUE)
	{
		if(0 !=
				asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_3(&asn1_ctx,
					&p_s1ap_pws_response->protocolIEs,
					&broadcast_completed_area_list))
		{
			RRC_S1AP_TRACE(RRC_ERROR, 
					"s1ap_WriteReplaceWarningResponse_protocolIEs_3 append "
					"failed.");
			break;
		}
	}
    }


    
    rtxDListInit(&p_s1ap_pws_response->extElem1);
    /* ASN Encode message */
    pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
        S1AP_MAX_ASN1_BUF_LEN, TRUE);
    if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
    {
      RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of Write Replace Warning Resp "
          "failed.");
      break;
    }
    else
    {
      *asn_enc_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
      response = RRC_SUCCESS;
    }
  }while(0);

  /* Free ASN1 context */
  rtFreeContext(&asn1_ctx);

  if (PNULL != p_s1ap_gb_ctx->p_pws_info)
  {
	  p_mme_ctx = p_s1ap_gb_ctx->mme_contexts
		  [ p_s1ap_gb_ctx->p_pws_info->mme_id];
  }

   
  /* Send the message to MME */
  if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx, 
                    asn_enc_msg_buf,
                    *asn_enc_buf_len,
                    PNULL))
  {
      RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d write replace warning Message "
                         "Send Failed", p_mme_ctx->mme_id);
      RRC_S1AP_UT_TRACE_EXIT();
      return RRC_FAILURE;
  }

  RRC_S1AP_UT_TRACE_EXIT();

  return response;
} 

/*****************************************************************************
 * Function Name  : enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP Global Context
 *                : p_mme_ctx -  pointer to MME Context
 *                  asn_enc_msg_buf - pointer to ASN encoded msg buf
 *                  asn_enc_buf_len - pointer to ASN encoded msg length 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is composing the write replace warning resp 
 ******************************************************************************/
rrc_return_t enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t       *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 )
{
  rrc_return_et response = RRC_FAILURE;
  OSCTXT asn1_ctx;
  S1AP_PDU s1ap_pdu;
  s1ap_WriteReplaceWarningResponse *p_s1ap_pws_response = PNULL;
  s1ap_MessageIdentifier message_identifier; 
  s1ap_SerialNumber serial_number;
  
  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT(PNULL != asn_enc_msg_buf);

  /* Init ASN1 context */
  if ( 0 != rtInitContext(&asn1_ctx) )
  {
    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
        __FUNCTION__);
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
  }

  do
  {
    asn1Init_S1AP_PDU(&s1ap_pdu);
    /* Fill the values in the ASN structures that shall be encoded by
     *  ASN Encoder */
    /* Set Pdu type to successfuloutcome message */
    s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

    s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx, 
        SuccessfulOutcome);
    if (PNULL == s1ap_pdu.u.successfulOutcome)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      break;
    }
    asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);
    s1ap_pdu.u.successfulOutcome->procedureCode = 
                        ASN1V_s1ap_id_WriteReplaceWarning;
    s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

    /* Set the initiating message type to Write Replace Warning */
    s1ap_pdu.u.successfulOutcome->value.t = T1_writeReplaceWarning; 

    p_s1ap_pws_response = 
        rtxMemAllocType(&asn1_ctx, s1ap_WriteReplaceWarningResponse);

    if (PNULL == p_s1ap_pws_response)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      response = RRC_FAILURE;
      break;
    }
    memset_wrapper(
            p_s1ap_pws_response, 0, sizeof (s1ap_WriteReplaceWarningResponse));

    s1ap_pdu.u.successfulOutcome->value.u.writeReplaceWarning =
      p_s1ap_pws_response;

    rtxDListInit(&p_s1ap_pws_response->protocolIEs);

    /****** compose message identifier ********/
    memset_wrapper(&message_identifier,0,sizeof(s1ap_MessageIdentifier));

    message_identifier.numbits =
     (MSG_ID_OCTET_SIZE*8);

    l3_memcpy_wrapper(message_identifier.data,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->message_identifier, 
        sizeof(p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->
            message_identifier));

    if (0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_1(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &message_identifier))
    {
        RRC_S1AP_TRACE(RRC_ERROR, 
                "s1ap_WriteReplaceWarningResponse_protocolIEs_1 append "
                "failed.");
        break;
    }

    /***Serial number****/
    memset_wrapper (&serial_number,0,sizeof(s1ap_SerialNumber));

    serial_number.numbits =
      SERIAL_NUMBER_OCTET_SIZE*8;

    l3_memcpy_wrapper(serial_number.data,
        p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request->serial_number));

    if(0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_2(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &serial_number))
    {
        RRC_S1AP_TRACE(RRC_ERROR, 
                "s1ap_WriteReplaceWarningResponse_protocolIEs_2 append "
                "failed.");
        break;
    }
    rtxDListInit(&p_s1ap_pws_response->extElem1);
    /* ASN Encode message */
    pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
        S1AP_MAX_ASN1_BUF_LEN, TRUE);
    if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
    {
      RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Resp "
          "failed.");
      break;
    }
    else
    {
      *asn_enc_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
      response = RRC_SUCCESS;
    }
  }while(0);

  /* Free ASN1 context */
  rtFreeContext(&asn1_ctx);

  if (PNULL != p_s1ap_gb_ctx->p_pws_info)
  {
	  p_mme_ctx = p_s1ap_gb_ctx->mme_contexts
		  [ p_s1ap_gb_ctx->p_pws_info->mme_id];
  }

  /* Send the message to MME */
  if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx, 
                    asn_enc_msg_buf,
                    *asn_enc_buf_len,
                    PNULL))
  {
      RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d write replace warning Message "
                         "Send Failed", p_mme_ctx->mme_id);

      response = RRC_FAILURE;
  }

  RRC_S1AP_UT_TRACE_EXIT();

  return response;
} 

/*****************************************************************************
 * Function Name  : enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure 
 * Inputs         : p_rrc_s1ap_wrw_req - pointer to 
 *                                       rrc_s1ap_write_replace_warning_request_t
 *                  p_mme_ctx - pointer to MME context
 *                  asn_enc_msg_buf - pointer to ASN encoded msg buffer 
 *                  asn_enc_buf_len - pointer to ASN encoded msg length 
 * Outputs        : None
 * Returns        : RRC_FAILURE or RRC_SUCCESS
 * Description    : This function is composing the 
 *                  s1ap_write_replace_warning_resp_init_failure
 ********************************************************************************/
rrc_return_t enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure
(
 rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_wrw_req,
 mme_context_t       *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 )
{
  rrc_return_et response = RRC_FAILURE;
  OSCTXT asn1_ctx;
  S1AP_PDU s1ap_pdu;
  s1ap_WriteReplaceWarningResponse *p_s1ap_pws_response = PNULL;
  s1ap_MessageIdentifier message_identifier;
  s1ap_SerialNumber serial_number;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != asn_enc_msg_buf);

  /* Init ASN1 context */
  if ( 0 != rtInitContext(&asn1_ctx) )
  {
    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
        __FUNCTION__);
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
  }

  do
  {
    asn1Init_S1AP_PDU(&s1ap_pdu);
    s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

    s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
        SuccessfulOutcome);
    if (PNULL == s1ap_pdu.u.successfulOutcome)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      break;
    }
    asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);
    s1ap_pdu.u.successfulOutcome->procedureCode =
                        ASN1V_s1ap_id_WriteReplaceWarning;
    s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

    /* Set the initiating message type to Write Replace Warning */
    s1ap_pdu.u.successfulOutcome->value.t = T1_writeReplaceWarning;

    p_s1ap_pws_response =
        rtxMemAllocType(&asn1_ctx, s1ap_WriteReplaceWarningResponse);

    if (PNULL == p_s1ap_pws_response)
    {
      RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      response = RRC_FAILURE;
      break;
    }
    memset_wrapper(
            p_s1ap_pws_response, 0, sizeof (s1ap_WriteReplaceWarningResponse));

    s1ap_pdu.u.successfulOutcome->value.u.writeReplaceWarning =
      p_s1ap_pws_response;

    rtxDListInit(&p_s1ap_pws_response->protocolIEs);
   /****** compose message identifier ********/
    memset_wrapper(&message_identifier,0,sizeof(s1ap_MessageIdentifier));
   
    message_identifier = p_rrc_s1ap_wrw_req->message_identifier;
    
    if (0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_1(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &message_identifier))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_WriteReplaceWarningResponse_protocolIEs_1 append "
                "failed.");
        break;
    }

    /***Serial number****/

    memset_wrapper (&serial_number,0,sizeof(s1ap_SerialNumber));

    serial_number = p_rrc_s1ap_wrw_req->serial_number;

    if(0 !=
            asn1Append_s1ap_WriteReplaceWarningResponse_protocolIEs_2(&asn1_ctx,
                &p_s1ap_pws_response->protocolIEs,
                &serial_number))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_WriteReplaceWarningResponse_protocolIEs_2 append "
                "failed.");
        break;
    }
    rtxDListInit(&p_s1ap_pws_response->extElem1);
    /* ASN Encode message */
    pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
        S1AP_MAX_ASN1_BUF_LEN, TRUE);
    if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
    {
      RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Resp "
          "failed.");
      break;
    }
    else
    {
      *asn_enc_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
      response = RRC_SUCCESS;
    }
  }while(0);

  /* Free ASN1 context */
  rtFreeContext(&asn1_ctx);
  /* Send the message to MME */
  if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx,
                    asn_enc_msg_buf,
                    *asn_enc_buf_len,
                    PNULL))
  {
      RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d write replace warning Message "
                         "Send Failed", p_mme_ctx->mme_id);

      response = RRC_FAILURE;
  }

  RRC_S1AP_UT_TRACE_EXIT();

  return response;
}


 
/*****************************************************************************
 * Function Name  : enb_mgr_fsm_pws_timer_expiry_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM 
 *                  handle the pws timer expiry while waiting for 
 *                  Write Replace warning response from RRM.
 ******************************************************************************/
void enb_mgr_fsm_pws_timer_expiry_handler(
    void                * p_api,    /* p_api */ 
    void                  * p_msg,  /* p_msg */
    s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
    mme_context_t         * p_mme_ctx)/* mme context */
{
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
  U16 asn_enc_buf_len = RRC_NULL;


  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_S1AP_TRACE(RRC_DETAILED,
          "Unused variable in FSM:p_api=%p, p_msg=%p", p_api, p_msg);
  p_s1ap_gb_ctx->pws_timer_id = PNULL;

  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

  /* Prepare write-replace warning response as failure (not containing
   * Broadcast completed area list) to MME*/ 
  /* compose the message which is send to mme as a response */
  if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure (
        p_s1ap_gb_ctx,
        p_mme_ctx,
        asn_enc_msg_buf,
        &asn_enc_buf_len))
  {

    RRC_S1AP_TRACE(RRC_ERROR,
        "%s:enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure failed.",
        __FUNCTION__);
    return;
  }
  
  /*builds and sends S1AP_RRM_PWS_CNF message to RRM*/
  if (RRC_FAILURE == s1ap_rrm_build_and_send_pws_cnf(
        p_s1ap_gb_ctx, RRC_FAILURE))
  {
    RRC_S1AP_TRACE(RRC_ERROR,
        "builds and sends S1AP_RRM_WRITE_REPLACE_WARNING_CNF message"
        " returnd Failure\n");
    return;
  }
  /* Timer expired, set the state to ENB_MGR_IDLE 
   */
  ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE);  

  RRC_S1AP_UT_TRACE_EXIT();
  return;  
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_csc_pws_timer_expiry_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM 
 *                  handle the pws timer expiry.
 ******************************************************************************/
void enb_mgr_fsm_csc_pws_timer_expiry_handler(
        void                * p_api,    /* p_api */ 
        void                  * p_msg,  /* p_msg */
        s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
        mme_context_t         * p_mme_ctx)/* mme context */
{
  U8 count = RRC_NULL;

  s1ap_pws_failure_req_t *p_s1ap_pws_failure_req = PNULL;
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
  s1ap_timer_buffer_t s1ap_pws_timer_buf;

  void *p_intrl_msg = PNULL;

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_S1AP_UT_TRACE_ENTER();
  RRC_S1AP_TRACE(RRC_DETAILED,
          "Unused variable in FSM:p_api=%p, p_msg=%p", p_api, p_msg);

  p_s1ap_gb_ctx->pws_timer_id = PNULL;
 
  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
  memset_wrapper(&s1ap_pws_timer_buf, 0,
            sizeof(s1ap_timer_buffer_t));

  if ((PNULL != p_s1ap_gb_ctx->p_pws_info) && (p_s1ap_gb_ctx->p_pws_info->pws_resp))
  {
      if (p_s1ap_gb_ctx->p_pws_info->pws_resp->count > 0) 
      {
          /* Start the Guard Timer */
          s1ap_pws_timer_buf.timer_type = 
              S1AP_PWS_GUARD_TIMER;

          s1ap_pws_timer_buf.auto_delete = RRC_TRUE;
          s1ap_pws_timer_buf.mme_id = p_mme_ctx->mme_id;

          /* Start the guard timer */
          p_s1ap_gb_ctx->pws_timer_id =  rrc_start_timer(
                  p_s1ap_gb_ctx->s1_pws_duration,
                  &s1ap_pws_timer_buf,
                  sizeof(s1ap_pws_timer_buf),
                  RRC_FALSE); 

          if (PNULL == p_s1ap_gb_ctx->pws_timer_id)
          {
              RRC_S1AP_TRACE(RRC_ERROR,
                      "Unable to start PWS Failure guard timer\n");

              return;
          } 
          else
          {
              RRC_S1AP_TRACE(RRC_INFO,
                      "[PWS guard timer] Started\n");
          }

          if ( RRC_NULL != p_s1ap_gb_ctx->p_pws_info->pws_cells)
          { 
              for (count = 0; count <
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->count; count++)
              {
                  /****** Sending S1AP_PWS_FAILURE_REQ ******/
                  p_intrl_msg = s1ap_alloc_intrl_msg(
                          RRC_CSC_MODULE_ID,
                          S1AP_PWS_FAILURE_REQ,
                          sizeof (s1ap_pws_failure_req_t));

                  if (PNULL != p_intrl_msg)
                  {
                      /* Fill and send message */
                      p_s1ap_pws_failure_req =
                          (s1ap_pws_failure_req_t *)((U8 *)p_intrl_msg +
                                  RRC_API_HEADER_SIZE);
                      memset_wrapper(p_s1ap_pws_failure_req,0,
                          sizeof(s1ap_pws_failure_req_t));
                      /* Filling the API to be sent to CSC */
                      if(p_s1ap_gb_ctx->p_pws_info->pws_cells & 
                              (1 << p_s1ap_gb_ctx->p_pws_info->
                               pws_resp->pws_resp[count].cell_index))
                      {
                          p_s1ap_pws_failure_req->transaction_id =
                              s1ap_enb_mgr_get_next_transaction_id (p_s1ap_gb_ctx);   
                          p_s1ap_pws_failure_req->cell_index = 
                              p_s1ap_gb_ctx->p_pws_info->pws_resp->
                              pws_resp[count].cell_index;


                          RRC_S1AP_TRACE(RRC_INFO,
                                  "[start roolback] - "
                                  "send failure indication to CSC ");

                          /* Send failure response to CSC */
                          rrc_send_message(
                                  p_intrl_msg,
                                  RRC_CSC_MODULE_ID);

                      }
                      else
                      {
                          /* coverity_fix_48260_start */
                          rrc_msg_mem_free(p_intrl_msg);
                          p_intrl_msg = PNULL;
                          /* coverity_fix_48260_stop */
                      }
                  }
              }
              /*Change the state to ENB_MGR_W_RRC_PWS_FAILURE_RESP*/
              ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRC_PWS_FAILURE_RESP);
              RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_W_RRC_PWS_FAILURE_RESP]");

          }
      }
  }
  RRC_S1AP_UT_TRACE_EXIT();
  return;  
}


/*****************************************************************************
 * Function Name  : enb_mgr_fsm_csc_pws_failure_timer_expiry_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM 
 *                  handle the pws failure timer expiry.
 ******************************************************************************/
void enb_mgr_fsm_csc_pws_failure_timer_expiry_handler(
        void                * p_api,    /* p_api */ 
        void                  * p_msg,  /* p_msg */
        s1ap_gb_context_t     * p_s1ap_gb_ctx,/* s1ap context */
        mme_context_t         * p_mme_ctx)/* mme context */
{
  U8 count = RRC_NULL;
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
  s1ap_timer_buffer_t s1ap_pws_timer_buf;


  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_S1AP_UT_TRACE_ENTER();
  RRC_S1AP_TRACE(RRC_DETAILED,
          "Unused variable in FSM:p_api=%p and p_msg=%p", p_api, p_msg);

  p_s1ap_gb_ctx->pws_timer_id = PNULL;

  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
  memset_wrapper(&s1ap_pws_timer_buf, 0,
            sizeof(s1ap_timer_buffer_t));

  if ((PNULL != p_s1ap_gb_ctx->p_pws_info) && (p_s1ap_gb_ctx->p_pws_info->pws_resp))
  {
    if (p_s1ap_gb_ctx->p_pws_info->pws_resp->count > 0) 
    {
        if ( RRC_NULL != p_s1ap_gb_ctx->p_pws_info->pws_cells)
        { 
            for (count = 0; count <
                    p_s1ap_gb_ctx->p_pws_info->pws_resp->count; count++)
            {
                /* Filling the API to be sent to CSC */
                if(p_s1ap_gb_ctx->p_pws_info->pws_cells & 
                        (1 << p_s1ap_gb_ctx->p_pws_info->
                         pws_resp->pws_resp[count].cell_index))
                {
                    /*fill response*/
                    p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[count].resp = RRC_FAILURE;
                    /*fill cause*/
                    p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[count].fail_cause =
                                 RRM_RRC_UNKNOWN_FAILURE;
                    /*set bitmask*/
                    p_s1ap_gb_ctx->p_pws_info->pws_resp->
                       pws_resp[count].bitmask |= 
                          S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT;

                    p_s1ap_gb_ctx->p_pws_info->pws_cells &= 
                         (rrc_bitmask_t)(~(1 << p_s1ap_gb_ctx->p_pws_info->
                                pws_resp->pws_resp[count].cell_index));  
                }
            }
            /*Change the state to ENB_MGR_W_RRC_PWS_FAILURE_RESP*/
           ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRC_PWS_FAILURE_RESP);
           RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_W_RRC_PWS_FAILURE_RESP]");
          }
    }
    /*Check if All CSCs has sent the response or not*/
    if (p_s1ap_gb_ctx->p_pws_info->pws_cells == RRC_NULL)
    {
      prepare_and_send_wrw_and_pws_cnf (p_s1ap_gb_ctx, p_mme_ctx);
    }
  }

  /*coverity_109004_start*/
  /* Clean pws_info fron Global Context */
  enb_mgr_clean_pws_info (p_s1ap_gb_ctx);
 /* coverity_109004_end*/
 /* set the state to ENB_MGR_IDLE */
  ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 

  RRC_S1AP_UT_TRACE_EXIT();
  return;  
}


/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrm_pws_resp_pws_request_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM and
 *                  build and send the write replace warning message to RRM.
 ******************************************************************************/
void enb_mgr_fsm_w_rrm_pws_resp_pws_request_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
	/* coverity_fix_54705_start */
	rrc_s1ap_write_replace_warning_request_t
		rrc_s1ap_write_replace_warning_request;    
	U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
	U16 asn_enc_buf_len = RRC_NULL;
	OSCTXT  asn1_ctx;

	RRC_S1AP_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_msg);
	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);


	memset_wrapper((void*)&rrc_s1ap_write_replace_warning_request,0,
			sizeof(rrc_s1ap_write_replace_warning_request_t));


	memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

	do
	{
		/* Init ASN1 context */
		if ( 0 != rtInitContext(&asn1_ctx) )
		{
			/* Drop message */
			RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
					"context initialization failed.",__FUNCTION__);
			RRC_S1AP_UT_TRACE_EXIT();
			return;
		}

		/* Parse the message */
		if (RRC_FAILURE == parse_asn_write_replace_warning_req(
					p_api,
					&asn1_ctx,
					*((U16*)p_msg),
					&rrc_s1ap_write_replace_warning_request,
					p_mme_ctx))
		{
			RRC_S1AP_TRACE(RRC_ERROR,
					"[%s]parse_asn_write_replace_warning_req returnd Failure\n",
					__FUNCTION__);
			/* Free ASN Context */
			rtFreeContext(&asn1_ctx);
			break;
		}

		RRC_S1AP_TRACE(RRC_INFO,
				" S1AP_RRM_WRITE_REPLACE_WARNING_REQ message : "
				" Sending the Failure for this Request to MME as another "
				" Request is in  Progress \n");

		/* Prepare write-replace warning response as failure (not containing
		 * Broadcast completed area list) to MME*/ 
		/* compose the message which is send to mme as a response */
		if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure(
					&rrc_s1ap_write_replace_warning_request,
					p_mme_ctx,
					asn_enc_msg_buf,
					&asn_enc_buf_len))
		{

			RRC_S1AP_TRACE(RRC_ERROR,
					"%s:enb_mgr_compose_s1ap_write_replace_warning_resp_failue failed; Unused variable p_s1ap_gb_ctx=%p",
					__FUNCTION__,p_s1ap_gb_ctx);/*SPR 17777 +-*/
			/* Free ASN Context */
			rtFreeContext(&asn1_ctx);
			return;
		}
	}while (0);

	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	RRC_S1AP_UT_TRACE_EXIT();
}
/* coverity_fix_54705_stop */

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrc_pws_resp_pws_req_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of write replace warning FSM and
 *                  build and send the write replace warning message to RRM.
 ******************************************************************************/
void enb_mgr_fsm_w_rrc_pws_resp_pws_req_handler(
    void                * p_api,             
    void                  * p_msg,
    s1ap_gb_context_t     * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
	/* coverity_fix_54704_start */
	rrc_s1ap_write_replace_warning_request_t
		rrc_s1ap_write_replace_warning_request;    
	U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
	U16 asn_enc_buf_len = RRC_NULL;
	OSCTXT  asn1_ctx;

	RRC_S1AP_UT_TRACE_ENTER();


	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_msg);
	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);


	memset_wrapper((void*)&rrc_s1ap_write_replace_warning_request,0,
			sizeof(rrc_s1ap_write_replace_warning_request_t));

	memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
				"context initialization failed.",__FUNCTION__);
		RRC_S1AP_UT_TRACE_EXIT();
		return;
	}

	/* Parse the message */
	if (RRC_FAILURE == parse_asn_write_replace_warning_req(
				p_api,
				&asn1_ctx,
				*((U16*)p_msg),
				&rrc_s1ap_write_replace_warning_request,
				p_mme_ctx))
	{
		RRC_S1AP_TRACE(RRC_ERROR,
				"[%s]parse_asn_write_replace_warning_req returnd Failure; Unused variable p_s1ap_gb_ctx=%p\n",
				__FUNCTION__,p_s1ap_gb_ctx);/*SPR 17777 +-*/
		/* Free ASN Context */
		rtFreeContext(&asn1_ctx);
		return;
	}


	/* Prepare write-replace warning response as failure (not containing
	 * Broadcast completed area list) to MME*/ 
	/* compose the message which is send to mme as a response */
	if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_write_replace_warning_resp_init_failure (
				&rrc_s1ap_write_replace_warning_request,
				p_mme_ctx,
				asn_enc_msg_buf,
				&asn_enc_buf_len))
	{

		RRC_S1AP_TRACE(RRC_ERROR,
				"%s:enb_mgr_compose_s1ap_write_replace_warning_resp_failue failed.",
				__FUNCTION__);
	}
	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	RRC_S1AP_UT_TRACE_EXIT();
}
/* coverity_fix_54704_stop */

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_cell_dreg_req_handler
 * Inputs         : p_api - pointer to filled buffer 
 *                  p_msg - pointer to filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is handling csc write replace warning success
 *                  handler 
 ******************************************************************************/
void enb_mgr_fsm_cell_dreg_req_handler
(
 void                * p_api,    /* p_api */ 
 void                 *p_msg,
 s1ap_gb_context_t   *p_s1ap_gb_ctx, /* s1ap context */
 mme_context_t       *p_mme_ctx
 )
{
  s1ap_cell_dreg_req_t *p_s1ap_cell_dereg_req =
      (s1ap_cell_dreg_req_t *)(p_msg);

  rrc_cell_index_t pws_cell_index = RRC_NULL;
  U8 pws_cell_counter = RRC_NULL;

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT (PNULL != p_api);
  RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT (PNULL != p_mme_ctx);
  RRC_ASSERT (PNULL != p_s1ap_cell_dereg_req);

  RRC_S1AP_TRACE(RRC_DETAILEDALL,
          "Unused variable in FSM: p_msg=%p p_mme_ctx=%p p_api=%p", p_msg, p_mme_ctx,p_api);/*SPR 17777+-*/
  pws_cell_index = p_s1ap_cell_dereg_req->cell_index;

  if (PNULL != p_s1ap_gb_ctx->p_pws_info)
  {
    if (PNULL != p_s1ap_gb_ctx->p_pws_info->pws_resp)
    {
      /*Check for which CSC the reply has come*/
      for (pws_cell_counter = 0; pws_cell_counter <
              p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++) 
      {
          if (pws_cell_index ==
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                  pws_resp[pws_cell_counter].cell_index)
          {
              if(p_s1ap_gb_ctx->p_pws_info->pws_cells & (1 << pws_cell_index))
              {
                  /*fill response*/
                  p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].resp = RRC_FAILURE;

                  if (RRC_FAILURE == p_s1ap_gb_ctx->p_pws_info->
                          pws_resp->pws_resp[pws_cell_counter].resp) 
                  {
                      /*fill cause*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].fail_cause =
                          RRM_RRC_LL_CONFIG_FAILURE;

                      /*set bitmask*/
                      p_s1ap_gb_ctx->p_pws_info->pws_resp->
                          pws_resp[pws_cell_counter].bitmask |= 
                          S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT;
                  }
                  p_s1ap_gb_ctx->p_pws_info->pws_cells &= (rrc_bitmask_t)(~(1 << pws_cell_index));  
               }
          }
      }
    }
    /*Check if All CSCs has sent the response or not*/
    if (p_s1ap_gb_ctx->p_pws_info->pws_cells == RRC_NULL)
    {
        prepare_and_send_wrw_and_pws_cnf (p_s1ap_gb_ctx, p_mme_ctx);
        /* stop the pws timer */
        if (PNULL != p_s1ap_gb_ctx->pws_timer_id)
        { 
            rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
            p_s1ap_gb_ctx->pws_timer_id = PNULL;
        }
        /* set the state to ENB_MGR_IDLE */
        ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 
    }
  }
  else if (PNULL != p_s1ap_gb_ctx->p_kill_data)
  {
    if (PNULL != p_s1ap_gb_ctx->p_kill_data->kill_resp)
    {
      /*Check for which CSC the reply has come*/
      for (pws_cell_counter = 0; pws_cell_counter <
              p_s1ap_gb_ctx->p_kill_data->kill_resp->count; pws_cell_counter++) 
      {
          if (pws_cell_index ==
                  p_s1ap_gb_ctx->p_kill_data->kill_resp->
                   kill_resp_data[pws_cell_counter].cell_index)
          {
              if(p_s1ap_gb_ctx->p_kill_data->pws_cells & (1 << pws_cell_index))
              {
                  /*fill response*/
                  p_s1ap_gb_ctx->p_kill_data->kill_resp->
                      kill_resp_data[pws_cell_counter].resp = RRC_FAILURE;

                  if (RRC_FAILURE == p_s1ap_gb_ctx->p_kill_data->
                          kill_resp->kill_resp_data[pws_cell_counter].resp) 
                  {
                      /*fill cause*/
                      p_s1ap_gb_ctx->p_kill_data->kill_resp->
                          kill_resp_data[pws_cell_counter].fail_cause =
                          RRM_RRC_LL_CONFIG_FAILURE;

                      /*set bitmask*/
                      p_s1ap_gb_ctx->p_kill_data->kill_resp->
                          kill_resp_data[pws_cell_counter].bitmask |= 
                          S1AP_CSC_KILL_RESP_FAIL_CAUSE_PRESENT;
                  }
                  p_s1ap_gb_ctx->p_kill_data->pws_cells &= (rrc_bitmask_t)(~(1 << pws_cell_index));  
            }
        }
      }
    }
    /*Check if All CSCs has sent the response or not*/
    if (p_s1ap_gb_ctx->p_kill_data->pws_cells == RRC_NULL)
    {
        prepare_and_send_kill_resp_and_kill_cnf (p_s1ap_gb_ctx, p_mme_ctx);
        /* stop the kill timer */
        if ( PNULL != p_s1ap_gb_ctx->kill_timer_id )
        {
            rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
            p_s1ap_gb_ctx->kill_timer_id = PNULL;
        }
        /*coverity_109003_start*/
        /* Clean pws_info fron Global Context */
        enb_mgr_clean_pws_info (p_s1ap_gb_ctx);
        /* coverity_109003_end*/
        /* set the state to ENB_MGR_IDLE */
        ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 
    }
  }
  return;
  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_idle_kill_request_handler 
 * Inputs         : p_api - pointer to filled buffer 
 *                  p_msg - pointer to filled buffer
 *                  p_s1ap_gb_ctx - S1AP global contxet 
 *                  p_mme_ctx - MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the fsm_idle_kill_request from eNB.
 ******************************************************************************/
void enb_mgr_fsm_idle_kill_request_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
	/* coverity_fix_54702_start */
	rrc_s1ap_kill_request_t *p_rrc_s1ap_kill_req = PNULL;
	s1ap_timer_buffer_t s1ap_kill_timer_buf;
	U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
	U16 *asn_msg = (U16 *)p_msg;
	OSCTXT  asn1_ctx;

	RRC_S1AP_UT_TRACE_ENTER();
	p_rrc_s1ap_kill_req =
		(rrc_s1ap_kill_request_t *)rrc_mem_get(sizeof(rrc_s1ap_kill_request_t));

	if(PNULL == p_rrc_s1ap_kill_req)
	{
		RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
		return;
	}

	memset_wrapper(p_rrc_s1ap_kill_req,0,
			sizeof(rrc_s1ap_kill_request_t));

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_msg);
	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);

	memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

	memset_wrapper(&s1ap_kill_timer_buf, 0,
			sizeof(s1ap_timer_buffer_t));

	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
				"context initialization failed.",__FUNCTION__);
		RRC_S1AP_UT_TRACE_EXIT();
		if (PNULL != p_rrc_s1ap_kill_req)
		{
			rrc_mem_free(p_rrc_s1ap_kill_req);
		}
		return;
	}

	/* Decode the message */
	if (RRC_FAILURE == decode_s1ap_asn_kill_request(
				p_api,
				&asn1_ctx,
				*asn_msg,
				p_rrc_s1ap_kill_req,
				p_mme_ctx))
	{
		RRC_S1AP_TRACE(RRC_ERROR,
				"decode_asn_kill_request returned Failure\n"
			      );
		if (PNULL != p_rrc_s1ap_kill_req)
		{
			rrc_mem_free(p_rrc_s1ap_kill_req);
		}

		/* Free ASN Context */
		rtFreeContext(&asn1_ctx);

		return;
	}
	/* Start the Guard Timer */
	s1ap_kill_timer_buf.timer_type =
		S1AP_KILL_GUARD_TIMER;

	s1ap_kill_timer_buf.auto_delete = RRC_TRUE;
	s1ap_kill_timer_buf.mme_id = p_mme_ctx->mme_id;

	/* Start the guard timer */
	p_s1ap_gb_ctx->kill_timer_id =  rrc_start_timer(
			p_s1ap_gb_ctx->s1_kill_duration,
			&s1ap_kill_timer_buf,
			sizeof(s1ap_kill_timer_buf),
			RRC_FALSE);

	if (PNULL == p_s1ap_gb_ctx->kill_timer_id)
	{
		RRC_S1AP_TRACE(RRC_ERROR,
				"Unable to start KILL guard timer\n");
		if (PNULL != p_rrc_s1ap_kill_req)
		{
			rrc_mem_free(p_rrc_s1ap_kill_req);
		}

		/* Free ASN Context */
		rtFreeContext(&asn1_ctx);

		return;
	}
	else
	{
		RRC_S1AP_TRACE(RRC_INFO,
				"[KILL guard timer] Started\n");
	}

	/*Allocate kill_info in s1ap global ctx*/
	p_s1ap_gb_ctx->p_kill_data = (kill_data_t*)rrc_mem_get(sizeof(kill_data_t));

	if(PNULL == p_s1ap_gb_ctx->p_kill_data)
	{
		RRC_S1AP_TRACE(RRC_ERROR,
				": Memory allocation failed!");
		if (PNULL != p_rrc_s1ap_kill_req)
		{
			rrc_mem_free(p_rrc_s1ap_kill_req);
		}

		/* Free ASN Context */
		rtFreeContext(&asn1_ctx);

		return;
	}

	/*Initializing pws_info*/
	memset_wrapper(p_s1ap_gb_ctx->p_kill_data,0,sizeof(kill_data_t));

	/* assigning mme_id */
	p_s1ap_gb_ctx->p_kill_data->mme_id = p_mme_ctx->mme_id;

	/*builds and sends S1AP_RRM_KILL_REQ message to RRM*/
	if (RRC_FAILURE == s1ap_rrm_build_and_send_kill_request(
				p_s1ap_gb_ctx,
				p_rrc_s1ap_kill_req))
	{
		RRC_S1AP_TRACE(RRC_ERROR,
				"builds and sends S1AP_RRM_KILL_REQ message"
				" returnd Failure\n");
		/* Stop the pws timer.*/
		if (PNULL != p_s1ap_gb_ctx->kill_timer_id)
		{
			rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
			p_s1ap_gb_ctx->kill_timer_id = PNULL;
		}

		/* Clean data related to kill */      
		enb_mgr_clean_kill_data(p_s1ap_gb_ctx);

		/*Change the state to ENB_MGR_ENB_MGR_IDLE*/
		ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_IDLE);
		RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_IDLE]");

	}
	else
	{
		/*Change the state to ENB_MGR_W_RRM_PWS_RESP*/
		ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRM_PWS_RESP);
		RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_W_RRM_PWS_RESP]");
	}

	if (PNULL != p_rrc_s1ap_kill_req)
	{
		rrc_mem_free(p_rrc_s1ap_kill_req);
	}

	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	RRC_S1AP_UT_TRACE_EXIT();
	return;
}
/* coverity_fix_54702_stop */
/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrm_resp_kill_rrm_resp
 * Inputs         : p_api - pointer to filled buffer 
 *                  p_msg -  pointer to filled buffer
 *                  p_s1ap_gb_ctx -  pointer to S1AP global context 
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function handels rrm_resp_kill_rrm_resp msg.
 ******************************************************************************/
void enb_mgr_fsm_w_rrm_resp_kill_rrm_resp(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
	s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_resp =
		(s1ap_rrm_kill_response_t *)p_msg;
	s1ap_kill_req_t *p_s1ap_kill_req = PNULL ;
	void *p_intrl_msg = PNULL;   
	U8   count = 0 ;
	U8   cell_index = 0 ;
	U32  pws_cell_index = 0; 
	U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
	U16 asn_enc_buf_len = RRC_NULL;
	rrc_bool_et  is_cell_found = RRC_FALSE;


	RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);
	memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

	RRC_S1AP_TRACE(RRC_DETAILED, 
			"Unused variable in FSM: p_api=%p", p_api);
	/* handling of response code */
    /* SPR 24009/SES-456 Fix Start */
    if (PNULL != p_s1ap_rrm_kill_resp)
    {
        /* SPR 24009/SES-456 Fix End */
        if ((RRC_SUCCESS == p_s1ap_rrm_kill_resp->response) &&
                (S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT & p_s1ap_rrm_kill_resp->bitmask) &&
                (p_s1ap_gb_ctx->p_kill_data != PNULL))
        {
            p_s1ap_gb_ctx->p_kill_data->kill_resp = rrc_mem_get (
                    sizeof(kill_resp_t));
            if(PNULL == p_s1ap_gb_ctx->p_kill_data->kill_resp)
            {
                RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
                return;
            }
            memset_wrapper (p_s1ap_gb_ctx->p_kill_data->kill_resp, RRC_NULL,
                    sizeof(kill_resp_t));
            for (count = 0; count <
                    p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.cell_count; count++)
            {
                /****** Sending S1AP_WRITE_REPLACE_WARNING_REQ ******/
                p_intrl_msg = s1ap_alloc_intrl_msg(
                        RRC_CSC_MODULE_ID,
                        S1AP_KILL_REQ,
                        sizeof (s1ap_kill_req_t));

                if (PNULL != p_intrl_msg)
                {
                    /* Fill and send message */
                    /* SPR 23905/SES-457 Fix +- */

                    p_s1ap_kill_req =
                        (s1ap_kill_req_t *)((U8 *)p_intrl_msg +
                                RRC_API_HEADER_SIZE);

                    /* To get the respective cell_index */
                    cell_index = p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                        rrc_rrm_pws_contents[count].cell_index;

                    /* To get the respective cell_index */
                    if ( PNULL != p_s1ap_gb_ctx->
                            cell_s1ap_contexts[cell_index])
                    {
                        if ( p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].cell_index == p_s1ap_gb_ctx->
                                cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.cell_index)
                        {
                            is_cell_found = RRC_TRUE;
                        }
                    }
                    /* Store the cells info received from RRM in s1ap_context */

                    /* --Increment the cell count*/
                    p_s1ap_gb_ctx->p_kill_data->kill_resp->count++;

                    /* --Store the cell index*/
                    p_s1ap_gb_ctx->p_kill_data->kill_resp->kill_resp_data[count].
                        cell_index = p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                        rrc_rrm_pws_contents[count].cell_index;

                    /* If Cell Index not found in the global context */
                    if ( RRC_FALSE == is_cell_found )
                    {
                        /*fill response*/
                        p_s1ap_gb_ctx->p_kill_data->kill_resp->
                            kill_resp_data[count].resp = RRC_FAILURE;

                        /*fill cause*/
                        p_s1ap_gb_ctx->p_kill_data->kill_resp->
                            kill_resp_data[count].fail_cause =
                            RRM_RRC_UNKNOWN_FAILURE;

                        /*set bitmask*/
                        p_s1ap_gb_ctx->p_kill_data->kill_resp->
                            kill_resp_data[count].bitmask = 
                            S1AP_CSC_KILL_RESP_FAIL_CAUSE_PRESENT;
                        if (p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                cell_count == 1)
                        {
                            RRC_S1AP_TRACE(RRC_ERROR,
                                    "Single Cell and Not found in S1AP Global Context\n");
                            /* Prepare kill response as failure 
                             * (not containing Broadcast cancelled area list) to MME*/ 
                            /* compose the message which is send to mme as a response */
                            if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_kill_resp_failure (
                                        p_s1ap_gb_ctx,
                                        p_mme_ctx,
                                        asn_enc_msg_buf,
                                        &asn_enc_buf_len))
                            {
                                RRC_S1AP_TRACE(RRC_ERROR,
                                        "%s:enb_mgr_build_and_send_s1ap_kill_resp_failure failed.",
                                        __FUNCTION__);
                                /* Coverity Fix 48263 Start */
                                if ( PNULL != p_intrl_msg )
                                {
                                    /* SPR 15887 Start */
                                    rrc_msg_mem_free(p_intrl_msg);
                                    /* SPR 15887 Stop */
                                    p_intrl_msg = PNULL;
                                }
                                /* Coverity Fix 48263 Stop */
                                return;
                            }
                            /*builds and sends S1AP_RRM_KILL_CNF message to RRM*/
                            if (RRC_FAILURE == s1ap_rrm_build_and_send_kill_cnf(
                                        p_s1ap_gb_ctx, RRC_FAILURE))
                            {
                                RRC_S1AP_TRACE(RRC_ERROR,
                                        "build_and_send_S1AP_RRM_KILL_CNF message returned Failure\n");
                                /* Coverity Fix 48263 Start */
                                if ( PNULL != p_intrl_msg )
                                {
                                    /* SPR 15887 Start */
                                    rrc_msg_mem_free(p_intrl_msg);
                                    /* SPR 15887 Stop */
                                    p_intrl_msg = PNULL;
                                }
                                /* Coverity Fix 48263 Stop */
                                return;
                            }
                            /* Set the state to ENB_MGR_IDLE */
                            ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE);
                            /* Coverity Fix 48263 Start */
                            if ( PNULL != p_intrl_msg )
                            {
                                /* SPR 15887 Start */
                                rrc_msg_mem_free(p_intrl_msg);
                                /* SPR 15887 Stop */
                                p_intrl_msg = PNULL;
                            }
                            /* coverity_fix_48263_stop */
                            return;
                        }
                    }
                    else
                    {
                        /* --Store the eCGI id */
                        is_cell_found = RRC_FALSE;
                        p_s1ap_gb_ctx->p_kill_data->kill_resp->
                            kill_resp_data[count].eCGI.plmn_identity =
                            p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.plmn_id;
                        /* --Store the cell id ,here first need to check cell index then only
                         * store the cell id of that cell*/
                        l3_memcpy_wrapper(p_s1ap_gb_ctx->p_kill_data->kill_resp->
                                kill_resp_data[count].eCGI.cell_identity,
                                p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index]->s1ap_cell_area_info.
                                cell_area_info.cell_id, CELL_ID_OCTET_SIZE);

                        /* Filling the API to be sent to CSC */

                        p_s1ap_kill_req->bitmask = 0;
                        /*cell_index*/
                        p_s1ap_kill_req->cell_index =
                            p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                            rrc_rrm_pws_contents[count].cell_index;

                        p_s1ap_kill_req->transaction_id =
                            s1ap_enb_mgr_get_next_transaction_id (p_s1ap_gb_ctx);

                        l3_memcpy_wrapper(p_s1ap_kill_req->message_id,
                                p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
                                message_identifier, MSG_ID_OCTET_SIZE);
                        l3_memcpy_wrapper(p_s1ap_kill_req->serial_number,
                                p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
                                serial_number, SERIAL_NUMBER_OCTET_SIZE);

                        /*scheduling info*/
                        p_s1ap_kill_req->scheduling_info_list =
                            p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                            rrc_rrm_pws_contents[count].scheduling_info_list;

                        if(RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT &
                                p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].bitmask)
                        {
                            p_s1ap_kill_req->bitmask |= KILL_SI_TRANSMISSION_INFO_LIST;
                            p_s1ap_kill_req->si_transmission_info = 
                                p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].si_transmission_info;
                        }

                        if(RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT &
                                p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].bitmask)
                        {
                            p_s1ap_kill_req->bitmask |= KILL_ETWS_SCHEDULING_INFO;
                            l3_memcpy_wrapper (&(p_s1ap_kill_req->etws_scheduling_info),
                                    &(p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list. \
                                        rrc_rrm_pws_contents[count].etws_scheduling_info),
                                    sizeof(etws_scheduling_info_t));
                        }

                        if(RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT &
                                p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].bitmask)
                        {
                            p_s1ap_kill_req->bitmask |= KILL_CMAS_SCHEDULING_INFO_LIST;
                            l3_memcpy_wrapper (&(p_s1ap_kill_req->cmas_scheduling_info_list),
                                    &(p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list. \
                                        rrc_rrm_pws_contents[count].cmas_scheduling_info_list),
                                    sizeof(cmas_scheduling_info_list_t));
                        }
                        /* Send response */
                        rrc_send_message(
                                p_intrl_msg,
                                RRC_CSC_MODULE_ID);

                        /* Storing the pws bitmask for this cell*/
                        pws_cell_index = (U16)(1 <<
                                p_s1ap_rrm_kill_resp->rrc_rrm_pws_contents_list.
                                rrc_rrm_pws_contents[count].cell_index);
                        p_s1ap_gb_ctx->p_kill_data->pws_cells |= (rrc_bitmask_t)pws_cell_index;
                    }
                } /* end of if condition */
            } /* end of for loop*/

            /*Change the state to ENB_MGR_W_RRC_PWS_RESP*/
            ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_W_RRC_PWS_RESP);
            RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [ENB_MGR_W_RRC_PWS_RESP]",
                    __FUNCTION__);
        }
        /* SPR 24009/SES-456 Fix Start */
        else if ((p_s1ap_rrm_kill_resp->response == RRC_FAILURE) &&
                (PNULL != p_s1ap_gb_ctx->p_kill_data))
            /* SPR 24009/SES-456 Fix End */
        {
            /* Prepare kill response as failure (not containing
             * Broadcast cancelled area list) to MME*/
            /* compose the message which is send to mme as a response */
            if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_kill_resp_failure (
                        p_s1ap_gb_ctx,
                        p_mme_ctx,
                        asn_enc_msg_buf,
                        &asn_enc_buf_len))
            {
                RRC_S1AP_TRACE(RRC_ERROR,
                        "%s:enb_mgr_build_and_send_s1ap_kill_resp_failure failed.",
                        __FUNCTION__);
                RRC_S1AP_UT_TRACE_EXIT();
                return;
            }
            /* Stop the kill timer.*/
            if (PNULL != p_s1ap_gb_ctx->kill_timer_id)
            {
                rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
                p_s1ap_gb_ctx->kill_timer_id = PNULL;
            }

            /*Change the state to ENB_MGR_ENB_MGR_IDLE*/
            /* SPR 24009/SES-456 Fix Start */
            enb_mgr_clean_kill_data(p_s1ap_gb_ctx);
            ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx, ENB_MGR_IDLE);
            RRC_S1AP_TRACE(RRC_INFO,"::Set State [ENB_MGR_IDLE]");
            /* SPR 24009/SES-456 Fix End */
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"Invalid response received from RRM");
            return;
        }
        /* SPR 24009/SES-456 Fix Start */
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"p_s1ap_rrm_kill_resp is NULL");
        return;
    }
    /* SPR 24009/SES-456 Fix End */
    /* SPR_19956 start */
    /* code_removed */
    RRC_S1AP_UT_TRACE_EXIT();
    /* SPR_19956 stop */
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrm_resp_kill_request_handler
 * Inputs         : p_api - pointer to input API data 
 *                  p_msg - poiter to filled message
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx - pointer to MME contxet
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the rrm_resp_kill_request message 
 ******************************************************************************/
void enb_mgr_fsm_w_rrm_resp_kill_request_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
    /* SPR 15641 Fix Start */
    s1ap_kill_queue_node_t	*p_s1ap_kill_queue_node = PNULL;
    list_t                	*p_s1ap_kill_queue = PNULL;
    U16 *asn_msg = (U16 *)p_msg;

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();
/* coverity_86753 start */
    p_s1ap_kill_queue_node = rrc_mem_get ((rrc_size_t)sizeof(s1ap_kill_queue_node_t) + 
        (rrc_size_t)rrc_get_api_buf_size(p_api)); 			
/* coverity_86753 stop */

    if ( PNULL != p_s1ap_kill_queue_node )
    {
        p_s1ap_kill_queue_node->mme_id = p_mme_ctx->mme_id;
    
        p_s1ap_kill_queue_node->asn_msg_len = *asn_msg; 
                            
        l3_memcpy_wrapper((void *)p_s1ap_kill_queue_node->message,
            (const void *)p_api, (rrc_get_api_buf_size(p_api)));
    
        p_s1ap_kill_queue = &p_s1ap_gb_ctx->s1ap_kill_queue;
                            
        S1AP_KILL_Q_ENQUEUE (p_s1ap_kill_queue, p_s1ap_kill_queue_node);
    	
        RRC_S1AP_TRACE(RRC_INFO," [%s]Kill Request enqueued in S1AP KILL QUEUE.",
    	    __FUNCTION__);
    }

    /* SPR 15641 Fix Stop */
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrm_resp_timer_expiry_handler
 * Inputs         : p_api - pointer to input API data
 *                  p_msg - pointer to filled msg
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx - pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles rrm responce timer expiry .
 ******************************************************************************/
void enb_mgr_fsm_w_rrm_resp_timer_expiry_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
    U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_enc_buf_len = RRC_NULL;


    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_DETAILED, 
            "Unused variable in FSM:p_api=%p and p_msg=%p", p_api, p_msg);
    p_s1ap_gb_ctx->kill_timer_id = PNULL;

    p_s1ap_gb_ctx->kill_timer_id = PNULL;

    memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
    if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_kill_resp_failure (
				p_s1ap_gb_ctx,
                                p_mme_ctx,
				asn_enc_msg_buf,
				&asn_enc_buf_len))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
             "%s:enb_mgr_build_and_send_s1ap_kill_resp_failure failed.",
                    __FUNCTION__);
        return;
    }
    /*builds and sends S1AP_RRM_KILL_CNF message to RRM*/
    if (RRC_FAILURE == s1ap_rrm_build_and_send_kill_cnf(
                                    p_s1ap_gb_ctx, RRC_FAILURE))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
          "build_and_send_S1AP_RRM_KILL_CNF message returned Failure\n");
        return;
    }
    /* Timer expired, set the state to ENB_MGR_IDLE */
    ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE);

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrc_resp_timer_expiry_handler
 * Inputs         : p_api - pointer to input API buffer  
 *                  p_msg - pointer to filled buffer 
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the rrc_resp_timer_expiry event.
 ******************************************************************************/
void enb_mgr_fsm_w_rrc_resp_timer_expiry_handler(
    void                *p_api,
    void                *p_msg,
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    mme_context_t* p_mme_ctx)
{
    U8 count = RRC_NULL;

    s1ap_kill_failure_ind_t *p_s1ap_kill_failure = PNULL;
    U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    s1ap_timer_buffer_t s1ap_kill_timer_buf;

    void *p_intrl_msg = PNULL;

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_DETAILED,
        "Unused variable in FSM:p_api=%p and p_msg=%p", p_api, p_msg);

    p_s1ap_gb_ctx->kill_timer_id = PNULL;
    
    p_s1ap_gb_ctx->kill_timer_id = PNULL;

    memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));

    memset_wrapper(&s1ap_kill_timer_buf, 0, sizeof(s1ap_timer_buffer_t));

    if ((PNULL != p_s1ap_gb_ctx->p_kill_data) && 
         (PNULL != p_s1ap_gb_ctx->p_kill_data->kill_resp) && 
         (p_s1ap_gb_ctx->p_kill_data->kill_resp->count > 0))
    {
        if (RRC_NULL != p_s1ap_gb_ctx->p_kill_data->pws_cells)
        {
            for (count = 0; count <
                    p_s1ap_gb_ctx->p_kill_data->kill_resp->count; count++)
            {

                /****** Sending S1AP_KILL_FAILURE_IND ******/
                p_intrl_msg = s1ap_alloc_intrl_msg(
                        RRC_CSC_MODULE_ID,
                        S1AP_KILL_FAILURE_IND,
                        sizeof (s1ap_kill_failure_ind_t));

                if (PNULL != p_intrl_msg)
                {
                    /* Fill and send message */
                    p_s1ap_kill_failure = (s1ap_kill_failure_ind_t *)(
                            (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

                    memset_wrapper (p_s1ap_kill_failure, 0, sizeof (s1ap_kill_failure_ind_t));
                    /* Filling the API to be sent to CSC */
                    if(p_s1ap_gb_ctx->p_kill_data->pws_cells &
                            (1 << p_s1ap_gb_ctx->p_kill_data->
                             kill_resp->kill_resp_data[count].cell_index))
                    {
                        p_s1ap_kill_failure->cell_index = p_s1ap_gb_ctx->p_kill_data->
                            kill_resp->kill_resp_data[count].cell_index;

                        RRC_S1AP_TRACE(RRC_INFO,
                                "[start roolback] - "
                                "send failure indication to CSC ");

                        /* Send failure response to CSC */
                        rrc_send_message(
                                p_intrl_msg,
                                RRC_CSC_MODULE_ID);
                        p_s1ap_gb_ctx->p_kill_data->pws_cells &= (rrc_bitmask_t)(~(1 << 
                                    p_s1ap_gb_ctx->p_kill_data->kill_resp->kill_resp_data[count].cell_index));  
                    }
                    else
                    {
                        /* coverity_fix_48262_start */
                        rrc_msg_mem_free(p_intrl_msg);
                        p_intrl_msg = PNULL;
                        /* coverity_fix_48262_stop */
                    }
                }
            }
        }


        /*Check if All CSCs has sent the response or not*/
        if (p_s1ap_gb_ctx->p_kill_data->pws_cells == RRC_NULL)
        {
            prepare_and_send_kill_resp_and_kill_cnf (p_s1ap_gb_ctx, p_mme_ctx);
            /* set the state to ENB_MGR_IDLE */
            ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE); 
        }
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : s1ap_rrm_build_and_send_kill_request 
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global contxet 
 *                  p_rrc_s1ap_kill_req - pointer to rrc_s1ap_kill_request_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE 
 * Description    : This function builds and sends S1AP_RRM_KILL_REQUEST message
 *                  to RRM module.
 ********************************************************************************/
rrc_return_et s1ap_rrm_build_and_send_kill_request (
    s1ap_gb_context_t * p_s1ap_gb_ctx,
    rrc_s1ap_kill_request_t *p_rrc_s1ap_kill_req )
{
     s1ap_rrm_kill_request_t *rrm_s1ap_kill_request = PNULL;
     rrc_return_et result = RRC_FAILURE; 
     rrc_transaction_id_t trans_id = RRC_NULL;

     RRC_S1AP_UT_TRACE_ENTER();

     RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
     RRC_ASSERT(PNULL != p_rrc_s1ap_kill_req);
     
     /* Allocating Memory */
     rrm_s1ap_kill_request = rrc_mem_get(sizeof(s1ap_rrm_kill_request_t));
     if (PNULL == rrm_s1ap_kill_request)
     {
         RRC_S1AP_TRACE(RRC_ERROR,"Uanble to allocate memory.");
         /*+-Coverity 88134 Fix Code Deleted*/
         RRC_S1AP_UT_TRACE_EXIT();
         return result ;
     }

     /* Clean up message */
     memset_wrapper(rrm_s1ap_kill_request,
                0,
                sizeof(s1ap_rrm_kill_request_t));
     trans_id = s1ap_enb_mgr_get_next_transaction_id(p_s1ap_gb_ctx);
     if (PNULL != p_rrc_s1ap_kill_req)
     {
       /* Fill Message */
       /* disable masks by default */
       rrm_s1ap_kill_request->bitmask = 0;
       l3_memcpy_wrapper(rrm_s1ap_kill_request->message_identifier,
                p_rrc_s1ap_kill_req->message_identifier.data,
                sizeof(U16));
       l3_memcpy_wrapper(rrm_s1ap_kill_request->serial_number,
                p_rrc_s1ap_kill_req->serial_number.data,
                sizeof(U16));  
      
       if(RRC_S1AP_KILL_REQUEST_WARNING_AREA_LIST_PRESENT & 
           p_rrc_s1ap_kill_req->bitmask)
       {
          enb_mgr_fill_warning_area_list(
             &(rrm_s1ap_kill_request->warning_area_list),
             &p_rrc_s1ap_kill_req->warning_area_list);

          rrm_s1ap_kill_request->bitmask = KILL_WARNING_AREA_LIST;

       }  
     } 
     /* store kill_data in s1ap global context*/
     p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request =
         rrm_s1ap_kill_request;
 
     /* PUP encode and send message to RRM */
     result = rrc_s1apRrm_il_send_s1ap_rrm_kill_request(
        rrm_s1ap_kill_request,
        RRC_S1AP_MODULE_ID, 
        RRC_RRM_MODULE_ID,
        trans_id,
        RRC_NULL);
     if (RRC_SUCCESS == result)
     {
       RRC_S1AP_TRACE(RRC_INFO, "KILL "
          " Message sent to RRM");
     }
     else
     {
       RRC_S1AP_TRACE(RRC_INFO,
          "Unable to send message to RRM");
     }
     RRC_S1AP_UT_TRACE_EXIT();
     return result ;
}

/*****************************************************************************
 * Function Name  : s1ap_rrm_build_and_send_kill_cnf 
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global context
 *                  kill_resp - responce
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function builds and sends S1AP_RRM_KILL_CNF message
 *                  to RRM module.
 ********************************************************************************/
rrc_return_et s1ap_rrm_build_and_send_kill_cnf
(
 s1ap_gb_context_t     *p_s1ap_gb_ctx,
 U8                     kill_resp
 )
{
  rrc_return_et result = RRC_FAILURE;
  s1ap_rrm_kill_cnf_t rrm_kill_cnf;
  U8 pws_cell_counter = RRC_NULL;

  rrc_transaction_id_t trans_id = RRC_NULL;

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

  /* Clean up message */
  memset_wrapper(&rrm_kill_cnf, 0, sizeof(rrm_kill_cnf));

  trans_id = s1ap_enb_mgr_get_next_transaction_id(p_s1ap_gb_ctx);

  do
  {
    rrm_kill_cnf.bitmask = 0;
    /* message identifier */
    l3_memcpy_wrapper(rrm_kill_cnf.message_identifier,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->message_identifier,
        sizeof(p_s1ap_gb_ctx->p_kill_data->
                s1ap_rrm_kill_request->message_identifier));

    /* serial number */
    l3_memcpy_wrapper(rrm_kill_cnf.serial_number,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->serial_number));

    if(RRC_FALSE != kill_resp)
    {

      /* Fill pws response */
      rrm_kill_cnf.kill_resp = RRC_SUCCESS;
      /* Fill Broadcast Cancellation info list bitmask */
      rrm_kill_cnf.bitmask
        |= S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT;

      /* Fill KILL Broadcast Cancellation Message list */
      for (pws_cell_counter = 0; pws_cell_counter <
          p_s1ap_gb_ctx->p_kill_data->kill_resp->count; pws_cell_counter++)
      {
        /* fill kill_broadcast_cancellation_list */
        (rrm_kill_cnf.broadcast_cancellation_list.count)++;
       
        /* fill bitmask */
        rrm_kill_cnf.broadcast_cancellation_list.
            broadcast_cancellation[pws_cell_counter].bitmask = 0;

        /* fill cell_index */
        rrm_kill_cnf.broadcast_cancellation_list.
            broadcast_cancellation[pws_cell_counter].cell_index =
              p_s1ap_gb_ctx->p_kill_data->kill_resp->
                 kill_resp_data[pws_cell_counter].cell_index;

        /* response */
        rrm_kill_cnf.broadcast_cancellation_list.
            broadcast_cancellation[pws_cell_counter].response =
          (U8)(p_s1ap_gb_ctx->p_kill_data->kill_resp->
            kill_resp_data[pws_cell_counter].resp);

        /* fail_cause */
        if (p_s1ap_gb_ctx->p_kill_data->kill_resp->
              kill_resp_data[pws_cell_counter].resp == RRC_FAILURE)
        {
          if (S1AP_CSC_KILL_RESP_FAIL_CAUSE_PRESENT &
              p_s1ap_gb_ctx->p_kill_data->kill_resp->
                kill_resp_data[pws_cell_counter].bitmask)
          {
            rrm_kill_cnf.broadcast_cancellation_list.
                broadcast_cancellation[pws_cell_counter].bitmask |=
                      S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT;
            rrm_kill_cnf.broadcast_cancellation_list.
                broadcast_cancellation[pws_cell_counter].fail_cause =
              p_s1ap_gb_ctx->p_kill_data->kill_resp->
                kill_resp_data[pws_cell_counter].fail_cause;
          }
        }
      } /* end of for loop*/
    }
    else
    {
      /* Fill pws response */
      rrm_kill_cnf.kill_resp = RRC_FAILURE;
    }
    /* PUP encode and send message to RRM */
    result = rrc_s1apRrm_il_send_s1ap_rrm_kill_cnf(
        &rrm_kill_cnf,
        RRC_S1AP_MODULE_ID,
        RRC_RRM_MODULE_ID,
        trans_id,
        RRC_NULL);
    if (RRC_SUCCESS == result)
    {
      RRC_S1AP_TRACE(RRC_INFO,
          "KILL CNF Message sent to RRM");
    }
    else
    {
      RRC_S1AP_TRACE(RRC_INFO,"%s: "
          "Unable to send message to RRM", __FUNCTION__);
      break;
    }
    result = RRC_SUCCESS;
  }while(0);

  enb_mgr_clean_kill_data(p_s1ap_gb_ctx);
  return result;
}

/*****************************************************************************
 * Function Name  : enb_mgr_build_and_send_s1ap_kill_resp_failure
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context 
 *                  asn_enc_msg_buf - pointer to ASN encoded msg buffer
 *                  asn_enc_buf_len - pointer to ASN encoded msg length 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is composing the kill resp.
 ******************************************************************************/
rrc_return_t enb_mgr_build_and_send_s1ap_kill_resp_failure
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t     *p_mme_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len
 )
{
  rrc_return_et response = RRC_FAILURE;
  OSCTXT asn1_ctx;
  S1AP_PDU s1ap_pdu;
  s1ap_KillResponse *p_s1ap_kill_reponse = PNULL ;
  s1ap_MessageIdentifier message_identifier;
  s1ap_SerialNumber serial_number;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT(PNULL != asn_enc_msg_buf);

  /* Init ASN1 context */
  if ( 0 != rtInitContext(&asn1_ctx) )
  {
    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
        __FUNCTION__);
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
  }

  do
  {
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /* Fill the values in the ASN structures that shall be encoded by
     *  ASN Encoder */
    /* Set Pdu type to successfuloutcome message */
    s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

    s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
        SuccessfulOutcome);
    if (PNULL == s1ap_pdu.u.successfulOutcome)
    {
             RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      break;
    }
    asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

    s1ap_pdu.u.successfulOutcome->procedureCode =
              ASN1V_s1ap_id_Kill ;

    s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

    /* Set the initiating message type to Write Replace Warning */
    s1ap_pdu.u.successfulOutcome->value.t = 
             T1_kill ;

     p_s1ap_kill_reponse =rtxMemAllocType(&asn1_ctx,s1ap_KillResponse);
   
     if (PNULL == p_s1ap_kill_reponse)
     {
          RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
          response = RRC_FAILURE;
          break;
      }

     memset_wrapper (p_s1ap_kill_reponse, 0, sizeof (s1ap_KillResponse));
     s1ap_pdu.u.successfulOutcome->value.u.kill =
         p_s1ap_kill_reponse;

     rtxDListInit(&p_s1ap_kill_reponse->protocolIEs);

    /****** compose message identifier ********/
    memset_wrapper(&message_identifier,0,sizeof(s1ap_MessageIdentifier));

    message_identifier.numbits = (MSG_ID_OCTET_SIZE*8);

    l3_memcpy_wrapper(message_identifier.data,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->message_identifier,
        sizeof(p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
            message_identifier));

    if (0 !=
            asn1Append_s1ap_KillResponse_protocolIEs_1(&asn1_ctx,
                &p_s1ap_kill_reponse->protocolIEs,
                &message_identifier))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_KillResponse_protocolIEs_1 append "
                "failed.");
        break;
    }

    /***Serial number****/
    memset_wrapper (&serial_number,0,sizeof(s1ap_SerialNumber));

    serial_number.numbits = SERIAL_NUMBER_OCTET_SIZE*8;

    l3_memcpy_wrapper(serial_number.data,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
                  serial_number));

    if(0 !=
            asn1Append_s1ap_KillResponse_protocolIEs_2(&asn1_ctx,
                &p_s1ap_kill_reponse->protocolIEs,
                &serial_number))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_KillResponse_protocolIEs_2 append "
                "failed.");
        break;
    }

    rtxDListInit(&p_s1ap_kill_reponse->extElem1);    
    /* ASN Encode message */
    pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
        S1AP_MAX_ASN1_BUF_LEN, TRUE);
    if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
    {
      RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of Kill Resp "
          "failed.");
      break;
    }
    else
    {
      *asn_enc_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
      response = RRC_SUCCESS;
    }
  }while(0);

  /* Free ASN1 context */
  rtFreeContext(&asn1_ctx);

  if (PNULL != p_s1ap_gb_ctx->p_kill_data)
  {
	  p_mme_ctx = p_s1ap_gb_ctx->mme_contexts
		  [ p_s1ap_gb_ctx->p_kill_data->mme_id];
  }

  /* Send the message to MME */
  if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx,
                                asn_enc_msg_buf,
                                *asn_enc_buf_len,
                                PNULL))
  {
      RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d Kill Message "
	      "Send Failed", p_mme_ctx->mme_id);

      response = RRC_FAILURE;
  }
  
  RRC_S1AP_UT_TRACE_EXIT();

  return response;
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_w_rrc_resp_csc_kill_resp
 * Inputs         : p_api - pointer to input API buffer 
 *                  p_msg - pointer to filled msg 
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is handling csc kill success handler
 ******************************************************************************/
void enb_mgr_fsm_w_rrc_resp_csc_kill_resp (
 void                * p_api,    /* p_api */
 void                 *p_msg,
 s1ap_gb_context_t   *p_s1ap_gb_ctx, /* s1ap context */
 mme_context_t       *p_mme_ctx
 )
{
  s1ap_kill_resp_t *p_s1ap_kill_resp =
    (s1ap_kill_resp_t *)(p_msg);

  rrc_cell_index_t pws_cell_index = RRC_NULL;
  U8 pws_cell_counter = RRC_NULL;
  U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];

  RRC_S1AP_UT_TRACE_ENTER();

  RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT (PNULL != p_mme_ctx);
  RRC_ASSERT (PNULL != p_s1ap_kill_resp);

  RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",p_api);
  memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
  pws_cell_index = p_s1ap_kill_resp->cell_index;

  do {
      if(PNULL == p_s1ap_gb_ctx->p_kill_data)
      {
          RRC_S1AP_TRACE(RRC_WARNING, "kill_data pointer is null.");
          break;
      }

      if(PNULL != p_s1ap_gb_ctx->p_kill_data->kill_resp)
      {
          /*Check for which CSC the reply has come*/
          for (pws_cell_counter = 0; pws_cell_counter <
                  p_s1ap_gb_ctx->p_kill_data->kill_resp->count; pws_cell_counter++)
          {
              if (pws_cell_index == p_s1ap_gb_ctx->p_kill_data->kill_resp->
                      kill_resp_data[pws_cell_counter].cell_index)
              {
                  if(p_s1ap_gb_ctx->p_kill_data->pws_cells & (1 << pws_cell_index))
                  {
                      /*fill response*/
                      p_s1ap_gb_ctx->p_kill_data->kill_resp->
                          kill_resp_data[pws_cell_counter].resp = 
                          p_s1ap_kill_resp->kill_response;

                      if (RRC_FAILURE == p_s1ap_gb_ctx->p_kill_data->
                              kill_resp->kill_resp_data[pws_cell_counter].resp)
                      {
                          /*fill cause*/
                          p_s1ap_gb_ctx->p_kill_data->kill_resp->
                              kill_resp_data[pws_cell_counter].fail_cause =
                              (rrm_fail_cause_et)p_s1ap_kill_resp->kill_fail_cause;

                          /*set bitmask*/
                          p_s1ap_gb_ctx->p_kill_data->kill_resp->
                              kill_resp_data[pws_cell_counter].bitmask |=
                              S1AP_CSC_KILL_RESP_FAIL_CAUSE_PRESENT;
                      }
                      else if (RRC_SUCCESS == p_s1ap_kill_resp->kill_response)
                      {
                          /*fill cause*/
                          p_s1ap_gb_ctx->p_kill_data->kill_resp->
                              kill_resp_data[pws_cell_counter].num_of_broadcasts =
                              p_s1ap_kill_resp->number_of_broadcasts;
                      }
                      p_s1ap_gb_ctx->p_kill_data->pws_cells &= 
                          (rrc_bitmask_t)(~(1 << pws_cell_index));

                      RRC_S1AP_TRACE(RRC_INFO,
                              "%s:s1ap_kill_resp recvd for cell index [%d].",
                              __FUNCTION__,pws_cell_index);
                  }
                  else
                  {
                      RRC_S1AP_TRACE(RRC_INFO,
                              "s1ap_kill_resp recvd for cell index [%d] again!."
                              ,pws_cell_index);

                  }
                  break;
              }
          }
      }

      /*Check if All CSCs has sent the response or not*/
      if (p_s1ap_gb_ctx->p_kill_data->pws_cells == RRC_NULL)
      {
          prepare_and_send_kill_resp_and_kill_cnf(p_s1ap_gb_ctx, p_mme_ctx);

          /* stop the kill timer */
          if (PNULL != p_s1ap_gb_ctx->kill_timer_id)
          {
              rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
              p_s1ap_gb_ctx->kill_timer_id = PNULL;
          }

          /* set the state to ENB_MGR_IDLE */
          ENB_MGR_SET_FSM_STATE(p_s1ap_gb_ctx,ENB_MGR_IDLE);

          break;
      }
  }
  while(0);
  if (PNULL != p_s1ap_gb_ctx->kill_timer_id)
  {
      rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
      p_s1ap_gb_ctx->kill_timer_id = PNULL;
  }

  RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fill_broadcast_cancelled_area_list
 * Inputs         : p_asn1_ctx
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                 
 * Outputs        : p_kill_reponse (s1ap_KillResponse)
 * Returns        : RRC_SUCCESS
 *                  RRC_FAILURE
 * Description    : This function fills broadcast_cancelled_area_list
 ******************************************************************************/
static rrc_return_t enb_mgr_fill_broadcast_cancelled_area_list(
        OSCTXT    *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_KillResponse *p_kill_reponse,
        s1ap_BroadcastCancelledAreaList *p_broadcast_cancelled_area_list,
        s1ap_CellID_Cancelled           *p_cell_id_cancelled,
        s1ap_TAI_Cancelled              *p_tai_cancelled,
        s1ap_CancelledCellinEAI         *p_cancelled_cell_eai,
        s1ap_CancelledCellinTAI         *p_cancelled_cell_tai,
        s1ap_EmergencyAreaID_Cancelled  *p_emer_area_cancelled)
{
    OSRTDListNode *p_node = PNULL;
    OSRTDListNode *p_node2 = PNULL;
    OSRTDListNode *p_node3 = PNULL;
    OSRTDListNode *p_node4 = PNULL;
    OSRTDListNode *p_node5 = PNULL;
    OSRTDListNode *p_node6 = PNULL;
    kill_resp_data_t  *p_kill_resp = PNULL;
    U8 cell_count = RRC_NULL;
    U8 tai_count = RRC_NULL;
    U8 pws_cell_counter = RRC_NULL;
    U8 pws_cell_index = RRC_NULL;
    U8 emer_count = RRC_NULL;

    s1ap_CancelledCellinEAI_Item *p_s1ap_CancelledCellinEAI_Item = PNULL ;
    s1ap_CancelledCellinTAI_Item *p_s1ap_CancelledCellinTAI_Item = PNULL ;
    s1ap_CellID_Cancelled_Item *p_s1ap_CellID_Cancelled_Item = PNULL ;
    s1ap_TAI_Cancelled_Item *p_s1ap_TAI_Cancelled_Item = PNULL ;
    s1ap_EmergencyAreaID_Cancelled_Item *p_s1ap_EmergencyAreaID_Cancelled_Item = PNULL ;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_INFO,"Filling broadcast cancelled area list");

    asn1Init_s1ap_BroadcastCancelledAreaList(p_broadcast_cancelled_area_list);


    /*****Fill Tracking Area Id Broadcast*****/
    if (p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
            warning_area_list.bitmask
            & WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST)
    {
        p_broadcast_cancelled_area_list->t =
            T_s1ap_BroadcastCancelledAreaList_tAI_Cancelled;

        /* Initialise the Broadcast list*/
        rtxDListInit(p_tai_cancelled);
        p_broadcast_cancelled_area_list->u.tAI_Cancelled =
            p_tai_cancelled;

        /* Traverse the TAI list(stored in gb context) and fetch the cell
         * ids corresponding to TAI element */
        for (tai_count = 0; tai_count <
                p_s1ap_gb_ctx->p_kill_data->tai_list->tai_count;
                tai_count++ )
        {
            /* Check the response of each cell*/
            /* if success, then sends its eCGI in broadcast info list*/

            rtxDListAllocNodeAndData(p_asn1_ctx,
                    s1ap_TAI_Cancelled_Item,
                    &p_node3,
                    &p_s1ap_TAI_Cancelled_Item);

            if (PNULL == p_node3)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return RRC_FAILURE;
            }
            /* Initialise the TAI item*/
            asn1Init_s1ap_TAI_Cancelled_Item(p_s1ap_TAI_Cancelled_Item);
            /* (1)fill the TAI */

            /*-- Fill the TAC*/
            p_s1ap_TAI_Cancelled_Item->tAI.tAC.numocts = TAC_OCTET_SIZE;
            l3_memcpy_wrapper(p_s1ap_TAI_Cancelled_Item->tAI.tAC.data,
                    p_s1ap_gb_ctx->p_kill_data->tai_list->tai_info[tai_count].
                    tai.tracking_area_code,TAC_OCTET_SIZE);

            /* --Fill the PLMN ID*/
            p_s1ap_TAI_Cancelled_Item->tAI.pLMNidentity.numocts =
                MAX_PLMN_ID_BYTES;

            l3_memcpy_wrapper(p_s1ap_TAI_Cancelled_Item->tAI.pLMNidentity.data,
                    p_s1ap_gb_ctx->p_kill_data->tai_list->tai_info[tai_count].
                    tai.plmn_identity.plmn_id,MAX_PLMN_ID_BYTES);

            /* (2)Fill the completedCellinTAI */
            /* Fill all the eCGIs corresponding to TAI*/

            /* Initilise the cancelled cell tai list*/
            rtxDListInit(p_cancelled_cell_tai);

            for(cell_count = 0; cell_count < p_s1ap_gb_ctx->
                    p_kill_data->tai_list->tai_info[tai_count].cell_count;
                    cell_count++ )
            {
                /* First check the cell index, if found in pws_resp list then
                 * add eCGI corresponding to that cell index*/
                for (pws_cell_counter = 0; pws_cell_counter <
                        p_s1ap_gb_ctx->p_kill_data->kill_resp->count; pws_cell_counter++)
                {
                    p_kill_resp = &p_s1ap_gb_ctx->p_kill_data->
                        kill_resp->kill_resp_data[pws_cell_counter];

                    pws_cell_index = p_s1ap_gb_ctx->p_kill_data->kill_resp->
                        kill_resp_data[pws_cell_counter].cell_index;

                    if(pws_cell_index == p_s1ap_gb_ctx->p_kill_data->tai_list->
                            tai_info[tai_count].cell_index[cell_count])
                    {
                        rtxDListAllocNodeAndData(p_asn1_ctx,
                                s1ap_CancelledCellinTAI_Item,
                                &p_node4,
                                &p_s1ap_CancelledCellinTAI_Item);

                        if (PNULL == p_node4)
                        {
                            /* Not enough memory */
                            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                            return RRC_FAILURE;
                        }
                        asn1Init_s1ap_CancelledCellinTAI_Item(
                                p_s1ap_CancelledCellinTAI_Item);
                        p_s1ap_CancelledCellinTAI_Item->eCGI.pLMNidentity.numocts
                            = MAX_PLMN_ID_BYTES;

                        l3_memcpy_wrapper(p_s1ap_CancelledCellinTAI_Item->eCGI.pLMNidentity.data,
                                p_kill_resp->eCGI.plmn_identity.plmn_id,
                                sizeof(p_kill_resp->eCGI.plmn_identity.plmn_id));

                        p_s1ap_CancelledCellinTAI_Item->eCGI.cell_ID.numbits =
                            CELL_ID_NUM_BITS;
                       
                        /* BUG 10974 start */
                        l3_memcpy_wrapper(p_s1ap_CancelledCellinTAI_Item->eCGI.cell_ID.data,
                                p_kill_resp->eCGI.cell_identity,
                                sizeof(p_kill_resp->eCGI.cell_identity));

                        /* BUG 10974 stop */

                        p_s1ap_CancelledCellinTAI_Item->numberOfBroadcasts =
                            p_kill_resp->num_of_broadcasts;
                        rtxDListAppendNode(p_cancelled_cell_tai, p_node4);
                    }
                }
            }
            /* Assigning the cancelled cell_tai_list to TAI_broadcast_item*/
            p_s1ap_TAI_Cancelled_Item->cancelledCellinTAI = *p_cancelled_cell_tai;
            rtxDListAppendNode(p_tai_cancelled, p_node3);
        }
    }

    /*****Fi1l Emergency Area Broadcast*****/
    else if (p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
            warning_area_list.bitmask
            & WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST)
    {
        RRC_S1AP_TRACE(RRC_INFO,"Emergency Area Broadcast");

        p_broadcast_cancelled_area_list->t =
            T_s1ap_BroadcastCancelledAreaList_emergencyAreaID_Cancelled;

        /* Initialise the Broadcast list*/
        rtxDListInit(p_emer_area_cancelled);
        p_broadcast_cancelled_area_list->u.cellID_Cancelled =
            p_emer_area_cancelled;

        /* Traverse the Emergency list(stored in gb context) and fetch the cell
         * ids corresponding to emergency id element */
        for (emer_count = 0; emer_count <
                p_s1ap_gb_ctx->p_kill_data->emergency_list->emer_area_count;
                emer_count++ )
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    s1ap_EmergencyAreaID_Cancelled_Item,
                    &p_node5,
                    &p_s1ap_EmergencyAreaID_Cancelled_Item);

            if (PNULL == p_node5)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return RRC_FAILURE;
            }

            /* Initialise the TAI item*/
            asn1Init_s1ap_EmergencyAreaID_Cancelled_Item(
                    p_s1ap_EmergencyAreaID_Cancelled_Item);

            /*(1)Fill the emergencyAreaID*/
            p_s1ap_EmergencyAreaID_Cancelled_Item->emergencyAreaID.numocts =
                EMERGENCY_AREA_SIZE;

            l3_memcpy_wrapper(p_s1ap_EmergencyAreaID_Cancelled_Item->
                    emergencyAreaID.data,
                    p_s1ap_gb_ctx->p_kill_data->emergency_list->
                    emer_area_info[emer_count].emergency_area_id,
                    EMERGENCY_AREA_SIZE);

            /* (1)Filll the cancelledCellinEAI list*/
            rtxDListInit(p_cancelled_cell_eai);

            for(cell_count = 0; cell_count < p_s1ap_gb_ctx->p_kill_data->
                    emergency_list->emer_area_info[emer_count].cell_count; cell_count++)
            {
                for (pws_cell_counter = 0;
                        pws_cell_counter < p_s1ap_gb_ctx->p_kill_data->kill_resp->count;
                        pws_cell_counter++)
                {
                    p_kill_resp =
                        &p_s1ap_gb_ctx->p_kill_data->kill_resp->kill_resp_data[pws_cell_counter];
                    pws_cell_index = p_s1ap_gb_ctx->p_kill_data->
                        kill_resp->kill_resp_data[pws_cell_counter].cell_index;

                    if(pws_cell_index == p_s1ap_gb_ctx->p_kill_data->emergency_list->
                            emer_area_info[emer_count].cell_index[cell_count])
                    {
                        rtxDListAllocNodeAndData(p_asn1_ctx,
                                s1ap_CancelledCellinEAI_Item,
                                &p_node6,
                                &p_s1ap_CancelledCellinEAI_Item);

                        if (PNULL == p_node6)
                        {
                            /* Not enough memory */
                            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                            return RRC_FAILURE;
                        }
                        asn1Init_s1ap_CancelledCellinEAI_Item(
                                p_s1ap_CancelledCellinEAI_Item);

                        p_s1ap_CancelledCellinEAI_Item->eCGI.pLMNidentity.numocts
                            = MAX_PLMN_ID_BYTES;

                        l3_memcpy_wrapper(p_s1ap_CancelledCellinEAI_Item->
                                eCGI.pLMNidentity.data,
                                p_kill_resp->eCGI.
                                plmn_identity.plmn_id,MAX_PLMN_ID_BYTES);

                        p_s1ap_CancelledCellinEAI_Item->eCGI.cell_ID.numbits =
                            CELL_ID_NUM_BITS;

                        /* BUG 10974 start */
                        l3_memcpy_wrapper(p_s1ap_CancelledCellinEAI_Item->eCGI.cell_ID.data,
                                p_kill_resp->eCGI.cell_identity,
                                sizeof(p_s1ap_CancelledCellinEAI_Item->eCGI.cell_ID.data));
                        /* BUG 10974 stop */


                        p_s1ap_CancelledCellinEAI_Item->numberOfBroadcasts =
                            p_kill_resp->num_of_broadcasts;

                        rtxDListAppendNode(p_cancelled_cell_eai, p_node6);
                    }
                }
            }
            p_s1ap_EmergencyAreaID_Cancelled_Item->cancelledCellinEAI = *p_cancelled_cell_eai;
            rtxDListAppendNode(p_emer_area_cancelled, p_node5);
        }
    }
    
    /*****Fill CellId Broadcast*****/
    else
    {
        p_broadcast_cancelled_area_list->t =
            T_s1ap_BroadcastCancelledAreaList_cellID_Cancelled;
        /* Start filling up the kill resp by fetching all the cell index */
        rtxDListInit(p_cell_id_cancelled);
        p_broadcast_cancelled_area_list->u.cellID_Cancelled =
            p_cell_id_cancelled;

        for (cell_count = 0; cell_count <
                p_s1ap_gb_ctx->p_kill_data->kill_resp->count;
                cell_count++ )
        {
            /* Check the response of each cell*/
            /* if success, then sends its eCGI in broadcast info list*/
            p_kill_resp =
                &p_s1ap_gb_ctx->p_kill_data->kill_resp->kill_resp_data[cell_count];

            if(RRC_SUCCESS == p_kill_resp->resp)
            {
                rtxDListAllocNodeAndData(p_asn1_ctx,
                        s1ap_CellID_Cancelled_Item,
                        &p_node2,
                        &p_s1ap_CellID_Cancelled_Item);

                if (PNULL == p_node2)
                {
                    /* Not enough memory */
                    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    return RRC_FAILURE;
                }

                /* Initialise the Cell Id item*/
                asn1Init_s1ap_CellID_Cancelled_Item(p_s1ap_CellID_Cancelled_Item);

                p_s1ap_CellID_Cancelled_Item->eCGI.pLMNidentity.numocts
                    = MAX_PLMN_ID_BYTES;

                l3_memcpy_wrapper(p_s1ap_CellID_Cancelled_Item->eCGI.pLMNidentity.data,
                        p_kill_resp->eCGI.
                        plmn_identity.plmn_id, sizeof(p_kill_resp->
                            eCGI.plmn_identity.plmn_id));

                p_s1ap_CellID_Cancelled_Item->eCGI.cell_ID.numbits = 
                    CELL_ID_NUM_BITS;

                /* BUG 10974 start */
                l3_memcpy_wrapper(p_s1ap_CellID_Cancelled_Item->eCGI.cell_ID.data,
                        p_kill_resp->eCGI.cell_identity, 
                        sizeof(p_kill_resp->eCGI.cell_identity));
                /* BUG 10974 stop */

                p_s1ap_CellID_Cancelled_Item->numberOfBroadcasts =
                    p_kill_resp->num_of_broadcasts;
                rtxDListAppendNode(p_cell_id_cancelled, p_node2);
            }
        }
    }
    rtxDListAppendNode(&p_kill_reponse->protocolIEs, p_node);

    if(0 != asn1Append_s1ap_KillResponse_protocolIEs_3(p_asn1_ctx,
                &p_kill_reponse->protocolIEs,
                p_broadcast_cancelled_area_list))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_KillResponse_protocolIEs_3 append "
                "failed.");
        return RRC_FAILURE;
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : enb_mgr_compose_s1ap_kill_resp
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 *                  asn_enc_msg_buf - pointer to ASN encoded msg buffer
 *                  asn_enc_buf_len - pointer to ASN encoded msg length 
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is composing the kill resp.
 ******************************************************************************/
rrc_return_t enb_mgr_compose_s1ap_kill_resp 
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 U8 *asn_enc_msg_buf,
 U16 *asn_enc_buf_len,
 mme_context_t       *p_mme_ctx
 )
{
  rrc_return_et response = RRC_FAILURE;
  OSCTXT asn1_ctx;
  S1AP_PDU s1ap_pdu;

  s1ap_KillResponse *p_kill_reponse = PNULL ;
  s1ap_MessageIdentifier message_identifier;
  s1ap_SerialNumber serial_number;

  s1ap_BroadcastCancelledAreaList broadcast_cancelled_area_list;
  s1ap_CellID_Cancelled cell_id_cancelled ;
  s1ap_TAI_Cancelled tai_cancelled ;
  s1ap_CancelledCellinEAI cancelled_cell_eai ;
  s1ap_CancelledCellinTAI cancelled_cell_tai ;
  s1ap_EmergencyAreaID_Cancelled emer_area_cancelled ;
  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
  RRC_ASSERT(PNULL != asn_enc_msg_buf);
  /* Init ASN1 context */
  if ( 0 != rtInitContext(&asn1_ctx) )
  {
    RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
        __FUNCTION__);
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
  }

  memset_wrapper(&broadcast_cancelled_area_list,
             0,
             sizeof(s1ap_BroadcastCancelledAreaList));

  do
  {
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /* Fill the values in the ASN structures that shall be encoded by
     *  ASN Encoder */
    /* Set Pdu type to successfuloutcome message */
    s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

    s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
        SuccessfulOutcome);
    if (PNULL == s1ap_pdu.u.successfulOutcome)
    {
             RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
      break;
    }
    asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

    s1ap_pdu.u.successfulOutcome->procedureCode =
              ASN1V_s1ap_id_Kill ;

    s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

    /* Set the initiating message type to Write Replace Warning */
    s1ap_pdu.u.successfulOutcome->value.t =
             T1_kill ;

     p_kill_reponse =rtxMemAllocType(&asn1_ctx,s1ap_KillResponse);

     if (PNULL == p_kill_reponse)
     {
          RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
          response = RRC_FAILURE;
          break;
      }

     memset_wrapper (p_kill_reponse, 0, sizeof (s1ap_KillResponse));
     s1ap_pdu.u.successfulOutcome->value.u.kill = p_kill_reponse;

     rtxDListInit(&p_kill_reponse->protocolIEs);
     /****** compose message identifier ********/
     memset_wrapper(&message_identifier,0,sizeof(s1ap_MessageIdentifier));

     message_identifier.numbits = (MSG_ID_OCTET_SIZE*8);

     l3_memcpy_wrapper(message_identifier.data,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->message_identifier,
        sizeof(p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->
            message_identifier));

    if (0 !=
            asn1Append_s1ap_KillResponse_protocolIEs_1(&asn1_ctx,
                &p_kill_reponse->protocolIEs,
                &message_identifier))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_KillResponse_protocolIEs_1 append failed.");
        break;
    }

    /***Serial number****/
    memset_wrapper (&serial_number,0,sizeof(s1ap_SerialNumber));

    serial_number.numbits = SERIAL_NUMBER_OCTET_SIZE*8;

    l3_memcpy_wrapper(serial_number.data,
        p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request->serial_number,
        sizeof(p_s1ap_gb_ctx->p_kill_data->
            s1ap_rrm_kill_request->serial_number));

    if(0 !=
            asn1Append_s1ap_KillResponse_protocolIEs_2(&asn1_ctx,
                &p_kill_reponse->protocolIEs,
                &serial_number))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "s1ap_KillResponse_protocolIEs_2 append "
                "failed.");
        break;
    }


    /****broadcast cancelled area list ******/
    if (RRC_SUCCESS != enb_mgr_fill_broadcast_cancelled_area_list(
                &asn1_ctx,p_s1ap_gb_ctx, p_kill_reponse,
                &broadcast_cancelled_area_list, &cell_id_cancelled,
                &tai_cancelled, &cancelled_cell_eai,
                &cancelled_cell_tai, &emer_area_cancelled))
    {
        response = RRC_FAILURE;
        break;
    }


    rtxDListInit(&p_kill_reponse->extElem1);
    /* ASN Encode message */
    pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
        S1AP_MAX_ASN1_BUF_LEN, TRUE);
    if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
    {
      RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of Kill Resp "
          "failed.");
      break;
    }
    else
    {
      *asn_enc_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
      response = RRC_SUCCESS;
    }
  }while(0);

  /* Free ASN1 context */
  rtFreeContext(&asn1_ctx);

  if (PNULL != p_s1ap_gb_ctx->p_kill_data)
  {
	  p_mme_ctx = p_s1ap_gb_ctx->mme_contexts
		  [ p_s1ap_gb_ctx->p_kill_data->mme_id];
  }

  /* Send the message to MME */
  if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx,
                                asn_enc_msg_buf,
                                *asn_enc_buf_len,
                                PNULL))
  {
      RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d Kill Message "
	      "Send Failed", p_mme_ctx->mme_id);
      RRC_S1AP_UT_TRACE_EXIT();
      return RRC_FAILURE;
  }
  
  RRC_S1AP_UT_TRACE_EXIT();

  return response;
}

/*****************************************************************************
 * Function Name  : enb_mgr_clean_pws_info
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is cleaning the pws_info stored
 *                  in the S1AP Global Context
 ******************************************************************************/
void enb_mgr_clean_pws_info 
(
 s1ap_gb_context_t *p_s1ap_gb_ctx
)
{
    RRC_ASSERT ( PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_ENTER();
    
    if (PNULL != p_s1ap_gb_ctx->p_pws_info)
    {
        if (PNULL != p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request);
            p_s1ap_gb_ctx->p_pws_info->s1ap_rrm_pws_request = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents);
            p_s1ap_gb_ctx->p_pws_info->p_warning_message_contents = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_pws_info->tai_list)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_pws_info->tai_list);
            p_s1ap_gb_ctx->p_pws_info->tai_list = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_pws_info->emergency_list)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_pws_info->emergency_list);
            p_s1ap_gb_ctx->p_pws_info->emergency_list = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_pws_info->pws_resp)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_pws_info->pws_resp);
            p_s1ap_gb_ctx->p_pws_info->pws_resp = PNULL;
        }
        rrc_mem_free(p_s1ap_gb_ctx->p_pws_info);
        p_s1ap_gb_ctx->p_pws_info = PNULL;
    }
    if ( p_s1ap_gb_ctx->pws_timer_id != NULL )
    {
        rrc_stop_timer(p_s1ap_gb_ctx->pws_timer_id);
        p_s1ap_gb_ctx->pws_timer_id = PNULL;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : enb_mgr_clean_kill_data
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is cleaning the kill_data stored
 *                  in the S1AP Global Context
 ******************************************************************************/
void enb_mgr_clean_kill_data 
(
 s1ap_gb_context_t *p_s1ap_gb_ctx
)
{
    RRC_ASSERT ( PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_ENTER();
    
    if (PNULL != p_s1ap_gb_ctx->p_kill_data)
    {
        if (PNULL != p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request);
            p_s1ap_gb_ctx->p_kill_data->s1ap_rrm_kill_request = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_kill_data->tai_list)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_kill_data->tai_list);
            p_s1ap_gb_ctx->p_kill_data->tai_list = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_kill_data->emergency_list)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_kill_data->emergency_list);
            p_s1ap_gb_ctx->p_kill_data->emergency_list = PNULL;
        }
        if (PNULL != p_s1ap_gb_ctx->p_kill_data->kill_resp)
        {
            rrc_mem_free(p_s1ap_gb_ctx->p_kill_data->kill_resp);
            p_s1ap_gb_ctx->p_kill_data->kill_resp = PNULL;
        }
        rrc_mem_free(p_s1ap_gb_ctx->p_kill_data);
        p_s1ap_gb_ctx->p_kill_data = PNULL;
    }
    if ( PNULL != p_s1ap_gb_ctx->kill_timer_id )
    {
        rrc_stop_timer(p_s1ap_gb_ctx->kill_timer_id);
        p_s1ap_gb_ctx->kill_timer_id = PNULL;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : prepare_and_send_wrw_and_pws_cnf
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context   
 * Outputs        : None
 * Returns        : None
 * Description    : This function is preparing successful write-replace warning
 *                  response and PWS CNF to be sent to RRM.
 ******************************************************************************/
void prepare_and_send_wrw_and_pws_cnf
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t       *p_mme_ctx
)
{
   rrc_bool_et pws_flag = RRC_FALSE;
   U8          pws_cell_counter = RRC_NULL;
   U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
   U16 asn_enc_buf_len = RRC_NULL;

   RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
   
   RRC_S1AP_UT_TRACE_ENTER();
   /* prepare tracking area info */
   if (WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST &
         p_s1ap_gb_ctx->p_pws_info->
               s1ap_rrm_pws_request->warning_area_list.bitmask)
   {
       enb_mgr_prepare_tai_info(
                      p_s1ap_gb_ctx);
   }

   /* prepare emergency area info */
   if (WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST &
                  p_s1ap_gb_ctx->p_pws_info->
                  s1ap_rrm_pws_request->warning_area_list.bitmask)
   {
       enb_mgr_prepare_emergency_area_info(
                      p_s1ap_gb_ctx);/*SPR 17777 +-*/
   }
   for (pws_cell_counter = 0; pws_cell_counter <
               p_s1ap_gb_ctx->p_pws_info->pws_resp->count; pws_cell_counter++) 
   {
       if (RRC_SUCCESS == p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].resp)
       {
           pws_flag = RRC_TRUE;
           break;
       }
       else if ((RRC_FAILURE == p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].resp)&&
                (RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].fail_cause ))
       {
           pws_flag = RRC_TRUE;
           break;
       }
   }
   if (pws_flag)
   {
       /* Prepare write-replace warning response as success (containing
        * Broadcast completed area list) to MME*/ 
       /* compose the message which is send to mme as a response */
       if (RRC_FAILURE == enb_mgr_compose_s1ap_write_replace_warning_resp (
                       p_s1ap_gb_ctx,
                       asn_enc_msg_buf,
                       &asn_enc_buf_len, p_mme_ctx))
       {
           RRC_S1AP_TRACE(RRC_ERROR,
                  "%s:enb_mgr_compose_s1ap_write_replace_warning_resp failed.",
                   __FUNCTION__);
           /*builds and sends S1AP_RRM_PWS_CNF message to RRM*/
           if (RRC_FAILURE == 
                   s1ap_rrm_build_and_send_pws_cnf(
                          p_s1ap_gb_ctx, RRC_FAILURE))
           {
               RRC_S1AP_TRACE(RRC_ERROR,
                    "builds and sends S1AP_RRM_PWS_CNF "
                           " message returnd Failure\n");
               return;
           }
           return;
       }
       else
       {
           if(RRM_RRC_DUPLICATE_WARNING_MESSAGE == p_s1ap_gb_ctx->p_pws_info->pws_resp->
                      pws_resp[pws_cell_counter].fail_cause )
           {
               /*builds and sends S1AP_RRM_PWS_CNF message to RRM*/
               if (RRC_FAILURE == s1ap_rrm_build_and_send_pws_cnf(
                           p_s1ap_gb_ctx, RRC_FAILURE))
               {
                   RRC_S1AP_TRACE(RRC_WARNING,
                           "builds and sends S1AP_RRM_PWS_CNF message"
                           " returnd Failure\n");
                   return;
               }
           }
           else
           {
               /*builds and sends S1AP_RRM_PWS_CNF message to RRM*/
               if (RRC_FAILURE == s1ap_rrm_build_and_send_pws_cnf(
                           p_s1ap_gb_ctx, RRC_SUCCESS))
               {
                   RRC_S1AP_TRACE(RRC_WARNING,
                           "builds and sends S1AP_RRM_PWS_CNF message"
                           " returnd Failure\n");
                   return;
               }
           }
       }
   }
   else
   {
      /* Prepare write-replace warning response as failure 
       * (not containing Broadcast completed area list) to MME*/ 
      /* compose the message which is send to mme as a response */
      if (RRC_FAILURE == 
         enb_mgr_build_and_send_s1ap_write_replace_warning_resp_failure (
                          p_s1ap_gb_ctx,
                          p_mme_ctx, 
                          asn_enc_msg_buf,
                          &asn_enc_buf_len))
      {
          RRC_S1AP_TRACE(RRC_ERROR,
              "enb_mgr_compose_s1ap_write_replace_warning_resp_failue"
              " failed.");

      }
      /*builds and sends S1AP_RRM_PWS_CNF(FAILURE ) message to RRM*/
      if (RRC_FAILURE == 
               s1ap_rrm_build_and_send_pws_cnf(
                 p_s1ap_gb_ctx, RRC_FAILURE))
      {
          RRC_S1AP_TRACE(RRC_ERROR,
            "builds and sends S1AP_RRM_PWS_CNF "
                 "message returnd Failure\n");
          return;
      }
   }

   RRC_S1AP_UT_TRACE_EXIT();
   return;
}

/*****************************************************************************
 * Function Name  : prepare_and_send_kill_resp_and_kill_cnf
 * Inputs         : p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - pointer to MME context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is preparing successful Kill response to
 *                  be sent to MME and KILL CNF to be sent to RRM.
 ******************************************************************************/
void prepare_and_send_kill_resp_and_kill_cnf
(
 s1ap_gb_context_t *p_s1ap_gb_ctx,
 mme_context_t       *p_mme_ctx
)
{
   rrc_bool_et kill_flag = RRC_FALSE;
   U8          pws_cell_counter = RRC_NULL;
   U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
   U16 asn_enc_buf_len = RRC_NULL;

   RRC_ASSERT (PNULL != p_s1ap_gb_ctx);
   
   RRC_S1AP_UT_TRACE_ENTER();

   /* prepare tracking area info */
   if (WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST &
               p_s1ap_gb_ctx->p_kill_data->
               s1ap_rrm_kill_request->warning_area_list.bitmask)
   {
       enb_mgr_prepare_kill_tai_info(p_s1ap_gb_ctx);
   }
   /* prepare emergency area info */
   if (WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST &
                  p_s1ap_gb_ctx->p_kill_data->
                    s1ap_rrm_kill_request->warning_area_list.bitmask)
   {
       enb_mgr_prepare_kill_emergency_area_info(p_s1ap_gb_ctx);
   }
   for (pws_cell_counter = 0; pws_cell_counter <
         p_s1ap_gb_ctx->p_kill_data->kill_resp->count; pws_cell_counter++)
   {
       if (RRC_SUCCESS == p_s1ap_gb_ctx->p_kill_data->kill_resp->
                     kill_resp_data[pws_cell_counter].resp)
       {
           kill_flag = RRC_TRUE;
           break;
       }
   }
   if (kill_flag)
   {
       /* Prepare KILL response as success (containing
        * Broadcast cancelled area list) to MME*/
       /* compose the message which is send to mme as a response */
       if (RRC_FAILURE == enb_mgr_compose_s1ap_kill_resp (
                      p_s1ap_gb_ctx,
                      asn_enc_msg_buf,
                       &asn_enc_buf_len,
                      p_mme_ctx))
       {
           RRC_S1AP_TRACE(RRC_ERROR,
                  "%s:enb_mgr_compose_s1ap_kill_resp failed.",
                          __FUNCTION__);
           /*builds and sends S1AP_RRM_WRITE_REPLACE_WARNING_CNF
            * message to RRM*/
           if (RRC_FAILURE ==
                  s1ap_rrm_build_and_send_kill_cnf(
                         p_s1ap_gb_ctx, RRC_FAILURE))
           {
               RRC_S1AP_TRACE(RRC_ERROR,
                     "builds and sends S1AP_RRM_KILL_CNF "
                           "message returnd Failure\n");
                return;
           }
           return;
       }
       else
       {
           /*builds and sends S1AP_RRM_KILL_CNF message to RRM*/
           if (RRC_FAILURE == s1ap_rrm_build_and_send_kill_cnf(
                        p_s1ap_gb_ctx, RRC_SUCCESS))
           {
               RRC_S1AP_TRACE(RRC_WARNING,
                      "builds and sends S1AP_RRM_KILL_CNF message"
                      " returnd Failure\n");
               return;
           }
       }
   }
   else
   {
       if (RRC_FAILURE == enb_mgr_build_and_send_s1ap_kill_resp_failure (
				     p_s1ap_gb_ctx,
                                     p_mme_ctx,
				     asn_enc_msg_buf,
				     &asn_enc_buf_len))
       {
           RRC_S1AP_TRACE(RRC_ERROR,
                "%s:enb_mgr_build_and_send_s1ap_kill_resp_failure failed.",
                 __FUNCTION__);
           return;
       }
       /*builds and sends S1AP_RRM_PWS_CNF (FAILURE) message to RRM*/
       if (RRC_FAILURE == s1ap_rrm_build_and_send_kill_cnf(
                          p_s1ap_gb_ctx, RRC_FAILURE))
       {
           RRC_S1AP_TRACE(RRC_ERROR,
                 "[%s]builds and sends S1AP_RRM_KILL_CNF message"
                 " returnd Failure\n",
                  __FUNCTION__);
           return;
       }
   }
   RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_mgr_fsm_cleanup_handler
 * Inputs         : p_api - Filled Buffer
 *                  p_msg - Filled buffer
 *                  p_s1ap_gb_ctx - S1AP Global Context
 *                  p_mme_ctx - MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function deallocates memory from S1AP Global Context.
 ******************************************************************************/
void enb_mgr_fsm_cleanup_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    

    RRC_S1AP_UT_TRACE_ENTER();

  RRC_S1AP_TRACE(RRC_DETAILEDALL,
          "Unused variable in FSM:p_msg=%p and p_mme_ctx=%p p_api=%p", p_msg,
          p_mme_ctx,p_api);/*SPR 17777 +-*/
    /* Clean pws_info fron Global Context */
    enb_mgr_clean_pws_info (p_s1ap_gb_ctx);

    /* Clean kill_data fron Global Context */
    enb_mgr_clean_kill_data (p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
}


/* SPR 15641 Fix Start */
/*****************************************************************************
 * Function Name  : s1ap_kill_req_dequeue
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function dequeues kill request from the s1ap_kill_queue 
 ******************************************************************************/
void s1ap_kill_req_dequeue (
    s1ap_gb_context_t     *p_s1ap_gb_ctx
)
{
   YLIST                 *p_s1ap_kill_queue = PNULL; 
   YLNODE                *snode = PNULL;
   RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
   RRC_S1AP_UT_TRACE_ENTER();

   p_s1ap_kill_queue = &p_s1ap_gb_ctx->s1ap_kill_queue;

   do
   {
       if (S1AP_KILL_Q_GET_COUNT(p_s1ap_kill_queue))  
       {
           RRC_S1AP_TRACE(RRC_DETAILED,
             " [%s] Dequeuing S1AP KILL QUEUE.",__FUNCTION__);
       }
       else
       {
           break;
       }

       S1AP_KILL_Q_DEQUEUE(p_s1ap_kill_queue, snode);

       if ( PNULL == snode )
       {
           RRC_S1AP_TRACE(RRC_ERROR, "[%s] snode is NULL", __FUNCTION__);
           break;
       }

       enb_mgr_fsm_write_replace_warning_process_event(
		ENB_EV_KILL_REQ,
		((s1ap_kill_queue_node_t *)snode)->message,
		&(((s1ap_kill_queue_node_t *)snode)->asn_msg_len),
		p_s1ap_gb_ctx,
		p_s1ap_gb_ctx->mme_contexts[((s1ap_kill_queue_node_t *)snode)->mme_id]);

       if ( PNULL != snode )
       { 		
           rrc_mem_free(snode);
           snode = PNULL;
       }
	
    } 
    while(0);

    RRC_S1AP_UT_TRACE_EXIT();
}


/* SPR 15641 Fix Stop */
