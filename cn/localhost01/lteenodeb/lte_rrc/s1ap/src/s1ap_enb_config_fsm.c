/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_enb_config_fsm.c,v 1.6 2010/11/26 07:04:23 gur25381 Exp $
 ****************************************************************************
 *
 *  File Description : This file comtains the S1AP ENB Config Update FSM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_mme_fsm.c,v $
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_enb_config_fsm.h"
#include "s1ap_asn_codec.h"
#include "s1ap_oam.h" 
#include "list_wrap.h"
#include "rtxDList.h"
#include "s1ap_mme.h"

 /****************************************************************************
   Private Definitions
  ****************************************************************************/


 /****************************************************************************
   Private Types
  ****************************************************************************/

 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
static U32  convert_asn_time_to_wait(s1ap_TimeToWait_Root asn_time_to_wait);

 /*****************************************************************************
  * Private Constants
  ****************************************************************************/

 /****************************************************************************
  * Exported Variables
  ****************************************************************************/
 /****************************************************************************
  * Private Variables (Must be declared static)
  ****************************************************************************/

const S8* ENB_CONFIG_UPDATE_FSM_STATES_NAMES[] =
{
    (const S8*)"ENB_STATE_RECV_ENB_CONFIG_FROM_OAM",
    (const S8*)"ENB_STATE_W_FOR_ENB_CONFIG_RESP",
    (const S8*)"ENB_STATE_ENB_CONFIG_FAILURE",
    (const S8*)"ENB_STATE_ABNORMAL_ENB_CONFIG",
    (const S8*)"ENB_STATE_ENB_CONFIG_COMPLETE",
    (const S8*)"ENB_STATE_MAX_ENB_CONFIG_VAL"
};

const S8* ENB_CONFIG_UPDATE_FSM_EVENTS_NAMES[] =
{
    (const S8*)"S1AP_MME_EV_ENB_CONFIG_SENT",
    (const S8*)"S1AP_MME_EV_ENB_CONFIG_ACK_RECV",
    (const S8*)"S1AP_MME_EV_TIMER_EXPIRY",
    (const S8*)"S1AP_MME_EV_ENB_CONFIG_NACK",
    (const S8*)"S1AP_MME_EV_ENB_CONFIG_RESEND",
    (const S8*)"S1AP_MME_EV_MAX_RETRIES_OVER",
    (const S8*)"S1AP_OAM_EV_ENB_CONFIG_UPDATE",
    (const S8*)"S1AP_MME_EV_MAX_EVENT_VAL" 
};

/* Handlers for events occuring during Initial state */
static enb_config_update_fsm_ret 
  enb_config_update_fsm[ENB_STATE_MAX_ENB_CONFIG_VAL][ S1AP_MME_EV_MAX_EVENT_VAL] =
{
    /* State ENB_STATE_RECV_ENB_CONFIG_FROM_OAM */
    {
        enb_fsm_config_update_req_handler,      /* S1AP_MME_EV_ENB_CONFIG_SENT,*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_ACK_RECV*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_TIMER_EXPIRY */
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_NACK*/
        enb_fsm_config_update_req_handler,          /* S1AP_MME_EV_ENB_CONFIG_RESEND*/
        enb_fsm_invalid_event_handler, /* S1AP_MME_EV_MAX_RETRIES_OVER*/
        enb_fsm_invalid_event_handler           /* S1AP_OAM_EV_ENB_CONFIG_UPDATE*/ 
    },
    /* State ENB_STATE_W_FOR_ENB_CONFIG_RESP*/
    {
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_SENT,*/
        enb_fsm_config_update_complete_handler, /* S1AP_MME_EV_ENB_CONFIG_ACK_RECV*/
        enb_fsm_config_update_timer_expiry_handler, /* S1AP_MME_EV_TIMER_EXPIRY */
        enb_fsm_config_update_nack_handler,     /* S1AP_MME_EV_ENB_CONFIG_NACK*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_RESEND*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_MAX_RETRIES_OVER*/ 
        enb_fsm_invalid_event_handler           /* S1AP_OAM_EV_ENB_CONFIG_UPDATE*/ 

    },
    /* State ENB_STATE_ENB_CONFIG_FAILURE*/
    {
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_SENT,*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_ACK_RECV*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_TIMER_EXPIRY */
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_NACK*/
        enb_fsm_config_update_time_to_wait_timer_expiry_handler,      /* S1AP_MME_EV_ENB_CONFIG_RESEND*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_MAX_RETRIES_OVER*/ 
        enb_fsm_invalid_event_handler           /* S1AP_OAM_EV_ENB_CONFIG_UPDATE*/ 

    },
    /* State ENB_STATE_ABNORMAL_ENB_CONFIG*/
    {
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_SENT,*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_ACK_RECV*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_TIMER_EXPIRY */
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_NACK*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_RESEND*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_MAX_RETRIES_OVER*/ 
        enb_fsm_invalid_event_handler           /* S1AP_OAM_EV_ENB_CONFIG_UPDATE*/ 

    },
    /* State ENB_STATE_ENB_CONFIG_COMPLETE*/
    {
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_SENT,*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_ACK_RECV*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_TIMER_EXPIRY */
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_NACK*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_ENB_CONFIG_RESEND*/
        enb_fsm_invalid_event_handler,          /* S1AP_MME_EV_MAX_RETRIES_OVER*/ 
        enb_fsm_invalid_event_handler /* S1AP_OAM_EV_ENB_CONFIG_UPDATE*/ 
    }
};

/*****************************************************************************
 * Function Name  : enb_fsm_config_update_process_event
 * Inputs         : p_enb_config_update - OAM Sent filled buffer
 * Outputs        : asn_enc_msg_buf - ASN.1 Buffer to be encoded 
 *                  asn_enc_buf_len - ASN.1 Buffer length 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function compose the ASN.1 eNB Configuraion update 
 *                  message
 ******************************************************************************/
void enb_fsm_config_update_process_event(
        enb_configuration_update_event_et event_id,
        void                *param1,            
        void                *param2,            
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        mme_context_t* p_mme_ctx)
{
    enb_configuration_update_state_et state = ENB_STATE_ENB_CONFIG_INVALID;

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    state = (enb_configuration_update_state_et)ENB_CONFIG_GET_FSM_STATE (p_mme_ctx);

    ENB_CONFIG_FSM_EVENT_TRACE (state, event_id);

    if ((state < ENB_STATE_MAX_ENB_CONFIG_VAL) && 
            (event_id < S1AP_MME_EV_MAX_EVENT_VAL))
    {
        /* Get appropriate event handler */
        (*enb_config_update_fsm[state][event_id])(
                                    param1, 
                                    param2, 
                                    p_s1ap_gb_ctx, 
                                    p_mme_ctx);
   
        RRC_S1AP_TRACE(RRC_BRIEF, "mme_index: %u OldState: %s Event: %s NewState: %s ", 
                    p_mme_ctx->mme_id,
                    ENB_CONFIG_UPDATE_FSM_STATES_NAMES[state], 
                    ENB_CONFIG_UPDATE_FSM_EVENTS_NAMES[event_id],
                    ENB_CONFIG_UPDATE_FSM_STATES_NAMES[p_mme_ctx->current_enb_config_update_state]);
   
    }
    else
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]::Invalid Event : "
                "unexpected state or event!",
                __FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_fsm_config_update_req_handler
 * Inputs         : p_api - pointer to api buffer 
 *                  p_msg - pointer to Filled buffer
 *                  p_s1ap_gb_ctx - pointer to S1AP Global Context
 *                  p_mme_ctx - pointer to MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of eNB configuration udpate FSM and
 *                  build and send the eNB Configuration update message to MME.
 ******************************************************************************/
void enb_fsm_config_update_req_handler(
        void                * p_api,             
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_enc_buf_len = 0;
    s1ap_timer_buffer_t enb_config_update_resp_timer_buf;
    /*SPR 17777 +-*/

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_msg); 

    RRC_S1AP_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_api=%p p_msg=%p", p_api,p_msg);/*SPR 17777+-*/
    memset_wrapper(asn_enc_msg_buf, 0, sizeof(asn_enc_msg_buf));
    memset_wrapper(&enb_config_update_resp_timer_buf, 0,
            sizeof(s1ap_timer_buffer_t));
        /* If this method is invoked from Timer Expiry,
         * we have to pick the old configuration else if 
         * called from Failure, we can send the updated 
         * configuration */
        if (RRC_FAILURE == 
                compose_asn_enb_config_update(p_mme_ctx->p_enb_configuration_update,
                    /*SPR 17777 +-*/
                    asn_enc_msg_buf,
                    &asn_enc_buf_len))
            /*SPR 17777 +-*/
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN.1 Compose ENB Config update Failed");

            /* Sending failure message to OAM */
            build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
                OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                /*SPR 17777 +-*/
                p_mme_ctx->oam_transaction_id);

            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        /* Send the message to MME */
        if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx, 
                    asn_enc_msg_buf,
                    asn_enc_buf_len,
                    PNULL))
        {
            RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d ENB Configuration Update Message "
                    "Send Failed", p_mme_ctx->mme_id);

            /* Sending failure message to OAM */
            build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
                OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                /*SPR 17777 +-*/
                p_mme_ctx->oam_transaction_id);

            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        /* Set the state to ENB_STATE_W_FOR_ENB_CONFIG_RESP */
        ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_W_FOR_ENB_CONFIG_RESP);

        RRC_S1AP_TRACE(RRC_INFO,"ENB Configuration update Sent to MME %d", 
                p_mme_ctx->mme_id);
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [ENB_STATE_W_FOR_ENB_CONFIG_RESP]",
                __FUNCTION__);

        /* Start the Guard Timer */
        enb_config_update_resp_timer_buf.timer_type = 
            S1AP_ENB_CONFIGURAION_UPDATE_TIMER;
        enb_config_update_resp_timer_buf.auto_delete = RRC_TRUE;
        enb_config_update_resp_timer_buf.mme_id = p_mme_ctx->mme_id;

        /* Start the guard timer */
        p_mme_ctx->enb_config_update_timer_id=  rrc_start_timer(
                p_s1ap_gb_ctx->s1_config_resp_duration ,
                &enb_config_update_resp_timer_buf,
                sizeof(enb_config_update_resp_timer_buf),
                RRC_FALSE);

        p_mme_ctx->s1_config_retry_count++;

    RRC_S1AP_UT_TRACE_EXIT();

    return;
}

/*****************************************************************************
 * Function Name  : enb_fsm_config_update_timer_expiry_handler
 * Inputs         : p_api -  pointer to  api Buffer
 *                  p_msg - pointer to Filled buffer
 *                  p_s1ap_gb_ctx - pointer to S1AP Global Context
 *                  p_mme_ctx - pointer to MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of eNB configuration udpate FSM and
 *                  build and send the eNB Configuration update timer expiry 
 *                  message to MME.
 ******************************************************************************/
void enb_fsm_config_update_timer_expiry_handler (
        void             *p_api,
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_api=%p p_msg=%p p_s1ap_gb_ctx=%p", p_api,
            p_msg,p_s1ap_gb_ctx);/*SPR 17777 +-*/

    /* SPR 23795/SES-343 Fix Start */
    /* Timer expired and is not valid any more */
    p_mme_ctx->enb_config_update_timer_id = PNULL;
    /* SPR 23795/SES-343 Fix End */

    /* Timer expired, set the state to ENB_STATE_RECV_ENB_CONFIG_FROM_OAM
     * and re-send the message
     */
    ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_RECV_ENB_CONFIG_FROM_OAM);

    /* Sending Success message to OAM */
    build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
            OAM_ENB_CONFIG_UPDATE_FAILURE_TIMER_EXPIRY,
            /*SPR 17777 +-*/
            p_mme_ctx->oam_transaction_id);

    RRC_S1AP_UT_TRACE_EXIT();

    return;
}

/*****************************************************************************
 * Function Name  : enb_fsm_invalid_event_handler 
 * Inputs         : p_api - pointer to api data 
 *                  p_msg - pointer to Filled buffer
 *                  p_s1ap_gb_ctx - pointer to S1AP Global Context
 *                  p_mme_ctx - pointer to MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of eNB configuration udpate FSM and
 *                  build and send the invalid event message to MME.
 ******************************************************************************/
void enb_fsm_invalid_event_handler(
        void                * p_api,                     
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED, "Unused variable in fsm p_api=%p p_msg=%p"
            " p_s1ap_gb_ctx=%p", p_api, p_msg, p_s1ap_gb_ctx);
    
    RRC_S1AP_TRACE(RRC_WARNING,"[%s]::Invalid Event, state=%s",
            __FUNCTION__,
            ENB_CONFIG_UPDATE_FSM_STATES_NAMES[(ENB_CONFIG_GET_FSM_STATE(p_mme_ctx))]);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : enb_fsm_config_update_complete_handler
 * Inputs         : p_api - pointer to api Buffer
 *                  p_msg - pointer to Filled buffer
 *                  p_s1ap_gb_ctx - pointer ro S1AP Global Context
 *                  p_mme_ctx - pointer to MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of eNB configuration udpate FSM and
 *                  build and send the eNB Configuration update complete 
 *                  message to MME.
 ******************************************************************************/
void enb_fsm_config_update_complete_handler(
        void                * p_api,                     
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_api=%p and p_msg=%p p_s1ap_gb_ctx=%p",
            p_api, p_msg,p_s1ap_gb_ctx);/*SPR 17777 +-*/
    /* We have received the ACK, now stop the Guard Timer
     * and change the state to get ready to received next update.
     */
    /*Stop enb_config_update_timer_id guard timer*/
    if (PNULL != p_mme_ctx->enb_config_update_timer_id)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Stop timer "
                "[S1AP_ENB_CONFIGURAION_UPDATE_TIMER]",
                __FUNCTION__);
        rrc_stop_timer(p_mme_ctx->enb_config_update_timer_id);
        p_mme_ctx->enb_config_update_timer_id = PNULL;
    }

    /* Setting the initital state to receive the message again */
    ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_RECV_ENB_CONFIG_FROM_OAM);

    /* Sending Success message to OAM */
    build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
            OAM_ENB_CONFIG_UPDATE_SUCCESS,
            /*SPR 17777 +-*/
            p_mme_ctx->oam_transaction_id);

    /* Preventing double free */
    if (PNULL != p_mme_ctx->p_enb_configuration_update)
    {
        /* Free the
         * memory
         * allocated */
        rrc_mem_free(p_mme_ctx->p_enb_configuration_update);
        p_mme_ctx->p_enb_configuration_update = PNULL;
    }
    /* Transaction complete */
    p_mme_ctx->oam_transaction_id = RRC_NULL;




    RRC_S1AP_UT_TRACE_EXIT();
    return;
}


/*****************************************************************************
 * Function Name  : enb_fsm_config_update_time_to_wait_timer_expiry_handler 
 * Inputs         : p_api - pointer to api buffer
 *                  p_msg - pointer to the filled buffer
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the waiting time time expiry 
 *                  handler and sends this message to MME.
 ********************************************************************************/
void enb_fsm_config_update_time_to_wait_timer_expiry_handler(
        void                * p_api,                     
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    U8 asn_enc_msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_enc_buf_len = 0;
    s1ap_timer_buffer_t enb_config_update_resp_timer_buf;

    RRC_S1AP_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p p_msg=%p", p_api, p_msg);
    /* SPR 23795/SES-343 Fix Start */
    /* Timer expired and is not valid any more */
    p_mme_ctx->enb_config_update_timer_id = PNULL;
    /* SPR 23795/SES-343 Fix End */

    if (RRC_FAILURE == 
            compose_asn_enb_config_update(p_mme_ctx->p_enb_configuration_update,
                /*SPR 17777 +-*/
                asn_enc_msg_buf,
                &asn_enc_buf_len))
        /*SPR 17777 +-*/
    {
        RRC_S1AP_TRACE(RRC_ERROR, "ASN.1 Compose ENB Config update Failed");

        /* Sending failure message to OAM */
        build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
                OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                /*SPR 17777 +-*/
                p_mme_ctx->oam_transaction_id);

        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send the message to MME */
    if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx, 
                asn_enc_msg_buf,
                asn_enc_buf_len,
                PNULL))
    {
        RRC_S1AP_TRACE(RRC_ERROR,"MME_ID %d ENB Configuration Update Message "
                "Send Failed", p_mme_ctx->mme_id);

        /* Sending failure message to OAM */
        build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
                OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
                /*SPR 17777 +-*/
                p_mme_ctx->oam_transaction_id);

        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    RRC_S1AP_TRACE(RRC_INFO,"ENB Configuration update Sent to MME %d", 
            p_mme_ctx->mme_id);
    RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [ENB_STATE_W_FOR_ENB_CONFIG_RESP]",
            __FUNCTION__);

    /* Start the Guard Timer */
    enb_config_update_resp_timer_buf.timer_type = 
        S1AP_ENB_CONFIGURAION_UPDATE_TIMER;
    enb_config_update_resp_timer_buf.auto_delete = RRC_TRUE;
    enb_config_update_resp_timer_buf.mme_id = p_mme_ctx->mme_id;

    /* Start the guard timer */
    p_mme_ctx->enb_config_update_timer_id=  rrc_start_timer(
            p_s1ap_gb_ctx->s1_config_resp_duration ,
            &enb_config_update_resp_timer_buf,
            sizeof(enb_config_update_resp_timer_buf),
            RRC_FALSE);

    ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_W_FOR_ENB_CONFIG_RESP);

   }

/*****************************************************************************
 * Function Name  : enb_fsm_config_update_nack_handler
 * Inputs         : p_api - pointer to api Buffer
 *                  p_msg - pointer to Filled buffer
 *                  p_s1ap_gb_ctx - pointer to S1AP Global Context
 *                  p_mme_ctx - pointer to MME Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is part of eNB configuration udpate FSM and
 *                  build and send the eNB Configuration update message for 
 *                  nack to MME.
 ******************************************************************************/
void enb_fsm_config_update_nack_handler(
        void                * p_api,                     
        void                  * p_msg,
        s1ap_gb_context_t     * p_s1ap_gb_ctx,
        mme_context_t         * p_mme_ctx)
{
    U16 *asn_msg_len = (U16 *)p_msg;
    s1ap_timer_buffer_t enb_config_update_resp_timer_buf;
    U32 time_to_wait = 0;
    enb_config_update_failure_t enb_config_update_failure;


    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);
    memset_wrapper(&enb_config_update_failure, 0, 
            sizeof(enb_config_update_failure_t));

    memset_wrapper(&enb_config_update_resp_timer_buf, 0,
            sizeof(s1ap_timer_buffer_t));

    /* Parse the message */
    if (RRC_FAILURE == parse_asn_enb_config_update_failure(
                p_mme_ctx,
                p_api,
                *asn_msg_len,
                &enb_config_update_failure))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                "parse_asn_enb_config_update_failure returned failure",__FUNCTION__);

        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }


    /*Stop enb_config_update_timer_id guard timer*/
    if (PNULL != p_mme_ctx->enb_config_update_timer_id)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Stop timer "
                "[S1AP_ENB_CONFIGURAION_UPDATE_TIMER]",
                __FUNCTION__);
        rrc_stop_timer(p_mme_ctx->enb_config_update_timer_id);
        p_mme_ctx->enb_config_update_timer_id = PNULL;
    }


    if (S1AP_TIME_TO_WAIT_PRESENT & enb_config_update_failure.bitmask)
    {
        time_to_wait = convert_asn_time_to_wait(enb_config_update_failure.time_to_wait);

    }



    if ((RRC_NULL != time_to_wait) &&
            (p_mme_ctx->enb_config_updated_count <
            p_s1ap_gb_ctx->max_retry_for_s1_config))
    {
        RRC_S1AP_TRACE(RRC_INFO,"ENB Configuration update Failure Time to wait");
        
        RRC_S1AP_TRACE(RRC_INFO,"ENB Configuration update Retry Counter = %d",
                       p_mme_ctx->enb_config_updated_count);

        p_mme_ctx->enb_config_updated_count++;

        /* Start the Guard Timer */
        enb_config_update_resp_timer_buf.timer_type =
            S1AP_ENB_CONFIGURATION_TIME_TO_WAIT_TIMER;
        enb_config_update_resp_timer_buf.auto_delete = RRC_TRUE;
        enb_config_update_resp_timer_buf.mme_id = p_mme_ctx->mme_id;

        /* Start the
         * guard
         * timer */
        p_mme_ctx->enb_config_update_timer_id=  rrc_start_timer(
                time_to_wait,
                &enb_config_update_resp_timer_buf,
                sizeof(enb_config_update_resp_timer_buf),
                RRC_FALSE);

        /* Set the state to ENB_STATE_ENB_CONFIG_FAILURE*/
        ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_ENB_CONFIG_FAILURE);

        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [ENB_STATE_ENB_CONFIG_FAILURE]",
                __FUNCTION__);
        return;

    }
    else
    {
        /* SES-454 Fix Start */
        RRC_S1AP_TRACE(RRC_INFO,"ENB Configuration update Failure Received"
            "Sending failure response to OAM");
        /* SES-454 Fix End */
    }

    /* Sending Success message to OAM */
    build_and_send_enb_config_update_resp_to_oam((U8)(p_mme_ctx->mme_id),
            OAM_ENB_CONFIG_UPDATE_FAILURE,
            /*SPR 17777 +-*/
            p_mme_ctx->oam_transaction_id);

    /* Setting the initital state to receive the message again */
    ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_RECV_ENB_CONFIG_FROM_OAM);

    p_mme_ctx->enb_config_updated_count = 0; /* Reset Counter */
    /* SES-454 Fix Start */
    /* Preventing double free */
    if (PNULL != p_mme_ctx->p_enb_configuration_update)
    {
        /* Free the
         * memory
         * allocated */
        rrc_mem_free(p_mme_ctx->p_enb_configuration_update);
        p_mme_ctx->p_enb_configuration_update = PNULL;
    }
    /* Transaction complete */
    p_mme_ctx->oam_transaction_id = RRC_NULL;
    /* SES-454 Fix End */

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
* Function Name  : convert_asn_time_to_wait
* Inputs         : asn_time_to_wait - timer to s1ap_TimeToWait_Root
* Outputs        : time_to_wait
* Returns        : U32
* Description    : This function convert the enumarated value to absolute value.
******************************************************************************/
U32  convert_asn_time_to_wait(s1ap_TimeToWait_Root asn_time_to_wait)
{
    U32 time_to_wait = 0;
    switch (asn_time_to_wait)
    {
        case s1ap_v1s:
            {
                time_to_wait = 1000;
                break;
            }
        case s1ap_v2s:
            {
                time_to_wait = 2000;
                break;
            }
        case s1ap_v5s:
            {
                time_to_wait = 5000;
                break;
            }
        case s1ap_v10s:
            {
                time_to_wait = 10000;
                break;
            }
        case s1ap_v20s:
            {
                time_to_wait = 20000;
                break;
            }
        case s1ap_v60s:
            {
                time_to_wait = 60000;
                break;
            }
        default:
            {
                RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Time To Wait Sent in"
                        " eNB Config Update Failure, resetting it to 0s", asn_time_to_wait);
            }
    }

    return time_to_wait;
}
