
/*******************************************************************************
 *
 *  FILE NAME   : x2ap_reset_sm.c
 *
 *  DESCRIPTION : This file contains the X2AP RESET update state machine 
 *                and the state machine Entry function definition. 
 *
 *  DATE 04-September-2012  NAME Deepak Sah
 *                          REFERENCE  36.423_960(3GPP Spec) REASON Initial
 *
 *  Copyright 2010, Aricent Inc.
 *
 *******************************************************************************/
#include "x2ap_uecc.h"
#include "x2ap_rsu_sm.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "rrc_x2apCommon_intf.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_il_parser.h"
#include "rrc_logging.h"

x2ap_return_et x2ap_process_reset_response_msg(
		U8 *p_msg_ptr,
		x2ap_gb_context_t  *p_x2ap_gl_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_process_reset_request_msg(
		U8 *p_msg_ptr,
		x2ap_gb_context_t  *p_x2ap_gl_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_process_reset_req( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx); 

x2ap_return_et x2ap_enb_reset_timer_expiry_evt_hdl(
		x2ap_timer_buffer_t *p_timer_buf ,
		x2ap_gb_context_t *p_x2ap_gb_ctx);

void x2ap_send_reset_response(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2_gb_enb_id_t* p_peer_gb_enb_id,
		U8 response,
		U16 trans_id);

void x2ap_stop_resource_status_update_procedure(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

void x2ap_stop_x2_handover_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

void x2ap_send_reset_ind(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_peer_enb_cntx,
		x2ap_Cause cause);

void x2ap_reset_compose_asn_x2ap_cause(
		x2ap_cause_t cause,
		x2ap_Cause* p_asn_cause);

void x2ap_reset_parse_asn_x2ap_cause(
		x2ap_Cause asn_cause,
		x2ap_cause_t* p_cause);

void x2ap_stop_enb_configuration_update_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

void x2ap_stop_mobility_setting_ch_procedure(
		x2ap_peer_enb_context_t  *p_enb_ctx);

void x2ap_stop_reset_procedure( 
		x2ap_peer_enb_context_t  *p_enb_ctx);

void x2ap_stop_cell_activation_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

x2ap_return_et x2ap_internal_encode_reset_response_msg
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_reset_response_t  *p_x2ap_reset_response,
 U8          *p_ip_addr 
 );

x2ap_return_et x2ap_build_and_send_reset_response(
		x2ap_gb_context_t  *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx,
		x2ap_CriticalityDiagnostics *p_criticality_diagnostics
		);

x2ap_return_et x2ap_internal_encode_reset_msg
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_reset_req_t  *p_x2ap_reset_request,
 U8          *p_ip_addr
 );

x2ap_return_et x2ap_enb_build_and_send_reset_req(
		x2ap_gb_context_t  *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx,
		x2ap_reset_req_t *p_x2ap_reset_req
		);

x2ap_return_et
x2ap_enb_reset_start_timer
(
 x2ap_timer_id_en               timer_type,
 x2ap_peer_enb_context_t        *p_enb_ctx,
 U32                            timer_value,
 x2ap_timer_t                   *p_timer_id
 );

 #ifdef ENDC_ENABLED
 void x2ap_stop_endc_configuration_update_procedure(x2ap_peer_enb_context_t  *p_peer_enb_ctx);
 #endif
 
/*****************************************************************************
 * Function Name  : x2ap_process_reset_req 
 * Inputs         : p_api - pointer to input api data
 *                  p_x2ap_gb_ctx - pointer to x2AP global context
 * Outputs        : None
 * Returns        : X2AP_FAILURE / X2AP_SUCCESS
 * Description    : This function process the X2AP_RESET_REQ received from the rrm.
 ********************************************************************************/

x2ap_return_et x2ap_process_reset_req( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx) 
{
	x2ap_reset_req_t *p_x2ap_reset_req;
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U16 trans_id = X2AP_NULL;
	U8 counter = 0;
	x2ap_peer_enb_context_t  *p_enb_ctx = X2AP_P_NULL;


	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);

	p_x2ap_reset_req = (x2ap_reset_req_t*)
		x2ap_mem_get(sizeof(x2ap_reset_req_t));

	if(X2AP_P_NULL == p_x2ap_reset_req)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}

	/* Decode the received message from RRM */
	response = rrc_parse_x2ap_reset_req(
			p_x2ap_reset_req,
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
							&p_x2ap_reset_req->gb_enb_id, 
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
		else if(p_enb_ctx->x2ap_reset_sm_ctx.state == X2_RESET_ACTIVE_ST)
		{
			p_enb_ctx->x2ap_reset_sm_ctx.retry_count = 
				p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_enb_reset_retry_cnt;

			X2AP_TRACE(X2AP_INFO, "X2AP RESET retry count:%u", p_enb_ctx->x2ap_reset_sm_ctx.retry_count);

			/*CSR 51441 Start*/
			ALARM_MSG_L3(RRC_MODULE_ID, X2AP_RRM_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
			/*CSR 51441 Stop*/

			/* Stop X2 HANDOVER procedure */
			x2ap_stop_x2_handover_procedure(p_enb_ctx);

			/* Stop RESOURCE STATUS UPDATE procedure */
			x2ap_stop_resource_status_update_procedure(p_x2ap_gb_ctx, p_enb_ctx);

			/* Stop eNB CONFIGURATION UPDATE procedure */
			x2ap_stop_enb_configuration_update_procedure(p_enb_ctx);

			/* Stop endc CONFIGURATION UPDATE procedure */
			x2ap_stop_endc_configuration_update_procedure(p_enb_ctx);

			/* Stop Mobility Setting Change procedure */
			x2ap_stop_mobility_setting_ch_procedure(p_enb_ctx);

			/* Stop Cell Activation procedure */
			x2ap_stop_cell_activation_procedure(p_enb_ctx);

			/*E. Send X2AP RESET REQUEST to peer eNB.*/
			response = x2ap_enb_build_and_send_reset_req(p_x2ap_gb_ctx, p_enb_ctx, p_x2ap_reset_req);
			if (X2AP_SUCCESS == response)
			{
				/*save the transaction_ID in RESET context*/
				p_enb_ctx->x2ap_reset_sm_ctx.trans_id = trans_id;
			}
			else
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]: X2AP RESET REQUEST send failure",__FUNCTION__);
				response = X2AP_FAILURE;
			}
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "Invalid X2AP RESET SM state:%u, ignoring reset req.",p_enb_ctx->x2ap_reset_sm_ctx.state);
			response = X2AP_FAILURE;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "x2ap_reset_req decode failed");
		/* coverity_fix_63302_start */
		x2ap_mem_free((void *)p_x2ap_reset_req);
		/* coverity_fix_63302_stop */
		return X2AP_FAILURE;
	}

	if(response == X2AP_FAILURE)
	{
		x2ap_send_reset_response(p_x2ap_gb_ctx, &p_x2ap_reset_req->gb_enb_id, X2AP_FAILURE, trans_id);
	}
	/* coverity_fix_63302_start */
	x2ap_mem_free((void *)p_x2ap_reset_req);
	/* coverity_fix_63302_stop */
	return response;
}

/*****************************************************************************
 * Function Name  : x2ap_enb_reset_start_timer
 * Inputs         : timer_type    : Timer Type
 *                  p_enb_ctx - peer eNB context
 *                  timer_value   : Timer Value
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This start x2ap enb reset timer 
 *****************************************************************************/
x2ap_return_et 
x2ap_enb_reset_start_timer
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
	X2AP_MEMSET(&timer_buf, 0, sizeof(x2ap_timer_buffer_t));

	timer_buf.timer_type = timer_type;
	timer_buf.auto_delete = X2AP_TRUE;
	timer_buf.sd = p_enb_ctx->sctp_sd;
	timer_buf.p_timer_specific_data = (void*)p_enb_ctx;

	*p_timer_id = x2ap_start_timer(
			timer_value, &timer_buf,
			sizeof(timer_buf), X2AP_FALSE );

	X2AP_TRACE(X2AP_INFO,"[%s] Start X2AP ENB RESET Timer: %d Timer Value:%d",
			__FUNCTION__, timer_type, timer_value);
	X2AP_UT_TRACE_EXIT();
	return ret_val;
}

/******************************************************************************
 * Function Name : x2ap_enb_build_and_send_reset_req
 * Inputs        : p_x2ap_gb_ctx - pointer to global context
 *               : p_enb_ctx - pointer to peer eNB context
 *               : p_x2ap_reset_req - pointer to x2ap_reset_req_t
 * Output        : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function builds and sends X2AP RESET message
 *       to X2AP module.
 *******************************************************************************/

x2ap_return_et x2ap_enb_build_and_send_reset_req(
		x2ap_gb_context_t  *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx,
		x2ap_reset_req_t *p_x2ap_reset_req
		)
{
	x2ap_return_et result = X2AP_FAILURE;
	OSCTXT asn1_ctx;
	U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	U16    asn_msg_len = 0;

	rrc_x2ap_reset_req_t msg;

	x2ap_peer_enb_msg_t  peer_enb_msg;
	X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(PNULL != p_x2ap_reset_req);
	X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);

	/* Init ASN context */
	if (RT_OK != rtInitContext(&asn1_ctx))
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResetRequest] "
				"ASN context initialization failed");
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE;
	}
	/* Reset message */
	X2AP_MEMSET(&msg, 0, sizeof(msg));
	X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

	/* Fill message */
	/* Fill the cause */
	x2ap_reset_compose_asn_x2ap_cause(p_x2ap_reset_req->cause, &msg.cause);

	/* Save Reset request message for next retry */
	X2AP_MEMCPY(&p_enb_ctx->x2ap_reset_sm_ctx.reset_request_msg, p_x2ap_reset_req, sizeof(x2ap_reset_req_t));

	/* Encode message */
	result = x2ap_internal_encode_reset_msg(&asn1_ctx,
			asn_msg_buff,
			&asn_msg_len,
			&msg,
			p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);

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
            x2ap_timer_t timer_id ={0,};

            /*Sending the X2AP RESET REQUEST to Peer eNB via SCTP */
            result = x2ap_sctp_sm_entry(X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);

            if(result == X2AP_SUCCESS)
            {
                /*F. Start X2AP_ENB_RESET_TIMER and save timer ID.*/
                if (X2AP_SUCCESS == x2ap_enb_reset_start_timer(X2AP_ENB_RESET_TIMER,
                            p_enb_ctx,
                            p_x2ap_gb_ctx->config_param.timer_conf_info.
                            x2ap_enb_reset_guard_tim_val,
                            &timer_id))
                {
                    X2AP_TRACE(X2AP_INFO,"X2AP_ENB_RESET_TIMER:%p Started Successfully.\n",timer_id);

                    /*G. Change state to X2_RESET_W_RESET_RESP_ST.*/
                    p_enb_ctx->x2ap_reset_sm_ctx.state = X2_RESET_W_RESET_RESP_ST;

                    /* save the timer id in RESET context */
                    p_enb_ctx->x2ap_reset_sm_ctx.timer_id = timer_id;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"Starting X2AP_ENB_RESET_TIMER failed.\n");
                }
            }
        }
    }
	else
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResetRequest] Encode failure");
	}

	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	X2AP_UT_TRACE_EXIT();

	return result;
}
/*******************************************************************************
 * Function Name : x2ap_internal_encode_reset_msg
 * Inputs        : pctxt  - pointer to OSCTXT
 *                 p_buff -  ASN.1 encoded message
 *                 p_buff_size -  size of result buffe
 *                 p_x2ap_reset_request - pointer to rrc_x2ap_reset_req_t
 *                 p_ip_addr - ip address
 * Outputs       : p_buff (ASN.1 encoded message),
 *                p_buff_size (size of result buffer)
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE    
 * Description   : This function used to encode x2ap reset request.
 ********************************************************************************/
x2ap_return_et x2ap_internal_encode_reset_msg 
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_reset_req_t  *p_x2ap_reset_request,
 U8          *p_ip_addr
 )
{
	x2ap_return_et                   result = X2AP_FAILURE;
	OSRTDListNode       *p_node = PNULL;
	x2ap_X2AP_PDU                    x2ap_pdu;
	x2ap_ResetRequest  *p_ResetRequest = PNULL;
	x2ap_ResetRequest_protocolIEs_element* p_protocolIE_elem;   
	LOCAL_EVENT_X2_RESET_REQUEST_STRUCT *p_event_x2_reset_req = PNULL;
	EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
	U32                     encoded_msg_len = 0;
	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != pctxt);
	X2AP_ASSERT(PNULL != p_buff);
	X2AP_ASSERT(PNULL != p_buff_size);
	X2AP_ASSERT(PNULL != p_x2ap_reset_request);

	do
	{
		/* Init X2AP PDU */
		asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
		/* Set Pdu type to initiating Message */
		x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

		x2ap_pdu.u.initiatingMessage =
			rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
		if (PNULL == x2ap_pdu.u.initiatingMessage)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

		x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_reset; 

		x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

		/* Set the initiating message type to resource status failure */
		x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__reset; 

		x2ap_pdu.u.initiatingMessage->value.u.reset = rtxMemAllocType(pctxt,x2ap_ResetRequest);
		if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.reset)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		p_ResetRequest = x2ap_pdu.u.initiatingMessage->value.u.reset;

		asn1Init_x2ap_ResetRequest(p_ResetRequest);
		rtxDListAllocNodeAndData(pctxt,                                         
				x2ap_ResetRequest_protocolIEs_element,
				&p_node,
				&p_protocolIE_elem);
		if (X2AP_P_NULL == p_node)
		{                                                                         
			/* Not enough memory */                                               
			X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
			break;                                                              
		}

		asn1Init_x2ap_ResetRequest_protocolIEs_element(p_protocolIE_elem);

		/* Fill protocol IE Id and criticality and Value */                                   
		p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
		p_protocolIE_elem->criticality = x2ap_ignore;

		p_protocolIE_elem->value.u._x2ap_ResetRequest_IEs_1 =
			rtxMemAllocType(pctxt, x2ap_Cause);
		if (PNULL == p_protocolIE_elem->value.u._x2ap_ResetRequest_IEs_1)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		asn1Init_x2ap_Cause(p_protocolIE_elem->value.u._x2ap_ResetRequest_IEs_1);
		p_protocolIE_elem->value.t = T22x2ap___x2ap_ResetRequest_IEs_1;

		X2AP_MEMCPY(p_protocolIE_elem->value.u._x2ap_ResetRequest_IEs_1,&p_x2ap_reset_request->cause, sizeof(x2ap_Cause));

		rtxDListAppendNode(&p_ResetRequest->protocolIEs, p_node);

		/* ASN Encode message */
		pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);


		if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
			break;
		}
		else
		{
			/* generating Protocol Event LOCAL_RESET_REQUEST*/ 
			encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

			/* Print the asn encoded message */
	
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : RESET REQUEST", &x2ap_pdu);

			p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
					encoded_msg_len);
			if (PNULL != p_event_header)
			{
				x2ap_fill_protocol_event_params(p_event_header,
						encoded_msg_len, LOCAL_RESET_REQUEST,
						MESSAGE_DIRECTION_SEND);

				l3_memcpy_wrapper((void*)((U8*)p_event_header + 
							sizeof(EVENT_EXTERNAL_HEADER)),
						(const void*)p_buff,
						encoded_msg_len);

				rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
			}

			/* generating Protocol Event LOCAL_EVENT_X2_RESET_REQUEST*/ 
			p_event_x2_reset_req = rrc_mem_get(sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT));
			if (PNULL != p_event_x2_reset_req)
			{
				memset_wrapper(p_event_x2_reset_req, RRC_NULL, sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT));
				p_event_x2_reset_req->header.length = sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT);
				p_event_x2_reset_req->header.event_id = LOCAL_EVENT_X2_RESET_REQUEST;

				p_event_x2_reset_req->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
				p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE_GROUP = p_x2ap_reset_request->cause.t;

				switch(p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE_GROUP)
				{ 
					case T_x2ap_Cause_radioNetwork: 
						p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
							p_x2ap_reset_request->cause.u.radioNetwork;
						break;

					case T_x2ap_Cause_transport: 
						p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
							p_x2ap_reset_request->cause.u.transport;
						break;

					case T_x2ap_Cause_protocol:
						p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
							p_x2ap_reset_request->cause.u.protocol;
						break; 

					case T_x2ap_Cause_misc:
						p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
							p_x2ap_reset_request->cause.u.misc;
						break; 
				}

				l3_memcpy_wrapper(p_event_x2_reset_req->EVENT_PARAM_ENB_IP_ADDRESS,p_ip_addr,
						MAX_X2AP_IP_ADDRESS_LENGTH);
				rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_reset_req);
			}
		}

		*p_buff_size = (U16)pe_GetMsgLen(pctxt);


		result=X2AP_SUCCESS;
	} while(0);
	X2AP_UT_TRACE_EXIT();
	return result;
}
/****************************************************************************
 * Function Name  : x2ap_process_reset_request_msg
 * Inputs         : p_msg_ptr     - Pointer to message buffer
 *                : p_x2ap_gl_ctx - pointer to X2AP global context
 *                : p_enb_ctx     - Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the reset request
 *                  mesaage received from peer enb.
 ****************************************************************************/
x2ap_return_et x2ap_process_reset_request_msg(
		U8 *p_msg_ptr,
		x2ap_gb_context_t  *p_x2ap_gl_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx)
{
	/* coverity_fix_54722_start */
	x2ap_ResetRequest  *p_x2ap_reset = X2AP_P_NULL;

	U32  index = X2AP_NULL;
	OSRTDListNode  *p_node     = PNULL;
	x2ap_ResetRequest_protocolIEs_element *p_protocol_IE = PNULL;

	x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
	U8                     *p_asn_msg;
	U32                    *p_asn_msg_len;
	x2ap_X2AP_PDU          x2ap_pdu;
	OSCTXT                 asn1_ctx;
	x2ap_Cause cause;
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	U16 ie_list_index = 0;
	x2ap_message_data_t message_map =
	{1, X2AP_TRUE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL}
		}};

	LOCAL_EVENT_X2_RESET_REQUEST_STRUCT *p_event_x2_reset_req = PNULL;
	EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
	U32                     encoded_msg_len = 0;
	X2AP_UT_TRACE_ENTER();

	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

	p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;

	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_MEMSET(&cause, 0, sizeof(x2ap_Cause));
	X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));
	X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));

	p_asn_msg = (p_peer_enb_msg)->p_msg;
	p_asn_msg_len = &(p_peer_enb_msg->msg_len);

	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
				"context initialization failed.",__FUNCTION__);
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
		x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
		return X2AP_FAILURE;
	}

	if (T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
	{
		/* generating Protocol Event LOCAL_RESET_REQUEST*/ 
		encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

		
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : RESET REQUEST", &x2ap_pdu);

		p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
				encoded_msg_len);
		if (PNULL != p_event_header)
		{
			x2ap_fill_protocol_event_params(p_event_header,
					encoded_msg_len, LOCAL_RESET_REQUEST,
					MESSAGE_DIRECTION_RECEIVED);

			l3_memcpy_wrapper((void*)((U8*)p_event_header + 
						sizeof(EVENT_EXTERNAL_HEADER)),
					(const void*)p_asn_msg,
					encoded_msg_len);

			rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
		}

		/* generating Protocol Event LOCAL_EVENT_X2_RESET_REQUEST*/ 
		p_event_x2_reset_req = rrc_mem_get(sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT));
		if (PNULL != p_event_x2_reset_req)
		{
			memset_wrapper(p_event_x2_reset_req, RRC_NULL, sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT));
			p_event_x2_reset_req->header.length = sizeof(LOCAL_EVENT_X2_RESET_REQUEST_STRUCT);
			p_event_x2_reset_req->header.event_id = LOCAL_EVENT_X2_RESET_REQUEST;

			p_event_x2_reset_req->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_RECEIVED;
			p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE_GROUP = cause.t;

			switch(p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE_GROUP)
			{ 
				case T_x2ap_Cause_radioNetwork: 
					p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
						cause.u.radioNetwork;
					break;

				case T_x2ap_Cause_transport: 
					p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
						cause.u.transport;
					break;

				case T_x2ap_Cause_protocol:
					p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
						cause.u.protocol;
					break; 

				case T_x2ap_Cause_misc:
					p_event_x2_reset_req->EVENT_PARAM_3GPP_CAUSE = 
						cause.u.misc;
					break; 
			}

			l3_memcpy_wrapper(p_event_x2_reset_req->EVENT_PARAM_ENB_IP_ADDRESS, p_enb_ctx->
					enb_comm_info.ip_addr[0].ip_addr,
					MAX_X2AP_IP_ADDRESS_LENGTH);
			rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_reset_req);
		}

		p_x2ap_reset = x2ap_pdu.u.initiatingMessage->value.u.reset;

		do
		{
			p_node = p_x2ap_reset->protocolIEs.head;
			/* Save the decoded Protocol IEs */
			for (index =0; index < p_x2ap_reset->protocolIEs.count; index++)
			{
				if (!p_node)
				{
					X2AP_TRACE(X2AP_ERROR,"%s: next node returned NULL",__FUNCTION__);
					break;
				}
				p_protocol_IE = (x2ap_ResetRequest_protocolIEs_element *)p_node->data;
				if (ASN1V_x2ap_id_Cause == p_protocol_IE->id)
				{
					if(X2AP_SUCCESS == x2ap_validate_ie_value(
								&message_map,
								index,
								p_protocol_IE->id,
								p_protocol_IE->value.u._x2ap_ResetRequest_IEs_1))
					{
						X2AP_MEMCPY(&cause,p_protocol_IE->value.u._x2ap_ResetRequest_IEs_1,sizeof(x2ap_Cause));
					}
				}
				else
				{
					X2AP_TRACE(X2AP_ERROR,"%s:Invalid protocol ID recieved", __FUNCTION__);
					x2ap_add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							&send_err_ind,
							X2AP_FALSE,
							X2AP_FALSE);
					/*return X2AP_FAILURE;*/
				}

				p_node= p_node->next;
			}
		}while(0);
		/* Parse the map for Error Indication */
		if ((X2AP_SUCCESS == x2ap_parse_message_map(
						&asn1_ctx,
						p_x2ap_gl_ctx,
						p_enb_ctx,
						&message_map,
						&iE_list,
						&ie_list_index,
						&send_err_ind,
						ASN1V_x2ap_id_reset,
						T_x2ap_X2AP_PDU_initiatingMessage,
						x2ap_reject,
						(rrc_x2ap_error_indication_t *)PNULL)))
		{
			X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in Reset_Request_msg");
		}

		if(p_enb_ctx->x2ap_reset_sm_ctx.state == X2_RESET_ACTIVE_ST)
		{
			/* Stop X2 HANDOVER procedure */
			x2ap_stop_x2_handover_procedure(p_enb_ctx);

			/* Stop RESOURCE STATUS UPDATE procedure */
			x2ap_stop_resource_status_update_procedure(p_x2ap_gl_ctx, p_enb_ctx);

			/* Stop eNB CONFIGURATION UPDATE procedure */
			x2ap_stop_enb_configuration_update_procedure(p_enb_ctx);

			/*Build and send Send reset indication to RRM.*/
			x2ap_send_reset_ind(p_x2ap_gl_ctx, p_enb_ctx, cause);
			/*CSR 51441 Start*/
			ALARM_MSG_L3(RRC_MODULE_ID, X2AP_PEER_ENB_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
			/*CSR 51441 Stop*/

		}
		else if (p_enb_ctx->x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST)
		{
		    X2AP_TRACE(X2AP_INFO, "X2AP RESET SM state X2_RESET_W_RESET_RESP_ST."
			    "Reset X2 interface done, Sending RESET RESPONSE");
		    /* SPR 14394  Fix Start */
		    /*1. Stop X2AP_ENB_RESET_TIMER.*/
		    x2ap_stop_timer(p_enb_ctx->x2ap_reset_sm_ctx.timer_id);
		    p_enb_ctx->x2ap_reset_sm_ctx.timer_id = X2AP_P_NULL;

		    /*2. Send X2AP_RRM_RESET_RES to RRM.*/
		    x2ap_send_reset_response(p_x2ap_gl_ctx, &p_enb_ctx->gb_enb_id, X2AP_FAILURE, p_enb_ctx->x2ap_reset_sm_ctx.trans_id);
		    /*SPR 14394 Fix End */
		    p_enb_ctx->x2ap_reset_sm_ctx.state = X2_RESET_ACTIVE_ST;
		}
		else
		{
		    X2AP_TRACE(X2AP_ERROR, "Invalid X2AP RESET SM state:%u.",p_enb_ctx->x2ap_reset_sm_ctx.state);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
		    return X2AP_FAILURE;
		}

		/*Build and send RESET RESPONSE to peer enb*/
		if (X2AP_FAILURE == x2ap_build_and_send_reset_response(
			    p_x2ap_gl_ctx,p_enb_ctx,X2AP_P_NULL)) 
		{
		    X2AP_TRACE(X2AP_ERROR,"[%s]: Failed to send reset Reset response to peer eNB.",
			    __FUNCTION__);
               /*SPR_18125_START*/
               rtFreeContext(&asn1_ctx);
               /*SPR_18125_END*/
		    return X2AP_FAILURE;
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

	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	return X2AP_SUCCESS;
}
/* coverity_fix_54722_stop */

/******************************************************************************
 * Function Name : x2ap_build_and_send_reset_response
 * INPUTS        : p_x2ap_gb_ctx - pointer to global context
 *                 p_enb_ctx - peer eNB context
 *                 p_criticality_diagnostics - pointer to criticality diagnostics
 * Outputs       : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Descrition    : This function builds and sends X2AP RESET message
 *       to X2AP module.
 *******************************************************************************/

x2ap_return_et x2ap_build_and_send_reset_response(
	x2ap_gb_context_t  *p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t *p_enb_ctx,
	x2ap_CriticalityDiagnostics *p_criticality_diagnostics
	)
{
    x2ap_return_et result = X2AP_FAILURE;
    OSCTXT asn1_ctx;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;

    rrc_x2ap_reset_response_t msg;

    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_enb_ctx);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResetRequest] "
		"ASN context initialization failed");
	X2AP_UT_TRACE_EXIT();
	return X2AP_FAILURE;
    }
    /* Reset message */
    X2AP_MEMSET(&msg, 0, sizeof(msg));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

    /* Fill message */
    /*As of now p_criticality_diagnostics is NULL*/
    if (PNULL != p_criticality_diagnostics)
    {
	msg.criticality_diagnostics = *p_criticality_diagnostics;
	msg.bitmask |= X2AP_RESET_RESPONSE_CRIT_DIAGNO_PRESENT;
    }

    /* Encode message */
    result = x2ap_internal_encode_reset_response_msg(&asn1_ctx,
	    asn_msg_buff,
	    &asn_msg_len,
	    &msg,
	    p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);

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
	    /*Sending the X2AP RESET REQUEST to Peer eNB via SCTP */
	    result = x2ap_sctp_sm_entry
		(X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);
	}    
    }    
    else
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResetResponse] Encode failure");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;
}
/*******************************************************************************
 * Function Name : x2ap_internal_encode_reset_response_msg
 * Inputs        : pctxt - pointer to OSCTXT
 *                 p_buff - ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_x2ap_reset_response - pointer to rrc_x2ap_reset_response_t
 *                 p_ip_addr - ip address
 * Outputs       : p_buff (ASN.1 encoded message),
 *                p_buff_size (size of result buffer)
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to encode x2ap reset response.
 ********************************************************************************/
x2ap_return_et x2ap_internal_encode_reset_response_msg 
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_reset_response_t  *p_x2ap_reset_response,
 U8          *p_ip_addr
 )
{
    x2ap_return_et                   result = X2AP_FAILURE;
    OSRTDListNode       *p_node = PNULL;
    x2ap_X2AP_PDU                    x2ap_pdu;
    x2ap_ResetResponse  *p_ResetResponse = PNULL;
    x2ap_ResetResponse_protocolIEs_element* p_protocolIE_elem;  
    LOCAL_RESET_RESPONSE_STRUCT     *p_local_x2_reset_resp = PNULL;
    U32                             encoded_msg_len = 0;
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_x2ap_reset_response);

    do
    {
	/* Init X2AP PDU */
	asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
	/* Set Pdu type to initiating Message */
	x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

	x2ap_pdu.u.successfulOutcome =
	    rtxMemAllocType(pctxt, x2ap_SuccessfulOutcome);
	if (PNULL == x2ap_pdu.u.successfulOutcome)
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
	    break;
	}

	asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

	x2ap_pdu.u.successfulOutcome->procedureCode = ASN1V_x2ap_id_reset; 

	x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;

	/* Set the initiating message type to resource status failure */
	x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__reset; 

	p_ResetResponse = rtxMemAllocType(pctxt,x2ap_ResetResponse);
	if (PNULL == p_ResetResponse)
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
	    break;
	}

	asn1Init_x2ap_ResetResponse(p_ResetResponse);
	x2ap_pdu.u.successfulOutcome->value.u.reset = p_ResetResponse;

	if(p_x2ap_reset_response->bitmask)
	{
	    rtxDListAllocNodeAndData(pctxt,                                         
		    x2ap_ResetResponse_protocolIEs_element,
		    &p_node,
		    &p_protocolIE_elem);
	    if (X2AP_P_NULL == p_node)
	    {                                                                         
		/* Not enough memory */                                               
		X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
		break;                                                              
	    }

	    asn1Init_x2ap_ResetResponse_protocolIEs_element(p_protocolIE_elem);

	    /* Fill protocol IE Id and criticality and Value */                                   
	    p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
	    p_protocolIE_elem->criticality = x2ap_ignore;

	    p_protocolIE_elem->value.u._x2ap_ResetResponse_IEs_1 = 
		rtxMemAllocType(pctxt,x2ap_CriticalityDiagnostics) ;

	    if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ResetResponse_IEs_1)
	    {
		X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
		break;
	    }
	    asn1Init_x2ap_CriticalityDiagnostics(p_protocolIE_elem->value.u._x2ap_ResetResponse_IEs_1 );

	    p_protocolIE_elem->value.u._x2ap_ResetResponse_IEs_1 = &p_x2ap_reset_response->criticality_diagnostics;
	    p_protocolIE_elem->value.t =  T23x2ap___x2ap_ResetResponse_IEs_1;

	    rtxDListAppendNode(&p_ResetResponse->protocolIEs, p_node);
	}

	/* ASN Encode message */
	pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);


	if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
	    break;
	}
	else
	{
	    /* Generating Protocol Event LOCAL_RESET_RESPONSE*/
	    encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

	    /* Print the asn encoded message */
	    
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : RESET REQUEST", &x2ap_pdu);

	    p_local_x2_reset_resp = rrc_mem_get(sizeof(LOCAL_RESET_RESPONSE_STRUCT) + 
		    encoded_msg_len);
	    if (PNULL != p_local_x2_reset_resp)
	    {
		memset_wrapper(p_local_x2_reset_resp, RRC_NULL, 
			(sizeof(LOCAL_RESET_RESPONSE_STRUCT) + encoded_msg_len));

		p_local_x2_reset_resp->header.length = sizeof(LOCAL_RESET_RESPONSE_STRUCT) + 
		    encoded_msg_len;
		p_local_x2_reset_resp->header.event_id = LOCAL_RESET_RESPONSE;

		p_local_x2_reset_resp->EVENT_PARAM_MESSAGE_DIRECTION = 
		    MESSAGE_DIRECTION_SEND;
		p_local_x2_reset_resp->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

		l3_memcpy_wrapper(p_local_x2_reset_resp->EVENT_PARAM_ENB_IP_ADDRESS,p_ip_addr,
			MAX_X2AP_IP_ADDRESS_LENGTH); 

		l3_memcpy_wrapper((void *)((U8*)(p_local_x2_reset_resp) + 
			    sizeof(LOCAL_RESET_RESPONSE_STRUCT)), 
			(const void*)p_buff,
			encoded_msg_len);

		rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_x2_reset_resp);
	    }
	}

	*p_buff_size = (U16)pe_GetMsgLen(pctxt);

	result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/****************************************************************************
 * Function Name  : x2ap_process_reset_response_msg
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx-  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the reset response
 *                  mesaage received from peer enb.
 ****************************************************************************/
x2ap_return_et x2ap_process_reset_response_msg(
	U8 *p_msg_ptr,
	x2ap_gb_context_t  *p_x2ap_gl_ctx,
	x2ap_peer_enb_context_t *p_enb_ctx)
{
    /* coverity_fix_54723_start */ 
    x2ap_ResetResponse  *p_x2ap_reset_response = X2AP_P_NULL;
    U8 response = X2AP_SUCCESS;

    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    LOCAL_RESET_RESPONSE_STRUCT     *p_local_x2_reset_resp = PNULL;
    U32                             encoded_msg_len = 0;

    x2ap_reset_resp_t  x2ap_reset_response;
    X2AP_MEMSET(&x2ap_reset_response, 0, sizeof(x2ap_reset_resp_t));

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;

    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);

    p_asn_msg = (p_peer_enb_msg)->p_msg;
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
	/* Drop message */
	X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
		"context initialization failed.",__FUNCTION__);
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
	x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
	return X2AP_FAILURE;
    }

    if (T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t)
    {
	/* Generating Protocol Event LOCAL_RESET_RESPONSE*/
	encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

	
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

	p_local_x2_reset_resp = rrc_mem_get(sizeof(LOCAL_RESET_RESPONSE_STRUCT)+ 
		encoded_msg_len);
	if (PNULL != p_local_x2_reset_resp)
	{
	    memset_wrapper(p_local_x2_reset_resp, RRC_NULL, 
		    (sizeof(LOCAL_RESET_RESPONSE_STRUCT) + encoded_msg_len));

	    p_local_x2_reset_resp->header.length = sizeof(LOCAL_RESET_RESPONSE_STRUCT) + 
		encoded_msg_len;
	    p_local_x2_reset_resp->header.event_id = LOCAL_RESET_RESPONSE;

	    p_local_x2_reset_resp->EVENT_PARAM_MESSAGE_DIRECTION = 
		MESSAGE_DIRECTION_RECEIVED;
	    p_local_x2_reset_resp->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

	    l3_memcpy_wrapper((void *)(((U8*)p_local_x2_reset_resp) + 
			sizeof(LOCAL_RESET_RESPONSE_STRUCT)), 
		    (const void*)p_asn_msg,
		    encoded_msg_len);

	    l3_memcpy_wrapper(&p_local_x2_reset_resp->EVENT_PARAM_ENB_IP_ADDRESS, p_enb_ctx->
		    enb_comm_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);

	    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_x2_reset_resp);
	}

	p_x2ap_reset_response = x2ap_pdu.u.successfulOutcome->value.u.reset;

	if(p_x2ap_reset_response->protocolIEs.count)
	{
	    response = X2AP_FAILURE;
	    X2AP_TRACE(X2AP_INFO,"Criticality diagnostics present.");
	}
	else
	{
	    X2AP_TRACE(X2AP_INFO,"Criticality diagnostics not present.");
	}

	/*Check the state of Reset SM*/
	if(p_enb_ctx->x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST )
	{
	    /*1. Stop X2AP_ENB_RESET_TIMER.*/
	    x2ap_stop_timer(p_enb_ctx->x2ap_reset_sm_ctx.timer_id);
	    p_enb_ctx->x2ap_reset_sm_ctx.timer_id = X2AP_P_NULL; 

	    /*2. Send X2AP_RRM_RESET_RES to RRM.*/
	    x2ap_send_reset_response(p_x2ap_gl_ctx, &p_enb_ctx->gb_enb_id, response, p_enb_ctx->x2ap_reset_sm_ctx.trans_id);

	    /*3. Change state to X2_RESET_ACTIVE_ST.*/
	    p_enb_ctx->x2ap_reset_sm_ctx.state = X2_RESET_ACTIVE_ST;
	} 
	else
	{
	    X2AP_TRACE(X2AP_WARNING,"Invalid X2AP RESET SM state:%u, ignoring RESET RESPONSE", 
		    p_enb_ctx->x2ap_reset_sm_ctx.state);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
	    return X2AP_FAILURE;
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

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    return X2AP_SUCCESS;
}
/* coverity_fix_54723_stop */ 

/*****************************************************************************
 * Function Name  : x2ap_stop_enb_configuration_update_procedure 
 * Inputs         : p_peer_enb_ctx - pointer to peer eNb context
 * Outputs        : None
 * Returns        : None
 * Description    : This function stops enb configuration update procedure
 ********************************************************************************/
void x2ap_stop_enb_configuration_update_procedure(x2ap_peer_enb_context_t  *p_peer_enb_ctx)
{
    switch(p_peer_enb_ctx->enb_config_sm_ctx.state)
    {
	case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP eNB CONFIGURATION UPDATE Ongoing ...\n"
			"Current state: X2AP_ENB_CONFIG_W_CONF_RESP_ST, \n"
			" Stopping X2AP_ENB_CONFIG_UPDATE_RSP_TIMER:%p.\n",
			p_peer_enb_ctx->enb_config_sm_ctx.req_timer_id);
		x2ap_stop_timer(p_peer_enb_ctx->enb_config_sm_ctx.req_timer_id);
		p_peer_enb_ctx->enb_config_sm_ctx.req_timer_id = X2AP_P_NULL;
		p_peer_enb_ctx->enb_config_sm_ctx.req_retry_count =0;
		p_peer_enb_ctx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
	    }break;
	case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP eNB CONFIGURATION UPDATE Ongoing ...\n"
			"Current state: X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST, \n"
			" Stopping X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER:%p.\n",
			p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_timer_id);
		x2ap_stop_timer(p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_timer_id);
		p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_timer_id = X2AP_P_NULL;
		p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_retry_count =0;
		p_peer_enb_ctx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
	    }break;
	default:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP eNB CONFIGURATION UPDATE not ongoing ...\n");
	    }
	    break;
    }
}
/*****************************************************************************
 * Function Name  : x2ap_stop_cell_activation_procedure
 * Inputs         : p_enb_ctx     - Pointer to peer eNB context
 * Outputs        : None
 * Returns        : none
 * Description    : This function stops any ongoinf reset procesure.
 ****************************************************************************/
void x2ap_stop_cell_activation_procedure( 
	x2ap_peer_enb_context_t  *p_enb_ctx)
{
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_enb_ctx);

    if((p_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_W_PEER_ENB_RESP_ST) ||
	    (p_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_W_RRM_RESP_ST))

    {
	X2AP_TRACE(X2AP_INFO,"CELL ACTIVATION procedure stopped.\n");
	/* Stop X2AP_ENB_RESET_TIMER.*/
	x2ap_stop_timer(p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id);
	p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = X2AP_P_NULL;

	/* Change state to X2_RESET_IDLE_ST.*/
	p_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST; 

	/* Setting retry_count to 0 */
	p_enb_ctx->x2ap_cell_act_sm_ctx.retry_count = 0;
    }
    else
    {
	X2AP_TRACE(X2AP_INFO,"No Cell Activation procedure on-going.\n");
    }
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_stop_mobility_setting_ch_procedure
 * Inputs         : p_enb_ctx     - Pointer to peer eNB context
 * Outputs        : None
 * Returns        : none
 * Description    : This function stops any ongoinf reset procesure.
 ****************************************************************************/
void x2ap_stop_mobility_setting_ch_procedure(
	x2ap_peer_enb_context_t  *p_enb_ctx)
{
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_enb_ctx);

    if((p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_PEER_RESP_ST) ||
	    (p_enb_ctx->x2ap_mobility_ch_sm_ctx.state == X2_W_MOBILITY_CHANGE_RRM_RESP_ST))

    {
	X2AP_TRACE(X2AP_INFO,"MOBILITY SETTING CHANGE procedure stopped.\n");
	/* Stop Mobility setting change timer.*/
	x2ap_stop_timer(p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id);
	p_enb_ctx->x2ap_mobility_ch_sm_ctx.timer_id = X2AP_P_NULL;

	/* Change state to X2_MOBILITY_CHANGE_ACTIVE_ST.*/
	p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;

	/* Setting retry_count to 0 */
	p_enb_ctx->x2ap_mobility_ch_sm_ctx.retry_count = 0;
    }
    else
    {
	X2AP_TRACE(X2AP_INFO,"No Mobility Setting Change procedure on-going.\n");
    }
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_enb_reset_timer_expiry_evt_hdl 
 * Inputs         : p_timer_buf - pointer to timer buffer
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function resets eNB reset timer expiry event handler 
 ********************************************************************************/
x2ap_return_et x2ap_enb_reset_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf , x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    x2ap_return_et retVal = X2AP_SUCCESS;
    x2ap_peer_enb_context_t* p_peer_enb_cntx =X2AP_P_NULL;
    LOCAL_EVENT_X2_RESET_TO_STRUCT  *p_event_x2_reset_to = PNULL;
    p_peer_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

    X2AP_ASSERT(PNULL != p_peer_enb_cntx);

    if(p_peer_enb_cntx->x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST)
    {
	p_peer_enb_cntx->x2ap_reset_sm_ctx.timer_id = X2AP_NULL;

	if(p_peer_enb_cntx->x2ap_reset_sm_ctx.retry_count >0)
	{
	    X2AP_TRACE(X2AP_INFO,"X2AP Reset retry count:%u.\n", p_peer_enb_cntx->x2ap_reset_sm_ctx.retry_count);
	    p_peer_enb_cntx->x2ap_reset_sm_ctx.retry_count--;
	    X2AP_TRACE(X2AP_INFO,"X2AP Reset retry count left:%u.\n", p_peer_enb_cntx->x2ap_reset_sm_ctx.retry_count);

	    /*CSR 51441 Start*/
	    ALARM_MSG_L3(RRC_MODULE_ID, X2AP_SELF_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
	    /*CSR 51441 Stop*/
	    retVal =  x2ap_enb_build_and_send_reset_req(p_x2ap_gb_ctx, p_peer_enb_cntx, &p_peer_enb_cntx->x2ap_reset_sm_ctx.reset_request_msg);
	}
	else
	{
	    X2AP_TRACE(X2AP_INFO, "X2AP RESET retry count exhausted\n");
	    x2ap_send_reset_response(p_x2ap_gb_ctx, &p_peer_enb_cntx->gb_enb_id, X2AP_FAILURE, p_peer_enb_cntx->x2ap_reset_sm_ctx.trans_id);
	    p_peer_enb_cntx->x2ap_reset_sm_ctx.state = X2_RESET_ACTIVE_ST;

	    p_event_x2_reset_to = rrc_mem_get(sizeof(LOCAL_EVENT_X2_RESET_TO_STRUCT));
	    if (PNULL != p_event_x2_reset_to)
	    {
		memset_wrapper(p_event_x2_reset_to, RRC_NULL, sizeof(LOCAL_EVENT_X2_RESET_TO_STRUCT));
		p_event_x2_reset_to->header.length = sizeof(LOCAL_EVENT_X2_RESET_TO_STRUCT);
		p_event_x2_reset_to->header.event_id = LOCAL_EVENT_X2_RESET_TO;

		l3_memcpy_wrapper(p_event_x2_reset_to->EVENT_PARAM_ENB_IP_ADDRESS,p_peer_enb_cntx->
			enb_comm_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);
		rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_reset_to);
	    }
	}
    }
    else
    {
	X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_peer_enb_cntx->enb_config_sm_ctx.state);
    }
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_send_reset_response 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_peer_gb_enb_id - peer eNB id
 *                  response - responce to sent
 *                  trans_id - transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends reset responce to RRC.
 ********************************************************************************/
void x2ap_send_reset_response(x2ap_gb_context_t *p_x2ap_gb_ctx, x2_gb_enb_id_t* p_peer_gb_enb_id, U8 response, U16 trans_id)
{
    x2ap_reset_resp_t reset_resp;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    X2AP_MEMSET(&reset_resp, 0x00, sizeof(x2ap_reset_resp_t));

    X2AP_MEMCPY(&reset_resp.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
    X2AP_MEMCPY(&reset_resp.enb2_gb_id, p_peer_gb_enb_id, sizeof(x2_gb_enb_id_t));
    reset_resp.response = response;

    if(p_x2ap_gb_ctx->is_son_present)
    {
	dest_module_id = RRC_SON_MODULE_ID;
    }

    if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_reset_resp(
		&reset_resp,
		RRC_X2AP_MODULE_ID,
		dest_module_id,
		trans_id,
		RRC_NULL))
    {
	X2AP_TRACE(X2AP_ERROR,"Failed to send reset response.\n");
    }
}

/*****************************************************************************
 * Function Name  : x2ap_stop_resource_status_update_procedure
 * Inputs         : p_x2ap_gb_ctx - pointer to x2AP global context
 *                  p_peer_enb_ctx - pointyer to prrt eNB context
 * Outputs        : None
 * Returns        : None
 * Description    : This function stops esource status update procedure
 ********************************************************************************/
void x2ap_stop_resource_status_update_procedure(x2ap_gb_context_t *p_x2ap_gb_ctx, x2ap_peer_enb_context_t  *p_peer_enb_ctx)
{
    U16 measurement_count = 0;
    rsu_enb_meas_id_search_node_t* p_temp_node = 	X2AP_P_NULL;
    rsu_enb_meas_id_search_node_t* p_next_node = 	X2AP_P_NULL;

    /*Fetch all the measurement context and delete it*/
    p_temp_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_first_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree));
    p_next_node = p_temp_node;

    while(X2AP_P_NULL != p_temp_node)
    {
	p_next_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_next_node((x2ap_search_tree_node_t*)p_temp_node);
	if (p_temp_node->x2ap_rsu_ctx.peer_enb_id == p_peer_enb_ctx->peer_enodeb_id)
	{
	    X2AP_TRACE(X2AP_INFO,"X2AP RSU SM State:%u\n", p_temp_node->x2ap_rsu_ctx.state);

	    if((p_temp_node->x2ap_rsu_ctx.state == X2AP_RSU_STATE_START_ONGOING) || (p_temp_node->x2ap_rsu_ctx.state == X2AP_RSU_STATE_STOP_ONGOING))
	    {
		/* Stop the timer whichever is running */
		X2AP_TRACE(X2AP_INFO,"Stoping the timer : %p\n", p_temp_node->x2ap_rsu_ctx.timer_id);
		x2ap_stop_timer(p_temp_node->x2ap_rsu_ctx.timer_id);
		p_temp_node->x2ap_rsu_ctx.timer_id = X2AP_P_NULL;

		/* Delete the measurement context */
		X2AP_TRACE(X2AP_INFO,"X2AP RESET : Context is Deleted for Measurement ID : %d\n", p_temp_node->x2ap_rsu_ctx.enb1_meas_id);
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree), &p_temp_node->nodeAnchor);
		x2ap_mem_free(p_temp_node);
		p_temp_node = X2AP_P_NULL;
		measurement_count++;
	    }
	    else if ((p_temp_node->x2ap_rsu_ctx.state == X2AP_RSU_STATE_SEND_RSU )|| ( p_temp_node->x2ap_rsu_ctx.state == X2AP_RSU_STATE_LISTEN_RSU))
	    {
		/* Delete the measurement context */
		X2AP_TRACE(X2AP_INFO,"X2AP RESET : Context is Deleted for Measurement ID : %d\n", p_temp_node->x2ap_rsu_ctx.enb1_meas_id);
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree), &p_temp_node->nodeAnchor);
		x2ap_mem_free(p_temp_node);
		p_temp_node = X2AP_P_NULL;
		measurement_count++;
	    }
	}
	p_temp_node = p_next_node;
    }

    X2AP_TRACE(X2AP_INFO,"%u RESOURCE STATUS UPDATE measurements with peer eNB stopped.\n", measurement_count);
}

/*****************************************************************************
 * Function Name  : x2ap_stop_x2_handover_procedure 
 * Inputs         : p_peer_enb_ctx - peer eNB context
 * Outputs        : None
 * Returns        : None
 * Description    : This function stops X2-AP handober procedure
 ********************************************************************************/
void x2ap_stop_x2_handover_procedure(x2ap_peer_enb_context_t  *p_peer_enb_ctx)
{

    /* Build and send reset indication to UECC MODULE */
    if(X2AP_FAILURE ==  x2ap_build_and_send_reset_ind(p_peer_enb_ctx))
    {
	X2AP_TRACE(X2AP_ERROR, "x2ap_build_and_send_reset_ind failed.");
    }
}

/*****************************************************************************
 * Function Name  : x2ap_send_reset_ind 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global contrxt
 *                  p_peer_enb_cntx - peer eNB context
 *                  cause - cause for reset
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends reset indication 
 ********************************************************************************/
void x2ap_send_reset_ind(x2ap_gb_context_t *p_x2ap_gb_ctx, x2ap_peer_enb_context_t* p_peer_enb_cntx, x2ap_Cause cause)
{
    x2ap_reset_ind_t  x2ap_reset_indication;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    X2AP_MEMSET(&x2ap_reset_indication, 0, sizeof(x2ap_reset_ind_t));

    x2ap_reset_indication.bitmask = 0;
    x2ap_reset_indication.enb1_gb_id = p_peer_enb_cntx->gb_enb_id;
    x2ap_reset_indication.enb2_gb_id = p_x2ap_gb_ctx->global_enb_id;

    x2ap_reset_parse_asn_x2ap_cause(cause, &x2ap_reset_indication.cause);

    if(p_x2ap_gb_ctx->is_son_present)
    {
	dest_module_id = RRC_SON_MODULE_ID;
    }

    if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_reset_ind(
		&x2ap_reset_indication,
		RRC_X2AP_MODULE_ID,
		dest_module_id,
		p_peer_enb_cntx->x2ap_reset_sm_ctx.trans_id,
		RRC_NULL))
    {
	X2AP_TRACE(X2AP_ERROR,"[%s] Failed to send reset indication.\n",
		__FUNCTION__);
    }
}

/*****************************************************************************
 * Function Name  : x2ap_reset_compose_asn_x2ap_cause 
 * Inputs         : cause i type of x2ap_cause_t
 *                  p_asn_cause - pointer to x2ap_Cause
 * Outputs        : p_asn_cause
 * Returns        : None
 * Description    : This function resets and composes the asn cause.
 ********************************************************************************/
void x2ap_reset_compose_asn_x2ap_cause(x2ap_cause_t cause, x2ap_Cause* p_asn_cause)
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
		X2AP_TRACE(X2AP_ERROR,"Unknown cause type.%u\n", cause.type);
	    }break;
    }
}

/*****************************************************************************
 * Function Name  : x2ap_reset_parse_asn_x2ap_cause 
 * Inputs         : asn_cause - type of x2ap_Cause
 *                  p_cause - pointer to x2ap_cause_t
 * Outputs        : p_cause
 * Returns        : None
 * Description    : This function resets the asn_cause
 ********************************************************************************/
void x2ap_reset_parse_asn_x2ap_cause(x2ap_Cause asn_cause, x2ap_cause_t* p_cause)
{
    switch(asn_cause.t)
    {
	case T_x2ap_Cause_radioNetwork:
	    {
		p_cause->type = T_x2ap_Cause_radioNetwork;
		p_cause->value = asn_cause.u.radioNetwork;
	    }break;
	case T_x2ap_Cause_transport:
	    {
		p_cause->type = T_x2ap_Cause_transport;
		p_cause->value = asn_cause.u.transport;
	    }break;
	case T_x2ap_Cause_protocol:
	    {
		p_cause->type = T_x2ap_Cause_protocol;
		p_cause->value = asn_cause.u.protocol;
	    }break;
	case T_x2ap_Cause_misc:
	    {
		p_cause->type = T_x2ap_Cause_misc;
		p_cause->value = asn_cause.u.misc;
	    }break;
	default:
	    {
		X2AP_TRACE(X2AP_ERROR,"Unknown cause type.%u\n", asn_cause.t);
	    }break;
    }

}


/*****************************************************************************
 * Function Name  : x2ap_stop_reset_procedure
 * Inputs         : p_enb_ctx     - Pointer to peer eNB context
 * Outputs        : None
 * Returns        : none
 * Description    : This function stops any ongoinf reset procesure.
 ****************************************************************************/
void x2ap_stop_reset_procedure( 
	x2ap_peer_enb_context_t  *p_enb_ctx)
{
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_enb_ctx);

    if(p_enb_ctx->x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST )
    {
	X2AP_TRACE(X2AP_INFO,"RESET procedure stopped.\n");
	/* Stop X2AP_ENB_RESET_TIMER.*/
	x2ap_stop_timer(p_enb_ctx->x2ap_reset_sm_ctx.timer_id);
	p_enb_ctx->x2ap_reset_sm_ctx.timer_id = X2AP_P_NULL;

	/* Change state to X2_RESET_IDLE_ST.*/
	p_enb_ctx->x2ap_reset_sm_ctx.state = X2_RESET_IDLE_ST; 

	/* Setting retry_count to 0 */
	p_enb_ctx->x2ap_reset_sm_ctx.retry_count = 0;
    }
    else
    {
	X2AP_TRACE(X2AP_INFO,"No RESET procedure on-going.\n");
    }

    X2AP_UT_TRACE_EXIT();
}

#ifdef ENDC_ENABLED
/*****************************************************************************
 * Function Name  : x2ap_stop_endc_configuration_update_procedure 
 * Inputs         : p_peer_enb_ctx - pointer to peer en-gNb context
 * Outputs        : None
 * Returns        : None
 * Description    : This function stops endc configuration update procedure
 ********************************************************************************/
void x2ap_stop_endc_configuration_update_procedure(x2ap_peer_enb_context_t  *p_peer_enb_ctx)
{
    switch(p_peer_enb_ctx->endc_config_sm_ctx.state)
    {
	case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP ENDC CONFIGURATION UPDATE Ongoing ...\n"
			"Current state: X2AP_ENB_CONFIG_W_CONF_RESP_ST, \n"
			" Stopping X2AP_ENB_CONFIG_UPDATE_RSP_TIMER:%p.\n",
			p_peer_enb_ctx->endc_config_sm_ctx.req_timer_id);
		x2ap_stop_timer(p_peer_enb_ctx->endc_config_sm_ctx.req_timer_id);
		p_peer_enb_ctx->endc_config_sm_ctx.req_timer_id = X2AP_P_NULL;
		p_peer_enb_ctx->endc_config_sm_ctx.req_retry_count =0;
		p_peer_enb_ctx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
	    }break;
	case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP ENDC CONFIGURATION UPDATE Ongoing ...\n"
			"Current state: X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST, \n"
			" Stopping X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER:%p.\n",
			p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_timer_id);
		x2ap_stop_timer(p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_timer_id);
		p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_timer_id = X2AP_P_NULL;
		p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_retry_count =0;
		p_peer_enb_ctx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
	    }break;
	default:
	    {
		X2AP_TRACE(X2AP_INFO,"X2AP ENDC CONFIGURATION UPDATE not ongoing ...\n");
	    }
	    break;
    }
}
#endif

