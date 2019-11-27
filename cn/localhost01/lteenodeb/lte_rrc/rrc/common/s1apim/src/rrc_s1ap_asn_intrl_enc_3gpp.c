/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_s1ap_asn_intrl_enc_3gpp.c,v 1.20 2010/12/01 12:57:19 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description :
 *   This file contains functions that encode
 *   ASN.1 based internal representation of S1AP messages
 *   into ASN.1 encoded S1AP messages.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rrc_s1ap_asn_intrl_enc_3gpp.c,v $
 * Revision 1.20  2010/12/01 12:57:19  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.19  2010/10/14 12:53:13  gur18569
 * fixed memory leaks
 *
 * Revision 1.3.4.21  2010/10/14 10:19:44  gur25381
 * fixed memory leak
 *
 * Revision 1.3.4.20  2010/10/11 10:48:09  gur18569
 * commented asn print
 *
 * Revision 1.3.4.19  2010/10/11 07:23:23  gur18569
 * commented asn1 print statements
 *
 * Revision 1.3.4.18  2010/10/08 12:36:38  gur21006
 * TDD changes merged
 *
 * Revision 1.3.4.17  2010/10/08 05:47:45  gur18569
 * fixed merge issue
 *
 * Revision 1.3.4.16  2010/10/07 12:30:33  gur22047
 * updated
 *
 * Revision 1.3.4.15  2010/10/07 12:00:31  gur22047
 * file modified for UT after ssit changes
 *
 * Revision 1.3.4.14  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.3.4.13  2010/10/04 07:04:02  gur18569
 * commentd asn print
 *
 * Revision 1.3.4.12  2010/09/22 14:38:24  gur16768
 * made the alignment
 *
 * Revision 1.3.4.11  2010/09/07 12:30:42  gur18569
 * bug fixed during UT
 *
 * Revision 1.3.4.10  2010/09/04 12:42:42  gur16768
 * removed printPDU
 *
 * Revision 1.3.4.9  2010/08/30 12:21:58  gur16768
 * modified for src ho
 *
 * Revision 1.3.4.8  2010/08/30 11:55:15  gur25381
 * Added print for ASN Message
 *
 * Revision 1.3.4.7  2010/08/20 14:57:12  gur18569
 * code fix for trg ho
 *
 * Revision 1.3.4.6  2010/08/10 13:55:07  gur16768
 * code review comments incorporated
 *
 * Revision 1.3.4.5  2010/08/05 08:02:31  gur15697
 * merging round 2
 *
 * Revision 1.3.4.4  2010/07/26 06:40:36  gur18569
 * added ho requset intrl enc
 *
 * Revision 1.3.4.3  2010/07/21 08:21:59  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3.4.2  2010/07/16 05:18:02  gur22047
 * review comments incorporation
 *
 * Revision 1.3.4.1  2010/06/23 13:37:15  gur15697
 * Updated for EUTRAN/UTRAN asn enc dec changes
 *
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.4  2010/02/02 13:18:15  ukr15916
 * tabs->spaces expanded
 *
 * Revision 1.2.2.3  2010/01/08 13:36:03  ukr18880
 * UECC S1AP  updated
 *
 * Revision 1.2.2.2  2010/01/08 13:29:41  ukr15916
 * error_indication handling updated
 *
 * Revision 1.2.2.1  2010/01/08 12:52:33  ukr15916
 * error_indication_added
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.5  2009/12/17 18:39:49  ukr15916
 * function header comment updated
 *
 * Revision 1.1.4.4  2009/12/09 13:21:21  ukr15916
 * ASN_OK removed. Use RT_OK instead
 *
 * Revision 1.1.4.3  2009/12/08 16:45:45  ukr15916
 * PN_2, PN_3 fixed
 *
 * Revision 1.1.4.2  2009/12/07 00:15:19  ukr15916
 * ASN_S1AP_BUF_TYPE and ASN_RRC_BUF_TYPE are used instead of values
 *
 * Revision 1.1.4.1  2009/11/26 18:33:45  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.22  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.21  2009/11/19 15:00:37  ukr16018
 * W80.
 *
 * Revision 1.1.2.20  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.19  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.18  2009/11/11 20:31:01  ukr15916
 * uecc_s1ap_build_and_send_initial_context_setup_failure() moved to ASN S1AP
 *
 * Revision 1.1.2.17  2009/11/11 19:04:26  ukr15916
 * rrc_s1ap_initial_context_setup_response_intrl_enc(),
 *  rrc_s1ap_initial_context_setup_failure_intrl_enc() added
 *
 * Revision 1.1.2.16  2009/11/11 08:23:50  ukr16032
 * UECC - UE Connection Setup failure at the final stage handling added
 *
 * Revision 1.1.2.15  2009/11/10 16:45:55  ukr15916
 * rrc_s1ap_initial_context_setup_request_intrl_dec(),
 *
 * Revision 1.1.2.14  2009/11/10 11:42:41  ukr15916
 * downlink_nas_transport and nas_non_delivery_indication updated for ASN S1AP
 *
 * Revision 1.1.2.13  2009/11/09 15:10:51  ukr15916
 * ue_release_request updated for ASN S1AP
 *
 * Revision 1.1.2.12  2009/11/09 11:43:54  ukr15916
 * uplink_nas_transport updated to S1AP ASN
 *
 * Revision 1.1.2.11  2009/11/08 20:12:52  ukr15916
 * uecc_s1ap_build_and_send_initial_ue_msg_req updated for ASN S1AP.
 * Test scripts updated.
 *
 * Revision 1.1.2.10  2009/11/08 13:58:07  ukr15916
 * rrc_s1ap_initial_ue_message_t added
 *
 * Revision 1.1.2.9  2009/11/08 12:53:15  ukr15916
 * s1ap_ue_context_release_complete ASN version implemented
 *
 * Revision 1.1.2.8  2009/11/08 00:27:16  ukr15916
 * s1ap-related code updated in UECC
 *
 * Revision 1.1.2.7  2009/11/03 14:22:55  ukr15916
 * updated
 *
 * Revision 1.1.2.6  2009/11/03 13:46:15  ukr15916
 * rrc_s1ap_asn_intrl_dec_3gpp added.
 * parameters check added.
 *
 * Revision 1.1.2.5  2009/11/03 12:29:41  ukr15916
 *
 *
 ****************************************************************************/

 /****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_s1ap_asn_intrl_enc_3gpp.h"
#include "rrc_common_utils.h"
#include "rrc_logging.h"

 /****************************************************************************
 * Global Variables
 ****************************************************************************/
S1AP_PDU s1ap_pdu;

/* ERAB SETUP START */

/****************************************************************************
 * Function Name  : rrc_s1ap_erab_setup_response_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_erab_setup_response
 *                  p_uecc_gb_context
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_erab_setup_response into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_erab_setup_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context*/
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_setup_response_t *p_erab_setup_response
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_E_RABSetupResponse  *p_s1ap_msg;
    
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_erab_setup_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_E_RABSetup;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;
        s1ap_pdu.u.successfulOutcome->value.t = T1_e_RABSetup;
        s1ap_pdu.u.successfulOutcome->value.u.e_RABSetup =
            rtxMemAllocType(pctxt, s1ap_E_RABSetupResponse);

        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.e_RABSetup)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.e_RABSetup;

        asn1Init_s1ap_E_RABSetupResponse (p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_E_RABSetupResponse_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_erab_setup_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_E_RABSetupResponse_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_erab_setup_response->enb_ue_s1ap_id))
        {
            break;
        }

        if (RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_SETUP_LIST_PRESENT &
            p_erab_setup_response->bitmask)
        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABSetupResponse_protocolIEs_3(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_erab_setup_response->e_rab_setup_list))
            {
                break;
            }
        }

        if (RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT &
            p_erab_setup_response->bitmask)
        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABSetupResponse_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_erab_setup_response->e_rab_failed_to_setup_list))
            {
                break;
            }
        }

        if (RRC_S1AP_ERAB_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT &
            p_erab_setup_response->bitmask)
        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABSetupResponse_protocolIEs_5(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_erab_setup_response->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
 
        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}



/* ERAB SETUP STOP */
/****************************************************************************
 * Function Name  : rrc_s1ap_initial_context_setup_response_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_initial_context_setup_response
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_context_setup_response into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_initial_context_setup_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context*/
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_initial_context_setup_response_t *p_initial_context_setup_response
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_InitialContextSetupResponse *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_initial_context_setup_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_InitialContextSetup;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_initialContextSetup;
        s1ap_pdu.u.successfulOutcome->value.u.initialContextSetup =
            rtxMemAllocType(pctxt, s1ap_InitialContextSetupResponse);
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.initialContextSetup)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.initialContextSetup;

        asn1Init_s1ap_InitialContextSetupResponse(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_InitialContextSetupResponse_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_context_setup_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialContextSetupResponse_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_context_setup_response->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialContextSetupResponse_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_initial_context_setup_response->e_rab_setup_list))
        {
            break;
        }

        if
   (RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT &
            p_initial_context_setup_response->bitmask)

        {
            if (RT_OK !=
                asn1Append_s1ap_InitialContextSetupResponse_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_initial_context_setup_response->e_rab_failed_to_setup_list))
            {
                break;
            }
        }

        if
        (RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT
            &
            p_initial_context_setup_response->bitmask)

        {
            if (RT_OK !=
                asn1Append_s1ap_InitialContextSetupResponse_protocolIEs_5(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_initial_context_setup_response->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
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
 * Function Name  : rrc_s1ap_initial_context_setup_failure_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_initial_context_setup_failure
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_context_setup_failure into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_initial_context_setup_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_initial_context_setup_failure_t *p_initial_context_setup_failure
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_InitialContextSetupFailure *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_initial_context_setup_failure);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_unsuccessfulOutcome;

        s1ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, UnsuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_UnsuccessfulOutcome(s1ap_pdu.u.unsuccessfulOutcome);

        s1ap_pdu.u.unsuccessfulOutcome->procedureCode =
            ASN1V_s1ap_id_InitialContextSetup;

        s1ap_pdu.u.unsuccessfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.unsuccessfulOutcome->value.t = T1_initialContextSetup;
        s1ap_pdu.u.unsuccessfulOutcome->value.u.initialContextSetup =
            rtxMemAllocType(pctxt, s1ap_InitialContextSetupFailure);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome->value.u.
            initialContextSetup)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.unsuccessfulOutcome->value.u.
            initialContextSetup;

        asn1Init_s1ap_InitialContextSetupFailure(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_InitialContextSetupFailure_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_context_setup_failure->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialContextSetupFailure_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_context_setup_failure->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialContextSetupFailure_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_initial_context_setup_failure->cause))
        {
            break;
        }

        if
        (RRC_S1AP_INITIAL_CONTEXT_SETUP_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT
            &
            p_initial_context_setup_failure->bitmask)

        {
            if (RT_OK !=
                asn1Append_s1ap_InitialContextSetupFailure_protocolIEs_4(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_initial_context_setup_failure->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
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
 * Function Name  : rrc_s1ap_ue_context_release_request_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_uecc_gb_context
 *                  p_ue_context_release_request
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_context_release_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_context_release_request_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_release_request_t   *p_ue_context_release_request
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UEContextReleaseRequest *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_context_release_request);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_UEContextReleaseRequest;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_uEContextReleaseRequest;
        s1ap_pdu.u.initiatingMessage->value.u.uEContextReleaseRequest =
            rtxMemAllocType(pctxt, s1ap_UEContextReleaseRequest);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.
            uEContextReleaseRequest)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.
            uEContextReleaseRequest;

        asn1Init_s1ap_UEContextReleaseRequest(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UEContextReleaseRequest_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_release_request->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UEContextReleaseRequest_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_release_request->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UEContextReleaseRequest_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_context_release_request->cause))
        {
            break;
        }
        
        if(p_ue_context_release_request->bitmask & RRC_S1AP_CONTEXT_RELEASE_REQUEST_GW_RELEASE_INDICATION)
        {
            if (RT_OK != asn1Append_s1ap_UEContextReleaseRequest_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_ue_context_release_request->gw_context_release_indication))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
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
 * Function Name  : rrc_s1ap_ue_context_release_complete_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_context_release_complete
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_context_release_complete into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_context_release_complete_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_release_complete_t  *p_ue_context_release_complete
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UEContextReleaseComplete *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_context_release_complete);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_UEContextRelease;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_uEContextRelease;
        s1ap_pdu.u.successfulOutcome->value.u.uEContextRelease =
            rtxMemAllocType(pctxt, s1ap_UEContextReleaseComplete);
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.uEContextRelease)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.uEContextRelease;

        asn1Init_s1ap_UEContextReleaseComplete(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UEContextReleaseComplete_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_release_complete->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UEContextReleaseComplete_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_release_complete->enb_ue_s1ap_id))
        {
            break;
        }

        if (RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_CRITICALITY_DIAGNOSTICS_PRESENT
            &
            p_ue_context_release_complete->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_UEContextReleaseComplete_protocolIEs_3
                (pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_ue_context_release_complete->criticality_diagnostics))
            {
                break;
            }
        }

#ifdef ENDC_ENABLED
		if(RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_SECONDARY_REPORT_PRESENT &
			p_ue_context_release_complete->bitmask)
		{
			OSRTDListNode				*p_node = PNULL;
			s1ap_UEContextReleaseComplete_protocolIEs_element *	   p_protocolIE_elem = PNULL;
			OSRTDList  ueContextReleaseComplete_IEs_7;
			
			rtxDListAllocNodeAndData(pctxt, 
					s1ap_UEContextReleaseComplete_protocolIEs_element, 
					&p_node, 
					&p_protocolIE_elem);
			if (PNULL == p_node)
			{
				/* Not enough memory */
				RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
				break;
			}
			asn1Init_s1ap_UEContextReleaseComplete_protocolIEs_element (
					p_protocolIE_elem);
			
			/* Fill protocol IE Id and criticality */
			p_protocolIE_elem->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportList;
			p_protocolIE_elem->criticality = s1ap_ignore;
			
			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T57__UEContextReleaseComplete_IEs_7;
			
			rtxDListInit(&ueContextReleaseComplete_IEs_7);
			p_protocolIE_elem->value.u._UEContextReleaseComplete_IEs_7  = 
											   &ueContextReleaseComplete_IEs_7;			

			if (RRC_SUCCESS !=	 compose_s1ap_secondary_rat_data_usage_report_list(
						pctxt,
						p_protocolIE_elem->value.u._UEContextReleaseComplete_IEs_7,
						&p_ue_context_release_complete->secondary_rat_data_usage_report_list))
			{
				break;
			}

			rtxDListAppendNode(&p_s1ap_msg->protocolIEs, p_node);
		}
#endif

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
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
 * Function Name  : rrc_s1ap_initial_ue_message_intrl_enc
 * Inputs         : p_uecc_gb_context 
 *                  pctxt (for memory allocation),
 *                  p_initial_ue_message
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_ue_message into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_initial_ue_message_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_initial_ue_message_t   *p_initial_ue_message   /* source */
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_InitialUEMessage *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_initial_ue_message);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "[S1AP_INITIAL_UE_MSG_REQ] ASN malloc failed");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_initialUEMessage;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_initialUEMessage;
        s1ap_pdu.u.initiatingMessage->value.u.initialUEMessage =
            rtxMemAllocType(pctxt, s1ap_InitialUEMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.initialUEMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "[S1AP_INITIAL_UE_MSG_REQ] ASN malloc failed");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.initialUEMessage;

        asn1Init_s1ap_InitialUEMessage(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_1(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_ue_message->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_2(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_initial_ue_message->nas_pdu))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_3(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_initial_ue_message->tai))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_4(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_initial_ue_message->eutran_cgi))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_5(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_initial_ue_message->rrc_establishment_cause))
        {
            break;
        }

        if (RRC_S1AP_INITIAL_UE_MESSAGE_S_TMSI_PRESENT &
            p_initial_ue_message->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_6(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_initial_ue_message->s_tmsi))
            {
                break;
            }
        }

        if (RRC_S1AP_INITIAL_UE_MESSAGE_CSG_ID_PRESENT &
            p_initial_ue_message->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_7(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_initial_ue_message->csg_id))
            {
                break;
            }
        }
        /* SPR 13807 Fix Start */
        if (RRC_S1AP_INITIAL_UE_MESSAGE_GUMMEI_PRESENT &
            p_initial_ue_message->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_8(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_initial_ue_message->gummei))
            {
                break;
            }
        }
        /* SPR 13807 Fix Stop */
        if (RRC_S1AP_INITIAL_CELL_ACCESS_MODE_PRESENT &
            p_initial_ue_message->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_9(pctxt,
                &p_s1ap_msg->protocolIEs,
                p_initial_ue_message->cell_access_mode))
            {
                break;
            }
        }
        /* lipa start */
	if (RRC_S1AP_TRANSPORT_LAYER_ADDRESS_PRESENT &
            p_initial_ue_message->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_InitialUEMessage_protocolIEs_10(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_initial_ue_message->transport_layer_address))
            {
                break;
            }
        }
        /* lipa end */
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "[S1AP_INITIAL_UE_MSG_REQ] ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_uplink_nas_transport_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_uplink_nas_transport
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_uplink_nas_transport into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_uplink_nas_transport_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */ 
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_uplink_nas_transport_t *p_uplink_nas_transport /* source */
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UplinkNASTransport *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_uplink_nas_transport);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_uplinkNASTransport;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_uplinkNASTransport;
        s1ap_pdu.u.initiatingMessage->value.u.uplinkNASTransport =
            rtxMemAllocType(pctxt, s1ap_UplinkNASTransport);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.uplinkNASTransport)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.uplinkNASTransport;

        asn1Init_s1ap_UplinkNASTransport(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_1(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_uplink_nas_transport->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_2(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_uplink_nas_transport->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_3(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_uplink_nas_transport->nas_pdu))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_4(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_uplink_nas_transport->eutran_cgi))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_5(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_uplink_nas_transport->tai))
        {
            break;
        }
        /* lipa start */
        if ( p_uplink_nas_transport->bitmask & 
              RRC_LGW_TRANSPORT_LAYER_ADDRESS_PRESENT )
        {
            if (RT_OK != asn1Append_s1ap_UplinkNASTransport_protocolIEs_6(pctxt,
                         &p_s1ap_msg->protocolIEs,
                         &p_uplink_nas_transport->transport_layer_address))
            {
              break;
            }
        }
        /* lipa end */
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
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
 * Function Name  : rrc_s1ap_nas_non_delivery_indication_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_nas_non_delivery_indication
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_nas_non_delivery_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_nas_non_delivery_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_nas_non_delivery_indication_t  *p_nas_non_delivery_indication
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_NASNonDeliveryIndication *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_nas_non_delivery_indication);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_NASNonDeliveryIndication;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_nASNonDeliveryIndication;
        s1ap_pdu.u.initiatingMessage->value.u.nASNonDeliveryIndication =
            rtxMemAllocType(pctxt, s1ap_NASNonDeliveryIndication);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.
            nASNonDeliveryIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.
            nASNonDeliveryIndication;

        asn1Init_s1ap_NASNonDeliveryIndication(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_NASNonDeliveryIndication_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_nas_non_delivery_indication->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_NASNonDeliveryIndication_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_nas_non_delivery_indication->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_NASNonDeliveryIndication_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_nas_non_delivery_indication->nas_pdu))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_NASNonDeliveryIndication_protocolIEs_4(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_nas_non_delivery_indication->cause))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
 * Function Name  : rrc_uecc_error_indication_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_context_release_complete
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_uecc_error_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_error_indication_t  *p_error_indication
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_ErrorIndication *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_error_indication);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_ErrorIndication;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_errorIndication;
        s1ap_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, s1ap_ErrorIndication);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_s1ap_ErrorIndication(p_s1ap_msg);

        if (RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_ErrorIndication_protocolIEs_1(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_error_indication->mme_ue_s1ap_id))
            {
                break;
            }
        }

        if (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_ErrorIndication_protocolIEs_2(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_error_indication->enb_ue_s1ap_id))
            {
                break;
            }
        }

        if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_ErrorIndication_protocolIEs_3
                (pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_error_indication->cause))
            {
                break;
            }
        }

        if (RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_ErrorIndication_protocolIEs_4
                (pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_error_indication->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* Trace Start Procedure start */
/****************************************************************************
 * Function Name  : rrc_s1ap_trace_failure_indication_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_trace_failure_indication
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_trace_failure_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_trace_failure_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_trace_failure_indication_t  *p_trace_failure_indication
)
{
    rrc_return_et result = RRC_FAILURE;
    S1AP_PDU s1ap_pdu;
    s1ap_TraceFailureIndication *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_trace_failure_indication);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_TraceFailureIndication;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_traceFailureIndication;
        s1ap_pdu.u.initiatingMessage->value.u.traceFailureIndication =
            rtxMemAllocType(pctxt, s1ap_TraceFailureIndication);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.traceFailureIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.traceFailureIndication;

        asn1Init_s1ap_TraceFailureIndication(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_TraceFailureIndication_protocolIEs_1(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_trace_failure_indication->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_TraceFailureIndication_protocolIEs_2(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_trace_failure_indication->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_TraceFailureIndication_protocolIEs_3(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_trace_failure_indication->e_utran_trace_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_TraceFailureIndication_protocolIEs_4(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_trace_failure_indication->cause))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)(pe_GetMsgLen(pctxt));

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_ue_capability_info_indication_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_context_release_complete
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_capability_info_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_capability_info_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_capability_info_indication_t  *p_ue_capability_info_indication
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UECapabilityInfoIndication *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_capability_info_indication);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_UECapabilityInfoIndication;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_uECapabilityInfoIndication;
        s1ap_pdu.u.initiatingMessage->value.u.uECapabilityInfoIndication =
            rtxMemAllocType(pctxt, s1ap_UECapabilityInfoIndication);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.
            uECapabilityInfoIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.
            uECapabilityInfoIndication;

        asn1Init_s1ap_UECapabilityInfoIndication(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UECapabilityInfoIndication_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_capability_info_indication->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UECapabilityInfoIndication_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_capability_info_indication->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UECapabilityInfoIndication_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_capability_info_indication->ue_radio_capability))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
 * Function Name  : rrc_s1ap_handover_required_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_handover_required
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_handover_required into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_required_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_required_t *p_handover_required
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverRequired *p_s1ap_msg = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_handover_required);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_HandoverPreparation;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        s1ap_pdu.u.initiatingMessage->value.t = T1_handoverPreparation;
        s1ap_pdu.u.initiatingMessage->value.u.handoverPreparation =
            rtxMemAllocType(pctxt, s1ap_HandoverRequired);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.handoverPreparation)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.handoverPreparation;

        asn1Init_s1ap_HandoverRequired(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_handover_required->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_handover_required->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_handover_required->handover_type))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_4(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_handover_required->cause))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_5(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_handover_required->target_id))
        {
            break;
        }

        if (
          RRC_S1AP_HANDOVER_REQUIRED_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_6(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_handover_required->direct_forwarding_path_availability))
            {
                break;
            }
        }

        if (
          RRC_S1AP_HANDOVER_REQUIRED_SRVCC_HO_INDICATION_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_7(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_handover_required->srvcc_ho_indication))
            {
                break;
            }
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_8(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_handover_required->source_to_target_transparent_container))
        {
            break;
        }

        if (
          RRC_S1AP_HANDOVER_REQUIRED_SOURCE_TO_TARGET_TRANSPARENT_CONTAINER_SECONDARY_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_9(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_handover_required->\
                source_to_target_transparent_container_secondary))
            {
                break;
            }
        }

        if (
          RRC_S1AP_HANDOVER_REQUIRED_MS_CLASSMARK2_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_10(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_handover_required->ms_classmark2))
            {
                break;
            }
        }

        if (
          RRC_S1AP_HANDOVER_REQUIRED_MS_CLASSMARK3_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_11(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_handover_required->ms_classmark3))
            {
                break;
            }
        }
    /* CR_592_start*/

        if (
          RRC_S1AP_HANDOVER_REQUIRED_CSG_ID_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_12(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_handover_required->csg_id))
            {
                break;
            }
        }
        if (
          RRC_S1AP_HANDOVER_REQUIRED_CELL_ACCESS_MODE_PRESENT
          &
          p_handover_required->bitmask)
        {
        
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_13(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_handover_required->cell_access_mode))
            {
                break;
            }
        }
    /* CR_592_stop*/
        /* SRVCC start */
        if(
         RRC_S1AP_HANDOVER_REQUIRED_PS_SERVICE_NOT_AVAILABLE_PRESENT
         &
         p_handover_required->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_HandoverRequired_protocolIEs_14(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                p_handover_required->ps_service_not_available))
            {
                break;
            }
        }
        /* SRVCC stop */


        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        /*asn1Print_S1AP_PDU("HO REQUIRED PDU ", &s1ap_pdu);*/

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
 * Function Name  : rrc_s1ap_rrc_container_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_handover_required
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs HandoverPreparationInformation
 *                  into s1ap_RRC_Container ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_rrc_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_RRC_Container  *p_rrc_container,

    /* source */
    HandoverPreparationInformation *p_ho_preparation_information
)
{
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_FAILURE;
    U8 msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    OSOCTET *p_data = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_rrc_container);
    RRC_ASSERT(PNULL != p_ho_preparation_information);

    /* Init ASN1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
            p_uecc_gb_context->facility_name,
            RRC_ERROR,
            "[HandoverPreparationInformation] "
            "ASN1 context initialization failed.");
        return RRC_FAILURE;
    }

    do
    {
        pu_setBuffer(&asn1_ctx, msg_buf,
                              S1AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_HandoverPreparationInformation(&asn1_ctx, 
                    p_ho_preparation_information))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "[HandoverPreparationInformation] ASN1 encoding failed.");
            break;
        }

        p_rrc_container->numocts = pe_GetMsgLen(&asn1_ctx);

        p_data = (U8 *)rtxMemAlloc(pctxt, p_rrc_container->numocts);

        if (PNULL == p_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
             p_uecc_gb_context->facility_name,
             RRC_FATAL,
             "[HandoverPreparationInformation]Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_data, msg_buf, p_rrc_container->numocts);

        p_rrc_container->data = p_data; 

        result = RRC_SUCCESS;
    }while(0);

    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_last_visited_utran_cell_information_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_handover_required
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs ranap_LastVisitedUTRANCell_Item
 *                  into s1ap_LastVisitedUTRANCellInformation ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_last_visited_utran_cell_information_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */

     /* source */
    ranap_LastVisitedUTRANCell_Item  *p_last_visited_utran_cell_Item
)
{
    rrc_return_et  result = RRC_FAILURE;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_last_visited_utran_cell_Item);

    do
    {
        pu_setBuffer(pctxt, p_buff, UECC_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_ranap_LastVisitedUTRANCell_Item (pctxt,
                    p_last_visited_utran_cell_Item)) 
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "[rrc_s1ap_last_visited_utran_cell_information_intrl_enc] ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result = RRC_SUCCESS;
    }while(0);

    RRC_UT_TRACE_EXIT();
    return result;



}

/****************************************************************************
 * Function Name  : rrc_s1ap_source_enb_to_targer_enb_container_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_handover_required
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs s1ap_SourceeNB_ToTargeteNB_TransparentContainer
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_source_enb_to_targer_enb_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
    s1ap_SourceeNB_ToTargeteNB_TransparentContainer *p_container
)
{
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_FAILURE;
    U8 msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    OSOCTET *p_data = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_source_to_target_transparent_container);
    RRC_ASSERT(PNULL != p_container);

    do 
    {
        /* ASN Encode message */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                    "[rrc_s1ap_source_enb_to_targer_enb_container_intrl_enc] "
                    "ASN1 context initialization failed.");
            return RRC_FAILURE;
        }

        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
                asn1PE_s1ap_SourceeNB_ToTargeteNB_TransparentContainer(&asn1_ctx, p_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }
       
        p_source_to_target_transparent_container->numocts = pe_GetMsgLen(&asn1_ctx);

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
             p_uecc_gb_context->facility_name,
             RRC_INFO,
             "[SourceeNB_ToTargeteNB_TransparentContainer]Container length %d.",
             p_source_to_target_transparent_container->numocts);

        p_data = (U8 *)rtxMemAlloc(pctxt, p_source_to_target_transparent_container->numocts);

        if (PNULL == p_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,
               RRC_FATAL,
               "[SourceeNB_ToTargeteNB_TransparentContainer]Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_data, msg_buf, p_source_to_target_transparent_container->numocts);

     p_source_to_target_transparent_container->data = p_data;
     
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    
    RRC_UT_TRACE_EXIT();
    return result;
    
}


/****************************************************************************
 * Function Name  : rrc_s1ap_enb_status_transfer_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_enb_status_transfer
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_enb_status_transfer into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_enb_status_transfer_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT   *pctxt,         /* for memory allocation */
    U8           *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_enb_status_transfer_t *p_enb_status_transfer
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_ENBStatusTransfer *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_enb_status_transfer);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_eNBStatusTransfer;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;
        s1ap_pdu.u.initiatingMessage->value.t = T1_eNBStatusTransfer;

        s1ap_pdu.u.initiatingMessage->value.u.eNBStatusTransfer =
            rtxMemAllocType(pctxt, s1ap_ENBStatusTransfer);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.eNBStatusTransfer)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.eNBStatusTransfer;

        asn1Init_s1ap_ENBStatusTransfer(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_ENBStatusTransfer_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_enb_status_transfer->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_ENBStatusTransfer_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_enb_status_transfer->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_ENBStatusTransfer_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_enb_status_transfer->enb_status_transfer_transparent_container))
        {
            break;
        }

        /* ASN Encode message */
        /*asn1Print_S1AP_PDU("eNB STATUS TRANSFER ", &s1ap_pdu);*/
        
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    }while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : rrc_s1ap_handover_cancel_intrl_enc
 * Inputs         :  p_uecc_gb_context
 *                   pctxt (for memory allocation),
 *                     p_ue_handover_cancel
 * Outputs        : p_buff (ASN.1 encoded message),
 *                      p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_context_release_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_cancel_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_cancel_t   *p_ue_handover_cancel
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverCancel *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_handover_cancel);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_HandoverCancel;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        s1ap_pdu.u.initiatingMessage->value.t = T1_handoverCancel;
        s1ap_pdu.u.initiatingMessage->value.u.handoverCancel =
            rtxMemAllocType(pctxt, s1ap_HandoverCancel);
        
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.handoverCancel)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.handoverCancel;

        asn1Init_s1ap_HandoverCancel(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverCancel_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_cancel->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverCancel_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_cancel->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverCancel_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_handover_cancel->cause))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
 * Function Name  : rrc_s1ap_handover_failure_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                     p_ue_handover_failure
 * Outputs        : p_buff (ASN.1 encoded message),
 *                      p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_context_release_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_failure_t   *p_ue_handover_failure
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverFailure *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_handover_failure);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_unsuccessfulOutcome;

        s1ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, UnsuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_UnsuccessfulOutcome(s1ap_pdu.u.unsuccessfulOutcome);

        s1ap_pdu.u.unsuccessfulOutcome->procedureCode = 
                                                   ASN1V_s1ap_id_HandoverResourceAllocation;

        s1ap_pdu.u.unsuccessfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.unsuccessfulOutcome->value.t = T1_handoverResourceAllocation;
        s1ap_pdu.u.unsuccessfulOutcome->value.u.handoverResourceAllocation =
            rtxMemAllocType(pctxt, s1ap_HandoverFailure);
        
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome->value.u.handoverResourceAllocation)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_s1ap_msg = s1ap_pdu.u.unsuccessfulOutcome->value.u.handoverResourceAllocation;

        asn1Init_s1ap_HandoverFailure(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_1(
                                pctxt,
                                &p_s1ap_msg->protocolIEs,
                                p_ue_handover_failure->mme_ue_s1ap_id))
        {
            break;
        }


        if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_2(
                                pctxt,
                                &p_s1ap_msg->protocolIEs,
                                &p_ue_handover_failure->cause))
        {
            break;
        }

        if (RRC_S1AP_HANDOVER_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_ue_handover_failure->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_3(
                                   pctxt,
                                   &p_s1ap_msg->protocolIEs,
                                   &p_ue_handover_failure->criticality_diagnostics))
            {
                break;
            }
        }
        
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
 * Function Name  : rrc_s1ap_handover_notify_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_handover_notify
 * Outputs        : p_buff (ASN.1 encoded message),
 *                      p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_context_release_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_notify_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_notify_t   *p_ue_handover_notify
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverNotify *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_handover_notify);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_HandoverNotification;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_handoverNotification;
        s1ap_pdu.u.initiatingMessage->value.u.handoverNotification =
            rtxMemAllocType(pctxt, s1ap_HandoverNotify);
        
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.handoverNotification)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.handoverNotification;

        asn1Init_s1ap_HandoverNotify(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverNotify_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_notify->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverNotify_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_notify->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverNotify_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_handover_notify->e_utran_cgi))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverNotify_protocolIEs_4(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_handover_notify->tai))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        /*asn1Print_S1AP_PDU("S1AP HO NOTIFY", &s1ap_pdu);*/

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;

}


/****************************************************************************
 * Function Name  : rrc_s1ap_handover_request_ack_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                     p_ue_handover_request_ack
 * Outputs        : p_buff (ASN.1 encoded message),
 *                      p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_ue_handover_request_ack into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_request_ack_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_handover_request_ack_t *p_ue_handover_request_ack
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverRequestAcknowledge *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_handover_request_ack);
    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode = ASN1V_s1ap_id_HandoverResourceAllocation;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_handoverResourceAllocation;
        s1ap_pdu.u.successfulOutcome->value.u.handoverResourceAllocation =
            rtxMemAllocType(pctxt, s1ap_HandoverRequestAcknowledge);
        
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.handoverResourceAllocation)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.handoverResourceAllocation;

        asn1Init_s1ap_HandoverRequestAcknowledge(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_request_ack->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_handover_request_ack->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_handover_request_ack->e_rab_admitted_list))
        {
            break;
        }

        if (p_ue_handover_request_ack->bitmask & 
            RRC_S1AP_HANDOVER_REQUEST_ACK_FAILED_TO_SETUP_LIST_PRESENT)
        {
            if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_ue_handover_request_ack->e_rab_failed_to_setup_list))
            {
                break;
            }
        }

        if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_5(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_ue_handover_request_ack->target_to_source_transparent_container))
        {
            break;
        }

/*CR_592_start*/
        if(p_ue_handover_request_ack->bitmask &
           RRC_S1AP_HANDOVER_REQUEST_ACK_CSG_ID_PRESENT)
        {
          if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_6(
             pctxt,
             &p_s1ap_msg->protocolIEs,
             &p_ue_handover_request_ack->csg_id))
          {
              break;
          }
        }
/*CR_592_stop*/


        if (p_ue_handover_request_ack->bitmask & 
            RRC_S1AP_HANDOVER_REQUEST_ACK_DIAGNOSTICS_PRESENT)
        {
            if (RT_OK != asn1Append_s1ap_HandoverRequestAcknowledge_protocolIEs_7(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_ue_handover_request_ack->criticality_diagnostics))
            {
                break;
            }
        }

        
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }
        /*asn1Print_S1AP_PDU("ASN: HO ACK ",&s1ap_pdu);*/

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;

}


/****************************************************************************
 * Function Name  : rrc_s1ap_target_enb_to_source_enb_container_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                     p_enb_container
 * Outputs        : p_container contains the ASN.1 encoded message and number of octets
 *
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs s1ap_SourceeNB_ToTargeteNB_TransparentContainer
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_target_enb_to_source_enb_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    s1ap_TargeteNB_ToSourceeNB_TransparentContainer *p_enb_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
)
{
    rrc_return_et result = RRC_FAILURE;
    U8 msg[S1AP_MAX_ASN1_BUF_LEN];
    OSCTXT asn1_ctx;
    U8 *p_cont_data = PNULL;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_enb_container);
    RRC_ASSERT(PNULL != p_container);

        /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[ s1ap_ENBStatusTransfer] ASN context initialization failed");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do 
    {
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, msg, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
 asn1PE_s1ap_TargeteNB_ToSourceeNB_TransparentContainer(&asn1_ctx, p_enb_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        p_container->numocts = pe_GetMsgLen(&asn1_ctx);

        /* p_container->data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts); */
        p_cont_data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts);

        if (PNULL == p_cont_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,RRC_FATAL,
               "[uecc_s1ap_target_enb_to_source_enb_container_intrl_enc]Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_cont_data, msg, p_container->numocts);

        p_container->data = p_cont_data;

        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : rrc_var_short_mac_input_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                     p_var_short_mac_input
 * Outputs        : p_buff (ASN.1 encoded message),
 *                      p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_var_short_mac_input into ASN
 ****************************************************************************/
rrc_return_et rrc_var_short_mac_input_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8              *p_buff,        /* ASN.1 encoded message */
    U16            *p_buff_size,   /* size of result buffer */
    /* source */
    VarShortMAC_Input  *p_var_short_mac_input
)
{
    rrc_return_et  result = RRC_FAILURE;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_var_short_mac_input);

    do
    {
        pu_setBuffer(pctxt, p_buff, UECC_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_VarShortMAC_Input(pctxt, 
                                                                           p_var_short_mac_input))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[rrc_var_short_mac_input_intrl_enc] ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result = RRC_SUCCESS;
    }while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_initial_ue_context_modification_response_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_context_mod_response
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_ue_context_modification_response into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_initial_ue_context_modification_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_modification_response_t *p_ue_context_mod_response
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UEContextModificationResponse *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_context_mod_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_UEContextModification;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_uEContextModification;
        s1ap_pdu.u.successfulOutcome->value.u.uEContextModification =
            rtxMemAllocType(pctxt, s1ap_UEContextModificationResponse);
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.uEContextModification)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.uEContextModification;

        asn1Init_s1ap_UEContextModificationResponse(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UEContextModificationResponse_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_mod_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UEContextModificationResponse_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_mod_response->enb_ue_s1ap_id))
        {
            break;
        }

        if(RRC_S1AP_UE_CONTEXT_MODIFICATION_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT &
            p_ue_context_mod_response->bitmask)
        {
            if (RT_OK !=
                  asn1Append_s1ap_UEContextModificationResponse_protocolIEs_3(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_ue_context_mod_response->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
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
 * Function Name  : rrc_s1ap_ue_context_modification_failure_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_ue_context_mod_response
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_ue_context_modification_failure into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_context_modification_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_ue_context_modification_failure_t *p_ue_context_mod_response
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UEContextModificationFailure *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_ue_context_mod_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_unsuccessfulOutcome;

        s1ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, UnsuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_UnsuccessfulOutcome(s1ap_pdu.u.unsuccessfulOutcome);

        s1ap_pdu.u.unsuccessfulOutcome->procedureCode =
            ASN1V_s1ap_id_UEContextModification;

        s1ap_pdu.u.unsuccessfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.unsuccessfulOutcome->value.t = T1_uEContextModification;
        s1ap_pdu.u.unsuccessfulOutcome->value.u.uEContextModification =
            rtxMemAllocType(pctxt, s1ap_UEContextModificationFailure);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome->value.u.uEContextModification)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.unsuccessfulOutcome->value.u.uEContextModification;

        asn1Init_s1ap_UEContextModificationFailure(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UEContextModificationFailure_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_mod_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UEContextModificationFailure_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_ue_context_mod_response->enb_ue_s1ap_id))
        {
            break;
        }
        if (RT_OK != asn1Append_s1ap_UEContextModificationFailure_protocolIEs_3(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_ue_context_mod_response->cause))
        {
            break;
        }

        if(RRC_S1AP_UE_CONTEXT_MODIFICATION_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT &
            p_ue_context_mod_response->bitmask)
        {
            if (RT_OK !=
                  asn1Append_s1ap_UEContextModificationFailure_protocolIEs_4(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_ue_context_mod_response->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}



/*Start E-RAB Modify*/

/****************************************************************************
 * Function Name  : rrc_s1ap_erb_modify_response_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                   p_erb_modify_response
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_initial_context_setup_response into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_erb_modify_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_modify_response_t *p_erb_modify_response
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_E_RABModifyResponse *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_erb_modify_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_E_RABModify;
        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_e_RABModify;
        s1ap_pdu.u.successfulOutcome->value.u.e_RABModify =
            rtxMemAllocType(pctxt, s1ap_E_RABModifyResponse);
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.e_RABModify)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.e_RABModify;

        asn1Init_s1ap_E_RABModifyResponse(p_s1ap_msg);
        
        if (RT_OK != asn1Append_s1ap_E_RABModifyResponse_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_erb_modify_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_E_RABModifyResponse_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_erb_modify_response->enb_ue_s1ap_id))
        {
            break;
        }
        
        if
   (RRC_S1AP_ERAB_MODIFY_RESPONSE_TO_BE_MODIFIED_LIST_PRESENT &
            p_erb_modify_response->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_E_RABModifyResponse_protocolIEs_3(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_erb_modify_response->e_rab_modified_list))
            {
                break;
            }
        }

        if
   (RRC_S1AP_ERAB_MODIFY_RESPONSE_E_RAB_FAILED_TO_MODIFY_LIST_PRESENT &
            p_erb_modify_response->bitmask)

        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABModifyResponse_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_erb_modify_response->e_rab_failed_to_modify_list))
            {
                break;
            }
        }

        if
        (RRC_S1AP_ERAB_MODIFY_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT
            &p_erb_modify_response->bitmask)

        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABModifyResponse_protocolIEs_5(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_erb_modify_response->criticality_diagnostics))
            {
                break;
            }
        }
			
#ifdef ENDC_ENABLED
		if(RRC_S1AP_ERAB_MODIFY_RESPONSE_SECONDARY_REPORT_LIST_PRESENT &
			p_erb_modify_response->bitmask)
		{
			OSRTDListNode               *p_node = PNULL;
		    s1ap_E_RABModifyResponse_protocolIEs_element *     p_protocolIE_elem = PNULL;
			OSRTDList  e_RABModifyResponseIEs_6;
			
			rtxDListAllocNodeAndData(pctxt, 
					s1ap_E_RABModifyResponse_protocolIEs_element, 
					&p_node, 
					&p_protocolIE_elem);
			if (PNULL == p_node)
			{
				/* Not enough memory */
				RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
				break;
			}
			asn1Init_s1ap_E_RABModifyResponse_protocolIEs_element (
					p_protocolIE_elem);
			
			/* Fill protocol IE Id and criticality */
			p_protocolIE_elem->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportList;
			p_protocolIE_elem->criticality = s1ap_ignore;
			
			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T37__E_RABModifyResponseIEs_6;
			
			rtxDListInit(&e_RABModifyResponseIEs_6);
			p_protocolIE_elem->value.u._E_RABModifyResponseIEs_6  = 
											   &e_RABModifyResponseIEs_6;			

	        if (RRC_SUCCESS !=   compose_s1ap_secondary_rat_data_usage_report_list(
	                    pctxt,
	                    p_protocolIE_elem->value.u._E_RABModifyResponseIEs_6,
	                    &p_erb_modify_response->secondary_rat_data_usage_report_list))
	        {
	            break;
	        }

			rtxDListAppendNode(&p_s1ap_msg->protocolIEs, p_node);
		}
#endif

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
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
 * Function Name  : rrc_s1ap_cell_traffic_trace_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_cell_traffic_trace
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_cell_traffic_trace into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_cell_traffic_trace_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_cell_traffic_trace_t* p_cell_traffic_trace
)
{
    s1ap_CellTrafficTrace* p_s1ap_msg = PNULL;
    rrc_return_et result = RRC_FAILURE;
    S1AP_PDU s1ap_pdu;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_cell_traffic_trace);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_CellTrafficTrace;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_cellTrafficTrace;
        s1ap_pdu.u.initiatingMessage->value.u.cellTrafficTrace =
            rtxMemAllocType(pctxt, s1ap_CellTrafficTrace);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.cellTrafficTrace)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.cellTrafficTrace;

        asn1Init_s1ap_CellTrafficTrace(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_CellTrafficTrace_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_cell_traffic_trace->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_CellTrafficTrace_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_cell_traffic_trace->enb_ue_s1ap_id))
        {
            break;
        }
        if (RT_OK != asn1Append_s1ap_CellTrafficTrace_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_cell_traffic_trace->e_utran_trace_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_CellTrafficTrace_protocolIEs_4(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_cell_traffic_trace->e_utran_cgi))
        {
            break;
        }
        if (RT_OK != asn1Append_s1ap_CellTrafficTrace_protocolIEs_5(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_cell_traffic_trace->trace_collection_entity))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        /*swati*/
        rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU", &s1ap_pdu);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* ERB RELEASE COMMAND START */
/****************************************************************************
 * Function Name  : rrc_s1ap_e_rab_release_response_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_e_rab_release_response
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_e_rab_rlease_response into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_e_rab_release_response_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_release_response_t* p_e_rab_release_response
)
{
    s1ap_E_RABReleaseResponse* p_s1ap_msg = PNULL;
    rrc_return_et result = RRC_FAILURE;
    

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_e_rab_release_response);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome(s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_s1ap_id_E_RABRelease;

        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        s1ap_pdu.u.successfulOutcome->value.t = T1_e_RABRelease;
        s1ap_pdu.u.successfulOutcome->value.u.e_RABRelease =
            rtxMemAllocType(pctxt, s1ap_E_RABReleaseResponse);
        if (PNULL == s1ap_pdu.u.successfulOutcome->value.u.e_RABRelease)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.successfulOutcome->value.u.e_RABRelease;

        asn1Init_s1ap_E_RABReleaseResponse(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_E_RABReleaseResponse_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_e_rab_release_response->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_E_RABReleaseResponse_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_e_rab_release_response->enb_ue_s1ap_id))
        {
            break;
        }

        if (RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_RELEASE_LIST_PRESENT &
            p_e_rab_release_response->bitmask)
        {
            if (RT_OK != 
                  asn1Append_s1ap_E_RABReleaseResponse_protocolIEs_3(
                  pctxt,
                  &p_s1ap_msg->protocolIEs,
                  &p_e_rab_release_response->e_rab_release_list))
            {
                break;
            }
        }

        if (
           RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_FAILED_TO_RELEASE_LIST_PRESENT
            & p_e_rab_release_response->bitmask)



        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABReleaseResponse_protocolIEs_4(
                pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_e_rab_release_response->e_rab_failed_to_release_list))
            {
                break;
            }
        }

        if
        (RRC_S1AP_ERAB_RELEASE_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT &
            p_e_rab_release_response->bitmask)
        {
            if (RT_OK !=
                asn1Append_s1ap_E_RABReleaseResponse_protocolIEs_5(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_e_rab_release_response->criticality_diagnostics))
            {
                break;
            }
        }
						
#ifdef ENDC_ENABLED
		if(RRC_S1AP_ERAB_RELEASE_RESPONSE_SECONDARY_REPORT_PRESENT &
			p_e_rab_release_response->bitmask)
		{
			OSRTDListNode				*p_node = PNULL;
			s1ap_E_RABReleaseResponse_protocolIEs_element *	   p_protocolIE_elem = PNULL;
			OSRTDList  e_RABReleaseResponseIEs_7;
			
			rtxDListAllocNodeAndData(pctxt, 
					s1ap_E_RABReleaseResponse_protocolIEs_element, 
					&p_node, 
					&p_protocolIE_elem);
			if (PNULL == p_node)
			{
				/* Not enough memory */
				RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
				break;
			}
			asn1Init_s1ap_E_RABReleaseResponse_protocolIEs_element (
					p_protocolIE_elem);
			
			/* Fill protocol IE Id and criticality */
			p_protocolIE_elem->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportList;
			p_protocolIE_elem->criticality = s1ap_ignore;
			
			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T41__E_RABReleaseResponseIEs_7;
			
			rtxDListInit(&e_RABReleaseResponseIEs_7);
			p_protocolIE_elem->value.u._E_RABReleaseResponseIEs_7  = 
											   &e_RABReleaseResponseIEs_7;			

			if (RRC_SUCCESS !=	 compose_s1ap_secondary_rat_data_usage_report_list(
						pctxt,
						p_protocolIE_elem->value.u._E_RABReleaseResponseIEs_7,
						&p_e_rab_release_response->secondary_rat_data_usage_report_list))
			{
				break;
			}

			rtxDListAppendNode(&p_s1ap_msg->protocolIEs, p_node);
		}
#endif

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }
        /* Printing ASN encoded msg */
        /*asn1Print_S1AP_PDU("S1AP-ERAB-REL-RESP", &s1ap_pdu);*/

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* ERB RELEASE INDICATION START */
/****************************************************************************
 * Function Name  : rrc_s1ap_e_rab_release_indication_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_e_rab_release_indication
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_e_rab_rlease_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_e_rab_release_indication_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT*      pctxt,         /* for memory allocation */
    U8*          p_buff,        /* ASN.1 encoded message */
    U16*         p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_e_rab_release_indication_t* p_e_rab_release_indication
)
{
    s1ap_E_RABReleaseIndication* p_s1ap_msg = PNULL;
    rrc_return_et result = RRC_FAILURE;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_e_rab_release_indication);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_E_RABReleaseIndication;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_e_RABReleaseIndication;
        s1ap_pdu.u.initiatingMessage->value.u.e_RABReleaseIndication =
            rtxMemAllocType(pctxt, s1ap_E_RABReleaseIndication);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.
            e_RABReleaseIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.
            e_RABReleaseIndication;

        asn1Init_s1ap_E_RABReleaseIndication(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_E_RABReleaseIndication_protocolIEs_1(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_e_rab_release_indication->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_E_RABReleaseIndication_protocolIEs_2(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            p_e_rab_release_indication->enb_ue_s1ap_id))
        {
            break;
        }


        if (RT_OK != asn1Append_s1ap_E_RABReleaseIndication_protocolIEs_3(
            pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_e_rab_release_indication->e_rab_released_list))
        {
            break;
        }
									
#ifdef ENDC_ENABLED
		if(RRC_S1AP_ERAB_RELEASE_INDICATION_SECONDARY_REPORT_PRESENT &
			p_e_rab_release_indication->bitmask)
		{
			OSRTDListNode				*p_node = PNULL;
			s1ap_E_RABReleaseIndication_protocolIEs_element *    p_protocolIE_elem = PNULL;
			OSRTDList  e_RABReleaseIndicationIEs_5;
			
			rtxDListAllocNodeAndData(pctxt, 
					s1ap_E_RABReleaseIndication_protocolIEs_element, 
					&p_node, 
					&p_protocolIE_elem);
			if (PNULL == p_node)
			{
				/* Not enough memory */
				RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
				break;
			}
			asn1Init_s1ap_E_RABReleaseIndication_protocolIEs_element (
					p_protocolIE_elem);
			
			/* Fill protocol IE Id and criticality */
			p_protocolIE_elem->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportList;
			p_protocolIE_elem->criticality = s1ap_ignore;
			
			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T44__E_RABReleaseIndicationIEs_5;
			
			rtxDListInit(&e_RABReleaseIndicationIEs_5);
			p_protocolIE_elem->value.u._E_RABReleaseIndicationIEs_5 = 
											   &e_RABReleaseIndicationIEs_5;			

			if (RRC_SUCCESS !=	 compose_s1ap_secondary_rat_data_usage_report_list(
						pctxt,
						p_protocolIE_elem->value.u._E_RABReleaseIndicationIEs_5,
						&p_e_rab_release_indication->secondary_rat_data_usage_report_list))
			{
				break;
			}

			rtxDListAppendNode(&p_s1ap_msg->protocolIEs, p_node);
		}
#endif


        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}
/* ERB RELEASE INDICATION END */

/****************************************************************************
 * Function Name  : rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_uecc_gb_context
 *                  p_source_to_target_transparent_container
 *                  p_container
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs  ranap_SourceRNC_ToTargetRNC_TransparentContainer
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT                                    *pctxt,   /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer *p_source_to_target_transparent_container,
    ranap_SourceRNC_ToTargetRNC_TransparentContainer *p_container /* source */
)
{
    OSCTXT          asn1_ctx;
    rrc_return_et   result = RRC_FAILURE;
    U8              msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    OSOCTET         *p_data = PNULL;

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_source_to_target_transparent_container);
    RRC_ASSERT(PNULL != p_container);
    
    do 
    {
        /* ASN Encode message */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc]"
                "ASN1 context initialization failed.");
            break;
        }
        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        if (RT_OK != 
        asn1PE_ranap_SourceRNC_ToTargetRNC_TransparentContainer(&asn1_ctx, p_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc]"
                "ASN1 encoding failed.");
            break;
        }
        p_source_to_target_transparent_container->numocts = pe_GetMsgLen(&asn1_ctx);
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_INFO,
                "[rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc]"
                "ranap_SourceRNC_ToTargetRNC_TransparentContainer length %d.",
             p_source_to_target_transparent_container->numocts);
        p_data = (U8 *)rtxMemAlloc(pctxt, p_source_to_target_transparent_container->numocts);
        if (PNULL == p_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_FATAL,
                "[rrc_s1ap_source_rnc_to_targer_rnc_container_intrl_enc]"
                "Memory allocation failed.");
            break;        
        }
        l3_memcpy_wrapper(p_data, msg_buf, p_source_to_target_transparent_container->numocts);
        p_source_to_target_transparent_container->data = p_data;
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_old_bss_to_new_bss_information_intrl_enc 
 * Inputs         : pctxt (for memory allocation),
 *                  p_uecc_gb_context
 *                  p_source_to_target_transparent_container
 *                  container
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs s1ap_OldBSS_ToNewBSS_Information 
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_old_bss_to_new_bss_information_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
     s1ap_OldBSS_ToNewBSS_Information container
)
{
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_FAILURE;
    U8 msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    OSOCTET *p_data = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_source_to_target_transparent_container);

    do 
    {
        /* ASN Encode message */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,RRC_ERROR,
                    "[rrc_s1ap_old_bss_to_new_bss_information_intrl_enc] "
                    "ASN1 context initialization failed.");
            return RRC_FAILURE;
        }

        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
 asn1PE_s1ap_OldBSS_ToNewBSS_Information(&asn1_ctx, container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }
       
        p_source_to_target_transparent_container->numocts = pe_GetMsgLen(&asn1_ctx);

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
             p_uecc_gb_context->facility_name,RRC_INFO,
             "[rrc_s1ap_old_bss_to_new_bss_information_intrl_enc]Container length %d.",
             p_source_to_target_transparent_container->numocts);

        p_data = (U8 *)rtxMemAlloc(pctxt, p_source_to_target_transparent_container->numocts);

        if (PNULL == p_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,RRC_FATAL,
               "[rrc_s1ap_old_bss_to_new_bss_information_intrl_enc]Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_data, msg_buf, p_source_to_target_transparent_container->numocts);

     p_source_to_target_transparent_container->data = p_data;
     
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    
    RRC_UT_TRACE_EXIT();
    return result;
    
}

/****************************************************************************
 * Function Name  : rrc_s1ap_source_bss_to_target_bss_container_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt
 *                  p_source_to_target_transparent_container
 *                  container
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs s1ap_source_bss_to_targetbss_transparentcontainer 
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_source_bss_to_target_bss_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    s1ap_Source_ToTarget_TransparentContainer  *p_source_to_target_transparent_container,

    /* source */
     s1ap_SourceBSS_ToTargetBSS_TransparentContainer container
)
{
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_FAILURE;
    U8 msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    OSOCTET *p_data = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_source_to_target_transparent_container);

    do 
    {
        /* ASN Encode message */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,RRC_ERROR,
                    "[rrc_s1ap_source_bss_to_target_bss_container_intrl_enc] "
                    "ASN1 context initialization failed.");
            return RRC_FAILURE;
        }

        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
 asn1PE_s1ap_SourceBSS_ToTargetBSS_TransparentContainer(&asn1_ctx, container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }
       
        /*SPR 19592 FIX START*/
        p_source_to_target_transparent_container->numocts = pe_GetMsgLen(&asn1_ctx) - 1;
        /*SPR 19592 FIX END*/

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
             p_uecc_gb_context->facility_name,RRC_INFO,
             "[rrc_s1ap_source_bss_to_target_bss_container_intrl_enc]Container length %d.",
             p_source_to_target_transparent_container->numocts);

        p_data = (U8 *)rtxMemAlloc(pctxt, p_source_to_target_transparent_container->numocts);

        if (PNULL == p_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,RRC_FATAL,
               "[rrc_s1ap_old_bss_to_new_bss_information_intrl_enc]Memory allocation failed.");
            break;        
        }

        /*SPR 19592 FIX START*/
	l3_memcpy_wrapper(p_data, msg_buf+1, p_source_to_target_transparent_container->numocts);
        /*SPR 19592 FIX END*/

     p_source_to_target_transparent_container->data = p_data;
     
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    
    RRC_UT_TRACE_EXIT();
    return result;
    
}

/****************************************************************************
* Function Name  : uecc_s1ap_location_report_intrl_enc
* Inputs         : p_uecc_gb_context
*                  p_buff  pointer to the mesage to be decoded
*                  p_size length of asn message
* Outputs        : p_report_msg encoded message
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : encodes the asn message and fills the structure of
*                  after reading from input buffer
****************************************************************************/
rrc_return_et uecc_s1ap_location_report_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    rrc_s1ap_location_report_t* p_report_msg,
    U8*     p_buff,
    U16*    p_size
)
{
    OSCTXT      asn1_ctx;
    S1AP_PDU    s1ap_pdu;
    OSRTDListNode   *p_node = PNULL;
    s1ap_LocationReport_protocolIEs_element* p_protocolIE_elem = PNULL;
    s1ap_LocationReport* p_location_report = PNULL;
    
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_report_msg);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[s1ap_HandoverFailure] "
                "ASN context initialization failed");
        
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage ;
        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                                                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);
        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_LocationReport;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;
        s1ap_pdu.u.initiatingMessage->value.t = T1_locationReport;
        s1ap_pdu.u.initiatingMessage->value.u.locationReport = 
                rtxMemAllocType(&asn1_ctx, s1ap_LocationReport);
        if (PNULL ==  s1ap_pdu.u.initiatingMessage->value.u.locationReport)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_location_report = s1ap_pdu.u.initiatingMessage->value.u.locationReport;
        
        asn1Init_s1ap_LocationReport(p_location_report);
         
        rtxDListInit(&p_location_report->protocolIEs);
          
        /*** Compose id: id-mme_ue_s1ap_id ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReport_protocolIEs_element(p_protocolIE_elem);
                
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_MME_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T94__LocationReportIEs_1;
        p_protocolIE_elem->value.u._LocationReportIEs_1 = p_report_msg->mme_ue_s1ap_id;
        rtxDListAppendNode(&p_location_report->protocolIEs, p_node);
                        
       /*** Compose id: id-enb_ue_s1ap_id ***/
                
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReport_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_eNB_UE_S1AP_ID;
            p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T94__LocationReportIEs_2;
        p_protocolIE_elem->value.u._LocationReportIEs_2 = p_report_msg->enb_ue_s1ap_id;
        rtxDListAppendNode(&p_location_report->protocolIEs, p_node);
               
        /*** Compose id: id-eutran_cgi ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReport_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_EUTRAN_CGI;
            p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T94__LocationReportIEs_3;
        p_protocolIE_elem->value.u._LocationReportIEs_3 = &(p_report_msg->eutran_cgi);
        rtxDListAppendNode(&p_location_report->protocolIEs, p_node);
               
        /*** Compose id: id-tai ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "%s:ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReport_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_TAI;
            p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T94__LocationReportIEs_4;
        p_protocolIE_elem->value.u._LocationReportIEs_4 =&(p_report_msg->tai);
        rtxDListAppendNode(&p_location_report->protocolIEs, p_node);
                
        /*** Compose id: id-request_type***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReport_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_RequestType;
        p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T94__LocationReportIEs_5;
        p_protocolIE_elem->value.u._LocationReportIEs_5 =&(p_report_msg->request_type);
        rtxDListAppendNode(&p_location_report->protocolIEs, p_node);
        
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx,(OSOCTET *)p_buff,
                    S1AP_MAX_ASN1_BUF_LEN, TRUE);
        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN1 encoding of Location Report"
                    " failed.");
            break;
        }
        else
        {
            *p_size= (U16)pe_GetMsgLen(&asn1_ctx);
        }
    }while(0);
    
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    
    return RRC_SUCCESS;
}

/****************************************************************************
* Function Name  : uecc_s1ap_ue_associated_location_report_failure_intrl_enc 
* Inputs         : p_uecc_gb_context
*                  p_buff  pointer to the mesage to be decoded
*                  p_size length of asn message
* Outputs        : p_failure_msg encoded message
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : encodes the asn message and fills the structure of
*                  after reading from input buffer
****************************************************************************/
rrc_return_et uecc_s1ap_ue_associated_location_report_failure_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    rrc_s1ap_failure_report_t*      p_failure_msg,
    U8*                             p_buff,
    U16*                            p_size
 )
{
    OSCTXT      asn1_ctx;
    S1AP_PDU    s1ap_pdu;
    s1ap_LocationReportingFailureIndication* p_failure_report =PNULL ;
    OSRTDListNode   *p_node = PNULL;
    s1ap_LocationReportingFailureIndication_protocolIEs_element* 
        p_protocolIE_elem = PNULL;
    
    RRC_UT_TRACE_ENTER();
    
    RRC_ASSERT(PNULL != p_failure_msg);
    
    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[s1ap_HandoverFailure] "
                "ASN context initialization failed");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage ;
        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                                        InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);
        
         
        s1ap_pdu.u.initiatingMessage->procedureCode = 
            ASN1V_s1ap_id_LocationReportingFailureIndication;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore ;
        s1ap_pdu.u.initiatingMessage->value.t = 
            T1_locationReportingFailureIndication;
        s1ap_pdu.u.initiatingMessage->value.u.
            locationReportingFailureIndication = 
            rtxMemAllocType(&asn1_ctx, s1ap_LocationReportingFailureIndication);
        
        if (PNULL == s1ap_pdu.u.initiatingMessage->
                value.u.locationReportingFailureIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        p_failure_report = s1ap_pdu.u.initiatingMessage->
            value.u.locationReportingFailureIndication ;
        asn1Init_s1ap_LocationReportingFailureIndication(p_failure_report);
        rtxDListInit(&p_failure_report->protocolIEs);
        
        /*** Compose id: id-mme_ue_s1ap_id ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReportingFailureIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReportingFailureIndication_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        
        p_protocolIE_elem->id =  ASN1V_s1ap_id_MME_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T93__LocationReportingFailureIndicationIEs_1;
        p_protocolIE_elem->value.u._LocationReportingFailureIndicationIEs_1 = 
            p_failure_msg->mme_ue_s1ap_id;
        rtxDListAppendNode(&p_failure_report->protocolIEs, p_node);
                
        /*** Ce id: id-enb_ue_s1ap_id ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReportingFailureIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReportingFailureIndication_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_eNB_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T93__LocationReportingFailureIndicationIEs_2;
        p_protocolIE_elem->value.u._LocationReportingFailureIndicationIEs_2 = 
            p_failure_msg->enb_ue_s1ap_id;
        rtxDListAppendNode(&p_failure_report->protocolIEs, p_node);

        /*** Compose id: cause ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_LocationReportingFailureIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_LocationReportingFailureIndication_protocolIEs_element(p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id =  ASN1V_s1ap_id_Cause;
        p_protocolIE_elem->criticality = s1ap_ignore;
        p_protocolIE_elem->value.t = T93__LocationReportingFailureIndicationIEs_3;
        p_protocolIE_elem->value.u._LocationReportingFailureIndicationIEs_3 = 
            &(p_failure_msg->cause);
        rtxDListAppendNode(&p_failure_report->protocolIEs, p_node);
            
       /* ASN Encode message */
        pu_setBuffer(&asn1_ctx,(OSOCTET *)p_buff, S1AP_MAX_ASN1_BUF_LEN, TRUE);
        
        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,"ASN1 encoding of Location Report Failure msg"
                    "failed.");
            break;
        }
        else
        {
            *p_size= (U16)pe_GetMsgLen(&asn1_ctx);
        }
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/****************************************************************************
* Function Name  : rrc_s1ap_uplink_s1_cdma2000_tunnelling_intrl_enc 
* Inputs         : p_uecc_gb_context
*                  pctxt
*                  p_buff  pointer to the mesage to be decoded
*                  p_size length of asn message
* Outputs        : p_uplink_s1_cdma2000_tunnelling encoded message
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : encodes the asn message and fills the structure of
*                  after reading from input buffer
****************************************************************************/
rrc_return_et rrc_s1ap_uplink_s1_cdma2000_tunneling_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */ 
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_uplink_s1_cdma2000_tunneling_t *p_uplink_s1_cdma2000_tunnelling /* source */
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_UplinkS1cdma2000tunnelling *p_s1ap_msg = PNULL;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_uplink_s1_cdma2000_tunnelling);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_UplinkS1cdma2000tunnelling;

        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        s1ap_pdu.u.initiatingMessage->value.t = T1_uplinkS1cdma2000tunnelling ;
        s1ap_pdu.u.initiatingMessage->value.u.uplinkS1cdma2000tunnelling =
            rtxMemAllocType(pctxt, s1ap_UplinkS1cdma2000tunnelling);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.uplinkS1cdma2000tunnelling)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.uplinkS1cdma2000tunnelling;

        asn1Init_s1ap_UplinkS1cdma2000tunnelling(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_1(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_uplink_s1_cdma2000_tunnelling->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_2(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_uplink_s1_cdma2000_tunnelling->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_3(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_uplink_s1_cdma2000_tunnelling->cdma2000_rat_type))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_4(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_uplink_s1_cdma2000_tunnelling->cdma2000_sector_id))
        {
            break;
        }

	if( RRC_S1AP_UL_CDMA2000_TUNNELING_HO_REQUIRED_IND_PRESENT &
		p_uplink_s1_cdma2000_tunnelling->bitmask )
	{
	     if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_5(pctxt,
        	&p_s1ap_msg->protocolIEs,
           	p_uplink_s1_cdma2000_tunnelling->cdma2000_ho_required_ind))
        	{
           	    break;
        	}
       }
	if( RRC_S1AP_UL_CDMA2000_TUNNELING_1X_SRVCC_INFO_PRESENT &
		p_uplink_s1_cdma2000_tunnelling->bitmask )
	{
	     if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_6(pctxt,
        	&p_s1ap_msg->protocolIEs,
           	&p_uplink_s1_cdma2000_tunnelling->cdma2000_1x_srvcc))
        	{
                    break;
        	}
	}

	if( RRC_S1AP_UL_CDMA2000_TUNNELING_1X_RAND_PRESENT &
		p_uplink_s1_cdma2000_tunnelling->bitmask )
	{
	     if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_7(pctxt,
        	&p_s1ap_msg->protocolIEs,
           	&p_uplink_s1_cdma2000_tunnelling->cdma2000_1x_rand))
        	{
                    break;
        	}
	}

	 if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_8(pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_uplink_s1_cdma2000_tunnelling->cdma2000_pdu))
        {
                    break;
        }
	
	if( RRC_S1AP_UL_CDMA2000_TUNNELING_ROUND_TRIP_DELAY_ESTIMATION_INFO_PRESENT &
		p_uplink_s1_cdma2000_tunnelling->bitmask )
	{
	     if (RT_OK != asn1Append_s1ap_UplinkS1cdma2000tunnelling_protocolIEs_9(pctxt,
        	&p_s1ap_msg->protocolIEs,
           	p_uplink_s1_cdma2000_tunnelling->round_trip_delay_estimation_info))
        	{
                    break;
        	}
	}
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* CDMA + CSFB Changes end: */
/*x2ap-start*/
/****************************************************************************
 * Function Name  : rrc_s1ap_path_switch_request_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_path_switch_request
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_path_switch_request into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_path_switch_request_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    rrc_s1ap_path_switch_request_t   *p_path_switch_request   /* source */
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_PathSwitchRequest *p_s1ap_msg;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_path_switch_request);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_PathSwitchRequest;

        /*SPR_18844_START*/
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;
        /*SPR_18844_END*/

        s1ap_pdu.u.initiatingMessage->value.t = T1_pathSwitchRequest;
        s1ap_pdu.u.initiatingMessage->value.u.pathSwitchRequest =
            rtxMemAllocType(pctxt, s1ap_PathSwitchRequest);
        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.pathSwitchRequest)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.pathSwitchRequest;

        asn1Init_s1ap_PathSwitchRequest(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_1(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_path_switch_request->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_2(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_path_switch_request->erab_to_be_switched_dl_list))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_3(pctxt,
            &p_s1ap_msg->protocolIEs,
            p_path_switch_request->mme_ue_s1ap_id))
        {
            break;
        }
        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_4(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_path_switch_request->eutran_cgi))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_5(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_path_switch_request->tai))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_6(pctxt,
            &p_s1ap_msg->protocolIEs,
            &p_path_switch_request->ue_security_capabilities))
        {
            break;
        }
        
        if(RRC_S1AP_PATH_SWITCH_REQUEST_CSG_ID_PRESENT & p_path_switch_request->bitmask)
	{
            if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_7(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_path_switch_request->csg_id))
            {
                break;
            }
	}

        if(RRC_S1AP_PATH_SWITCH_REQUEST_CELL_ACCESS_MODE_PRESENT & p_path_switch_request->bitmask)
	{
            if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_8(pctxt,
                &p_s1ap_msg->protocolIEs,
                p_path_switch_request->cell_access_mode))
            {
                break;
            }
	}

        if(RRC_S1AP_PATH_SWITCH_REQUEST_SOURCE_GUMMEI_PRESENT & p_path_switch_request->bitmask)
		{
            if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_9(pctxt,
                &p_s1ap_msg->protocolIEs,
                &p_path_switch_request->gummei))
            {
                break;
            }
		}

		if(RRC_S1AP_PATH_SWITCH_REQUEST_NR_SECURITY_CAP_PRESENT & p_path_switch_request->bitmask)
		{
			s1ap_NRUESecurityCapabilities* pElem = 
      			rtxMemAllocType (pctxt, s1ap_NRUESecurityCapabilities);
			asn1Init_s1ap_NRUESecurityCapabilities(pElem);
			if(p_path_switch_request->nr_security_cap.ue_NR_Encryption_Algo
				& UECC_CIPHERING_NEA1_BIT)
			{
				rtxSetBit(pElem->nRencryptionAlgorithms.data,
						pElem->nRencryptionAlgorithms.numbits,
						0);
			}

			if(p_path_switch_request->nr_security_cap.ue_NR_Encryption_Algo
				& UECC_CIPHERING_NEA2_BIT)
			{
				rtxSetBit(pElem->nRencryptionAlgorithms.data,
						pElem->nRencryptionAlgorithms.numbits,
						1);
			}

			if(p_path_switch_request->nr_security_cap.ue_NR_Encryption_Algo
				& UECC_CIPHERING_NEA3_BIT)
			{
				rtxSetBit(pElem->nRencryptionAlgorithms.data,
						pElem->nRencryptionAlgorithms.numbits,
						2);
			}

			if(p_path_switch_request->nr_security_cap.ue_NR_Integrity_Protection_Algo
				& UECC_INTEGRITY_PROTECTION_NIA1_BIT)
			{
				rtxSetBit(pElem->nRintegrityProtectionAlgorithms.data,
						pElem->nRintegrityProtectionAlgorithms.numbits,
						0);
			}

			if(p_path_switch_request->nr_security_cap.ue_NR_Integrity_Protection_Algo
				& UECC_INTEGRITY_PROTECTION_NIA2_BIT)
			{
				rtxSetBit(pElem->nRintegrityProtectionAlgorithms.data,
						pElem->nRintegrityProtectionAlgorithms.numbits,
						1);
			}

			if(p_path_switch_request->nr_security_cap.ue_NR_Integrity_Protection_Algo
				& UECC_INTEGRITY_PROTECTION_NIA3_BIT)
			{
				rtxSetBit(pElem->nRintegrityProtectionAlgorithms.data,
						pElem->nRintegrityProtectionAlgorithms.numbits,
						2);
			}
				
			if (RT_OK != asn1Append_s1ap_PathSwitchRequest_protocolIEs_14(pctxt,
				&p_s1ap_msg->protocolIEs,
				pElem))
			{
				break;
			}
		}

		
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}
/*x2ap-end*/


/* CSR 101813 Fix Start */
/****************************************************************************
 * Function Name  : rrc_ranap_target_RNC_to_source_RNC_container_intrl_enc 
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                     p_enb_container
 * Outputs        : p_container contains the ASN.1 encoded message and number of octets
 *
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs ranap_TargetRNC_ToSourceRNC_TransparentContainer 
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_ranap_target_RNC_to_source_RNC_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    ranap_TargetRNC_ToSourceRNC_TransparentContainer *p_rnc_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
)
{
    rrc_return_et result = RRC_FAILURE;
    U8 msg[S1AP_MAX_ASN1_BUF_LEN];
    OSCTXT asn1_ctx;
    U8 *p_cont_data = PNULL;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_rnc_container);
    RRC_ASSERT(PNULL != p_container);

        /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[ s1ap_ENBStatusTransfer] ASN context initialization failed");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do 
    {
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, msg, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
 	    asn1PE_ranap_TargetRNC_ToSourceRNC_TransparentContainer(&asn1_ctx, p_rnc_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        p_container->numocts = pe_GetMsgLen(&asn1_ctx);

        /* p_container->data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts); */
        p_cont_data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts);

        if (PNULL == p_cont_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,RRC_FATAL,
               "[rrc_s1ap_target_enb_to_source_enb_container_intrl_enc] Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_cont_data, msg, p_container->numocts);

        p_container->data = p_cont_data;

        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_ranap_target_BSS_to_source_BSS_container_intrl_enc 
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                     p_enb_container
 * Outputs        : p_container contains the ASN.1 encoded message and number of octets
 *
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs ranap_TargetBSS_ToSourceBSS_TransparentContainer  
 *                  into s1ap_Source_ToTarget_TransparentContainer ASN
 ****************************************************************************/
rrc_return_et rrc_ranap_target_BSS_to_source_BSS_container_intrl_enc 
(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_Target_ToSource_TransparentContainer  *p_container,
    ranap_TargetBSS_ToSourceBSS_TransparentContainer *p_bss_container,     /* source */
    OSCTXT      *pctxt         /* for memory allocation */
)
{
    rrc_return_et result = RRC_FAILURE;
    U8 msg[S1AP_MAX_ASN1_BUF_LEN];
    OSCTXT asn1_ctx;
    U8 *p_cont_data = PNULL;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_bss_container);
    RRC_ASSERT(PNULL != p_container);

        /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "[ s1ap_ENBStatusTransfer] ASN context initialization failed");
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do 
    {
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, msg, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
 	    asn1PE_ranap_TargetBSS_ToSourceBSS_TransparentContainer(&asn1_ctx, *p_bss_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,RRC_ERROR,
                "ASN1 encoding failed.");
            break;
        }

        p_container->numocts = pe_GetMsgLen(&asn1_ctx);

        /* p_container->data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts); */
        p_cont_data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts);

        if (PNULL == p_cont_data)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
               p_uecc_gb_context->facility_name,RRC_FATAL,
               "[rrc_ranap_target_BSS_to_source_BSS_container_intrl_enc] Memory allocation failed.");
            break;        
        }

        l3_memcpy_wrapper(p_cont_data, msg, p_container->numocts);

        p_container->data = p_cont_data;

        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UT_TRACE_EXIT();
    return result;
}

/* CSR 101813 Fix Stop */
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : rrc_s1ap_erab_modification_ind_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_erab_modification_ind
 *                  p_uecc_gb_context
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_erab_modification_ind into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_erab_modification_ind_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context*/
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_erab_modification_ind_t *p_erab_modification_ind
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_E_RABModificationIndication  *p_s1ap_msg;
    
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_erab_modification_ind);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_E_RABModificationIndication;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t = T1_e_RABModificationIndication;


        s1ap_pdu.u.initiatingMessage->value.u.e_RABModificationIndication =
            rtxMemAllocType(pctxt, s1ap_E_RABModificationIndication);

        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.e_RABModificationIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = 
            s1ap_pdu.u.initiatingMessage->value.u.e_RABModificationIndication;

        asn1Init_s1ap_E_RABModificationIndication(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_E_RABModificationIndication_protocolIEs_1(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_erab_modification_ind->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_E_RABModificationIndication_protocolIEs_2(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_erab_modification_ind->enb_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK !=
                asn1Append_s1ap_E_RABModificationIndication_protocolIEs_3(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    &p_erab_modification_ind->erab_to_be_modified_list_ind))
        {
            break;
        }

        if(RRC_S1AP_ERAB_MODIFICATION_IND_ERAB_NOT_TO_BE_MODIFIED_LIST_IND_PRESENT &
                p_erab_modification_ind->bitmask)
        {
            if (RT_OK !=
                    asn1Append_s1ap_E_RABModificationIndication_protocolIEs_4(
                        pctxt,
                        &p_s1ap_msg->protocolIEs,
                        &p_erab_modification_ind->erab_not_to_be_modified_list_ind))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result = RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}


/*****************************************************************************
 * Function Name  : compose_E_RABUsageReportList
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_IE_E_RABUsageReportList - pointer to the OSRTDList
 *                  p_erab_usage_report_list - This contains the information to
 *                                        encode rrc_s1ap_erab_usage_report_list_t list.
 * Outputs        : p_IE_E_RABUsageReportList - This structure is populated with
 *                                        information from p_IE_E_RABUsageReportList.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_secondary_rat_data_usage_report_list with 
 *                  information necessary to encode s1ap_E_RABUsageReportList_element list.
 *****************************************************************************/

rrc_return_et compose_E_RABUsageReportList(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_IE_E_RABUsageReportList,
        rrc_s1ap_erab_usage_report_list_t * p_erab_usage_report_list)
{
    OSRTDListNode               *p_node = PNULL;
    s1ap_E_RABUsageReportList_element        *p_E_RABUsageReportList_element = PNULL;
    U16                              count = 0;


    RRC_UT_TRACE_ENTER();

	for(count = 0; count < p_erab_usage_report_list->count;count++)
	{
		s1ap_E_RABUsageReportItem e_RABUsageReportItemIEs_1;
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                s1ap_E_RABUsageReportList_element, 
                &p_node, 
                &p_E_RABUsageReportList_element);

        if (PNULL == p_node)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            RRC_UT_TRACE_EXIT();
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_s1ap_E_RABUsageReportList_element (p_E_RABUsageReportList_element);

		/* Fill protocol IE Id and criticality */
		p_E_RABUsageReportList_element->id = ASN1V_s1ap_id_E_RABUsageReportItem;
		p_E_RABUsageReportList_element->criticality = s1ap_ignore;
		
		/* Fill the type of Protocol IE element */
		p_E_RABUsageReportList_element->value.t = T182__E_RABUsageReportItemIEs_1;

		asn1Init_s1ap_E_RABUsageReportItem(&e_RABUsageReportItemIEs_1);
		p_E_RABUsageReportList_element->value.u._E_RABUsageReportItemIEs_1  = 
										   &e_RABUsageReportItemIEs_1;	
		
		e_RABUsageReportItemIEs_1.startTimestamp.numocts = 4;
		l3_memcpy_wrapper(e_RABUsageReportItemIEs_1.startTimestamp.data,
                p_erab_usage_report_list->erab_usage_report_item[count].start_time_stamp,
                4);
		e_RABUsageReportItemIEs_1.endTimestamp.numocts = 4;
		l3_memcpy_wrapper(e_RABUsageReportItemIEs_1.endTimestamp.data,
                p_erab_usage_report_list->erab_usage_report_item[count].end__time_stamp,
                4);		
		e_RABUsageReportItemIEs_1.usageCountUL = 
			p_erab_usage_report_list->erab_usage_report_item[count].ul_usage_count;
		e_RABUsageReportItemIEs_1.usageCountDL = 
			p_erab_usage_report_list->erab_usage_report_item[count].dl_usage_count;		
				
        rtxDListAppendNode(p_IE_E_RABUsageReportList, p_node);		
	}

    RRC_UT_TRACE_EXIT();

    return RRC_SUCCESS;


}


/*****************************************************************************
 * Function Name  : compose_s1ap_secondary_rat_data_usage_report_list
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_IE_secondary_rat_data_usage_report_list - pointer to the OSRTDList
 *                  p_secondary_rat_data_usage_report_list - This contains the information to
 *                                        encode secondary_rat_data_usage_report list.
 * Outputs        : p_IE_secondary_rat_data_usage_report_list - This structure is populated with
 *                                        information from p_secondary_rat_data_usage_report_list.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_secondary_rat_data_usage_report_list with 
 *                  information necessary to encode secondary_rat_data_usage_report list.
 *****************************************************************************/
rrc_return_et compose_s1ap_secondary_rat_data_usage_report_list(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_IE_secondary_rat_data_usage_report_list,
        rrc_s1ap_SecRATDataReport_list_t * p_secondary_rat_data_usage_report_list)
{
    OSRTDListNode               *p_node = PNULL;
    s1ap_SecondaryRATDataUsageReportList_element        *p_SecondaryRATDataUsageReportList_ele = PNULL;
    U16                              count = 0;
	s1ap_SecondaryRATDataUsageReportItem  *p_secondaryRATDataUsageReportItemIEs_1;


    RRC_UT_TRACE_ENTER();

    for (count = 0 ; count < p_secondary_rat_data_usage_report_list->count;count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                s1ap_SecondaryRATDataUsageReportList_element, 
                &p_node, 
                &p_SecondaryRATDataUsageReportList_ele);

        if (PNULL == p_node)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            RRC_UT_TRACE_EXIT();
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_s1ap_SecondaryRATDataUsageReportList_element (p_SecondaryRATDataUsageReportList_ele);

		/* Fill protocol IE Id and criticality */
		p_SecondaryRATDataUsageReportList_ele->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportItem;
		p_SecondaryRATDataUsageReportList_ele->criticality = s1ap_ignore;
		
		/* Fill the type of Protocol IE element */
		p_SecondaryRATDataUsageReportList_ele->value.t = T229__SecondaryRATDataUsageReportItemIEs_1;


		p_secondaryRATDataUsageReportItemIEs_1 = 
			rtxMemAllocType(p_asn1_ctx, s1ap_SecondaryRATDataUsageReportItem);		
		
		if (PNULL == p_secondaryRATDataUsageReportItemIEs_1)
		{
			RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
			RRC_UT_TRACE_EXIT();
			/* Not enough memory */
			return RRC_FAILURE;
		}
		asn1Init_s1ap_SecondaryRATDataUsageReportItem (p_secondaryRATDataUsageReportItemIEs_1);
		p_secondaryRATDataUsageReportItemIEs_1->e_RAB_ID = 
						p_secondary_rat_data_usage_report_list->rrc_s1ap_SecondaryRATDataUsageReport_ele[count].erab_id;
		p_secondaryRATDataUsageReportItemIEs_1->secondaryRATType =
						p_secondary_rat_data_usage_report_list->rrc_s1ap_SecondaryRATDataUsageReport_ele[count].secondary_rat_type;
		
		p_SecondaryRATDataUsageReportList_ele->value.u._SecondaryRATDataUsageReportItemIEs_1  = 
										   p_secondaryRATDataUsageReportItemIEs_1;	

		if (RRC_SUCCESS !=   compose_E_RABUsageReportList(
				   p_asn1_ctx,
				   &p_SecondaryRATDataUsageReportList_ele->value.u._SecondaryRATDataUsageReportItemIEs_1->e_RABUsageReportList,
				   &p_secondary_rat_data_usage_report_list->rrc_s1ap_SecondaryRATDataUsageReport_ele[count].erab_usage_report_list))
		{
		   
		    RRC_TRACE(RRC_ERROR, "compose_SecondaryRATDataUsageReportItem failed.");
            RRC_UT_TRACE_EXIT();
            /* Not enough memory */
            return RRC_FAILURE;
		}


        rtxDListAppendNode(p_IE_secondary_rat_data_usage_report_list, p_node);
    }

    RRC_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}


/****************************************************************************
 * Function Name  : rrc_s1ap_secodary_rat_data_usage_report_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  secondary_rat_data_usage_rep
 *                  p_uecc_gb_context
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs secondary_rat_data_usage_rep into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_secodary_rat_data_usage_report_intrl_enc
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context*/
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_s1ap_secondary_rat_data_usage_rep_t *p_secondary_rat_data_usage_rep
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_SecondaryRATDataUsageReport  *p_s1ap_msg;
    
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_secondary_rat_data_usage_rep);

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_s1ap_id_SecondaryRATDataUsageReport;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t = T1_secondaryRATDataUsageReport;


        s1ap_pdu.u.initiatingMessage->value.u.secondaryRATDataUsageReport =
            rtxMemAllocType(pctxt, s1ap_SecondaryRATDataUsageReport);

        if (PNULL == s1ap_pdu.u.initiatingMessage->value.u.secondaryRATDataUsageReport)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = 
            s1ap_pdu.u.initiatingMessage->value.u.secondaryRATDataUsageReport;

        asn1Init_s1ap_SecondaryRATDataUsageReport(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_SecondaryRATDataUsageReport_protocolIEs_1(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_secondary_rat_data_usage_rep->mme_ue_s1ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_s1ap_SecondaryRATDataUsageReport_protocolIEs_2(
                    pctxt,
                    &p_s1ap_msg->protocolIEs,
                    p_secondary_rat_data_usage_rep->enb_ue_s1ap_id))
        {
            break;
        }
		/*encode s1ap_secondary_rat_data_usage_report_list start*/
        {
		    OSRTDListNode               *p_node = PNULL;
		    s1ap_SecondaryRATDataUsageReport_protocolIEs_element *     p_protocolIE_elem = PNULL;
			OSRTDList  secondaryRATDataUsageReportIEs_3;
			
			rtxDListAllocNodeAndData(pctxt, 
					s1ap_SecondaryRATDataUsageReport_protocolIEs_element, 
					&p_node, 
					&p_protocolIE_elem);
			if (PNULL == p_node)
			{
				/* Not enough memory */
				RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
				break;
			}
			asn1Init_s1ap_SecondaryRATDataUsageReport_protocolIEs_element (
					p_protocolIE_elem);
			
			/* Fill protocol IE Id and criticality */
			p_protocolIE_elem->id = ASN1V_s1ap_id_SecondaryRATDataUsageReportList;
			p_protocolIE_elem->criticality = s1ap_ignore;
			
			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T137__SecondaryRATDataUsageReportIEs_3;
			
			rtxDListInit(&secondaryRATDataUsageReportIEs_3);
			p_protocolIE_elem->value.u._SecondaryRATDataUsageReportIEs_3  = 
											   &secondaryRATDataUsageReportIEs_3;			

	        if (RRC_SUCCESS !=   compose_s1ap_secondary_rat_data_usage_report_list(
	                    pctxt,
	                    p_protocolIE_elem->value.u._SecondaryRATDataUsageReportIEs_3,
	                    &p_secondary_rat_data_usage_rep->secondary_rat_data_usage_report_list))
	        {
	            break;
	        }

			rtxDListAppendNode(&p_s1ap_msg->protocolIEs, p_node);
		}
		/*encode s1ap_secondary_rat_data_usage_report_list end*/
		
        if(RRC_S1AP_SECONDARY_RAT_DATA_USAGE_REP_HANDOVER_FLAG_PRESENT &
                p_secondary_rat_data_usage_rep->bitmask)
        {
            if (RT_OK !=
                    asn1Append_s1ap_SecondaryRATDataUsageReport_protocolIEs_4(
                        pctxt,
                        &p_s1ap_msg->protocolIEs,
                        &p_secondary_rat_data_usage_rep->handover_flag))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result = RRC_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();
    return result;
}

#endif
/* OPTION3X Changes End */

