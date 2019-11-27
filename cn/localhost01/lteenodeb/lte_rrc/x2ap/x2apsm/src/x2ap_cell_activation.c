/*******************************************************************************
*
*  FILE NAME   : x2ap_cell_activation.c
*
*  DESCRIPTION : This file contains the X2AP CELL ACTIVATION  
*                function definition. 
*
*  DATE 25-Oct-2012  NAME: Hari Oum Sharan
*                    REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_uecc.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "rrc_x2apCommon_intf.h"
#include "x2ap_utils.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_il_parser.h"
#include "asn_dec_wrappers.h"
#include "asn_enc_wrappers.h"
#include "sm_entry.h"
#include "x2ap_timer.h"
#include "x2ap_utility.h"
#include "x2ap_cell_activation.h"
#include "rrc_logging.h"


#define X2AP_EUTRAN_CELL_ID_BIT_STR_LEN      28

x2ap_return_et x2ap_encode_cell_activation_request(
        x2ap_peer_cell_activation_req_t *p_peer_cell_act_req,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len);

x2ap_return_et x2ap_cell_act_req_compose_served_cells_to_activate(
        OSCTXT*       p_asn1_ctx,
        OSRTDList*    p_x2ap_served_cells_to_activate,
        x2ap_peer_cell_activation_req_t *p_peer_cell_act_req);

x2ap_return_et  x2ap_send_cell_activation_req_to_rrm_son (
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2_gb_enb_id_t* p_peer_gb_enb_id,
        x2ap_cell_activation_req_t *p_cell_act_req,
        U16 trans_id);

rrc_return_et x2ap_decode_cell_activation_request(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_cell_activation_req_t *p_cell_act_req,
        x2ap_X2AP_PDU  x2ap_pdu);

x2ap_return_et x2ap_enb_build_and_send_cell_activation_resp(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_cell_activation_response_t *p_x2ap_cell_act_resp,
        x2ap_CriticalityDiagnostics *p_criticality_diagnostics);

x2ap_return_et x2ap_encode_cell_activation_response (
        OSCTXT             *pctxt,
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len,
        rrc_x2ap_cell_activation_response_t   *p_msg_response);

x2ap_return_et x2ap_decode_cell_activation_response(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_peer_cell_activation_resp_t *p_peer_cell_act_resp,
        x2ap_X2AP_PDU  x2ap_pdu);

x2ap_return_et x2ap_encode_cell_activation_failure (
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len,
        rrc_x2ap_cell_activation_failure_t *p_msg_failure);

x2ap_return_et x2ap_decode_cell_activation_failure(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_peer_cell_activation_resp_t *p_peer_cell_act_resp,
        x2ap_X2AP_PDU  x2ap_pdu);

x2ap_return_et x2ap_compose_cell_activated_list(
        OSCTXT                      *p_asn1_ctx,
        OSRTDList                   *p_x2ap_IE_serv_cell,
        x2ap_ecgi_t                 *p_serv_cell,
        U16                          num_served_cell);

x2ap_return_et x2ap_validate_served_cell_id (
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_cell_activation_req_t *p_cell_act_req,
        U8 *peer_enodeb_id);

void x2ap_send_cell_activation_failure (
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2_gb_enb_id_t* p_peer_gb_enb_id, 
        U8 response,
        U16 trans_id);

x2ap_return_et x2ap_validate_peer_cell_ecgi (
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_cell_activation_req_t *p_peer_cell_act_req);

x2ap_return_et x2ap_build_and_send_cell_act_faiure_to_enb (
     x2ap_gb_context_t  *p_x2ap_gb_ctx,
     x2ap_peer_enb_context_t *p_enb_ctx,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics);

x2ap_return_et x2ap_build_and_send_cell_act_faiure_to_enb_validation_fail (
     x2ap_gb_context_t  *p_x2ap_gb_ctx,
     x2ap_peer_enb_context_t *p_enb_ctx,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics);

void x2ap_cell_act_compose_asn_x2ap_cause(
     x2ap_cause_t cause, 
     x2ap_Cause* p_asn_cause);

x2ap_return_et x2ap_enb_cell_activation_start_timer (
    x2ap_timer_id_en               timer_type,
    x2ap_peer_enb_context_t        *p_enb_ctx,
    U32                            timer_value,
    x2ap_timer_t                   *p_timer_id);

void x2ap_parse_cell_act_cause(
        x2ap_Cause asn_cause, 
        x2ap_cause_t* p_cause);

/*****************************************************************************
 * Function Name  : x2ap_enb_cell_activation_start_timer
 * Inputs         : timer_type    : Timer Type
 *                  timer_value   : Timer Value
 *                  p_timer_id : Timer Id
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS 
 * Description    : This start x2ap enb cell activation timer 
 *****************************************************************************/
x2ap_return_et x2ap_enb_cell_activation_start_timer
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

    X2AP_TRACE(X2AP_INFO,"[%s] Start X2AP_CELL_ACTIVATION_TIMER "
            "Timer: %d Timer Value:%d",
            __FUNCTION__, timer_type, timer_value);
    X2AP_UT_TRACE_EXIT();
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_cell_activation_req 
 * Inputs         : p_api - pointer to input API buffer
 *                  p_x2ap_gb_ctx - pointer to X2Ap global context
 *                  dest_module_id - destination id 
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function process the X2AP_CELL_ACTIVATION_REQ received
 *         from the rrm.
 ********************************************************************************/

x2ap_return_et x2ap_process_peer_cell_activation_req( 
            void                *p_api,
            x2ap_gb_context_t   *p_x2ap_gb_ctx) 
{
    x2ap_peer_cell_activation_req_t *p_peer_cell_act_req;
    S32 length_read = X2AP_NULL;
    x2ap_return_et response = X2AP_FAILURE;
    U16 trans_id = X2AP_NULL; 
    U8 counter = 0;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    x2ap_peer_enb_context_t  *p_peer_enb_ctx = X2AP_P_NULL;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = X2AP_NULL;
    x2ap_timer_t timer_id = {0};    

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);   

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));


    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    p_peer_cell_act_req = (x2ap_peer_cell_activation_req_t*)
                    x2ap_mem_get(sizeof(x2ap_peer_cell_activation_req_t));

    if(PNULL ==  p_peer_cell_act_req)
    {
        X2AP_TRACE(X2AP_FATAL, "Unable to allocate memory.");
        return X2AP_FAILURE;
    }

    /* Decode the received message from RRM */
    response = rrc_parse_x2ap_peer_cell_activation_req(
            p_peer_cell_act_req,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if(X2AP_SUCCESS == response)
    {
        if (X2AP_FAILURE == x2ap_encode_cell_activation_request(
                    p_peer_cell_act_req,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            response = X2AP_FAILURE;    
            X2AP_TRACE(X2AP_WARNING, 
                    "x2ap cell activation request encode failed");
        }
        else
        {
            /*Fetch peer eNB context*/
            for(counter = 0; counter < MAX_PEER_ENB; counter++)
            {
                if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                                &p_peer_cell_act_req->gb_enb_id, 
                                sizeof(x2_gb_enb_id_t))))
                {
                    p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                    break;
                }
            }
            if(p_peer_enb_ctx == X2AP_P_NULL)
            {
                response = X2AP_FAILURE; 
                X2AP_TRACE(X2AP_WARNING, 
                        "Process Cell Activation Req : No peer eNB context found");
            }   
            else if (p_peer_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_ACTIVE_ST)
            {
                /* Save Cell Activation request message */
                X2AP_MEMCPY(&p_peer_enb_ctx->x2ap_cell_act_sm_ctx.cell_act_request_msg,
                        p_peer_cell_act_req, sizeof(x2ap_peer_cell_activation_req_t));

                if (X2AP_FAILURE == x2ap_enb_cell_activation_start_timer(
                            X2AP_CELL_ACTIVATION_TIMER,
                            p_peer_enb_ctx,
                            p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_cell_activation_guard_tim_val,
                            &timer_id))
                {
                    X2AP_TRACE(X2AP_INFO,"Starting X2AP_CELL_ACTIVATION_TIMER failed.\n"); 
                    /* SPR 19078: Coverity Fix 63325 Start */
                    x2ap_mem_free((void *)p_peer_cell_act_req);
                    /* SPR 19078: Coverity Fix 63325 End */
                    return X2AP_FAILURE;
                }
                /* save the timer id in cell act context */
                p_peer_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = timer_id;
                /* SPR_19619 start */
                p_peer_enb_ctx->x2ap_cell_act_sm_ctx.trans_id = trans_id;
                /* SPR_19619 stop */
                peer_enb_msg.p_msg = asn_msg_buff;
                peer_enb_msg.msg_len = asn_msg_len;
                /* SPR_14436_Fix Start */
                peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
                    stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
                /* SPR_14436_Fix Stop */

                if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
                {
                    X2AP_TRACE(X2AP_INFO, 
                            "Sending the Cell-Activation Request to Peer eNB via SCTP");
                    /*sending the Cell-Activation Request to Peer eNB via SCTP */
                    if(X2AP_FAILURE == x2ap_sctp_sm_entry
                            (X2AP_SCTP_SEND_DATA_EV,
                             (U8*)&peer_enb_msg,
                             p_x2ap_gb_ctx,
                             p_peer_enb_ctx))
                    {
                        X2AP_TRACE(X2AP_INFO,"x2ap_sctp_sm_entry- failed.\n"); 
                        /* SPR 19078: Coverity Fix 63325 Start */
                        x2ap_mem_free((void *)p_peer_cell_act_req);
                        /* SPR 19078: Coverity Fix 63325 End */
                        return X2AP_FAILURE;
                    }
                }
                /*Change state to X2_CELL_ACT_W_PEER_ENB_RESP_ST.*/
                p_peer_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_W_PEER_ENB_RESP_ST;

                response = X2AP_SUCCESS;
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "Invalid X2AP CELL ACTIVATION state:%u, ignoring cell act req.",
                        p_peer_enb_ctx->x2ap_cell_act_sm_ctx.state); 
                response = X2AP_FAILURE;
            }
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "X2AP Peer Cell Activation - Decode Failed");
        /* coverity_fix_63325_start */
        x2ap_mem_free((void *)p_peer_cell_act_req);
        /* coverity_fix_63325_stop */
        return X2AP_FAILURE;
    }

    if(response == X2AP_FAILURE)
    {
       X2AP_TRACE(X2AP_INFO, "Sending Cell activation Failure to RRM/SON");
       x2ap_send_cell_activation_failure (p_x2ap_gb_ctx,
                    &p_peer_cell_act_req->gb_enb_id,
                    X2AP_FAILURE,
                    trans_id);
    }
    /* coverity_fix_63325_start */
    x2ap_mem_free((void *)p_peer_cell_act_req);
    /* coverity_fix_63325_stop */
    return response;
}
/*****************************************************************************
 * Function Name  : x2ap_encode_cell_activation_request 
 * Inputs         : p_peer_cell_act_req - pointer to 
 *                                        x2ap_peer_cell_activation_req_t
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length 
 * Outputs        : p_asn_msg , p_asn_msg_len
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function used to encode Cell Activation request.
 ********************************************************************************/
x2ap_return_et x2ap_encode_cell_activation_request(
        x2ap_peer_cell_activation_req_t *p_peer_cell_act_req,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len)
{
    OSCTXT              asn1_ctx;
    OSRTDListNode       *p_node = PNULL;
    x2ap_X2AP_PDU       x2ap_pdu;
    x2ap_return_et         retVal = X2AP_FAILURE;
    x2ap_CellActivationRequest   *p_x2ap_CellActivationRequest;
    x2ap_CellActivationRequest_protocolIEs_element* p_protocolIE_elem;

    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.", __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }

    do{
        X2AP_MEMSET ( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage; 

        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, x2ap_InitiatingMessage);
        
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_cellActivation;
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

        /* Set the initiating message type to X2 Setup */ 
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__cellActivation;

        p_x2ap_CellActivationRequest = rtxMemAllocType(&asn1_ctx, x2ap_CellActivationRequest);

        if (X2AP_P_NULL == p_x2ap_CellActivationRequest)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"Mem Alloc successful to p_x2ap_CellActivationRequest");
        }

        asn1Init_x2ap_CellActivationRequest (p_x2ap_CellActivationRequest);
        x2ap_pdu.u.initiatingMessage->value.u.cellActivation = p_x2ap_CellActivationRequest;

        rtxDListAllocNodeAndData(&asn1_ctx,                                         
                                 x2ap_CellActivationRequest_protocolIEs_element,
                                 &p_node,
                                 &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {                                                                         
            /* Not enough memory */                                               
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;                                                              
        }                                                                       

        asn1Init_x2ap_CellActivationRequest_protocolIEs_element (p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */                                   
        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCellsToActivate;
        p_protocolIE_elem->criticality = x2ap_reject;

        /* Fill the type of Protocol IE element */                                  

        p_protocolIE_elem->value.u._x2ap_CellActivationRequest_IEs_1 = rtxMemAllocType(&asn1_ctx,
        x2ap_ServedCellsToActivate);
        
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_CellActivationRequest_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ServedCellsToActivate(p_protocolIE_elem->value.u._x2ap_CellActivationRequest_IEs_1);
        p_protocolIE_elem->value.t = T53x2ap___x2ap_CellActivationRequest_IEs_1; 

        if (X2AP_FAILURE == 
                x2ap_cell_act_req_compose_served_cells_to_activate(&asn1_ctx, 
                    p_protocolIE_elem->value.u._x2ap_CellActivationRequest_IEs_1, 
                    p_peer_cell_act_req))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Compose x2ap_ServedCellsToActivate - failure.",
                     __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "%s:Compose x2ap_ServedCellsToActivate - Success.",
                     __FUNCTION__);
        }

        rtxDListAppendNode(&p_x2ap_CellActivationRequest->protocolIEs, p_node);
        
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of Cell activation request - failed.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of Cell activation request - Success.");
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);

           
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : CELL ACTIVATION REQUEST", 
                &x2ap_pdu);

            retVal = X2AP_SUCCESS;
        }
    }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_cell_act_req_compose_served_cells_to_activate 
 * Inputs         : p_asn1_ctx - pointer to ASN context
 *                  p_x2ap_served_cells_to_activate - pointer to OSRTDList
 *                  p_peer_cell_act_req - pointer to 
 *                                       x2ap_peer_cell_activation_req_t
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function used to encode served cells to activate list.
 * Error          : This function report error to the caller function, If encoder
 *         fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_cell_act_req_compose_served_cells_to_activate(
        OSCTXT*       p_asn1_ctx,
        OSRTDList*    p_x2ap_served_cells_to_activate,
        x2ap_peer_cell_activation_req_t *p_peer_cell_act_req)
{
    U16  count = X2AP_NULL;
    OSRTDListNode*    p_node_served_cells_to_act = X2AP_P_NULL;
    x2ap_ServedCellsToActivate_Item* p_served_cells_to_act_item = X2AP_P_NULL; 

    /* filling cell information */
    for (count = 0; count <
            p_peer_cell_act_req->peer_cell_activation_info.num_served_cell; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_ServedCellsToActivate_Item,
                &p_node_served_cells_to_act,
                &p_served_cells_to_act_item);

        if (X2AP_P_NULL == p_node_served_cells_to_act)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_ServedCellsToActivate_Item (p_served_cells_to_act_item);
        
        /*1. Compose x2ap_ECGI */
        /* Encode PLMN ID */
        p_served_cells_to_act_item->ecgi.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
        
        X2AP_MEMCPY(
                p_served_cells_to_act_item->ecgi.pLMN_Identity.data,
                p_peer_cell_act_req->peer_cell_activation_info.served_cell_list[count].plmn_identity.plmn_id,
                MAX_PLMN_ID_BYTES);

        p_served_cells_to_act_item->ecgi.eUTRANcellIdentifier.numbits = CELL_ID_NBITS;
        X2AP_MEMCPY(p_served_cells_to_act_item->ecgi.eUTRANcellIdentifier.data,
                p_peer_cell_act_req->peer_cell_activation_info.served_cell_list[count].eutran_cell_id,
                sizeof(U32));

        rtxDListAppendNode(p_x2ap_served_cells_to_activate, p_node_served_cells_to_act);
    }
    return X2AP_SUCCESS;
}

/****************************************************************************
 * Function Name  : x2ap_enb_process_cell_activation_request_msg
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the cell activation
 *                  request mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_enb_process_cell_activation_request_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_cell_activation_req_t cell_act_req;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U8 peer_enodeb_id = X2AP_NULL;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    x2ap_timer_t timer_id = {0};    
    
    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    X2AP_MEMSET(&cell_act_req, 0, sizeof(x2ap_cell_activation_req_t));

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
        cell_act_req.bitmask = 0;
        
        if (RRC_FAILURE == 
                x2ap_decode_cell_activation_request(
                    p_x2ap_gb_ctx,
                    p_enb_ctx,
                    &asn1_ctx, 
                    &cell_act_req,
                    x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Cell Activation request- "
                    "ASN decoding failed",__FUNCTION__);
            /*sending cell activation failure response*/
            /*without any state change*/
            x2ap_build_and_send_cell_act_faiure_to_enb (p_x2ap_gb_ctx,p_enb_ctx,X2AP_P_NULL);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        else
        {
            if (X2AP_FAILURE == x2ap_validate_served_cell_id (p_x2ap_gb_ctx,&cell_act_req, &peer_enodeb_id))
            {
                X2AP_TRACE(X2AP_ERROR, "%s: Cell Activation request- "
                    "validation failed",__FUNCTION__);
                /*sending cell activation failure response*/
                /*without any state change*/
                x2ap_build_and_send_cell_act_faiure_to_enb_validation_fail(
                            p_x2ap_gb_ctx,
                            p_enb_ctx,
                            X2AP_P_NULL);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "%s: Cell Activation request- "
                    "validation Success",__FUNCTION__);
            }
            if (p_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_ACTIVE_ST)
            {
                X2AP_MEMCPY(&cell_act_req.enb1_gb_id, &p_enb_ctx->gb_enb_id,
                        sizeof(x2_gb_enb_id_t)); 
                X2AP_MEMCPY(&cell_act_req.enb2_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                        sizeof(x2_gb_enb_id_t));

                if(p_x2ap_gb_ctx->is_son_present)
                {
                    dest_module_id = RRC_SON_MODULE_ID;
                }
                if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_cell_activation_req(
                            &cell_act_req,
                            RRC_X2AP_MODULE_ID,
                            dest_module_id,
                            trans_id,
                            RRC_NULL))
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "Failed to send Cell activation Request to RRM/SON.\n");
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return X2AP_FAILURE;
                }
                p_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_W_RRM_RESP_ST;
                /*spr_fix*/
                /*Start X2AP_CELL_ACT_RRM_GUARD_TIMER*/
                if (X2AP_FAILURE == x2ap_enb_cell_activation_start_timer(
                            X2AP_CELL_ACT_RRM_GUARD_TIMER,
                            p_enb_ctx,
                            p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_rsu_rrm_guard_tim_val,
                            &timer_id))
                {
                    X2AP_TRACE(X2AP_INFO,"Starting X2AP_CELL_ACTIVATION_TIMER failed.\n"); 
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return X2AP_FAILURE;
                }
                /* save the timer id in cell act context */
                p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = timer_id;
                /*spr_fix*/

            }
            else if (p_enb_ctx->x2ap_cell_act_sm_ctx.state ==
                                X2_CELL_ACT_W_PEER_ENB_RESP_ST)
            {
                X2AP_TRACE(X2AP_INFO, "X2AP CELL_ACT sm state X2_CELL_ACT_W_PEER_ENB_RESP_ST. "
                   "Cell activation X2 interface done, Sending CELL ACTIVATION FAILURE");
                /*sending cell activation failure response*/
                /*without any state change*/
                x2ap_build_and_send_cell_act_faiure_to_enb (p_x2ap_gb_ctx,p_enb_ctx,X2AP_P_NULL);
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "Invalid X2AP CELL ACT SM state:%u.",
                        p_enb_ctx->x2ap_cell_act_sm_ctx.state);
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

/******************************************************************************
 * Function Name  : x2ap_send_cell_activation_req_to_rrm_son
 * Inputs         : p_x2ap_gb_ctx  pointer to X2AP global context
 *                  p_peer_gb_enb_id - pointer to peer eNB id
 *                  p_cell_act_req - pointer to x2ap_cell_activation_req_t
 *                  trans_id - transaction id 
 * Outputs        : None
 * Returns        : X2AP_SUCCESS  or X2AP_FAILURE 
 * Description    : This function is called when x2 setup link is UP to send
 *                  to rrm/son.
 ******************************************************************************/
x2ap_return_et  x2ap_send_cell_activation_req_to_rrm_son (
                x2ap_gb_context_t *p_x2ap_gb_ctx,
                x2_gb_enb_id_t* p_peer_gb_enb_id,
                x2ap_cell_activation_req_t *p_cell_act_req,
                U16 trans_id)
{
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_x2ap_gb_ctx != PNULL);
    X2AP_ASSERT(p_peer_gb_enb_id != PNULL);
    X2AP_ASSERT(p_cell_act_req != PNULL);
    
    X2AP_MEMCPY(&p_cell_act_req->enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                sizeof(x2_gb_enb_id_t)); 
    X2AP_MEMCPY(&p_cell_act_req->enb2_gb_id, p_peer_gb_enb_id,
        sizeof(x2_gb_enb_id_t));

    if(p_x2ap_gb_ctx->is_son_present)
    {
        dest_module_id = RRC_SON_MODULE_ID;
    }

    if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_cell_activation_req(
                                              p_cell_act_req,
                                              RRC_X2AP_MODULE_ID,
                                              dest_module_id,
                                              trans_id,
                                              RRC_NULL))
    {
        X2AP_TRACE(X2AP_ERROR,
            "Failed to send Cell activation Request to RRM/SON.\n");
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_cell_activation_request. 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_ctx - pointer to peer eNB context
 *                  pctxt - pointer to OSCTXT
 *                  p_cell_act_req - pointer to x2ap_cell_activation_req_t
 *                  x2ap_pdu - pointer to x2ap_X2AP_PDU
 * Outputs        : p_cell_act_req 
 * Returns        : RRC_SUCCESS or RRC_FAILURE
 * Description    : This function used to decode Cell Activation requset.
 * Error          : This function report error to the caller function, If decoder
 * fails to decode message.
 ********************************************************************************/
rrc_return_et x2ap_decode_cell_activation_request(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_cell_activation_req_t *p_cell_act_req,
        x2ap_X2AP_PDU  x2ap_pdu)
{

    rrc_return_et retval = RRC_SUCCESS;
    x2ap_CellActivationRequest_protocolIEs_element* p_elem1;
    x2ap_ServedCellsToActivate_Item* p_elem2 ;
    OSRTDListNode*p_node1 = PNULL;
    OSRTDListNode*p_node2 = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16  ie_list_index = 0;
    U16 index_1;
    U16 index_2;
    U16 count_1;
    U16 count_2;
    
    x2ap_message_data_t message_map =
    {1, X2AP_TRUE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ServedCellsToActivate, x2ap_mandatory, x2ap_reject, X2AP_NULL,
             X2AP_NULL, X2AP_NULL, X2AP_NULL}}};

    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));

    p_node1 =
        x2ap_pdu.u.initiatingMessage->value.u.cellActivation->protocolIEs.head;
    count_1 =
        x2ap_pdu.u.initiatingMessage->value.u.cellActivation->protocolIEs.count;

    for (index_1 = 0; index_1 < count_1; index_1++)
    {
        p_elem1 = (x2ap_CellActivationRequest_protocolIEs_element*)p_node1->data;
        if(p_elem1 == NULL)
        {
            X2AP_TRACE(X2AP_WARNING,"%s: p_elem1 is NULL",__FUNCTION__);
            return X2AP_FAILURE;
        }

        switch(p_elem1->id)
        {
            case ASN1V_x2ap_id_ServedCellsToActivate:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index_1,
                                p_elem1->id,
                                p_elem1->value.u._x2ap_CellActivationRequest_IEs_1))
                    {
                        p_node2 = p_elem1->value.u._x2ap_CellActivationRequest_IEs_1->head;
                        count_2 = p_elem1->value.u._x2ap_CellActivationRequest_IEs_1->count;
                        if(X2AP_MAX_IES <= count_2)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                            " in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
                            retval = X2AP_FAILURE;
                            break;
                        }
                        for (index_2 = 0; (index_2 < count_2) && (index_2 < MAX_SERVED_CELLS); index_2++)
                        {
                            p_elem2 = (x2ap_ServedCellsToActivate_Item*)p_node2->data;
                            if(p_elem2 == NULL)
                            {
                                X2AP_TRACE(X2AP_WARNING,"%s: p_elem2 is NULL",__FUNCTION__);
                                return X2AP_FAILURE;
                            }
                            p_cell_act_req->cell_activation_info.bitmask = 0;
                            p_cell_act_req->cell_activation_info.num_served_cell = count_2;

                            X2AP_MEMCPY(p_cell_act_req->cell_activation_info.served_cell_list[index_2].plmn_identity.plmn_id,
                                    p_elem2->ecgi.pLMN_Identity.data,
                                    p_elem2->ecgi.pLMN_Identity.numocts);

                            X2AP_MEMCPY(
                                    &p_cell_act_req->cell_activation_info.served_cell_list[index_2].eutran_cell_id,
                                    &p_elem2->ecgi.eUTRANcellIdentifier.data,
                                    sizeof(U32));

                            p_node2 = p_node2->next;
                        }
                    }
                }
                break;
            default:
                /* Class - 1 Error - Unknown IE */
                /* Base on Criticality fill the value */
                x2ap_add_to_err_ind_ie_list(&iE_list,
                        p_elem1->criticality,
                        p_elem1->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);

				/*SPR_19067_FIX_START*/
                if (x2ap_reject == p_elem1->criticality) 
                {
                    retval = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
        }
    }

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    pctxt,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_cellActivation,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retval = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retval;
}

/*****************************************************************************
 * Function Name  : x2ap_process_cell_activation_resp 
 * Inputs         : p_api - pointer to input API data 
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function process the X2AP_CELL_ACTIVATION_RESP received
 *         from the rrm.
 ********************************************************************************/
x2ap_return_et x2ap_process_cell_activation_resp( 
            void                *p_api,
            x2ap_gb_context_t   *p_x2ap_gb_ctx) 
{
    x2ap_cell_activation_response_t *p_cell_act_resp;
    S32 length_read = X2AP_NULL;
    x2ap_return_et response = X2AP_SUCCESS;
    U8 counter = 0;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    x2ap_peer_enb_context_t  *p_peer_enb_ctx = X2AP_P_NULL;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);   

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));


    /* Get the transaction id */
    /* SPR 19838 : Coverity 12781 fix start */
    /* Code Deleted */
    /* SPR 19838 : Coverity 12781 fix end */
    p_cell_act_resp = (x2ap_cell_activation_response_t*)
        x2ap_mem_get(sizeof(x2ap_cell_activation_response_t));
    if (PNULL == p_cell_act_resp)
    {
        X2AP_TRACE(X2AP_FATAL, "Unable to allocate memory.");
        return X2AP_FAILURE;
    }

    /* Decode the received message from RRM */
    response = rrc_parse_x2ap_cell_activation_response(
            p_cell_act_resp,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if(X2AP_SUCCESS == response)
    {
        X2AP_TRACE(X2AP_INFO, "X2AP Peer Cell Activation Response - Parsing Success");
            /*Fetch peer eNB context*/
            for(counter = 0; counter < MAX_PEER_ENB; counter++)
            {
            /* SPR_19619 start */
                if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                            &p_cell_act_resp->cell_activation_resp.enb2_gb_id,
                                sizeof(x2_gb_enb_id_t))))

                /* SPR_19619 stop */
                {
                    p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                    break;
                }
            }
            if(p_peer_enb_ctx == X2AP_P_NULL)
            {
                response = X2AP_FAILURE; 
                X2AP_TRACE(X2AP_WARNING, 
                        "Process Cell Activation Resp : No peer eNB context found");
            }   
            else if (X2_CELL_ACT_W_RRM_RESP_ST ==
                        p_peer_enb_ctx->x2ap_cell_act_sm_ctx.state)
            {
                /* SPR 14753 fix start */
                /* Stop X2AP_CELL_ACT_RRM_GUARD_TIMER.*/
                x2ap_stop_timer(p_peer_enb_ctx->x2ap_cell_act_sm_ctx.timer_id);
                p_peer_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = X2AP_P_NULL;
                /* SPR 14753 fix end */

                X2AP_TRACE(X2AP_INFO, "peer eNB context found");
                if (X2AP_FAILURE == x2ap_enb_build_and_send_cell_activation_resp (
                            p_x2ap_gb_ctx,
                            p_peer_enb_ctx,
                            p_cell_act_resp,
                            X2AP_P_NULL))
                {
                    response = X2AP_FAILURE; 
                    X2AP_TRACE(X2AP_WARNING, 
                            "Process Cell Activation Resp : "
                            "x2ap_enb_build_and_send_cell_activation_resp -failed");
                }
                else
                {
                    X2AP_TRACE(X2AP_WARNING, 
                            "Process Cell Activation Resp : "
                            "Send cell activation response to peer enb -success :"
                            "Change current state to X2_CELL_ACT_ACTIVE_ST");
                    p_peer_enb_ctx->x2ap_cell_act_sm_ctx.state =
                        X2_CELL_ACT_ACTIVE_ST;
                }
            }
            else
            {
                response = X2AP_FAILURE; 
                X2AP_TRACE(X2AP_WARNING, 
                            "Process Cell Activation Resp : invalid enb state");
            }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "X2AP Peer Cell Activation Response - Parsing Failed");
        /* coverity_fix_63260_start */
        x2ap_mem_free(p_cell_act_resp);
        /* coverity_fix_63260_stop */
        return X2AP_FAILURE;
    }

    /* coverity_fix_63260_start */
    x2ap_mem_free(p_cell_act_resp);
    /* coverity_fix_63260_stop */
    return response;
}
/******************************************************************************
 * Function Name : x2ap_enb_build_and_send_cell_activation_resp
 * Inputs        : p_x2ap_gb_ctx -  pointer to X2AP global context
 *               : p_enb_ctx - pointer to Peer eNB context 
 *               : p_x2ap_cell_act_resp- pointer to  X2AP CELL ACTIVATION RESPONSE
 *               : p_criticality_diagnostics - pointer to criticaly diagnostics
 * Outputs       : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function builds and sends X2AP CELL ACTIVATION RESPONSE message
 *       to X2AP module.
 *******************************************************************************/
x2ap_return_et x2ap_enb_build_and_send_cell_activation_resp(
     x2ap_gb_context_t  *p_x2ap_gb_ctx,
     x2ap_peer_enb_context_t *p_enb_ctx,
     x2ap_cell_activation_response_t *p_x2ap_cell_act_resp,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics)
{
    OSCTXT asn1_ctx;
    x2ap_return_et result = X2AP_FAILURE;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;

    rrc_x2ap_cell_activation_failure_t msg_failure;
    rrc_x2ap_cell_activation_response_t msg_response;
    x2ap_peer_enb_msg_t  peer_enb_msg;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_cell_act_resp);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));
    X2AP_MEMSET(&msg_failure, 0, sizeof(rrc_x2ap_cell_activation_failure_t));
    X2AP_MEMSET(&msg_response, 0, sizeof(rrc_x2ap_cell_activation_response_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Encode message */
    if ((p_x2ap_cell_act_resp->cell_activation_resp.num_served_cell > X2AP_NULL) &&
            (X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT &
                p_x2ap_cell_act_resp->cell_activation_resp.bitmask) &&
        (X2AP_SUCCESS == p_x2ap_cell_act_resp->cell_activation_resp.response))
    {
        /*As of now p_criticality_diagnostics is NULL*/
        if (PNULL != p_criticality_diagnostics)
        {
            msg_response.criticality_diagnostics = *p_criticality_diagnostics;
            msg_response.bitmask |= RRC_X2AP_CELL_ACT_RESPONSE_CRIT_DIAGNO_PRESENT;
        }

        if (X2AP_FAILURE == x2ap_compose_cell_activated_list(
                    &asn1_ctx,
                    &msg_response.activated_cell_list,
                    p_x2ap_cell_act_resp->cell_activation_resp.served_cell_list,
                    p_x2ap_cell_act_resp->cell_activation_resp.num_served_cell) )
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:x2ap_compose_cell_activated_list Failure.",
                    __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        result = x2ap_encode_cell_activation_response (&asn1_ctx,
                asn_msg_buff,
                &asn_msg_len,
                &msg_response);
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
                /*Sending the X2AP CELL ACTIVATION RESPONSE to Peer eNB via SCTP */
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
                    "[x2ap_enb_build_and_send_cell_activation_resp] Encode failure");
        }
    }
    if ((X2AP_CELL_ACTIVATION_CAUSE_PRESENT &
                p_x2ap_cell_act_resp->cell_activation_resp.bitmask) ||
        (X2AP_FAILURE == p_x2ap_cell_act_resp->cell_activation_resp.response))
    {
        /*As of now p_criticality_diagnostics is NULL*/
        if (PNULL != p_criticality_diagnostics)
        {
            msg_failure.criticality_diagnostics = *p_criticality_diagnostics;
            msg_failure.bitmask |= RRC_X2AP_CELL_ACT_FAILURE_CRIT_DIAGNO_PRESENT;
        }
        if (X2AP_CELL_ACTIVATION_CAUSE_PRESENT & 
                p_x2ap_cell_act_resp->cell_activation_resp.bitmask)
        {
            x2ap_cell_act_compose_asn_x2ap_cause (p_x2ap_cell_act_resp->cell_activation_resp.cause, 
                    &msg_failure.cause);
        }
        else
        {
            msg_failure.cause.t = T_x2ap_Cause_radioNetwork;
            msg_failure.cause.u.radioNetwork = x2ap_unspecified_2; 
        }

        result = x2ap_encode_cell_activation_failure (
                asn_msg_buff,
                &asn_msg_len,
                &msg_failure);
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
                /*Sending the X2AP CELL ACTIVATION RESPONSE to Peer eNB via SCTP */
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
                    "[x2ap_enb_build_and_send_cell_activation_resp] Encode failure");
        }
    }
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/******************************************************************************
 * Function Name : x2ap_encode_cell_activation_response
 * Inputs        : pctxt - pointer to OSCTXT
 *                 p_asn_msg - pointer to ASN encoded message 
 *                 p_asn_msg_len - pointer to ASN encoded message length 
 *                 p_msg_response - pointer to rrc_x2ap_cell_activation_response_t
 * Outputs       : p_asn_msg , p_asn_msg_len 
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to encode X2 Setup response.
 * Error         : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
x2ap_return_et x2ap_encode_cell_activation_response (
        OSCTXT             *pctxt,
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len,
        rrc_x2ap_cell_activation_response_t *p_msg_response)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    x2ap_CellActivationResponse_protocolIEs_element *     p_protocolIE_elem = PNULL;
    x2ap_CellActivationResponse         *p_x2ap_CellActivationResponse = X2AP_P_NULL;
    rrc_x2ap_cell_activation_response_t msg;
    
    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&msg, 0, sizeof(rrc_x2ap_cell_activation_response_t));
    
    do
    {
        X2AP_MEMSET(&(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome; 

        x2ap_pdu.u.successfulOutcome = rtxMemAllocType(pctxt,
                x2ap_SuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);
   
        x2ap_pdu.u.successfulOutcome->procedureCode = ASN1V_x2ap_id_cellActivation;
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to Cell activation */ 
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__cellActivation;

        p_x2ap_CellActivationResponse = rtxMemAllocType(pctxt,
                                                x2ap_CellActivationResponse);  
        if (X2AP_P_NULL == p_x2ap_CellActivationResponse)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                    "Mem Alloc successful to x2ap_CellActivationResponse");
        }

        asn1Init_x2ap_CellActivationResponse (p_x2ap_CellActivationResponse);
        x2ap_pdu.u.successfulOutcome->value.u.cellActivation = p_x2ap_CellActivationResponse;


        ///////////////////////////////////////////////////////////
        /*** Compose id-x2ap_ActivatedCellList start ***/
        rtxDListAllocNodeAndData(
                pctxt,                                  
                x2ap_CellActivationResponse_protocolIEs_element,                     
                &p_node,                                                      
                &p_protocolIE_elem);        
        if (X2AP_P_NULL == p_node)
        {                                                                     
            /* Not enough memory */                                      
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);    
            break;                                                         
        }                                                                   
        asn1Init_x2ap_CellActivationResponse_protocolIEs_element(
                 p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */                              
        p_protocolIE_elem->id = ASN1V_x2ap_id_ActivatedCellList;                   
        p_protocolIE_elem->criticality = x2ap_ignore;  

        /* Fill the type of Protocol IE element */                            
        p_protocolIE_elem->value.t = T55x2ap___x2ap_CellActivationResponse_IEs_1;  

        p_protocolIE_elem->value.u._x2ap_CellActivationResponse_IEs_1  =                 
                    &p_msg_response->activated_cell_list;

        rtxDListAppendNode(&p_x2ap_CellActivationResponse->protocolIEs, p_node);
        /*** Compose id-ServedCells end ***/
        //////////////////////////////////////////
        /****************************critical diag******/
        if (p_msg_response->bitmask &
                RRC_X2AP_CELL_ACT_RESPONSE_CRIT_DIAGNO_PRESENT)
        {
            rtxDListAllocNodeAndData(pctxt,
                    x2ap_CellActivationResponse_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            }

            if(PNULL != p_protocolIE_elem)
            {
            X2AP_MEMSET(p_protocolIE_elem,0,
                    sizeof(x2ap_CellActivationResponse_protocolIEs_element));

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = x2ap_ignore;


            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T55x2ap___x2ap_CellActivationResponse_IEs_2;
            p_protocolIE_elem->value.u._x2ap_CellActivationResponse_IEs_2 =
                &p_msg_response->criticality_diagnostics;
            }

            rtxDListAppendNode(&p_x2ap_CellActivationResponse->protocolIEs, p_node);
        }
        /***********************************/
        rtxDListInit(&p_x2ap_CellActivationResponse->extElem1);

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of X2 Setup Response "
                    "failed.");
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(pctxt);
            retVal = X2AP_SUCCESS;
        }

    }while(0);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/****************************************************************************
 * Function Name  : x2ap_enb_process_cell_activation_response_msg
 * Inputs         : p_msg_ptr      Pointer to message buffer
 *                : p_x2ap_gb_ctx  pointer to X2AP global context
 *                : p_enb_ctx      Pointer to peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the cell activation
 *                  response mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_enb_process_cell_activation_response_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_peer_cell_activation_resp_t peer_cell_act_resp;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    
    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    X2AP_MEMSET(&peer_cell_act_resp, 0, sizeof(x2ap_peer_cell_activation_resp_t));
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

    if (T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t)
    {
        peer_cell_act_resp.peer_cell_activation_resp.bitmask = 0;
        if (X2AP_FAILURE == 
                x2ap_decode_cell_activation_response (
                    p_x2ap_gb_ctx,
                    p_enb_ctx,
                    &asn1_ctx, 
                    &peer_cell_act_resp,
                    x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Cell Activation response- "
                "ASN decoding failed",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        if (p_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_W_PEER_ENB_RESP_ST)
        {
            if ((X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT & 
                        peer_cell_act_resp.peer_cell_activation_resp.bitmask) && 
                (p_enb_ctx->x2ap_cell_act_sm_ctx.cell_act_request_msg.
                    peer_cell_activation_info.num_served_cell ==
                peer_cell_act_resp.peer_cell_activation_resp.num_served_cell))
            {
                /* Stop X2AP_ENB_CELL_ACT_TIMER.*/
                x2ap_stop_timer(p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id);
                p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = X2AP_P_NULL;

                /*Change state to X2_CELL_ACT_ACTIVE_ST.*/
                p_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST;
            }
            /*Send X2AP_CELL_ACTIVATION_RESP to RRM/SON.*/

            X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t)); 
            X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb2_gb_id, &p_enb_ctx->gb_enb_id,
                    sizeof(x2_gb_enb_id_t));
            peer_cell_act_resp.peer_cell_activation_resp.response = X2AP_SUCCESS;
            /* SPR_19619 start */
            trans_id = p_enb_ctx->x2ap_cell_act_sm_ctx.trans_id;
            /* SPR_19619 stop */

            if(p_x2ap_gb_ctx->is_son_present)
            {
                dest_module_id = RRC_SON_MODULE_ID;
            }
            if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_peer_cell_activation_resp(
                        &peer_cell_act_resp,
                        RRC_X2AP_MODULE_ID,
                        dest_module_id,
                        trans_id,
                        RRC_NULL))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "Failed to send Cell activation Response to RRM/SON.\n");
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,
                        "send Cell activation Response success to RRM/SON.\n");
            }
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"Invalid X2AP CELL_ACTIVATION state:%u, "
                " ignoring CELL ACTIVATION RESPONSE",
                p_enb_ctx->x2ap_cell_act_sm_ctx.state);
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
 * Function Name : x2ap_decode_cell_activation_response.
 * Inputs        : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                 p_enb_ctx - pointer to peer eNB context 
 *                 pctxt - pointer to OSCTXT
 *                 p_peer_cell_act_resp - pointer to x2ap_peer_cell_activation_resp_t 
 *                 x2ap_pdu - pointer to X2AP pdu
 * Outputs       : p_peer_cell_act_resp 
 * Returns       : X2AP_SUCCESS or X2APP_FAILURE
 * Description   : This function used to decode Cell Activation response.
 * Error         : This function report error to the caller function, If decoder
 * fails to decode message.
 *******************************************************************************/
x2ap_return_et x2ap_decode_cell_activation_response(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_peer_cell_activation_resp_t *p_peer_cell_act_resp,
        x2ap_X2AP_PDU  x2ap_pdu)
{
    x2ap_return_et retval = X2AP_SUCCESS;
    x2ap_CellActivationResponse_protocolIEs_element* p_elem1;
    x2ap_ActivatedCellList_Item* p_elem2 ;
    OSRTDListNode*p_node1 = PNULL;
    OSRTDListNode*p_node2 = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16  ie_list_index = 0;
    U16 index_1;
    U16 index_2;
    U16 count_1;
    U16 count_2;
    
    x2ap_message_data_t message_map =
    {2, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ActivatedCellList, x2ap_mandatory, x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {1, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));

    p_node1 =
        x2ap_pdu.u.successfulOutcome->value.u.cellActivation->protocolIEs.head;
    count_1 =
        x2ap_pdu.u.successfulOutcome->value.u.cellActivation->protocolIEs.count;

    for (index_1 = 0; index_1 < count_1; index_1++)
    {
        p_elem1 = (x2ap_CellActivationResponse_protocolIEs_element*)p_node1->data;
        if(p_elem1 == NULL)
        {
            X2AP_TRACE(X2AP_WARNING,"%s: p_elem1 is NULL",__FUNCTION__);
            return X2AP_FAILURE;
        }

        switch(p_elem1->id)
        {
            case ASN1V_x2ap_id_ActivatedCellList:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index_1,
                                p_elem1->id,
                                p_elem1->value.u._x2ap_CellActivationResponse_IEs_1))
                    {
                        p_node2 = p_elem1->value.u._x2ap_CellActivationResponse_IEs_1->head;
                        count_2 = p_elem1->value.u._x2ap_CellActivationResponse_IEs_1->count;
                        if(X2AP_MAX_IES <= count_2)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                            " in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
                            retval = X2AP_FAILURE;
                            break;
                        }
                        for (index_2 = 0; (index_2 < count_2) && (index_2 < MAX_SERVED_CELLS); index_2++)
                        {
                            p_elem2 = (x2ap_ActivatedCellList_Item*)p_node2->data;
                            if(p_elem2 == NULL)
                            {
                                X2AP_TRACE(X2AP_WARNING,"%s: p_elem2 is NULL",__FUNCTION__);
                                return X2AP_FAILURE;
                            }
                            p_peer_cell_act_resp->peer_cell_activation_resp.num_served_cell = count_2;

                            /* filling cell-id */
                            X2AP_MEMCPY(p_peer_cell_act_resp->peer_cell_activation_resp.served_cell_list[index_2].plmn_identity.plmn_id,
                                    p_elem2->ecgi.pLMN_Identity.data,
                                    p_elem2->ecgi.pLMN_Identity.numocts);

                            X2AP_MEMCPY(
                                    &p_peer_cell_act_resp->peer_cell_activation_resp.served_cell_list[index_2].eutran_cell_id,
                                    &p_elem2->ecgi.eUTRANcellIdentifier.data,
                                    sizeof(U32));
                            p_node2 = p_node2->next;
                        }
                        p_peer_cell_act_resp->peer_cell_activation_resp.bitmask |=
                           X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT;
                    }
                }
                break;
            case ASN1V_x2ap_id_CriticalityDiagnostics:
                X2AP_TRACE(X2AP_INFO,"%s: "\
                        "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                if (X2AP_SUCCESS == x2ap_validate_ie_value(
                            &message_map,
                            index_1,
                            p_elem1->id,
                            p_elem1->value.u._x2ap_CellActivationResponse_IEs_2))
                {
                    x2ap_log_criticality_diagostics(p_elem1->value.u._x2ap_CellActivationResponse_IEs_2);
                }
                break;

            default:
                /* Class - 1 Error - Unknown IE */
                /* Base on Criticality fill the value */
                x2ap_add_to_err_ind_ie_list(&iE_list,
                        p_elem1->criticality,
                        p_elem1->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                X2AP_TRACE(X2AP_ERROR,"%s:"\
                       "invalid Protocol IE id",__FUNCTION__);

					  /*SPR_19067_FIX_START*/
                        if (x2ap_reject == p_elem1->criticality) 
                        {
                      retval = X2AP_FAILURE;
        }
                        /*SPR_19067_FIX_STOP*/
        }
        p_node1 = p_node1->next;
    }
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    pctxt,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_cellActivation,
                    T_x2ap_X2AP_PDU_successfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retval = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retval;
}
/******************************************************************************
 * Function Name : x2ap_encode_cell_activation_failure
 * Inputs        : p_asn_msg - pointer to ASN encoded msg
 *                 p_asn_msg_len - pointer to ASN encoded msg length 
 *                 p_msg_failure - pointer to rrc_x2ap_cell_activation_failure_t
 * Outputs       : None 
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to encode Cell Activation Failure.
 * Error         : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
x2ap_return_et x2ap_encode_cell_activation_failure (
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len,
        rrc_x2ap_cell_activation_failure_t *p_msg_failure)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    x2ap_CellActivationFailure_protocolIEs_element *     p_protocolIE_elem = PNULL;
    x2ap_CellActivationFailure         *p_x2ap_CellActivationFailure = X2AP_P_NULL;
    
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET(&(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome; 

        x2ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_UnsuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.unsuccessfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);
   
        x2ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_x2ap_id_cellActivation;
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to Cell activation */ 
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__cellActivation;

        p_x2ap_CellActivationFailure = rtxMemAllocType(&asn1_ctx,
                                                x2ap_CellActivationFailure);  
        if (X2AP_P_NULL == p_x2ap_CellActivationFailure)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                    "Mem Alloc successful to x2ap_CellActivationFailure");
        }

        asn1Init_x2ap_CellActivationFailure (p_x2ap_CellActivationFailure);
        x2ap_pdu.u.unsuccessfulOutcome->value.u.cellActivation = p_x2ap_CellActivationFailure;


       /*** Compose id-Cause start ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_CellActivationFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        if (X2AP_P_NULL == p_protocolIE_elem)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;
        }

        asn1Init_x2ap_CellActivationFailure_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_protocolIE_elem->criticality = x2ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T57x2ap___x2ap_CellActivationFailure_IEs_1;

        p_protocolIE_elem->value.u._x2ap_CellActivationFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_Cause);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_CellActivationFailure_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause (p_protocolIE_elem->value.u.
                _x2ap_CellActivationFailure_IEs_1);

        X2AP_MEMCPY(p_protocolIE_elem->value.u._x2ap_CellActivationFailure_IEs_1,&p_msg_failure->cause,
                        sizeof(x2ap_Cause));
        rtxDListAppendNode(&p_x2ap_CellActivationFailure->protocolIEs, p_node);
        /*** Compose id-Cause end ***/
        //////////////////////////////////////////
        /****************************critical diag******/
        if (p_msg_failure->bitmask &
            RRC_X2AP_CELL_ACT_FAILURE_CRIT_DIAGNO_PRESENT)
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_CellActivationFailure_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            }

            if(PNULL != p_protocolIE_elem)
            {
            X2AP_MEMSET(p_protocolIE_elem,0,
                    sizeof(x2ap_CellActivationFailure_protocolIEs_element));

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = x2ap_ignore;


            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T57x2ap___x2ap_CellActivationFailure_IEs_1;
            p_protocolIE_elem->value.u._x2ap_CellActivationFailure_IEs_2 =
                &p_msg_failure->criticality_diagnostics;
            }

            rtxDListAppendNode(&p_x2ap_CellActivationFailure->protocolIEs, p_node);
        }
        /***********************************/
        rtxDListInit(&p_x2ap_CellActivationFailure->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of X2 Setup Response "
                    "failed.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/****************************************************************************
 * Function Name  : x2ap_enb_process_cell_activation_failure_msg
 * Inputs         : p_msg_ptr      Pointer to message buffer
 *                : p_x2ap_gb_ctx  pointer to X2AP global context
 *                : p_enb_ctx      Pointer to peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the cell activation
 *                  failure mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_enb_process_cell_activation_failure_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_peer_cell_activation_resp_t peer_cell_act_resp;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_X2AP_PDU          x2ap_pdu;
    OSCTXT                 asn1_ctx;
    U32                    trans_id = X2AP_NULL;
    U8                     *p_asn_msg;
    U32                    *p_asn_msg_len;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    
    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    p_asn_msg_len = &(p_peer_enb_msg->msg_len);

    X2AP_MEMSET(&peer_cell_act_resp, 0, sizeof(x2ap_peer_cell_activation_resp_t));
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
        peer_cell_act_resp.peer_cell_activation_resp.bitmask = 0;
        if (X2AP_FAILURE == 
                x2ap_decode_cell_activation_failure (
                    p_x2ap_gb_ctx,
                    p_enb_ctx,
                    &asn1_ctx, 
                    &peer_cell_act_resp,
                    x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Cell Activation failure- "
                    "ASN decoding failed",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return X2AP_FAILURE;
        }
        if (p_enb_ctx->x2ap_cell_act_sm_ctx.state == X2_CELL_ACT_W_PEER_ENB_RESP_ST)
        {

            /*1. Stop X2AP_ENB_CELL_ACT_TIMER.*/
            x2ap_stop_timer(p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id);
            p_enb_ctx->x2ap_cell_act_sm_ctx.timer_id = X2AP_P_NULL;
 
            X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t)); 
            X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb2_gb_id, &p_enb_ctx->gb_enb_id,
                    sizeof(x2_gb_enb_id_t));

            if(p_x2ap_gb_ctx->is_son_present)
            {
                dest_module_id = RRC_SON_MODULE_ID;
            }
            if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_peer_cell_activation_resp(
                        &peer_cell_act_resp,
                        RRC_X2AP_MODULE_ID,
                        dest_module_id,
                        trans_id,
                        RRC_NULL))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "Failed to send Cell activation Response to RRM/SON.\n");
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return X2AP_FAILURE;
            }
            /*Change state to X2_CELL_ACT_ACTIVE_ST.*/
            p_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST;
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"Invalid X2AP CELL_ACTIVATION state:%u, "
                    " ignoring CELL ACTIVATION RESPONSE",
                    p_enb_ctx->x2ap_cell_act_sm_ctx.state);
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
 * Function Name  : x2ap_decode_cell_activation_failure.
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_enb_ctx - pointer to peer eNB context
 *                  pctxt - pointer to OSCTXT
 *                  p_peer_cell_act_resp -pointer to x2ap_peer_cell_activation_resp_t
 *                  x2ap_pdu- pointer to x2ap pdu
 * Outputs        : p_peer_cell_act_resp 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function used to decode Cell Activation failure.
 * Error          : This function report error to the caller function, If decoder
 * fails to decode message.
 *******************************************************************************/
x2ap_return_et x2ap_decode_cell_activation_failure(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_peer_cell_activation_resp_t *p_peer_cell_act_resp,
        x2ap_X2AP_PDU  x2ap_pdu)
{
    x2ap_return_et retval = X2AP_SUCCESS;
    x2ap_CellActivationFailure_protocolIEs_element* p_elem1;
    OSRTDListNode*p_node1 = PNULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    x2ap_Cause cause;
    U16  ie_list_index = 0;
    U16 index_1;
    U16 count_1;
    
    x2ap_message_data_t message_map =
    {2, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {1, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
}};

    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));
    X2AP_MEMSET(&cause, 0, sizeof(x2ap_Cause));

    p_node1 =
        x2ap_pdu.u.unsuccessfulOutcome->value.u.cellActivation->protocolIEs.head;
    count_1 =
        x2ap_pdu.u.unsuccessfulOutcome->value.u.cellActivation->protocolIEs.count;

    for (index_1 = 0; index_1 < count_1; index_1++)
    {
        p_elem1 = (x2ap_CellActivationFailure_protocolIEs_element*)p_node1->data;
        if(p_elem1 == NULL)
        {
            X2AP_TRACE(X2AP_WARNING,"%s: p_elem1 is NULL",__FUNCTION__);
            return X2AP_FAILURE;
        }

        switch(p_elem1->id)
        {
            case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                index_1,
                                p_elem1->id,
                                p_elem1->value.u._x2ap_CellActivationFailure_IEs_1))
                    {
                        X2AP_MEMCPY(&cause,p_elem1->value.u._x2ap_CellActivationFailure_IEs_1,sizeof(x2ap_Cause));
                        x2ap_parse_cell_act_cause(
                              cause,
                              &(p_peer_cell_act_resp->peer_cell_activation_resp.cause));

                        p_peer_cell_act_resp->peer_cell_activation_resp.bitmask |=
                            X2AP_CELL_ACTIVATION_CAUSE_PRESENT;
                    }
                }
                break;
            case ASN1V_x2ap_id_CriticalityDiagnostics:
                X2AP_TRACE(X2AP_INFO,"%s: "\
                        "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                if (X2AP_SUCCESS == x2ap_validate_ie_value(
                            &message_map,
                            index_1,
                            p_elem1->id,
                            p_elem1->value.u._x2ap_CellActivationFailure_IEs_2))
                {
                    x2ap_log_criticality_diagostics(p_elem1->value.u._x2ap_CellActivationFailure_IEs_2);
                }
                break;

            default:
                /* Class - 1 Error - Unknown IE */
                /* Base on Criticality fill the value */
                x2ap_add_to_err_ind_ie_list(&iE_list,
                        p_elem1->criticality,
                        p_elem1->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                X2AP_TRACE(X2AP_ERROR,"%s:"\
                       "invalid Protocol IE id",__FUNCTION__);
				/*SPR_19067_FIX_START*/
                if (x2ap_reject == p_elem1->criticality) 
                {
                      retval = X2AP_FAILURE;
        }
                /*SPR_19067_FIX_STOP*/
        }
        p_node1 = p_node1->next;
    }

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    pctxt,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_cellActivation,
                    /* SPR 23762/SES-459 Fix Start */
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    /* SPR 23762/SES-459 Fix End */
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retval = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retval;
}

/******************************************************************************
 * Function Name  : x2ap_compose_cell_activated_list
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to OSRTDList
 *                  p_serv_cell - This contains the information of served cells
 *                  num_served_cell - num of served cell
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_cell_activated_list(
                             OSCTXT                      *p_asn1_ctx,
                             OSRTDList                   *p_x2ap_IE_serv_cell,
                             x2ap_ecgi_t                 *p_serv_cell,
                             U16                          num_served_cell)
{

    OSRTDListNode               *p_node_serv_cell = NULL;
    x2ap_ServedCellsToActivate_Item    *p_servedCells_elem = X2AP_P_NULL;
    U16                         cell_count = 0;
    
    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    for (cell_count = 0; cell_count < num_served_cell; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_ServedCellsToActivate_Item,
                                 &p_node_serv_cell,
                                 &p_servedCells_elem);
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ServedCellsToActivate_Item(p_servedCells_elem);

        /* Encode Cell ID */
        p_servedCells_elem->ecgi.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
        X2AP_MEMCPY(p_servedCells_elem->ecgi.pLMN_Identity.data,
                p_serv_cell->plmn_identity.plmn_id,
                MAX_PLMN_ID_BYTES);

        p_servedCells_elem->ecgi.eUTRANcellIdentifier.numbits = 
            X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
        X2AP_MEMCPY(p_servedCells_elem->ecgi.eUTRANcellIdentifier.data,
                &(p_serv_cell->eutran_cell_id),
                sizeof(U32));

        /*** compose served cell info end ***/
        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
        p_serv_cell = (x2ap_ecgi_t *)(p_serv_cell + 1);
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name : x2ap_validate_served_cell_id
 * Inputs        : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                 p_cell_act_req - pointer to x2ap_cell_activation_req_t
 *                 peer_enodeb_id - peer eNB id
 * Outputs       : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function validate  served cell id of peer eNB,
 *       received in 
 *******************************************************************************/
x2ap_return_et x2ap_validate_served_cell_id
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_cell_activation_req_t *p_cell_act_req,
    U8 *peer_enodeb_id )
{
    U8 o_c, i_c = X2AP_NULL;
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL != p_cell_act_req);

    for(o_c = 0; o_c < MAX_PEER_ENB; o_c ++)
    {
        for(i_c = 0 ;i_c < p_x2ap_gb_ctx->num_served_cells; i_c++)
        {
            if (0 ==
                    X2AP_MEMCMP(p_cell_act_req->cell_activation_info.served_cell_list[0].eutran_cell_id,
                        p_x2ap_gb_ctx->served_cell_info[i_c].ecgi.eutran_cell_id,
                        HOME_ENB_ID_OCTET_SIZE))

            {
                *peer_enodeb_id = o_c;
                retVal  =  X2AP_SUCCESS;
                break;
            }
        }
        if(retVal  ==  X2AP_SUCCESS)
        {
            break;
        }
    }
    if(MAX_PEER_ENB == o_c)
    {
        X2AP_TRACE(X2AP_WARNING,"No serving cell info present in peer eNodeB"
                "Context ECGI value received is invalid");
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_cell_activation_timer_expiry_hdl
 * Inputs         : p_timer_buf - pointer to time buufer data
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function handles timer expiry of cell activation timer.
 ****************************************************************************/
x2ap_return_et x2ap_cell_activation_timer_expiry_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_return_et retVal = X2AP_SUCCESS;
	x2ap_peer_enb_context_t* p_peer_enb_cntx =X2AP_P_NULL;
    U16 trans_id = X2AP_NULL;
	p_peer_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	X2AP_ASSERT(PNULL != p_peer_enb_cntx);
	
    if ((p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state ==
                        X2_CELL_ACT_W_PEER_ENB_RESP_ST) ||
	    (p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state ==
                        X2_CELL_ACT_W_RRM_RESP_ST))
	{
		p_peer_enb_cntx->x2ap_cell_act_sm_ctx.timer_id = X2AP_NULL;

		if(p_peer_enb_cntx->x2ap_cell_act_sm_ctx.retry_count >0)
		{
		    X2AP_TRACE(X2AP_INFO,"X2AP cell act retry count:%u.\n",
                    p_peer_enb_cntx->x2ap_cell_act_sm_ctx.retry_count);
		    p_peer_enb_cntx->x2ap_cell_act_sm_ctx.retry_count--;
		    X2AP_TRACE(X2AP_INFO,"X2AP cell act retry count left:%u.\n",
                    p_peer_enb_cntx->x2ap_cell_act_sm_ctx.retry_count);

		}
		else
		{
			X2AP_TRACE(X2AP_INFO, "X2AP CELL ACTIVATION retry count exhausted\n");
            /*Send cell activation failure*/ 
            X2AP_TRACE(X2AP_INFO, "Sending Cell activation Failure to RRM/SON");
            x2ap_send_cell_activation_failure (p_x2ap_gb_ctx,
                    &p_peer_enb_cntx->gb_enb_id,
                    X2AP_FAILURE,
                    trans_id);
            p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n",
                    p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state);
	}
    return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_send_cell_activation_failure
 * Inputs         : p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  p_peer_gb_enb_id - pointer to peer  gb eNb id 
 *                  response - responce to sent
 *                  trans_id - transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : This function send cell activation failure to RRM/SON
 ****************************************************************************/
void x2ap_send_cell_activation_failure (
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2_gb_enb_id_t* p_peer_gb_enb_id, 
        U8 response,
        U16 trans_id)
{
    x2ap_peer_cell_activation_resp_t peer_cell_act_resp;
    U16 dest_module_id = RRC_RRM_MODULE_ID;
    X2AP_MEMSET(&peer_cell_act_resp, 0x00, 
            sizeof(x2ap_peer_cell_activation_resp_t));
 
    peer_cell_act_resp.peer_cell_activation_resp.bitmask = 0; 
    peer_cell_act_resp.peer_cell_activation_resp.response = response; 
    X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id,
                    sizeof(x2_gb_enb_id_t)); 
    X2AP_MEMCPY(&peer_cell_act_resp.peer_cell_activation_resp.enb2_gb_id, p_peer_gb_enb_id,
            sizeof(x2_gb_enb_id_t));
    
    if(p_x2ap_gb_ctx->is_son_present)
    {
        dest_module_id = RRC_SON_MODULE_ID;
    }
    if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_peer_cell_activation_resp(
                &peer_cell_act_resp,
                RRC_X2AP_MODULE_ID,
                dest_module_id,
                trans_id,
                RRC_NULL))
    {
        X2AP_TRACE(X2AP_ERROR,
                "Failed to send Cell activation Response to RRM/SON.\n");
    }
}

/******************************************************************************
 * Function Name : x2ap_cell_act_compose_asn_x2ap_cause
 * Inputs        : cause - type of x2ap_cause_t
 *                 p_asn_cause - pointer to x2ap_Cause
 * Outputs       : p_asn_cause
 * Returns       : None
 * Description   : This function compose cause.
 *******************************************************************************/
void x2ap_cell_act_compose_asn_x2ap_cause(
        x2ap_cause_t cause, 
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
            X2AP_TRACE(X2AP_ERROR,"Unknown cause type.%u\n", cause.type);
        }break;
    }
}
/******************************************************************************
 * Function Name : x2ap_build_and_send_cell_act_faiure_to_enb
 * Inputs        : p_x2ap_gb_ctx - pointer to X2ao global context
 *                 p_enb_ctx - pointer to peer eNB context
 *                 p_criticality_diagnostics - pointer to critical Diagnostics
 * Outputs       : None 
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE 
 * Description   : This function build nad send cell act failure to peer eNB.
 *******************************************************************************/
x2ap_return_et x2ap_build_and_send_cell_act_faiure_to_enb (
     x2ap_gb_context_t  *p_x2ap_gb_ctx,
     x2ap_peer_enb_context_t *p_enb_ctx,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics)
{
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    rrc_x2ap_cell_activation_failure_t msg_failure;    
    x2ap_return_et result = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();    


    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));
    X2AP_MEMSET(&msg_failure, 0, sizeof(rrc_x2ap_cell_activation_failure_t));
    
    /*As of now p_criticality_diagnostics is NULL*/
    if (PNULL != p_criticality_diagnostics)
    {
        msg_failure.criticality_diagnostics = *p_criticality_diagnostics;
        msg_failure.bitmask |= RRC_X2AP_CELL_ACT_FAILURE_CRIT_DIAGNO_PRESENT;
    }

    msg_failure.cause.t = T_x2ap_Cause_radioNetwork;
    msg_failure.cause.u.radioNetwork = x2ap_unspecified_2; 

    result = x2ap_encode_cell_activation_failure (
            asn_msg_buff,
            &asn_msg_len,
            &msg_failure);
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
            /*Sending the X2AP CELL ACTIVATION RESPONSE to Peer eNB via SCTP */
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
                "[x2ap_build_and_send_cell_act_faiure_to_enb] Encode failure");
        return X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();    
    return result;
}

/******************************************************************************
 * Function Name : x2ap_build_and_send_cell_act_faiure_to_enb_validation_fail
 * Input         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                 p_enb_ctx - pointer to peer eNb context
 *                 p_criticality_diagnostics - pointer to critical diagnostics
 * Outputs       : None
 * Returns       : X2AP_SUCCESS/X2AP_FAILURE
 * Description   : This function build nad send cell act failure to peer eNB.
 *******************************************************************************/
x2ap_return_et x2ap_build_and_send_cell_act_faiure_to_enb_validation_fail (
     x2ap_gb_context_t  *p_x2ap_gb_ctx,
     x2ap_peer_enb_context_t *p_enb_ctx,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics)
{
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    rrc_x2ap_cell_activation_failure_t msg_failure;    
    x2ap_return_et result = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();    


    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));
    X2AP_MEMSET(&msg_failure, 0, sizeof(rrc_x2ap_cell_activation_failure_t));
    
    /*As of now p_criticality_diagnostics is NULL*/
    if (PNULL != p_criticality_diagnostics)
    {
        msg_failure.criticality_diagnostics = *p_criticality_diagnostics;
        msg_failure.bitmask |= RRC_X2AP_CELL_ACT_FAILURE_CRIT_DIAGNO_PRESENT;
    }

    msg_failure.cause.t = T_x2ap_Cause_radioNetwork;
    msg_failure.cause.u.radioNetwork = x2ap_cell_not_available; 

    result = x2ap_encode_cell_activation_failure (
            asn_msg_buff,
            &asn_msg_len,
            &msg_failure);
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
            /*Sending the X2AP CELL ACTIVATION RESPONSE to Peer eNB via SCTP */
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
                "[x2ap_build_and_send_cell_act_faiure_to_enb] Encode failure");
        return X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();    
    return result;
}
/*******************************************************************************
 * Function Name : x2ap_parse_cell_act_cause
 * Inputs        : asn_cause - pointer to x2ap_Cause
 *                 p_cause - pointer to x2ap_cause_t
 * Outputs       : p_cause
 * Returns       : None
 * Description   : This function used to parse x2ap_cause
*******************************************************************************/
void x2ap_parse_cell_act_cause(x2ap_Cause asn_cause, x2ap_cause_t* p_cause)
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
/*spr_fix*/
/*****************************************************************************
 * Function Name  : x2ap_cell_activation_rrm_guard_timer_expiry_hdl
 * Inputs         : p_timer_buf - pointer to timer buffer 
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function handles timer expiry of cell activation timer.
 ****************************************************************************/
x2ap_return_et x2ap_cell_activation_rrm_guard_timer_expiry_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_return_et retVal = X2AP_SUCCESS;
	x2ap_peer_enb_context_t* p_peer_enb_cntx =X2AP_P_NULL;
    
	p_peer_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	X2AP_ASSERT(PNULL != p_peer_enb_cntx);
	
    if (p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state ==
                        X2_CELL_ACT_W_RRM_RESP_ST) 
	{
        p_peer_enb_cntx->x2ap_cell_act_sm_ctx.timer_id = X2AP_NULL;

        /*sending cell activation failure response*/
        X2AP_TRACE(X2AP_INFO, "Sending Cell activation Failure to eNB");

        x2ap_build_and_send_cell_act_faiure_to_enb (p_x2ap_gb_ctx, 
                    p_peer_enb_cntx, X2AP_P_NULL);

        p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST;
    }
	else
	{
		X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n",
                    p_peer_enb_cntx->x2ap_cell_act_sm_ctx.state);
        retVal = X2AP_FAILURE;
	}
    return retVal;
}
/*spr_fix*/

