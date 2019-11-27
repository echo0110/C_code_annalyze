
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_lppa_asn_codec.c
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definitions for
 *                     composing and parsing Asn Messages.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "s1ap_lppa_asn_codec.h"
#include "s1ap_uecc.h"
#include "rrc_s1apRrm_il_composer.h"
#include "rrc_s1apRrm_il_parser.h"
#include "s1ap_error_indication.h"
#include "s1ap_mme.h"
#include "rrc_logging.h"


/****************************************************************************
 * Function Name  : lppa_internal_decode_otdoa_information_req
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et  lppa_internal_decode_otdoa_information_req(
        OSCTXT         *p_asn1_ctx,
        lppa_OTDOAInformationRequest    *p_3gpp,
        s1ap_rrm_lppa_otdoa_info_req_t  *p_intrl,
        mme_context_t *p_mme_context,
        rrc_lppa_error_indication_t *p_error_indication)
{
    U32    index = RRC_NULL;
    U16    count = RRC_NULL;
    OSRTDListNode   *p_node = PNULL;
    OSRTDListNode   *p_node1 = PNULL;
    rrc_return_et   result = RRC_SUCCESS;
    U16             ie_list_index = 0;
    lppa_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t      send_err_ind;
    lppa_OTDOAInformationRequest_protocolIEs_element  *p_protocol_IE = PNULL;
    lppa_OTDOA_Information_Type_element    *p_protocol_elem = PNULL;

    lppa_message_data_t message_map =
    {1, RRC_TRUE,RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_lppa_id_OTDOA_Information_Type_Group, lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }};

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    memset_wrapper(&iE_list, RRC_NULL, sizeof(lppa_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,
                        "%s: next node returned NULL", __FUNCTION__);
                break;
            }

            p_protocol_IE = 
                (lppa_OTDOAInformationRequest_protocolIEs_element*)p_node->data;
            if(ASN1V_lppa_id_OTDOA_Information_Type_Group == p_protocol_IE->id)
            {
                if(RRC_SUCCESS == lppa_validate_ie_value(
                            &message_map,
                            index,
                            p_protocol_IE->id,
                            p_protocol_IE->value.u._lppa_OTDOAInformationRequest_IEs_1))
                {
                    p_node1 = p_protocol_IE->value.u._lppa_OTDOAInformationRequest_IEs_1->head;
                    p_intrl->otdoa_info_count = (U16)p_protocol_IE->value.u._lppa_OTDOAInformationRequest_IEs_1->count;
                    for(count = 0; count < p_intrl->otdoa_info_count; count++)
                    {
                        if(!p_node1)
                        {
                            RRC_TRACE(RRC_ERROR,
                                   "%s: next node returned NULL", __FUNCTION__);
                            break;
                        }
                        p_protocol_elem = 
                            (lppa_OTDOA_Information_Type_element*)p_node1->data;
                        if (ASN1V_lppa_id_OTDOA_Information_Type_Item == p_protocol_elem->id)
                        {
                            if (LPPA_PRSMUTICONFIGURATION < 
                                    p_protocol_elem->value.u._lppa_OTDOA_Information_TypeIEs_1->oTDOA_Information_Type_Item)
                            {
                                lppa_update_message_map(&message_map, INVALID_VALUE, 
                                        index, p_protocol_IE->id);
                                break;
                            }
                            else
                            {
                                p_intrl->otda_info_type_arr[count].otdoa_info_item = 
                                    p_protocol_elem->value.u._lppa_OTDOA_Information_TypeIEs_1->oTDOA_Information_Type_Item;
                            }
                            p_node1 = p_node1->next;
                        }
                        else
                        {
                            lppa_update_message_map(&message_map, INVALID_VALUE,index, p_protocol_IE->id);
                        }
                    }
                }
            }
            else
            {
                RRC_TRACE(RRC_ERROR, 
                        "%s:Invalid protocol ID recieved", __FUNCTION__);
                /* Class - 1 Error - Unknown IE */
                /* Base on Criticality fill the value */
                lppa_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        RRC_FALSE,
                        RRC_FALSE);
            }
            p_node = p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if(RRC_SUCCESS == lppa_parse_message_map(
                    p_asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_lppa_id_oTDOAInformationExchange,
                    T_lppa_LPPA_PDU_initiatingMessage,
                    lppa_reject,
                    p_error_indication) ) 
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "DownlinkNonUEAssociatedLPPaTransport Initiating Message ");

        result = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : s1ap_decode_asn_s1ap_LPPa_PDU
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_decode_asn_s1ap_LPPa_PDU(
                s1ap_gb_context_t   *p_s1ap_gb_ctx,
                mme_context_t       * p_mme_ctx,
                dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
                dl_non_ue_asso_lppa_trans_t  *p_non_ue_asso_lppa_trans,
                U8      lppa_asso_type)
{
    lppa_LPPA_PDU    lppa_pdu;
    OSCTXT           asn1_ctx;
    rrc_return_et    result = RRC_SUCCESS;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);

    /* Init ASN1 context */
    if(0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    asn1Init_lppa_LPPA_PDU(&lppa_pdu);

    p_mme_ctx->lppa_info.ue_asso_type = lppa_asso_type;
    /*Set pointer of asn buffer in asn context*/
    if (lppa_asso_type == UE_ASSO_LPPA)
    {
        if(PNULL != p_ue_asso_lppa_trans)
        {
            pu_setBuffer(&asn1_ctx, (OSOCTET*)p_ue_asso_lppa_trans->lppa_pdu.data,
                    p_ue_asso_lppa_trans->lppa_pdu.numocts, TRUE);
            p_mme_ctx->lppa_info.mme_ue_s1ap_id = p_ue_asso_lppa_trans->mme_ue_s1ap_id;
            p_mme_ctx->lppa_info.enb_ue_s1ap_id = p_ue_asso_lppa_trans->enb_ue_s1ap_id;
            p_mme_ctx->lppa_info.routing_id = p_ue_asso_lppa_trans->routing_id;
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "ue_asso_lppa_trans pointer is null.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return RRC_FAILURE;
        }
    }
    else if (lppa_asso_type == NON_UE_ASSO_LPPA)
    {
        if (PNULL != p_non_ue_asso_lppa_trans)
        {
            pu_setBuffer(&asn1_ctx, (OSOCTET*)p_non_ue_asso_lppa_trans->lppa_pdu.data,
                     p_non_ue_asso_lppa_trans->lppa_pdu.numocts, TRUE);
            p_mme_ctx->lppa_info.routing_id = p_non_ue_asso_lppa_trans->routing_id;
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "non_ue_asso_lppa_trans pointer is null.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return RRC_FAILURE;
        }
    }

    do
    {
        if (0 != asn1PD_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "%s: ASN decode failed.",
                           __FUNCTION__);
            /* Send Transfer Syntax Error */
            lppa_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }

        if (PNULL != lppa_pdu.u.initiatingMessage)
        {
            p_mme_ctx->lppa_info.lppa_trans_id = 
                (U32)lppa_pdu.u.initiatingMessage->lppatransactionID;
        }

        if ((T_lppa_LPPA_PDU_initiatingMessage == lppa_pdu.t) &&
            (PNULL != lppa_pdu.u.initiatingMessage))
        {
            switch (lppa_pdu.u.initiatingMessage->value.t)
            {
                case T1lppa__e_CIDMeasurementInitiation:
                    s1ap_lppa_process_e_CIDMeasurementInitiation(&asn1_ctx, p_mme_ctx,
                          lppa_pdu.u.initiatingMessage->value.u.e_CIDMeasurementInitiation,
                          p_ue_asso_lppa_trans,
                          (U32)lppa_pdu.u.initiatingMessage->lppatransactionID,
                          p_s1ap_gb_ctx);              /* spr_11705_fix */ 
                    break;

                case T1lppa__e_CIDMeasurementTermination:
                    s1ap_lppa_process_e_CIDMeasurementTermination(&asn1_ctx, p_mme_ctx,
                          lppa_pdu.u.initiatingMessage->value.u.e_CIDMeasurementTermination,
                          p_ue_asso_lppa_trans,
                          (U32)lppa_pdu.u.initiatingMessage->lppatransactionID,
                          p_s1ap_gb_ctx);             /* spr_11705_fix */
                    break;

                case T1lppa__oTDOAInformationExchange:
                    s1ap_lppa_oTDOAInformationExchange(&asn1_ctx, p_mme_ctx,
                          lppa_pdu.u.initiatingMessage->value.u.oTDOAInformationExchange,
                          p_non_ue_asso_lppa_trans,
                          (U32)lppa_pdu.u.initiatingMessage->lppatransactionID);
                    break;

                case T1lppa__errorIndication:
                    s1ap_lppa_process_errorIndication(p_mme_ctx,/*SPR 17777 +-*/
                            lppa_pdu.u.initiatingMessage->value.u.errorIndication,
                            (U32)lppa_pdu.u.initiatingMessage->lppatransactionID,
                            p_mme_ctx->lppa_info.enb_ue_s1ap_id,
                            p_mme_ctx->lppa_info.routing_id,
                            lppa_asso_type,
                            p_s1ap_gb_ctx);          /* spr_11705_fix */
                    break;

                default:
                    RRC_TRACE(RRC_ERROR, 
                        "[s1ap_decode_asn_s1ap_LPPa_PDU] Invalid Message Type.");
                    result = RRC_FAILURE;
                    break;
            }
        }
        else
        {
            RRC_TRACE(RRC_ERROR, "Invalid Message.");
            result = RRC_FAILURE;
        }
    } while(0);

   /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : s1ap_parse_asn_dl_ue_associated_lppa_transport
 * Inputs         : 
 * Outputs        : 
 * Returns        : RRC_SUCCESS 
 *                  RRC_FAILURE 
 * Description    : 
 *****************************************************************************/
rrc_return_et s1ap_parse_asn_dl_ue_associated_lppa_transport(
        OSCTXT              *p_asn1_ctx,
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans)
{
    rrc_return_et                   result = RRC_SUCCESS;
    S1AP_PDU                        s1ap_pdu;
    U32 ue_id;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    U8 ue_id_pair_presence = 0;
    
    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_asn_msg);
    
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(p_asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        
        if ((T_S1AP_PDU_initiatingMessage == s1ap_pdu.t) &&
            (PNULL != s1ap_pdu.u.initiatingMessage) &&
            (T1_downlinkUEAssociatedLPPaTransport == s1ap_pdu.u.initiatingMessage->value.t) &&
            (PNULL != s1ap_pdu.u.initiatingMessage->value.u.downlinkUEAssociatedLPPaTransport))
        {
            if( RRC_FAILURE == dl_ue_associated_lppa_trans_intrl_dec( p_asn1_ctx, p_mme_ctx,
                      s1ap_pdu.u.initiatingMessage->value.u.downlinkUEAssociatedLPPaTransport,
                      p_ue_asso_lppa_trans))
             {
                 result = RRC_FAILURE;
                 break;
             }
            
            RRC_TRACE(RRC_INFO, "S1AP_MSG: DownlinkUEAssociatedLPPaTransport %s", change_data_to_str(asn_msg_len, p_asn_msg));
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU", &s1ap_pdu);
            ue_id = s1ap_get_ue_id(p_ue_asso_lppa_trans->enb_ue_s1ap_id);
 
            SET_UE_INDEX(ue_id);

            if(ue_id < MAX_NUM_UE)
            {
                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx, &p_s1ap_context, 
                    p_ue_asso_lppa_trans->enb_ue_s1ap_id);
                if(p_s1ap_context != PNULL )
                {
                    if(p_ue_asso_lppa_trans->mme_ue_s1ap_id == p_s1ap_context->mme_ue_s1ap_id)
                    {
                        /* mme_ue_s1ap_id and enb_ue_s1ap_id pair verified */
                        break;
                    }
                }
            }
            RRC_TRACE(RRC_INFO, "ErrorIndication occured due to ue_id_pair mismatch."); 
            ue_id_pair_presence |= ENB_UE_S1AP_ID_PRESENT;
            ue_id_pair_presence |= MME_UE_S1AP_ID_PRESENT;
            fill_and_send_error_ind_msg(p_mme_ctx,
                        ue_id_pair_presence,
                        p_ue_asso_lppa_trans->enb_ue_s1ap_id,
                        p_ue_asso_lppa_trans->mme_ue_s1ap_id,
                        s1ap_unknown_pair_ue_s1ap_id,
                        PNULL);
            result = RRC_FAILURE;
        }
        else
        {
            RRC_TRACE(RRC_ERROR, "Invalid Message.");
            result = RRC_FAILURE;
        }
    }while (0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : s1ap_parse_asn_dl_non_ue_associated_lppa_transport
 * Inputs         :
 * Outputs        : 
 * Returns        : RRC_SUCCESS 
 *                  RRC_FAILURE 
 * Description    :  
 *****************************************************************************/
rrc_return_et  s1ap_parse_asn_dl_non_ue_associated_lppa_transport(
        OSCTXT              *p_asn1_ctx,
        /*SPR 17777 +-*/
        mme_context_t       * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        dl_non_ue_asso_lppa_trans_t *p_non_ue_asso_lppa_trans)
{
	/* coverity_fix_54725_start */
	S1AP_PDU              s1ap_pdu;
	rrc_return_et         result = RRC_SUCCESS;

	RRC_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
	RRC_ASSERT(PNULL != p_mme_ctx);
	RRC_ASSERT(PNULL != p_asn_msg);
	RRC_ASSERT(PNULL != p_non_ue_asso_lppa_trans);
    /*+ SPR 22111 Fix*/
    /*Code Removed*/
    /*- SPR 22111 Fix*/
	asn1Init_S1AP_PDU(&s1ap_pdu);

	/*Set pointer of asn buffer in asn context*/
	pu_setBuffer(p_asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

	do
	{
		if (0 != asn1PD_S1AP_PDU(p_asn1_ctx, &s1ap_pdu) )
		{
			/* Drop message */
			RRC_TRACE(RRC_ERROR, "%s: ASN "
					"decode failed.",__FUNCTION__);
			/* Send Transfer Syntax Error */
			s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

			result = RRC_FAILURE;
			break;
		}

		if ((T_S1AP_PDU_initiatingMessage == s1ap_pdu.t) &&
				(PNULL != s1ap_pdu.u.initiatingMessage) &&
				(T1_downlinkNonUEAssociatedLPPaTransport == s1ap_pdu.u.initiatingMessage->value.t) &&
				(PNULL != s1ap_pdu.u.initiatingMessage->value.u.downlinkNonUEAssociatedLPPaTransport))
		{
			RRC_TRACE(RRC_ERROR, "S1AP_MSG: DownlinkNonUEAssociatedLPPaTransport %s", change_data_to_str(asn_msg_len, p_asn_msg));
			/* Decode message */
			if( RRC_FAILURE == dl_non_ue_associated_lppa_trans_intrl_dec( 
						p_asn1_ctx, p_mme_ctx,
						s1ap_pdu.u.initiatingMessage->value.u.downlinkNonUEAssociatedLPPaTransport,
						p_non_ue_asso_lppa_trans))
			{
				result = RRC_FAILURE;
				break;
			}                                  
			
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU", &s1ap_pdu);
		}
		else
		{
			RRC_TRACE(RRC_ERROR, "Invalid Message.");
			result = RRC_FAILURE;
		}
	}while (0);

	RRC_UT_TRACE_EXIT();
	return result;
}
/* coverity_fix_54725_stop */

/****************************************************************************
 * Function Name  : dl_non_ue_associated_lppa_trans_intrl_dec
 * Inputs         : p_mme_context
 *                  p_3gpp and p_intrl
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et  dl_non_ue_associated_lppa_trans_intrl_dec 
(
 OSCTXT        *p_asn1_ctx,
 mme_context_t *p_mme_context,
 s1ap_DownlinkNonUEAssociatedLPPaTransport  *p_3gpp,  /* unpacked src */
 dl_non_ue_asso_lppa_trans_t  *p_intrl /* dest */
 )
{
    U32             index = 0;
    OSRTDListNode   *p_node = RRC_NULL;
    rrc_return_et   result = RRC_SUCCESS;
    U16             ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t      send_err_ind;
    s1ap_DownlinkNonUEAssociatedLPPaTransport_protocolIEs_element  *p_protocol_IE = RRC_NULL;
    
    s1ap_message_data_t message_map =
    {2, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_Routing_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_LPPa_PDU, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};
    
    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    memset_wrapper(&iE_list, RRC_NULL, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_DownlinkNonUEAssociatedLPPaTransport_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_Routing_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                            "ASN1V_s1ap_id_Routing_ID",
                            __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                &p_protocol_IE->value.u._DownlinkNonUEAssociatedLPPaTransport_IEs_1
                                ))
                      {
                          p_intrl->routing_id = 
                              p_protocol_IE->value.u._DownlinkNonUEAssociatedLPPaTransport_IEs_1;
                      }
                }
                break;
                case ASN1V_s1ap_id_LPPa_PDU:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                            "ASN1V_s1ap_id_LPPa_PDU",
                            __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._DownlinkNonUEAssociatedLPPaTransport_IEs_2
                                ))
                      {
                          asn1Copy_s1ap_LPPa_PDU(p_asn1_ctx,
                           p_protocol_IE->value.u._DownlinkNonUEAssociatedLPPaTransport_IEs_2,
                           &p_intrl->lppa_pdu); 
                      }
                }
                break;
                default:
                    RRC_TRACE(RRC_ERROR,"%s: "
                            "invalid Protocol IE id",
                            __FUNCTION__);
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    add_to_err_ind_ie_list(&iE_list, 
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            &send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);


            }
            p_node = p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    p_asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_downlinkNonUEAssociatedLPPaTransport,
                    T_S1AP_PDU_initiatingMessage,
                    s1ap_ignore,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "DownlinkNonUEAssociatedLPPaTransport Initiating Message ");

        result = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : dl_ue_associated_lppa_trans_intrl_dec
 * Inputs         : p_mme_context
 *                : p_3gpp 
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE 
 * Description    : This function decode Downlink UE Associated LPPa Transport 
 ****************************************************************************/
rrc_return_et  dl_ue_associated_lppa_trans_intrl_dec 
(
 OSCTXT        *p_asn1_ctx,
 mme_context_t *p_mme_context,
 s1ap_DownlinkUEAssociatedLPPaTransport  *p_3gpp,  /* unpacked src */
 dl_ue_asso_lppa_trans_t  *p_intrl/* dest */
 )
{
    U32             index = 0;
    OSRTDListNode   *p_node = RRC_NULL;
    rrc_return_et   result = RRC_SUCCESS;
    U16             ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t      send_err_ind ;
    s1ap_DownlinkUEAssociatedLPPaTransport_protocolIEs_element  *p_protocol_IE = RRC_NULL;
    
    s1ap_message_data_t message_map =
    {4, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_Routing_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {3, ASN1V_s1ap_id_LPPa_PDU, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};
    
    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: next node returned NULL", __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_DownlinkUEAssociatedLPPaTransport_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_s1ap_id_MME_UE_S1AP_ID",
                                   __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_1))
                    {
                          p_intrl->mme_ue_s1ap_id =
                              p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_1;
                    }
                }
                break;
                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_s1ap_id_eNB_UE_S1AP_ID",
                                   __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_2))
                    {
                          p_intrl->enb_ue_s1ap_id =
                              p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_2;
                    }
                }
                break;
                case ASN1V_s1ap_id_Routing_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_s1ap_id_Routing_ID",
                                   __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_3))
                    {
                          p_intrl->routing_id = 
                              p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_3;
                    }
                }
                break;
                case ASN1V_s1ap_id_LPPa_PDU:
                {
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_s1ap_id_LPPa_PDU",
                                   __FUNCTION__);

                    if(RRC_SUCCESS == validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_4))
                    {
                          asn1Copy_s1ap_LPPa_PDU(p_asn1_ctx,
                           p_protocol_IE->value.u._DownlinkUEAssociatedLPPaTransport_IEs_4,
                           &p_intrl->lppa_pdu); 
                    }
                }
                break;
                default:
                    RRC_TRACE(RRC_ERROR,"%s: invalid Protocol IE id",
                                   __FUNCTION__);
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    add_to_err_ind_ie_list(&iE_list, 
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            &send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);


            }
            p_node = p_node->next;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    p_asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_downlinkUEAssociatedLPPaTransport,
                    T_S1AP_PDU_initiatingMessage,
                    s1ap_ignore,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "DownlinkUEAssociatedLPPaTransport Initiating Message ");

        result = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : s1ap_lppa_oTDOAInformationExchange 
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_oTDOAInformationExchange(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_OTDOAInformationRequest  *p_otdoa_info_request,
        dl_non_ue_asso_lppa_trans_t   *p_non_ue_asso_lppa_trans,
        U32             lppa_trans_id)
{
    s1ap_rrm_lppa_otdoa_info_req_t  lppa_otdoa_info_req;
    rrc_lppa_error_indication_t error_indication;

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_otdoa_info_request);
    RRC_ASSERT(PNULL != p_non_ue_asso_lppa_trans);

    if (PNULL == p_non_ue_asso_lppa_trans)
    {
        RRC_TRACE(RRC_ERROR,"p_non_ue_asso_lppa_trans pointer is null.");
        return;
    }
    memset_wrapper(&lppa_otdoa_info_req, 0, 
               sizeof(s1ap_rrm_lppa_otdoa_info_req_t));
    memset_wrapper(&error_indication, 0, sizeof(rrc_lppa_error_indication_t));

    if(RRC_FAILURE == lppa_internal_decode_otdoa_information_req(
                p_asn1_ctx,
                p_otdoa_info_request, 
                &lppa_otdoa_info_req, 
                p_mme_ctx,
                &error_indication))
    {
        RRC_TRACE(RRC_ERROR,"%s: "
          "lppa_internal_decode_otdoa_information_req failed", __FUNCTION__);

        error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT;
        error_indication.routing_id = p_non_ue_asso_lppa_trans->routing_id;

        s1ap_lppa_build_and_send_OTDOAInformationFailure(p_mme_ctx,
                                    lppa_trans_id, &error_indication);
        return;
    }

    lppa_otdoa_info_req.mme_id = p_mme_ctx->mme_id;
    lppa_otdoa_info_req.routing_id = p_non_ue_asso_lppa_trans->routing_id;
    lppa_otdoa_info_req.lppa_trans_id = lppa_trans_id;


    /* send meas_initiation_req to rrm */
    if (RRC_SUCCESS != 
         rrc_s1apRrm_il_send_s1ap_rrm_lppa_otdoa_info_req(
                 &lppa_otdoa_info_req,
                 RRC_S1AP_MODULE_ID,
                 RRC_RRM_MODULE_ID,
                 0,
                 RRC_NULL))
    {
        RRC_TRACE(RRC_ERROR,"%s: Unable to send message to RRM",__FUNCTION__);
        return;
    }
    else
    {
        RRC_TRACE(RRC_INFO,"%s: lppa_otdoa_info_req message sent to RRM",
                  __FUNCTION__);
    }

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_lppa_process_e_CIDMeasurementInitiation
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_process_e_CIDMeasurementInitiation(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_E_CIDMeasurementInitiationRequest *p_e_CIDMeasurementInitiation,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
        U32             lppa_trans_id,
        s1ap_gb_context_t   *p_s1ap_gb_ctx)         /* spr_11705_fix */
{
	s1ap_rrm_lppa_ecid_meas_init_req_t lppa_meas_init_req;
	rrc_lppa_error_indication_t error_indication;
	/* spr_11705_fix_start */
	/* CID: 113469 Start */
	U32 ue_id  = RRC_NULL;          /* id of UE in RRC */
	/* CID: 113469 End */
	U8 cell_id = MAX_NUM_CELLS;        /* id to identify a cell in enB*/
	/* spr_11705_fix_stop */

	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_mme_ctx);
	RRC_ASSERT(PNULL != p_e_CIDMeasurementInitiation);
	RRC_ASSERT(PNULL != p_ue_asso_lppa_trans);

	if (PNULL == p_ue_asso_lppa_trans)
	{
		RRC_TRACE(RRC_ERROR,"p_ue_asso_lppa_trans pointer is null.");
		return;
	}
	RRC_UT_TRACE_ENTER();
	memset_wrapper(&lppa_meas_init_req, 0, 
			sizeof(s1ap_rrm_lppa_ecid_meas_init_req_t));
	memset_wrapper(&error_indication, 0, sizeof(rrc_lppa_error_indication_t));

	error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
	error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
	error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT;

	error_indication.mme_ue_s1ap_id = p_ue_asso_lppa_trans->mme_ue_s1ap_id;
	error_indication.enb_ue_s1ap_id = p_ue_asso_lppa_trans->enb_ue_s1ap_id;
	error_indication.routing_id = p_ue_asso_lppa_trans->routing_id;

	if(RRC_SUCCESS != s1ap_lppa_decode_e_CIDMeasurementInitiationRequest(
				p_asn1_ctx, p_mme_ctx, p_e_CIDMeasurementInitiation, 
				&lppa_meas_init_req, &error_indication))
	{
		RRC_TRACE(RRC_ERROR,"%s: "
				"s1ap_lppa_decode_e_CIDMeasurementInitiationRequest failed", __FUNCTION__);
		/* build and send e_CIDMeasurementInitiationFailure */
		s1ap_lppa_build_and_send_e_CIDMeasurementInitiationFailure(p_mme_ctx,
				lppa_trans_id, &error_indication);
		return;
	}

	if (((lppa_meas_init_req.report_char != LPPA_PERIODIC) &&
				(lppa_meas_init_req.bitmask & LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT)) ||
			((lppa_meas_init_req.report_char == LPPA_PERIODIC) &&
			 (!(lppa_meas_init_req.bitmask & LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT))))
	{
		/* Handle the negative scenario */
		/* Its a Abstract Syntax Error */
		RRC_TRACE(RRC_ERROR,"%s: ReportCharacteristic not equal to LPPA_PERIODIC"
				" and MeasurementPeriodicity Present.", __FUNCTION__);
		error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT;

		error_indication.cause.t = T_lppa_Cause_protocol;
		error_indication.cause.u.protocol = lppa_abstract_syntax_error_falsely_constructed_message;

		s1ap_lppa_build_and_send_e_CIDMeasurementInitiationFailure(p_mme_ctx,
				lppa_trans_id, &error_indication);
		return;
	}

	lppa_meas_init_req.mme_id = p_mme_ctx->mme_id;
	lppa_meas_init_req.ue_index = 
		s1ap_get_ue_id(p_ue_asso_lppa_trans->enb_ue_s1ap_id);
	lppa_meas_init_req.routing_id = p_ue_asso_lppa_trans->routing_id;
	lppa_meas_init_req.lppa_trans_id = lppa_trans_id;

	/* spr_11705_fix_start */
	/* Get ue id from enb_ue_s1ap_id */
	ue_id = s1ap_get_ue_id(p_ue_asso_lppa_trans->enb_ue_s1ap_id);

	if (PNULL != p_s1ap_gb_ctx)
	{
		/* Get cell id from enb_ue_s1ap_id */
		cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, p_ue_asso_lppa_trans->enb_ue_s1ap_id);

		RRC_S1AP_TRACE(RRC_INFO,
				"[%s]:Cell id %d Ue id %d "
				" \n ",__FUNCTION__,cell_id,ue_id);
		if ((cell_id >= MAX_NUM_CELLS) || 
				(!(p_s1ap_gb_ctx->
				   cell_s1ap_contexts[cell_id])))
		{
			RRC_S1AP_TRACE(RRC_WARNING,
					"Cell id is invalid\n");
			return;
		}
	}
	/* spr_11705_fix_stop */


	/* send meas_initiation_req to rrm */
	if (RRC_FAILURE == 
			rrc_s1apRrm_il_send_s1ap_rrm_lppa_ecid_meas_init_req(
				&lppa_meas_init_req,
				RRC_S1AP_MODULE_ID,
				RRC_RRM_MODULE_ID,
				0,
				cell_id))
	{
		RRC_TRACE(RRC_ERROR,"%s: Unable to send message to RRM",__FUNCTION__);
	}
	else
	{
		RRC_TRACE(RRC_INFO,"%s: lppa_meas_init_req message sent to RRM",
				__FUNCTION__);
	}

	RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_lppa_process_e_CIDMeasurementTermination
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_process_e_CIDMeasurementTermination(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_E_CIDMeasurementTerminationCommand *p_e_CIDMeasurementTermination,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
        U32             lppa_trans_id,
        s1ap_gb_context_t   *p_s1ap_gb_ctx) /*spr_11705_fix*/
{
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t lppa_meas_term_cmd;
    /* spr_11705_fix_start */
    /* CID: 113472 Start */
    U32 ue_id  = RRC_NULL;          /* id of UE in RRC */
    /* CID: 113472 End */
    U8 cell_id = MAX_NUM_CELLS;        /* id to identify a cell in enB*/
    /* spr_11705_fix_stop */

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_e_CIDMeasurementTermination);
    RRC_ASSERT(PNULL != p_ue_asso_lppa_trans);

    if (PNULL == p_ue_asso_lppa_trans)
    {
        RRC_TRACE(RRC_ERROR,"p_ue_asso_lppa_trans pointer is null.");
        return;
    }
    memset_wrapper(&lppa_meas_term_cmd, 0,
               sizeof(s1ap_rrm_lppa_ecid_meas_termination_cmd_t));

    if(RRC_FAILURE == s1ap_lppa_decode_e_CIDMeasurementTermination(
               p_asn1_ctx, p_mme_ctx, p_e_CIDMeasurementTermination, &lppa_meas_term_cmd))
    {
        RRC_TRACE(RRC_ERROR,"%s: "
          "s1ap_lppa_decode_e_CIDMeasurementTerminationfailed", __FUNCTION__);
        return;
    }

    lppa_meas_term_cmd.mme_id = p_mme_ctx->mme_id;
    lppa_meas_term_cmd.ue_index =
        s1ap_get_ue_id(p_ue_asso_lppa_trans->enb_ue_s1ap_id);
    lppa_meas_term_cmd.routing_id = p_ue_asso_lppa_trans->routing_id;
    lppa_meas_term_cmd.lppa_trans_id = lppa_trans_id;

    /* spr_11705_fix_start */
    /* Get ue id from enb_ue_s1ap_id */
    ue_id = s1ap_get_ue_id(p_ue_asso_lppa_trans->enb_ue_s1ap_id);

    if (PNULL != p_s1ap_gb_ctx)
    {
        /* Get cell id from enb_ue_s1ap_id */
        cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, p_ue_asso_lppa_trans->enb_ue_s1ap_id);

        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]:Cell id %d Ue id %d "
                " \n ",__FUNCTION__,cell_id,ue_id);
        if ((cell_id >= MAX_NUM_CELLS) || 
                (!(p_s1ap_gb_ctx->
                   cell_s1ap_contexts[cell_id])))
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "Cell id is invalid\n");
            return;
        }
    }
    /* spr_11705_fix_stop */

    /* send meas_initiation_req to rrm */
    if (RRC_FAILURE ==
         rrc_s1apRrm_il_send_s1ap_rrm_lppa_ecid_meas_termination_cmd(
                 &lppa_meas_term_cmd,
                 RRC_S1AP_MODULE_ID,
                 RRC_RRM_MODULE_ID,
                 0,
                 cell_id))
    {
        RRC_TRACE(RRC_ERROR,"%s: Unable to send message to RRM",__FUNCTION__);
        return;
    }
    else
    {
        RRC_TRACE(RRC_INFO,"%s: lppa_meas_term_cmd message sent to RRM",
                  __FUNCTION__);
    }

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_lppa_process_errorIndication
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_process_errorIndication(
        /*SPR 17777 +-*/
        mme_context_t   * p_mme_ctx,
        lppa_ErrorIndication  *p_errorIndication,
        U32             lppa_trans_id,
        U32             enb_ue_s1ap_id,
        U8              routing_id,
        U8              lppa_asso_type,
        s1ap_gb_context_t   *p_s1ap_gb_ctx)   /*spr_11705_fix*/
{
    s1ap_rrm_lppa_error_ind_t  lppa_error_ind;
    /* spr_11705_fix_start */
    /* CID: 113473 Start */
    U32 ue_id  = RRC_NULL;          /* id of UE in RRC */
    /* CID: 113473 End */
    U8 cell_id = MAX_NUM_CELLS;        /* id to identify a cell in enB*/
    /* spr_11705_fix_stop */

    RRC_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_errorIndication);

    memset_wrapper(&lppa_error_ind, 0, sizeof(s1ap_rrm_lppa_error_ind_t));

    if(RRC_FAILURE == s1ap_lppa_decode_ErrorIndication(
                p_errorIndication, &lppa_error_ind))/*SPR 17777 +-*/
    {
        RRC_TRACE(RRC_ERROR,"%s: "
          "s1ap_lppa_decode_e_CIDMeasurementTerminationfailed", __FUNCTION__);
        return;
    }

    lppa_error_ind.mme_id = p_mme_ctx->mme_id;
    if (lppa_asso_type == UE_ASSO_LPPA)
    {
        lppa_error_ind.bitmask |= LPPA_ERROR_IND_UE_INDEX_PRESENT;
        lppa_error_ind.ue_index = s1ap_get_ue_id(enb_ue_s1ap_id);
    }

    lppa_error_ind.routing_id = routing_id;
    lppa_error_ind.lppa_trans_id = lppa_trans_id;

    /* spr_11705_fix_start */
    /* Get ue id from enb_ue_s1ap_id */
    ue_id = s1ap_get_ue_id(enb_ue_s1ap_id);

    if (PNULL != p_s1ap_gb_ctx)
    {
        /* Get cell id from enb_ue_s1ap_id */
        cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, enb_ue_s1ap_id);

        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]:Cell id %d Ue id %d "
                " \n ",__FUNCTION__,cell_id,ue_id);
        if ((cell_id >= MAX_NUM_CELLS) || 
                (!(p_s1ap_gb_ctx->
                   cell_s1ap_contexts[cell_id])))
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "Cell id is invalid\n");
            return;
        }
    }
    /* spr_11705_fix_stop */

    /* send ErrorIndication to rrm */
    if (RRC_FAILURE ==
         rrc_s1apRrm_il_send_s1ap_rrm_lppa_error_ind(
                 &lppa_error_ind,
                 RRC_S1AP_MODULE_ID,
                 RRC_RRM_MODULE_ID,
                 0,
                 cell_id))
    {
        RRC_TRACE(RRC_ERROR,"%s: Unable to send message to RRM",__FUNCTION__);
        return;
    }
    else
    {
        RRC_TRACE(RRC_INFO,"%s: lppa_error_ind message sent to RRM", __FUNCTION__);
    }

    RRC_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : s1ap_lppa_decode_e_CIDMeasurementInitiationRequest
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_lppa_decode_e_CIDMeasurementInitiationRequest(
    OSCTXT          *p_asn1_ctx,
    mme_context_t   *p_mme_ctx,
    lppa_E_CIDMeasurementInitiationRequest *p_e_CIDMeasurementInitiation,
    s1ap_rrm_lppa_ecid_meas_init_req_t   *p_lppa_meas_init_req,
    rrc_lppa_error_indication_t *p_error_indication)
{
    U32             index = 0;
    U32             count = 0;
    OSRTDListNode   *p_node = RRC_NULL;
    OSRTDListNode   *p_node1 = RRC_NULL;
    rrc_return_et   result = RRC_SUCCESS;
    lppa_E_CIDMeasurementInitiationRequest_protocolIEs_element 
                                              *p_protocolIEs_element = PNULL;
    lppa_MeasurementQuantities_element *p_meas_quant_elem = PNULL;
    lppa_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    lppa_message_data_t message_map =
    {4, RRC_TRUE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_lppa_id_E_SMLC_UE_Measurement_ID,lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_lppa_id_ReportCharacteristics,lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_lppa_id_MeasurementPeriodicity,lppa_optional, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {3, ASN1V_lppa_id_MeasurementQuantities,lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_CIDMeasurementInitiation);
    RRC_ASSERT(PNULL != p_lppa_meas_init_req);
    RRC_ASSERT(PNULL != p_error_indication);

    memset_wrapper(&iE_list, RRC_NULL, sizeof(lppa_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));

    do
    {
        p_node = p_e_CIDMeasurementInitiation->protocolIEs.head;

        for (index = 0; index < p_e_CIDMeasurementInitiation->protocolIEs.count; 
                        index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: next node returned NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }

            p_protocolIEs_element = 
             (lppa_E_CIDMeasurementInitiationRequest_protocolIEs_element*)p_node->data;

            switch(p_protocolIEs_element->id)
            {
                case ASN1V_lppa_id_E_SMLC_UE_Measurement_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_E_SMLC_UE_Measurement_ID", __FUNCTION__);
                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                &p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementInitiationRequest_IEs_1))
                    {
                        p_lppa_meas_init_req->e_smlc_meas_id = 
                        p_protocolIEs_element->value.u._lppa_E_CIDMeasurementInitiationRequest_IEs_1;
                        p_mme_ctx->lppa_info.e_smlc_meas_id = p_lppa_meas_init_req->e_smlc_meas_id;

                    }
                    break;
                }
                case ASN1V_lppa_id_ReportCharacteristics:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_ReportCharacteristics", __FUNCTION__);

                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                &p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementInitiationRequest_IEs_2))
                    {
                        p_lppa_meas_init_req->report_char = 
                        p_protocolIEs_element->value.u._lppa_E_CIDMeasurementInitiationRequest_IEs_2;
                    }
                    break;
                }
                case ASN1V_lppa_id_MeasurementPeriodicity:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_MeasurementPeriodicity", __FUNCTION__);

                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                &p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementInitiationRequest_IEs_3))
                    {
                        p_lppa_meas_init_req->bitmask |= 
                            LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT;
                        p_lppa_meas_init_req->meas_periodicity = 
                        p_protocolIEs_element->value.u._lppa_E_CIDMeasurementInitiationRequest_IEs_3;
                    }
                    break;
                }
                case ASN1V_lppa_id_MeasurementQuantities:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_MeasurementQuantities", __FUNCTION__);

                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementInitiationRequest_IEs_4))
                    {
                        p_node1 = p_protocolIEs_element->value.u.
                            _lppa_E_CIDMeasurementInitiationRequest_IEs_4->head;
                        for (count = 0; 
                                count < p_protocolIEs_element->value.u.
                                _lppa_E_CIDMeasurementInitiationRequest_IEs_4->count; count++)
                        {
                            if (!p_node1)
                            {
                                RRC_TRACE(RRC_ERROR,"%s: next node returned NULL", __FUNCTION__);
                                result = RRC_FAILURE;
                                break;
                            }
                            p_meas_quant_elem = (lppa_MeasurementQuantities_element*)p_node1->data;

                            p_lppa_meas_init_req->meas_quantifier[count].meas_quantity_item = 
                                p_meas_quant_elem->value.measurementQuantitiesValue;

                            p_node1 = p_node1->next;
                        }

                        p_lppa_meas_init_req->num_meas_quantifier = (U8)p_protocolIEs_element->value.u.
                            _lppa_E_CIDMeasurementInitiationRequest_IEs_4->count;
                    }
                    break;
                }
                default:
                    RRC_TRACE(RRC_ERROR,"%s: Invalid IE id.", __FUNCTION__);
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    lppa_add_to_err_ind_ie_list(&iE_list, 
                            p_protocolIEs_element->criticality,
                            p_protocolIEs_element->id,
                            &ie_list_index,
                            &send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
                    result = RRC_FAILURE;
                    break;
            }

            p_node = p_node->next;
        }

    }while(0);

    /* Parse the map for Error Indication */
    if (RRC_SUCCESS == lppa_parse_message_map(
                    p_asn1_ctx,
                    p_mme_ctx, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_lppa_id_e_CIDMeasurementInitiation,
                    T_lppa_LPPA_PDU_initiatingMessage,
                    lppa_reject,
                    p_error_indication))
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }
    RRC_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : s1ap_lppa_decode_e_CIDMeasurementTermination
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_lppa_decode_e_CIDMeasurementTermination(
    OSCTXT          *p_asn1_ctx,
    mme_context_t   *p_mme_ctx,
    lppa_E_CIDMeasurementTerminationCommand *p_e_CIDMeasurementTermination,
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t   *p_lppa_meas_term_cmd)
{
    U32             index = 0;
    OSRTDListNode   *p_node = RRC_NULL;
    lppa_E_CIDMeasurementTerminationCommand_protocolIEs_element
                                              *p_protocolIEs_element = PNULL;
    rrc_return_et  result = RRC_SUCCESS;
    lppa_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    lppa_message_data_t message_map =
      {2, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_lppa_id_E_SMLC_UE_Measurement_ID,lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_lppa_id_eNB_UE_Measurement_ID,lppa_mandatory, lppa_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_CIDMeasurementTermination);
    RRC_ASSERT(PNULL != p_lppa_meas_term_cmd);

    memset_wrapper(&iE_list, RRC_NULL, sizeof(lppa_error_ind_ie_list_t)); 
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t)); 

    do
    {
        p_node = p_e_CIDMeasurementTermination->protocolIEs.head;

        for (index = 0; index < p_e_CIDMeasurementTermination->protocolIEs.count; 
                        index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: next node returned NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }

            p_protocolIEs_element = 
            (lppa_E_CIDMeasurementTerminationCommand_protocolIEs_element*)p_node->data;

            switch(p_protocolIEs_element->id)
            {
                case ASN1V_lppa_id_E_SMLC_UE_Measurement_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_E_SMLC_UE_Measurement_ID", __FUNCTION__);

                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                &p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementTerminationCommand_IEs_1))
                    {
                        p_lppa_meas_term_cmd->e_smlc_ue_meas_id =
                                p_protocolIEs_element->value.u.
                                _lppa_E_CIDMeasurementTerminationCommand_IEs_1;
                    }
                    break;
                }
                case ASN1V_lppa_id_eNB_UE_Measurement_ID:
                {
                    RRC_TRACE(RRC_INFO,"%s: "
                      "ASN1V_lppa_id_eNB_UE_Measurement_ID", __FUNCTION__);

                    if (RRC_SUCCESS == lppa_validate_ie_value (
                                &message_map,
                                index,
                                p_protocolIEs_element->id,
                                &p_protocolIEs_element->value.u.
                                  _lppa_E_CIDMeasurementTerminationCommand_IEs_2))
                    {
                        p_lppa_meas_term_cmd->enb_ue_meas_id =
                                p_protocolIEs_element->value.u.
                                _lppa_E_CIDMeasurementTerminationCommand_IEs_2;
                    }
                    break;
                }
                default:
                    RRC_TRACE(RRC_ERROR,"%s: Invalid IE id.", __FUNCTION__);
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    lppa_add_to_err_ind_ie_list(&iE_list, 
                            p_protocolIEs_element->criticality,
                            p_protocolIEs_element->id,
                            &ie_list_index,
                            &send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
                    result = RRC_FAILURE;
                    break;
            }
            p_node = p_node->next;
        }
    } while(0);

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == lppa_parse_message_map(
                    p_asn1_ctx,
                    p_mme_ctx, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_lppa_id_e_CIDMeasurementTermination,
                    T_lppa_LPPA_PDU_initiatingMessage,
                    lppa_reject,
                    (rrc_lppa_error_indication_t *)PNULL)) ) 
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }
    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : s1ap_lppa_decode_ErrorIndication
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_lppa_decode_ErrorIndication(
        /*SPR 17777 +-*/
        lppa_ErrorIndication *p_errorIndication,
        s1ap_rrm_lppa_error_ind_t *p_lppa_error_ind)
{

    rrc_return_et  result = RRC_SUCCESS;
    U32             index = 0;
    OSRTDListNode   *p_node = RRC_NULL;
    lppa_ErrorIndication_protocolIEs_element *p_protocolIEs_element = PNULL;

    RRC_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    RRC_ASSERT(PNULL != p_errorIndication);
    RRC_ASSERT(PNULL != p_lppa_error_ind);


    do
    {
        p_node = p_errorIndication->protocolIEs.head;

        for (index = 0; index < p_errorIndication->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: next node returned NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
            p_protocolIEs_element =
              (lppa_ErrorIndication_protocolIEs_element*)p_node->data;

            switch(p_protocolIEs_element->id)
            {
                case ASN1V_lppa_id_Cause:
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_lppa_id_Cause", __FUNCTION__);
                    if (RRC_SUCCESS == validate_lppa_cause(
                                      p_protocolIEs_element->value.u._lppa_ErrorIndication_IEs_1))
                    {
                        p_lppa_error_ind->bitmask |= LPPA_ERROR_IND_CAUSE_PRESENT;
                        lppa_parse_cause(&p_lppa_error_ind->cause,
                                p_protocolIEs_element->value.u._lppa_ErrorIndication_IEs_1);
                    }
                    else
                    {
                        RRC_TRACE(RRC_INFO,"%s: Invalid lppa_cause.", __FUNCTION__);
                    }
                    break;

                case ASN1V_lppa_id_CriticalityDiagnostics:
                    RRC_TRACE(RRC_INFO,"%s: ASN1V_lppa_id_CriticalityDiagnostics", __FUNCTION__);
                    p_lppa_error_ind->bitmask |= LPPA_ERROR_IND_CRITIC_DIAG_PRESENT;
                    lppa_parse_CriticalityDiagnostics(
                                &p_lppa_error_ind->criticality_diagnostics,
                                p_protocolIEs_element->value.u._lppa_ErrorIndication_IEs_2);
                    lppa_log_criticality_diagostics(
                            p_protocolIEs_element->value.u._lppa_ErrorIndication_IEs_2);
                    break;
                default:
                    RRC_TRACE(RRC_ERROR,"%s: Invalid IE id.", __FUNCTION__);
                    result = RRC_FAILURE;
                    break;
            }
            p_node = p_node->next;
        }

    }while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : lppa_compose_lppa_ECGI
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void lppa_compose_lppa_ECGI(
    lppa_ECGI *p_asn_ecgi,
    lppa_ecgi_t *p_lppa_ecgi)
{
    p_asn_ecgi->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    l3_memcpy_wrapper(&p_asn_ecgi->pLMN_Identity.data,
               &p_lppa_ecgi->plmn_identity.plmn_id,
               MAX_PLMN_ID_BYTES);

    p_asn_ecgi->eUTRANcellIdentifier.numbits = CELL_ID_NBITS;
    l3_memcpy_wrapper(&p_asn_ecgi->eUTRANcellIdentifier.data,
               &p_lppa_ecgi->eutran_cell_id,
               EUTRAN_OCTET_SIZE);

    rtxDListInit(&p_asn_ecgi->iE_Extensions);
    rtxDListInit(&p_asn_ecgi->extElem1);
}


/****************************************************************************
 * Function Name  : lppa_compose_lppa_E_UTRANAccessPointPosition
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void lppa_compose_lppa_E_UTRANAccessPointPosition(
    lppa_E_UTRANAccessPointPosition *p_asn_acc_pt_pos,
    lppa_e_utran_access_point_pos_t *p_lppa_acc_pt_pos)
{
    p_asn_acc_pt_pos->latitudeSign = p_lppa_acc_pt_pos->latitude_sign;
    p_asn_acc_pt_pos->latitude     = p_lppa_acc_pt_pos->deg_of_latitude;
    p_asn_acc_pt_pos->longitude    = p_lppa_acc_pt_pos->deg_of_longitude;
    p_asn_acc_pt_pos->directionOfAltitude = 
                p_lppa_acc_pt_pos->deg_of_altitude;
    p_asn_acc_pt_pos->altitude     = p_lppa_acc_pt_pos->altitude;
    p_asn_acc_pt_pos->uncertaintySemi_major = 
                p_lppa_acc_pt_pos->uncertainty_semi_major;
    p_asn_acc_pt_pos->uncertaintySemi_minor = 
                p_lppa_acc_pt_pos->uncertainty_semi_minor;
    p_asn_acc_pt_pos->orientationOfMajorAxis = 
                p_lppa_acc_pt_pos->orientation_of_major_axis;
    p_asn_acc_pt_pos->uncertaintyAltitude = 
                p_lppa_acc_pt_pos->uncertainty_altitude;
    p_asn_acc_pt_pos->confidence = p_lppa_acc_pt_pos->confidence;

    rtxDListInit(&p_asn_acc_pt_pos->extElem1);
}

/****************************************************************************
 * Function Name  : lppa_compose_lppa_MeasuredResults
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_compose_lppa_MeasuredResults(
    OSCTXT    *p_asn1_ctx,
    lppa_MeasuredResults *p_asn_meas_results,
    lppa_meas_results_t  *p_lppa_meas_results)
{
    rrc_return_et result = RRC_SUCCESS;
    U32 count  = 0;
    U32 rsrp_count = 0;
    U32 rsrq_count = 0;
    OSRTDListNode  *p_node = PNULL;
    OSRTDListNode  *p_node_rsrp = PNULL;
    OSRTDListNode  *p_node_rsrq = PNULL;
    lppa_MeasuredResultsValue *p_meas_results_value = PNULL;
    lppa_ResultRSRP_Item  *p_rsrp_item = PNULL;
    lppa_ResultRSRQ_Item  *p_rsrq_item = PNULL;

    for (count = 0; count < p_lppa_meas_results->num_measured_results;
          count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
              lppa_MeasuredResultsValue,
              &p_node,
              &p_meas_results_value);
        if ((PNULL == p_node) || (PNULL == p_meas_results_value))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        asn1Init_lppa_MeasuredResultsValue(p_meas_results_value);

        if(p_lppa_meas_results->measured_results[count].bitmask &
                        LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT)
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_valueAngleOfArrival;
            p_meas_results_value->u.valueAngleOfArrival = 
              p_lppa_meas_results->measured_results[count].value_angle_of_arrival;
        }
        else if(p_lppa_meas_results->measured_results[count].bitmask &
                        LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT)
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_valueTimingAdvanceType1;
            p_meas_results_value->u.valueTimingAdvanceType1 = 
              p_lppa_meas_results->measured_results[count].value_timing_adv_type_1;
        }
        else if(p_lppa_meas_results->measured_results[count].bitmask &
                        LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT)
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_valueTimingAdvanceType2;
            p_meas_results_value->u.valueTimingAdvanceType2 = 
              p_lppa_meas_results->measured_results[count].value_timing_adv_type_2;
        }
        else if(p_lppa_meas_results->measured_results[count].bitmask &
                        LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT)
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_resultRSRP;
            p_meas_results_value->u.resultRSRP = 
                        rtxMemAllocType(p_asn1_ctx, lppa_ResultRSRP);
            if (PNULL == p_meas_results_value->u.resultRSRP)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
            asn1Init_lppa_ResultRSRP(p_meas_results_value->u.resultRSRP);

            for (rsrp_count = 0; rsrp_count < p_lppa_meas_results->measured_results[count].
                                              meas_result_rsrp.rsrp_count; rsrp_count++)
            {
                rtxDListAllocNodeAndData(p_asn1_ctx,
                      lppa_ResultRSRP_Item,
                      &p_node_rsrp,
                      &p_rsrp_item);
                if ((PNULL == p_node_rsrp) || (PNULL == p_rsrp_item))
                {
                    /* Not enough memory */
                    RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    result = RRC_FAILURE;
                    break;
                }
                asn1Init_lppa_ResultRSRP_Item(p_rsrp_item);

                p_rsrp_item->pCI = p_lppa_meas_results->measured_results[count].
                                       meas_result_rsrp.result_rsrp[rsrp_count].pci;
                p_rsrp_item->eARFCN = p_lppa_meas_results->measured_results[count].
                                      meas_result_rsrp.result_rsrp[rsrp_count].earfcn;
                if (p_lppa_meas_results->measured_results[count].meas_result_rsrp.
                    result_rsrp[rsrp_count].bitmask & LPPA_RESULT_RSRP_ECGI_PRESENT)
                {
                    p_rsrp_item->m.eCGIPresent = 1;
                    lppa_compose_lppa_ECGI(&p_rsrp_item->eCGI,
                                  &p_lppa_meas_results->measured_results[count].
                                  meas_result_rsrp.result_rsrp[rsrp_count].ecgi);
                }
                p_rsrp_item->valueRSRP = p_lppa_meas_results->measured_results[count].
                                      meas_result_rsrp.result_rsrp[rsrp_count].value_rsrp;
                rtxDListInit(&p_rsrp_item->iE_Extensions);
                rtxDListInit(&p_rsrp_item->extElem1);

                rtxDListAppendNode(p_meas_results_value->u.resultRSRP, p_node_rsrp);
            }
        }
        else if(p_lppa_meas_results->measured_results[count].bitmask &
                        LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT)
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_resultRSRQ;
            p_meas_results_value->u.resultRSRQ = 
                        rtxMemAllocType(p_asn1_ctx, lppa_ResultRSRQ);
            if (PNULL == p_meas_results_value->u.resultRSRQ)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
            asn1Init_lppa_ResultRSRQ(p_meas_results_value->u.resultRSRQ);

            for(rsrq_count = 0; rsrq_count < p_lppa_meas_results->measured_results[count].
                                              meas_result_rsrq.rsrq_count; rsrq_count++)
            {
                rtxDListAllocNodeAndData(p_asn1_ctx,
                      lppa_ResultRSRQ_Item,
                      &p_node_rsrq,
                      &p_rsrq_item);
                if ((PNULL == p_node_rsrq) || (PNULL == p_rsrq_item))
                {
                    /* Not enough memory */
                    RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    result = RRC_FAILURE;
                    break;
                }
                asn1Init_lppa_ResultRSRQ_Item(p_rsrq_item);

                p_rsrq_item->pCI = p_lppa_meas_results->measured_results[count].
                                      meas_result_rsrq.result_rsrq[rsrq_count].pci;
                p_rsrq_item->eARFCN = p_lppa_meas_results->measured_results[count].
                                      meas_result_rsrq.result_rsrq[rsrq_count].earfcn;

                if(p_lppa_meas_results->measured_results[count].meas_result_rsrq.
                   result_rsrq[rsrq_count].bitmask & LPPA_RESULT_RSRQ_ECGI_PRESENT)
                {
                    p_rsrq_item->m.eCGIPresent = 1;
                    lppa_compose_lppa_ECGI(&p_rsrq_item->eCGI,
                                &p_lppa_meas_results->measured_results[count].
                                  meas_result_rsrq.result_rsrq[rsrq_count].ecgi);
                }
                p_rsrq_item->valueRSRQ = p_lppa_meas_results->measured_results[count].
                                      meas_result_rsrq.result_rsrq[rsrq_count].value_rsrq;
                rtxDListInit(&p_rsrq_item->iE_Extensions);
                rtxDListInit(&p_rsrq_item->extElem1);

                rtxDListAppendNode(p_meas_results_value->u.resultRSRQ, p_node_rsrq);
            }
        }
        else
        {
            p_meas_results_value->t = T_lppa_MeasuredResultsValue_extElem1;
            p_meas_results_value->u.extElem1 = 
                          rtxMemAllocType(p_asn1_ctx, ASN1OpenType);
            if (PNULL == p_meas_results_value->u.extElem1)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        rtxDListAppendNode(p_asn_meas_results, p_node);
    }
    return result;
}
    
/****************************************************************************
 * Function Name  : lppa_compose_E_CID_MeasurementResult
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_compose_E_CID_MeasurementResult(
      OSCTXT    *p_asn1_ctx,
      lppa_E_CID_MeasurementResult *p_asn_meas_result,
      lppa_e_cid_meas_result_t  *p_lppa_meas_result)
{
    rrc_return_et result = RRC_SUCCESS;
    lppa_compose_lppa_ECGI(&p_asn_meas_result->servingCell_ID,
          &p_lppa_meas_result->serving_cell_id);

    p_asn_meas_result->servingCellTAC.numocts = TAC_OCTET_SIZE;
    l3_memcpy_wrapper(&p_asn_meas_result->servingCellTAC.data,
               &p_lppa_meas_result->serving_cell_tac,
               TAC_OCTET_SIZE);

    if (p_lppa_meas_result->bitmask & LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT)
    {
        p_asn_meas_result->m.e_UTRANAccessPointPositionPresent = 1;
        lppa_compose_lppa_E_UTRANAccessPointPosition( 
              &p_asn_meas_result->e_UTRANAccessPointPosition,
              &p_lppa_meas_result->e_utran_access_point_pos);
    }

    if (p_lppa_meas_result->bitmask & LPPA_MEASUREMENT_RESULT_PRESENT)
    {
        p_asn_meas_result->m.measuredResultsPresent = 1;
        result = lppa_compose_lppa_MeasuredResults(p_asn1_ctx,
              &p_asn_meas_result->measuredResults,
              &p_lppa_meas_result->meas_result);
    }

    rtxDListInit(&p_asn_meas_result->extElem1);
    return result;
}

/*******************************************************************************
 * FUNCTION NAME : lppa_parse_CriticalityDiagnostics
 *
 * DESCRIPTION : This function used to decode Criticality Diagnostics
 *
 * RETURNS : None
 *
 * ERROR : This function report error to the caller function, If decoder fails
 *         to decode message.
*******************************************************************************/
void  lppa_parse_CriticalityDiagnostics(
        lppa_criticality_diagno_t    *p_CriticalityDiagno,
        lppa_CriticalityDiagnostics  *p_CriticalityDiagnostics)
{
    RRC_UT_TRACE_ENTER();

    if(p_CriticalityDiagnostics->m.procedureCodePresent)
    {
        p_CriticalityDiagno->procedure_code =
                p_CriticalityDiagnostics->procedureCode;
        p_CriticalityDiagno->bitmask |=
            LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT;
    }

    if(p_CriticalityDiagnostics->m.triggeringMessagePresent)
    {
        p_CriticalityDiagno->triggering_msg =
                            p_CriticalityDiagnostics->triggeringMessage;
        p_CriticalityDiagno->bitmask |=
            LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT;
    }

    if(p_CriticalityDiagnostics->m.procedureCriticalityPresent)
    {
        p_CriticalityDiagno->proc_critic =
                p_CriticalityDiagnostics->procedureCriticality;
        p_CriticalityDiagno->bitmask |=
            LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT;
    }

    if(p_CriticalityDiagnostics->m.lppatransactionIDPresent)
    {
        p_CriticalityDiagno->lppa_trans_id =
                p_CriticalityDiagnostics->lppatransactionID;
        p_CriticalityDiagno->bitmask |=
            LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT;
    }

    RRC_UT_TRACE_EXIT();
}

/*******************************************************************************
 * FUNCTION NAME : lppa_parse_cause
 *
 * DESCRIPTION : This function used to decode lppa Cause.
 *
 * RETURNS : None
*******************************************************************************/
void  lppa_parse_cause(
            lppa_cause_t   *p_cause,
            lppa_Cause     *p_Cause)
{
    RRC_UT_TRACE_ENTER();

    p_cause->type = (U32)p_Cause->t;
    if(T_lppa_Cause_radioNetwork == p_Cause->t)
    {
        p_cause->value = p_Cause->u.radioNetwork;
    }
    else if(T_lppa_Cause_protocol == p_Cause->t)
    {
        p_cause->value = p_Cause->u.protocol;
    }
    else if (T_lppa_Cause_misc == p_Cause->t)
    {
        p_cause->value = p_Cause->u.misc;
    }
    else
    {
        RRC_TRACE(RRC_ERROR,"%s: ""Unknown cause type",
                   __FUNCTION__);
    }
    RRC_UT_TRACE_EXIT();
}

/*******************************************************************************
 * FUNCTION NAME : lppa_compose_Cause
 *
 * DESCRIPTION : This function used to encode lppa_Cause.
 *
 * RETURNS : SUCCESS/FAILURE
*******************************************************************************/
rrc_return_et  lppa_compose_Cause(lppa_Cause   *p_asn_cause,
                                  lppa_cause_t *p_lppa_cause)
{
    rrc_return_et   result = RRC_SUCCESS;


    p_asn_cause->t = (OSINT32)p_lppa_cause->type;
    switch (p_lppa_cause->type)
    {
        case T_lppa_Cause_radioNetwork:
            p_asn_cause->u.radioNetwork = p_lppa_cause->value;
            break;

        case T_lppa_Cause_protocol:
            p_asn_cause->u.protocol = p_lppa_cause->value;
            break;

        case T_lppa_Cause_misc:
            p_asn_cause->u.misc = p_lppa_cause->value;
            break;

        case T_lppa_Cause_extElem1:
            break;

        default:
            RRC_TRACE(RRC_ERROR, "Invalid Cause Type.");
            result = RRC_FAILURE;
    }
    return result;
}

/****************************************************************************
 * Function Name  : lppa_compose_CriticalityDiagnostics
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_compose_CriticalityDiagnostics(
    lppa_CriticalityDiagnostics *p_asn_crit_diag,
    lppa_criticality_diagno_t   *p_lppa_crit_diag)
{
    
    if (p_lppa_crit_diag->bitmask & LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT)
    {
        p_asn_crit_diag->m.procedureCodePresent = 1;
        p_asn_crit_diag->procedureCode = p_lppa_crit_diag->procedure_code;
    }

    if (p_lppa_crit_diag->bitmask & LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT)
    {
        p_asn_crit_diag->m.triggeringMessagePresent = 1;
        p_asn_crit_diag->triggeringMessage = p_lppa_crit_diag->triggering_msg;
    }

    if (p_lppa_crit_diag->bitmask & LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT)
    {
        p_asn_crit_diag->m.procedureCriticalityPresent = 1;
        p_asn_crit_diag->procedureCriticality = p_lppa_crit_diag->proc_critic;
    }

    if (p_lppa_crit_diag->bitmask & LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT)
    {
        p_asn_crit_diag->m.lppatransactionIDPresent = 1;
        p_asn_crit_diag->lppatransactionID = (U16)p_lppa_crit_diag->lppa_trans_id;
    }

    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : lppa_encode_E_CIDMeasurementInitiationResponse
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_encode_E_CIDMeasurementInitiationResponse(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_lppa_meas_init_resp)
{
    lppa_LPPA_PDU  lppa_pdu;
    rrc_return_et result = RRC_FAILURE;
    OSCTXT         asn1_ctx;
    OSRTDListNode  *p_node = PNULL;
    lppa_E_CIDMeasurementInitiationResponse *p_asn_meas_init_resp = PNULL;
    lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element
                                *p_protocolIE_elem = PNULL;

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded
         * by ASN Encoder */
        /* Set Pdu type to successfulOutcome message */
        lppa_pdu.t = T_lppa_LPPA_PDU_successfulOutcome;

        lppa_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
                                  lppa_SuccessfulOutcome);

        if (PNULL == lppa_pdu.u.successfulOutcome)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_SuccessfulOutcome(lppa_pdu.u.successfulOutcome);

        lppa_pdu.u.successfulOutcome->procedureCode = 
                               ASN1V_lppa_id_e_CIDMeasurementInitiation;
        lppa_pdu.u.successfulOutcome->criticality = lppa_reject;
        lppa_pdu.u.successfulOutcome->lppatransactionID = 
                               (U16)p_lppa_meas_init_resp->lppa_trans_id;

        lppa_pdu.u.successfulOutcome->value.t = T1lppa__e_CIDMeasurementInitiation;

        p_asn_meas_init_resp = rtxMemAllocType(&asn1_ctx,
                                lppa_E_CIDMeasurementInitiationResponse);
        if (PNULL == p_asn_meas_init_resp)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationResponse(p_asn_meas_init_resp);

        lppa_pdu.u.successfulOutcome->value.u.e_CIDMeasurementInitiation = 
                      p_asn_meas_init_resp;

        /* Compose id-E-SMLC-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element(
                              p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_E_SMLC_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T5lppa___lppa_E_CIDMeasurementInitiationResponse_IEs_1;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_1 = 
            p_lppa_meas_init_resp->e_smlc_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_init_resp->protocolIEs, p_node);

        /* Compose id-eNB-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element(
                              p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_eNB_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T5lppa___lppa_E_CIDMeasurementInitiationResponse_IEs_2;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_2 = 
            p_lppa_meas_init_resp->enb_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_init_resp->protocolIEs, p_node);

        /* Compose id-E-CID-MeasurementResult */
        if (p_lppa_meas_init_resp->bitmask & 
                    LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT)
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
            if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
            {
                /* Not enough memory */
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_E_CIDMeasurementInitiationResponse_protocolIEs_element(
                              p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_lppa_id_E_CID_MeasurementResult;
            p_protocolIE_elem->criticality = lppa_ignore;
            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T5lppa___lppa_E_CIDMeasurementInitiationResponse_IEs_3;

            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_3 =
                rtxMemAllocType(&asn1_ctx, lppa_E_CID_MeasurementResult);
            if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_3)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_E_CID_MeasurementResult(
             p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_3);
            
            if(RRC_FAILURE == lppa_compose_E_CID_MeasurementResult(&asn1_ctx,
               p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationResponse_IEs_3,
               &p_lppa_meas_init_resp->e_cid_meas_result))
            {
                /* failure */
                RRC_TRACE(RRC_ERROR, 
                    "%s: compose_E_CID_MeasurementResult failed.",__FUNCTION__);
                break;
            }

            rtxDListAppendNode(&p_asn_meas_init_resp->protocolIEs, p_node);
        }

        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "ASN1 Encoding of LPPA PDU failed.");
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            result = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;
}

/****************************************************************************
 * Function Name  : lppa_encode_E_CIDMeasurementInitiationFailure
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_encode_E_CIDMeasurementInitiationFailure(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_lppa_meas_init_fail,
    rrc_lppa_error_indication_t *p_error_indication)
{
    rrc_return_et result = RRC_FAILURE;
    lppa_LPPA_PDU  lppa_pdu;
    OSCTXT         asn1_ctx;
    OSRTDListNode  *p_node = PNULL;
    lppa_E_CIDMeasurementInitiationFailure *p_asn_meas_init_fail = PNULL;
    lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element
                                    *p_protocolIE_elem = PNULL;
    lppa_CriticalityDiagnostics *p_asn_crit_diag = NULL;


    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded
         * by ASN Encoder */
        /* Set Pdu type to successfulOutcome message */
        lppa_pdu.t = T_lppa_LPPA_PDU_unsuccessfulOutcome;

        lppa_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                        lppa_UnsuccessfulOutcome);

        if (PNULL == lppa_pdu.u.unsuccessfulOutcome)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_UnsuccessfulOutcome(lppa_pdu.u.unsuccessfulOutcome);

        lppa_pdu.u.unsuccessfulOutcome->procedureCode =
                            ASN1V_lppa_id_e_CIDMeasurementInitiation;
        lppa_pdu.u.unsuccessfulOutcome->criticality = lppa_reject;
        lppa_pdu.u.unsuccessfulOutcome->lppatransactionID =
                            (U16)p_lppa_meas_init_fail->lppa_trans_id;
        lppa_pdu.u.unsuccessfulOutcome->value.t = T1lppa__e_CIDMeasurementInitiation;

        p_asn_meas_init_fail = rtxMemAllocType(&asn1_ctx,
                                 lppa_E_CIDMeasurementInitiationFailure);
        if (PNULL == p_asn_meas_init_fail)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationFailure(p_asn_meas_init_fail);

        lppa_pdu.u.unsuccessfulOutcome->value.u.e_CIDMeasurementInitiation = 
                      p_asn_meas_init_fail;

        /* Compose id-E-SMLC-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element(
                            p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_E_SMLC_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T6lppa___lppa_E_CIDMeasurementInitiationFailure_IEs_1;
        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_1 = 
            p_lppa_meas_init_fail->e_smlc_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_init_fail->protocolIEs, p_node);

        /* Compose id-Cause */
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element(
                            p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_Cause;
        p_protocolIE_elem->criticality = lppa_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T6lppa___lppa_E_CIDMeasurementInitiationFailure_IEs_2;
        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_2 =
                    rtxMemAllocType(&asn1_ctx, lppa_Cause);
        if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_2)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_Cause(
            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_2);

        if(RRC_SUCCESS != lppa_compose_Cause(
                p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_2,
                &p_lppa_meas_init_fail->cause))
        {
            /* failure */
            RRC_TRACE(RRC_ERROR,
                    "%s: lppa_compose_Cause failed.",__FUNCTION__);
            break;
        }

        rtxDListAppendNode(&p_asn_meas_init_fail->protocolIEs, p_node);

        /* Compose id-CriticalityDiagnostics */
        if((T_lppa_Cause_protocol == p_lppa_meas_init_fail->cause.type) &&
                ((lppa_message_not_compatible_with_receiver_state == p_lppa_meas_init_fail->cause.value) || 
                 (lppa_semantic_error == p_lppa_meas_init_fail->cause.value)))
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
            if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
            {
                /* Not enough memory */
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element(
                              p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_lppa_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = lppa_ignore;
            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T6lppa___lppa_E_CIDMeasurementInitiationFailure_IEs_3;

            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3 =
                rtxMemAllocType(&asn1_ctx, lppa_CriticalityDiagnostics);
            if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_CriticalityDiagnostics(
              p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3);

            p_asn_crit_diag = p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3;

            p_asn_crit_diag->m.procedureCodePresent = 1;
            p_asn_crit_diag->procedureCode = ASN1V_lppa_id_e_CIDMeasurementInitiation;

            p_asn_crit_diag->m.triggeringMessagePresent = 1;
            p_asn_crit_diag->triggeringMessage = lppa_initiating_message;

            p_asn_crit_diag->m.procedureCriticalityPresent = 1;
            p_asn_crit_diag->procedureCriticality = lppa_reject; 

            p_asn_crit_diag->m.lppatransactionIDPresent = 1;
            p_asn_crit_diag->lppatransactionID = (U16)p_lppa_meas_init_fail->lppa_trans_id; 


            rtxDListAppendNode(&p_asn_meas_init_fail->protocolIEs, p_node);
        }
        else if((p_error_indication != PNULL) &&
                (p_error_indication->bitmask & 
                RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT))
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
            if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
            {
                /* Not enough memory */
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_E_CIDMeasurementInitiationFailure_protocolIEs_element(
                              p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_lppa_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = lppa_ignore;
            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T6lppa___lppa_E_CIDMeasurementInitiationFailure_IEs_3;

            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3 =
                rtxMemAllocType(&asn1_ctx, lppa_CriticalityDiagnostics);
            if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_CriticalityDiagnostics(
              p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3);

            p_asn_crit_diag = p_protocolIE_elem->value.u._lppa_E_CIDMeasurementInitiationFailure_IEs_3;

            if(RRC_FAILURE == lppa_compose_meas_init_fail_CriticalityDiagnostics(
                    &asn1_ctx, p_asn_crit_diag, 
                    &p_error_indication->criticality_diagnostics))
            {
               RRC_TRACE(RRC_ERROR,
                   "%s:lppa_compose_meas_init_fail_CriticalityDiagnostics.",__FUNCTION__);
                break;
            }

            rtxDListAppendNode(&p_asn_meas_init_fail->protocolIEs, p_node);
        }

        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "ASN1 Encoding of LPPA PDU failed.");
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            result = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;
}

/*****************************************************************************
 * Function Name  : lppa_compose_meas_init_fail_CriticalityDiagnostics
 * Inputs         :
 * Outputs        :
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                : RRC_FAILURE - Asn Encoding was not successful.
 * Description    :
 ******************************************************************************/
rrc_return_et lppa_compose_meas_init_fail_CriticalityDiagnostics(
        OSCTXT                       *p_asn1_ctx,
        lppa_CriticalityDiagnostics  *p_asn_crit_diag,
        lppa_CriticalityDiagnostics  *p_err_ind_crit_diag)
{
    rrc_return_et               retVal = RRC_SUCCESS;
    OSRTDListNode               *p_node = RRC_PNULL;
    OSRTDListNode               *p_crit_diag_list = RRC_PNULL;
    U16                         critic_diagno_count = 0;
    lppa_CriticalityDiagnostics_IE_List_element *p_critic_diag_info = RRC_PNULL;
    lppa_CriticalityDiagnostics_IE_List_element *p_critic_diag_elem = RRC_PNULL;

    if(p_err_ind_crit_diag->m.procedureCodePresent)
    {
        p_asn_crit_diag->m.procedureCodePresent = 1;
        p_asn_crit_diag->procedureCode =
            p_err_ind_crit_diag->procedureCode;
    }

    if(p_err_ind_crit_diag->m.triggeringMessagePresent)
    {
        p_asn_crit_diag->m.triggeringMessagePresent = 1;
        p_asn_crit_diag->triggeringMessage =
            p_err_ind_crit_diag->triggeringMessage;
    }

    if(p_err_ind_crit_diag->m.procedureCriticalityPresent)
    {
        p_asn_crit_diag->m.procedureCriticalityPresent = 1;
        p_asn_crit_diag->procedureCriticality =
            p_err_ind_crit_diag->procedureCriticality;
    }

    if(p_err_ind_crit_diag->m.lppatransactionIDPresent)
    {
        p_asn_crit_diag->m.lppatransactionIDPresent = 1;
        p_asn_crit_diag->lppatransactionID =
            p_err_ind_crit_diag->lppatransactionID;
    }

    if(p_err_ind_crit_diag->m.iEsCriticalityDiagnosticsPresent)
    {
        p_asn_crit_diag->m.iEsCriticalityDiagnosticsPresent = 1;
        p_crit_diag_list = p_err_ind_crit_diag->iEsCriticalityDiagnostics.head;
        for(critic_diagno_count = 0;
                (critic_diagno_count < p_err_ind_crit_diag->iEsCriticalityDiagnostics.count)
                && (RRC_PNULL != p_crit_diag_list);
                critic_diagno_count++)
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    lppa_CriticalityDiagnostics_IE_List_element,
                    &p_node,
                    &p_critic_diag_info);
            if((PNULL == p_node) || (PNULL == p_critic_diag_info))
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return RRC_FAILURE;;
            }
            asn1Init_lppa_CriticalityDiagnostics_IE_List_element(p_critic_diag_info);

            p_critic_diag_elem = (lppa_CriticalityDiagnostics_IE_List_element*)p_crit_diag_list->data;
            p_critic_diag_info->iECriticality = p_critic_diag_elem->iECriticality;
            p_critic_diag_info->iE_ID = p_critic_diag_elem->iE_ID;
            p_critic_diag_info->typeOfError = p_critic_diag_elem->typeOfError;

            rtxDListAppendNode(&p_asn_crit_diag->iEsCriticalityDiagnostics, p_node);
            p_crit_diag_list = p_crit_diag_list->next;
        }
    }
    return retVal;
}


/****************************************************************************
 * Function Name  : lppa_encode_E_CIDMeasurementFailureIndication
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_encode_E_CIDMeasurementFailureIndication(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_lppa_meas_fail_ind)
{
    rrc_return_et result = RRC_FAILURE;
    lppa_LPPA_PDU  lppa_pdu;
    OSCTXT         asn1_ctx;
    OSRTDListNode  *p_node = PNULL;
    lppa_E_CIDMeasurementFailureIndication *p_asn_meas_fail_ind = PNULL;
    lppa_E_CIDMeasurementFailureIndication_protocolIEs_element
                                *p_protocolIE_elem = PNULL;

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded
         * by ASN Encoder */
        /* Set Pdu type to Initiating message */
        lppa_pdu.t = T_lppa_LPPA_PDU_initiatingMessage;

        lppa_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                                  lppa_InitiatingMessage);

        if (PNULL == lppa_pdu.u.initiatingMessage)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_InitiatingMessage(lppa_pdu.u.initiatingMessage);

        lppa_pdu.u.initiatingMessage->procedureCode = 
                       ASN1V_lppa_id_e_CIDMeasurementFailureIndication;
        lppa_pdu.u.initiatingMessage->criticality = lppa_ignore;
        lppa_pdu.u.initiatingMessage->lppatransactionID = 
                               (U16)p_lppa_meas_fail_ind->lppa_trans_id;

        lppa_pdu.u.initiatingMessage->value.t = T1lppa__e_CIDMeasurementFailureIndication;

        p_asn_meas_fail_ind = rtxMemAllocType(&asn1_ctx,
                                lppa_E_CIDMeasurementFailureIndication);
        if (PNULL == p_asn_meas_fail_ind)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementFailureIndication(p_asn_meas_fail_ind);

        lppa_pdu.u.initiatingMessage->value.u.e_CIDMeasurementFailureIndication = 
                      p_asn_meas_fail_ind;

        /* Compose id-E-SMLC-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementFailureIndication_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementFailureIndication_protocolIEs_element(
                              p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_E_SMLC_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T7lppa___lppa_E_CIDMeasurementFailureIndication_IEs_1;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_1 = 
            p_lppa_meas_fail_ind->e_smlc_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_fail_ind->protocolIEs, p_node);

        /* Compose id-eNB-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementFailureIndication_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementFailureIndication_protocolIEs_element(
                              p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_eNB_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T7lppa___lppa_E_CIDMeasurementFailureIndication_IEs_2;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_2 = 
            p_lppa_meas_fail_ind->enb_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_fail_ind->protocolIEs, p_node);

        /* Compose id-Cause */
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_E_CIDMeasurementFailureIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementFailureIndication_protocolIEs_element(
                             p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_Cause;
        p_protocolIE_elem->criticality = lppa_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T7lppa___lppa_E_CIDMeasurementFailureIndication_IEs_3;
        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_3 =
                    rtxMemAllocType(&asn1_ctx, lppa_Cause);
        if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_3)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_Cause(
            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_3);

        if(RRC_SUCCESS != lppa_compose_Cause(
            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementFailureIndication_IEs_3,
            &p_lppa_meas_fail_ind->cause))
        {
            /* failure */
            RRC_TRACE(RRC_ERROR,
                    "%s: lppa_compose_Cause failed.",__FUNCTION__);
            break;
        }

        rtxDListAppendNode(&p_asn_meas_fail_ind->protocolIEs, p_node);

        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "ASN1 Encoding of LPPA PDU failed.");
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            result = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;
}

/****************************************************************************
 * Function Name  : lppa_encode_E_CIDMeasurementReport
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_encode_E_CIDMeasurementReport(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_report_t *p_lppa_meas_report)
{
    rrc_return_et result = RRC_FAILURE;
    lppa_LPPA_PDU  lppa_pdu;
    OSCTXT         asn1_ctx;
    OSRTDListNode  *p_node = PNULL;
    lppa_E_CIDMeasurementReport		 *p_asn_meas_report = PNULL;
    lppa_E_CIDMeasurementReport_protocolIEs_element
                                	 *p_protocolIE_elem = PNULL;
	
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded
         * by ASN Encoder */
        /* Set Pdu type to initiating message */
        lppa_pdu.t = T_lppa_LPPA_PDU_initiatingMessage;

        lppa_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                                  lppa_InitiatingMessage);

        if (PNULL == lppa_pdu.u.initiatingMessage)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_InitiatingMessage(lppa_pdu.u.initiatingMessage);

         lppa_pdu.u.initiatingMessage->procedureCode =
                       ASN1V_lppa_id_e_CIDMeasurementReport;
        lppa_pdu.u.initiatingMessage->criticality = lppa_ignore; 
        lppa_pdu.u.initiatingMessage->lppatransactionID =
                               (U16)p_lppa_meas_report->lppa_trans_id;
        lppa_pdu.u.initiatingMessage->value.t = T1lppa__e_CIDMeasurementReport;

        p_asn_meas_report = rtxMemAllocType(&asn1_ctx,
                                lppa_E_CIDMeasurementReport);
        if (PNULL == p_asn_meas_report)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementReport(p_asn_meas_report);

        lppa_pdu.u.initiatingMessage->value.u.e_CIDMeasurementReport =
                                      p_asn_meas_report;

        /* Compose id-E-SMLC-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementReport_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementReport_protocolIEs_element(
                              p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_E_SMLC_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T8lppa___lppa_E_CIDMeasurementReport_IEs_1;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_1 =
                    p_lppa_meas_report->e_smlc_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_report->protocolIEs, p_node);

        /* Compose id-eNB-UE-Measurement-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
               lppa_E_CIDMeasurementReport_protocolIEs_element,
               &p_node,
               &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementReport_protocolIEs_element(
                              p_protocolIE_elem);
        
	/* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_eNB_UE_Measurement_ID;
        p_protocolIE_elem->criticality = lppa_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T8lppa___lppa_E_CIDMeasurementReport_IEs_2;

        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_2 =
        	p_lppa_meas_report->enb_ue_meas_id;

        rtxDListAppendNode(&p_asn_meas_report->protocolIEs, p_node);

        /* Compose id-E-CID-MeasurementResult */
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_E_CIDMeasurementReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_E_CIDMeasurementReport_protocolIEs_element(
                             p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_E_CID_MeasurementResult;
        p_protocolIE_elem->criticality = lppa_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T8lppa___lppa_E_CIDMeasurementReport_IEs_3;
        p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_3 =
                    rtxMemAllocType(&asn1_ctx, lppa_E_CID_MeasurementResult);
        if (PNULL == p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_3)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        
	    asn1Init_lppa_E_CID_MeasurementResult(
            p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_3);
            
	    if(RRC_FAILURE == lppa_compose_E_CID_MeasurementResult(&asn1_ctx,
               p_protocolIE_elem->value.u._lppa_E_CIDMeasurementReport_IEs_3,
               &p_lppa_meas_report->e_cid_meas_result))
        {
            /* failure */
            RRC_TRACE(RRC_ERROR,
                "%s: compose_E_CID_MeasurementResult failed.",__FUNCTION__);
            break;
        }

        rtxDListAppendNode(&p_asn_meas_report->protocolIEs, p_node);

        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "ASN1 Encoding of LPPA PDU failed.");
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            result = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;
}


/*******************************************************************************
 * FUNCTION NAME : lppa_internal_encode_otdoa_information_resp
 *
 * DESCRIPTION : This function used to encode otdoa information response.
 *
 * RETURNS : This function returns encoded message buffer.
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 *******************************************************************************/
rrc_return_et  lppa_internal_encode_otdoa_information_resp(
        s1ap_rrm_lppa_otdoa_info_resp_t  *p_otdoa_resp,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len)
{
    rrc_return_et     retVal = RRC_FAILURE;

    OSCTXT                      asn1_ctx;
    lppa_LPPA_PDU               lppa_pdu;
    OSRTDListNode               *p_node = PNULL;
    lppa_OTDOAInformationResponse    *p_lppa_otdoa_info_resp = PNULL;
    lppa_OTDOAInformationResponse_protocolIEs_element  *p_protocolIE_elem = PNULL;

    RRC_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Successful Outcome */
        lppa_pdu.t = T_lppa_LPPA_PDU_successfulOutcome;

        lppa_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
                lppa_SuccessfulOutcome);
        if (PNULL == lppa_pdu.u.successfulOutcome)
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_SuccessfulOutcome(lppa_pdu.u.successfulOutcome);

        lppa_pdu.u.successfulOutcome->procedureCode = 
                    ASN1V_lppa_id_oTDOAInformationExchange;
        lppa_pdu.u.successfulOutcome->criticality = lppa_reject;

        /* Set the successful Outcome type to OTDOAInformationExchange*/
        lppa_pdu.u.successfulOutcome->value.t = T1lppa__oTDOAInformationExchange;

        p_lppa_otdoa_info_resp = rtxMemAllocType(&asn1_ctx,
                lppa_OTDOAInformationResponse);
        if(PNULL == p_lppa_otdoa_info_resp)
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_lppa_OTDOAInformationResponse(p_lppa_otdoa_info_resp);
        lppa_pdu.u.successfulOutcome->value.u.oTDOAInformationExchange = 
                                                   p_lppa_otdoa_info_resp;
        
        /*Compose id-OTDOACells*/
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_OTDOAInformationResponse_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_OTDOAInformationResponse_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_OTDOACells;
        p_protocolIE_elem->criticality = lppa_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T13lppa___lppa_OTDOAInformationResponse_IEs_1;


        p_protocolIE_elem->value.u._lppa_OTDOAInformationResponse_IEs_1 =
            rtxMemAllocType(&asn1_ctx, lppa_OTDOACells);
        if(PNULL == p_protocolIE_elem->value.u._lppa_OTDOAInformationResponse_IEs_1)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_OTDOACells(
                p_protocolIE_elem->value.u._lppa_OTDOAInformationResponse_IEs_1);

        if(RRC_FAILURE == lppa_compose_otdoa_cells(&asn1_ctx,
                    p_protocolIE_elem->value.u._lppa_OTDOAInformationResponse_IEs_1,
                    p_otdoa_resp))
        {
                RRC_TRACE(RRC_ERROR,
                    "%s: compose_otdoa_cells failed.",__FUNCTION__);
                break;
        }
        rtxDListAppendNode(&p_lppa_otdoa_info_resp->protocolIEs, p_node);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if(RT_OK != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = RRC_SUCCESS;
        }
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();

    return retVal;
}

/****************************************************************************
 * Function Name  : lppa_compose_otdoa_cells
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et lppa_compose_otdoa_cells(
		OSCTXT      *p_asn1_ctx,
		lppa_OTDOACells   *p_asn_otdoa_cells,
		s1ap_rrm_lppa_otdoa_info_resp_t  *p_otdoa_info_resp)
{
    U16  count = RRC_NULL;
    OSRTDListNode*    p_node_otdoa_cells = PNULL;
    lppa_OTDOACells_element*  p_otdoa_cells_elem = PNULL;
    for(count = 0; count < p_otdoa_info_resp->otdoa_cell_count; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                lppa_OTDOACells_element,
                &p_node_otdoa_cells,
                &p_otdoa_cells_elem);
        if((PNULL == p_node_otdoa_cells) || (PNULL == p_otdoa_cells_elem))
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return RRC_FAILURE;
        }
        asn1Init_lppa_OTDOACells_element(p_otdoa_cells_elem);
        if(RRC_SUCCESS != lppa_compose_otdoa_cell_info(p_asn1_ctx,
                   &p_otdoa_cells_elem->oTDOACellInfo,
                   &p_otdoa_info_resp->otda_cells_arr[count]))
        {
            RRC_TRACE(RRC_ERROR,
                    "%s: compose_otdoa_cell failed.",__FUNCTION__);
            return RRC_FAILURE;
        }
        rtxDListInit(&p_otdoa_cells_elem->iE_Extensions);
        rtxDListInit(&p_otdoa_cells_elem->extElem1);

        rtxDListAppendNode(p_asn_otdoa_cells, p_node_otdoa_cells);
    }
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : lppa_compose_otdoa_cell_info
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et  lppa_compose_otdoa_cell_info(
        OSCTXT      *p_asn1_ctx,
        lppa_OTDOACell_Information  *p_cell_info,
        lppa_otdoa_cells_t  *p_otdoa_cells)
{
    U16  count = RRC_NULL;    
    OSRTDListNode*    p_node_otdoa_cell_info = PNULL;
    lppa_OTDOACell_Information_Item  *p_otdoa_cell_info = PNULL;
	
    for(count = 0; count < p_otdoa_cells->otdoa_cell_info_count; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                lppa_OTDOACell_Information_Item,
                &p_node_otdoa_cell_info,
                &p_otdoa_cell_info);
        if(PNULL == p_node_otdoa_cell_info)
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_lppa_OTDOACell_Information_Item(p_otdoa_cell_info);
    
        if(p_otdoa_cells->otda_cells_info_arr[count].bitmask & 
                LPPA_OTDOA_CELLS_PCI_PRESENT)
        {
            p_otdoa_cell_info->u.pCI = p_otdoa_cells->otda_cells_info_arr[count].pci;
            p_otdoa_cell_info->t = T_lppa_OTDOACell_Information_Item_pCI;	
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask & 
                LPPA_OTDOA_CELLS_CELL_ID_PRESENT)
        {
            p_otdoa_cell_info->u.cellId = rtxMemAllocType(p_asn1_ctx, lppa_ECGI);
            if (PNULL == p_otdoa_cell_info->u.cellId)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_ECGI(p_otdoa_cell_info->u.cellId);
            lppa_compose_lppa_ECGI(
                    p_otdoa_cell_info->u.cellId,
                    &(p_otdoa_cells->otda_cells_info_arr[count].cell_id));
            p_otdoa_cell_info->t = T_lppa_OTDOACell_Information_Item_cellId;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask & 
                LPPA_OTDOA_CELLS_TAC_PRESENT)
        {
            p_otdoa_cell_info->u.tAC = rtxMemAllocType(p_asn1_ctx, lppa_TAC);
            if (PNULL == p_otdoa_cell_info->u.tAC)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_TAC(p_otdoa_cell_info->u.tAC);
            p_otdoa_cell_info->u.tAC->numocts = TAC_OCTET_SIZE;
            l3_memcpy_wrapper(&p_otdoa_cell_info->u.tAC->data,
                    &p_otdoa_cells->otda_cells_info_arr[count].otdoa_cells_tac.tac,
                    TAC_OCTET_SIZE);	
            p_otdoa_cell_info->t = T_lppa_OTDOACell_Information_Item_tAC;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_EARFCN_PRESENT)
        {
            p_otdoa_cell_info->u.eARFCN = 
                p_otdoa_cells->otda_cells_info_arr[count].earfcn;
            p_otdoa_cell_info->t = T_lppa_OTDOACell_Information_Item_eARFCN;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT)
        {
            p_otdoa_cell_info->u.pRS_Bandwidth = 
                p_otdoa_cells->otda_cells_info_arr[count].prs_band_width;
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_pRS_Bandwidth;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask & 
                LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT)
        {
            p_otdoa_cell_info->u.pRS_ConfigurationIndex = 
                p_otdoa_cells->otda_cells_info_arr[count].prs_config_index;
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_pRS_ConfigurationIndex;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT)
        {
            p_otdoa_cell_info->u.cPLength = 
                p_otdoa_cells->otda_cells_info_arr[count].cp_length;
            p_otdoa_cell_info->t = T_lppa_OTDOACell_Information_Item_cPLength;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT)
        {
            p_otdoa_cell_info->u.numberOfDlFrames = 
                p_otdoa_cells->otda_cells_info_arr[count].no_of_dl_frames;
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_numberOfDlFrames;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT)
        {
            p_otdoa_cell_info->u.numberOfAntennaPorts = 
                p_otdoa_cells->otda_cells_info_arr[count].no_of_antenna_ports;
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_numberOfAntennaPorts;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT)
        {
            p_otdoa_cell_info->u.sFNInitialisationTime = 
                    rtxMemAllocType(p_asn1_ctx, lppa_SFNInitialisationTime);
            if (PNULL == p_otdoa_cell_info->u.sFNInitialisationTime)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_SFNInitialisationTime(p_otdoa_cell_info->u.sFNInitialisationTime);
            p_otdoa_cell_info->u.sFNInitialisationTime->numbits = SFN_OCTET_SIZE*8;
            l3_memcpy_wrapper(p_otdoa_cell_info->u.sFNInitialisationTime->data,
                    &p_otdoa_cells->otda_cells_info_arr[count].
                    otdoa_sfn_init_time.sfn_initiation_time,
                    (SFN_OCTET_SIZE));
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_sFNInitialisationTime;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT)
        {
            p_otdoa_cell_info->u.e_UTRANAccessPointPosition = 
                    rtxMemAllocType(p_asn1_ctx, lppa_E_UTRANAccessPointPosition);
            if (PNULL == p_otdoa_cell_info->u.e_UTRANAccessPointPosition)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_E_UTRANAccessPointPosition(p_otdoa_cell_info->u.e_UTRANAccessPointPosition);
            lppa_compose_lppa_E_UTRANAccessPointPosition(
                    p_otdoa_cell_info->u.e_UTRANAccessPointPosition,
                    &p_otdoa_cells->otda_cells_info_arr[count].
                    e_utran_acces_point_pos);
            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_e_UTRANAccessPointPosition;
        }
        else if(p_otdoa_cells->otda_cells_info_arr[count].bitmask &
                LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT)
        {	
            p_otdoa_cell_info->u.pRSMutingConfiguration = rtxMemAllocType(p_asn1_ctx,
                                                           lppa_PRSMutingConfiguration);
            if (PNULL == p_otdoa_cell_info->u.pRSMutingConfiguration)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_PRSMutingConfiguration(
                                    p_otdoa_cell_info->u.pRSMutingConfiguration);
            lppa_compose_lppa_pRSMutingConfiguration(p_asn1_ctx,
                    p_otdoa_cell_info->u.pRSMutingConfiguration,
                    &p_otdoa_cells->otda_cells_info_arr[count].prs_muting_config);

            p_otdoa_cell_info->t = 
                T_lppa_OTDOACell_Information_Item_pRSMutingConfiguration;
        }
        rtxDListAppendNode(p_cell_info, p_node_otdoa_cell_info);
    }
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : lppa_compose_lppa_pRSMutingConfiguration
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void  lppa_compose_lppa_pRSMutingConfiguration(
        OSCTXT         *p_asn1_ctx,
        lppa_PRSMutingConfiguration  *p_asn_prs_muting_config,
		lppa_prs_muting_config_t  *p_prs_muting_config)
{
    if(p_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_TWO_PRESENT)
    {
        p_asn_prs_muting_config->u.two = 
            rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);
        if (PNULL == p_asn_prs_muting_config->u.two)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return;
        }
        p_asn_prs_muting_config->u.two->numbits = 2;
        p_asn_prs_muting_config->u.two->data[0] =
            p_prs_muting_config->muting_config_two.two << 6;
        p_asn_prs_muting_config->t = T_lppa_PRSMutingConfiguration_two;
    }
    else if(p_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT)
    {
        p_asn_prs_muting_config->u.four = 
            rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);
        if (PNULL == p_asn_prs_muting_config->u.four)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return;
        }
        p_asn_prs_muting_config->u.four->numbits = 4;
        p_asn_prs_muting_config->u.four->data[0] =
                p_prs_muting_config->muting_config_four.four << 4;
        p_asn_prs_muting_config->t = T_lppa_PRSMutingConfiguration_four;
    }
    else if(p_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT)
    {
        p_asn_prs_muting_config->u.eight = 
            rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);
        if (PNULL == p_asn_prs_muting_config->u.eight)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return;
        }
        p_asn_prs_muting_config->u.eight->numbits = 8;
        p_asn_prs_muting_config->u.eight->data[0] =
                p_prs_muting_config->muting_config_eight.eight[0];
        p_asn_prs_muting_config->t = T_lppa_PRSMutingConfiguration_eight;
    }
    else if(p_prs_muting_config->bitmask & LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT)
    {
        p_asn_prs_muting_config->u.sixteen = 
            rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);
        if (PNULL == p_asn_prs_muting_config->u.sixteen)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return;
        }
        p_asn_prs_muting_config->u.sixteen->numbits = 16;
        l3_memcpy_wrapper(p_asn_prs_muting_config->u.sixteen->data,
                p_prs_muting_config->muting_config_sixteen.sixteen,
                sizeof(U16));
        p_asn_prs_muting_config->t = T_lppa_PRSMutingConfiguration_sixteen;
    }
}

/*******************************************************************************
 * FUNCTION NAME : lppa_internal_encode_otdoa_information_fail
 *
 * DESCRIPTION : This function used to encode otdoa information failure.
 *
 * RETURNS : This function returns encoded message buffer.
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
*******************************************************************************/
rrc_return_et  lppa_internal_encode_otdoa_information_fail(
        s1ap_rrm_lppa_otdoa_info_fail_t  *p_otdoa_fail,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len)
{
    rrc_return_et     retVal = RRC_FAILURE;

    OSCTXT                      asn1_ctx;
    lppa_LPPA_PDU               lppa_pdu;
    OSRTDListNode               *p_node = PNULL;
    lppa_OTDOAInformationFailure_protocolIEs_element  *p_protocolIE_elem = PNULL;
    lppa_OTDOAInformationFailure    *p_lppa_otdoa_info_fail = PNULL;

    lppa_CriticalityDiagnostics *p_asn_crit_diag = NULL;


    RRC_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        memset_wrapper(&lppa_pdu, 0, sizeof(lppa_LPPA_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Successful Outcome */
        lppa_pdu.t = T_lppa_LPPA_PDU_unsuccessfulOutcome;

        lppa_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                lppa_UnsuccessfulOutcome);
        if (PNULL == lppa_pdu.u.unsuccessfulOutcome)
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_UnsuccessfulOutcome(lppa_pdu.u.unsuccessfulOutcome);

        lppa_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_lppa_id_oTDOAInformationExchange;
        lppa_pdu.u.unsuccessfulOutcome->criticality = lppa_reject;

        /* Set the successful Outcome type to OTDOAInformationExchange*/
        lppa_pdu.u.unsuccessfulOutcome->value.t = T1lppa__oTDOAInformationExchange;

        p_lppa_otdoa_info_fail = rtxMemAllocType(&asn1_ctx,
                lppa_OTDOAInformationFailure);
        if(PNULL == p_lppa_otdoa_info_fail)
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        else
        {
            RRC_TRACE(RRC_INFO,"Mem Alloc successful to OTDOAInformationFailure");
        }

        asn1Init_lppa_OTDOAInformationFailure(p_lppa_otdoa_info_fail);
        lppa_pdu.u.unsuccessfulOutcome->value.u.oTDOAInformationExchange = 
                                                     p_lppa_otdoa_info_fail;

        /*Compose id-Cause*/
        rtxDListAllocNodeAndData(&asn1_ctx,
                lppa_OTDOAInformationFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_OTDOAInformationFailure_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_lppa_id_Cause;
        p_protocolIE_elem->criticality = lppa_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T14lppa___lppa_OTDOAInformationFailure_IEs_1;


        p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx, lppa_Cause);
        if(PNULL == p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_1)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_lppa_Cause(
                p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_1);

        if(RRC_SUCCESS != lppa_compose_Cause(
                p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_1,
                &p_otdoa_fail->cause))
        {
            RRC_TRACE(RRC_ERROR,
                    "%s: compose_Cause failed.",__FUNCTION__);
            break;
        }
        rtxDListAppendNode(&p_lppa_otdoa_info_fail->protocolIEs, p_node);

        /* Compose id-CriticalityDiagnostics */
        if((T_lppa_Cause_protocol == p_otdoa_fail->cause.type) &&
                ((lppa_message_not_compatible_with_receiver_state == p_otdoa_fail->cause.value) || 
                 (lppa_semantic_error == p_otdoa_fail->cause.value)))
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
                    lppa_OTDOAInformationFailure_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);
            if((PNULL == p_node) || (PNULL == p_protocolIE_elem))
            {
                /* Not enough memory */
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_OTDOAInformationFailure_protocolIEs_element(
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_lppa_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = lppa_ignore;

            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T14lppa___lppa_OTDOAInformationFailure_IEs_2;


            p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_2 =
                rtxMemAllocType(&asn1_ctx, lppa_CriticalityDiagnostics);
            if(PNULL == p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_2)
            {
                RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_lppa_CriticalityDiagnostics(
                    p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_2);

            p_asn_crit_diag = p_protocolIE_elem->value.u._lppa_OTDOAInformationFailure_IEs_2;

            p_asn_crit_diag->m.procedureCodePresent = 1;
            p_asn_crit_diag->procedureCode = ASN1V_lppa_id_oTDOAInformationExchange;

            p_asn_crit_diag->m.triggeringMessagePresent = 1;
            p_asn_crit_diag->triggeringMessage = lppa_initiating_message;

            p_asn_crit_diag->m.procedureCriticalityPresent = 1;
            p_asn_crit_diag->procedureCriticality = lppa_reject; 

            p_asn_crit_diag->m.lppatransactionIDPresent = 1;
            p_asn_crit_diag->lppatransactionID = p_otdoa_fail->lppa_trans_id; 

            rtxDListAppendNode(&p_lppa_otdoa_info_fail->protocolIEs, p_node);
        }	

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if(RT_OK != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }

        *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
        retVal = RRC_SUCCESS;
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    return retVal;
}

/****************************************************************************
 * Function Name  : s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
    U8     routing_id,
    U16    asn_lppa_pdu_len,
    U8     *p_asn_lppa_pdu,
    U16    *p_asn_s1ap_pdu_len,
    U8     *p_asn_s1ap_pdu_msg)
{
    rrc_return_et         response = RRC_SUCCESS;
    S1AP_PDU              s1ap_pdu;
    OSCTXT                asn1_ctx;
    OSRTDListNode         *p_node = PNULL;
    s1ap_UplinkNonUEAssociatedLPPaTransport *p_ul_non_ue_asso_lppa = PNULL;
    s1ap_UplinkNonUEAssociatedLPPaTransport_protocolIEs_element 
                          *p_protocolIE_elem = PNULL;
    s1ap_LPPa_PDU         *p_s1ap_lppa_pdu = PNULL;

    RRC_UT_TRACE_ENTER();
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu, 0, sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded 
         * by ASN Encoder */
        /* Set Pdu type to Initiating message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = 
                    ASN1V_s1ap_id_uplinkNonUEAssociatedLPPaTransport;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        /*Set the initiating message type to uplinkNonUEAssociatedLPPaTransport*/
        s1ap_pdu.u.initiatingMessage->value.t = 
                    T1_uplinkNonUEAssociatedLPPaTransport;

        p_ul_non_ue_asso_lppa = rtxMemAllocType(&asn1_ctx, 
                                s1ap_UplinkNonUEAssociatedLPPaTransport);
        if (PNULL == p_ul_non_ue_asso_lppa)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkNonUEAssociatedLPPaTransport(p_ul_non_ue_asso_lppa);

        s1ap_pdu.u.initiatingMessage->value.u.
                uplinkNonUEAssociatedLPPaTransport = p_ul_non_ue_asso_lppa;

        /* Compose id-Routing-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkNonUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkNonUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Routing_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T111__UplinkNonUEAssociatedLPPaTransport_IEs_1;

        p_protocolIE_elem->value.u._UplinkNonUEAssociatedLPPaTransport_IEs_1=
            routing_id;

        rtxDListAppendNode(&p_ul_non_ue_asso_lppa->protocolIEs, p_node);

        /* Compose id-LPPa-PDU */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkNonUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkNonUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_LPPa_PDU;;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T111__UplinkNonUEAssociatedLPPaTransport_IEs_2;

        p_protocolIE_elem->value.u._UplinkNonUEAssociatedLPPaTransport_IEs_2 =
            rtxMemAllocType(&asn1_ctx,s1ap_LPPa_PDU);
        if(PNULL == p_protocolIE_elem->value.u._UplinkNonUEAssociatedLPPaTransport_IEs_2)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_LPPa_PDU(
         p_protocolIE_elem->value.u._UplinkNonUEAssociatedLPPaTransport_IEs_2);

        p_s1ap_lppa_pdu = p_protocolIE_elem->value.u._UplinkNonUEAssociatedLPPaTransport_IEs_2;

        p_s1ap_lppa_pdu->numocts = asn_lppa_pdu_len;
        p_s1ap_lppa_pdu->data = (U8 *)rtxMemAlloc(&asn1_ctx, p_s1ap_lppa_pdu->numocts);

        if(PNULL == p_s1ap_lppa_pdu->data)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        l3_memcpy_wrapper((void*)p_s1ap_lppa_pdu->data,
                    p_asn_lppa_pdu, p_s1ap_lppa_pdu->numocts);

        rtxDListAppendNode(&p_ul_non_ue_asso_lppa->protocolIEs, p_node);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_s1ap_pdu_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if(0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_TRACE(RRC_ERROR, 
            "ASN1 encoding of UplinkNonUEAssociatedLPPaTransport failed.");
            break;
        }
        else
        {
            *p_asn_s1ap_pdu_len= (U16)pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();

    return response;
}

/****************************************************************************
 * Function Name  : s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
rrc_return_et s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
    U32    mme_ue_s1ap_id,
    U32    enb_ue_s1ap_id,
    U8     routing_id,
    U16    asn_lppa_pdu_len,
    U8     *p_asn_lppa_pdu,
    U16    *p_asn_s1ap_pdu_len,
    U8     *p_asn_s1ap_pdu_msg)
{
    S1AP_PDU              s1ap_pdu;
    OSCTXT                asn1_ctx;
    rrc_return_et         response = RRC_SUCCESS;
    OSRTDListNode         *p_node = PNULL;
    s1ap_UplinkUEAssociatedLPPaTransport *p_ul_ue_asso_lppa = PNULL;
    s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element 
                          *p_protocolIE_elem = PNULL;
    s1ap_LPPa_PDU         *p_s1ap_lppa_pdu = PNULL;

    RRC_UT_TRACE_ENTER();
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded 
         * by ASN Encoder */
        /* Set Pdu type to Initiating message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = 
                    ASN1V_s1ap_id_uplinkUEAssociatedLPPaTransport;
        /*SPR_18844_START*/
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;
        /*SPR_18844_END*/

        /*Set the initiating message type to uplinkUEAssociatedLPPaTransport*/
        s1ap_pdu.u.initiatingMessage->value.t = 
                    T1_uplinkUEAssociatedLPPaTransport;

        p_ul_ue_asso_lppa = rtxMemAllocType(&asn1_ctx, 
                                s1ap_UplinkUEAssociatedLPPaTransport);
        if (PNULL == p_ul_ue_asso_lppa)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_UplinkUEAssociatedLPPaTransport(p_ul_ue_asso_lppa);

        s1ap_pdu.u.initiatingMessage->value.u.
                uplinkUEAssociatedLPPaTransport = p_ul_ue_asso_lppa;

        /* Compose id-MME-UE-S1AP-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */ 
        p_protocolIE_elem->id = ASN1V_s1ap_id_MME_UE_S1AP_ID; 
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T109__UplinkUEAssociatedLPPaTransport_IEs_1;

        p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_1 = 
            mme_ue_s1ap_id;

        rtxDListAppendNode(&p_ul_ue_asso_lppa->protocolIEs, p_node);

        /* Compose id-eNB-UE-S1AP-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_eNB_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T109__UplinkUEAssociatedLPPaTransport_IEs_2;

        p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_2 =
            enb_ue_s1ap_id;

        rtxDListAppendNode(&p_ul_ue_asso_lppa->protocolIEs, p_node);

        /* Compose id-Routing-ID */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Routing_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T109__UplinkUEAssociatedLPPaTransport_IEs_3;

        p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_3 =
            routing_id;

        rtxDListAppendNode(&p_ul_ue_asso_lppa->protocolIEs, p_node);

        /* Compose id-LPPa-PDU */
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if ((PNULL == p_node) || (PNULL == p_protocolIE_elem))
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_UplinkUEAssociatedLPPaTransport_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_LPPa_PDU;;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T109__UplinkUEAssociatedLPPaTransport_IEs_4;

        p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_4 =
            rtxMemAllocType(&asn1_ctx,s1ap_LPPa_PDU);
        if (PNULL == p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_4)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_LPPa_PDU(
         p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_4);

        p_s1ap_lppa_pdu = p_protocolIE_elem->value.u._UplinkUEAssociatedLPPaTransport_IEs_4;

        p_s1ap_lppa_pdu->numocts = asn_lppa_pdu_len;
        p_s1ap_lppa_pdu->data = (U8 *)rtxMemAlloc(&asn1_ctx, p_s1ap_lppa_pdu->numocts);

        if(PNULL == p_s1ap_lppa_pdu->data)
        {
            RRC_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        l3_memcpy_wrapper((void*)p_s1ap_lppa_pdu->data,
                    p_asn_lppa_pdu, p_s1ap_lppa_pdu->numocts);

        rtxDListAppendNode(&p_ul_ue_asso_lppa->protocolIEs, p_node);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_s1ap_pdu_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_TRACE(RRC_ERROR, 
            "ASN1 encoding of UplinkUEAssociatedLPPaTransport failed.");
            break;
        }
        else
        {
            *p_asn_s1ap_pdu_len= (U16)pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();

    return response;
}

/****************************************************************************
 * Function Name  : rrc_lppa_error_indication_tse_intrl_enc
 * Inputs         : pctxt (for memory allocation)
 *                  
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into lppa_LPPA_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_lppa_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,  /* size of result buffer */
 mme_context_t  *p_mme_context
 )
{
    lppa_LPPA_PDU  lppa_pdu;
    rrc_return_et result = RRC_FAILURE;
    lppa_ErrorIndication *p_lppa_ErrorIndication = PNULL;
    lppa_Cause cause;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt); 
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);

    do
    {
        /* S1PDU Init */
        asn1Init_lppa_LPPA_PDU(&lppa_pdu);
        lppa_pdu.t = T_lppa_LPPA_PDU_initiatingMessage;

        lppa_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, lppa_InitiatingMessage);

        if (PNULL == lppa_pdu.u.initiatingMessage)
        {       
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;  
        }

        asn1Init_lppa_InitiatingMessage(lppa_pdu.u.initiatingMessage);

        lppa_pdu.u.initiatingMessage->procedureCode =
            ASN1V_lppa_id_errorIndication;

        lppa_pdu.u.initiatingMessage->criticality = lppa_ignore;
        lppa_pdu.u.initiatingMessage->lppatransactionID = 
                         (U16)p_mme_context->lppa_info.lppa_trans_id; 

        lppa_pdu.u.initiatingMessage->value.t = T1lppa__errorIndication;
        lppa_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, lppa_ErrorIndication);

        if (PNULL == lppa_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_lppa_ErrorIndication = lppa_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_lppa_ErrorIndication(p_lppa_ErrorIndication);

        /* ONLY CAUSE HAS TO BE SENT IN CASE OF TSE */

        /*Fill the cause */

        cause.t = T_lppa_Cause_protocol;
        cause.u.protocol = lppa_transfer_syntax_error;

        if (RT_OK != asn1Append_lppa_ErrorIndication_protocolIEs_1
                (pctxt,
                 &p_lppa_ErrorIndication->protocolIEs,
                 &cause))
        {
            break;
        }
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (RT_OK != asn1PE_lppa_LPPA_PDU(pctxt, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    }
    while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_lppa_error_indication_ase_intrl_enc
 * Inputs         : pctxt (for memory allocation)
 *                  
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into lppa_LPPA_PDU
 ****************************************************************************/
rrc_return_et rrc_lppa_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_lppa_error_indication_t  *p_error_indication,
 mme_context_t  *p_mme_context
 )
{
    rrc_return_et result = RRC_FAILURE;
    lppa_LPPA_PDU lppa_pdu;
    lppa_ErrorIndication *p_lppa_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_error_indication);

    do
    {
        asn1Init_lppa_LPPA_PDU(&lppa_pdu);
        lppa_pdu.t = T_lppa_LPPA_PDU_initiatingMessage;

        lppa_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, lppa_InitiatingMessage);
        if (PNULL == lppa_pdu.u.initiatingMessage)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_lppa_InitiatingMessage(lppa_pdu.u.initiatingMessage);

        lppa_pdu.u.initiatingMessage->procedureCode =
            ASN1V_lppa_id_errorIndication;

        lppa_pdu.u.initiatingMessage->criticality = lppa_ignore;
        lppa_pdu.u.initiatingMessage->lppatransactionID = (U16)p_mme_context->lppa_info.lppa_trans_id;

        lppa_pdu.u.initiatingMessage->value.t = T1lppa__errorIndication;
        lppa_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, lppa_ErrorIndication);
        
        if (PNULL == lppa_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_lppa_msg = lppa_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_lppa_ErrorIndication(p_lppa_msg);

        if (RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_lppa_ErrorIndication_protocolIEs_1
                    (pctxt,
                     &p_lppa_msg->protocolIEs,
                     &p_error_indication->cause))
            {
                break;
            }
        }

        if (RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_lppa_ErrorIndication_protocolIEs_2
                    (pctxt,
                     &p_lppa_msg->protocolIEs,
                     &p_error_indication->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (RT_OK != asn1PE_lppa_LPPA_PDU(pctxt, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : s1ap_lppa_build_and_send_e_CIDMeasurementInitiationFailure
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_build_and_send_e_CIDMeasurementInitiationFailure(
        mme_context_t   *p_mme_ctx,
        U32             lppa_trans_id,
        rrc_lppa_error_indication_t *p_error_indication)
{
    s1ap_rrm_lppa_ecid_meas_init_fail_t lppa_meas_init_fail;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U16 asn_s1ap_pdu_msg_len = 0;
    rrc_bool_et send_meas_init_fail = RRC_TRUE;
    U32 count = 0;
    OSRTDListNode           *p_cric_diagno_info = RRC_PNULL;

    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_error_indication);

    RRC_UT_TRACE_ENTER();
    memset_wrapper(&lppa_meas_init_fail, RRC_NULL, sizeof(s1ap_rrm_lppa_ecid_meas_init_fail_t));

    lppa_meas_init_fail.lppa_trans_id = lppa_trans_id;

    if(p_error_indication->bitmask & RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT)
    {
        lppa_parse_cause(&lppa_meas_init_fail.cause,
                         &p_error_indication->cause);
    }

    if(p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent)
    {
        p_cric_diagno_info = p_error_indication->criticality_diagnostics.
                                          iEsCriticalityDiagnostics.head;
        for (count = 0; (count < p_error_indication->criticality_diagnostics.
                         iEsCriticalityDiagnostics.count) &&
                        (PNULL != p_cric_diagno_info); count++)
        {
            if(ASN1V_lppa_id_E_SMLC_UE_Measurement_ID ==
                    ((lppa_CriticalityDiagnostics_IE_List_element *)p_cric_diagno_info->data)->iE_ID)
                send_meas_init_fail = RRC_FALSE;

            p_cric_diagno_info = p_cric_diagno_info->next;
        }
    }

    if (RRC_TRUE == send_meas_init_fail)
    {
        lppa_meas_init_fail.e_smlc_ue_meas_id = p_mme_ctx->lppa_info.e_smlc_meas_id;

        if(RRC_SUCCESS != 
           lppa_encode_E_CIDMeasurementInitiationFailure(lppa_pdu_asn_msg_buff,
               &asn_lppa_pdu_msg_len, &lppa_meas_init_fail, p_error_indication))
        {
            RRC_S1AP_TRACE (RRC_ERROR, 
                "lppa_encode_E_CIDMeasurementInitiationFailure failed.");
            return;
        }

        if (RRC_SUCCESS !=
                s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                    p_error_indication->mme_ue_s1ap_id, 
                    p_error_indication->enb_ue_s1ap_id, 
                    p_error_indication->routing_id,
                    asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                    &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
        {
            RRC_S1AP_TRACE (RRC_ERROR,
                    "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
            return;
        }
        /* Check if the MME is in active state */
        if (MME_ACTIVE != p_mme_ctx->current_mme_state)
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "MME %d is not in active state", p_mme_ctx->mme_id);
            return;
        }

        if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx,
                    s1ap_pdu_asn_msg_buff,
                    asn_s1ap_pdu_msg_len,
                    PNULL))
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "s1ap_UplinkUEAssociatedLPPaTransport Message"
                    " failed to MME ID %d", p_mme_ctx->mme_id);
            return;
        }
    }
    else
    {
        /* send lppa_ErrorIndication */
        lppa_build_and_send_err_indication(p_mme_ctx,
                p_error_indication, PNULL);
    }
    RRC_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : s1ap_lppa_build_and_send_OTDOAInformationFailure
 * Inputs         : 
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void s1ap_lppa_build_and_send_OTDOAInformationFailure(
        mme_context_t   *p_mme_ctx,
        U32             lppa_trans_id,
        rrc_lppa_error_indication_t *p_error_indication)
{
    s1ap_rrm_lppa_otdoa_info_fail_t lppa_otdoa_info_fail;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U16 asn_s1ap_pdu_msg_len = 0;

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_error_indication);

    memset_wrapper(&lppa_otdoa_info_fail, RRC_NULL, sizeof(s1ap_rrm_lppa_otdoa_info_fail_t));

    lppa_otdoa_info_fail.lppa_trans_id = lppa_trans_id;

    if(p_error_indication->bitmask & RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT)
    {
        lppa_parse_cause(&lppa_otdoa_info_fail.cause,
                         &p_error_indication->cause);
    }

    if (RRC_SUCCESS != lppa_internal_encode_otdoa_information_fail(&lppa_otdoa_info_fail, 
                           lppa_pdu_asn_msg_buff, &asn_lppa_pdu_msg_len))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_internal_encode_otdoa_information_fail failed.");
        return;
    }

    if (RRC_SUCCESS !=
            s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
                p_error_indication->routing_id,
                asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport failed.");
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_ctx->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", p_mme_ctx->mme_id);
        return;
    }

    if (RRC_FAILURE == send_msg_to_mme(p_mme_ctx,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkNonUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", p_mme_ctx->mme_id);
        return;
    }

    RRC_UT_TRACE_EXIT();
}


/*******************************************************************************
 * FUNCTION NAME : lppa_encode_ErrorIndication
 *
 * DESCRIPTION : This function used to encode LPPA ErrorIndication.
 *
 * RETURNS : This function returns encoded message buffer.
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
*******************************************************************************/
rrc_return_et  lppa_encode_ErrorIndication(
        U8                *p_asn_msg,
        U16               *p_asn_msg_len,
        s1ap_rrm_lppa_error_ind_req_t  *p_lppa_error_ind)
{
    rrc_return_et     retVal = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    lppa_LPPA_PDU  lppa_pdu;
    lppa_ErrorIndication *p_lppa_ErrorIndication = PNULL;
    lppa_Cause  cause;
    lppa_CriticalityDiagnostics crit_diag;

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_lppa_error_ind);
    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_asn_msg_len);

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        /* S1PDU Init */
        asn1Init_lppa_LPPA_PDU(&lppa_pdu);
        lppa_pdu.t = T_lppa_LPPA_PDU_initiatingMessage;

        lppa_pdu.u.initiatingMessage =
            rtxMemAllocType(&asn1_ctx, lppa_InitiatingMessage);

        if (PNULL == lppa_pdu.u.initiatingMessage)
        {       
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;  
        }

        asn1Init_lppa_InitiatingMessage(lppa_pdu.u.initiatingMessage);

        lppa_pdu.u.initiatingMessage->procedureCode =
            ASN1V_lppa_id_errorIndication;

        lppa_pdu.u.initiatingMessage->criticality = lppa_ignore;
        lppa_pdu.u.initiatingMessage->lppatransactionID = 
                         (U16)p_lppa_error_ind->lppa_trans_id; 

        lppa_pdu.u.initiatingMessage->value.t = T1lppa__errorIndication;
        lppa_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(&asn1_ctx, lppa_ErrorIndication);

        if (PNULL == lppa_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_lppa_ErrorIndication = lppa_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_lppa_ErrorIndication(p_lppa_ErrorIndication);

        /*Compose id-Cause*/
        memset_wrapper(&cause, RRC_NULL, sizeof(lppa_Cause));

        if (RRC_SUCCESS != lppa_compose_Cause(
                            &cause, &p_lppa_error_ind->cause))
            break;

        if (RT_OK != asn1Append_lppa_ErrorIndication_protocolIEs_1(
                 &asn1_ctx,
                 &p_lppa_ErrorIndication->protocolIEs,
                 &cause))
            break;

        /*Compose CriticalityDiagnostics*/
        memset_wrapper(&crit_diag, RRC_NULL, sizeof(lppa_CriticalityDiagnostics));

        if (RRC_SUCCESS != lppa_compose_CriticalityDiagnostics(
                    &crit_diag, &p_lppa_error_ind->criticality_diagnostics))
            break;

        if (RT_OK != asn1Append_lppa_ErrorIndication_protocolIEs_2(
                    &asn1_ctx,
                    &p_lppa_ErrorIndication->protocolIEs,
                    &crit_diag))
            break;

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg, LPPA_MAX_ASN1_BUF_LEN, TRUE);

        if (RT_OK != asn1PE_lppa_LPPA_PDU(&asn1_ctx, &lppa_pdu))
        {
            RRC_TRACE(RRC_ERROR, "ASN1 encoding failed.");
            break;
        }

        *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
        retVal = RRC_SUCCESS;
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return retVal;
}
