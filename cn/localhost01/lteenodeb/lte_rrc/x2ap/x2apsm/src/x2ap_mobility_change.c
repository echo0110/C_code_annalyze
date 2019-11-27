
/*******************************************************************************
*
*  FILE NAME   : x2ap_mobility_change.c 
*
*  DESCRIPTION : This file contains the X2AP Mobility Setting Change
*                function definitions.
*
*  DATE 30-Oct-2012  NAME: Sudhansu Sekhar Nanda
*                    REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/

/**********************  HEADER FILE INCLUSION ********************************/
#include "x2ap_uecc.h"
#include "sm_entry.h"
#include "x2ap_timer.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "x2ap_utility.h"
#include "rrc_x2apCommon_intf.h"
#include "x2ap_error_indication.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_il_parser.h"
#include "rrc_logging.h"

/**************** FUNCTION PROTOTYPES ***************************************/
x2ap_return_et  x2ap_process_mobility_change_req(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

void x2ap_send_mobility_change_response(
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2_gb_enb_id_t* p_peer_gb_enb_id, 
        U8 response, 
        U16 trans_id);

x2ap_return_et x2ap_enb_build_and_send_mobility_change_req(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_change_req_t *p_x2ap_mobility_change_req);

x2ap_return_et x2ap_internal_encode_mobility_change_req(
        OSCTXT      *pctxt,        
        U8          *p_buff,        
        U16         *p_buff_size,   
        rrc_x2ap_mobility_change_req_t  *p_mobility_ch_req);

x2ap_return_et x2ap_internal_encode_mobility_change_ack(
        OSCTXT      *pctxt,        
        U8          *p_buff,      
        U16         *p_buff_size,
        rrc_x2ap_mobility_change_ack_t  *p_mobility_change_ack);

x2ap_return_et x2ap_internal_decode_mobility_change_req(
        OSCTXT      *p_asn1_ctx,  
        x2ap_MobilityChangeRequest  *p_3gpp,   
        rrc_x2ap_mobility_change_req_t *p_intrl, 
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

void x2ap_stop_mobility_setting_change_procedure( 
        x2ap_peer_enb_context_t  *p_enb_ctx);

x2ap_return_et x2ap_internal_decode_mobility_change_ack(
        OSCTXT      *p_asn1_ctx, 
        x2ap_MobilityChangeAcknowledge  *p_3gpp,
        rrc_x2ap_mobility_change_ack_t  *p_intrl, 
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

static x2ap_return_et x2ap_validate_eNB1_cell_id(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req);

static x2ap_return_et x2ap_validate_served_cell_id(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        rrc_x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req);

x2ap_return_et x2ap_enb_build_and_send_mobility_change_failure(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_change_resp);

void x2ap_fill_mobility_setting_change_cause(
        x2ap_cause_t cause, 
        x2ap_Cause* p_asn_cause);

x2ap_return_et x2ap_enb_mobility_change_start_timer(
        x2ap_timer_id_en               timer_type,
        x2ap_peer_enb_context_t        *p_enb_ctx,
        U32                            timer_value,
        x2ap_timer_t                   *p_timer_id);

x2ap_return_et  x2ap_process_mobility_change_resp(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

x2ap_return_et x2ap_enb_build_and_send_mobility_change_resp(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_change_resp);

x2ap_return_et x2ap_process_mobility_change_request_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_process_mobility_change_response_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_process_mobility_change_failure_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_internal_encode_mobility_change_fail(
        OSCTXT      *pctxt,       
        U8          *p_buff,     
        U16         *p_buff_size, 
        rrc_x2ap_mobility_change_fail_t  *p_mobility_change_fail);

void x2ap_fill_rrm_mobility_change_request(
        x2ap_mobility_setting_change_req_t *p_x2ap_mobility_change_req,
        rrc_x2ap_mobility_change_req_t  *p_msg);

void x2ap_fill_rrm_mobility_change_response(
        x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
        rrc_x2ap_mobility_change_ack_t  *p_msg);

x2ap_return_et  x2ap_parse_mobility_cause(
        x2ap_Cause    *p_cause,
        x2ap_Cause     *p_Cause,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id);

x2ap_return_et  x2ap_parse_mobility_CriticalityDiagnostics(
        x2ap_crit_diagno_t            *p_CriticalityDiagno,
        x2ap_CriticalityDiagnostics   *p_CriticalityDiagnostics);

void x2ap_mobility_change_fill_cause(
        x2ap_Cause* p_src,
        x2ap_cause_t* p_dst);

x2ap_return_et x2ap_decode_mobility_change_failure(
        OSCTXT      *p_asn1_ctx, 
        x2ap_MobilityChangeFailure *p_3gpp,  
        rrc_x2ap_mobility_change_fail_t *p_intrl, 
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

void x2ap_fill_rrm_mobility_change_failure(
        x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
        rrc_x2ap_mobility_change_fail_t *p_msg);

x2ap_return_et  x2ap_build_and_send_mobility_change_failure(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
        U8  cause);

x2ap_return_et x2ap_enb_mobility_change_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);

/*spr_fix*/
x2ap_return_et x2ap_mobility_change_rrm_guard_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);
/*spr_fix*/

/*******************************************************************************
 * Function Name  : x2ap_process_mobility_change_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - destination Id
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS 
 * Description    : This function processes the Mobility Change Request message
 *                  received from RRM.
 ******************************************************************************/
x2ap_return_et  x2ap_process_mobility_change_req(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req = X2AP_P_NULL;
    x2ap_return_et response = X2AP_FAILURE;
    x2ap_peer_enb_context_t  *p_enb_ctx = X2AP_P_NULL;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    U16 trans_id = X2AP_NULL;
    S32 length_read = X2AP_NULL;
    U8 counter = 0;
    
    X2AP_UT_TRACE_ENTER();
    /* Receive pointer should not be NULL */
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));    

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    p_x2ap_mobility_change_req = (x2ap_mobility_change_req_t*)
        x2ap_mem_get(sizeof(x2ap_mobility_change_req_t));

    if(PNULL ==  p_x2ap_mobility_change_req)
    {
        X2AP_TRACE(X2AP_FATAL, "Unable to allocate memory.");
        return X2AP_FAILURE;
    }

    X2AP_MEMSET(p_x2ap_mobility_change_req, X2AP_NULL, sizeof(x2ap_mobility_change_req_t));

    /* Decode the received message from RRM */
    response = rrc_parse_x2ap_mobility_change_req(
            p_x2ap_mobility_change_req,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if(X2AP_SUCCESS == response)
    {
        /* Validate eNB1 Cell IDs. If invalid, return failure. */
        if (X2AP_FAILURE ==
                x2ap_validate_eNB1_cell_id(p_x2ap_gb_ctx,
                   p_x2ap_mobility_change_req))
        {
            X2AP_TRACE(X2AP_WARNING, "validation of eNB1 cell id failed");
            response = X2AP_FAILURE;
        }

        /*Fetch peer eNB context*/
        for(counter = 0; counter < MAX_PEER_ENB; counter++)
        {
            if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                            &p_x2ap_mobility_change_req->gb_enb_id, 
                            sizeof(x2_gb_enb_id_t))))
            {
                p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                break;
            }
        }
        if(p_enb_ctx == X2AP_P_NULL)
        {
            response = X2AP_FAILURE; 
            X2AP_TRACE(X2AP_WARNING, "No peer eNB context found");
        }   
        else if(p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_MOBILITY_CHANGE_ACTIVE_ST)
        {
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.retry_count = 
                p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_mobility_setting_change_retry_cnt;

            X2AP_TRACE(X2AP_INFO, "X2AP MOBILITY SETTING CHANGE retry count:%u", 
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.retry_count);

            /* Send MOBILITY CHANGE REQUEST to peer eNB.*/
            response = x2ap_enb_build_and_send_mobility_change_req(
                    p_x2ap_gb_ctx, p_enb_ctx, p_x2ap_mobility_change_req);
            if(X2AP_SUCCESS == response)
            {
                /*save the transaction_ID in MOBILITY CHANGE context*/
                p_enb_ctx->x2ap_mobility_ch_sm_ctx.trans_id = trans_id;
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, 
                        "[%s]: X2AP MOBILITY SETTING CHANGE REQUEST send failure",__FUNCTION__);
                response = X2AP_FAILURE;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, 
                    "Invalid X2AP MOBILITY CHANGE REQUEST SM state:%u, ignoring mobility change req.",
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.state);
            response = X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap_mobility_change_req decode failed");
        /* coverity_fix_63315_start */
        x2ap_mem_free((void *)p_x2ap_mobility_change_req);
        /* coverity_fix_63315_stop */
        return X2AP_FAILURE;
    }

    if(response == X2AP_FAILURE)
    {
		x2ap_send_mobility_change_response(p_x2ap_gb_ctx, 
                &p_x2ap_mobility_change_req->gb_enb_id, 
                X2AP_FAILURE, 
                trans_id);
    }
    /* coverity_fix_63315_start */
    x2ap_mem_free((void *)p_x2ap_mobility_change_req);
    /* coverity_fix_63315_stop */
    return response;
}

/******************************************************************************
 * Function Name : x2ap_send_mobility_change_response 
 * Input         : p_x2ap_gb_ctx - pointer to x2ap_gb_context_t
 *                 p_peer_gb_enb_id - pointer to x2_gb_enb_id_t
 *                 response - responce to be sent
 *                 trans_id - transaction id
 * Outputs       : None
 * Returns       : None
 * Description   : This function sends Mobility Change Response to RRM/SON 
 *******************************************************************************/
void x2ap_send_mobility_change_response(
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2_gb_enb_id_t* p_peer_gb_enb_id, 
        U8 response, 
        U16 trans_id)
{
	x2ap_mobility_change_resp_t  mobility_change_resp;
	U16 dest_module_id = RRC_RRM_MODULE_ID;
	X2AP_MEMSET(&mobility_change_resp, X2AP_NULL, sizeof(x2ap_mobility_change_resp_t));

	X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb1_gb_id, 
            &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
	X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb2_gb_id, 
            p_peer_gb_enb_id, sizeof(x2_gb_enb_id_t));
	mobility_change_resp.mobility_change_resp.response = response;

	if(p_x2ap_gb_ctx->is_son_present)
	{
		dest_module_id = RRC_SON_MODULE_ID;
	}

	if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_mobility_change_resp(
											  &mobility_change_resp,
											  RRC_X2AP_MODULE_ID,
											  dest_module_id,
											  trans_id,
                                              RRC_NULL))
	{
		X2AP_TRACE(X2AP_ERROR,"Failed to send Mobility Change response.\n");
	}
}

/*******************************************************************************
 * Funxtion Name : x2ap_enb_build_and_send_mobility_change_req
 * Inputs        : p_x2ap_gb_ctx - pointer to global context
 *                 p_enb_ctx - pointer to peer enb context
 *                 p_x2ap_mobility_change_req - pointer to mobility change req 
 * OutPuts       : None
 * Returns       : x2ap_SUCCESS or X2AP_FAILURE
 * Description   : This function builds and sends X2AP MOBILITY CHANGE REQ message
 *                to X2AP module.
 * RETURNS      : x2ap_return_et
 *
 *******************************************************************************/

x2ap_return_et x2ap_enb_build_and_send_mobility_change_req(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_change_req_t *p_x2ap_mobility_change_req)
{
    x2ap_return_et result = X2AP_FAILURE;
    OSCTXT asn1_ctx;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;
    
    rrc_x2ap_mobility_change_req_t msg;
    
    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    
    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_mobility_change_req);
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap mobility setting change request] "
                "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    
    /* Reset message */
    X2AP_MEMSET(&msg, 0, sizeof(msg));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

    /* Filling eNB1 cell id */
    msg.enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg.enb1_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_req->mobility_change_info.enb1_cell_id.
            plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);

    msg.enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg.enb1_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_req->mobility_change_info.
            enb1_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);
    
    /* Filling eNB2 cell id */
    msg.enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg.enb2_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_req->mobility_change_info.enb2_cell_id.
            plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);

    msg.enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg.enb2_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_req->mobility_change_info.
            enb2_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);
    
    /* Filling eNB1 Mobility Parameters */
    if(p_x2ap_mobility_change_req->mobility_change_info.bitmask & 
            X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {
        msg.enb1_mobility_param.handoverTriggerChange = 
            p_x2ap_mobility_change_req->mobility_change_info.
            enb1_mob_param.handover_trigger_change;
        msg.bitmask |= RRC_X2AP_ENB1_MOBILITY_PARAMETER_PRESENT;
    }
    /* Filling eNB2 Proposed Mobility Parameters */
    msg.enb2_proposed_mob_param.handoverTriggerChange = 
        p_x2ap_mobility_change_req->mobility_change_info.
        enb2_mob_param.handover_trigger_change;
    
    /* Filling cause */
    x2ap_fill_mobility_setting_change_cause(
            p_x2ap_mobility_change_req->mobility_change_info.cause,
            &msg.cause);
    /* Save Reset request message for next retry */
    X2AP_MEMCPY(&p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req, 
            p_x2ap_mobility_change_req, sizeof(x2ap_mobility_change_req_t));

    /* Encode message */
    result = x2ap_internal_encode_mobility_change_req(&asn1_ctx,
            asn_msg_buff,
            &asn_msg_len,
            &msg);

    if(X2AP_SUCCESS == result)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */
        if(INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
        {
            x2ap_timer_t timer_id ={0};

            /*Sending the X2AP Mobility change Request to Peer eNB via SCTP */
            result = x2ap_sctp_sm_entry(X2AP_SCTP_SEND_DATA_EV,
                    (U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);

            if(X2AP_SUCCESS == result)
            {
                /* Start X2AP_ENB_MOBILITY_CH_TIMER and save timer ID.*/
                if(X2AP_SUCCESS == x2ap_enb_mobility_change_start_timer(
                            X2AP_ENB_MOBILITY_CH_TIMER,
                            p_enb_ctx,
                            p_x2ap_gb_ctx->config_param.timer_conf_info.
                            x2ap_mobility_setting_change_guard_tim_val,
                            &timer_id))
                {
                    X2AP_TRACE(X2AP_INFO,"X2AP_ENB_MOBILITY_CH_TIMER:%p Started Successfully.\n",timer_id);
                    /* Change state to X2_W_MOBILITY_CHANGE_PEER_RESP_ST.*/
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_W_MOBILITY_CHANGE_PEER_RESP_ST;

                    /* save the timer id in Mobility Change context */
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = timer_id;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"Starting X2AP_ENB_MOBILITY_CH_TIMER failed.\n");
                }
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,"Sending Mobility Change Req Message to peer failed.\n");
            }

        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_Mobility Setting Change Request] Encode failure");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;

}
/******************************************************************************
 * Function Name : x2ap_internal_encode_mobility_change_req
 * Inputs        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - pointer to (size of result buffer
 *                 p_mobility_ch_req
 * Outputs       : p_buff (ASN.1 encoded message),
 *                 p_buff_size (size of result buffer)
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE    
 * Description   : This function used to encode Mobility Change Request.
 *****************************************************************************/
x2ap_return_et x2ap_internal_encode_mobility_change_req 
(
     OSCTXT      *pctxt,         /* for memory allocation */
     U8          *p_buff,        /* ASN.1 encoded message */
     U16         *p_buff_size,   /* size of result buffer */
     /* source */
     rrc_x2ap_mobility_change_req_t  *p_mobility_ch_req)
{
    x2ap_return_et              result = X2AP_FAILURE;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_MobilityChangeRequest *p_x2ap_msg = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_mobility_ch_req);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_mobilitySettingsChange;

        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

        /* Set the initiating message type to Mobility setting change request */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__mobilitySettingsChange;
        x2ap_pdu.u.initiatingMessage->value.u.mobilitySettingsChange = 
            rtxMemAllocType(pctxt,x2ap_MobilityChangeRequest);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.mobilitySettingsChange)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.mobilitySettingsChange;

        asn1Init_x2ap_MobilityChangeRequest(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_MobilityChangeRequest_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_ch_req->enb1_cell_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_MobilityChangeRequest_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_ch_req->enb2_cell_id))
        {
            break;
        }

        if( RRC_X2AP_ENB1_MOBILITY_PARAMETER_PRESENT &
                p_mobility_ch_req->bitmask)
        {
            if(RT_OK != asn1Append_x2ap_MobilityChangeRequest_protocolIEs_3(
                        pctxt,//&asn1_ctx,
                        &p_x2ap_msg->protocolIEs,
                        &p_mobility_ch_req->enb1_mobility_param))
            {
                break;
            }
        }

        if (RT_OK != asn1Append_x2ap_MobilityChangeRequest_protocolIEs_4(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_ch_req->enb2_proposed_mob_param))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_MobilityChangeRequest_protocolIEs_5(
                    pctxt,
                    &p_x2ap_msg->protocolIEs,
                    &p_mobility_ch_req->cause))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        /* SPR 20158 Fix Start */ 
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU: MOBILITY CHANGE REQUEST", &x2ap_pdu);
            *p_buff_size = (U16)pe_GetMsgLen(pctxt);
            result=X2AP_SUCCESS;
        }
        /* SPR 20158 Fix End */ 
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/*******************************************************************************
 * Function Name : x2ap_internal_encode_mobility_change_ack
 * Inputs        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - pointer to size of result buffer
 *                 p_mobility_change_ack
 * Outputs       : p_buff (ASN.1 encoded message),
 *                 p_buff_size (size of result buffer)
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE    
 * Description   : This function used to encode Mobility Change Acknowledge.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_mobility_change_ack 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_mobility_change_ack_t  *p_mobility_change_ack
)
{
    x2ap_return_et                  result = X2AP_FAILURE;
    x2ap_X2AP_PDU                   x2ap_pdu;
    x2ap_MobilityChangeAcknowledge  *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_mobility_change_ack);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to successful outcome */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

        x2ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, x2ap_SuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_x2ap_id_mobilitySettingsChange;
         
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to mobility change acknowledge */
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__mobilitySettingsChange;

        x2ap_pdu.u.successfulOutcome->value.u.mobilitySettingsChange = 
            rtxMemAllocType(pctxt,x2ap_MobilityChangeAcknowledge);
        if (PNULL == x2ap_pdu.u.successfulOutcome->value.u.mobilitySettingsChange)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        p_x2ap_msg = x2ap_pdu.u.successfulOutcome->value.u.mobilitySettingsChange;
        
        asn1Init_x2ap_MobilityChangeAcknowledge(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_MobilityChangeAcknowledge_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_change_ack->enb1_cell_id))
        {
            break;
        }
        
        if (RT_OK != asn1Append_x2ap_MobilityChangeAcknowledge_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_change_ack->enb2_cell_id))
        {
            break;
        }
        if(
                RRC_X2AP_CRITICALITY_DIAGNOSTICS_PRESENT & 
                p_mobility_change_ack->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_MobilityChangeAcknowledge_protocolIEs_3(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_mobility_change_ack->criticality_diagnostics))
            {
                break;
            }
        }
        
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        /* SPR 20158 Fix Start */ 
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU: MOBILITY CHANGE ACKNOWLEDGE", &x2ap_pdu);
            *p_buff_size = (U16)pe_GetMsgLen(pctxt);
            result=X2AP_SUCCESS;
        }
        /* SPR 20158 Fix End */ 
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/****************************************************************************
 * Function Name : x2ap_internal_decode_mobility_change_req 
 * Inputs        : p_asn1_ctx -  Pointer to ASN Context
 *                 p_3gpp - Pointer to x2ap_MobilityChangeRequest
 *                 p_intrl - Pointer to rrc_x2ap_mobility_change_req_t
 *                 p_x2ap_gl_ctx - Pointer to X2AP global context
 *                 p_enb_ctx - Pointer to peer eNb context
 * Outputs       : p_intrl 
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to decode mobility change req
 ****************************************************************************/
x2ap_return_et x2ap_internal_decode_mobility_change_req 
(
    OSCTXT      *p_asn1_ctx,   /* Pointer to ASN Context */
    x2ap_MobilityChangeRequest  *p_3gpp,    /* unpacked src */
    rrc_x2ap_mobility_change_req_t *p_intrl, /* dest */
    x2ap_gb_context_t  *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t *p_enb_ctx
)
{
    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_MobilityChangeRequest_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    x2ap_return_et result = X2AP_SUCCESS;

    x2ap_message_data_t message_map =
    {5, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ENB1_Cell_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {1, ASN1V_x2ap_id_ENB2_Cell_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {2, ASN1V_x2ap_id_ENB1_Mobility_Parameters, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {3, ASN1V_x2ap_id_ENB2_Proposed_Mobility_Parameters, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {4, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));

    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: next node returned NULL", __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_MobilityChangeRequest_protocolIEs_element*)p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_ENB1_Cell_ID:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_1))
                    {
                        /*p_intrl->enb1_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_1;*/
                        p_intrl->enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_1->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_1->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);
                    }
                }
                break;

                case ASN1V_x2ap_id_ENB2_Cell_ID:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_2))
                    {
                        /*p_intrl->enb2_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_2;*/
                        p_intrl->enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_2->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_2->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);
                    }
                }
                break;

                case ASN1V_x2ap_id_ENB1_Mobility_Parameters:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_3))
                    {
                        p_intrl->enb1_mobility_param = 
                            *p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_3;
                        p_intrl->bitmask |= RRC_X2AP_ENB1_MOBILITY_PARAMETER_PRESENT;
                    }
                }
                break;

                case ASN1V_x2ap_id_ENB2_Proposed_Mobility_Parameters:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_4))
                    {
                        p_intrl->enb2_proposed_mob_param = 
                            *p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_4;
                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_5))
                    {
                        if(X2AP_FAILURE == x2ap_parse_mobility_cause(
                                    &(p_intrl->cause),
                                    p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_5,
                                    &message_map,
                                    (U8)index,
                                    (U8)p_protocol_IE->id))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s:x2ap_parse_cause failed",__FUNCTION__);
                            break;
                        }
                        /*p_intrl->cause = 
                         *p_protocol_IE->value.u._x2ap_MobilityChangeRequest_IEs_5;*/
                    }
                }
                break;
                
                default:
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

					/*SPR_19067_FIX_START*/
                	if (x2ap_reject == p_protocol_IE->criticality) 
                	{
                    	result = X2AP_FAILURE;
                	}
                	/*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_mobilitySettingsChange,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in"
                "Mobility setting change request");

        result = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  : x2ap_internal_decode_mobility_change_ack 
 * Inputs         : p_asn1_ctx - Pointer to ASN Context
 *                  p_3gpp - pointer to x2ap_MobilityChangeAcknowledge
 *                  p_intrl -  pointer to rrc_x2ap_mobility_change_ack_t
 *                  p_x2ap_gl_ctx - pointer to X2Ap global context 
 *                  p_enb_ctx - pointer to peer eNb context 
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function used to decode mobility change ack
 ****************************************************************************/
x2ap_return_et x2ap_internal_decode_mobility_change_ack
(
    OSCTXT      *p_asn1_ctx,   /* Pointer to ASN Context */
    x2ap_MobilityChangeAcknowledge  *p_3gpp,    /* unpacked src */
    rrc_x2ap_mobility_change_ack_t  *p_intrl,    /* dest */
    x2ap_gb_context_t  *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t *p_enb_ctx
)
{
    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_MobilityChangeAcknowledge_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    x2ap_return_et result = X2AP_SUCCESS;

    x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ENB1_Cell_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {1, ASN1V_x2ap_id_ENB2_Cell_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
         {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));
    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_MobilityChangeAcknowledge_protocolIEs_element*)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_ENB1_Cell_ID:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_1))
                    {
                        /*p_intrl->enb1_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_1;*/
                        p_intrl->enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_1->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_1->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);
                    }
                }
                break;

                case ASN1V_x2ap_id_ENB2_Cell_ID:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_2))
                    {
                        /*p_intrl->enb2_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_2;*/
                        p_intrl->enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_2->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_2->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);
                        
                    }
                }
                break;

                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_3))
                    {
                        /*p_intrl->criticality_diagnostics = 
                         *p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_3;*/
                        /*result = x2ap_parse_mobility_CriticalityDiagnostics(
                                &(p_intrl->criticality_diagnostics),
                                p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_3);*/
                        x2ap_log_criticality_diagostics(p_protocol_IE->value.u._x2ap_MobilityChangeAcknowledge_IEs_3);
                        p_intrl->bitmask |= RRC_X2AP_CRITICALITY_DIAGNOSTICS_PRESENT;
                    }
                }
                break;

                default:
                    X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

					/*SPR_19067_FIX_START*/
                	if (x2ap_reject == p_protocol_IE->criticality) 
                	{
                    	result = X2AP_FAILURE;
                	}
                	/*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_mobilitySettingsChange,
                    T_x2ap_X2AP_PDU_successfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in"
                "Mobility setting change response");

        result = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();

    return result;
}

/*****************************************************************************
 * Function Name  : x2ap_validate_eNB 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_x2ap_mobility_change_req - pointer to
 *                                                x2ap_mobility_change_req_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function validate the ecgi for the eNB1 cell id
 ********************************************************************************/
static
x2ap_return_et x2ap_validate_eNB1_cell_id
(
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req
)
{
    U8 count = 0;
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_mobility_change_req);

    for(count = 0; count < p_x2ap_gb_ctx->num_served_cells; count++)
    {
        /* SPR 11678 Start */
     if (0 ==
            X2AP_MEMCMP(p_x2ap_mobility_change_req->mobility_change_info.enb1_cell_id.eutran_cell_id,
            p_x2ap_gb_ctx->served_cell_info[p_x2ap_gb_ctx->act_cell_info[count].cell_id].ecgi.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE))
        /* SPR 11678 Stop */
     {
           retVal  =  X2AP_SUCCESS;
           break;
     }
    }
    
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_validate_served_cell_id 
 * Inputs         : p_x2ap_gb_ctx - pointer to x2ap_gb_context_t
 *                  p_x2ap_mobility_change_req -  pointer to
 *                                             rrc_x2ap_mobility_change_req_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function validate the ecgi for the served cell id
 ********************************************************************************/
static
x2ap_return_et x2ap_validate_served_cell_id
(
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 rrc_x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req
)
{
    U8 count = 0;
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_mobility_change_req);

    for(count = 0; count < p_x2ap_gb_ctx->num_served_cells; count++)
    {	/* SPR 11688 Fix Start */
     if (0 ==
            X2AP_MEMCMP(p_x2ap_mobility_change_req->enb2_cell_id.eUTRANcellIdentifier.data,
	    p_x2ap_gb_ctx->served_cell_info[p_x2ap_gb_ctx->act_cell_info[count].cell_id].ecgi.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE))/* SPR 11688 Fix Stop */
     {
           retVal  =  X2AP_SUCCESS;
           break;
     }
    }
    
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_fill_mobility_setting_change_cause 
 * Inputs         : cause - type of x2ap_cause_t
 *                  p_asn_cause - pointer to x2ap_cause_t
 * Outputs        : p_asn_cause
 * Returns        : None
 * Description    : This function fills Cause received from RRM
 ********************************************************************************/
void x2ap_fill_mobility_setting_change_cause(x2ap_cause_t cause, 
        x2ap_Cause* p_asn_cause)
{
	switch(cause.type)
	{
		case T_x2ap_Cause_radioNetwork:
		{
			p_asn_cause->t = T_x2ap_Cause_radioNetwork;
			p_asn_cause->u.radioNetwork = cause.value;
		}break;
		case T_x2ap_Cause_transport:
		{
			p_asn_cause->t = T_x2ap_Cause_transport;
			p_asn_cause->u.transport = cause.value;
		}break;
		case T_x2ap_Cause_protocol:
		{
			p_asn_cause->t = T_x2ap_Cause_protocol;
			p_asn_cause->u.protocol = cause.value;
		}break;
		case T_x2ap_Cause_misc:
		{
			p_asn_cause->t = T_x2ap_Cause_misc;
			p_asn_cause->u.misc = cause.value;
		}break;
		default:
		{
			X2AP_TRACE(X2AP_ERROR,"[%s]Unknown cause type.%u\n",__FUNCTION__ , cause.type);
		}break;
	}
}

/*****************************************************************************
 * Function Name  : x2ap_enb_mobility_change_start_timer
 * Inputs         : timer_type    - Timer Type
 *                  p_enb_ctx - pointer to peer eNB context 
 *                  timer_value  - Timer Value
 *                  p_timer_id  - Timer Id
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This starts x2ap enb mobility setting change timer 
 *****************************************************************************/
x2ap_return_et x2ap_enb_mobility_change_start_timer 
(
 x2ap_timer_id_en               timer_type,
 x2ap_peer_enb_context_t        *p_enb_ctx,
 U32                            timer_value,
 x2ap_timer_t                   *p_timer_id
 )
{
    x2ap_return_et ret_val = X2AP_SUCCESS;
    x2ap_timer_buffer_t timer_buf;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&timer_buf, X2AP_NULL, sizeof(x2ap_timer_buffer_t));
    
    timer_buf.timer_type = timer_type;
    timer_buf.auto_delete = X2AP_TRUE;
    timer_buf.sd = p_enb_ctx->sctp_sd;
    timer_buf.p_timer_specific_data = (void*)p_enb_ctx;

    *p_timer_id = x2ap_start_timer(
            timer_value, &timer_buf,
            sizeof(timer_buf), X2AP_FALSE );

    X2AP_TRACE(X2AP_INFO,
            "[%s] Start X2AP ENB Mobility Setting Change Timer: %d Timer Value:%d",
            __FUNCTION__, timer_type, timer_value);
    X2AP_UT_TRACE_EXIT();
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_process_mobility_change_resp 
 * Inputs         : p_api - pointer to input API buffer
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function process the X2AP_MOBILITY_SETTING_CHANGE_RESP
 *                  received from the rrm 
 ********************************************************************************/
x2ap_return_et  x2ap_process_mobility_change_resp(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_ch_resp = X2AP_P_NULL;
    S32 length_read = X2AP_NULL;
    x2ap_return_et response = X2AP_FAILURE;
    x2ap_peer_enb_context_t  *p_peer_enb_ctx = X2AP_P_NULL;
    U8  counter = 0;

    X2AP_UT_TRACE_ENTER();

    /* Receive pointer should not be NULL */
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);


    p_x2ap_mobility_setting_ch_resp = (x2ap_mobility_setting_change_resp_t*)
        x2ap_mem_get(sizeof(x2ap_mobility_setting_change_resp_t));

    if(PNULL ==  p_x2ap_mobility_setting_ch_resp)
    {
        X2AP_TRACE(X2AP_FATAL, "Unable to allocate memory.");
        return X2AP_FAILURE;
    }

    /*X2AP_MEMSET(p_x2ap_mobility_setting_ch_resp, X2AP_NULL, 
            sizeof(x2ap_mobility_setting_change_resp_t));*/

    /* Decode the received message from RRM */
    response = rrc_parse_x2ap_mobility_setting_change_resp(
            p_x2ap_mobility_setting_ch_resp,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if(X2AP_SUCCESS == response)
    {
        /*Fetch peer eNB context*/
        for(counter = 0; counter < MAX_PEER_ENB; counter++)
        {
            if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                            &p_x2ap_mobility_setting_ch_resp->mobility_setting_change_resp.enb1_gb_id,
                            sizeof(x2_gb_enb_id_t))))
            {
                p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                break;
            }
        }
        if(p_peer_enb_ctx == X2AP_P_NULL)
        {
            X2AP_TRACE(X2AP_WARNING,
                    "Process Mobility change response : No peer eNB context found");
            /* coverity_fix_63316_start */
            x2ap_mem_free((void *)p_x2ap_mobility_setting_ch_resp);
            /* coverity_fix_63316_stop */
            return X2AP_FAILURE;
        }
        /*Check the state of Mobility Change SM*/
        else if(X2_W_MOBILITY_CHANGE_RRM_RESP_ST ==
                p_peer_enb_ctx->x2ap_mobility_ch_sm_ctx.state) 
        {
	    /* CSR 95572_SPR_14532 start */
            /* Stop X2AP_MOBILITY_CH_RRM_GUARD_TIMER .*/
            x2ap_stop_timer(p_peer_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id);
            p_peer_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_P_NULL;
            /* CSR 95572_SPR_14532 end */

            X2AP_TRACE(X2AP_INFO, "peer eNB context found");
            if((X2AP_MOBILITY_SETTING_CAUSE_PRESENT &
                    p_x2ap_mobility_setting_ch_resp->mobility_setting_change_resp.bitmask) ||
                    (X2AP_FAILURE == p_x2ap_mobility_setting_ch_resp->mobility_setting_change_resp.response))
            {
                /*Build and send the X2AP: Mobility Change Failure to Peer Enb */
                if(X2AP_FAILURE == x2ap_enb_build_and_send_mobility_change_failure(
                            p_x2ap_gb_ctx, p_peer_enb_ctx, p_x2ap_mobility_setting_ch_resp))
                {
                    response = X2AP_FAILURE;
                    X2AP_TRACE(X2AP_WARNING,
                            "Process Mobility change failure:"
                            "x2ap_enb_build_and_send_mobility_change_failure -failed");
                }
                else
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "Process Mobility change failure: "
                            "x2ap_enb_build_and_send_mobility_change_failure -success :");
                    p_peer_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST; 
                }

            }
            else
            {
                /*Build and send the X2AP: Mbility change response Peer Enb */
                if(X2AP_FAILURE == x2ap_enb_build_and_send_mobility_change_resp(
                            p_x2ap_gb_ctx, p_peer_enb_ctx, p_x2ap_mobility_setting_ch_resp))
                {
                    response = X2AP_FAILURE;
                    X2AP_TRACE(X2AP_WARNING,
                            "Process Mobility change response:"
                            "x2ap_enb_build_and_send_mobility_change_resp -failed");
                }
                else
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "Process Mobility change response: "
                            "x2ap_enb_build_and_send_mobility_change_resp -success :");
                    p_peer_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST; 
                }
            }
        }
        else
        {
            response = X2AP_FAILURE;
            X2AP_TRACE(X2AP_WARNING,
                    "Process Process Mobility change response : invalid enb state");
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "X2AP Peer Mobility Change Response - Decode Failed");
        /* coverity_fix_63316_start */
        x2ap_mem_free((void *)p_x2ap_mobility_setting_ch_resp);
        /* coverity_fix_63316_stop */
        return X2AP_FAILURE;
    }
    if(response == X2AP_FAILURE)
    {
        x2ap_build_and_send_mobility_change_failure(
                p_x2ap_gb_ctx,
                p_peer_enb_ctx,
                X2AP_P_NULL,
                X2AP_NULL);
    }
    /* coverity_fix_63316_start */
    x2ap_mem_free((void *)p_x2ap_mobility_setting_ch_resp);
    /* coverity_fix_63316_stop */
    return response;
}

/******************************************************************************
 * Function Name : x2ap_enb_build_and_send_mobility_change_resp 
 * Inputs        : p_x2ap_gb_ctx - pointer to global context
 *                 p_enb_ctx - pointer to the peer enodeb context
 *                 p_x2ap_mobility_change_resp - pointer to mobility change resp
 * Outputs       : Noneee 
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * DESCRIPTION   : This function builds and sends Mobility change Response message
 *                 to X2AP module.
 *******************************************************************************/

x2ap_return_et x2ap_enb_build_and_send_mobility_change_resp(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_change_resp
)
{
    x2ap_return_et result = X2AP_FAILURE;
    OSCTXT asn1_ctx;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;

    rrc_x2ap_mobility_change_ack_t msg_resp;
    
    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_mobility_change_resp);
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_enb_ctx);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_Mobility Change Response] "
                "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Reset message */
    X2AP_MEMSET(&msg_resp, X2AP_NULL, sizeof(rrc_x2ap_mobility_change_ack_t));
    X2AP_MEMSET(asn_msg_buff, X2AP_NULL, sizeof(X2AP_MAX_ASN1_BUF_LEN));


    /* Fill message */
    /* For response message */ 
    /* Filling eNB1 cell id */
    msg_resp.enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_resp.enb1_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb1_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_resp.enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_resp.enb1_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb1_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);
    
    /* Filling eNB2 cell id */
    msg_resp.enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_resp.enb2_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_resp.enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_resp.enb2_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);

    /* Encode message */
    result = x2ap_internal_encode_mobility_change_ack(
            &asn1_ctx,
            asn_msg_buff,
            &asn_msg_len,
            &msg_resp); 
    
    if (X2AP_FAILURE == result)
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_Mobility change resp] Build failure");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
       return result;
    }
    peer_enb_msg.p_msg = asn_msg_buff;
    peer_enb_msg.msg_len = asn_msg_len;

    /* SPR_14436_Fix Start */
    peer_enb_msg.stream_id = p_enb_ctx->stream_info.
	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
    /* SPR_14436_Fix Stop */

    if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
    {
        /*Sending the X2AP Mobility Change Resp to Peer eNB via SCTP */
        result = x2ap_sctp_sm_entry
            (X2AP_SCTP_SEND_DATA_EV,
             (U8*)&peer_enb_msg,
             p_x2ap_gb_ctx,
             p_enb_ctx);
    }    
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  : x2ap_process_mobility_change_request_msg 
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the mobility change
 *                  request mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_process_mobility_change_request_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_mobility_setting_change_req_t  mobility_setting_change_req;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_MobilityChangeRequest  *p_x2ap_3gpp = X2AP_P_NULL;
    rrc_x2ap_mobility_change_req_t   x2ap_mobility_change_req;
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_Id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    U8 fail_cause = X2AP_NULL;
    x2ap_timer_t timer_id ={0};

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&error_indication,X2AP_NULL,sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&mobility_setting_change_req, X2AP_NULL, 
            sizeof(x2ap_mobility_setting_change_req_t));
    X2AP_MEMSET(&x2ap_mobility_change_req, X2AP_NULL, 
            sizeof(rrc_x2ap_mobility_change_req_t));

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    /*p_asn_msg_len = (U16*)&(p_peer_enb_msg->msg_len);*/
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
         X2AP_TRACE(X2AP_ERROR,
                "%s: ASN context initialization failed.",__FUNCTION__);
         return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);
    if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "decode failed.",__FUNCTION__);
        /* Send Transfer Syntax Error */
        x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    else
    {
        
       rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
    }

    if (T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
    {
        p_x2ap_3gpp = x2ap_pdu.u.initiatingMessage->value.u.mobilitySettingsChange; 
        if (X2AP_FAILURE ==
                x2ap_internal_decode_mobility_change_req(
                    &asn1_ctx,
                    p_x2ap_3gpp,
                    &x2ap_mobility_change_req,
                    p_x2ap_gb_ctx,
                    p_enb_ctx))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Mobility Change Request - "
                    "ASN decoding failed",__FUNCTION__);
            if (error_indication.cause.t == T_x2ap_Cause_protocol)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: X2AP Error Indication detected",__FUNCTION__);
                x2ap_build_and_send_mobility_change_failure(
                        p_x2ap_gb_ctx,
                        p_enb_ctx,
                        &error_indication.criticality_diagnostics,
                        (U8)error_indication.cause.u.protocol);
            }
            else
            {
                x2ap_build_and_send_mobility_change_failure(
                        p_x2ap_gb_ctx,
                        p_enb_ctx,
                        &error_indication.criticality_diagnostics,
                        fail_cause);
            }
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        else
        {
            /* validate served cell id*/
            if(X2AP_FAILURE == x2ap_validate_served_cell_id(
                        p_x2ap_gb_ctx,&x2ap_mobility_change_req))
            {
                X2AP_TRACE(X2AP_ERROR, "%s: Mobility change Request Message"
                        "validation failed",__FUNCTION__);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO, "%s: Mobility Change Request Message"
                        "validation Success",__FUNCTION__);
            }

            if (p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_MOBILITY_CHANGE_ACTIVE_ST)
            {
                x2ap_fill_rrm_mobility_change_request(
                        &mobility_setting_change_req,
                        &x2ap_mobility_change_req);
                
                X2AP_MEMCPY(&p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req.mobility_change_info, 
                &mobility_setting_change_req.mobility_change_info, sizeof(x2ap_mobility_change_info_t));

                X2AP_MEMCPY(&mobility_setting_change_req.enb1_gb_id, 
                        &p_enb_ctx->gb_enb_id,
                        sizeof(x2_gb_enb_id_t));
                X2AP_MEMCPY(&mobility_setting_change_req.enb2_gb_id, 
                        &p_x2ap_gb_ctx->global_enb_id,
                        sizeof(x2_gb_enb_id_t));

                if(p_x2ap_gb_ctx->is_son_present)
                {
                    dest_module_id = RRC_SON_MODULE_ID;
                }

                /* sending mobility change request to RRM/SON */
                if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_mobility_setting_change_req(
                            &mobility_setting_change_req,
                            RRC_X2AP_MODULE_ID,
                            dest_module_id,
                            trans_Id,
                            RRC_NULL))
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "Failed to send Mobility Change Request to RRM/SON.\n");
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return X2AP_FAILURE;
                }
                p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_W_MOBILITY_CHANGE_RRM_RESP_ST;    

                /* Start X2AP_MOBILITY_CH_RRM_GUARD_TIMER and save timer ID.*/
                if(X2AP_SUCCESS == x2ap_enb_mobility_change_start_timer(
                            X2AP_MOBILITY_CH_RRM_GUARD_TIMER,
                            p_enb_ctx,
                            p_x2ap_gb_ctx->config_param.timer_conf_info.
                            x2ap_rsu_rrm_guard_tim_val,
                            &timer_id))
                {
                    X2AP_TRACE(X2AP_INFO,
                            "X2AP_MOBILITY_CH_RRM_GUARD_TIMER:%p Started Successfully.\n",timer_id);

                    /* save the timer id in Mobility Change context */
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = timer_id;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,
                            "Starting X2AP_MOBILITY_CH_RRM_GUARD_TIMER failed.\n");
                }

            }
            else if(p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_PEER_RESP_ST)
            {
                X2AP_TRACE(X2AP_INFO, "X2AP MOBILITY CH sm state X2_W_MOBILITY_CHANGE_PEER_RESP_ST. "
                        "Mobility ch done, Sending MOBILITY CH FAILURE");
                /*sending failure response*/
                x2ap_build_and_send_mobility_change_failure(
                        p_x2ap_gb_ctx,
                        p_enb_ctx,
                        &error_indication.criticality_diagnostics,
                        fail_cause);
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "Invalid X2AP MOBILITY CHANGE SM state:%u.",
                        p_enb_ctx->x2ap_mobility_ch_sm_ctx.state);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "Invalid Message received.",__FUNCTION__);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    return X2AP_SUCCESS;
}

/****************************************************************************
 * Function Name  : x2ap_process_mobility_change_response_msg 
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx   -   Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the mobility change
 *                  response mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_process_mobility_change_response_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_mobility_change_resp_t  mobility_change_resp;
    x2ap_MobilityChangeAcknowledge  *p_x2ap_3gpp = X2AP_P_NULL;
    rrc_x2ap_mobility_change_ack_t  x2ap_mob_ch_ack;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_Id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U16 dest_module_id = RRC_RRM_MODULE_ID;

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    /*p_asn_msg_len = (U16*)&(p_peer_enb_msg->msg_len);*/
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    X2AP_MEMSET(&mobility_change_resp, X2AP_NULL, sizeof(x2ap_mobility_change_resp_t));
     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
         X2AP_TRACE(X2AP_ERROR,
                "%s: ASN context initialization failed.",__FUNCTION__);
         return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);
    if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "decode failed.",__FUNCTION__);
        /*error_indication_start*/
        /* Send Transfer Syntax Error */
        x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
        /*error_indication_end*/
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    else
    {
        
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
    }

    if (T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t)
    {
        p_x2ap_3gpp = x2ap_pdu.u.successfulOutcome->value.u.mobilitySettingsChange;
        if (X2AP_FAILURE ==
                x2ap_internal_decode_mobility_change_ack(
                    &asn1_ctx,
                    p_x2ap_3gpp,
                    &x2ap_mob_ch_ack,
                    p_x2ap_gb_ctx,
                    p_enb_ctx))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Mobility Change Response- "
                "ASN decoding failed",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        if(X2_W_MOBILITY_CHANGE_PEER_RESP_ST == p_enb_ctx->x2ap_mobility_ch_sm_ctx.state)
        {
            /* Stop MOBILITY CHANGE TIMER.*/
            x2ap_stop_timer(p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id);
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_P_NULL;

            /*Change state to X2_CELL_ACT_ACTIVE_ST.*/
             p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;

            x2ap_fill_rrm_mobility_change_response(
                    &mobility_change_resp,
                    &x2ap_mob_ch_ack);

            X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb1_gb_id,
                    &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t));
            X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb2_gb_id, 
                    &p_enb_ctx->gb_enb_id,
                    sizeof(x2_gb_enb_id_t));
            mobility_change_resp.mobility_change_resp.response = X2AP_SUCCESS;


            if(p_x2ap_gb_ctx->is_son_present)
            {
                dest_module_id = RRC_SON_MODULE_ID;
            }
            if(RRC_FAILURE == rrc_x2apCommon_send_x2ap_mobility_change_resp(
                        &mobility_change_resp,
                        RRC_X2AP_MODULE_ID,
                        dest_module_id,
                        trans_Id,
                        RRC_NULL))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "Failed to send Mobility Change Response to RRM/SON.\n");
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,
                        "Sending Mobility Change Response to RRM/SON Success.\n");
            }
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"Invalid X2AP MOBILITY SETTING CHANGE state:%u,"
                    " ignoring MOBILITY CHANGE RESPONSE",
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.state);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                "Invalid Message received.",__FUNCTION__);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    return X2AP_SUCCESS;
}
/****************************************************************************
 * Function Name  : x2ap_process_mobility_change_failure_msg 
 * Inputs         : p_msg_ptr -      Pointer to message buffer
 *                : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx  -    Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the Mobility setting
 *                  Change failure mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_process_mobility_change_failure_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_mobility_change_resp_t  mobility_change_resp;
    x2ap_MobilityChangeFailure   *p_x2ap_3gpp = X2AP_P_NULL;
    /*SPR 20523 START*/
    rrc_x2ap_mobility_change_fail_t x2ap_mob_change_fail = {X2AP_NULL};
    /*SPR 20523 END*/
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_Id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U16 dest_module_id = RRC_RRM_MODULE_ID;

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    /*p_asn_msg_len = (U16*)&(p_peer_enb_msg->msg_len);*/
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    X2AP_MEMSET(&mobility_change_resp, 0, sizeof(x2ap_mobility_change_resp_t));
     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
         X2AP_TRACE(X2AP_ERROR,
                "%s: ASN context initialization failed.",__FUNCTION__);
         return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);
    if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "decode failed.",__FUNCTION__);
        /* Send Transfer Syntax Error */
        x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    else
    {
        
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
    }

    if (T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t)
    {
        p_x2ap_3gpp = x2ap_pdu.u.unsuccessfulOutcome->value.u.mobilitySettingsChange;
        if (X2AP_FAILURE ==
                x2ap_decode_mobility_change_failure(
                    &asn1_ctx,
                    p_x2ap_3gpp,
                    &x2ap_mob_change_fail,
                    p_x2ap_gb_ctx,
                    p_enb_ctx))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Mobility Setting Change failure- "
                "ASN decoding failed",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        x2ap_fill_rrm_mobility_change_failure(
                &mobility_change_resp,
                &x2ap_mob_change_fail);

        if (p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_PEER_RESP_ST)
        {
            /*1. Stop X2AP_ENB_MOBILITY_CH_TIMER.*/
            x2ap_stop_timer(p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id);
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_P_NULL;

            X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb1_gb_id, 
                    &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t));
            X2AP_MEMCPY(&mobility_change_resp.mobility_change_resp.enb2_gb_id, 
                    &p_enb_ctx->gb_enb_id,
                    sizeof(x2_gb_enb_id_t));

            if(p_x2ap_gb_ctx->is_son_present)
            {
                dest_module_id = RRC_SON_MODULE_ID;
            }
            if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_mobility_change_resp(
                        &mobility_change_resp,
                        RRC_X2AP_MODULE_ID,
                        dest_module_id,
                        trans_Id,
                        RRC_NULL))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "Failed to send Mobility Change Response to RRM/SON.\n");
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            /*3. Change state to X2_MOBILITY_CHANGE_ACTIVE_ST.*/
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"Invalid MOBILITY CHANGE STATE state:%u, "
                    " ignoring MOBILITY CHANGE RESPONSE",
                    p_enb_ctx->x2ap_mobility_ch_sm_ctx.state);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                "Invalid Message received.",__FUNCTION__);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    return X2AP_SUCCESS;
}

/*******************************************************************************
 * Function Name : x2ap_internal_encode_mobility_change_fail
 * Inputs        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff - pointer to ASN.1 encoded message length 
 *                 p_mobilitity_change_fail - pointer to 
 *                                           rrc_x2ap_mobility_change_fail_t
 * Outputs      : p_buff (ASN.1 encoded message),
 *                p_buff_size (size of result buffer)
 * Returns      : X2AP_SUCCESS / X2AP_FAILURE    
 * Description  : This function used to encode Mobility Change Failure.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_mobility_change_fail 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_mobility_change_fail_t  *p_mobility_change_fail
)
{
    x2ap_return_et                  result = X2AP_FAILURE;
    x2ap_X2AP_PDU                   x2ap_pdu;
    x2ap_MobilityChangeFailure     *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_mobility_change_ack);
   
    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to unsuccessful outcome */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

        x2ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, x2ap_UnsuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.unsuccessfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode =
            ASN1V_x2ap_id_mobilitySettingsChange;
         
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to mobility change failure */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__mobilitySettingsChange;

        x2ap_pdu.u.unsuccessfulOutcome->value.u.mobilitySettingsChange = 
            rtxMemAllocType(pctxt,x2ap_MobilityChangeFailure);
        if (PNULL == x2ap_pdu.u.unsuccessfulOutcome->value.u.mobilitySettingsChange)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        p_x2ap_msg = x2ap_pdu.u.unsuccessfulOutcome->value.u.mobilitySettingsChange;
        
        asn1Init_x2ap_MobilityChangeFailure(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_MobilityChangeFailure_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_change_fail->enb1_cell_id))
        {
            break;
        }
        
        if (RT_OK != asn1Append_x2ap_MobilityChangeFailure_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_change_fail->enb2_cell_id))
        {
            break;
        }
        if (RT_OK != asn1Append_x2ap_MobilityChangeFailure_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_mobility_change_fail->cause))
        {
            break;
        }
        if(RRC_X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT & 
                p_mobility_change_fail->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_MobilityChangeFailure_protocolIEs_4(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_mobility_change_fail->mob_param_mod_range))
            {
                break;
            }
        }
        if(RRC_X2AP_MOB_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT & 
                p_mobility_change_fail->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_MobilityChangeFailure_protocolIEs_5(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_mobility_change_fail->criticality_diagnostics))
            {
                break;
            }
        }
        
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        /* SPR 20158 Fix Start */ 
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU: MOBILITY CHANGE FAILURE", &x2ap_pdu);
            *p_buff_size = (U16)pe_GetMsgLen(pctxt);
            result=X2AP_SUCCESS;
        }
        /* SPR 20158 Fix End */ 
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}


/*****************************************************************************
 * Function Name  : x2ap_fill_rrm_mobility_change_request 
 * Inputs         : p_x2ap_mobility_change_req - pointer to 
 *                                         x2ap_mobility_setting_change_req_t
 *                  p_msg - pointer to rrc_x2ap_mobility_change_req_t
 * Outputs        : p_x2ap_mobility_change_req 
 * Returns        : None 
 * Description    : This function fills mobility change request 
 ********************************************************************************/
void x2ap_fill_rrm_mobility_change_request(
                x2ap_mobility_setting_change_req_t *p_x2ap_mobility_change_req,
                rrc_x2ap_mobility_change_req_t  *p_msg)
{
    /* Filling eNB1 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_req->mobility_change_info.enb1_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb1_cell_id.pLMN_Identity.data,
            p_msg->enb1_cell_id.pLMN_Identity.numocts);

    X2AP_MEMCPY(p_x2ap_mobility_change_req->mobility_change_info.
            enb1_cell_id.eutran_cell_id,            
            p_msg->enb1_cell_id.eUTRANcellIdentifier.data,
            p_msg->enb1_cell_id.eUTRANcellIdentifier.numbits);
    
    /* Filling eNB2 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_req->mobility_change_info.enb2_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb2_cell_id.pLMN_Identity.data,
            p_msg->enb2_cell_id.pLMN_Identity.numocts);

    X2AP_MEMCPY(p_x2ap_mobility_change_req->mobility_change_info.
            enb2_cell_id.eutran_cell_id,            
            p_msg->enb2_cell_id.eUTRANcellIdentifier.data,
            p_msg->enb2_cell_id.eUTRANcellIdentifier.numbits);
    
    /* Filling eNB1 Mobility Parameters */
    if(p_x2ap_mobility_change_req->mobility_change_info.bitmask & X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {
        p_x2ap_mobility_change_req->mobility_change_info.enb1_mob_param.handover_trigger_change
            = p_msg->enb1_mobility_param.handoverTriggerChange;
        p_x2ap_mobility_change_req->mobility_change_info.bitmask |= X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT;
    }

    /* Filling eNB2 Proposed Mobility Parameters */
    p_x2ap_mobility_change_req->mobility_change_info.enb2_mob_param.handover_trigger_change
        = p_msg->enb2_proposed_mob_param.handoverTriggerChange; 
    
    /* Filling cause */
    x2ap_mobility_change_fill_cause(&p_msg->cause,
            &p_x2ap_mobility_change_req->mobility_change_info.cause);
}
/*****************************************************************************
 * Function Name  : x2ap_fill_rrm_mobility_change_response 
 * Inputs         : p_x2ap_mobility_change_resp - pointer to 
 *                                                 x2ap_mobility_change_resp_t
 *                  p_msg - pointer to rrc_x2ap_mobility_change_ack_t
 * Outputs        : p_x2ap_mobility_change_resp
 * Returns        : None
 * Description    : This function fills RRM mobility change responce
 ********************************************************************************/
void x2ap_fill_rrm_mobility_change_response(
        x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
        rrc_x2ap_mobility_change_ack_t  *p_msg)
{
    /* Filling eNB1 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.enb1_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb1_cell_id.pLMN_Identity.data,
            p_msg->enb1_cell_id.pLMN_Identity.numocts);

    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.
            enb1_cell_id.eutran_cell_id,            
            p_msg->enb1_cell_id.eUTRANcellIdentifier.data,
            p_msg->enb1_cell_id.eUTRANcellIdentifier.numbits);
    
    /* Filling eNB2 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.enb2_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb2_cell_id.pLMN_Identity.data,
            p_msg->enb2_cell_id.pLMN_Identity.numocts);

    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.
            enb2_cell_id.eutran_cell_id,            
            p_msg->enb2_cell_id.eUTRANcellIdentifier.data,
            p_msg->enb2_cell_id.eUTRANcellIdentifier.numbits);

    /*Filling criticality diagnostics */

}
/*******************************************************************************
 * Function Name : x2ap_parse_mobility_cause
 * Inputs        : p_cause - pointer to x2ap_Cause
 *                 p_Cause - pointer to x2ap_Cause
 *                 p_ie_order_map - pointer to x2ap_message_data_t
 *                 order_index - order index 
 *                 id - Id
 * Outputs       : p_cause
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to decode Mobility change cause.
 * Error         : This function report error to the caller function,                                                                                          If decoder fails to decode message.
*******************************************************************************/
x2ap_return_et  x2ap_parse_mobility_cause (x2ap_Cause    *p_cause,
                                  x2ap_Cause     *p_Cause,
                                  x2ap_message_data_t *p_ie_order_map,
                                  U8 order_index,
                                  U8 id)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_ENTER();

    p_cause->t = p_Cause->t;

    if (T_x2ap_Cause_radioNetwork == p_cause->t)
    {
        p_cause->u.radioNetwork =
           ((x2ap_Cause*)p_Cause)->u.radioNetwork;
    }
    else if (T_x2ap_Cause_transport == p_cause->t)
    {
        p_cause->u.transport =
           ((x2ap_Cause*)p_Cause)->u.transport;
    }
    else if (T_x2ap_Cause_protocol == p_cause->t)
    {
        p_cause->u.protocol =
           ((x2ap_Cause*)p_Cause)->u.protocol;
    }
    else if (T_x2ap_Cause_misc == p_cause->t)
    {
        p_cause->u.misc =
           ((x2ap_Cause*)p_Cause)->u.misc;
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"%s: ""Unknown cause type",
                   __FUNCTION__);
        x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
        retVal = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*******************************************************************************
 * Function Name : x2ap_parse_mobility_CriticalityDiagnostics
 * Inputs        : p_CriticalityDiagno - pointer to x2ap_crit_diagno_t
 *                 p_CriticalityDiagnostics - pointer to x2ap_CriticalityDiagnostics
 * Outputs       : p_CriticalityDiagno
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to decode Criticality Diagnostics
 * Error         : This function report error to the caller function, If decoder 
 *                 fails to decode message.
*******************************************************************************/
x2ap_return_et  x2ap_parse_mobility_CriticalityDiagnostics(
                      x2ap_crit_diagno_t            *p_CriticalityDiagno,
                      x2ap_CriticalityDiagnostics   *p_CriticalityDiagnostics)
{
    x2ap_return_et                   retVal = X2AP_SUCCESS;
    OSRTDListNode                    *p_cric_diagno_info = X2AP_P_NULL;
    U32                              count_cric_diag_info = 0;
    X2AP_UT_TRACE_ENTER();

    if (p_CriticalityDiagnostics->m.procedureCodePresent)
    {
        p_CriticalityDiagno->procedure_code =
                                p_CriticalityDiagnostics->procedureCode;
    }

    if (p_CriticalityDiagnostics->m.triggeringMessagePresent)
    {
        p_CriticalityDiagno->trigger_msg =
             (x2ap_trigg_msg_en)p_CriticalityDiagnostics->triggeringMessage;
    }

    if (p_CriticalityDiagnostics->m.procedureCriticalityPresent)
    {
        p_CriticalityDiagno->proc_critic =
             (x2ap_proc_critical_en)p_CriticalityDiagnostics->procedureCriticality; 
    }

    if (p_CriticalityDiagnostics->m.iEsCriticalityDiagnosticsPresent)
    {
        p_cric_diagno_info =
            p_CriticalityDiagnostics->iEsCriticalityDiagnostics.head;
        for (count_cric_diag_info = 0; count_cric_diag_info <
                p_CriticalityDiagnostics->iEsCriticalityDiagnostics.count;
                count_cric_diag_info++)
        {
            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
            ie_ctiticality = 
                  (x2ap_proc_critical_en)((x2ap_CriticalityDiagnostics_IE_List_element *)
                  p_cric_diagno_info->data)->iECriticality;

            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
            ie_id = ((x2ap_CriticalityDiagnostics_IE_List_element *)
                  p_cric_diagno_info->data)->iE_ID;

            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
            type_of_error = 
                  (x2ap_type_of_err_en)((x2ap_CriticalityDiagnostics_IE_List_element *)
                  p_cric_diagno_info->data)->typeOfError; 

            p_cric_diagno_info = p_cric_diagno_info->next;
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_mobility_change_fill_cause 
 * Inputs         : p_src - pointer to x2ap_Cause
 *                  p_dst - pointer to x2ap_Cause
 * Outputs        : p_dst
 * Returns        : None.
 * Description    : This function used to fill Mobility change cause in RRM message
*******************************************************************************/
void x2ap_mobility_change_fill_cause(x2ap_Cause* p_src, x2ap_cause_t* p_dst)
{
    if (T_x2ap_Cause_radioNetwork == p_src->t)
    {
        p_dst->type = T_x2ap_Cause_radioNetwork;
        p_dst->value = p_src->u.radioNetwork;
    }
    else if (T_x2ap_Cause_transport == p_src->t)
    {
        p_dst->type = T_x2ap_Cause_transport;
        p_dst->value = p_src->u.transport ;
    }
    else if (T_x2ap_Cause_protocol == p_src->t)
    {
        p_dst->type = T_x2ap_Cause_protocol;
        p_dst->value = p_src->u.protocol;
    }
    else if (T_x2ap_Cause_misc == p_src->t)
    {
        p_dst->type = T_x2ap_Cause_misc;
        p_dst->value = p_src->u.misc;
    }
}

/****************************************************************************
 * Function Name  : x2ap_decode_mobility_change_failure 
 * Inputs         : p_asn1_ctx - Pointer to ASN Context
 *                  p_3gpp - Pointer to x2ap_MobilityChangeFailure
 *                  p_intrl - Pointer to rrc_x2ap_mobility_change_fail_t
 *                  p_x2ap_gl_ctx - Pointer to X2AP global context
 *                  p_enb_ctx  -  Pointer to PEER eNB context 
 * Outputs        : p_intrl 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function used to decode Mobility Setting Change Failure
 ****************************************************************************/
x2ap_return_et x2ap_decode_mobility_change_failure 
(
 OSCTXT      *p_asn1_ctx,   /* Pointer to ASN Context */
 x2ap_MobilityChangeFailure *p_3gpp,    /* unpacked src */
 rrc_x2ap_mobility_change_fail_t *p_intrl, /* dest */
 x2ap_gb_context_t  *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t *p_enb_ctx
 )
{
    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_MobilityChangeFailure_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    x2ap_return_et result = X2AP_SUCCESS;

    x2ap_message_data_t message_map =
    {5, X2AP_FAILURE ,X2AP_FAILURE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ENB1_Cell_ID, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_ENB2_Cell_ID, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_ENB2_Mobility_Parameters_Modification_Range,
x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));
    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_MobilityChangeFailure_protocolIEs_element*)p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_ENB1_Cell_ID:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_1))
                    {
                        /*p_intrl->enb1_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_1;*/
                        p_intrl->enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_1->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb1_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_1->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);
                    }
                }
                break;
                case ASN1V_x2ap_id_ENB2_Cell_ID:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_2))
                    {
                        /*p_intrl->enb2_cell_id =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_2;*/
                        p_intrl->enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.pLMN_Identity.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_2->pLMN_Identity.data,
                                MAX_PLMN_ID_BYTES);

                        p_intrl->enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
                        X2AP_MEMCPY(p_intrl->enb2_cell_id.eUTRANcellIdentifier.data,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_2->eUTRANcellIdentifier.data,
                                HOME_ENB_ID_OCTET_SIZE);

                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_3))
                    {
                        if(X2AP_FAILURE == x2ap_parse_mobility_cause( 
                                    &(p_intrl->cause),
                                    p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_3,
                                    &message_map,
                                    (U8)index,
                                    (U8)p_protocol_IE->id))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s:x2ap_parse_cause failed",__FUNCTION__);
                            break;
                        }
                        /*p_intrl->cause =
                         *p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_3;*/
                    }
                }
                break;
                case ASN1V_x2ap_id_ENB2_Mobility_Parameters_Modification_Range:
                {  
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_4))
                    {
                        /*Bug_11766_start*/
                        /*Removing the if check*/
                        /*Bug_11766_start*/
                            p_intrl->mob_param_mod_range.handoverTriggerChangeLowerLimit =
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_4->handoverTriggerChangeLowerLimit;
                            p_intrl->mob_param_mod_range.handoverTriggerChangeUpperLimit =
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_4->handoverTriggerChangeUpperLimit;
                            p_intrl->bitmask |= RRC_X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT;
                    }
                }
                break;
                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    if(X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_5))
                    {
                        /*p_intrl->criticality_diagnostics = 
                         *p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_5;*/
                        /*result = x2ap_parse_CriticalityDiagnostics(
                          &(p_intrl->criticality_diagnostics),
                          p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_5);
                          result = x2ap_parse_mobility_CriticalityDiagnostics(
                          &(p_intrl->criticality_diagnostics),
                          p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_5);*/
                        x2ap_log_criticality_diagostics(p_protocol_IE->value.u._x2ap_MobilityChangeFailure_IEs_5);
                        p_intrl->bitmask |= RRC_X2AP_MOB_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT;
                    }
                }
                break;
                default:
                    X2AP_TRACE(X2AP_ERROR, "%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

					/*SPR_19067_FIX_START*/
                	if (x2ap_reject == p_protocol_IE->criticality) 
                	{
                    	result = X2AP_FAILURE;
                	}
                	/*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_mobilitySettingsChange,
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in"
                "Mobility setting change failure");

        result = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();

    return result;
}
/********************************************************************************
 * Function Name  : x2ap_fill_rrm_mobility_change_failure 
 * Inputs         : p_x2ap_mobility_change_resp - pointer to mobility change resp
 *                  p_msg - pointer to rrc_x2ap_mobility_change_fail_t
 * Outputs        : p_x2ap_mobility_change_resp,
 * Returns        : None
 * Description    : This function fills the moblity change failure in 
 *                  p_x2ap_mobility_change_resp
*********************************************************************************/
void x2ap_fill_rrm_mobility_change_failure(
        x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,
        rrc_x2ap_mobility_change_fail_t *p_msg)
{
    X2AP_UT_TRACE_ENTER();
    /* Filling eNB1 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.enb1_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb1_cell_id.pLMN_Identity.data,
            p_msg->enb1_cell_id.pLMN_Identity.numocts);

/*SPR 20523 START*/
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.
            enb1_cell_id.eutran_cell_id,            
            p_msg->enb1_cell_id.eUTRANcellIdentifier.data,
            HOME_ENB_ID_OCTET_SIZE);
/*SPR 20523 END*/
    
    /* Filling eNB2 cell id */
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.enb2_cell_id.
            plmn_identity.plmn_id,
            p_msg->enb2_cell_id.pLMN_Identity.data,
            p_msg->enb2_cell_id.pLMN_Identity.numocts);

/*SPR 20523 START*/
    X2AP_MEMCPY(p_x2ap_mobility_change_resp->mobility_change_resp.
            enb2_cell_id.eutran_cell_id,            
            p_msg->enb2_cell_id.eUTRANcellIdentifier.data,
            HOME_ENB_ID_OCTET_SIZE);
/*SPR 20523 END*/

    /* Filling cause */
    p_x2ap_mobility_change_resp->mobility_change_resp.bitmask |= 
        X2AP_MOBILITY_SETTING_CAUSE_PRESENT;
    x2ap_mobility_change_fill_cause(&p_msg->cause,
            &p_x2ap_mobility_change_resp->mobility_change_resp.cause);

    /*Filling eNB2 Mobility Parameters Modification Range*/
    if(p_msg->bitmask & RRC_X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {
        /*Bug_11766_start*/
        p_x2ap_mobility_change_resp->mobility_change_resp.bitmask |=
            X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT;
        /*Bug_11766_stop*/
        p_x2ap_mobility_change_resp->mobility_change_resp.enb2_mob_param_mod_range.
            ho_trigger_change_lower_limit = 
            p_msg->mob_param_mod_range.handoverTriggerChangeLowerLimit;
        p_x2ap_mobility_change_resp->mobility_change_resp.enb2_mob_param_mod_range.
            ho_trigger_change_upper_limit = 
            p_msg->mob_param_mod_range.handoverTriggerChangeUpperLimit;
        /*SPR 20523 START*/
        X2AP_TRACE(X2AP_INFO, "p_msg->mob_param_mod_range.handoverTriggerChangeUpperLimit = %d,"
                "p_msg->mob_param_mod_range.handoverTriggerChangeLowerLimit = %d",
                p_msg->mob_param_mod_range.handoverTriggerChangeUpperLimit,
                p_msg->mob_param_mod_range.handoverTriggerChangeLowerLimit);
        /*SPR 20523 END*/
    }
    
    p_x2ap_mobility_change_resp->mobility_change_resp.response = X2AP_FAILURE; 
    X2AP_UT_TRACE_EXIT();
}
/********************************************************************************
 * Function Name  : x2ap_enb_mobility_change_timer_expiry_evt_hdl 
 * Inputs         : p_timer_buf - pointer to x2ap_timer_buffer_t
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function handles timer expiry of Mobility Setting Change.
 *********************************************************************************/
x2ap_return_et x2ap_enb_mobility_change_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf, x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_return_et retVal = X2AP_SUCCESS;
	x2ap_peer_enb_context_t* p_peer_enb_cntx =X2AP_P_NULL;
	p_peer_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	X2AP_ASSERT(PNULL != p_peer_enb_cntx);
    
	if(p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_PEER_RESP_ST)
	{
		p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_NULL;

		if(p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.retry_count > 0)
		{
		    X2AP_TRACE(X2AP_INFO,"X2AP Mobility change retry count:%u.\n", 
                    p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.retry_count);
		    p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.retry_count--;
		    X2AP_TRACE(X2AP_INFO,"X2AP Mobility Change retry count left:%u.\n", 
                    p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.retry_count);

		    retVal = x2ap_enb_build_and_send_mobility_change_req(
                    p_x2ap_gb_ctx, p_peer_enb_cntx, 
                    &p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.mobility_change_req);
		}
		else
		{
			X2AP_TRACE(X2AP_INFO, "X2AP MOBILITY_CHANGE retry count exhausted\n");
			x2ap_send_mobility_change_response(p_x2ap_gb_ctx, 
                    &p_peer_enb_cntx->gb_enb_id, X2AP_FAILURE, 
                    p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.trans_id);
            p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state);
	}
    return retVal;
}
/***************************************************************************************
 * Function Name : x2ap_build_and_send_mobility_change_failure
 * Input         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                 p_enb_ctx - pointer to peer ENB context
 *                 p_criticality_diagnostics, - pointer to x2ap_CriticalityDiagnostics
 *                 cause - cause
 * Outputs       : None
 * Retuens       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function build nad send cell act failure to peer eNB.
 ****************************************************************************************/
x2ap_return_et  x2ap_build_and_send_mobility_change_failure(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
        U8 cause)
{
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    rrc_x2ap_mobility_change_fail_t msg_fail;
    x2ap_return_et result = X2AP_SUCCESS;
    OSCTXT asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                 "[x2ap_MobilityChangeRequest] "
                 "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));
    X2AP_MEMSET(&msg_fail, 0, sizeof(rrc_x2ap_mobility_change_fail_t));
    /* Filling eNB1 cell id */
    msg_fail.enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_fail.enb1_cell_id.pLMN_Identity.data,
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req.
            mobility_change_info.enb1_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_fail.enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_fail.enb1_cell_id.eUTRANcellIdentifier.data,
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req.
            mobility_change_info.enb1_cell_id.eutran_cell_id,
            sizeof(U32));
    
    /* Filling eNB2 cell id */
    msg_fail.enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_fail.enb2_cell_id.pLMN_Identity.data,
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req.
            mobility_change_info.enb2_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_fail.enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_fail.enb2_cell_id.eUTRANcellIdentifier.data,
            p_enb_ctx->x2ap_mobility_ch_sm_ctx.mobility_change_req.
            mobility_change_info.enb2_cell_id.eutran_cell_id,
            sizeof(U32));
    if (PNULL != p_criticality_diagnostics)
    {
        X2AP_TRACE(X2AP_INFO,"[%s]Filling Cause and Criticality Diagnostics",__FUNCTION__);
        
        msg_fail.cause.t = T_x2ap_Cause_protocol;
        msg_fail.cause.u.protocol = cause;
        
        msg_fail.criticality_diagnostics = *p_criticality_diagnostics;
        msg_fail.bitmask |= RRC_X2AP_MOB_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT;
    }
    else
    {
        msg_fail.cause.t = T_x2ap_Cause_radioNetwork;
        msg_fail.cause.u.radioNetwork = x2ap_unspecified_2;
    }

    result = x2ap_internal_encode_mobility_change_fail(
                    &asn1_ctx,
                    asn_msg_buff,
                    &asn_msg_len,
                    &msg_fail);
    if (X2AP_SUCCESS == result)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;

        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */

        if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
        {
            /*Sending the Mobility Change Failure to Peer eNB via SCTP */
            result = x2ap_sctp_sm_entry(
                    X2AP_SCTP_SEND_DATA_EV,
                    (U8*)&peer_enb_msg,
                    p_x2ap_gb_ctx,
                    p_enb_ctx);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_build_and_send_mobility_change_failure] Encode failed");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return X2AP_FAILURE;
    }
    /*Free ASN context*/
    rtFreeContext(&asn1_ctx);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*****************************************************************************
 * Function Name  : x2ap_stop_mobility_setting_change_procedure
 * Outputs        : None
 * Returns        : none
 * Description    : This function stops any ongoing mobility change procedure.
 ****************************************************************************/
void x2ap_stop_mobility_setting_change_procedure( 
        x2ap_peer_enb_context_t  *p_enb_ctx)
{
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_enb_ctx);

    if(p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_PEER_RESP_ST)
    {
        X2AP_TRACE(X2AP_INFO,"Mobility change procedure stop.\n");
        /* Stop Mobility Change Timer.*/
        x2ap_stop_timer(p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id);
        p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_P_NULL;

        /* Change state to X2_MOBILITY_CHANGE_IDLE_ST.*/
        p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_IDLE_ST; 

        /* Setting retry_count to 0 */
        p_enb_ctx->x2ap_mobility_ch_sm_ctx.retry_count = 0;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"No Mobility Change procedure on-going.\n");
    }

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * FUNCTION NAME: x2ap_enb_build_and_send_mobility_change_failure 
 * INPUTS       : p_x2ap_gb_ctx - pointer to X2AP global context
 *                p_enb_ctx - pointer to the peer enodeb context
 *                p_x2ap_mobility_change_resp - pointer to mobility change resp
 * Outputs      : None
 * Returns      : X2AP_SUCCESS or X2AP_FAILURE
 * Description  : This function builds and sends Mobility change Failure message
 *                to X2AP module.
 *******************************************************************************/
x2ap_return_et x2ap_enb_build_and_send_mobility_change_failure(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_change_resp
)
{
    x2ap_return_et result = X2AP_FAILURE;
    OSCTXT asn1_ctx;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;

    rrc_x2ap_mobility_change_fail_t msg_fail;
    
    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_mobility_change_resp);
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_enb_ctx);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_Mobility Change Failure] "
                "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Reset message */
    X2AP_MEMSET(&msg_fail, X2AP_NULL, sizeof(rrc_x2ap_mobility_change_fail_t));
    X2AP_MEMSET(asn_msg_buff, X2AP_NULL, sizeof(X2AP_MAX_ASN1_BUF_LEN));

    /* Filling eNB1 cell id */
    msg_fail.enb1_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_fail.enb1_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb1_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_fail.enb1_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_fail.enb1_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb1_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);
    
    /* Filling eNB2 cell id */
    msg_fail.enb2_cell_id.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(msg_fail.enb2_cell_id.pLMN_Identity.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_cell_id.plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);
    msg_fail.enb2_cell_id.eUTRANcellIdentifier.numbits = MAX_CELL_ID_BITS;
    X2AP_MEMCPY(msg_fail.enb2_cell_id.eUTRANcellIdentifier.data,
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_cell_id.eutran_cell_id,
            HOME_ENB_ID_OCTET_SIZE);
    /* Filling Cause */
    if(p_x2ap_mobility_change_resp->mobility_setting_change_resp.bitmask &
            X2AP_MOBILITY_SETTING_CAUSE_PRESENT)
    {
        x2ap_fill_mobility_setting_change_cause(
                p_x2ap_mobility_change_resp->mobility_setting_change_resp.cause,
                &msg_fail.cause);

    }
    /* Filling MobilityParametersModificationRange */
    if(p_x2ap_mobility_change_resp->mobility_setting_change_resp.bitmask & 
            X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {
        msg_fail.mob_param_mod_range.handoverTriggerChangeLowerLimit = 
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_mob_param_mod_range.ho_trigger_change_lower_limit;  

        msg_fail.mob_param_mod_range.handoverTriggerChangeUpperLimit = 
            p_x2ap_mobility_change_resp->mobility_setting_change_resp.
            enb2_mob_param_mod_range.ho_trigger_change_upper_limit; 
        
        msg_fail.bitmask |= RRC_X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT;
    }

    result = x2ap_internal_encode_mobility_change_fail(&asn1_ctx,
            asn_msg_buff,
            &asn_msg_len,
            &msg_fail); 
    if(X2AP_FAILURE == result)
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_Mobility change Failure] Encoding  failure");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
       return result;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,
                "[x2ap_Mobility change Failure] Encoding  Success");
    }
    peer_enb_msg.p_msg = asn_msg_buff;
    peer_enb_msg.msg_len = asn_msg_len;
    
    /* SPR_14436_Fix Start */
    peer_enb_msg.stream_id = p_enb_ctx->stream_info.
 	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
    /* SPR_14436_Fix Stop */

    if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
    {
        /*Sending the X2AP Mobility Change Failure to Peer eNB via SCTP */
        result = x2ap_sctp_sm_entry
            (X2AP_SCTP_SEND_DATA_EV,
             (U8*)&peer_enb_msg,
             p_x2ap_gb_ctx,
             p_enb_ctx);
    }    
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;
}
/*spr_fix*/
/********************************************************************************
 * Function Name  : x2ap_mobility_change_rrm_guard_timer_expiry_evt_hdl 
 * Inputs         : p_timer_buf - pointer to x2ap_timer_buffer_t
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function handles gaurd timer expiry event handler 
 *********************************************************************************/
x2ap_return_et x2ap_mobility_change_rrm_guard_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf, x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_return_et retVal = X2AP_SUCCESS;
	x2ap_peer_enb_context_t* p_peer_enb_cntx =X2AP_P_NULL;
	p_peer_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	X2AP_ASSERT(PNULL != p_peer_enb_cntx);
    
	if(p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state ==
            X2_W_MOBILITY_CHANGE_RRM_RESP_ST)
    {
        p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_NULL;

        X2AP_TRACE(X2AP_INFO, 
                "send x2ap_build_and_send_mobility_change_failure to eNB\n");
        x2ap_build_and_send_mobility_change_failure(
                p_x2ap_gb_ctx,
                p_peer_enb_cntx,
                X2AP_P_NULL,
                X2AP_NULL);
        p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_peer_enb_cntx->x2ap_mobility_ch_sm_ctx.state);
	}
    return retVal;
}
/*spr_fix*/
