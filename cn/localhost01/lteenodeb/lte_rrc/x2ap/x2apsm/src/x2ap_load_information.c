/*******************************************************************************
*
*  FILE NAME   : x2ap_load_information.c
*
*  DESCRIPTION : This file contains the load information state machine
*                and the state machine Entry function definition.
*
*  DATE 20-May-2012  NAME Sudhansu Sekhar Nanda
*                      REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/

#include "rrc_x2apRrm_intf.h"
#include "x2ap_db.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_x2apRrm_il_parser.h"
#include "rrc_x2apRrm_il_composer.h"
#include "x2ap_sctp_sm.h"
#include "x2ap_utility.h"
#include "x2ap_error_indication.h"
#include "asn_dec_wrappers.h"
#include "rrc_logging.h"

void  x2ap_process_rrm_load_information(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

static
x2ap_return_et
x2ap_validate_served_cell_id_and_get_peer_enb_context(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_rrm_load_information_req_t *p_x2ap_load_info
        );

static
x2ap_return_et
x2ap_validate_target_cell_id_and_get_peer_enb_context(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_high_intf_info,
        U8 *peer_enodeb_id,
        U8 *serving_cell_index
        );

x2ap_return_et x2ap_encode_load_information(
        x2ap_rrm_load_information_req_t *p_load_info,
        U8                    *p_asn_msg,
        U16                   *p_asn_msg_len);

x2ap_return_et x2ap_compose_UL_High_intf_Info(
        OSCTXT              *p_asn1_ctx,
        U16  count,
        x2ap_rrm_li_ul_high_interference_info_t *p_ul_high_intf_info,
        x2ap_UL_HighInterferenceIndicationInfo  *p_x2ap_high_intf_info);

rrc_return_et x2ap_decode_load_information(
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_rrm_load_information_ind_t *p_load_info,
        x2ap_X2AP_PDU  x2ap_pdu);

x2ap_return_et x2ap_parse_ul_high_intf_ind(
        OSRTDListNode* p_node,
        x2ap_rrm_li_ul_high_interference_info_t  *p_ul_high_interference_info,
        U16  count_3,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id,
        x2ap_bool_et *p_send_tse);

x2ap_return_et x2ap_process_load_information_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_send_load_information_to_rrm(
        x2ap_rrm_load_information_resp_t
        *p_load_info_resp,
        x2ap_return_et     retval);                  
x2ap_return_et x2ap_load_info_compose_cell_information(
        OSCTXT*       p_asn1_ctx,
        OSRTDList*    p_x2ap_cell_information,
        x2ap_rrm_load_information_req_t*   p_load_info);

extern x2ap_return_et  x2ap_sctp_sm_entry(
         U16                api,
         U8                 *p_msg_ptr,
         x2ap_gb_context_t *p_x2ap_gb_ctx,
         x2ap_peer_enb_context_t *p_enb_cntxt_arr);

/*****************************************************************************
 * Function Name  : x2ap_process_rrm_load_information
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the load information request
 *                  received from RRM .
 ****************************************************************************/
void x2ap_process_rrm_load_information(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    x2ap_rrm_load_information_req_t  *p_x2ap_load_info = X2AP_P_NULL;
    x2ap_rrm_load_information_resp_t *p_load_info_resp = X2AP_P_NULL;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    x2ap_peer_enb_context_t* p_peer_enb_ctx = X2AP_P_NULL;
    S32   length_read = X2AP_NULL;
    x2ap_return_et   response = X2AP_FAILURE;
    x2ap_return_et   retval = X2AP_SUCCESS;
    U8 peer_enodeb_id = X2AP_NULL;
    U8 serving_cell_index = X2AP_NULL;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = X2AP_NULL;
    U32 counter = 0;

    X2AP_UT_TRACE_ENTER();
    
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));    

    p_x2ap_load_info = (x2ap_rrm_load_information_req_t*)
        x2ap_mem_get(sizeof(x2ap_rrm_load_information_req_t));
    if(X2AP_P_NULL == p_x2ap_load_info)
    {
        X2AP_TRACE(X2AP_FATAL,"[%s]\nMemory allocation failed ",__FUNCTION__);
        return;
    }
    X2AP_MEMSET(p_x2ap_load_info, X2AP_NULL, sizeof(x2ap_rrm_load_information_req_t));

    p_load_info_resp = (x2ap_rrm_load_information_resp_t *)x2ap_mem_get(sizeof(x2ap_rrm_load_information_req_t));
    if(X2AP_P_NULL == p_load_info_resp)
    {
        /* coverity_fix_63313_start */
        x2ap_mem_free((void *)p_x2ap_load_info);
        /* coverity_fix_63313_stop */
        X2AP_TRACE(X2AP_FATAL,"[%s]\nMemory allocation failed ",__FUNCTION__);
        return;
    }

    /* Decode the received message from RRM */
    response = rrc_il_parse_x2ap_rrm_load_information_req(
            p_x2ap_load_info ,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if(X2AP_SUCCESS == response)
    {
        /* 2. Validate served Cell IDs. If invalid, return failure. */
        if (X2AP_FAILURE ==
                x2ap_validate_served_cell_id_and_get_peer_enb_context(p_x2ap_gb_ctx,
                    p_x2ap_load_info))
        {
            X2AP_TRACE(X2AP_WARNING, "validation of source cell id failed");
            retval = X2AP_FAILURE;
        }

        /* 2. Validate target Cell IDs. If invalid, return failure. */
        if ((retval) && (p_x2ap_load_info->cell_information[0].bitmask & 
                X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT))
        {
            if (X2AP_FAILURE ==
                    x2ap_validate_target_cell_id_and_get_peer_enb_context(p_x2ap_gb_ctx,
                        &p_x2ap_load_info->cell_information[0].ul_high_interference_info,
                        &peer_enodeb_id,
                        &serving_cell_index))
            {
                X2AP_TRACE(X2AP_WARNING, "validation of target cell id failed");
                retval = X2AP_FAILURE;
            }
        }
        if (retval && X2AP_FAILURE == x2ap_encode_load_information(p_x2ap_load_info,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            X2AP_TRACE(X2AP_WARNING, "x2ap load information encode failed");
            X2AP_UT_TRACE_EXIT();
        }
        else if(retval)
        {
            /*Fetch peer eNB context*/
            for(counter = 0; counter < MAX_PEER_ENB; counter++)
            {
                if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                                &p_x2ap_load_info->gb_enb_id, 
                                sizeof(x2_gb_enb_id_t))))
                {
                    p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                    break;
                }
            }

            if(p_peer_enb_ctx == X2AP_P_NULL)
            {
                retval = X2AP_FAILURE; 
                X2AP_TRACE(X2AP_WARNING, "No peer eNB context found");
            }   
            else
            {
              peer_enb_msg.p_msg = asn_msg_buff;
              peer_enb_msg.msg_len = asn_msg_len;
	      /* SPR_14436_Fix Start */
	      peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
		      stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
	      /* SPR_14436_Fix Stop */

              if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
              {
                  /*Sending the load information to Peer eNB via SCTP */
                  retval = x2ap_sctp_sm_entry
                      (X2AP_SCTP_SEND_DATA_EV,
                       (U8*)&peer_enb_msg,
                       p_x2ap_gb_ctx,
                      p_peer_enb_ctx);
              }
              
            }
        }
    }    
    else
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap RRM Load Information parsing failed");
        retval = X2AP_FAILURE;
    }

    x2ap_send_load_information_to_rrm(p_load_info_resp,retval);
    /* coverity_fix_63313_start */
    x2ap_mem_free((void *)p_x2ap_load_info);
    /* coverity_fix_63313_stop */
    /* coverity_fix_63314_start */
    x2ap_mem_free((void *)p_load_info_resp);
    /* coverity_fix_63314_stop */
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_validate_served_cell_id_and_get_peer_enb_context 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_x2ap_load_info - pointer to x2ap_rrm_load_information_req_t 
 * Outputs        : p_x2ap_load_info
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function validate the ecgi for the served cell of peer 
 *                  eNB,received in 
 ********************************************************************************/
static
x2ap_return_et
x2ap_validate_served_cell_id_and_get_peer_enb_context
(
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_rrm_load_information_req_t *p_x2ap_load_info
)
{
    U8 count = 0;
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_load_info);

    for(count = 0; count < p_x2ap_gb_ctx->num_served_cells; count++)

    {
        /*SPR_20083_start*/
        /* SPR 13531 Fix Start */
        if (!(X2AP_MEMCMP(p_x2ap_load_info->cell_information[0].source_cell_id.cell_identity,
            p_x2ap_gb_ctx->served_cell_info[p_x2ap_gb_ctx->act_cell_info[count].cell_id].ecgi.eutran_cell_id,
                        MACRO_ENB_ID_OCTET_SIZE))) /* SPR 13531 Fix Stop */

        {
            if ((p_x2ap_load_info->cell_information[0].source_cell_id.cell_identity[3] & 0xF0) ==
                    (p_x2ap_gb_ctx->served_cell_info[p_x2ap_gb_ctx->act_cell_info[count].cell_id].ecgi.eutran_cell_id[3] & 0xF0))
            {

                /*SPR_20083_stop*/

                retVal  =  X2AP_SUCCESS;
                break;
            }
        }
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_validate_target_cell_id_and_get_peer_enb_context
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context  
 *                  p_x2ap_high_intf_info - pointer to 
 *                                      x2ap_rrm_li_ul_high_interference_info_t
 *                  peer_enodeb_id - PEER eNB id
 *                  serving_cell_index - cell index 
 * Outputs        : p_x2ap_high_intf_info 
 * RETURNS        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function validate the ecgi for the target cell of peer eNB,
 *       received in 
 ********************************************************************************/
static
x2ap_return_et
x2ap_validate_target_cell_id_and_get_peer_enb_context
(
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_high_intf_info,
 U8 *peer_enodeb_id,
 U8 *serving_cell_index
 )
{
    U8 o_c, i_c = X2AP_NULL;
    U16 num_served_cells = X2AP_NULL;
    void* p_srvd_cell_info = X2AP_P_NULL;
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_high_intf_info);

    for(o_c = 0; o_c < MAX_PEER_ENB; o_c ++)
    {
        if(X2AP_TRUE ==
                p_x2ap_gb_ctx->x2ap_link_info[o_c].
                connection_status)
        {

            if( X2AP_P_NULL != (p_srvd_cell_info =
                        p_x2ap_gb_ctx->x2ap_link_info[o_c].p_srvd_cell_info))
            {
                num_served_cells = p_x2ap_gb_ctx->x2ap_link_info[o_c].num_served_cells;

                for(i_c = 0 ;i_c < num_served_cells; i_c++)
                {
                    if (0 ==
                            /*Bug_12881_fix_start*/
                            X2AP_MEMCMP(&p_x2ap_high_intf_info->ul_high_intf_info[0].target_cell_id.cell_identity,
                                (((x2ap_ecgi_t*)((U8*)p_srvd_cell_info +(sizeof(x2ap_ecgi_t)* i_c)))->eutran_cell_id),
                                 HOME_ENB_ID_OCTET_SIZE))
                            /*Bug_12881_fix_stop*/
                                             
                    {
                        *peer_enodeb_id = o_c;
                        *serving_cell_index = i_c;
                        retVal  =  X2AP_SUCCESS;
                        break;
                    }
                }

                if(retVal  ==  X2AP_SUCCESS)
                {
                    break;
                }
            }
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

/*******************************************************************************
 * Function Name : x2ap_encode_load_information
 * Inputs        : p_load_info - pointer to x2ap_rrm_load_information_req_t
 *                 p_asn_msg - pointer to asn msg
 *                 p_asn_msg_len - pointer to asn msg length 
 * Outputs       : p_asn_msg , p_asn_msg_len   
 * RETURNS       : X2AP_SUCCESS or X2AP_FAILURE
 * DESCRIPTION : This function used to encode Load Information.
 * ERROR : This function report error to the caller function, If encoder
 *         fails to encode message.
 *******************************************************************************/
x2ap_return_et x2ap_encode_load_information(
        x2ap_rrm_load_information_req_t *p_load_info,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len)
{
    OSCTXT              asn1_ctx;
    OSRTDList           list_1;
    OSRTDListNode       *p_node = PNULL;
    x2ap_X2AP_PDU       x2ap_pdu;
    x2ap_return_et         retVal = X2AP_FAILURE;
    x2ap_LoadInformation   *p_x2ap_loadInformation;
    x2ap_LoadInformation_protocolIEs_element* p_protocolIE_elem;

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

        x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_loadIndication;
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

        /* Set the initiating message type to X2 Setup */ 
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__loadIndication;

        p_x2ap_loadInformation = rtxMemAllocType(&asn1_ctx, x2ap_LoadInformation);

        if (X2AP_P_NULL == p_x2ap_loadInformation)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"Mem Alloc successful to p_x2ap_loadInformation");
        }

        asn1Init_x2ap_LoadInformation (p_x2ap_loadInformation);
        x2ap_pdu.u.initiatingMessage->value.u.loadIndication = p_x2ap_loadInformation;

        rtxDListAllocNodeAndData(&asn1_ctx,                                         
                                 x2ap_LoadInformation_protocolIEs_element,
                                 &p_node,
                                 &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {                                                                         
            /* Not enough memory */                                               
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;                                                              
        }                                                                       

        asn1Init_x2ap_LoadInformation_protocolIEs_element (p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */                                   
        p_protocolIE_elem->id = ASN1V_x2ap_id_CellInformation;
        p_protocolIE_elem->criticality = x2ap_ignore;

        /* Fill the type of Protocol IE element */                                  
        rtxDListInit(&list_1);

        p_protocolIE_elem->value.u._x2ap_LoadInformation_IEs_1 = rtxMemAllocType(&asn1_ctx,x2ap_CellInformation_List);
        p_protocolIE_elem->value.t =  T27x2ap___x2ap_LoadInformation_IEs_1;
        
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_LoadInformation_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_CellInformation_List(p_protocolIE_elem->value.u._x2ap_LoadInformation_IEs_1 );


        if (X2AP_FAILURE == 
                x2ap_load_info_compose_cell_information(&asn1_ctx, 
                    p_protocolIE_elem->value.u._x2ap_LoadInformation_IEs_1, 
                    p_load_info))
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Compose cell info Failure.", __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "%s:Compose cell info Success.", __FUNCTION__);
        }

        rtxDListAppendNode(&p_x2ap_loadInformation->protocolIEs, p_node);
        
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of Load information failed.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;

            
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Load Information",
                &x2ap_pdu);
        }
    }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_load_info_compose_cell_information 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_x2ap_cell_information - pointer to OSRTDList
 *                  p_load_info - pointer to x2ap_rrm_load_information_req_t
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function composes cell information  
 ********************************************************************************/
x2ap_return_et x2ap_load_info_compose_cell_information(
        OSCTXT*       p_asn1_ctx,
        OSRTDList*    p_x2ap_cell_information,
        x2ap_rrm_load_information_req_t*   p_load_info)
{
	U16  count = X2AP_NULL;
	OSRTDListNode*    p_node_cell_information = X2AP_P_NULL;
	x2ap_CellInformation_List_element* p_cell_information = X2AP_P_NULL;
	/* eICIC changes start */
	U16  counter = X2AP_NULL;
	OSRTDListNode*    p_node_cellinfo_item_extIEs_1 = X2AP_P_NULL;
	x2ap_CellInformation_Item_iE_Extensions_element* p_cellinfo_item_extIEs_1 = X2AP_P_NULL;
	OSRTDListNode*    p_node_cellinfo_item_extIEs_2 = X2AP_P_NULL;
	x2ap_CellInformation_Item_iE_Extensions_element* p_cellinfo_item_extIEs_2 = X2AP_P_NULL;
	/* eICIC changes stop */

	/* filling cell information */
	for (count = 0; count < p_load_info->cell_information_count; count++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx,
				x2ap_CellInformation_List_element,
				&p_node_cell_information,
				&p_cell_information);

		if (X2AP_P_NULL == p_node_cell_information)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}

		asn1Init_x2ap_CellInformation_List_element(p_cell_information);

		p_cell_information->id = ASN1V_x2ap_id_CellInformation_Item;
		p_cell_information->criticality = x2ap_ignore;

		/*1. Compose x2ap_ECGI */
		p_cell_information->value.cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;

		x2ap_compose_asn_plmn_identity(
				p_cell_information->value.cell_ID.pLMN_Identity.data,
				&(p_load_info->cell_information[count].source_cell_id.plmn_identity));

		p_cell_information->value.cell_ID.eUTRANcellIdentifier.numbits = CELL_ID_NBITS;
		X2AP_MEMCPY(p_cell_information->value.cell_ID.eUTRANcellIdentifier.data,
				p_load_info->cell_information[count].source_cell_id.cell_identity,
				sizeof(U32));

		/*2. Compose x2ap_UL_InterferenceOverloadIndication */
		if(p_load_info->cell_information[count].bitmask &
				X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT)
		{
			U16 ul_intf_overload_ind_count =0; 
			X2AP_TRACE(X2AP_INFO,
					"%s: UL interference overload indication present.",__FUNCTION__);
			ul_intf_overload_ind_count = 
				p_load_info->cell_information[count].ul_intf_overload_ind.count;

			p_cell_information->value.m.ul_InterferenceOverloadIndicationPresent = 1;
			p_cell_information->value.ul_InterferenceOverloadIndication.n = 
				ul_intf_overload_ind_count;
			for(counter=0; counter<ul_intf_overload_ind_count; counter++)
			{
				p_cell_information->value.ul_InterferenceOverloadIndication.elem[counter] =
					p_load_info->cell_information[count].ul_intf_overload_ind.ul_intf_overload_indication[counter];
			}
		}
		else
		{
			X2AP_TRACE(X2AP_INFO,
					"%s: UL interference overload indication absent.",__FUNCTION__);
		}

		/*3. Compose x2ap_UL_HighInterferenceIndicationInfo */
		if(p_load_info->cell_information[count].bitmask &
				X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT)
		{
			X2AP_TRACE(X2AP_INFO,
					"%s: Ul High Intereference info present.",__FUNCTION__);

			p_cell_information->value.m.ul_HighInterferenceIndicationInfoPresent = 1;
			if(X2AP_FAILURE == x2ap_compose_UL_High_intf_Info(
						p_asn1_ctx,
						p_load_info->cell_information[count].
						ul_high_interference_info.ul_high_interference_info_count,
						&p_load_info->cell_information[count].ul_high_interference_info,
						&p_cell_information->value.ul_HighInterferenceIndicationInfo))
			{
				X2AP_TRACE(X2AP_ERROR,
						"%s:x2ap_compose_UL_High_intf_Info returned Failure.",
						__FUNCTION__);
				break;
			}
		}
		else
		{
			X2AP_TRACE(X2AP_INFO,
					"%s: Ul High Intereference info absent.",__FUNCTION__);
		}

		/*4. Compose x2ap_RelativeNarrowbandTxPower */
		if(p_load_info->cell_information[count].bitmask &
				X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT)
		{
			X2AP_TRACE(X2AP_INFO, "%s: RNTP present.",__FUNCTION__);

			p_cell_information->value.m.relativeNarrowbandTxPowerPresent = 1;
			p_cell_information->value.relativeNarrowbandTxPower.rNTP_PerPRB.numbits = 
				p_load_info->cell_information[count].relative_narrowband_tx_power.prb_count;

			p_cell_information->value.relativeNarrowbandTxPower.rNTP_PerPRB.data =
				rtxMemAlloc(p_asn1_ctx,sizeof(p_load_info->cell_information[count].relative_narrowband_tx_power.rntp_per_prb));
			/* SPR 12964 Fix Start */
			if(X2AP_P_NULL == p_cell_information->value.relativeNarrowbandTxPower.rNTP_PerPRB.data)
			{
                X2AP_TRACE(X2AP_ERROR, "[%s]: Unable to allocate memory",__FUNCTION__);
				return X2AP_FAILURE;
			}
			/* SPR 12964 Fix Stop */

			X2AP_MEMCPY ((void *)p_cell_information->value.relativeNarrowbandTxPower.rNTP_PerPRB.data,
					p_load_info->cell_information[count].relative_narrowband_tx_power.rntp_per_prb,
					sizeof(p_load_info->cell_information[count].relative_narrowband_tx_power.rntp_per_prb));

			p_cell_information->value.relativeNarrowbandTxPower.rNTP_Threshold = p_load_info->cell_information[count].
				relative_narrowband_tx_power.rntp_threshold;

			p_cell_information->value.relativeNarrowbandTxPower.numberOfCellSpecificAntennaPorts =
				p_load_info->cell_information[count].
				relative_narrowband_tx_power.num_cell_specific_antena_port;

			p_cell_information->value.relativeNarrowbandTxPower.p_B = p_load_info->cell_information[count].
				relative_narrowband_tx_power.p_b;

			p_cell_information->value.relativeNarrowbandTxPower.pDCCH_InterferenceImpact = p_load_info->
				cell_information[count].relative_narrowband_tx_power.
				pdcch_interference_impact;
		}
		else
		{
			X2AP_TRACE(X2AP_INFO, "%s: RNTP is absent.",__FUNCTION__);
		}

		/* eICIC changes start */
		/*5. Compose x2ap_ABSInformation */
		if(p_load_info->cell_information[count].bitmask &
				X2AP_LI_ABS_INFORMATION_PRESENT)
		{
			X2AP_TRACE(X2AP_INFO, "%s: ABS Info present.",__FUNCTION__);
			p_cell_information->value.m.iE_ExtensionsPresent = 1;


			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_CellInformation_Item_iE_Extensions_element,
					&p_node_cellinfo_item_extIEs_1,
					&p_cellinfo_item_extIEs_1);

			if (X2AP_P_NULL == p_node_cellinfo_item_extIEs_1)
			{
				X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}
			asn1Init_x2ap_CellInformation_Item_iE_Extensions_element(p_cellinfo_item_extIEs_1);

			p_cellinfo_item_extIEs_1->id = ASN1V_x2ap_id_ABSInformation;
			p_cellinfo_item_extIEs_1->criticality = x2ap_ignore;

			p_cellinfo_item_extIEs_1->extensionValue.t = T29x2ap___x2ap_CellInformation_Item_ExtIEs_1;

			p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1 = 
				rtxMemAllocType(p_asn1_ctx,x2ap_ABSInformation);
			if (X2AP_P_NULL == p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1)
			{
				X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}
			else
			{
				X2AP_TRACE(X2AP_INFO,"Mem Alloc successful to x2ap_CellInformation_Item_ExtIEs_1");
			}
			asn1Init_x2ap_ABSInformation(p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1);

			if(p_load_info->cell_information[count].abs_infomation.bitmask & X2AP_LI_ABS_INFORMATION_FDD_PRESENT)
			{
				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->t = T_x2ap_ABSInformation_fdd; 
				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd =
					rtxMemAllocType(p_asn1_ctx, x2ap_ABSInformationFDD);
				if (X2AP_P_NULL == p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd)
				{
					X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
					X2AP_UT_TRACE_EXIT();
					return X2AP_FAILURE;
				}
				else
				{
					X2AP_TRACE(X2AP_INFO,"Mem Alloc successful to x2ap_ABSInformationFDD");
				}
				asn1Init_x2ap_ABSInformationFDD(p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd);

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd->numberOfCellSpecificAntennaPorts =
					p_load_info->cell_information[count].abs_infomation.abs_info_fdd.no_cell_specific_antenna_port;

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd->
					abs_pattern_info.numbits = MAX_SUBFRAME_CONFIG_FDD_NUMBITS;

				X2AP_MEMCPY((void *)p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd->abs_pattern_info.data,
						p_load_info->cell_information[count].abs_infomation.abs_info_fdd.abs_pattern_info,
						sizeof(p_load_info->cell_information[count].abs_infomation.abs_info_fdd.abs_pattern_info));

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd->
					measurement_subset.numbits = MAX_SUBFRAME_CONFIG_FDD_NUMBITS;

				X2AP_MEMCPY((void *)p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.fdd->measurement_subset.data,
						p_load_info->cell_information[count].abs_infomation.abs_info_fdd.measurement_subset,
						sizeof(p_load_info->cell_information[count].abs_infomation.abs_info_fdd.measurement_subset));
			}
			else if(p_load_info->cell_information[count].abs_infomation.bitmask & X2AP_LI_ABS_INFORMATION_TDD_PRESENT)
			{

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->t = T_x2ap_ABSInformation_tdd; 
				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd = 
					rtxMemAllocType(p_asn1_ctx, x2ap_ABSInformationTDD);
				if (X2AP_P_NULL == p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd)
				{
					X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
					X2AP_UT_TRACE_EXIT();
					return X2AP_FAILURE;
				}
				else
				{
					X2AP_TRACE(X2AP_INFO,"Mem Alloc successful to p_x2ap_loadInformation");
				}
				asn1Init_x2ap_ABSInformationTDD(p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd);

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->numberOfCellSpecificAntennaPorts =
					p_load_info->cell_information[count].abs_infomation.abs_info_tdd.no_cell_specific_antenna_port;

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->
					abs_pattern_info.numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->abs_pattern_info.data =
					rtxMemAlloc(p_asn1_ctx,(sizeof(U8)*X2AP_ABS_PATTERN_SIZE_TDD));
				if(X2AP_P_NULL == p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->abs_pattern_info.data)
				{
					X2AP_TRACE(X2AP_ERROR, "[%s] Unable to allocate memory",__FUNCTION__);
					return X2AP_FAILURE;
				}
				X2AP_MEMCPY((void *)p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->abs_pattern_info.data,
						p_load_info->cell_information[count].abs_infomation.abs_info_tdd.abs_pattern_info,
						sizeof(p_load_info->cell_information[count].abs_infomation.abs_info_tdd.abs_pattern_info));

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->
					measurement_subset.numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;

				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->measurement_subset.data =
					rtxMemAlloc(p_asn1_ctx,(sizeof(p_load_info->cell_information[count].abs_infomation.abs_info_tdd.
									measurement_subset)*X2AP_MEASUREMENT_SUBSET_SIZE_TDD));
				if(X2AP_P_NULL == p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->measurement_subset.data)
				{
					X2AP_TRACE(X2AP_ERROR, "[%s] Unable to allocate memory",__FUNCTION__);
					return X2AP_FAILURE;
				}

				X2AP_MEMCPY((void *)p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->u.tdd->measurement_subset.data,
						p_load_info->cell_information[count].abs_infomation.abs_info_tdd.measurement_subset,
						sizeof(p_load_info->cell_information[count].abs_infomation.abs_info_tdd.measurement_subset));

			}
			else if(p_load_info->cell_information[count].abs_infomation.bitmask & X2AP_LI_ABS_INFORMATION_ABS_INACTIVE_PRESENT)
			{
				p_cellinfo_item_extIEs_1->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->t = T_x2ap_ABSInformation_abs_inactive;
			}

			rtxDListAppendNode(&p_cell_information->value.iE_Extensions,p_node_cellinfo_item_extIEs_1);
		}
		else
		{
			X2AP_TRACE(X2AP_INFO, "%s: ABS Info is absent.",__FUNCTION__);

		}

		/*6. Compose x2ap_InvokeIndication */
		if(p_load_info->cell_information[count].bitmask &
				X2AP_LI_INVOKE_INDICATION_PRESENT)
		{
			X2AP_TRACE(X2AP_INFO, "%s: Invoke Ind present.",__FUNCTION__);
			p_cell_information->value.m.iE_ExtensionsPresent = 1;

			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_CellInformation_Item_iE_Extensions_element,
					&p_node_cellinfo_item_extIEs_2,
					&p_cellinfo_item_extIEs_2);

			if (X2AP_P_NULL == p_node_cellinfo_item_extIEs_2)
			{
				X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}
			asn1Init_x2ap_CellInformation_Item_iE_Extensions_element(p_cellinfo_item_extIEs_2);

			p_cellinfo_item_extIEs_2->id = ASN1V_x2ap_id_InvokeIndication;
			p_cellinfo_item_extIEs_2->criticality = x2ap_ignore;

			p_cellinfo_item_extIEs_2->extensionValue.t = T29x2ap___x2ap_CellInformation_Item_ExtIEs_2;

			p_cellinfo_item_extIEs_2->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_2 =  
				p_load_info->cell_information[count].invoke_indication.invoke_ind;

			rtxDListAppendNode(&p_cell_information->value.iE_Extensions,p_node_cellinfo_item_extIEs_2);

		}
		else
		{
			X2AP_TRACE(X2AP_INFO, "%s: Invoke Ind is absent.",__FUNCTION__);
		}
		/* eICIC changes stop */

		rtxDListAppendNode(p_x2ap_cell_information, p_node_cell_information);
	}
	return X2AP_SUCCESS;
}
/******************************************************************************
 * Function Name  : x2ap_compose_UL_High_intf_Info.
 * Input          : p_asn1_ctx - pointer to OSCTXT
 *                  ul_hi_intf_count - UL hi interface count 
 *                  p_ul_high_intf_info - pointer to 
 *                                   x2ap_rrm_li_ul_high_interference_info_t
 *                  p_x2ap_high_intf_info - pointer to
 *                                      x2ap_UL_HighInterferenceIndicationInfo
 * Output         : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE.
 * Description    : This function populates Ul High Interference indication.
 ******************************************************************************/

x2ap_return_et x2ap_compose_UL_High_intf_Info(
        OSCTXT              *p_asn1_ctx,
        U16  ul_hi_intf_count,
        x2ap_rrm_li_ul_high_interference_info_t *p_ul_high_intf_info,
        x2ap_UL_HighInterferenceIndicationInfo  *p_x2ap_high_intf_info)
{
    OSRTDListNode               *p_node = X2AP_P_NULL;
    x2ap_UL_HighInterferenceIndicationInfo_Item  *p_ul_high_intf_item;
    U16    count;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_asn1_ctx);
    X2AP_ASSERT(PNULL != p_ul_high_intf_info);
    
    for (count = 0; count < ul_hi_intf_count; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_UL_HighInterferenceIndicationInfo_Item,
                &p_node,
                &p_ul_high_intf_item);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_UL_HighInterferenceIndicationInfo_Item(p_ul_high_intf_item);

        p_ul_high_intf_item->target_Cell_ID.pLMN_Identity.numocts =
            MAX_PLMN_ID_BYTES;
        x2ap_compose_asn_plmn_identity(
                p_ul_high_intf_item->target_Cell_ID.pLMN_Identity.data,
                &p_ul_high_intf_info->ul_high_intf_info[count].
                target_cell_id.plmn_identity);

        p_ul_high_intf_item->target_Cell_ID.eUTRANcellIdentifier.numbits =
            CELL_ID_NBITS;
        X2AP_MEMCPY(p_ul_high_intf_item->target_Cell_ID.eUTRANcellIdentifier.data,
                p_ul_high_intf_info->ul_high_intf_info[count].
                target_cell_id.cell_identity,
                sizeof(U32));

        p_ul_high_intf_item->ul_interferenceindication.numbits =
            p_ul_high_intf_info->ul_high_intf_info[count].ul_high_intf_ind.prb_count;

       p_ul_high_intf_item->ul_interferenceindication.data = 
           rtxMemAlloc(p_asn1_ctx, sizeof(p_ul_high_intf_info->ul_high_intf_info[count].ul_high_intf_ind.hii));
        /* SPR 12964 Fix Start */
        if(X2AP_P_NULL == p_ul_high_intf_item->ul_interferenceindication.data)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /* SPR 12964 Fix Stop */
        
        X2AP_MEMCPY((void *)p_ul_high_intf_item->ul_interferenceindication.data,
                p_ul_high_intf_info->ul_high_intf_info[count].ul_high_intf_ind.hii,
                sizeof(p_ul_high_intf_info->ul_high_intf_info[count].ul_high_intf_ind.hii));
        
        rtxDListAppendNode(p_x2ap_high_intf_info,p_node);
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}


/*****************************************************************************
 * Function Name  : x2ap_decode_load_information. 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context 
 *                  p_enb_ctx - pointer to x2ap_peer_enb_context_t
 *                  pctxt - pointer to OSCTXT
 *                  p_load_info - pointer to x2ap_rrm_load_information_ind_t
 *                  x2ap_pdu - pointer to x2ap_X2AP_PDU
 * Outputs        : p_load_info 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function used to decode Load Information.
 * Error          : This function report error to the caller function, If decoder
 * fails to decode message.
 ********************************************************************************/
rrc_return_et x2ap_decode_load_information(
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        OSCTXT  *pctxt,
        x2ap_rrm_load_information_ind_t *p_load_info,
        x2ap_X2AP_PDU  x2ap_pdu)
{

	rrc_return_et retval = RRC_SUCCESS;
	x2ap_LoadInformation_protocolIEs_element* p_elem1;
	x2ap_CellInformation_List_element* p_elem2;
	/* eICIC changes start */
	x2ap_CellInformation_Item_iE_Extensions_element* p_elem3;
	/* eICIC changes stop */
	OSRTDListNode*p_node1 = PNULL;
	OSRTDListNode*p_node2 = PNULL;
	OSRTDListNode*p_node3 = PNULL;
	/* eICIC changes start */
	OSRTDListNode*p_node4 = PNULL;
	U16 count_4 = X2AP_NULL;
	U16 index_3 = X2AP_NULL;
	/* eICIC changes stop */
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	x2ap_bool_et send_tse = X2AP_FALSE;
	U16  ie_list_index = X2AP_NULL;
	U16 index_1;
	U16 index_2;
	U16 count_1;
	U16 count_2;
	U16 count_3;

	x2ap_message_data_t message_map =
	{1, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_CellInformation, x2ap_mandatory, x2ap_ignore, X2AP_NULL,
			 X2AP_NULL, X2AP_NULL, X2AP_NULL}}};
	/*ARGUSED*/

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(PNULL != p_load_info);

	X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
	X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));

	p_node1 =
		x2ap_pdu.u.initiatingMessage->value.u.loadIndication->protocolIEs.head;
	count_1 =
		x2ap_pdu.u.initiatingMessage->value.u.loadIndication->protocolIEs.count;

	for (index_1 = 0; index_1 < count_1; index_1++)
	{
		p_elem1 = (x2ap_LoadInformation_protocolIEs_element*)p_node1->data;

		if(p_elem1 == NULL)
		{
			X2AP_TRACE(X2AP_WARNING,"%s: p_elem1 is NULL",__FUNCTION__);
			return X2AP_FAILURE;
		}

		switch(p_elem1->id)
		{
			case ASN1V_x2ap_id_CellInformation:
				{
					if (X2AP_SUCCESS == x2ap_validate_ie_value(
								&message_map,
								index_1,
								p_elem1->id,
								p_elem1->value.u._x2ap_LoadInformation_IEs_1))
					{ 
						if(p_elem1->id!= ASN1V_x2ap_id_CellInformation)
						{
							X2AP_TRACE(X2AP_WARNING,
									"p_elem1!=ASN1V_x2ap_id_CellInformation"
									"p_elem1->id (%i)",
									p_elem1->id);
							retval = X2AP_FAILURE;
							break;

						}

						p_node2 = p_elem1->value.u._x2ap_LoadInformation_IEs_1->head;
						count_2 = p_elem1->value.u._x2ap_LoadInformation_IEs_1->count;
						if(X2AP_MAX_IES <= count_2)
						{
							X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
									" in ASN.1 Sequence, Sending Transfer Syntax Error");
							x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
							retval = X2AP_FAILURE;
							break;
						}

						for (index_2 = 0; index_2 < count_2 && index_2 <
								MAX_SERVED_CELLS ; index_2++)
						{
							p_elem2 = (x2ap_CellInformation_List_element*)p_node2->data;
							if(p_elem2 == NULL)
							{
								X2AP_TRACE(X2AP_WARNING,"%s: p_elem1 is NULL",__FUNCTION__);
								return X2AP_FAILURE;
							}

							if(p_elem2->id == ASN1V_x2ap_id_CellInformation_Item)
							{
								p_node3 = p_elem2->value.ul_HighInterferenceIndicationInfo.head;
								count_3 = p_elem2->value.ul_HighInterferenceIndicationInfo.count;

								p_load_info->cell_information_count = count_2;

								/* filling cell-id */
								x2ap_parse_asn_plmn_identity(
										&p_load_info->cell_load_info[index_2].source_cell_id.plmn_identity,
										p_elem2->value.cell_ID.pLMN_Identity.data);

								X2AP_MEMCPY(
										&p_load_info->cell_load_info[index_2].source_cell_id.cell_identity,
										&p_elem2->value.cell_ID.eUTRANcellIdentifier.data,
										sizeof(U32));

								/*filling UL InterferenceOverloadIndication*/
								if (p_elem2->value.m.ul_InterferenceOverloadIndicationPresent)
								{
									U16 ul_intf_overload_ind_count = p_elem2->value.ul_InterferenceOverloadIndication.n;
									p_load_info->cell_load_info[index_2].ul_intf_overload_ind.count = ul_intf_overload_ind_count; 
									p_load_info->cell_load_info[index_2].bitmask |= X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT;
									U16 counter = X2AP_NULL;
									for(counter =0; counter<ul_intf_overload_ind_count; counter++)
									{
										p_load_info->cell_load_info[index_2].ul_intf_overload_ind.ul_intf_overload_indication[counter] =
											p_elem2->value.ul_InterferenceOverloadIndication.elem[counter];
									}

								}
								if(p_elem2->value.m.ul_HighInterferenceIndicationInfoPresent)
								{
									if ( X2AP_FAILURE == x2ap_parse_ul_high_intf_ind(
												p_node3,
												&p_load_info->cell_load_info[index_2].ul_high_interference_info,
												count_3,
												&message_map,
												(U8)index_1,
												(U8)p_elem1->id,
												&send_tse))
									{
										X2AP_TRACE(X2AP_ERROR,
												"%s:x2ap_x2ap_parse_ul_high_intf_ind returned Failure.",
												__FUNCTION__);
										break;
									}
									/* This will only happen when the element list exceeds
									 * the max number */
									if (X2AP_TRUE == send_tse)
									{
										X2AP_TRACE(X2AP_WARNING, "PARSE Error is Detected in "
												"ASN.1 Sequence, Sending Transfer Syntax Error");
										x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
										retval = X2AP_FAILURE;
										break;
									}

									p_load_info->cell_load_info[index_2].bitmask |= 
										X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT;
								}

								/*filling  RNTP*/
								if (p_elem2->value.m.relativeNarrowbandTxPowerPresent)
								{
									U16 count = 0;
									U16 prb_octet_count = 
										((p_elem2->value.relativeNarrowbandTxPower.rNTP_PerPRB.numbits)%8?
										 (((p_elem2->value.relativeNarrowbandTxPower.rNTP_PerPRB.numbits)/8)+1):
										 ((p_elem2->value.relativeNarrowbandTxPower.rNTP_PerPRB.numbits)/8));

									p_load_info->cell_load_info[index_2].bitmask |= 
										X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT;

									p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.prb_count =
										p_elem2->value.relativeNarrowbandTxPower.rNTP_PerPRB.numbits;

									for(count =0; count<prb_octet_count; count++)
									{
										p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.rntp_per_prb[count] =
											p_elem2->value.relativeNarrowbandTxPower.rNTP_PerPRB.data[count];
									}   

									p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.rntp_threshold =
										p_elem2->value.relativeNarrowbandTxPower.rNTP_Threshold;

									p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.num_cell_specific_antena_port =
										p_elem2->value.relativeNarrowbandTxPower.numberOfCellSpecificAntennaPorts;

									p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.p_b =
										p_elem2->value.relativeNarrowbandTxPower.p_B;

									p_load_info->cell_load_info[index_2].relative_narrowband_tx_power.pdcch_interference_impact =
										p_elem2->value.relativeNarrowbandTxPower.pDCCH_InterferenceImpact;
								}
								/* eICIC changes start */
								if (p_elem2->value.m.iE_ExtensionsPresent)
								{
									U16 count = X2AP_NULL;
									p_node4 = p_elem2->value.iE_Extensions.head;
									count_4 = p_elem2->value.iE_Extensions.count;
									/* This will only happen when the element list exceeds
									 * the max number */
									if(X2AP_MAX_IES <= count_4)
									{
										X2AP_TRACE(X2AP_WARNING, "PARSE Error is Detected in "
												"ASN.1 Sequence, Sending Transfer Syntax Error");
										x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
										retval = X2AP_FAILURE;
										break;
									}
									for(index_3 = 0; index_3 < count_4 && index_3 < MAX_SERVED_CELLS; index_3++)
									{
										p_elem3 = (x2ap_CellInformation_Item_iE_Extensions_element*)p_node4->data;
										if(p_elem3 == NULL)
										{
											X2AP_TRACE(X2AP_WARNING,"%s: p_elem3 is NULL",__FUNCTION__);
											return X2AP_FAILURE;
										}
										/*filling  ABS Information */
										if(ASN1V_x2ap_id_ABSInformation == p_elem3->id)
										{
											p_load_info->cell_load_info[index_2].bitmask |= X2AP_LI_ABS_INFORMATION_PRESENT;
											p_load_info->cell_load_info[index_2].abs_infomation.bitmask = 0;

											if(T_x2ap_ABSInformation_fdd == p_elem3->extensionValue.u.
													_x2ap_CellInformation_Item_ExtIEs_1->t)
											{
												p_load_info->cell_load_info[index_2].abs_infomation.bitmask |= 
													X2AP_LI_ABS_INFORMATION_FDD_PRESENT;

												p_load_info->cell_load_info[index_2].abs_infomation.abs_info_fdd.no_cell_specific_antenna_port =
													p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
													u.fdd->numberOfCellSpecificAntennaPorts;

												for(count = 0; count < X2AP_ABS_PATTERN_SIZE_FDD; count++)
												{
													p_load_info->cell_load_info[index_2].abs_infomation.abs_info_fdd.abs_pattern_info[count] =
														p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
														u.fdd->abs_pattern_info.data[count];

												}
												for(count = 0; count < X2AP_MEASUREMENT_SUBSET_SIZE_FDD; count++)
												{
													p_load_info->cell_load_info[index_2].abs_infomation.abs_info_fdd.measurement_subset[count] =
														p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
														u.fdd->measurement_subset.data[count];

												}

											}
											else if(T_x2ap_ABSInformation_tdd == p_elem3->extensionValue.u.
													_x2ap_CellInformation_Item_ExtIEs_1->t)
											{
												p_load_info->cell_load_info[index_2].abs_infomation.bitmask |= 
													X2AP_LI_ABS_INFORMATION_TDD_PRESENT;

												p_load_info->cell_load_info[index_2].abs_infomation.abs_info_tdd.no_cell_specific_antenna_port =
													p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
													u.tdd->numberOfCellSpecificAntennaPorts;

												for(count = 0; count < X2AP_ABS_PATTERN_SIZE_TDD; count++)
												{
													p_load_info->cell_load_info[index_2].abs_infomation.abs_info_tdd.abs_pattern_info[count] =
														p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
														u.tdd->abs_pattern_info.data[count];
												}
												for(count = 0; count < X2AP_MEASUREMENT_SUBSET_SIZE_TDD; count++)
												{
													p_load_info->cell_load_info[index_2].abs_infomation.abs_info_tdd.measurement_subset[count] =
														p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_1->
														u.tdd->measurement_subset.data[count];
												}
											}
											else if(T_x2ap_ABSInformation_abs_inactive == p_elem3->extensionValue.u.
													_x2ap_CellInformation_Item_ExtIEs_1->t)
											{
												p_load_info->cell_load_info[index_2].abs_infomation.bitmask |= 
													X2AP_LI_ABS_INFORMATION_ABS_INACTIVE_PRESENT;
											}
										}
										else if(ASN1V_x2ap_id_InvokeIndication == p_elem3->id)
										{
											/*filling  Invoke indication */
											p_load_info->cell_load_info[index_2].bitmask |= X2AP_LI_INVOKE_INDICATION_PRESENT;

											p_load_info->cell_load_info[index_2].invoke_indication.invoke_ind =
												p_elem3->extensionValue.u._x2ap_CellInformation_Item_ExtIEs_2;   
											/* enum declared as x2ap_InvokeIndication_Root */

										}
										else
										{
											/* Class - 1 Error - Unknown IE */
											/* Base on Criticality fill the value */
											x2ap_add_to_err_ind_ie_list(&iE_list,
													p_elem3->criticality,
													p_elem3->id,
													&ie_list_index,
													&send_err_ind,
													X2AP_FALSE,
													X2AP_FALSE);
										}
										p_node4 = p_node4->next;
									}
								}
								/* eICIC changes stop */
							}
							else
							{
								/* Class - 1 Error - Unknown IE */
								/* Base on Criticality fill the value */
								x2ap_add_to_err_ind_ie_list(&iE_list,
										p_elem2->criticality,
										p_elem2->id,
										&ie_list_index,
										&send_err_ind,
										X2AP_FALSE,
										X2AP_FALSE);
							}

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

	/*error_indication_start*/
	/* Parse the map for Error Indication */
	if ((X2AP_SUCCESS == x2ap_parse_message_map(
					pctxt,
					p_x2ap_gl_ctx,
					p_enb_ctx, 
					&message_map,
					&iE_list,
					&ie_list_index,
					&send_err_ind,
					ASN1V_x2ap_id_loadIndication,
					T_x2ap_X2AP_PDU_initiatingMessage,
					x2ap_ignore,
					(rrc_x2ap_error_indication_t *)PNULL))
	   )
	{
		X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

		retval = X2AP_FAILURE;
	}

	/*error_indication_end*/

	X2AP_UT_TRACE_EXIT();
	return retval;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_ul_high_intf_ind 
 * Inputs         : p_node - pointer to OSRTDListNode
 *                  p_ul_high_interference_info - pointer to 
 *                                       x2ap_rrm_li_ul_high_interference_info_t
 *                  count_3 - count
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index - order index
 *                  id - Id
 *                  p_send_tse - pointer to x2ap_bool_et
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP _FAILURE
 * Description    : This function returns decoded message buffer
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message
 ********************************************************************************/

x2ap_return_et x2ap_parse_ul_high_intf_ind(
		OSRTDListNode* p_node,
		x2ap_rrm_li_ul_high_interference_info_t  *p_ul_high_interference_info,
		U16  count_3,
		x2ap_message_data_t *p_ie_order_map,
		U8 order_index,
		U8 id,
		x2ap_bool_et *p_send_tse)
{
	x2ap_return_et  retVal = X2AP_FAILURE;
	U16 index_3;
	U16 count = X2AP_NULL;
	x2ap_UL_HighInterferenceIndicationInfo_Item* p_elem = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();
	if (X2AP_MAX_IES <= count_3)
	{
		*p_send_tse = X2AP_TRUE;
	}
	p_ul_high_interference_info->ul_high_interference_info_count = count_3;
	p_elem = (x2ap_UL_HighInterferenceIndicationInfo_Item*)p_node->data;

	for (index_3 =0; index_3 < count_3; index_3++)
	{
		U16 prb_octet_count = ((p_elem->ul_interferenceindication.numbits)%8?
				((p_elem->ul_interferenceindication.numbits/8)+1):
				(p_elem->ul_interferenceindication.numbits/8));
		p_elem = (x2ap_UL_HighInterferenceIndicationInfo_Item*)p_node->data;
		if (PNULL == p_elem)
		{
			x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
			return X2AP_FAILURE;
		}

		x2ap_parse_asn_plmn_identity(
				&p_ul_high_interference_info->ul_high_intf_info[index_3].
				target_cell_id.plmn_identity,
				p_elem->target_Cell_ID.pLMN_Identity.data);

		X2AP_MEMCPY(
				p_ul_high_interference_info->ul_high_intf_info[index_3].
				target_cell_id.cell_identity,
				p_elem->target_Cell_ID.eUTRANcellIdentifier.data,
				sizeof(U32));

		p_ul_high_interference_info->ul_high_intf_info[index_3].ul_high_intf_ind.prb_count =
			p_elem->ul_interferenceindication.numbits;

		for(count =0; count <prb_octet_count; count++)
		{
			p_ul_high_interference_info->ul_high_intf_info[index_3].ul_high_intf_ind.hii[count] =
				p_elem->ul_interferenceindication.data[count];
		}

	}
	retVal = X2AP_SUCCESS;
	X2AP_UT_TRACE_EXIT();
	return retVal;
}

/****************************************************************************
 * Function Name  : x2ap_process_load_information_msg
 * Inputs         : p_msg_ptr      Pointer to message buffer
 *                : p_x2ap_gl_ctx  pointer to X2AP global context
 *                : p_enb_ctx      Pointer to eNB context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the load information
 *                  mesaage received from peer enb.
 ****************************************************************************/
x2ap_return_et x2ap_process_load_information_msg(
		U8 *p_msg_ptr,
		x2ap_gb_context_t  *p_x2ap_gl_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx)
{
	/* coverity_fix_54732_start */
	/* + Coverity 28767 */
	static x2ap_rrm_load_information_ind_t load_info_ind;
	/* - Coverity 28767 */
	x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
	U32                    trans_Id = X2AP_NULL;
	U8                     *p_asn_msg;
	U16                    *p_asn_msg_len;
	x2ap_X2AP_PDU          x2ap_pdu;
	OSCTXT                 asn1_ctx;

	X2AP_UT_TRACE_ENTER();

	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);

	p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
	p_asn_msg = (p_peer_enb_msg)->p_msg;
	p_asn_msg_len = (U16*)&(p_peer_enb_msg->msg_len);

	X2AP_MEMSET(&load_info_ind, 0, sizeof(x2ap_rrm_load_information_ind_t));
	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR,  "%s: [X2AP_ErrorIndication] "\
				"ASN context initialization failed.",__FUNCTION__);
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
		x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
/*error_indication_end*/            
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
		return X2AP_FAILURE;
	}
	else
	{
		
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU ",
                    &x2ap_pdu);
	}

	if (T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
	{
		if (RRC_FAILURE == 
				x2ap_decode_load_information(
					p_x2ap_gl_ctx,
					p_enb_ctx,
					&asn1_ctx, 
					&load_info_ind,
					x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR, "%s: ASN "
					"decoding failed",__FUNCTION__);
		}
		rrc_x2apRrm_il_send_x2ap_rrm_load_information_ind(
				&load_info_ind,
				RRC_X2AP_MODULE_ID,
				RRC_RRM_MODULE_ID,
				trans_Id,
				RRC_NULL);

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
/* coverity_fix_54732_stop */

/******************************************************************************
 * Function Name  : x2ap_send_load_information_to_rrm
 * Inputs         : p_load_info_resp - Pointer to x2ap_rrm_load_information_resp_t
 *                  retval - type of x2ap_return_et
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is to send load information to  to rrm.
 ******************************************************************************/
x2ap_return_et x2ap_send_load_information_to_rrm (
                                x2ap_rrm_load_information_resp_t
                                                  *p_load_info_resp,
                                x2ap_return_et     retval)                  
{

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(p_load_info_resp!= PNULL);
	X2AP_MEMSET(p_load_info_resp, X2AP_NULL,
			sizeof(x2ap_rrm_load_information_resp_t));

	/*build*/
	p_load_info_resp->response = retval;

	if (RRC_FAILURE ==
			rrc_x2apRrm_il_send_x2ap_rrm_load_information_resp(
				p_load_info_resp,
				RRC_X2AP_MODULE_ID,
				RRC_RRM_MODULE_ID,
				0,
				RRC_NULL)) 

	{
		return X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();
	return retval;
}




