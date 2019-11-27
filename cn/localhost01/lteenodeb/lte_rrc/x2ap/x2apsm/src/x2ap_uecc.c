/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: x2ap_uecc.c,v 1.5 2011/09/23 12:13:25 gur22047 Hari Oum Exp $
 ****************************************************************************
 *
 *  File Description : This is the source file of the X2AP UECC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: x2ap_uecc.c,v $
 *
 *
****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "x2ap_uecc.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "x2ap_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_ue_asso_sig_sm.h"
#include "rrc_common_utils.h"
#include "rrc_x2apCommon_il_composer.h"
/*RLF_IND_fix_start*/
#include "rrc_x2apRrm_il_composer.h"
/*RLF_IND_fix_start*/



/*****************************************************************************
   * DATA
    *      DESCRIPTION: This is used for the logging of X2AP UE Associated 
     *                   Signalling States.
      ****************************************************************************/
U8 *X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[] =
{
      (U8 *)"X2AP_UE_ASSO_SIG_IDLE_ST",
      (U8 *)"X2AP_UE_ASSO_SIG_ACTIVE_ST",
      (U8 *)"X2AP_UE_ASSO_SIG_MAX_STATE"
};

/******************************************************************************
   * DATA DESCRIPTION : This enum is used for the logging of X2AP UE Associated 
    *                    Signalling Events.
     ******************************************************************************/
U8 *X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[] =
{
     (U8 *)"X2AP_UE_ASSO_SIG_X2_LINK_UP_EV",
     (U8 *)"X2AP_UE_ASSO_SIG_MSG_IND_EV",
     (U8 *)"X2AP_UE_ASSO_SIG_MSG_REQ_EV",
     (U8 *)"X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV",
     (U8 *)"X2AP_UE_ASSO_SIG_MAX_EVENT"
};


/****************************************************************************
  Private Definitions
 ****************************************************************************/


/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
x2ap_peer_enb_context_t * x2ap_get_peer_enb_ctxt_from_ecgi(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req
        );

/* SPR_14436_Fix Start */
U8 x2ap_allocate_stream_id (
	x2ap_gb_context_t 	*p_x2ap_gb_ctx,
    /*SPR 17777 +-*/
 	x2ap_peer_enb_context_t *p_peer_enb_ctx
);
/* SPR_14436_Fix Stop */

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern x2ap_return_et x2ap_build_and_send_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig);

extern x2ap_return_et x2ap_enb_build_and_send_handover_report(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ho_report_info_t  *p_x2ap_ho_report);

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/


/****************************************************************************
 * Functions implementation
 ****************************************************************************/
/******************************************************************************
*   FUNCTION NAME: x2ap_uecc_process_ue_assoc_sig_msg_req
*
*   DESCRIPTION:
*       Function processes x2ap ue_associated_sig_msg_req message
*
*   RETURNS:
*       None
*
******************************************************************************/
static void x2ap_uecc_process_ue_assoc_sig_msg_req(void  *p_api,
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    x2ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req = PNULL;
    U8 *p_message   = PNULL;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    U32 message_buff_len= 0;
    x2ap_peer_enb_context_t *p_peer_enb_ctx = PNULL;
    /* CID: 113476 Start */
    U16 ue_id       = RRC_NULL;
    /* CID: 113476 End */
    ue_x2ap_context_t  *p_ue_x2ap_ctx      = PNULL;
    U8   current_state ;
    /* SPR_14436_Fix Start */
    x2ap_ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    /* SPR_14436_Fix Stop */

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_api != PNULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != PNULL);

    X2AP_MEMSET(&peer_enb_msg, X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    p_ue_associated_sig_req = (x2ap_ue_associated_sig_msg_req_t *)
        ((U8*)p_api + CV_HDRSIZE);

    SET_UE_INDEX(p_ue_associated_sig_req->ue_id);

    p_message = &p_ue_associated_sig_req->message_buff_p[0] ;
    message_buff_len = p_ue_associated_sig_req->message_buff_size;

    /*store message and message length*/
    peer_enb_msg.p_msg = p_message; 
    peer_enb_msg.msg_len = message_buff_len;

    /*Get peer eNB context*/
    p_peer_enb_ctx = &(p_x2ap_gb_ctx->
            peer_enb_cntxt_arr[p_ue_associated_sig_req->peer_enodeb_id]);

    if(PNULL == p_peer_enb_ctx)
    {
        X2AP_TRACE(X2AP_ERROR,"Invalid peer enb ctx for received "
                "peer enodeb id : %d", 
                p_ue_associated_sig_req->peer_enodeb_id);
        return;
    }

    /* Get ue index */
    ue_id = p_ue_associated_sig_req->ue_id;

    if (ue_id >= MAX_NUM_UE)
    {
        X2AP_TRACE(X2AP_ERROR,"Error ::"
                "UE id [%d[ is invalid \n",ue_id);
        return;
    }
    else
    {
        X2AP_TRACE (X2AP_INFO, "Received UE Id :%d",ue_id);
        /*Get UE context for peer EnodeB */
        /* CSR_00040799 Fix Start */
        if ( X2AP_P_NULL == p_peer_enb_ctx->ue_x2ap_contexts)
        {
            return;
        }
        /* CSR_00040799 Fix End */
        p_ue_x2ap_ctx = p_peer_enb_ctx->ue_x2ap_contexts[ue_id];
        /* SPR_14436_Fix Start */
        if (PNULL != p_ue_x2ap_ctx)
        {
            peer_enb_msg.stream_id = p_ue_x2ap_ctx->stream_id;
        }
        /* SPR_14436_Fix Stop */
    }


    switch (p_ue_associated_sig_req->message_type)
    {
        case X2AP_INITIATING_MESSAGE:
        {
            switch (p_ue_associated_sig_req->procedure_code)
            {
                case ASN1V_x2ap_id_handoverPreparation:
                {
                    X2AP_TRACE(X2AP_INFO,
                            "Handover Request(UECC->X2AP)");

                    if(PNULL != p_ue_x2ap_ctx )
                    {
                        X2AP_TRACE(X2AP_ERROR,
                                "UE Context already present at"
                                "X2AP for UE Id:%d", ue_id);
                        return;
                    }
                    else
                    {
                        /* Allocate Memory for UE context */
                        p_ue_x2ap_ctx = (ue_x2ap_context_t *)
                            x2ap_mem_get(sizeof(ue_x2ap_context_t));

                        if (PNULL == p_ue_x2ap_ctx)
                        {
                            X2AP_TRACE(X2AP_ERROR,
                                    "[%s]:ue_x2ap_context_t: unable to"
                                    " Allocate Memory for UE x2ap global "
                                    "context!!\n", __FUNCTION__);
                            return;
                        }
                        else
                        {
                            X2AP_TRACE (X2AP_INFO, "Allocating"
                                    " UE Context at X2AP for UE"
                                    " Id:%d", ue_id);
                        }
                    }

                    X2AP_MEMSET((void*) p_ue_x2ap_ctx, 0,
                            sizeof(ue_x2ap_context_t));

                    /*Store UE Context in the X2AP Global ctx*/
                    p_peer_enb_ctx->ue_x2ap_contexts[ue_id] = 
                        p_ue_x2ap_ctx;

                    /*Store old-enb-ue-x2ap-id in ue context*/
                    if(p_ue_associated_sig_req->bitmask & 
                            X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT)
                    {
                        p_ue_x2ap_ctx->old_enb_ue_x2ap_id = 
                            p_ue_associated_sig_req->old_enb_ue_x2ap_id;

                        X2AP_TRACE (X2AP_INFO, "Received value of Old eNB-UE-X2AP-ID is [%d]",
                                p_ue_associated_sig_req->old_enb_ue_x2ap_id);
                    }
                    /* SPR_14436_Fix Start */
                    if(p_ue_associated_sig_req->bitmask & 
                            X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT)
                    {
                        p_ue_x2ap_ctx->stream_id = x2ap_allocate_stream_id(p_x2ap_gb_ctx, 
                                /*SPR 17777 +-*/
                                p_peer_enb_ctx); 
                    }
                    peer_enb_msg.stream_id = p_ue_x2ap_ctx->stream_id;
                    /* SPR_14436_Fix Stop */

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);   

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;

                }
                /* X2AP Fix_AG_4 : Start*/
                case ASN1V_x2ap_id_handoverCancel:
                case ASN1V_x2ap_id_uEContextRelease:
                /* X2AP Fix_AG_4 : End*/
                {
                    /* Release UE Context at X2AP */
                    if(PNULL != p_ue_x2ap_ctx )
                    {
                        x2ap_mem_free(p_ue_x2ap_ctx);
                        p_peer_enb_ctx->ue_x2ap_contexts[ue_id]
                            = PNULL;
                        X2AP_TRACE (X2AP_INFO, "Releasing UE Context at X2AP"
                                " for UE Id:%d", ue_id);
                    }

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                }
                case ASN1V_x2ap_id_snStatusTransfer:
                {
                    X2AP_TRACE(X2AP_INFO,
                            "snStatus Transfer(UECC->X2AP)");
                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);  

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                } 
                case ASN1V_x2ap_id_errorIndication:
                {
                    X2AP_TRACE(X2AP_INFO,
                            "Error Indication (UECC->X2AP)");
                            
                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);   

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                } 
#ifdef ENDC_ENABLED
                /*NR_DC Code Change Start*/
                case ASN1V_x2ap_id_sgNBAdditionPreparation:
                {
                    X2AP_TRACE(X2AP_INFO, "SgNB Addition Required"
                            "(UECC->X2AP)");

                    if(PNULL == p_ue_x2ap_ctx )
                    {
                        /* Allocate Memory for UE context */
                        p_ue_x2ap_ctx = (ue_x2ap_context_t *)
                            x2ap_mem_get(sizeof(ue_x2ap_context_t));

                        if (PNULL == p_ue_x2ap_ctx)
                        {
                            X2AP_TRACE(X2AP_ERROR,
                                    "[%s]:ue_x2ap_context_t: unable to"
                                    " Allocate Memory for UE x2ap global "
                                    "context!!\n", __FUNCTION__);
                            return;
                        }
                        else
                        {
                            X2AP_TRACE (X2AP_INFO, "Allocating"
                                    " UE Context at X2AP for UE"
                                    " Id:%d", ue_id);
                        }
                        X2AP_MEMSET((void*) p_ue_x2ap_ctx, 0,
                                sizeof(ue_x2ap_context_t));
                    }

                    /*Store UE Context in the X2AP Global ctx*/
                    p_peer_enb_ctx->ue_x2ap_contexts[ue_id] = 
                        p_ue_x2ap_ctx;

                    /*Store menb_ue_x2ap_id in ue context*/
                    p_ue_x2ap_ctx->menb_ue_x2ap_id = 
                        p_ue_associated_sig_req->menb_ue_x2ap_id;

                    X2AP_TRACE (X2AP_INFO, "Received value of MeNB-UE-X2AP-ID is [%d]",
                            p_ue_associated_sig_req->menb_ue_x2ap_id);

                    /*
                     * Since Each UE related Messages are send at separate Stream
                     * and for each UE, MeNB-UE-X2AP-ID is uniquely generated,
                     * so allocating new stream for initiating case.
                     */
                    p_ue_x2ap_ctx->stream_id = x2ap_allocate_stream_id(p_x2ap_gb_ctx, 
                            p_peer_enb_ctx); 

                    /*
                     * Storing allocated stream id into peer_enb_msg which will be
                     * used for sending the message at the end.
                     */
                    peer_enb_msg.stream_id = p_ue_x2ap_ctx->stream_id;

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state;

                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s"
                            " Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, 
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->
                            x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);
                    break;
                }

                case ASN1V_x2ap_id_sgNBReconfigurationCompletion:
                {
                    X2AP_TRACE(X2AP_INFO, "SgNB Reconfiguration Complete"
                            "(UECC->X2AP)");

                    /* Getting UE X2AP CTX using MeNB_UE_X2AP_ID */
                    if(p_ue_associated_sig_req->bitmask & 
                            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT)
                    {
                        /*
                         * Storing allocated stream id into peer_enb_msg which will be
                         * used for sending the message at the end.
                         */
                        /* coverity fix 88186 */
                        if (PNULL != p_ue_x2ap_ctx)
                        {
                            peer_enb_msg.stream_id = p_ue_x2ap_ctx->stream_id;
                        }
                        /* coverity fix 88186 */

                        current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.
                            x2ap_ue_asso_sig_curr_state;

                        /*Invoke UE Associated sig msg FSM*/
                        x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                                (U8 *)(&peer_enb_msg),
                                p_x2ap_gb_ctx,
                                p_peer_enb_ctx, X2AP_NULL);

                        X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s"
                                " Event %s NewState %s ",
                                p_ue_associated_sig_req->ue_id, 
                                X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                                X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                                X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->
                                x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    }  
                    break;
                }
                /*NR_DC Code Change Stop*/
                /*EN-DC_changes_start*/
                case ASN1V_x2ap_id_meNBinitiatedSgNBRelease:
                {
                    X2AP_TRACE(X2AP_INFO, "SgNB Release Request"
                            "(UECC->X2AP)");

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state;

                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s"
                            " Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, 
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->
                            x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                }
                /*EN-DC_changes_end*/ 
#endif
                default:
                {
                    X2AP_TRACE(X2AP_ERROR," Unwanted  "
                            "Procedure Received");
                    return;
                }
            }
            return;
        }  

        case X2AP_SUCCESSFUL_OUTCOME:
        {
            switch (p_ue_associated_sig_req->procedure_code)
            { 
                case ASN1V_x2ap_id_handoverPreparation:
                {
                    X2AP_TRACE(X2AP_INFO,
                            "Handover Request Ack (UECC->X2AP)");
                    /* X2AP Fix_AG_4 : Start*/
                    if(PNULL != p_ue_x2ap_ctx )
                    {
                        X2AP_TRACE(X2AP_ERROR,
                                "UE Context already present at"
                                "X2AP for UE Id:%d", ue_id);
                        return;
                    }
                    else
                    {
                        /* Allocate Memory for UE context */
                        p_ue_x2ap_ctx = (ue_x2ap_context_t *)
                            x2ap_mem_get(sizeof(ue_x2ap_context_t));

                        if (PNULL == p_ue_x2ap_ctx)
                        {
                            X2AP_TRACE(X2AP_ERROR,
                                    "[%s]: unable to"
                                    " Allocate Memory for UE x2ap global "
                                    "context!!\n", __FUNCTION__);
                            return;
                        }
                        else
                        {
                            X2AP_TRACE (X2AP_INFO, "Allocating"
                                    " UE Context at X2AP for UE"
                                    " Id:%d", ue_id);
                        }
                    }

                    X2AP_MEMSET((void*) p_ue_x2ap_ctx, 0,
                            sizeof(ue_x2ap_context_t));

                    p_ue_x2ap_ctx->cell_index =  p_ue_associated_sig_req->cell_index;

                    /*Store UE Context in the X2AP Global ctx*/
                    p_peer_enb_ctx->ue_x2ap_contexts[ue_id] = 
                        p_ue_x2ap_ctx;

                    /*Store old-enb-ue-x2ap-id in ue context*/
                    if(p_ue_associated_sig_req->bitmask & 
                            X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT)
                    {
                        p_ue_x2ap_ctx->old_enb_ue_x2ap_id = 
                            p_ue_associated_sig_req->old_enb_ue_x2ap_id;

                        X2AP_TRACE (X2AP_INFO, 
                                "Received value of Old eNB UE X2AP ID is [%d]",
                                p_ue_associated_sig_req->old_enb_ue_x2ap_id);
                    }
                    /* X2AP Fix_AG_4 : End*/

                    /* SPR_14436_Fix Start */
                    p_ho_ongoing_ue_ctx_node = (x2ap_ho_ongoing_ue_ctx_node_t *)
                        list_first_node(&p_x2ap_gb_ctx->ho_ongoing_ue_ctx_list);

                    while(p_ho_ongoing_ue_ctx_node)
                    {    
                        if ((p_ue_associated_sig_req->bitmask &
                                    X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT ) && 
                                (p_ue_associated_sig_req->old_enb_ue_x2ap_id ==
                                 p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.old_enb_ue_x2ap_id))
                        {    
                            /* Match found!! */
                            X2AP_TRACE(X2AP_INFO,"[%s] HO ONGOING UE CTX NODE "
                                    "Match found\n",__FUNCTION__);
                            /*Delete ho_ongoing_ue_ctx from the
                             * ho_ongoing_ue_ctx_list */
                            p_ue_x2ap_ctx->stream_id = p_ho_ongoing_ue_ctx_node->
                                ho_ongoing_ue_ctx.stream_id;
                            peer_enb_msg.stream_id = p_ue_x2ap_ctx->stream_id; 

                            list_delete_node(&p_x2ap_gb_ctx->ho_ongoing_ue_ctx_list,
                                    (void *)p_ho_ongoing_ue_ctx_node);

                            if (PNULL != p_ho_ongoing_ue_ctx_node)
                            {
                                rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                                p_ho_ongoing_ue_ctx_node = PNULL;
                            }
                            break;
                        }
                        p_ho_ongoing_ue_ctx_node = (x2ap_ho_ongoing_ue_ctx_node_t *)
                            list_next_node(&p_ho_ongoing_ue_ctx_node->node);

                    }
                    /* SPR_14436_Fix Stop */	

                    /* Store the value of new eNB UE X2AP ID received in UECC context */
                    if(p_ue_associated_sig_req->bitmask &
                            X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT)
                    {
                        p_ue_x2ap_ctx->new_enb_ue_x2ap_id = 
                            p_ue_associated_sig_req->new_enb_ue_x2ap_id;

                        X2AP_TRACE (X2AP_INFO, 
                                "Received value of New eNB UE X2AP ID is [%d]",
                                p_ue_associated_sig_req->new_enb_ue_x2ap_id);
                    }

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);   

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                }
                #ifdef ENDC_ENABLED
                /*EN-DC_changes_start*/
                case ASN1V_x2ap_id_sgNBinitiatedSgNBRelease:
                {
                    X2AP_TRACE(X2AP_INFO, "SgNB Release Confirm"
                            "(UECC->X2AP)");

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state;

                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s"
                            " Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, 
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->
                            x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                }
                #endif

                default:
                {
                    X2AP_TRACE(X2AP_ERROR," Unknown Procedure");
                    return;
                }
            }
            return;
        }      
        case X2AP_UNSUCCESSFUL_OUTCOME:
        {
            switch (p_ue_associated_sig_req->procedure_code)
            { 
                case ASN1V_x2ap_id_handoverPreparation:
                {
                    X2AP_TRACE(X2AP_INFO,
                            "Handover Request Ack (UECC->X2AP)");

                    /* SPR_14436_Fix Start */
                    p_ho_ongoing_ue_ctx_node = (x2ap_ho_ongoing_ue_ctx_node_t *)
                        list_first_node(&p_x2ap_gb_ctx->ho_ongoing_ue_ctx_list);

                    while(p_ho_ongoing_ue_ctx_node)
                    {    
                        if ((p_ue_associated_sig_req->bitmask &
                                    X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT ) && 
                                (p_ue_associated_sig_req->old_enb_ue_x2ap_id ==
                                 p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.old_enb_ue_x2ap_id))
                        {    
                            /* Match found!! */
                            X2AP_TRACE(X2AP_INFO,"[%s] HO ONGOING UE CTX NODE "
                                    "Match found\n",__FUNCTION__);
                            /*Delete ho_ongoing_ue_ctx from the
                             * ho_ongoing_ue_ctx_list */
                            /*SPR_16713_START*/
                            peer_enb_msg.stream_id = p_ho_ongoing_ue_ctx_node->
                                ho_ongoing_ue_ctx.stream_id;
                            /*SPR_16713_END*/

                            list_delete_node(&p_x2ap_gb_ctx->ho_ongoing_ue_ctx_list,
                                    (void *)p_ho_ongoing_ue_ctx_node);

                            if (PNULL != p_ho_ongoing_ue_ctx_node)
                            {
                                rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                                p_ho_ongoing_ue_ctx_node = PNULL;
                            }
                            break;
                        }
                        p_ho_ongoing_ue_ctx_node = (x2ap_ho_ongoing_ue_ctx_node_t *)
                            list_next_node(&p_ho_ongoing_ue_ctx_node->node);
                    }
                    /* SPR_14436_Fix Stop */	

                    current_state =  p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
                    /*Invoke UE Associated sig msg FSM*/
                    x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_MSG_REQ_EV,
                            (U8 *)(&peer_enb_msg),
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx, X2AP_NULL);   

                    X2AP_TRACE(X2AP_INFO, "ue_x2ap_index: %d OldState: %s Event %s NewState %s ",
                            p_ue_associated_sig_req->ue_id, X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[current_state],
                            X2AP_UECC_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_REQ_EV],
                            X2AP_UECC_ASSO_SIG_FSM_STATES_NAMES[p_peer_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

                    break;
                }
                default:
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "Unknown Procedure Received");
                    return;
                }
            }
            return;
        }
        default:
        {
            X2AP_TRACE(X2AP_ERROR,
                    "Unknown Message Type Received");
            return;
        } 
    }
    X2AP_UT_TRACE_EXIT();
}

/* SPR_14436_Fix Start */
/****************************************************************************
 * Function Name  : x2ap_allocate_stream_id
 * Inputs          :p_mme_ctx  : Pointer to mme context
 * Outputs        : None
 * Returns        : stream_id
 * Description    : This function is called to  allocate  stream id  by 
 *                  round robin algorithm applied on the active streams in mme
 *****************************************************************************/
U8 x2ap_allocate_stream_id (
	x2ap_gb_context_t 	*p_x2ap_gb_ctx,
    /*SPR 17777 +-*/
 	x2ap_peer_enb_context_t *p_peer_enb_ctx
)
{
    U8 stream_id = MAX_NUM_X2AP_SCTP_STREAM;
	
    X2AP_UT_TRACE_ENTER();	
        
    if (X2AP_ACTIVE_ST == p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
    {
	x2ap_stream_info_t *p_sctp_stream_info = &(p_peer_enb_ctx->stream_info);

        if (X2AP_NULL == p_sctp_stream_info->number_of_streams)
        {    
            X2AP_TRACE(X2AP_ERROR,"[%s]:No Stream  present, hence cannot"
                    " allocate the streams ID\n",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return stream_id;
        }   
 
        if (MAX_NUM_X2AP_SCTP_STREAM > ( p_sctp_stream_info->stream_idx_returned + 1))
        {    
            stream_id = (U8) p_sctp_stream_info->
                stream_ids[p_sctp_stream_info->stream_idx_returned + 1];
            X2AP_TRACE(X2AP_DETAILED,"[%s]: Stream ID = [%d] allocated.\n",
                    __FUNCTION__, stream_id);

	    p_sctp_stream_info->stream_idx_returned++;

            if (p_sctp_stream_info->stream_idx_returned ==
                  (p_sctp_stream_info->number_of_streams -1)) 
            {    
                p_sctp_stream_info->stream_idx_returned = 0; 
            }    

            X2AP_UT_TRACE_EXIT();
            return stream_id;
        }
    }
    else
    {
	X2AP_TRACE(X2AP_ERROR,"x2ap not in ACTIVE state , hence cannot allocate streams from it !\n");
        X2AP_UT_TRACE_EXIT();
        return stream_id;
    }
    
    X2AP_UT_TRACE_EXIT();
    return stream_id;
}
/* SPR_14436_Fix Stop */	

/******************************************************************************
*   FUNCTION NAME: x2ap_uecc_process_ue_reestablishment_conn_setup_rcfg
*
*   DESCRIPTION:
*       Function processes x2ap_ue_reestab_conn_setup_reconfig_t message
*
*   RETURNS:
*       None
*
******************************************************************************/
static void x2ap_uecc_process_ue_reestablishment_conn_setup_rcfg(void  *p_api,
        x2ap_gb_context_t *p_x2ap_gb_ctx )
{
    x2ap_peer_enb_context_t *p_peer_enb_ctx = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_api != PNULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != PNULL);

    x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig = 
        (x2ap_ue_reestab_conn_setup_reconfig_t*)((U8*)p_api + CV_HDRSIZE);

    SET_CELL_INDEX(p_reestab_conn_setup_reconfig->cell_index);

    U16 nbr_cell_count  = 0;
    U16 count = 0;
    U16 peer_enb_count  = 0;

    for (nbr_cell_count = 0;
            nbr_cell_count < MAX_NEIGHBOURS; nbr_cell_count++)
    {
        if (p_reestab_conn_setup_reconfig->pci == 
                p_x2ap_gb_ctx->peer_enb_cell_info[nbr_cell_count].
                peer_cell_info.pci)
        {
            /* fetch the peer_gb_enb_id and send the RLF Ind to peer
             * ENB*/
            for (count = 0; count < MAX_PEER_ENB; count++)
            {
                if ((p_x2ap_gb_ctx->peer_enb_cntxt_arr[count].is_enb_conf == X2AP_TRUE) 
                        && !(X2AP_MEMCMP(
                                &p_x2ap_gb_ctx->peer_enb_cell_info[nbr_cell_count].peer_gb_enb_id,
                                &p_x2ap_gb_ctx->peer_enb_cntxt_arr[count].gb_enb_id,
                                sizeof(x2_gb_enb_id_t))))
                {
                    p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[count];

                    if(p_peer_enb_ctx->x2ap_reset_sm_ctx.state != 
                            X2_RESET_W_RESET_RESP_ST)
                    {
                        if( !(x2ap_build_and_send_rlf_indication(p_x2ap_gb_ctx,
                                        p_peer_enb_ctx,
                                        p_reestab_conn_setup_reconfig)))
                        {
                            X2AP_TRACE(X2AP_ERROR,"x2ap_encode_and_send_rlf_indication failed.");
                            return;
                        }
                    }
                    peer_enb_count++;
                    break;          
                }
            }
        }
    }

    if (peer_enb_count == 0)
    {
        X2AP_TRACE (X2AP_INFO, "No peer eNB found.");
        /*CA_PHASE3_INTER_CELL_MRO_START*/
        /*Checking if the reestablishment occured at source pci or target pci
          and sending internal rlf indication to UECCMD*/
        if (p_reestab_conn_setup_reconfig->pci != 
                p_x2ap_gb_ctx->served_cell_info[p_reestab_conn_setup_reconfig->
                cell_index].pci)
        {
            X2AP_TRACE (X2AP_INFO,"sending internal rlf indication");
            x2ap_build_and_send_internal_rlf_indication (p_x2ap_gb_ctx,
                    p_reestab_conn_setup_reconfig);
        }
        /*CA_PHASE3_INTER_CELL_MRO_START*/
    }
    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: x2ap_uecc_process_ue_rlf_resp
*
*   DESCRIPTION:
*       Function processes x2ap_ue_rlf_resp message
*
*   RETURNS:
*       None
*
******************************************************************************/
static void x2ap_uecc_process_ue_rlf_resp(void  *p_api,
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    x2ap_peer_enb_context_t *p_peer_enb_ctx = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_api != PNULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != PNULL);

    x2ap_ue_rlf_resp_t *p_rlf_resp = 
        (x2ap_ue_rlf_resp_t*)((U8*)p_api + CV_HDRSIZE);

    SET_UE_INDEX(p_rlf_resp->ue_index);

    /*CA_PHASE3_INTER_CELL_MRO_START*/
    x2ap_ho_report_ind_t  x2_ho_report_ind;/*ho report indication that to be
sent to RRM*/
    X2AP_MEMSET(&x2_ho_report_ind, 0, sizeof(x2ap_ho_report_ind_t));
    U16 dst_module_id = RRC_RRM_MODULE_ID;
    /*CA_PHASE3_INTER_CELL_MRO_STOP*/

    if (p_rlf_resp->rlf_ind_flag)
    {
        static x2ap_rlf_ind_t rlf_ind;

        /*Send rlf indication to RRM/SON */
        X2AP_MEMSET(&rlf_ind, 0, sizeof(x2ap_rlf_ind_t));
        rlf_ind.bitmask = 0;
        X2AP_MEMCPY(&rlf_ind.enb1_gb_id, &p_rlf_resp->rlf_ind.enb1_gb_id,
                sizeof(x2_gb_enb_id_t));
        X2AP_MEMCPY(&rlf_ind.enb2_gb_id, &p_rlf_resp->rlf_ind.enb2_gb_id,
                sizeof(x2_gb_enb_id_t));
        rlf_ind.failure_cell_pci = p_rlf_resp->rlf_ind.failure_cell_pci;
        X2AP_MEMCPY(&rlf_ind.re_est_cell_ecgi, &p_rlf_resp->rlf_ind.re_est_cell_ecgi,
                sizeof(x2ap_ecgi_t));
        rlf_ind.crnti = p_rlf_resp->rlf_ind.crnti;
/*CA_PHASE3_INTER_CELL_MRO_START*/
/*Overwriting the enb1_gb_id and enb2_gb_id in case of too late inter cell ho*/
        if (RRC_TRUE == p_rlf_resp->rlf_ind.too_late_inter_cell_ho_flg)
        {
            X2AP_MEMCPY(&rlf_ind.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t));
            X2AP_MEMCPY(&rlf_ind.enb2_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t));
            X2AP_MEMCPY(&rlf_ind.re_est_cell_ecgi, &p_x2ap_gb_ctx->
                    served_cell_info[p_rlf_resp->cell_index].ecgi,
                    sizeof(x2ap_ecgi_t));
        }
/*CA_PHASE3_INTER_CELL_MRO_STOP*/
        if (p_rlf_resp->rlf_ind.bitmask & X2AP_RLF_REPORT_PRESENT)
        {
            rlf_ind.bitmask |= X2AP_RLF_IND_RLF_REPORT_PRESENT;

            X2AP_MEMCPY(&rlf_ind.rlf_report,
                    &p_rlf_resp->rlf_report,
                    sizeof(rrc_rlf_report_t));
        }
        if (p_rlf_resp->rlf_ind.bitmask & X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT)
        {
            rlf_ind.bitmask |= X2AP_RLF_IND_CONN_SETUP_IND_PRESENT;
        }

        rlf_ind.bitmask |= X2AP_RLF_IND_UE_INDEX_PRESENT;
        rlf_ind.ue_index = p_rlf_resp->ue_index;

        if(X2AP_TRUE == p_rlf_resp->ue_context_released)
        {
            rlf_ind.bitmask |= X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT;
            rlf_ind.ue_context_released = p_rlf_resp->ue_context_released;
        }

        if (p_rlf_resp->ho_report_flag != X2AP_TRUE)
        {
            /*RLF_IND_fix_start*/
            if(RRC_FAILURE == rrc_x2apRrm_il_send_x2ap_rlf_ind(
            /*RLF_IND_fix_stop*/
                        &rlf_ind,
                        RRC_X2AP_MODULE_ID,
                        RRC_RRM_MODULE_ID,
                        0,
                        p_rlf_resp->cell_index))
            {
                X2AP_TRACE(X2AP_INFO,"Sending rlf indication failed.\n");
            }
        }
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"rlf_ind_flag is not set. "
                "So not sending rlf ind to RRM/SON.");
    }

    if (p_rlf_resp->ho_report_flag)
    {
        U16 count = 0;
        /*CA_PHASE3_INTER_CELL_MRO_START*/
        /*Checking the flag for too early inter cell ho and sending ho report
         * indication to RRM*/
        if (RRC_TRUE == p_rlf_resp->rlf_ind.too_early_inter_cell_ho_flg)
        {
            X2AP_MEMCPY(&x2_ho_report_ind.enb1_gb_id,
                    &p_rlf_resp->ho_report.enb1_gb_id, sizeof(x2_gb_enb_id_t));
            X2AP_MEMCPY(&x2_ho_report_ind.enb2_gb_id,
                    &p_rlf_resp->ho_report.enb2_gb_id, sizeof(x2_gb_enb_id_t));
            x2_ho_report_ind.ho_report_type = HO_TOO_EARLY;
            X2AP_MEMCPY(&x2_ho_report_ind.ho_cause,
                    &p_rlf_resp->ho_report.ho_cause,
                    sizeof(x2ap_cause_t));
            X2AP_MEMCPY(&x2_ho_report_ind.src_ecgi,
                    &p_rlf_resp->ho_report.src_ecgi,
                    sizeof(x2ap_ecgi_t));
            X2AP_MEMCPY(&x2_ho_report_ind.fail_cell_ecgi,
                    &p_rlf_resp->ho_report.fail_cell_ecgi,
                    sizeof(x2ap_ecgi_t));

            if (RRC_TRUE == p_x2ap_gb_ctx->is_son_present)
            {   
                dst_module_id = RRC_SON_MODULE_ID;
            }   

            if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_ho_report_ind(
                        &x2_ho_report_ind,
                        RRC_X2AP_MODULE_ID,
                        dst_module_id,
                        0,  
                        p_rlf_resp->cell_index))
            {   
                X2AP_TRACE(X2AP_ERROR,"Failed to send HO_REPORT_IND.\n");
            }
        }
        else
        {
            /*CA_PHASE3_INTER_CELL_MRO_STOP*/
            for (count = 0; count < MAX_PEER_ENB; count++)
            {
                /* Bug:9129 */
                if ((p_rlf_resp->ho_report.peer_enodeb_id == 
                            p_x2ap_gb_ctx->peer_enb_cntxt_arr[count].peer_enodeb_id) &&
                        (p_x2ap_gb_ctx->peer_enb_cntxt_arr[count].op_status == OPERATIONAL))
                    /* Bug:9129 */
                {
                    p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[count];

                    if(p_peer_enb_ctx->x2ap_reset_sm_ctx.state !=
                            X2_RESET_W_RESET_RESP_ST)
                    {
                        X2AP_MEMCPY(&p_rlf_resp->ho_report.enb2_gb_id,
                                &p_peer_enb_ctx->gb_enb_id, sizeof(x2_gb_enb_id_t));
                        X2AP_MEMCPY(&p_rlf_resp->ho_report.src_ecgi.plmn_identity.plmn_id,
                                &p_peer_enb_ctx->gb_enb_id.plmn_identity, MAX_PLMN_ID_BYTES);

                        if(!(X2AP_MEMCMP(&p_rlf_resp->ho_report.src_ecgi,
                                        &p_rlf_resp->rlf_ind.re_est_cell_ecgi, 
                                        sizeof(x2ap_ecgi_t))))
                        {
                            X2AP_TRACE(X2AP_INFO,"SrcCellECGI matched with Re-estCellECGI.");
                            p_rlf_resp->ho_report.ho_report_type = HO_TOO_EARLY;
                        }
                        else
                        {
                            X2AP_TRACE(X2AP_INFO,"SrcCellECGI did not match Re-estCellECGI.");
                            p_rlf_resp->ho_report.ho_report_type = HO_TO_WRONG_CELL;
                            p_rlf_resp->ho_report.bitmask |= X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT;
                            X2AP_MEMCPY(&p_rlf_resp->ho_report.re_est_cell_ecgi,
                                    &p_rlf_resp->rlf_ind.re_est_cell_ecgi, 
                                    sizeof(x2ap_ecgi_t));
                        }

                        if(X2AP_FAILURE == 
                                x2ap_enb_build_and_send_handover_report(
                                    p_x2ap_gb_ctx,
                                    p_peer_enb_ctx,
                                    &p_rlf_resp->ho_report))
                        {
                            X2AP_TRACE(X2AP_INFO,"Sending HandoverReport failed.");
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"ho_report_flag is not set. "
                "So not sending HandoverReport to Peer eNB.");
    }
    X2AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : x2ap_process_uecc_messages
 * Inputs         : p_api - Pointer to buffer containing input API
 *                  p_x2ap_gb_ctx -Pointer to the UECC global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when a message is received from
 *                  UECC and is to be sent on UE associated logical X2 
 *                  connection.
 ****************************************************************************/
void  x2ap_process_uecc_messages(
        void    *p_api,
        x2ap_gb_context_t *p_x2ap_gb_ctx )
{
    x2ap_peer_enb_context_t *p_peer_enb_ctx = PNULL;
    ue_x2ap_context_t  *p_ue_x2ap_ctx      = PNULL;
    /* CID: 113475 Start */
    U16 ue_id       = RRC_NULL;
    /* CID: 113475 End */
    U16 api_id      = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();
    
    X2AP_ASSERT(p_api != PNULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != PNULL);
 
    api_id = x2ap_get_api_id((U8*)p_api);   
    
    switch (api_id) 
    {
        case X2AP_UE_ASSOCIATED_SIG_MSG_REQ :
        {
            x2ap_uecc_process_ue_assoc_sig_msg_req(p_api, p_x2ap_gb_ctx);
        }
        break;

        case X2AP_UE_RELEASE_IND:
        {
            x2ap_ue_release_ind_t* p_x2ap_ue_release_ind =
                (x2ap_ue_release_ind_t*)((U8*)p_api + CV_HDRSIZE);
            
             /*Get peer eNB context*/
            p_peer_enb_ctx = &(p_x2ap_gb_ctx->
                    peer_enb_cntxt_arr[p_x2ap_ue_release_ind->peer_enodeb_id]);
            
            if(PNULL == p_peer_enb_ctx)
            {
                X2AP_TRACE(X2AP_ERROR,"Invalid peer enb ctx for received "
                          "peer enodeb id : %d", p_x2ap_ue_release_ind->peer_enodeb_id);
                return;
            }

            /* Get ue index */
            ue_id = p_x2ap_ue_release_ind->ue_id;
      
            SET_UE_INDEX(ue_id);

            if (ue_id >= MAX_NUM_UE)
            {
              X2AP_TRACE(X2AP_ERROR,"Error ::"
              "UE id [%d[ is invalid \n",ue_id);
              return;
            }
            else
            {
                X2AP_TRACE (X2AP_INFO, "Received UE Id :%d",ue_id);
                /*Get UE context for peer EnodeB */
                /* SPR 11006 Fix Start */
                if (PNULL != p_peer_enb_ctx->ue_x2ap_contexts)
                {
                    p_ue_x2ap_ctx = p_peer_enb_ctx->ue_x2ap_contexts[ue_id];
                }
                /* SPR 11006 Fix Stop */
            }
            /* Release the Ue Context */
            if (PNULL != p_ue_x2ap_ctx)
            {
                x2ap_mem_free(p_ue_x2ap_ctx);
                p_peer_enb_ctx->ue_x2ap_contexts[ue_id] = PNULL;
                X2AP_TRACE (X2AP_INFO, "Releasing UE Context at X2AP"
                        " for UE Id:%d", ue_id);
            }
            else
            {
                X2AP_TRACE (X2AP_INFO, "UE Context at X2AP has already released");
            }
        }
        break;

        case X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE:
        {
            x2ap_uecc_process_ue_reestablishment_conn_setup_rcfg(p_api, p_x2ap_gb_ctx);
        }
        break;

        case X2AP_UE_RLF_RESP:
        {
            x2ap_uecc_process_ue_rlf_resp(p_api, p_x2ap_gb_ctx);
        }
        break;

        /* CSR_00040799 Fix Start */
        case X2AP_UECCMD_UE_INFO:
        {
            x2ap_ueccmd_ue_info_t* p_x2ap_uemmd_ue_info =
                (x2ap_ueccmd_ue_info_t*)((U8*)p_api + CV_HDRSIZE);

            if (p_x2ap_uemmd_ue_info->total_ue_supported <= MAX_NUM_SUPPORTED_X2AP_UEID)
            {
                p_x2ap_gb_ctx->total_ue_supported =
                    p_x2ap_uemmd_ue_info->total_ue_supported;
            }
            else
            {
                /* X2 UE Context is allocated during X2 Handover only and
                 * maximum of 512 simultaneous X2 handovers are allowed*/
                X2AP_TRACE(X2AP_INFO,"Maximum of 512 Simultaneous X2 Handovers are possible");
                p_x2ap_gb_ctx->total_ue_supported = MAX_NUM_SUPPORTED_X2AP_UEID;
            }
            /*Get peer eNB context*/
            p_peer_enb_ctx = &(p_x2ap_gb_ctx->
                    peer_enb_cntxt_arr[p_x2ap_uemmd_ue_info->peer_enodeb_id]);
            
            if(PNULL == p_peer_enb_ctx)
            {
                X2AP_TRACE(X2AP_ERROR,"Invalid peer enb ctx for received "
                        "peer enodeb id : %d", 
                        p_x2ap_uemmd_ue_info->peer_enodeb_id);
                return;
            }

            if (PNULL == p_peer_enb_ctx->ue_x2ap_contexts)
            {
                if ( PNULL == ( p_peer_enb_ctx->ue_x2ap_contexts =
                            (ue_x2ap_context_t **) rrc_mem_get ( sizeof(ue_x2ap_context_t *) *
                                p_x2ap_gb_ctx->total_ue_supported )))
                {
                    X2AP_TRACE(X2AP_WARNING,"Unable to allocate memory.");
                    return;
                }
                else
                {
                    memset_wrapper( p_peer_enb_ctx->ue_x2ap_contexts, RRC_NULL, 
                            sizeof(ue_x2ap_context_t*) * p_x2ap_gb_ctx->total_ue_supported );
                }
            }
        }
        break;
        /* CSR_00040799 Fix End */

        default:
        {
          X2AP_TRACE (X2AP_ERROR,"Processing UECC Messages : "
            "Unknown API ::%d\n",api_id);
        }
    }
       
    X2AP_UT_TRACE_EXIT(); 
}

/* Bug 4691 Changes Start */
/*****************************************************************************
 * Function Name  : x2ap_build_and_send_update_info
 * Inputs         : p_nbour_info - pointer to peer eNB Info (PCI , ECGI and EARFCN) 
 * Outputs        : None
 * Returns        : None
 * Description    : This function  build and send x2ap update info
 *                  indication to uecc module.
 ****************************************************************************/
void x2ap_build_and_send_update_info(
           x2ap_nbour_info_t* p_nbour_info
)
{
    x2ap_nbour_info_t    *msg_p = PNULL;
    void *p_intrl_msg            = PNULL;

    X2AP_UT_TRACE_ENTER();

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_UPDATE_NBOUR_INFO,
            (U16)(sizeof(x2ap_nbour_info_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_TRACE(X2AP_ERROR, "[%s]: p_intrl_msg is NULL  \n",
            __FUNCTION__);		
		
        X2AP_UT_TRACE_EXIT();
        return ;
    }
	
    msg_p = (x2ap_nbour_info_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    X2AP_MEMCPY((void*)(msg_p),(const void*)p_nbour_info,sizeof(x2ap_nbour_info_t));
 
    X2AP_TRACE(X2AP_INFO, "[%s]:Forwarding X2AP_UPDATE_NBOUR_INFO message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return ;
}
/* Bug 4691 Changes End */


#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/*****************************************************************************
 * Function Name  : x2ap_build_and_send_nr_status_ind
 * Inputs         : p_enb_ctx - pointer to peer eNB context 
 *                  status - status
 *                  num_served_cell - num of served cells 
 *                  p_served_cell - pointer to x2ap_srvd_cell_arry_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function  build and send x2ap status msg
 *                  indication to uecc module.
 ****************************************************************************/
x2ap_return_et
x2ap_build_and_send_nr_status_ind
(
    x2ap_peer_enb_context_t    *p_enb_ctx,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    U8                         status,
    U16                        num_served_cell,
    x2ap_srvd_nr_cell_info_t   *p_served_cell,
    U16                        total_ue_supported
)
{
    x2ap_status_msg_ind_t    *msg_p = PNULL;
    void                     *p_intrl_msg = PNULL;
    U16                      count = X2AP_NULL;
    U16                      ue_id = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_STATUS_MSG_IND,
            (U16)(sizeof(x2ap_status_msg_ind_t)));

    if (PNULL == p_intrl_msg)
    {
        /*
         * out of memory
         */
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    msg_p = (x2ap_status_msg_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);
    msg_p->peer_enodeb_id = p_enb_ctx->peer_enodeb_id;

    msg_p->bitmask |=
        X2AP_STATUS_MSG_IND_PEER_GB_GNB_ID_PRESENT;
    msg_p->peer_gb_gnb_id = p_enb_ctx->gb_gnb_id;

    msg_p->connection_status = status;

    if (X2AP_LINK_UP == status 
            && X2AP_P_NULL != p_served_cell)
    {
        msg_p->bitmask |= 
            X2AP_STATUS_MSG_IND_NR_SERVED_CELL_ARRAY_PRESENT;

        msg_p->nr_srvd_cell.num_served_nr_cells = num_served_cell;

        for(count=X2AP_NULL; ((count < num_served_cell) &&
              (count < MAX_SERVED_CELLS)); count++)
        {
            X2AP_MEMCPY((void*)(&msg_p->nr_srvd_cell.srvd_cell[count]),
                (const void*)((p_served_cell + count)),
                sizeof(x2ap_srvd_nr_cell_info_t));
        }

    }
    else if (X2AP_LINK_DOWN == status)
    {
        if(X2AP_P_NULL != p_enb_ctx->ue_x2ap_contexts)
        {
            for(ue_id=0; ue_id < total_ue_supported; ue_id++)
            {
                if(X2AP_P_NULL != p_enb_ctx->ue_x2ap_contexts[ue_id])
                { 
                    x2ap_mem_free(p_enb_ctx->ue_x2ap_contexts[ue_id]);
                    p_enb_ctx->ue_x2ap_contexts[ue_id] = PNULL;
                    X2AP_TRACE (X2AP_INFO, "Releasing UE Context at X2AP"
                            " for X2AP UE Id:%d", ue_id);
                }
            }
            x2ap_mem_free(p_enb_ctx->ue_x2ap_contexts);
            p_enb_ctx->ue_x2ap_contexts = PNULL; 
        }
    }
   
    msg_p->bitmask |= X2AP_STATUS_MSG_IND_ENB_COMM_INFO_PRESENT;
    X2AP_MEMCPY((void*)(&msg_p->enb_comm_info),
            (const void*)&(p_enb_ctx->enb_comm_info),
            sizeof(x2_enb_comm_info_t));
    
    X2AP_TRACE(X2AP_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

/*****************************************************************************
 * Function Name  : x2ap_build_and_send_nr_update_info
 * Inputs         : p_nbour_info - pointer to peer gNB Info (PCI , CGI and ARFCN) 
 * Outputs        : None
 * Returns        : None
 * Description    : This function  build and send x2ap update info
 *                  indication to uecc module.
 ****************************************************************************/
void x2ap_build_and_send_nr_update_info(
           x2ap_nr_neigh_info_t* p_nbour_info
)
{
    x2ap_nr_neigh_info_t    *msg_p = PNULL;
    void *p_intrl_msg            = PNULL;

    X2AP_UT_TRACE_ENTER();

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_UPDATE_NR_NBOUR_INFO,
            (U16)(sizeof(x2ap_nr_neigh_info_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_TRACE(X2AP_ERROR, "[%s]: p_intrl_msg is NULL  \n",
            __FUNCTION__);		
		
        X2AP_UT_TRACE_EXIT();
        return ;
    }
	
    msg_p = (x2ap_nr_neigh_info_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    X2AP_MEMCPY((void*)(msg_p),(const void*)p_nbour_info,sizeof(x2ap_nr_neigh_info_t));
 
    X2AP_TRACE(X2AP_INFO, "[%s]:Forwarding X2AP_UPDATE_NR_NBOUR_INFO message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return ;
}
/* MENB CHANGES - END */
#endif
/*****************************************************************************
 * Function Name  : x2ap_build_and_send_status_ind
 * Inputs         : p_enb_ctx - pointer to peer eNB context 
 *                  status - status
 *                  num_served_cell - num of served cells 
 *                  p_served_cell - pointer to x2ap_srvd_cell_arry_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function  build and send x2ap status msg
 *                  indication to uecc module.
 ****************************************************************************/
x2ap_return_et x2ap_build_and_send_status_ind(
           x2ap_peer_enb_context_t *p_enb_ctx,
           U8 status,
           U16 num_served_cell,
           /* CSR_00040799 Fix Start */
           x2ap_srvd_cell_arry_t *p_served_cell,
           U16 total_ue_supported)
           /* CSR_00040799 Fix End */
{
    x2ap_status_msg_ind_t    *msg_p = PNULL;
    void *p_intrl_msg            = PNULL;
    U16 count = 0;
    /* CSR_00040799 Fix Start */
    U16 ue_id = 0;
    /* CSR_00040799 Fix End */

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_STATUS_MSG_IND,
            (U16)(sizeof(x2ap_status_msg_ind_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    msg_p = (x2ap_status_msg_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    

    msg_p->peer_enodeb_id = p_enb_ctx->peer_enodeb_id;

    msg_p->peer_gb_enb_id = p_enb_ctx->gb_enb_id;

    msg_p->connection_status = status;
    if (X2AP_LINK_UP == status 
            && X2AP_P_NULL != p_served_cell)
    {
        msg_p->bitmask |= 
            X2AP_STATUS_MSG_IND_SERVED_CELL_ARRAY_PRESENT;

        msg_p->srvd_cell.num_served_cells = num_served_cell;
       /* Klockwork Fix*/ 
        for(count=0;
            ((count < num_served_cell) 
                         &&
              (count < MAX_SERVED_CELLS)) ;
       /* Klockwork Fix*/ 
             count++)
        {
            X2AP_MEMCPY((void*)(&msg_p->srvd_cell.srvd_cell[count]),
                (const void*)&((p_served_cell + count)->served_cell_info),
                sizeof(x2ap_srvd_cell_info_t));
        }

    }
    /* CSR_00040799 Fix Start */
    else if (X2AP_LINK_DOWN == status)
    {
        if(X2AP_P_NULL != p_enb_ctx->ue_x2ap_contexts)
        {
            for(ue_id=0; ue_id < total_ue_supported; ue_id++)
            {
                if(X2AP_P_NULL != p_enb_ctx->ue_x2ap_contexts[ue_id])
                { 
                    x2ap_mem_free(p_enb_ctx->ue_x2ap_contexts[ue_id]);
                    p_enb_ctx->ue_x2ap_contexts[ue_id] = PNULL;
                    X2AP_TRACE (X2AP_INFO, "Releasing UE Context at X2AP"
                            " for UE Id:%d", ue_id);
                }
            }
            x2ap_mem_free(p_enb_ctx->ue_x2ap_contexts);
            p_enb_ctx->ue_x2ap_contexts = PNULL; 
        }
    }
    /* CSR_00040799 Fix End */
    
    msg_p->bitmask |= X2AP_STATUS_MSG_IND_ENB_COMM_INFO_PRESENT;
    X2AP_MEMCPY((void*)(&msg_p->enb_comm_info),
            (const void*)&(p_enb_ctx->enb_comm_info),
            sizeof(x2_enb_comm_info_t));
    
    X2AP_TRACE(X2AP_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

/******************************************************************************
 * Function Name  : x2ap_get_peer_enb_ctxt_from_ecgi
 * Inputs         : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                  p_ue_associated_sig_req - pointer to 
 *                                            x2ap_ue_associated_sig_msg_req_t
 * Outputs        : None
 * Returns        : peer eNode Context 
 * Description    : This function processes to get the peer enb context from 
 *                  ecgi.
 ******************************************************************************/

x2ap_peer_enb_context_t* x2ap_get_peer_enb_ctxt_from_ecgi(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req
        )
{
    x2ap_peer_enb_context_t *p_peer_enb_ctx = X2AP_P_NULL;
    S32 peer_cell_logical_idx = 0;

    X2AP_UT_TRACE_ENTER();

    do
    {
        /* get Peer Cell Logical Id */
        peer_cell_logical_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctxt,
                p_ue_associated_sig_req->ecgi, X2AP_FALSE, 0);
        if ((peer_cell_logical_idx == -1) || 
            (peer_cell_logical_idx >= MAX_NEIGHBOURS))
        {
            X2AP_TRACE(X2AP_WARNING, "%s:"
                    "Peer Cell Logical Id is not correct.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            p_peer_enb_ctx = X2AP_P_NULL;
            break;
        }

        /*get peer enb context from enb id*/
        if ((peer_cell_logical_idx < MAX_NEIGHBOURS))
        {
            p_peer_enb_ctx = x2ap_get_peer_enb_ctxt_from_enbid(p_x2ap_gb_ctxt,
                    p_x2ap_gb_ctxt->peer_enb_cell_info[
                    peer_cell_logical_idx].peer_gb_enb_id);
        }

    }while(0);

    X2AP_UT_TRACE_EXIT();
    return p_peer_enb_ctx;
}


/*****************************************************************************
 * Function Name  : x2ap_build_and_send_reset_ind
 * Inputs         : p_enb_ctx - pointer to x2ap_peer_enb_context_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function  build and send x2ap reset msg
 *                  indication to uecc module.
 ****************************************************************************/
x2ap_return_et x2ap_build_and_send_reset_ind(
           x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_reset_msg_ind_t    *msg_p = PNULL;
    void *p_intrl_msg            = PNULL;


    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_RESET_MSG_IND,
            (U16)(sizeof(x2ap_reset_msg_ind_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    msg_p = (x2ap_reset_msg_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    msg_p->peer_enodeb_id = p_enb_ctx->peer_enodeb_id;

    msg_p->peer_gb_enb_id = p_enb_ctx->gb_enb_id;
    
    X2AP_TRACE(X2AP_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}


/*****************************************************************************
 * Function Name  : x2ap_fill_protocol_event_params 
 * Inputs         : p_event_header - pointer to EVENT_EXTERNAL_HEADER
 *                  encoded_msg_len - length of encoded ASN msg
 *                  event_id - event id
 *                  message_direction - message of direction 
 * Outputs        : p_event_header
 * Returns        : None
 * Description    : Fills the Local Events' parameters .
 ********************************************************************************/
void x2ap_fill_protocol_event_params(
        EVENT_EXTERNAL_HEADER *p_event_header,
        U32 encoded_msg_len, 
        U16 event_id,
        U32 message_direction)
{
    X2AP_UT_TRACE_ENTER();

    if (PNULL != p_event_header)
    {
        memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len));

        p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len);
        p_event_header->event_id = event_id;

        p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = message_direction;
        p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;
    }
    
    X2AP_UT_TRACE_EXIT();
}

