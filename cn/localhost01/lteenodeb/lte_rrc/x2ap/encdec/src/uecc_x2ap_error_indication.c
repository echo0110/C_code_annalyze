/*******************************************************************************
*
*  FILE NAME   : uecc_x2ap_error_indication.c
*
*  DESCRIPTION : This file contains the wrapper function of ASN Decoders.
*
*  DATE 30-Aug-2012  NAME Sudhansu Sekhar Nanda  REFERENCE  36.423_960(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_asn_enc_dec_3gpp.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_common_utils.h"
#include "asn_dec_wrappers.h"
#include "asn_enc_wrappers.h"
#include "rrc_x2ap_uecc_intf.h"
#include "uecc_utils.h"
#include "rrc_logging.h"
#include "x2ap_uecc.h"

#define MAX_PLMN_ID_VAL        0x09
#define MAX_NEXT_HOP_CHAINING_COUNT 0x07
#define MAX_MME_UE_S1AP_ID 0xFFFFFFFF
#define MAX_TIME_UE_STAYEDINCELL 0xFFF
#define MAX_PDCP_SN 0xFFF
#define MAX_HFN 0xFFFFF
#define MAX_BITRATE 10000000000ull
#define EUTRAN_TRACE_ID_VAL_CHECK   3
#define RRC_X2AP_SET_MEMBER 1
#define MAX_SUB_PROF_ID 256
#define MAX_NUM_BITS 16
#define MAX_KEY_ENB_STAR_NUM_BITS 256

static U8 plmn_count;
static U8 plmn_count_1;
static U8 trace_count;
/*static U8 mme_grp_id_count;
static U8 mme_code_count;*/

/*****************************************************************************
* Private Function Prototypes
****************************************************************************/
void  uecc_x2ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        x2ap_CriticalityDiagnostics_IE_List *p_list,
        uecc_x2ap_error_ind_ie_list_t            *p_iE_list);


x2ap_return_et x2ap_verify_erab_id(x2ap_E_RAB_ID *p_e_RAB_ID, 
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_erab_qos_params(x2ap_E_RAB_Level_QoS_Parameters *p_qos,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_dl_forwarding(x2ap_DL_Forwarding *p_dl_forwarding,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_tnl_address(x2ap_TransportLayerAddress *p_tnl,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_count_value(x2ap_COUNTvalue *p_count_value, 
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_cause(x2ap_Cause *p_value,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

x2ap_return_et x2ap_verify_secondary_rat_type(x2ap_SecondaryRATUsageReport_Item_secondaryRATType *p_secondary_rat_type,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);  
x2ap_return_et x2ap_verify_erab_usage_report_list(OSRTDList *p_value,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);        
x2ap_return_et x2ap_check_plmn_id_validity(U8 index, U8 octateValue);
x2ap_return_et x2ap_check_mme_group_id_validity(U8 index, U8 octateValue);
x2ap_return_et x2ap_check_mme_code_validity(U8 index, U8 octateValue);



/****************************************************************************
* Function Name  : uecc_x2ap_build_and_send_err_indication
 * Inputs         : peer_enodeb_id - Peer EnodeB ID
 *                  p_error_indication - pointer to rrc_x2ap_error_indication_t
 * Outputs        : None
* Returns        : X2AP_SUCCESS / X2AP_FAILURE
* Description    : packs p_error_indication into x2ap_X2AP_PDU ASN
****************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_err_indication
(
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 rrc_x2ap_error_indication_t *p_error_indication
)
{
    OSCTXT asn1_ctx;
    U8 asn_buff[X2AP_MAX_ASN1_BUF_LEN];
    x2ap_return_et result = X2AP_FAILURE;
    U8 p_message[sizeof(x2ap_ue_associated_sig_msg_req_t) +
        X2AP_MAX_ASN1_BUF_LEN] = {0};
    LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT  *p_event_x2_err_ind = PNULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    x2ap_ue_associated_sig_msg_req_t* p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)p_message;

    X2AP_UT_TRACE_ENTER();

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_ErrorIndication] ASN context initialization failed");
        return X2AP_FAILURE;
    }

    X2AP_MEMSET(asn_buff, 0, X2AP_MAX_ASN1_BUF_LEN);
    X2AP_MEMSET(p_sig_msg_req, RRC_NULL, sizeof(x2ap_ue_associated_sig_msg_req_t));

    p_sig_msg_req->peer_enodeb_id = peer_enodeb_id;

    do
    {
        /* Its a Transfer Syntax Error */
        if (PNULL == p_error_indication)
        {
            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_errorIndication;

            X2AP_TRACE(X2AP_INFO, "Encoding Error_Indication");
            result = rrc_x2ap_error_indication_tse_intrl_enc(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size);

            if (X2AP_FAILURE == result)
            {
                X2AP_TRACE(X2AP_WARNING, "Unable to Encode X2AP Error_Indication");
            }
            else
            {
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)p_sig_msg_req->message_buff_size);

                if (PNULL != p_event_header)
                {
                    x2ap_fill_protocol_event_params(p_event_header, 
                            (U32)p_sig_msg_req->message_buff_size,
                            LOCAL_X2_ERROR_INDICATION, 
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            (U32)p_sig_msg_req->message_buff_size);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            break;
        }

        p_sig_msg_req->procedure_code = ASN1V_x2ap_id_errorIndication;

        /* Its a Abstract Syntax Error */
        //X2AP_TRACE(X2AP_INFO, "Encoding Abstract Syntax Error");

        result = rrc_x2ap_error_indication_ase_intrl_enc(&asn1_ctx,
                &p_sig_msg_req->message_buff_p[0],
                &p_sig_msg_req->message_buff_size,
                p_error_indication);
        if (result == X2AP_SUCCESS)
        {
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)p_sig_msg_req->message_buff_size);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, 
                        (U32)p_sig_msg_req->message_buff_size,
                        LOCAL_X2_ERROR_INDICATION, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)&p_sig_msg_req->message_buff_p[0],
                        (U32)p_sig_msg_req->message_buff_size);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    } while (0);

    if (X2AP_SUCCESS == result)
    {
        if(!( uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                      p_sig_msg_req->message_buff_size),
                p_message)))
        {
             result = X2AP_FAILURE;
             X2AP_TRACE(X2AP_ERROR,
                    "failure sending message to X2AP Module.");
        }
        else
        {
            p_event_x2_err_ind = rrc_mem_get(sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
            if (PNULL != p_event_x2_err_ind)
            {
                memset_wrapper(p_event_x2_err_ind, RRC_NULL, 
                        sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
                p_event_x2_err_ind->header.length = sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT);
                p_event_x2_err_ind->header.event_id = LOCAL_EVENT_X2_ERROR_INDICATION;

                p_event_x2_err_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;

                if ((PNULL != p_error_indication) && 
                        (p_error_indication->bitmask & RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT)) 
                {
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = 
                        p_error_indication->cause.t;

            /* CSR 51448 START */
                    switch(p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_x2ap_Cause_radioNetwork:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.radioNetwork;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t, p_error_indication->cause.u.radioNetwork);
                            break;

                        case T_x2ap_Cause_transport:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_error_indication->cause.u.transport;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t, p_error_indication->cause.u.transport);
                            break;
                        case T_x2ap_Cause_protocol:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_error_indication->cause.u.protocol;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t, p_error_indication->cause.u.protocol);
                            break;
                        case T_x2ap_Cause_misc:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_error_indication->cause.u.misc;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t, p_error_indication->cause.u.misc);
                            break;
                    }
            /* CSR 51448 STOP */
                }
                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_err_ind);
            }
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "Unable to encode the X2AP ERROR Indication"
                " message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
* Function Name  : uecc_x2ap_parse_message_map 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  peer_enodeb_id - PeeR eNB id
 *                  p_msg_map - pointer to uecc_x2ap_message_data_t
 *                  p_ie_list - pointer to uecc_x2ap_error_ind_ie_list_t
 *                  p_index_to_update - pointer to index to update 
 *                  p_send_error_indication - pointer to uecc_x2ap_error_ind_bool_t
 *                  proc_code - procedure code
 *                  triggering_msg - msg to triggered
 *                  p_error_indication - pointer to rrc_x2ap_error_indication_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : Thsi function parses message map of msg coming from UECC
****************************************************************************/
x2ap_return_et uecc_x2ap_parse_message_map(
        OSCTXT *p_asn1_ctx,
        U8 peer_enodeb_id,
        uecc_x2ap_message_data_t *p_msg_map,
        uecc_x2ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        uecc_x2ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_x2ap_error_indication_t *p_error_indication)
{
    U8 count = 0;
    rrc_x2ap_error_indication_t error_indication;
    x2ap_return_et result = X2AP_FAILURE;
    x2ap_bool_et send_cd_in_resp = X2AP_FALSE;
    x2ap_bool_et error_found = X2AP_FALSE;
    x2ap_bool_et bypass_ignore_check = X2AP_FALSE;
    x2ap_Cause     cause_temp;

    X2AP_ASSERT(PNULL != p_msg_map);
    X2AP_ASSERT(PNULL != p_ie_list);

    /* GDB Fix - compiler warning - "may be used uninitialized" - difficult to say if it matters. Just
                 zeroing out so it fails consistently */
    X2AP_MEMSET( &cause_temp, 0, sizeof(cause_temp) );


    cause_temp.t = X2AP_NULL;
    do 
    {
        /* If its a response message terminate the procedure 
         * and there is no Notify IE Added to the list
         * trigger local Error Handling */
        if (X2AP_FALSE == p_send_error_indication->send_err_indication_notify_ie_present) 
        {
            /* Find Errors in Successful and unsuccessful response messages*/
            if (T_x2ap_X2AP_PDU_initiatingMessage != triggering_msg)
            {
                X2AP_TRACE(X2AP_DETAILED, "%s: [EI] Parsing Response Message", __FUNCTION__);
                /* This is reject case for Unknow IE in 
                 * resonse we have to termiate the procedure */
                if (X2AP_TRUE == p_send_error_indication->send_err_indication_reject_ie_present)
                {
                    X2AP_TRACE(X2AP_WARNING, "%s: [EI] Unknow IE with Reject Criticality Detected "
                            "in Response MSG, Terminate the procedure and do the local"
                            " Error Handling", __FUNCTION__);
                    return X2AP_SUCCESS;
                }

                X2AP_TRACE(X2AP_DETAILED, "%s:[EI] Parsing the Known IE in the Response Message", __FUNCTION__);
                for (count = 0; count < p_msg_map->max_count; count++)
                {
                    /* We are checking for Ignore IE Errors*/
                    if (p_msg_map->msg_map[count].criticality == x2ap_ignore)
                    {
                        /* If there are too many occurances of Ignore IE,
                         * We have to terminate the procedure, so return Success */
                        if ((p_msg_map->msg_map[count].wrong_order >= X2AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].occurances > X2AP_IE_PRESENT))
                        {
                            X2AP_TRACE(X2AP_WARNING, "%s: [EI] IE ID [%d] "
                                    "Wrong Order or Too many occurance Detected"
                                    " for Ignore Criticality IE in Response MSG,"
                                    "Terminate the procedure and do the local"
                                    " Error Handling", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return X2AP_SUCCESS;
                        }
                    } /* This is the check for Know Reject IE with Invalid value */
                    else if (p_msg_map->msg_map[count].criticality == x2ap_reject)
                    {
                        if ((p_msg_map->msg_map[count].occurances > X2AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].invalid_value_present >= X2AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].wrong_order >= X2AP_IE_PRESENT) ||
                                ((p_msg_map->msg_map[count].occurances == X2AP_NULL) && 
                                 (p_msg_map->msg_map[count].presence == x2ap_mandatory)) ||
                                (p_msg_map->msg_map[count].data_missing >= X2AP_IE_PRESENT))
                        {
                            X2AP_TRACE(X2AP_WARNING, "%s: [EI] IE ID [%d] "
                                    "Detected Error Indication for Reject"
                                    " Criticatlity IE", __FUNCTION__,
                                    p_msg_map->msg_map[count].ie_id);
                            return X2AP_SUCCESS;
                        }
                    }
                    else /* This is for the notify, where know IE we will send EI */
                    {
                        X2AP_TRACE(X2AP_WARNING, "%s: [EI] IE ID [%d] "
                                "Detected Error Indication for Notify"
                                " Criticatlity IE, Error Indication should be sent", 
                                __FUNCTION__, p_msg_map->msg_map[count].ie_id);
                        break;
                    }
                }

                X2AP_TRACE(X2AP_DETAILED, "%s:[EI] Response message parse Success", 
                        __FUNCTION__);
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_DETAILED, "%s:[EI] X2AP Initiating Message", __FUNCTION__);
            }

        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"[EI] Unknow IE with Notify criticality detected,"
                    " Error Indication Should be Sent");
        }
    }while(0);

    if (T_x2ap_X2AP_PDU_initiatingMessage == triggering_msg)
    {
        X2AP_TRACE(X2AP_DETAILED,"[EI] Parsing Initiating Message");
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"[EI] Parsing Resposne Message with "
                "Notify IE Error present");
    }

    /* Update the list, for any error encountered */
    for (count = 0; count < p_msg_map->max_count; count++)
    {
        if ((p_msg_map->msg_map[count].occurances > X2AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].invalid_value_present >= X2AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].wrong_order >= X2AP_IE_PRESENT) ||
                ((p_msg_map->msg_map[count].occurances == X2AP_NULL) && 
                 (p_msg_map->msg_map[count].presence == x2ap_mandatory)) ||
                (p_msg_map->msg_map[count].data_missing >= X2AP_IE_PRESENT))
        {

            if (((p_msg_map->msg_map[count].occurances == X2AP_NULL) &&
                        (p_msg_map->msg_map[count].presence == x2ap_mandatory)) || 
                    (p_msg_map->msg_map[count].data_missing >= X2AP_IE_PRESENT))
            {
                /* Mandatory Missing or Data missing */
                X2AP_TRACE(X2AP_WARNING, "[EI] %d Mandatory EI/Data missing Detected",
                        p_msg_map->msg_map[count].ie_id);

                uecc_x2ap_add_to_err_ind_ie_list(p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        X2AP_TRUE,
                        X2AP_FALSE);
                
                if(x2ap_reject == p_msg_map->msg_map[count].criticality)
                {
                    X2AP_TRACE(X2AP_INFO,"[EI] uecc : x2ap_reject");
                    cause_temp.t = T_x2ap_Cause_protocol;
                    cause_temp.u.protocol = x2ap_abstract_syntax_error_reject;
                }
                else if (x2ap_notify == p_msg_map->msg_map[count].criticality)
                {
                    X2AP_TRACE(X2AP_INFO,"[EI] uecc : x2ap_ignore");
                    cause_temp.t = T_x2ap_Cause_protocol;
                    cause_temp.u.protocol =
                            x2ap_abstract_syntax_error_ignore_and_notify;
                }
            }
            else
            {
                if ((p_msg_map->msg_map[count].wrong_order >= X2AP_IE_PRESENT) ||
                        (p_msg_map->msg_map[count].occurances > X2AP_IE_PRESENT))
                {
                    /* We'll add the IE in the list even if the criticality 
                     * is ignore when there is wrong order or too many
                     * occurances */
                    X2AP_TRACE(X2AP_WARNING, 
                            "[EI] IE ID [%d] Wrong Order/Too many occurance Detected",
                            p_msg_map->msg_map[count].ie_id);
                    uecc_x2ap_add_to_err_ind_ie_list(p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        X2AP_FALSE,
                        X2AP_TRUE);

                    bypass_ignore_check = X2AP_TRUE;
                }
                else
                {
                    /* Invalid Value, Wrong Order, too many occurances */
                    X2AP_TRACE(X2AP_WARNING, "[EI] IE ID [%d] Wrong Value/Order/Too many occurance for "
                            "Reject/Notify Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);
                    uecc_x2ap_add_to_err_ind_ie_list(p_ie_list, 
                            p_msg_map->msg_map[count].criticality,
                            p_msg_map->msg_map[count].ie_id,
                            p_index_to_update,
                            p_send_error_indication,
                            X2AP_FALSE,
                            X2AP_FALSE);
                }
            }
        } 
    }

    /* If there are no error detected for the mandatory fields
     * We can send the CD in response message */
    for (count = 0; count < p_msg_map->max_count; count++)
    {
        if ((p_msg_map->msg_map[count].presence == x2ap_mandatory) &&
                (p_msg_map->msg_map[count].occurances == X2AP_IE_PRESENT) &&
                (p_msg_map->msg_map[count].data_missing == X2AP_NULL) &&
                (p_msg_map->msg_map[count].wrong_order == X2AP_NULL) &&
                ((p_msg_map->unsuccessful_outcome_present == X2AP_TRUE) || 
                 (p_msg_map->successful_outcome_present == X2AP_TRUE)))
        {
            error_found = X2AP_FALSE;
        }
        else
        {
            error_found = X2AP_TRUE;
            break;
        }
    }

    if (X2AP_FALSE == error_found)
        send_cd_in_resp = X2AP_TRUE;

    if(X2AP_TRUE == error_found)
    {
        /* Send the Error Indication in case if there is no Unsuccessful 
         * outcome present. If the message has Successful outcome and there
         * are notify IE with Error, We'll send the CD in ACK message */
        if (((p_msg_map->successful_outcome_present == X2AP_TRUE) && 
                    (X2AP_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)) ||
                (p_msg_map->unsuccessful_outcome_present == X2AP_TRUE))
        {
            /* We have to check if Info is sufficent */
            send_cd_in_resp = X2AP_TRUE;
        }
        else
        {
            send_cd_in_resp = X2AP_FALSE;
        }
    }

    do
    {
        if ((X2AP_TRUE == p_send_error_indication->send_err_indication_notify_ie_present) ||
                (X2AP_TRUE == p_send_error_indication->send_err_indication_reject_ie_present) || 
                (X2AP_TRUE == bypass_ignore_check))
        {
            /* Continue */
        }
        else
        {
            break;
        }

        /*---- The case below comply to where we have to send the -------- */
        /*---- Error Indication from here -------------------------------- */
        /* If p_send_error_indication is True that means the list is 
         * populated to be sent, we have to check if the message has an
         * Unsuccessful outcome */
        if (X2AP_FALSE == send_cd_in_resp)
        {
            X2AP_TRACE(X2AP_INFO, "[EI] Building and Sending Error Indication Message");

            X2AP_MEMSET(&error_indication, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));
            /* Send the Error Indication from here */
            /* For UE ASSOCIATED MESSAGE the data will be copied
             * autmatically */

            error_indication.bitmask = p_msg_map->bitmask;
            error_indication.old_enb_ue_x2ap_id = p_msg_map->old_enb_ue_x2ap_id;
            error_indication.new_enb_ue_x2ap_id = p_msg_map->new_enb_ue_x2ap_id;

            uecc_x2ap_fill_error_indication_values(&error_indication,
                    (x2ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            if (cause_temp.t == T_x2ap_Cause_protocol)
            {
               X2AP_TRACE(X2AP_INFO, "[EI] Fill Local error indication type [%u] cause [%u]",
                   T_x2ap_Cause_protocol, cause_temp.u.protocol);
               error_indication.cause.t = T_x2ap_Cause_protocol;
               error_indication.cause.u.protocol = cause_temp.u.protocol; 
            }

            error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_X2AP_SET_MEMBER;

            error_indication.bitmask |=
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

            uecc_x2ap_build_criticality_diagnostics_list(p_asn1_ctx,
                    &error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            uecc_x2ap_build_and_send_err_indication(peer_enodeb_id,
                    &error_indication);

            /* If Know IE has notify we have to continue the procedure */
            if (X2AP_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)
            {
                X2AP_TRACE(X2AP_INFO, "[EI] Continue the Procedure");
                result = X2AP_FAILURE;
            }
            else
            {
                result = X2AP_SUCCESS;
            }

            break;
        }
        else /* this case is for message with response messages */
        {
            if (PNULL == p_error_indication)
            {
                  X2AP_TRACE(X2AP_INFO, "p_error_indication Pointer Null");
                  break;
            }

            X2AP_TRACE(X2AP_INFO, "[EI]Build Error Indication to be sent in Response old eNB-UE-X2AP-Id[%u] new eNB-UE-X2AP-Id[%u]",
                p_msg_map->old_enb_ue_x2ap_id, p_msg_map->new_enb_ue_x2ap_id);

            p_error_indication->bitmask = p_msg_map->bitmask;
            p_error_indication->old_enb_ue_x2ap_id = p_msg_map->old_enb_ue_x2ap_id;
            p_error_indication->new_enb_ue_x2ap_id = p_msg_map->new_enb_ue_x2ap_id;

            uecc_x2ap_fill_error_indication_values(p_error_indication,
                    (x2ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            if (cause_temp.t == T_x2ap_Cause_protocol)
            {
               X2AP_TRACE(X2AP_INFO, "[EI] Fill Local error indication cause [%u] type [%u]",
                   cause_temp.u.protocol, T_x2ap_Cause_protocol);
               error_indication.cause.t          = T_x2ap_Cause_protocol;
               error_indication.cause.u.protocol = cause_temp.u.protocol; 
            }

            p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_X2AP_SET_MEMBER;

            p_error_indication->bitmask |=
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

            uecc_x2ap_build_criticality_diagnostics_list(p_asn1_ctx,
                    &p_error_indication->criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            if (X2AP_TRUE ==  p_send_error_indication->send_err_indication_reject_ie_present)
            {
                result = X2AP_SUCCESS;
            }
            else
            {
                result = X2AP_FAILURE;
            }
            break;
        }
    }
    while(0);

    return result;
}

/****************************************************************************
* Function Name  : uecc_x2ap_add_to_err_ind_ie_list
 * Inputs         : p_ie_list - pointer to uecc_x2ap_error_ind_ie_list_t 
 *                  iECriticality - pinter to x2ap_Criticality
 *                  iE_ID - pointer to x2ap_ProtocolIE_ID
 *                  p_index - pointer to index
 *                  p_send_error_indication - pointer to uecc_x2ap_error_ind_bool_t
 *                  ismissing - boolean value of type x2ap_bool_et
 *                  bypass_ignore_check - boolean value of type x2ap_bool_et
 * Outputs        : p_ie_list 
 * Returns        : None
 * Description    : This function adds error indication to Ie list
****************************************************************************/
void uecc_x2ap_add_to_err_ind_ie_list(
        uecc_x2ap_error_ind_ie_list_t *p_ie_list,
        x2ap_Criticality iECriticality,
        x2ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        uecc_x2ap_error_ind_bool_t *p_send_error_indication,
        x2ap_bool_et ismissing,
        x2ap_bool_et bypass_ignore_check)
{
    X2AP_ASSERT(PNULL != p_ie_list);
    X2AP_ASSERT(PNULL != p_index);

    /* Ignore for criticality Ignore */
    if ((x2ap_ignore != iECriticality) || (X2AP_TRUE == bypass_ignore_check))
    {
        if (X2AP_MAX_IES <=*p_index)
        {
            X2AP_TRACE(X2AP_WARNING, "%s: Invalid Index %d", __FUNCTION__, *p_index);
            return;
        }
        p_ie_list->ie_cnt++;
        p_ie_list->iE_list[*p_index].iECriticality = iECriticality;
        p_ie_list->iE_list[*p_index].iE_ID = iE_ID;

        /* set the value according to the added IE, to keep track
         * if there is any notify IE Added to the list we have to 
         * send the Error Indication Message to MME. */
        if (x2ap_notify == iECriticality)
        {
            p_send_error_indication->send_err_indication_notify_ie_present = X2AP_TRUE;
        }
        else
        {
            p_send_error_indication->send_err_indication_reject_ie_present = X2AP_TRUE;
        }

        if (X2AP_TRUE == ismissing)
        {
            p_ie_list->iE_list[*p_index].typeOfError = UECC_X2AP_ASN_MISSING;
        }

        else
        {
            p_ie_list->iE_list[*p_index].typeOfError =
                UECC_X2AP_ASN_NOT_UNDERSTOOD;
        }

        *p_index = (U16)(*p_index + 1);
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid IE encountered with Criticality Ignored");
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_x2ap_build_criticality_diagnostics_list 
 * Input         : p_asn1_ctx - pointer to OSCTXT
 *                 p_list - pointer to x2ap_CriticalityDiagnostics_IE_List
 *                 p_iE_list - pointer to uecc_x2ap_error_ind_ie_list_t
 * Output        : None 
 * RETURNS       : None
 * DESCRIPTION   : This function processes builds the criticality diagnostics 
 *                 for Abstract Syntax Error.
******************************************************************************/
void  uecc_x2ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        x2ap_CriticalityDiagnostics_IE_List *p_list,
        uecc_x2ap_error_ind_ie_list_t            *p_iE_list
)
{

    U8 count = RRC_NULL;
    OSRTDListNode* p_node = PNULL;
    x2ap_CriticalityDiagnostics_IE_List_element *p_elem = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL!=p_asn1_ctx);

    if (PNULL == p_iE_list)
    {
        return;
    }

    asn1Init_x2ap_CriticalityDiagnostics_IE_List(p_list);

    for(count =0; count < p_iE_list->ie_cnt;count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_CriticalityDiagnostics_IE_List_element,
                &p_node,
                &p_elem);

        if (PNULL==p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");
        }
        else
        {

            asn1Init_x2ap_CriticalityDiagnostics_IE_List_element(p_elem);

            p_elem->m.iE_ExtensionsPresent = 0;

            p_elem->iECriticality  = p_iE_list->iE_list[count].iECriticality;
            p_elem->iE_ID          = p_iE_list->iE_list[count].iE_ID;
            p_elem->typeOfError    = p_iE_list->iE_list[count].typeOfError;

            rtxDListAppendNode(p_list,p_node);
        }
    }

    X2AP_UT_TRACE_EXIT();

    return;
}

/****************************************************************************
 * Function Name  : uecc_x2ap_validate_ie_value
 * Inputs         : p_ie_order_map - pointer to uecc_x2ap_message_data_t
 *                  order_index - order index
 *                  id - id of ie
 *                  p_value - value to validate
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : Thid function validates ie values 
 *                  (Class -2 Errors)
 *                  (NOTE: NOT HANDLING SEQUENCE & CHOICE HERE)
 ****************************************************************************/
x2ap_return_et uecc_x2ap_validate_ie_value(
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,             /* Changed from U8 -> U16 for SgNBAdditionRequestAcknowledge IE ids Accomodation  */
        void *p_value)
{
    x2ap_return_et result = X2AP_SUCCESS;
    x2ap_HandoverRestrictionList* p_x2ap_HandoverRestrictionList;
    OSRTDListNode *p_node = PNULL;
    U8 counter = 0;
    U32 count = 0;
    U32 map_index = 0;
    U8 offset_count = 0;
    U32 orig_index = 0;
    x2ap_bool_et flag = X2AP_FALSE;
    x2ap_bool_et temp_flag = X2AP_TRUE;

    orig_index = order_index;

    do
    {
        /* Check if the IEs are in ORDER */
        /* Check if the IE before the present IE was optional */
        for (counter = 0; counter < p_ie_order_map->max_count;
                counter++)
        {    
            /* match found in the MAP */
            if (p_ie_order_map->msg_map[counter].ie_id == id)
            {    
                map_index = counter;
                order_index = map_index;

                /* Now the counter will have the index in MAP */
                /* Note: the counter is the exact position in the MAP*/

                /* Check before the this Array index how many optinals 
                 *                      * are present in the MAP */
                for (count = 0; count < map_index; count++)
                {
                    if ((x2ap_optional == p_ie_order_map->msg_map[count].presence)
                            && (X2AP_NULL == p_ie_order_map->msg_map[count].occurances))
                    {    
                        /* count the absent optional*/
                        offset_count++;
                    }    
                }    

                /* Re-Calculate the correct order for only that IE */
                p_ie_order_map->msg_map[counter].order_num = 
                    (U8)(p_ie_order_map->msg_map[counter].order_num - offset_count);

                break;
            }    
        }    

        /* Detect the wrong Order */
        if ((p_ie_order_map->msg_map[map_index].order_num == orig_index) &&
                (p_ie_order_map->msg_map[map_index].ie_id == id))
        {
            /* Duplicate Case */
            if (orig_index > map_index)
            {
                X2AP_TRACE(X2AP_WARNING, "[EI] Duplicate Detected");
                uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_WRONG_ORDER, map_index, id);
            }
        }
        else
        {
            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_WRONG_ORDER, map_index, id);
            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, map_index, id);
            flag = X2AP_TRUE;
        }

        if (X2AP_TRUE == flag)
        {
            result = X2AP_FAILURE;
            /* Entry found break out of the while*/
            break;
        }

        /* If wrong order is not detected and the IE is optional, update
         * the rest of the index */ 
        if (PNULL == p_value)
        {
            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_DATA_MISSING, order_index, id);
            break;
        }

        switch(id)
        {
#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
            /*NR_DC Code Changes Stop*/
#endif
            case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    if (MAX_OLD_ENB_UE_X2AP_ID < *(x2ap_UE_X2AP_ID *)p_value)
                    {
                        X2AP_TRACE(X2AP_WARNING, "%d Invalid MAX_OLD_ENB_UE_X2AP_ID",
                                *(x2ap_UE_X2AP_ID *)p_value);

                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    else
                    {
                        /* Update in the MAP */
                        /* set the bitmask */
                        p_ie_order_map->bitmask |= 
                            RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                        p_ie_order_map->old_enb_ue_x2ap_id = *(x2ap_UE_X2AP_ID *)p_value;
                    }
                    break;
                }

            case ASN1V_x2ap_id_Cause:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    if (X2AP_FAILURE == x2ap_verify_cause((x2ap_Cause *)p_value, p_ie_order_map,
                                order_index, id))
                    {
                        result = X2AP_FAILURE;
                    }
                    break;
                }
#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
            /*NR_DC Code Changes Stop*/
#endif
            case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    if (MAX_NEW_ENB_UE_X2AP_ID < *(x2ap_UE_X2AP_ID *)p_value)
                    {
                        X2AP_TRACE(X2AP_WARNING, "%d Invalid MAX_NEW_ENB_UE_X2AP_ID",
                                *(x2ap_UE_X2AP_ID *)p_value);

                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    else
                    {
                        /* Update in the MAP */
                        /* set the bitmask */
                        p_ie_order_map->bitmask |= 
                            RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                        p_ie_order_map->new_enb_ue_x2ap_id = *(x2ap_UE_X2AP_ID *)p_value;
                    }
                    break;
                }    
            case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                    /* Since this IE is of ignore type and every element is of
                     * Ignore TYPE not processing this message further.
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    break;
                }
            case ASN1V_x2ap_id_TargetCell_ID:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                    /* check for plmn_id */
                    for(plmn_count = 0 ; plmn_count < ((x2ap_ECGI *)(p_value))->
                            pLMN_Identity.numocts; plmn_count++)
                    {
                        if(X2AP_FAILURE == x2ap_check_plmn_id_validity(plmn_count,
                                    ((x2ap_ECGI *)(p_value))->pLMN_Identity.data[plmn_count]))
                        {
                            uecc_x2ap_update_message_map(p_ie_order_map,
                                    UECC_X2AP_INVALID_VALUE,order_index, id);
                            result = X2AP_FAILURE;
                        }
                    }  
                    break;
                }
            case ASN1V_x2ap_id_GUMMEI_ID:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    /* check for plmn_id */
                    for(plmn_count_1 = 0 ; plmn_count_1 < ((x2ap_GUMMEI *)(p_value))->
                            gU_Group_ID.pLMN_Identity.numocts; plmn_count_1++)
                    {
                        if(X2AP_FAILURE ==
                                x2ap_check_plmn_id_validity(plmn_count_1,
                                    ((x2ap_GUMMEI
                                      *)(p_value))->gU_Group_ID.pLMN_Identity.data[plmn_count_1]))
                        {
                            uecc_x2ap_update_message_map(p_ie_order_map,
                                    UECC_X2AP_INVALID_VALUE,order_index, id);
                            result = X2AP_FAILURE;
                        }
                    }

                    /* check for mME_Group_ID */
                    if((((x2ap_GUMMEI *)(p_value))->gU_Group_ID.
                                mME_Group_ID.numocts) > 2)
                    {
                        uecc_x2ap_update_message_map(p_ie_order_map,
                                UECC_X2AP_INVALID_VALUE,order_index, id);
                        result = X2AP_FAILURE;
                    }

                    /* check for mME code */
                    if((((x2ap_GUMMEI *)(p_value))->
                                mME_Code.numocts) > 1)
                    {
                        uecc_x2ap_update_message_map(p_ie_order_map,
                                UECC_X2AP_INVALID_VALUE,order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;
                }

            case ASN1V_x2ap_id_UE_ContextInformation:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    /*mme_ue_s1ap_id*/
                    if (MAX_MME_UE_S1AP_ID < ((x2ap_UE_ContextInformation*)p_value)->mME_UE_S1AP_ID)
                    {
                        X2AP_TRACE(X2AP_WARNING, "%d Invalid MAX_MME_UE_S1AP_ID",
                                *(x2ap_UE_S1AP_ID *)p_value);

                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    /*ue Security Capabilities*/
                    if(PNULL == 
                        ((x2ap_UE_ContextInformation*)p_value)->uESecurityCapabilities.encryptionAlgorithms.data)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid encryptionAlgorithms");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }
                    if(MAX_NUM_BITS < 
                        ((x2ap_UE_ContextInformation*)p_value)->uESecurityCapabilities.encryptionAlgorithms.numbits)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid encryptionAlgorithms");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }

                    if (PNULL == 
                        ((x2ap_UE_ContextInformation*)p_value)->uESecurityCapabilities.integrityProtectionAlgorithms.data)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid integrityProtectionAlgorithms");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }
                    if(MAX_NUM_BITS <
                        ((x2ap_UE_ContextInformation*)p_value)->uESecurityCapabilities.integrityProtectionAlgorithms.numbits)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid integrityProtectionAlgorithms");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }
                    /*As SecurityInformation*/
                    if (MAX_NEXT_HOP_CHAINING_COUNT <
                        ((x2ap_UE_ContextInformation*)p_value)->aS_SecurityInformation.nextHopChainingCount)
                        {
                            X2AP_TRACE(X2AP_WARNING, " Invalid NextHopChainingCount");

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                            result = X2AP_FAILURE;
                        }
                    if(MAX_KEY_ENB_STAR_NUM_BITS <
                        ((x2ap_UE_ContextInformation*)p_value)->aS_SecurityInformation.key_eNodeB_star.numbits)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid key eNB star in"
                                    "as seurity capability");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }
                    /*UE Aggregate Maximum Bit Rate*/
                    if((((x2ap_UE_ContextInformation*)p_value)->uEaggregateMaximumBitRate.
                                uEaggregateMaximumBitRateDownlink > MAX_BITRATE) || 
                            (((x2ap_UE_ContextInformation*)p_value)->uEaggregateMaximumBitRate.
                             uEaggregateMaximumBitRateUplink > MAX_BITRATE))
                    {
                        X2AP_TRACE(X2AP_WARNING, "Invalid UE aggregate Maximum Bitrate");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                order_index, id);
                        result = X2AP_FAILURE;
                    }
                    /*Subscriber Profile ID for RFP*/
                    if((((x2ap_UE_ContextInformation*)p_value)->m.subscriberProfileIDforRFPPresent)
                            && (((x2ap_UE_ContextInformation*)p_value)->
                                subscriberProfileIDforRFP > MAX_SUB_PROF_ID ))
                    {
                        X2AP_TRACE(X2AP_WARNING,"Invalid SubscriberProfileIDforRFP");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                order_index, id);
                        result = X2AP_FAILURE;
                    }
                    /*e_RABs_ToBeSetup_List*/
                    p_node =
                        ((x2ap_UE_ContextInformation*)p_value)->e_RABs_ToBeSetup_List.head;
                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_E_RABs_ToBeSetup_List_element* p_elem = 
                            (x2ap_E_RABs_ToBeSetup_List_element *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if(  X2AP_P_NULL != p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1 )
                        {
                            if (X2AP_FAILURE == 
                                    x2ap_verify_erab_qos_params(&p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters,
                                        p_ie_order_map, order_index, id))
                            {
                                result = X2AP_FAILURE;
                                continue;
                            }
                        }
                    }
                    /*rRC_Context*/
                    if(PNULL == //((x2ap_RRC_Context *)(p_value))->data)
                        ((x2ap_UE_ContextInformation*)p_value)->rRC_Context.data)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid rrc context");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                        }
                    /*handoverRestrictionList*/
                    p_x2ap_HandoverRestrictionList = (x2ap_HandoverRestrictionList *)p_value;

                    if((((x2ap_UE_ContextInformation*)p_value)->m.handoverRestrictionListPresent)
                            && (ENUM_NOT_VALID ==
                                p_x2ap_HandoverRestrictionList->forbiddenInterRATs))
                    {
                        X2AP_TRACE(X2AP_WARNING, "%d Invalid forbiddenInterRATs",
                                p_x2ap_HandoverRestrictionList->forbiddenInterRATs);
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    /* Other fields of Sequence need not to checked. If they are invalid
                     * then decoding of message will fail and that will be case of TSE*/

                    /*Location Reporting Information*/
                    if((((x2ap_UE_ContextInformation*)p_value)->m.locationReportingInformationPresent) &&
                            (x2ap_change_of_serving_cell <
                             ((x2ap_UE_ContextInformation*)p_value)->locationReportingInformation.eventType))
                    {
                        X2AP_TRACE(X2AP_WARNING,"Invalid location reporting info %u",
                                *(x2ap_EventType*)p_value);
                        uecc_x2ap_update_message_map(p_ie_order_map,
                                UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    if((((x2ap_UE_ContextInformation*)p_value)->m.locationReportingInformationPresent) &&
                       (x2ap_ecgi < ((x2ap_UE_ContextInformation*)p_value)->
                            locationReportingInformation.reportArea))
                    {
                        X2AP_TRACE(X2AP_WARNING,"Invalid location reporting info");

                        uecc_x2ap_update_message_map(p_ie_order_map,
                                UECC_X2AP_INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;
                }
            case ASN1V_x2ap_id_UE_HistoryInformation:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                    p_node = ((x2ap_UE_HistoryInformation*)(p_value))->head;
                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_LastVisitedCell_Item *p_elem =
                            (x2ap_LastVisitedCell_Item *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        switch(p_elem->t)
                        {
                            case T_x2ap_LastVisitedCell_Item_e_UTRAN_Cell:
                                {

                                    if((PNULL == p_elem->u.e_UTRAN_Cell))
                                    {
                                        uecc_x2ap_update_message_map(p_ie_order_map,
                                                UECC_X2AP_DATA_MISSING, order_index, id);
                                        continue;
                                    }
                                    for(plmn_count = 0 ; plmn_count <
                                            p_elem->u.e_UTRAN_Cell->global_Cell_ID.
                                            pLMN_Identity.numocts; plmn_count++)
                                    {
                                        if(X2AP_FAILURE == x2ap_check_plmn_id_validity(plmn_count,
                                                    (p_elem->u.e_UTRAN_Cell->global_Cell_ID.
                                                     pLMN_Identity.data[plmn_count])))
                                        {
                                            uecc_x2ap_update_message_map(p_ie_order_map,
                                                    UECC_X2AP_INVALID_VALUE,order_index, id);
                                            continue;
                                        }
                                    }  
                                    if(x2ap_large_ <
                                            p_elem->u.e_UTRAN_Cell->cellType.cell_Size)
                                    {
                                        X2AP_TRACE(X2AP_WARNING,"Invalid cell_size in ue history information %u",
                                                *(x2ap_Cell_Size*)p_value);
                                        uecc_x2ap_update_message_map(p_ie_order_map,
                                                UECC_X2AP_INVALID_VALUE, order_index, id);
                                        continue;
                                    }
                                    if (MAX_TIME_UE_STAYEDINCELL <
                                            p_elem->u.e_UTRAN_Cell->time_UE_StayedInCell)
                                    {
                                        X2AP_TRACE(X2AP_WARNING, "%d Invalid MAX_TIME_UE_STAYEDINCELL",
                                                *(x2ap_UE_S1AP_ID *)p_value);

                                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                                        continue;
                                    }
                                    break;
                                }
                            case T_x2ap_LastVisitedCell_Item_uTRAN_Cell:
                                {
                                    if((PNULL == p_elem->u.uTRAN_Cell->data))
                                    {
                                        temp_flag = X2AP_FALSE;
                                    }
                                    break;
                                }
                            case T_x2ap_LastVisitedCell_Item_gERAN_Cell:
                                {
                                    break;    
                                }
                            default:
                                temp_flag = X2AP_FALSE;

                        }
                        if(X2AP_FALSE == temp_flag)
                        {
                            X2AP_TRACE(X2AP_WARNING,"Invalid Ue History Information");
                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
                            result = X2AP_FAILURE;
                        }
                    }
                    break;
                }
            case ASN1V_x2ap_id_TraceActivation:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    if (x2ap_maximumWithoutVendorSpecificExtension< 
                            ((x2ap_TraceActivation *)(p_value))->traceDepth)
                    {
                        X2AP_TRACE(X2AP_WARNING,"Invalid ASN1V_x2ap_id_TraceActivation");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                order_index, id);
                        result = X2AP_FAILURE;
                    }

                    if (X2AP_FAILURE == 
                            x2ap_verify_tnl_address(&(((x2ap_TraceActivation *)
                                        (p_value))->traceCollectionEntityIPAddress),
                                p_ie_order_map, order_index, id))
                    {
                        result = X2AP_FAILURE;
                    }

                    for(trace_count = 0; trace_count < EUTRAN_TRACE_ID_VAL_CHECK; 
                            trace_count++)
                    {
                        if(X2AP_FAILURE == x2ap_check_plmn_id_validity(trace_count,
                                    ((x2ap_TraceActivation *)(p_value))->
                                    eUTRANTraceID.data[trace_count]))
                        {
                            uecc_x2ap_update_message_map(p_ie_order_map,
                                    UECC_X2AP_INVALID_VALUE,order_index, id);
                            result = X2AP_FAILURE;
                        }
                    }
                    break;
                }
            case ASN1V_x2ap_id_SRVCCOperationPossible:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    /* srvcc-u start */
                    if (x2ap_possible < *(x2ap_SRVCCOperationPossible*)p_value )
                    /* srvcc-u end */
                    {
                        X2AP_TRACE(X2AP_WARNING,"ASN1V_x2ap_id_SRVCCOperationPossible");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;
                }
#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            case ASN1V_x2ap_id_SgNBtoMeNBContainer:
                {
                    uecc_x2ap_update_message_map(
                            p_ie_order_map,
                            UECC_X2AP_OCCURANCE,
                            order_index,
                            id);

                    if (PNULL ==
                            ((nr_rrc_CG_Config *)
                             (p_value))->criticalExtensions.
                            u.c1)
                    {
                        X2AP_TRACE(X2AP_WARNING,
                                "Invalid SgNB to MeNB Container");
                        uecc_x2ap_update_message_map(
                                p_ie_order_map,
                                UECC_X2AP_INVALID_VALUE,
                                order_index,
                                id);

                        result = X2AP_FAILURE;
                    }
                    break;
                }
            case ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    p_node = ((x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList_element *p_elem = 
                            (x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList_element *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if (ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item != p_elem->id)
                        {
                            X2AP_TRACE(X2AP_WARNING, 
                                    "ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item != %d",
                                    p_elem->id);

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_erab_id(&p_elem->value.e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                    }
                    break;
                }
            case ASN1V_x2ap_id_SecondaryRATUsageReportList:    
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    p_node = ((x2ap_SecondaryRATUsageReportList *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_SecondaryRATUsageReportList_element *p_elem = 
                            (x2ap_SecondaryRATUsageReportList_element *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if (ASN1V_x2ap_id_SecondaryRATUsageReport_Item != p_elem->id)
                        {
                            X2AP_TRACE(X2AP_WARNING, 
                                    "ASN1V_x2ap_id_SecondaryRATUsageReport_Item != %d",
                                    p_elem->id);

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                            continue;
                        }

                        if (X2AP_FAILURE ==
                                x2ap_verify_erab_id(&p_elem->value.u._x2ap_SecondaryRATUsageReport_ItemIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_secondary_rat_type(&p_elem->value.u._x2ap_SecondaryRATUsageReport_ItemIEs_1->secondaryRATType,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_erab_usage_report_list(&p_elem->value.u._x2ap_SecondaryRATUsageReport_ItemIEs_1->e_RABUsageReportList,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                    }
                    break;
                }
            /*NR_DC Code Changes Start*/
#endif
            case ASN1V_x2ap_id_E_RABs_Admitted_List:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    p_node = ((x2ap_E_RABs_Admitted_List *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_E_RABs_Admitted_List_element *p_elem = 
                            (x2ap_E_RABs_Admitted_List_element *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if (ASN1V_x2ap_id_E_RABs_Admitted_Item != p_elem->id)
                        {
                            X2AP_TRACE(X2AP_WARNING, 
                                    "ASN1V_x2ap_id_E_RABs_Admitted_Item != %d",
                                    p_elem->id);

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_erab_id(&p_elem->value.e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if ((p_elem->value.m.uL_GTP_TunnelEndpointPresent)&&
                                (X2AP_FAILURE ==
                                 x2ap_verify_tnl_address(&p_elem->value.uL_GTP_TunnelEndpoint.transportLayerAddress,
                                     p_ie_order_map, order_index, id)))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if ((p_elem->value.m.dL_GTP_TunnelEndpointPresent)&&
                                (X2AP_FAILURE ==
                                 x2ap_verify_tnl_address(&p_elem->value.dL_GTP_TunnelEndpoint.transportLayerAddress,
                                     p_ie_order_map, order_index, id)))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }

                    }
                    break;
                }
            case ASN1V_x2ap_id_E_RABs_NotAdmitted_List:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);

                    p_node = ((x2ap_E_RAB_List*)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_E_RAB_List_element *p_elem = 
                            (x2ap_E_RAB_List_element*)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if (ASN1V_x2ap_id_E_RAB_Item!= p_elem->id)
                        {
                            X2AP_TRACE(X2AP_WARNING, 
                                    "ASN1V_x2ap_id_E_RAB_Item!= %d",
                                    p_elem->id);

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if(  X2AP_P_NULL != p_elem->value.u._x2ap_E_RAB_ItemIEs_1 ) 
                        {
                            if (X2AP_FAILURE ==
                                    x2ap_verify_erab_id(&p_elem->value.u._x2ap_E_RAB_ItemIEs_1->e_RAB_ID,
                                        p_ie_order_map, order_index, id))
                            {
                                result = X2AP_FAILURE;
                                continue;
                            }
                            if (X2AP_FAILURE ==
                                    x2ap_verify_cause(&p_elem->value.u._x2ap_E_RAB_ItemIEs_1->cause, p_ie_order_map,
                                        order_index, id))
                            {
                                result = X2AP_FAILURE;
                                continue;
                            }
                        }
                    }
                    break;
                }
            case ASN1V_x2ap_id_TargeteNBtoSource_eNBTransparentContainer:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                    if (PNULL ==
                            ((x2ap_TargeteNBtoSource_eNBTransparentContainer *)(p_value))->data)
                    {
                        X2AP_TRACE(X2AP_WARNING, "Invalid ASN1V_x2ap_id_TargeteNBtoSource_eNBTransparentContainer");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;
                }
            case ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_List:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                    p_node = ((x2ap_E_RABs_SubjectToStatusTransfer_List *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        x2ap_E_RABs_SubjectToStatusTransfer_List_element *p_elem = 
                            (x2ap_E_RABs_SubjectToStatusTransfer_List_element *)p_node->data;
                        X2AP_ASSERT(PNULL!=p_elem);

                        if (ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_Item != p_elem->id)
                        {
                            X2AP_TRACE(X2AP_WARNING, 
                                    "ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_Item != %d",
                                    p_elem->id);

                            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_erab_id(&p_elem->value.e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                        if (X2AP_FAILURE ==
                                x2ap_verify_count_value(&p_elem->value.uL_COUNTvalue,
                                    p_ie_order_map, order_index, id))
                        {
                            result = X2AP_FAILURE;
                            continue;
                        }
                    }
                    break; 
                }
/*36423_CR0407_start*/
            case ASN1V_x2ap_id_CSGMembershipStatus:
                {
                    uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_OCCURANCE, order_index, id);
                        
                    if (x2ap_not_member  < *(x2ap_CSGMembershipStatus *)p_value)
                    {
                        X2AP_TRACE(X2AP_WARNING, "Invalid CSGMembershipStatus IE value");
                        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                                    order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;
                } 
/*36423_CR0407_stop*/

            default:
                {
                    X2AP_TRACE(X2AP_WARNING, "%s: Unknow IE ", __FUNCTION__);
                    result = X2AP_FAILURE;
                }
        }
    }while(0);

    return result;
}

/****************************************************************************
 * Function Name  : uecc_x2ap_update_message_map
 * Inputs         : p_ie_order_map - pointer to uecc_x2ap_message_data_t
 *                  update_type - pointer to uecc_x2ap_map_updation_const_et
 *                  order_index - order 
 *                  id - id 
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_Failure
 * Description    : This function updates message map (Class -2 Errors)0000.
 ****************************************************************************/
x2ap_return_et uecc_x2ap_update_message_map
(
 uecc_x2ap_message_data_t *p_ie_order_map,
 uecc_x2ap_map_updation_const_et update_type,
 U32 order_index,
 U16 id
 )
{
    uecc_x2ap_message_map_t *p_msg_map = PNULL;
    U8 count = 0;
    x2ap_bool_et match_found = X2AP_FALSE;
    x2ap_return_et result = X2AP_SUCCESS;

    p_msg_map = p_ie_order_map->msg_map;

    do 
    {
        /* Check if we are updating the value against the correct
         * IE ID*/
        for (count = 0; count < p_ie_order_map->max_count; count++)
        {
            /* Match found */
            if (id == p_ie_order_map->msg_map[count].ie_id)
            {
                order_index = count;
                match_found = X2AP_TRUE;
                break;
            }
        }

        if (X2AP_TRUE != match_found)
        {
            result = X2AP_FAILURE;
            /* Do not update the Map */
            break;
        }

        switch(update_type)
        {
            case UECC_X2AP_OCCURANCE:
                {
                    /* Occurance can also tell the Missing parameters */
                    /* Since IE Order check has passed,
                     * We need not check here */
                    p_msg_map[order_index].occurances++;
                    break;
                }

            case UECC_X2AP_WRONG_ORDER:
                {
                    p_msg_map[order_index].wrong_order++;
                    break;
                }

            case UECC_X2AP_INVALID_VALUE:
                {
                    p_msg_map[order_index].invalid_value_present++;
                    break;
                }

            case UECC_X2AP_DATA_MISSING:
                {
                    p_msg_map[order_index].data_missing++;
                    break;
                }

            default:
                {
                    result = X2AP_FAILURE;
                    /* Do Nothing */
                    X2AP_TRACE(X2AP_WARNING, "Invalid Operation Value Given");
                }
        }
    } while(0);
    return result;
}

/****************************************************************************
* Function Name  :  uecc_x2ap_fill_error_indication_values
* Inputs         : p_err_ind - pointer to rrc_x2ap_error_indication_t
*                  proc_code - pointer to x2ap_ProcedureCode
*                  triggering_msg - piinter to x2ap_TriggeringMessage 
*                  proc_criticality - pointer to x2ap_Criticalityx2ap_Criticalityx2ap_Criticality
* Outputs        : p_err_ind
* Returns        : None
* Description    : This function fills error indication values.
****************************************************************************/
void uecc_x2ap_fill_error_indication_values(rrc_x2ap_error_indication_t *p_err_ind,
        x2ap_ProcedureCode proc_code,
        x2ap_TriggeringMessage triggering_msg,
        x2ap_Criticality proc_criticality)
{
    p_err_ind->criticality_diagnostics.m.procedureCodePresent = RRC_X2AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCode = proc_code;

    p_err_ind->criticality_diagnostics.m.triggeringMessagePresent = RRC_X2AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.triggeringMessage = triggering_msg -1;
    p_err_ind->criticality_diagnostics.m.procedureCriticalityPresent = RRC_X2AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCriticality = proc_criticality;

    p_err_ind->bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;

    uecc_x2ap_fill_cause_for_error_indication_message(triggering_msg, proc_code, p_err_ind);
}

/****************************************************************************
* Function Name  :  uecc_x2ap_fill_cause_for_error_indication_message
* Inputs         : triggering_msg - pointer to x2ap_TriggeringMessage
*                  procedure_code - X2AP procedure code
*                  p_error_indication - pointer to rrc_x2ap_error_indication_t
* Outputs        : p_error_indication 
* Returns        : None
* Description    : This function fills cause for erroe indication
****************************************************************************/

void uecc_x2ap_fill_cause_for_error_indication_message(
        x2ap_TriggeringMessage triggering_msg,
        U16 procedure_code,
        rrc_x2ap_error_indication_t *p_error_indication)
{
    switch(triggering_msg)
    {
        case T_x2ap_X2AP_PDU_initiatingMessage:
            {
                switch(procedure_code)
                {
                    case ASN1V_x2ap_id_uEContextRelease:
                    case ASN1V_x2ap_id_snStatusTransfer:
                    //case ASN1V_x2ap_id_errorIndication:
                        {
                            if ((RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT & 
                                    p_error_indication->bitmask) &&
                                !(RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = 
                                                x2ap_unknown_new_eNB_UE_X2AP_ID;
                            }
                            else if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                 p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork =
                                                    x2ap_unknown_old_eNB_UE_X2AP_ID;
                            }
                            else if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                !(RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork =
                                                    x2ap_unknown_pair_of_UE_X2AP_ID;
                            }
                            else
                            {
                                p_error_indication->cause.t = T_x2ap_Cause_protocol;
                                p_error_indication->cause.u.protocol =
                                    x2ap_abstract_syntax_error_falsely_constructed_message;
                            }

                            break;
                        }

                    case ASN1V_x2ap_id_handoverPreparation:
                    case ASN1V_x2ap_id_handoverCancel:
                        {
                            if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                        p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = 
                                                        x2ap_unknown_new_eNB_UE_X2AP_ID;
                            }

                            break;
                        }
                    default:
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_protocol;
                            p_error_indication->cause.u.protocol =
                                x2ap_abstract_syntax_error_falsely_constructed_message;
                        }
                }

                break;
            }
        case T_x2ap_X2AP_PDU_successfulOutcome:
        {
            switch (procedure_code)
            {
                case ASN1V_x2ap_id_handoverPreparation:
                    {
                        if ((RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT & 
                                    p_error_indication->bitmask) &&
                                !(RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = 
                                                x2ap_unknown_new_eNB_UE_X2AP_ID;
                        }
                        else if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                 p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork =
                                                    x2ap_unknown_old_eNB_UE_X2AP_ID;
                        }
                        else if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                !(RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork =
                                                    x2ap_unknown_pair_of_UE_X2AP_ID;
                        }
                        else
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_protocol;
                            p_error_indication->cause.u.protocol =
                                x2ap_abstract_syntax_error_falsely_constructed_message;
                        }

                        break;
                    }
                default:
                    {
                        p_error_indication->cause.t = T_x2ap_Cause_protocol;
                        p_error_indication->cause.u.protocol =
                            x2ap_abstract_syntax_error_falsely_constructed_message;
                    }
            }

            break;
        }
        case T_x2ap_X2AP_PDU_unsuccessfulOutcome:
        {
            switch (procedure_code)
            {
                case ASN1V_x2ap_id_handoverPreparation:
                    {
                        if (!(RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_x2ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = 
                                                    x2ap_unknown_new_eNB_UE_X2AP_ID;
                        }

                        break;
                    }

                default:
                    {
                        p_error_indication->cause.t = T_x2ap_Cause_protocol;
                        p_error_indication->cause.u.protocol =
                            x2ap_abstract_syntax_error_falsely_constructed_message;
                    }
            }

            break;
        }
        default:
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid Triggering Message");
        }
    }
}


/****************************************************************************
* Function Name  : x2ap_verify_cause 
* Inputs         : p_ie_order_map - pointer to uecc_x2ap_message_data_t 
*                  order_index - order index
*                  id - id
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : this function varifies the erroe indication cause
****************************************************************************/
x2ap_return_et x2ap_verify_cause(x2ap_Cause *p_value,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    x2ap_return_et result = X2AP_SUCCESS;
    x2ap_bool_et flag = X2AP_TRUE;
    switch (p_value->t)
    {
        case T_x2ap_Cause_radioNetwork:
            {
                if (p_value->u.radioNetwork > 
                        x2ap_not_supported_QCI_value)
                {
                    flag = X2AP_FALSE;
                }
                break;
            }
        case T_x2ap_Cause_transport:
            {
                if (p_value->u.transport > x2ap_unspecified_3)
                {
                    flag = X2AP_FALSE;
                }
                break;
            }
        case T_x2ap_Cause_protocol:
            {
                if (p_value->u.protocol >
                        x2ap_abstract_syntax_error_falsely_constructed_message)
                {
                    flag = X2AP_FALSE;
                }
                break;
            }
        case T_x2ap_Cause_misc:
            {
                if (p_value->u.misc > x2ap_unspecified)
                {
                    flag = X2AP_FALSE;
                }
                break;
            }
        default:
            flag = X2AP_FALSE;
    }

    if (X2AP_FALSE == flag)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid Cause Value");
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE, order_index, id);
        result = X2AP_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : x2ap_check_mme_group_id_validity
 * Inputs         : index - index of mme group id 
 *                  octateValue- octate value
 * Outputs        : None
 * Returns        : x2AP_SUCCESS/X2AP_FAILURE
 * Description    : Check the validity of mme group id
*****************************************************************************/
x2ap_return_et x2ap_check_mme_group_id_validity(U8 index, U8 octateValue)
{
    x2ap_return_et result = X2AP_FAILURE;

    switch (index)
    {
        case 0:
            /* Check the first Octate */
            if ((octateValue > 0x00) && (octateValue < 0xFF))
            {
                result = X2AP_SUCCESS;
            }
            break;
        case 1:
            /* Check the second Octate */

            if ((octateValue > 0x00) && (octateValue < 0xFE))
            {
                result = X2AP_SUCCESS;
            }
            break;
        default:
            {
                result = X2AP_FAILURE;
            }
    }
    return result;
}
/****************************************************************************
 * Function Name  : x2ap_check_mme_code_validity
 * Inputs         : index - index of mme group id 
 *                  octateValue - octate value
 * Outputs        : None
 * Returns        : x2AP_SUCCESS/X2AP_FAILURE
 * Description    : Check the validity of mme code
*****************************************************************************/
x2ap_return_et x2ap_check_mme_code_validity(U8 index, U8 octateValue)
{
    x2ap_return_et result = X2AP_FAILURE;

    switch (index)
    {
        case 0:
            /* Check the first Octate */
            if ((octateValue > 0x00) && (octateValue < 0xFE))
            {
                result = X2AP_SUCCESS;
            }
            break;
        default:
            {
                result = X2AP_FAILURE;
            }
    }
    return result;
}
               
/****************************************************************************
 * Function Name  : x2ap_check_plmn_id_validity
 * Inputs         : index - index of mme goup id
 *                  octateValue -octate value
 * Outputs        : None
 * Returns        : x2ap_return_et
 * Description    : Check the validity of PLMN
*****************************************************************************/
x2ap_return_et x2ap_check_plmn_id_validity(U8 index, U8 octateValue)
{
    x2ap_return_et result = X2AP_FAILURE;

    switch (index)
    {
        case 0:
        case 2:
            /* Check the first Octate */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID_VAL) &&
                    ((octateValue >> 0x04) <= MAX_PLMN_ID_VAL))
            {
                result = X2AP_SUCCESS;
            }
            break;
        case 1:
            /* Check the second Octate */

            /* check for the filler bit at second nibble */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID_VAL) &&
                    (((octateValue >> 0x04) == 0xF) ||
                     ((octateValue >> 0x04) <= MAX_PLMN_ID_VAL)))
            {
                result = X2AP_SUCCESS;
            }
            break;
        default:
            {
                result = X2AP_FAILURE;
            }
    }
    return result;
} 
/****************************************************************************
* Function Name  : x2ap_verify_erab_id
 * Inputs         : p_e_RAB_ID - pointer to x2ap_E_RAB_ID 
 *                  p_ie_order_map - pointer to uecc_x2ap_message_data_t
 *                  order_index - order index 
 *                  id - type of unsigned char 
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function verifies the erab id.
****************************************************************************/
x2ap_return_et x2ap_verify_erab_id(x2ap_E_RAB_ID *p_e_RAB_ID, 
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (INVALID_ERAB_ID <= *p_e_RAB_ID)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid ERAB ID %d", *p_e_RAB_ID);
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }

    return X2AP_SUCCESS;
}
/****************************************************************************
* Function Name  : x2ap_verify_count_value
* Inputs         : p_count_value - pointer to x2ap_COUNTvalue
*                  p_ie_order_m - pointer to uecc_x2ap_message_data_t 
*                  order_index - order index 
*                  id - type of unsigned char 
* Outputs        : None
* Returns        : X2AP_SUCESS/X2AP_FAILURE
* Description    : This function verifies the count value
****************************************************************************/
x2ap_return_et x2ap_verify_count_value(x2ap_COUNTvalue *p_count_value, 
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (MAX_PDCP_SN < p_count_value->pDCP_SN)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid count value for pdcp sn");
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }
    if (MAX_HFN < p_count_value->hFN)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid count value for hfn");
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }
    return X2AP_SUCCESS;
}
/****************************************************************************
* Function Name  : x2ap_verify_erab_qos_params 
* Inputs         : p_qos - pointer to x2ap_E_RAB_Level_QoS_Parameters
*                  p_ie_order_map - pointer to uecc_x2ap_message_data_t
*                  order_index - order index
*                  id - type of unsigned int 
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : This function validates erab qos pters 
****************************************************************************/
x2ap_return_et x2ap_verify_erab_qos_params(x2ap_E_RAB_Level_QoS_Parameters *p_qos,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    x2ap_return_et result = X2AP_SUCCESS;
    do
    {
        if (x2ap_spare == p_qos->allocationAndRetentionPriority.priorityLevel)
        {
            /* The values 1-15 are valid. Priority Level 0 must lead to a
             * logical error. */
            X2AP_TRACE(X2AP_WARNING, "Invalid ARP Priority Level. QCI = %u",
                           p_qos->qCI);
            result = X2AP_FAILURE;
        }

        if (X2AP_FAILURE == result)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid QCI in QOS Prameter");
            break;
        }

        if (x2ap_may_trigger_pre_emption < 
                p_qos->allocationAndRetentionPriority.pre_emptionCapability)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid pre_emptionCapability in QOS Prameter");
            result = X2AP_FAILURE;
            break;
        }

        if (x2ap_pre_emptable < 
                p_qos->allocationAndRetentionPriority.pre_emptionVulnerability)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid pre_emptionVulnerability in QOS Prameter");
            result = X2AP_FAILURE;
            break;
        }

        if (p_qos->m.gbrQosInformationPresent)
        {
            if ((MAX_BITRATE < p_qos->gbrQosInformation.e_RAB_MaximumBitrateDL) ||
                (MAX_BITRATE < p_qos->gbrQosInformation.e_RAB_MaximumBitrateUL) ||
                (MAX_BITRATE < p_qos->gbrQosInformation.e_RAB_GuaranteedBitrateDL) ||
                (MAX_BITRATE < p_qos->gbrQosInformation.e_RAB_GuaranteedBitrateUL))
            {
                X2AP_TRACE(X2AP_WARNING, "Invalid gbrQosInformation in QOS Prameter");
                result = X2AP_FAILURE;
                break;
            }
        }

    }while(0);

    if(X2AP_FAILURE == result)
    {
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
    }

    return result;
}
/****************************************************************************
* Function Name  : x2ap_verify_dl_forwarding
* Inputs         : p_dl_forwarding - pointer to x2ap_DL_Forwarding 
*                  p_ie_order_map - pointer to uecc_x2ap_message_data_t
*                  order_index - order index
*                  id - type of unsigned char
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : This function verifies DL forbarding.
****************************************************************************/
x2ap_return_et x2ap_verify_dl_forwarding(x2ap_DL_Forwarding *p_dl_forwarding,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (x2ap_dL_forwardingProposed < *p_dl_forwarding)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid dl forwarding %d", *p_dl_forwarding);
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }

    return X2AP_SUCCESS;
}
/****************************************************************************
* Function Name  : x2ap_verify_tnl_address 
* Inputs         : p_tnl - pointer to x2ap_TransportLayerAddress
*                  p_ie_order_map - pointer to uecc_x2ap_message_data_t 
*                  order_index - order index 
*                  id - type od unsigned char 
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : This function verifies tunnle address
****************************************************************************/
x2ap_return_et x2ap_verify_tnl_address(x2ap_TransportLayerAddress *p_tnl,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (PNULL == p_tnl->data)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid TNL Address");
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }

    return X2AP_SUCCESS;
}

/****************************************************************************
* Function Name  : x2ap_verify_secondary_rat_type
* Inputs         : p_secondary_rat_type - pointer to x2ap_SecondaryRATUsageReport_Item_secondaryRATType 
*                  p_ie_order_map - pointer to uecc_x2ap_message_data_t
*                  order_index - order index
*                  id - type of unsigned char
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : This function verifies Secondary Rat Type.
****************************************************************************/
x2ap_return_et x2ap_verify_secondary_rat_type(x2ap_SecondaryRATUsageReport_Item_secondaryRATType *p_secondary_rat_type,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (x2ap_nr < *p_secondary_rat_type)
    {
        X2AP_TRACE(X2AP_WARNING, "Invalid secondary rat type %d", *p_secondary_rat_type);
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
        return X2AP_FAILURE;
    }

    return X2AP_SUCCESS;
}

/****************************************************************************
* Function Name  : x2ap_verify_erab_usage_report_list 
* Inputs         : p_qos - pointer to x2ap_E_RAB_Level_QoS_Parameters
*                  p_ie_order_map - pointer to uecc_x2ap_message_data_t
*                  order_index - order index
*                  id - type of unsigned int 
* Outputs        : None
* Returns        : X2AP_SUCCESS/X2AP_FAILURE
* Description    : This function validates erab qos pters 
****************************************************************************/
x2ap_return_et x2ap_verify_erab_usage_report_list(OSRTDList *p_value,
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    x2ap_return_et result = X2AP_SUCCESS;
    OSRTDListNode *p_node = PNULL;

    p_node = ((x2ap_E_RABUsageReportList *)(p_value))->head;

    for ( ; PNULL != p_node; p_node = p_node->next)
    {
        x2ap_E_RABUsageReportList_element *p_elem = 
            (x2ap_SecondaryRATUsageReportList_element *)p_node->data;
        X2AP_ASSERT(PNULL!=p_elem);

        if (ASN1V_x2ap_id_E_RABUsageReport_Item != p_elem->id)
        {
            X2AP_TRACE(X2AP_WARNING, 
                    "ASN1V_x2ap_id_E_RABUsageReport_Item != %d",
                    p_elem->id);

            uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                    order_index, id);
            result = X2AP_FAILURE;
            continue;
        }

        if (4 < p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->startTimeStamp.numocts)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid startTimeStamp.numocts = %u",
                           p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->startTimeStamp.numocts);
            result = X2AP_FAILURE;
        }

        if (PNULL == p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->startTimeStamp.data)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid startTimeStamp.data is NULL");
            result = X2AP_FAILURE;
        }

        if (4 < p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->endTimeStamp.numocts)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid endTimeStamp.numocts = %u",
                           p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->endTimeStamp.numocts);
            result = X2AP_FAILURE;
        }

        if (PNULL == p_elem->value.u._x2ap_E_RABUsageReport_ItemIEs_1->endTimeStamp.data)
        {
            X2AP_TRACE(X2AP_WARNING, "Invalid endTimeStamp.data is NULL");
            result = X2AP_FAILURE;
        }

    }

    if(X2AP_FAILURE == result)
    {
        uecc_x2ap_update_message_map(p_ie_order_map, UECC_X2AP_INVALID_VALUE,
                order_index, id);
    }

    return result;
}
