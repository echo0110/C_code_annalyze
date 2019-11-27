/*******************************************************************************
*
*  FILE NAME   : x2ap_error_indication.c
*
*  DESCRIPTION : This file contains the x2ap error indication
*                function definitions.
*
*  DATE 27-July-2012  NAME - Hari Oum Sharan
*                      REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_db.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "x2ap_sctp_sm.h"
#include "x2ap_utility.h"
#include "asn_enc_wrappers.h"
#include "asn_dec_wrappers.h"
#include "x2ap_error_indication.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "x2setup_sm.h"
#include "x2ap_rsu_sm.h"
#include "x2ap_uecc.h"
#include "rrc_logging.h"
/* SPR 16750 Fix Start */
#include "rrc_common_md.h"
/* SPR 16750 Fix Stop */

#define RRC_X2AP_SET_MEMBER 1
#define MAX_PLMN_ID      0x09

static U8 plmn_count;
/*static U8 plmn_count_1;*/

x2ap_return_et x2ap_check_plmn_identity(U8 index, U8 octateValue);

/*****************************************************************************
 * Function Name  : x2ap_parse_and_decode_error_indication
 * Inputs         : p_parsed_error_indication - pointer to rrc_x2ap_error_indication_t 
 *                  p_asn_msg - Pointer to ERROR INDICATION Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 * Outputs        : p_asn_msg , asn_msg_len
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function parses and decode the received ERROR INDICATION 
 *                  ASN message.
 *****************************************************************************/
x2ap_return_et x2ap_parse_and_decode_error_indication ( 
        rrc_x2ap_error_indication_t *p_parsed_error_indication,
        void                      *p_asn_msg,
        U16                        asn_msg_len)
{
    x2ap_X2AP_PDU                   x2ap_pdu;
    x2ap_return_et                  result = X2AP_SUCCESS;
    OSCTXT                          asn1_ctx;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    U32                             encoded_msg_len = 0;
    LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT  *p_event_x2_err_ind = PNULL;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_parsed_error_indication != X2AP_P_NULL);
    X2AP_ASSERT(p_asn_msg != X2AP_P_NULL);

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            result = X2AP_FAILURE;
            break;
        }
        else
        {
            /* Generating Protocol Event LOCAL_X2_ERROR_INDICATION*/
            encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ERROR_INDICATION, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            
            /* Generating Protocol Event LOCAL_EVENT_X2_ERROR_INDICATION*/
            p_event_x2_err_ind = rrc_mem_get(sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
            if (PNULL != p_event_x2_err_ind)
            {
                memset_wrapper(p_event_x2_err_ind, RRC_NULL, 
                        sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
                p_event_x2_err_ind->header.length = sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT);
                p_event_x2_err_ind->header.event_id = LOCAL_EVENT_X2_ERROR_INDICATION;

                p_event_x2_err_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_RECEIVED;

                if (p_parsed_error_indication->bitmask & RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT)
                {
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = 
                        p_parsed_error_indication->cause.t;

			/* CSR 51448 START */
                    switch(p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_x2ap_Cause_radioNetwork:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_parsed_error_indication->cause.u.radioNetwork;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_parsed_error_indication->cause.t, p_parsed_error_indication->cause.u.radioNetwork);
                            break;

                        case T_x2ap_Cause_transport:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_parsed_error_indication->cause.u.transport;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_parsed_error_indication->cause.t, p_parsed_error_indication->cause.u.transport);
                            break;
                        case T_x2ap_Cause_protocol:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_parsed_error_indication->cause.u.protocol;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_parsed_error_indication->cause.t, p_parsed_error_indication->cause.u.protocol);
                            break;
                        case T_x2ap_Cause_misc:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                p_parsed_error_indication->cause.u.misc;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_parsed_error_indication->cause.t, p_parsed_error_indication->cause.u.misc);
                            break;
                    }
                    /* CSR 51448 STOP */
                }
                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_err_ind);
            }
        }

        /* check whether the message is successful outcome or not*/
        if ((T_x2ap_X2AP_PDU_initiatingMessage != x2ap_pdu.t) ||
                (ASN1V_x2ap_id_errorIndication != 
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                (PNULL == 
                  x2ap_pdu.u.initiatingMessage->value.u.errorIndication) ||
                 (T1x2ap__errorIndication!= 
                  x2ap_pdu.u.initiatingMessage->value.t))
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            result = X2AP_FAILURE;
            break;
        }

        /* Decode message */
        if(X2AP_FAILURE == rrc_x2ap_error_indication_intrl_dec(
                x2ap_pdu.u.initiatingMessage->value.u.errorIndication,
                p_parsed_error_indication))
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: X2AP error Indication "
                    "decode failed ",__FUNCTION__);
            result = X2AP_FAILURE;
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, "%s: X2AP error Indication "
                    "decode Success ",__FUNCTION__);
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    X2AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  : x2ap_log_criticality_diagostics 
 * Inputs         : p_criticality_diagostics -pointer to x2ap_CriticalityDiagnostics 
 * Outputs        : None
 * Returns        : None
 * Description    : THis function prepares logs for criticality diagostics
 *                  Class -2 Errors 
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
void x2ap_log_criticality_diagostics(
        x2ap_CriticalityDiagnostics *p_criticality_diagostics)
{
    x2ap_CriticalityDiagnostics_IE_List_element* p_item = PNULL;
    OSRTDListNode* p_node = PNULL;
    U8 count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_TRACE(X2AP_INFO, "[CRITICALITY DIAGNOSTICS]");

    X2AP_ASSERT(PNULL!=p_criticality_diagostics);
   
    if (p_criticality_diagostics->m.procedureCodePresent)
    {
        X2AP_TRACE(X2AP_INFO,"[CD] ProcedureCode = %d", 
                p_criticality_diagostics->procedureCode);
    }

    if (p_criticality_diagostics->m.triggeringMessagePresent)
    {
        X2AP_TRACE(X2AP_INFO,"[CD] TriggeringMessage = %d", 
                p_criticality_diagostics->triggeringMessage);
    }

    if (p_criticality_diagostics->m.procedureCriticalityPresent)
    {
        X2AP_TRACE(X2AP_INFO,"[CD] ProcedureCriticality = %d", 
                p_criticality_diagostics->procedureCriticality);
    }

    if (p_criticality_diagostics->m.iEsCriticalityDiagnosticsPresent)
    {
        X2AP_TRACE(X2AP_INFO,"[CD] iEsCriticalityDiagnostics" );

        p_node = p_criticality_diagostics->iEsCriticalityDiagnostics.head;
        for (count = 0; count < p_criticality_diagostics->iEsCriticalityDiagnostics.count;
                count++)
        {
            p_item = (x2ap_CriticalityDiagnostics_IE_List_element*)p_node->data;

            if (PNULL == p_item)
            {
                X2AP_TRACE(X2AP_INFO, "Pointer NULL");
                break;
            }

            X2AP_TRACE(X2AP_INFO, "------iE_Criticality %d", p_item->iECriticality);
            X2AP_TRACE(X2AP_INFO, "------iE_ID %d", p_item->iE_ID);

            switch (p_item->typeOfError)
            {
                case 0:
                    X2AP_TRACE(X2AP_INFO, "------%d TypeOfError: not-understood", 
                            p_item->iECriticality);
                    break;
                case 1:
                    X2AP_TRACE(X2AP_INFO, "------%d TypeOfError: missing", 
                            p_item->iECriticality);
                    break;
                default:
                    X2AP_TRACE(X2AP_INFO, "Unknwon Type of Error");
            }
            p_node = p_node->next;
        }
    }

    X2AP_UT_TRACE_EXIT();
}

/****************************************************************************
* Function Name  : x2ap_log_error_indication_message
* Inputs         : p_err_ind - pointer to rrc_x2ap_error_indication_t
* Outputs        : None
* Returns        : None 
* Description    : This function prepares logs for error indication msg  
*                  Class -2 Errors 
*                  NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
****************************************************************************/
void x2ap_log_error_indication_message(rrc_x2ap_error_indication_t *p_err_ind)
{
    X2AP_UT_TRACE_ENTER();

    X2AP_TRACE(X2AP_INFO, "[ERROR INDICATION MESSAGE]");

    if (RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        X2AP_TRACE(X2AP_INFO, "[EI] OLD ENB UE X2AP ID = %d ", 
                                    p_err_ind->old_enb_ue_x2ap_id);
    }

    if (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        X2AP_TRACE(X2AP_INFO, "[EI] NEW ENB UE X2AP ID = %d ", 
                                    p_err_ind->new_enb_ue_x2ap_id);
    }

    if (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT &
            p_err_ind->bitmask)
    {
        X2AP_TRACE(X2AP_INFO, "[EI] Cause Type: =%d [EI] Cause Value =%d", 
                p_err_ind->cause.t, p_err_ind->cause.u.radioNetwork);
    }

    if (RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_err_ind->bitmask)
    {
        x2ap_log_criticality_diagostics(&p_err_ind->criticality_diagnostics);
    }

    X2AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : x2ap_process_error_indication_msg 
 * Inputs         : p_msg_ptr      Pointer to message buffer
 *                : p_x2ap_gl_ctx  pointer to X2AP global context
 *                : p_enb_ctx      Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the Error Indication
 *                  mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_process_error_indication_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    rrc_x2ap_error_indication_t          error_indication;
    /*x2ap_ErrorIndication_protocolIEs x2ap_error_indication;*/
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    U8                    *p_asn_msg;
    U16                   asn_msg_len;
    void                  *p_intrl_msg = PNULL;
    x2ap_bool_et          fwd_to_uecc = X2AP_FALSE;
    x2ap_ue_associated_sig_msg_ind_t  *msg_p  = X2AP_P_NULL;
    x2_gb_enb_id_t       gb_enb_id;

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    x2_gb_gnb_id_t       gb_gnb_id;
    /* MENB CHNAGES - END */
#endif

    /*For RSU Procedure*/
    rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
    U8 counter = X2AP_NULL;
    x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;
    x2ap_peer_enb_context_t *p_temp_enb_ctx = X2AP_P_NULL;
    U32 enb_meas_Id = X2AP_NULL;
    /* SPR 16750 Fix Start */
    /* SPR 19326 Fix Start */
    rrc_module_id_t       uecc_module_id = RRC_UECC_1_MODULE_ID;
    /* SPR 19326 Fix Stop */
    /* SPR 16750 Fix Stop */

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    /*X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);*/
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&error_indication, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&gb_enb_id, X2AP_NULL, sizeof(x2_gb_enb_id_t));

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    X2AP_MEMSET(&gb_gnb_id, X2AP_NULL, sizeof(x2_gb_gnb_id_t));
    /* MENB CHNAGES - END */
#endif

    p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    p_asn_msg = (p_peer_enb_msg)->p_msg;
    asn_msg_len = (U16)(p_peer_enb_msg->msg_len);

    if (X2AP_SUCCESS == x2ap_parse_and_decode_error_indication(
                &error_indication,
                p_asn_msg,
                asn_msg_len))
    {
            /* SPR 16750 Fix Start */
        if((error_indication.bitmask &
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT) &&
                (error_indication.criticality_diagnostics.m.procedureCodePresent))
            /* SPR 16750 Fix Stop */
        {
            switch(error_indication.criticality_diagnostics.procedureCode)
            {
                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                case ASN1V_x2ap_id_endcX2Setup:
                /* MENB CHANGES - END */
                #endif

                case ASN1V_x2ap_id_x2Setup:
                case ASN1V_x2ap_id_errorIndication:
                case ASN1V_x2ap_id_eNBConfigurationUpdate:
                case ASN1V_x2ap_id_reset:
                case ASN1V_x2ap_id_rLFIndication:
                case ASN1V_x2ap_id_cellActivation:
                case ASN1V_x2ap_id_loadIndication:
                case ASN1V_x2ap_id_resourceStatusReporting:
                case ASN1V_x2ap_id_resourceStatusReportingInitiation:
                case ASN1V_x2ap_id_handoverReport:/*Not supported*/
                case ASN1V_x2ap_id_mobilitySettingsChange:/*Not supported*/
                    X2AP_TRACE(X2AP_INFO, "Error Indication Received "
                            "Procedure Code:%d",
                            error_indication.criticality_diagnostics.procedureCode);
                    break;
                default:
                    fwd_to_uecc = X2AP_TRUE;
            }
        }

        if (fwd_to_uecc == X2AP_TRUE)
        {
            p_intrl_msg = x2ap_alloc_intrl_msg(
                    RRC_UECC_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

            if (PNULL == p_intrl_msg)
            {
                /*out of memory*/
                X2AP_TRACE(X2AP_FATAL, "rrc_mem_get failed!!");
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            msg_p = (x2ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);
            msg_p->peer_enodeb_id = p_enb_ctx->peer_enodeb_id;

            X2AP_MEMCPY((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                        sizeof(x2ap_ue_associated_sig_msg_ind_t)),
                    (const void*)p_asn_msg,
                    asn_msg_len);

            X2AP_TRACE(X2AP_INFO, "[%s]: Forwarding message to UECC\n",
                    __FUNCTION__);

            /* SPR 16750 Fix Start */
            if (error_indication.criticality_diagnostics.m.procedureCodePresent)
            {
                if ( (error_indication.criticality_diagnostics.procedureCode == 
                        ASN1V_x2ap_id_handoverPreparation) ||
                     (error_indication.criticality_diagnostics.procedureCode ==
                        ASN1V_x2ap_id_handoverCancel) ||
                     (error_indication.criticality_diagnostics.procedureCode ==
                        ASN1V_x2ap_id_snStatusTransfer) )
                {
                    if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT )
                    {
                        uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.old_enb_ue_x2ap_id);
                    }
                    else
                    {
                        /* SPR_17377_Warning_fix + */
                        X2AP_TRACE(X2AP_ERROR, "[%s] Unable to forward message to UECC as "
                            "old_enb_ue_x2ap_id not present\n",
                             __FUNCTION__);
                        /* SPR_17377_Warning_fix - */
                    }
                }
                else
                {
                    if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT )
                    {
                        uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.new_enb_ue_x2ap_id);
                    }
                    else
                    {
                        /* SPR_17377_Warning_fix + */
                        X2AP_TRACE(X2AP_ERROR, "[%s] Unable to forward message to UECC as "
                            "new_enb_ue_x2ap_id not present\n",
                             __FUNCTION__);
                        /* SPR_17377_Warning_fix - */
                    }
                }
            }
            else
            {
                if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT )
                {
                    uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.old_enb_ue_x2ap_id);
                }
                else if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT )
                {
                    uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.new_enb_ue_x2ap_id);
                }
                else
                {
                    /* SPR_17377_Warning_fix + */
                    X2AP_TRACE(X2AP_ERROR, "[%s] Unable to forward message to UECC \n",
                            __FUNCTION__);
                    /* SPR_17377_Warning_fix - */
                }
            }
            /*SPR_19979_START*/
            if (((U8)(uecc_module_id - RRC_UECC_1_MODULE_ID)) < 
            /*SPR_19979_END*/
                    p_x2ap_gl_ctx->no_of_uecc_instances)
            {
                /* SPR_17377_Warning_fix + */
                    X2AP_TRACE(X2AP_ERROR, "[%s] uecc_module_id = %u \n",__FUNCTION__,
                            uecc_module_id);
                /* SPR_17377_Warning_fix - */
                /* Send Message to UECC*/
                rrc_send_message(
                        p_intrl_msg,
                        uecc_module_id);
            }
            else
            {
                 /*SPR_19979_START*/
                X2AP_TRACE(X2AP_INFO, "[%s]: Invalid UECC instance Id .. ignored.",
                        __FUNCTION__);
                    x2ap_msg_mem_free(p_intrl_msg);
                return X2AP_FAILURE;
                 /*SPR_19979_END*/
            }
            /* SPR 16750 Fix Stop */
        }
        else
        {
            x2ap_log_error_indication_message(&error_indication);

            /*If the procedure code is X2_SETUP*/
            if (ASN1V_x2ap_id_x2Setup == 
                    error_indication.criticality_diagnostics.procedureCode)
            {
                /*If the current state is X2_SETUP_W_X2_SETUP_RESP_ST 
                 *Stop the X2AP SETUP procedure*/
                if (X2_SETUP_W_X2_SETUP_RESP_ST == 
                        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state)
                {
                    X2AP_TRACE(X2AP_INFO, "Error Indication Received "
                            "while in state in X2_SETUP_W_X2_SETUP_RESP_ST "
                            "so stoping the X2_SETUP Procedure.");
                    /* Stop X2 Setup Response guard Timer */
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;

                    #ifdef ENDC_ENABLED
                    /* MENB CHANGES - START */
                    if ( p_x2ap_gl_ctx->num_peer_enb )
                    {
                    /* MENB CHANGES - END */
                    #endif
                    gb_enb_id = p_enb_ctx->gb_enb_id;
                    /* Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM */
                    if(X2AP_SUCCESS != x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                            (U8 *)&gb_enb_id,
                            p_x2ap_gl_ctx,
                            p_enb_ctx ))
                    {
                        X2AP_TRACE(X2AP_ERROR, "x2ap_sctp_sm_entry failed.");
                        return X2AP_FAILURE;
                    }
                    #ifdef ENDC_ENABLED
                    /* MENB CHANGES - START */
                    }
                    else if ( p_x2ap_gl_ctx->num_peer_gnb )
                    {
                        gb_gnb_id = p_enb_ctx->gb_gnb_id;
                        /* Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM */
                        if(X2AP_SUCCESS != x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                                    (U8 *)&gb_gnb_id,
                                    p_x2ap_gl_ctx,
                                    p_enb_ctx ))
                        {
                            X2AP_TRACE(X2AP_ERROR, "x2ap_sctp_sm_entry failed.");
                            return X2AP_FAILURE;
                        }
                    }
                    /* MENB CHANGES - END */
                    #endif

                    /* Change state to IDLE */
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state =
                        X2_SETUP_IDLE_ST;
                    /* reset the timer ids and retry count */
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;

                    /* Send Link down indication to OAM and RRM*/
                }
            }
            /*If the procedure code is RSU*/
            if (ASN1V_x2ap_id_resourceStatusReportingInitiation == 
                    error_indication.criticality_diagnostics.procedureCode)
            {
                enb_meas_Id = x2ap_rsu_get_meas_id(p_x2ap_gl_ctx);
                if ( X2AP_RSU_MEASUREMENT_ID_INVALID != enb_meas_Id)
                {
                    /* Get tree node */ 
                    p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)
                        x2ap_st_find_tree_node(&(p_x2ap_gl_ctx->x2ap_rsu_context_tree),
                                &(enb_meas_Id));

                    if(X2AP_P_NULL != p_rsu_tree_node)
                    {
                        /* get the measurement context */
                        p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;       
                        for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
                        {
                            /* Bug:9129 */
                            if((p_x2ap_gl_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id == 
                                    p_x2ap_rsu_ctx->peer_enb_id) && 
                                (p_x2ap_gl_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
                            /* Bug:9129 */
                            {
                                p_temp_enb_ctx = &p_x2ap_gl_ctx->peer_enb_cntxt_arr[counter];
                                break;
                            }
                            else
                                continue;
                        }
                    }
                    if(X2AP_P_NULL != p_temp_enb_ctx)
                    {
                        if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING) 
                        {
                            /* Stop the timer X2AP_RSU_RRM_GUARD_TIMER. */
                            x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
                            p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 


                            /* send the RESOURCE STATUS REQUEST (START) Failure to RRM */ 
                            x2ap_rsu_send_rrm_start_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
                                    X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,
                                    x2ap_unspecified_2,p_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
                                    ,X2AP_P_NULL,X2AP_NULL);

                            /*Delete the context from tree*/
                            x2ap_st_delete_node(&(p_x2ap_gl_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
                            x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
                            return X2AP_SUCCESS;
                        }
                        else if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING)
                        {
                            /* Stop the timer X2AP_RSU_RRM_GUARD_TIMER. */
                            x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
                            p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 


                            /* send the RESOURCE STATUS REQUEST (STOP) Failure to RRM */ 
                            x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
                                    X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,p_temp_enb_ctx->gb_enb_id,x2ap_unspecified_2);

                            /*Delete the context from tree*/
                            x2ap_st_delete_node(&(p_x2ap_gl_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
                            x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
                            return X2AP_SUCCESS;
                        }    
                    }
                    else 
                    {
                        X2AP_TRACE(X2AP_WARNING, 
                                "[%s]: Peer Enb Context is NULL while fetching "
                                "from x2ap global context\n", __FUNCTION__);
                    }
                }
                else
                {
                    X2AP_TRACE(X2AP_WARNING,"\nInValid MeasureMent ID ");
                }
            }

            /* SPR 16750 Fix Start */
            if (!error_indication.criticality_diagnostics.m.procedureCodePresent)
            {
                p_intrl_msg = x2ap_alloc_intrl_msg(
                        RRC_UECC_MODULE_ID,
                        X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                        (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

                if (PNULL == p_intrl_msg)
                {
                    /*out of memory*/
                    X2AP_TRACE(X2AP_FATAL, "rrc_mem_get failed!!");
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }

                msg_p = (x2ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
                        RRC_API_HEADER_SIZE);
                msg_p->peer_enodeb_id = p_enb_ctx->peer_enodeb_id;

                X2AP_MEMCPY((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                            sizeof(x2ap_ue_associated_sig_msg_ind_t)),
                        (const void*)p_asn_msg,
                        asn_msg_len);

                if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT )
                {
                    uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.old_enb_ue_x2ap_id);
                }
                else if ( error_indication.bitmask &
                        RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT )
                {
                    uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            error_indication.new_enb_ue_x2ap_id);
                }
                else
                {
                    /* SPR_17377_Warning_fix + */
                    X2AP_TRACE(X2AP_ERROR, "[%s] Unable to forward message to UECC \n",
                            __FUNCTION__);
                    /* SPR_17377_Warning_fix - */
                    /*SPR_18154_START*/
                    x2ap_msg_mem_free(p_intrl_msg);
                    return X2AP_FAILURE;
                    /*SPR_18154_END*/
                }

            /*SPR_19979_START*/
                if (((U8)(uecc_module_id - RRC_UECC_1_MODULE_ID)) < 
            /*SPR_19979_END*/
                        p_x2ap_gl_ctx->no_of_uecc_instances)
                {
                    /* SPR_17377_Warning_fix + */
                    X2AP_TRACE(X2AP_ERROR, "[%s] uecc_module_id = %u \n",__FUNCTION__,
                            uecc_module_id);
                    /* SPR_17377_Warning_fix - */
                    /* Send Message to UECC*/
                    rrc_send_message(
                            p_intrl_msg,
                            uecc_module_id);
                }
                else
                {
                    /*SPR_19979_START*/
                    X2AP_TRACE(X2AP_INFO, "[%s]: Invalid UECC instance Id .. ignored.",
                            __FUNCTION__);
                    x2ap_msg_mem_free(p_intrl_msg);
                    return X2AP_FAILURE;
                    /*SPR_19979_END*/
                }
            }
            /* SPR 16750 Fix Stop */
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Unable to parse Error Indication Message");
        return X2AP_FAILURE;
    }
    return X2AP_SUCCESS;
}




/*## part 2*/
/****************************************************************************
* Function Name  : x2ap_build_and_send_err_indication
* Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
*                  p_peer_enb_ctx - pointer to PEER eNB context
*                  p_error_indication - pointer to rrc_x2ap_error_indication_t
* Outputs        : None
* Returns        : X2AP_SUCCESS / X2AP_FAILURE
* Description    : packs p_error_indication into X2AP_PDU ASN
****************************************************************************/
x2ap_return_et x2ap_build_and_send_err_indication
(
 x2ap_gb_context_t *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_peer_enb_ctx,
 rrc_x2ap_error_indication_t *p_error_indication
 )
{
    OSCTXT asn1_ctx;
    U8 asn_buff[X2AP_MAX_ASN1_BUF_LEN];
    U16 asn_msg_len = 0;
    x2ap_return_et result = X2AP_FAILURE;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;

    LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT  *p_event_x2_err_ind = PNULL;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_peer_enb_ctx);


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_ErrorIndication] ASN context initialization failed");
        return X2AP_FAILURE;
    }

    X2AP_MEMSET(asn_buff, 0, X2AP_MAX_ASN1_BUF_LEN);

    do
    {
        /*Generating Protocol Event LOCAL_EVENT_X2_ERROR_INDICATION*/
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
                    (U32)(p_error_indication->cause.t);

                /* CSR 51448 START */
                switch(p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                {
                    case T_x2ap_Cause_radioNetwork:
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE = 
                        p_error_indication->cause.u.radioNetwork;
                    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                            NOTIFICATION, "cause type = %d\n and cause value = %d",
                            (U32)(p_error_indication->cause.t), p_error_indication->cause.u.radioNetwork);
                    break;

                    case T_x2ap_Cause_transport:
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                        p_error_indication->cause.u.transport;
                    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                            NOTIFICATION, "cause type = %d\n and cause value = %d",
                            (U32)(p_error_indication->cause.t), p_error_indication->cause.u.transport);
                    break;
                    case T_x2ap_Cause_protocol:
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                        p_error_indication->cause.u.protocol;
                    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                            NOTIFICATION, "cause type = %d\n and cause value = %d",
                            (U32)(p_error_indication->cause.t), p_error_indication->cause.u.protocol);
                    break;
                    case T_x2ap_Cause_misc:
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                        p_error_indication->cause.u.misc;
                    ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                            NOTIFICATION, "cause type = %d\n and cause value = %d",
                            (U32)(p_error_indication->cause.t), p_error_indication->cause.u.misc);
                    break;
                }
                /* CSR 51448 STOP */
            }
            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_err_ind);
        }
        /* Its a Transfer Syntax Error */
        if (PNULL == p_error_indication)
        {
            X2AP_TRACE(X2AP_INFO, "Encoding Transfer Syntax Error");
            result = rrc_x2ap_error_indication_tse_intrl_enc(&asn1_ctx,
                    asn_buff,
                    &asn_msg_len);
            if (X2AP_FAILURE == result)
            {
                X2AP_TRACE(X2AP_WARNING, "Unable to Encode Transfer Syntax Error");
            }
            else
            {
                /*coverity_fix_531*/ 

                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        (U32)asn_msg_len);

                if (PNULL != p_event_header)
                {
                    x2ap_fill_protocol_event_params(p_event_header, 
                            (U32)asn_msg_len,
                            LOCAL_X2_ERROR_INDICATION, 
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)asn_buff,
                            (U32)asn_msg_len);

                    /*coverity_fix_531*/ 
                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            break;
        }

        /* Its a Abstract Syntax Error */
        X2AP_TRACE(X2AP_INFO, "Encoding Abstract Syntax Error");

        result = rrc_x2ap_error_indication_ase_intrl_enc(&asn1_ctx,
                asn_buff,
                &asn_msg_len,
                p_error_indication);
        if (result == X2AP_SUCCESS)
        {
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, 
                        (U32)asn_msg_len,
                        LOCAL_X2_ERROR_INDICATION, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)asn_buff,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while (0);

    if (X2AP_SUCCESS == result)
    {
        result = x2ap_send_msg_to_peer_enb(p_x2ap_gb_ctx,
                p_peer_enb_ctx, 
                asn_buff, 
                asn_msg_len);

        if( X2AP_SUCCESS != result)
        {
            X2AP_TRACE(X2AP_ERROR,
                    "Unable to send message to Peer eNODEB ");
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "Unable to encode the ERROR Indication"
                " message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return result;
}

 /*****************************************************************************
 * Function Name  : x2ap_send_msg_to_peer_enb
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_peer_enb_ctx - peer eNB Context
 *                  p_message - ASN Encoded Buffer
 *                  message_buff_len - Buffer Length
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function send the message to peer eNODEB
 ******************************************************************************/
x2ap_return_et x2ap_send_msg_to_peer_enb(
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2ap_peer_enb_context_t* p_peer_enb_ctx,
        U8* p_message,
        U32 message_buff_len)
{
    x2ap_return_et return_val = X2AP_SUCCESS;
    x2ap_peer_enb_msg_t   peer_enb_msg;
    
    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_ctx != X2AP_P_NULL);
    
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    /* Check if the peer eNODEB is in ACTIVE State */
    if (p_peer_enb_ctx->sctp_sm_ctx.cur_sctp_sm_state != X2AP_SCTP_ESTABLISHED_ST)
    {
        X2AP_TRACE(X2AP_WARNING, "%s:"
                " peer eNB context with id %d "
                "is not active.",
                __FUNCTION__, p_peer_enb_ctx->peer_enodeb_id);

        return_val = X2AP_FAILURE;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"[%s]:Sending "
                "Message to peer eNODEB %d",
                __FUNCTION__,p_peer_enb_ctx->peer_enodeb_id);

        peer_enb_msg.p_msg = p_message;
        peer_enb_msg.msg_len = message_buff_len;
        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */
        if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
        {
            /*Sending the load information to Peer eNB via SCTP */
            return_val = x2ap_sctp_sm_entry
                (X2AP_SCTP_SEND_DATA_EV,
                 (U8*)&peer_enb_msg,
                 p_x2ap_gb_ctx,
                 p_peer_enb_ctx);
        }
    }
    X2AP_UT_TRACE_EXIT();

    return return_val;
}


/****************************************************************************
* Function Name  : x2ap_add_to_err_ind_ie_list
* Inputs         : p_ie_list - pointer to x2ap_error_ind_ie_list_t 
*                  iECriticality - pointer to x2ap_Criticality
*                  iE_ID - pointer to x2ap_ProtocolIE_ID
*                  p_index - pointer to index
*                  p_send_error_indication - pointer to x2ap_error_ind_bool_t
*                  ismissing - value of type x2ap_bool_et
*                  bypass_ignore_check -  value of type x2ap_bool_et
* Outputs        : p_ie_list
* Returns        : None
* Description    : This function adds error indication to ie list
****************************************************************************/
void x2ap_add_to_err_ind_ie_list(
        x2ap_error_ind_ie_list_t *p_ie_list,
        x2ap_Criticality iECriticality,
        x2ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        x2ap_error_ind_bool_t *p_send_error_indication,
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
            p_ie_list->iE_list[*p_index].typeOfError = ASN_MISSING;
        }

        else
        {
            p_ie_list->iE_list[*p_index].typeOfError =
                ASN_NOT_UNDERSTOOD;
        }

        *p_index = (U16)(*p_index + 1);
    }
    else
    {
        X2AP_TRACE(X2AP_INFO, "Invalid IE encountered with Criticality Ignored");
    }
}

/****************************************************************************
* Function Name  : x2ap_parse_message_map
* Inputs         : p_asn1_ctx - pointer to OSCTXT
*                  p_x2ap_gl_ctx - pointer to X2AP global context
*                  p_enb_ctx - pointer to PEER eNB context 
*                  p_msg_map -  pointer to x2ap_message_data_t
*                  p_ie_list -  pointer to x2ap_error_ind_ie_list_t
*                  p_index_to_update - pointer to index 
*                  p_send_error_indication - pointer to x2ap_error_ind_bool_t
*                  proc_code - procdeure code
*                  triggering_msg - triggering msg 
*                  proc_criticality - criticality level
*                  p_error_indication - pointer to rrc_x2ap_error_indication_t
* Outputs        : None
* Returns        : X2AP_SUCCESS or X2AP_FAILURE
* Description    : This function parses the message map
****************************************************************************/
x2ap_return_et x2ap_parse_message_map(
        OSCTXT *p_asn1_ctx,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_message_data_t *p_msg_map,
        x2ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        x2ap_error_ind_bool_t *p_send_error_indication,
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
                    X2AP_TRACE(X2AP_INFO, "%s: [EI] Unknow IE with Reject Criticality Detected "
                            "in Response MSG, Terminate the procedure and do the local"
                            " Error Handling", __FUNCTION__);
                    return X2AP_SUCCESS;
                }

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
                            X2AP_TRACE(X2AP_INFO, "%s: [EI] IE ID [%d] "
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
                            X2AP_TRACE(X2AP_INFO, "%s: [EI] IE ID [%d] "
                                    "Detected Error Indication for Reject"
                                    " Criticatlity IE", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return X2AP_SUCCESS;
                        }
                    }
                    else /* This is for the notify, where know IE we will send EI */
                    {
                        X2AP_TRACE(X2AP_INFO, "%s: [EI] IE ID [%d] "
                                "Detected Error Indication for Notify"
                                " Criticatlity IE, Error Indication should be sent", 
                                __FUNCTION__, p_msg_map->msg_map[count].ie_id);
                        break;
                    }
                }

                X2AP_TRACE(X2AP_DETAILED, "%s: Response message parse Success", __FUNCTION__);
                return X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO, "%s:[EI] X2AP Initiating Message", __FUNCTION__);
            }
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"[EI] Unknow IE with Notify criticality detected,"
                    " Error Indication Should be Sent");
        }
    } while(0);

    if (T_x2ap_X2AP_PDU_initiatingMessage == triggering_msg)
    {
        X2AP_TRACE(X2AP_DETAILED,"[EI] Parsing Initiating Message");
    }
    else
    {
        X2AP_TRACE(X2AP_DETAILED,"[EI] Parsing Resposne Message with "
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
                        (p_msg_map->msg_map[count].presence == x2ap_mandatory))  || 
                    (p_msg_map->msg_map[count].data_missing >= X2AP_IE_PRESENT))
            {
                X2AP_TRACE(X2AP_INFO, "[EI] %d Mandatory IE/Data missing Detected",
                        p_msg_map->msg_map[count].ie_id);
                /* Mandatory Missing or Data missing */
                x2ap_add_to_err_ind_ie_list(p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        X2AP_TRUE,
                        X2AP_FALSE);

                if(x2ap_reject == p_msg_map->msg_map[count].criticality)
                {
                    X2AP_TRACE(X2AP_INFO,"[EI]  : x2ap_reject");
                    cause_temp.t = T_x2ap_Cause_protocol;
                    cause_temp.u.protocol = x2ap_abstract_syntax_error_reject;
                }
                else if (x2ap_notify == p_msg_map->msg_map[count].criticality)
                {
                    X2AP_TRACE(X2AP_INFO,"[EI]  : x2ap_notify");
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
                    X2AP_TRACE(X2AP_INFO, "[EI] IE ID [%d] Wrong Order/Too many occurance for "
                            "Ignore Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);

                    x2ap_add_to_err_ind_ie_list(p_ie_list, 
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
                    X2AP_TRACE(X2AP_INFO, "[EI] IE ID [%d] Wrong Value/Order/Too many occurance for "
                            "Reject/Notify Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);
                    x2ap_add_to_err_ind_ie_list(p_ie_list, 
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
        if((X2AP_TRUE == p_send_error_indication->send_err_indication_notify_ie_present) ||
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

            x2ap_fill_error_indication_values(&error_indication,
                    (x2ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);
            
            if (cause_temp.t == T_x2ap_Cause_protocol)
            {
               X2AP_TRACE(X2AP_INFO, "[EI] Filling Local Error Indication cause");
               error_indication.cause.t          = T_x2ap_Cause_protocol;
               error_indication.cause.u.protocol = cause_temp.u.protocol; 
            }
            
            error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_X2AP_SET_MEMBER;

            error_indication.bitmask |=
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

            x2ap_build_criticality_diagnostics_list(p_asn1_ctx,
                    &error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            x2ap_build_and_send_err_indication(
                    p_x2ap_gl_ctx,
                    p_enb_ctx,
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
            X2AP_TRACE(X2AP_INFO, "[EI]Build Error Indication to be sent in Response");

            p_error_indication->bitmask = p_msg_map->bitmask;
            p_error_indication->old_enb_ue_x2ap_id = p_msg_map->old_enb_ue_x2ap_id;
            p_error_indication->old_enb_ue_x2ap_id = p_msg_map->old_enb_ue_x2ap_id;

            x2ap_fill_error_indication_values(p_error_indication,
                    (x2ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            if (cause_temp.t == T_x2ap_Cause_protocol)
            {
               X2AP_TRACE(X2AP_INFO, "[EI] Filling Local Error Indication cause");
               error_indication.cause.t          = T_x2ap_Cause_protocol;
               error_indication.cause.u.protocol = cause_temp.u.protocol; 
            }

            p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_X2AP_SET_MEMBER;

            p_error_indication->bitmask |=
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

            x2ap_build_criticality_diagnostics_list(p_asn1_ctx,
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
 * Function Name  : x2ap_fill_error_indication_values
 * Inputs         : p_err_ind - pointer to rrc_x2ap_error_indication_t
 *                  proc_code - value of x2ap_ProcedureCode
 *                  triggering_msg - value of x2ap_TriggeringMessage
 *                  proc_criticality - type of x2ap_Criticality  
 * Outputs        : p_err_ind
 * Returns        : None
 * Description    : This function fills erroe indication values
****************************************************************************/
void x2ap_fill_error_indication_values(rrc_x2ap_error_indication_t *p_err_ind,
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

    p_err_ind->cause.t = T_x2ap_Cause_protocol;
    p_err_ind->cause.u.protocol = 
        x2ap_abstract_syntax_error_falsely_constructed_message;
}

/******************************************************************************
 * Function Name : x2ap_build_criticality_diagnostics_list 
 * Inputs        : p_asn1_ctx - pointer to OSCTXT
 *                 p_list - pointer to x2ap_CriticalityDiagnostics_IE_List
 *                 p_iE_list - pointer to x2ap_error_ind_ie_list_t
 * Outputs       : None 
 * Returns       : None  
 * Description   : This function processes builds the criticality diagnostics 
 *                 for Abstract Syntax Error. 
******************************************************************************/
void  x2ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        x2ap_CriticalityDiagnostics_IE_List *p_list,
        x2ap_error_ind_ie_list_t              *p_iE_list
        )
{

    U8 count = X2AP_NULL;
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
 * Function Name  : x2ap_validate_ie_value
 * Inputs         : p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index - index  
 *                  id - id of ie
 *                  p_value - pointer to value to validate
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function validates passed value for passed ie
 *                  Class -2 Errors 
 *                  NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
x2ap_return_et x2ap_validate_ie_value(
        x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value)
{
    x2ap_return_et result = X2AP_SUCCESS;
    U8 counter = 0;
    S16 count = 0;
    U8 map_index = 0;
    U8 offset_count = 0;
    U32 orig_index = 0;

    x2ap_bool_et flag = X2AP_FALSE;
    x2ap_bool_et temp_flag= X2AP_TRUE;

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
                for (count = (S16)(map_index - 1); count >= 0; count--)
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
            }    
        }    
        /* Detect the wrong Order */
        if ((p_ie_order_map->msg_map[map_index].order_num == orig_index) &&
                (p_ie_order_map->msg_map[map_index].ie_id == id))
        {
            /* Duplicate Case */
            if (orig_index > map_index)
            {
                x2ap_update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            }
        }
        else
        {
            x2ap_update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            x2ap_update_message_map(p_ie_order_map, OCCURANCE, map_index, id);
            flag = X2AP_TRUE;
        }

        if (X2AP_TRUE == flag)
        {
            /* Entry found break out of the while*/
            break;
        }

        /* If wrong order is not detected and the IE is optional, update
         * the rest of the index */ 
        if (PNULL == p_value)
        {
            x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            break;
        }

        switch(id)
        {
            case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (MAX_OLD_ENB_UE_X2AP_ID < *(x2ap_UE_X2AP_ID *)p_value)
                    {
                        X2AP_TRACE(X2AP_INFO, "%d Invalid MAX_OLD_ENB_UE_X2AP_ID",
                                *(x2ap_UE_X2AP_ID *)p_value);

                        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
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

            case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (MAX_NEW_ENB_UE_X2AP_ID < *(x2ap_UE_X2AP_ID *)p_value)
                    {
                        X2AP_TRACE(X2AP_INFO, "%d Invalid MAX_NEW_ENB_UE_X2AP_ID",
                                *(x2ap_UE_X2AP_ID *)p_value);

                        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    else
                    {
                        /* Update in the Map */
                        /* set the bitmask */
                        p_ie_order_map->bitmask |= 
                            RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                        p_ie_order_map->new_enb_ue_x2ap_id = *(x2ap_UE_X2AP_ID *)p_value;
                    }

                    break;
                }
            case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    /* Since this IE is of ignore type and every element is of 
                     * Ignore TYPE not processing this message further. 
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    break;
                }

            case ASN1V_x2ap_id_Cause:
                {
                    x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    switch (((x2ap_Cause *)p_value)->t)
                    {
                        case T_x2ap_Cause_radioNetwork:
                            {
                                if (((x2ap_Cause *)p_value)->u.radioNetwork > 
                                       x2ap_not_supported_QCI_value)
                                {
                                    temp_flag = X2AP_FALSE;
                                }
                                break;
                            }
                        case T_x2ap_Cause_transport:
                            {
                                if (((x2ap_Cause *)p_value)->u.transport > x2ap_unspecified_3)
                                {
                                    temp_flag = X2AP_FALSE;
                                }
                                break;
                            }
                        case T_x2ap_Cause_protocol:
                            {
                                if (((x2ap_Cause *)p_value)->u.protocol >
                                        x2ap_abstract_syntax_error_falsely_constructed_message)
                                {
                                    temp_flag = X2AP_FALSE;
                                }
                                break;
                            }
                        case T_x2ap_Cause_misc:
                            {
                                if (((x2ap_Cause *)p_value)->u.misc >
                                        x2ap_unspecified)
                                {
                                    temp_flag = X2AP_FALSE;
                                }
                                break;
                            }
                        default:
                            temp_flag = X2AP_FALSE;
                    }

                    if (X2AP_FALSE == temp_flag)
                    {
                        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }

                    break;
                }
            case ASN1V_x2ap_id_TimeToWait:
                {
                    x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                    /* Since this IE is of ignore type and every element is of
                     * Ignore TYPE not processing this message further.
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    if (x2ap_v60s < *(x2ap_TimeToWait*)p_value)
                    {
                        X2AP_TRACE(X2AP_INFO, "%d Invalid TimeTo Wait ",
                                *(x2ap_TimeToWait *)p_value);

                        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = X2AP_FAILURE;
                    }
                    break;

                }
           case ASN1V_x2ap_id_ReportingPeriodicity:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
               
               if(x2ap_ten_thousand_ms < *(x2ap_ReportingPeriodicity*)p_value)
               {
                   X2AP_TRACE(X2AP_INFO, "%d invalid reporting periodicity ",
                           *(x2ap_ReportingPeriodicity*)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;
           }

           case ASN1V_x2ap_id_Registration_Request:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if(x2ap_stop < *(x2ap_Registration_Request*)p_value)
               {
                   X2AP_TRACE(X2AP_INFO, "%d invalid Registration Request ",
                           *(x2ap_Registration_Request*)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;
           }

           case ASN1V_x2ap_id_ENB1_Measurement_ID:
           case ASN1V_x2ap_id_ENB2_Measurement_ID:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if((*((x2ap_Measurement_ID*)p_value) < 1) ||
                  (*((x2ap_Measurement_ID*)p_value) > 4095))
               {
                   X2AP_TRACE(X2AP_INFO, "%d invalid Measurement ID ",
                           *(x2ap_Measurement_ID*)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;

               }
               break;
           }

           case ASN1V_x2ap_id_FailureCellPCI:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if(*((x2ap_PCI*)p_value) > 503)
               {
                   X2AP_TRACE(X2AP_INFO, "%d invalid Failure Cell PCI ",
                           *(x2ap_PCI*)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;
           }
           case ASN1V_x2ap_id_GlobalENB_ID:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for plmn_id */
               for(plmn_count = 0 ; plmn_count < ((x2ap_GlobalENB_ID  *)(p_value))->
                       pLMN_Identity.numocts; plmn_count++)
               {
                   if(X2AP_FAILURE == x2ap_check_plmn_identity(plmn_count,
                               ((x2ap_GlobalENB_ID *)(p_value))->pLMN_Identity.data[plmn_count]))
                   {
                       x2ap_update_message_map(p_ie_order_map,
                               INVALID_VALUE,order_index, id);
                       result = X2AP_FAILURE;
                   }
               } 
               break;
           }

           case ASN1V_x2ap_id_Re_establishmentCellECGI:
           case ASN1V_x2ap_id_SourceCellECGI:
           case ASN1V_x2ap_id_FailureCellECGI:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for plmn_id */
               for(plmn_count = 0 ; plmn_count < ((x2ap_ECGI *)(p_value))->
                       pLMN_Identity.numocts; plmn_count++)
               {
                   if(X2AP_FAILURE == x2ap_check_plmn_identity(plmn_count,
                               ((x2ap_ECGI *)(p_value))->pLMN_Identity.data[plmn_count]))
                   {
                       x2ap_update_message_map(p_ie_order_map,
                               INVALID_VALUE,order_index, id);
                       result = X2AP_FAILURE;
                   }
               } 
           }
           break;

           case ASN1V_x2ap_id_FailureCellCRNTI:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for bitstring 16*/
               if (16 < ((x2ap_CRNTI *)(p_value))->numbits)
               {
                   x2ap_update_message_map(p_ie_order_map,
                           INVALID_VALUE,order_index, id);
                   result = X2AP_FAILURE;
               }
           }
           break;

           case ASN1V_x2ap_id_ShortMAC_I:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for bitstring 16*/
               if (16 < ((x2ap_ShortMAC_I *)(p_value))->numbits)
               {
                   x2ap_update_message_map(p_ie_order_map,
                           INVALID_VALUE,order_index, id);
                   result = X2AP_FAILURE;
               }
           }
           break;
           case ASN1V_x2ap_id_RRCConnSetupIndicator:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if ( 0  != *(x2ap_RRCConnSetupIndicator*)p_value)
               {
                   X2AP_TRACE(X2AP_INFO, "%d Invalid RRCConnSetupIndicator ",
                           *(x2ap_RRCConnSetupIndicator *)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;

           }
           case ASN1V_x2ap_id_PartialSuccessIndicator:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if ( 0  != *(x2ap_PartialSuccessIndicator*)p_value)
               {
                   X2AP_TRACE(X2AP_INFO, "%d Invalid PartialSuccessIndicator ",
                           *(x2ap_PartialSuccessIndicator *)p_value);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;

           }

           case ASN1V_x2ap_id_HandoverReportType:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for range 0 - 1 */
               if ( *((x2ap_HandoverReportType *)(p_value)) > 1 )
               {
                   x2ap_update_message_map(p_ie_order_map,
                           INVALID_VALUE,order_index, id);
                   result = X2AP_FAILURE;
               }
           }
           break;

           case ASN1V_x2ap_id_ENB1_Cell_ID:
           case ASN1V_x2ap_id_ENB2_Cell_ID:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               /* check for plmn_id */
               for(plmn_count = 0 ; plmn_count < ((x2ap_ECGI *)(p_value))->
                       pLMN_Identity.numocts; plmn_count++)
               {
                   if(X2AP_FAILURE == x2ap_check_plmn_identity(plmn_count,
                               ((x2ap_ECGI *)(p_value))->pLMN_Identity.data[plmn_count]))
                   {
                       x2ap_update_message_map(p_ie_order_map,
                               INVALID_VALUE,order_index, id);
                       result = X2AP_FAILURE;
                   }
               } 
           }
           break;
           case ASN1V_x2ap_id_ENB1_Mobility_Parameters:
           case ASN1V_x2ap_id_ENB2_Proposed_Mobility_Parameters:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if((((x2ap_MobilityParametersInformation*)p_value)->handoverTriggerChange < (-20)) ||
                       (((x2ap_MobilityParametersInformation*)p_value)->handoverTriggerChange > 20))
               {
                   X2AP_TRACE(X2AP_INFO, "%d invalid ENB Mobility Parameters ",
                           ((x2ap_MobilityParametersInformation*)p_value)->handoverTriggerChange);

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               break;
           }

           case ASN1V_x2ap_id_ENB2_Mobility_Parameters_Modification_Range:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

               if((((x2ap_MobilityParametersModificationRange*)p_value)->handoverTriggerChangeLowerLimit > 20) ||
                       (((x2ap_MobilityParametersModificationRange*)p_value)->handoverTriggerChangeLowerLimit < -20))
               {
                   X2AP_TRACE(X2AP_INFO, "Invalid ENB2_Mobility_Parameters_Modification_Range "
                        "handoverTriggerChangeLowerLimit");

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
               if((((x2ap_MobilityParametersModificationRange*)p_value)->handoverTriggerChangeUpperLimit > 20) ||
                       (((x2ap_MobilityParametersModificationRange*)p_value)->handoverTriggerChangeUpperLimit < -20))
               {
                   X2AP_TRACE(X2AP_INFO, "Invalid ENB2_Mobility_Parameters_Modification_Range "
                        "handoverTriggerChangeUpperLimit");

                   x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                   result = X2AP_FAILURE;
               }
           break;
           }

           #ifdef ENDC_ENABLED
           /* MENB CHANGES - START */
           case ASN1V_x2ap_id_RespondingNodeType_EndcX2Setup:
           case ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup:
           case ASN1V_x2ap_id_RespondingNodeType_EndcConfigUpdate:
           case ASN1V_x2ap_id_InitiatingNodeType_EndcConfigUpdate:
           case ASN1V_x2ap_id_Globalen_gNB_ID:
           case ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList:
           case ASN1V_x2ap_id_ServedNRcellsToModifyListENDCConfUpd:
           case ASN1V_x2ap_id_ServedNRcellsToDeleteListENDCConfUpd:
           case ASN1V_x2ap_id_CellAssistanceInformation:
           /* MENB CHANGES - END */
           #endif
           case ASN1V_x2ap_id_GUGroupIDList:
           case ASN1V_x2ap_id_CellInformation:
           case ASN1V_x2ap_id_ReportCharacteristics:
           case ASN1V_x2ap_id_CellToReport:
           case ASN1V_x2ap_id_CellMeasurementResult:
           case ASN1V_x2ap_id_ServedCells:
           case ASN1V_x2ap_id_ServedCellsToAdd:
           case ASN1V_x2ap_id_ServedCellsToModify:
           case ASN1V_x2ap_id_ServedCellsToDelete:
           case ASN1V_x2ap_id_GUGroupIDToAddList:
           case ASN1V_x2ap_id_GUGroupIDToDeleteList:
           case ASN1V_x2ap_id_UE_RLF_Report_Container:
           case ASN1V_x2ap_id_ServedCellsToActivate:
           case ASN1V_x2ap_id_ActivatedCellList:
           case ASN1V_x2ap_id_CompleteFailureCauseInformation_List:
           case ASN1V_x2ap_id_MeasurementInitiationResult_List:
           {
               x2ap_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
               /* No logical range defined*/
               break;
           }

           default:
           {
               X2AP_TRACE(X2AP_INFO, "%s, Unknown IE", __FUNCTION__);
               result = X2AP_FAILURE;
           }

        }
    }
    while(0);

    return result;
}

/****************************************************************************
* Function Name  : x2ap_update_message_map
* Inputs         : p_ie_order_map - pointer to x2ap_message_data_t 
*                  update_type - type of map_updation_const_et
*                  order_index - index 
*                  id - id
* Outputs        : None
* Returns        : X2AP_SUCCESS or X2AP_FAILURE
* Description    : THis function updates message map 
*                  Class -2 Errors
****************************************************************************/
x2ap_return_et x2ap_update_message_map
(
 x2ap_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,/*PUT definition of this struct in rrc_defines.h*/
 U32 order_index,
 U16 id
)
{
    x2ap_message_map_t *p_msg_map = PNULL;
    U8 count = 0;
    x2ap_bool_et match_found = X2AP_FALSE;
    x2ap_return_et result = X2AP_SUCCESS;

    p_msg_map = (x2ap_message_map_t *)(p_ie_order_map->msg_map);

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
            /* Do not update the Map */
            result = X2AP_FAILURE;
            break;
        }

        switch(update_type)
        {
            case OCCURANCE:
                {
                    /* Occurance can also tell the Missing parameters */
                    /* Since IE Order check has passed,
                     * We need not check here */
                    p_msg_map[order_index].occurances++;
                    break;
                }

            case WRONG_ORDER:
                {
                    p_msg_map[order_index].wrong_order++;
                    break;
                }

            case INVALID_VALUE:
                {
                    p_msg_map[order_index].invalid_value_present++;
                    break;
                }

            case DATA_MISSING:
                {
                    p_msg_map[order_index].data_missing++;
                    break;
                }

            default:
                {
                    result=X2AP_FAILURE;
                    /* Do Nothing */
                    X2AP_TRACE(X2AP_INFO, "Invalid Operation Value Given");
                }
        }
    } while(0);
    return result;
}

/****************************************************************************
 * Function Name  : x2ap_check_plmn_identity
 * Inputs         : index - index of MME
 *                  octateValue - octate value
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : Check the validity of PLMN
*****************************************************************************/
x2ap_return_et x2ap_check_plmn_identity(U8 index, U8 octateValue)
{
    x2ap_return_et result = X2AP_FAILURE;

    switch (index)
    {
        case 0:
        case 2:
            /* Check the first Octate */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID) &&
                    ((octateValue >> 0x04) <= MAX_PLMN_ID))
            {
                result = X2AP_SUCCESS;
            }
            break;
        case 1:
            /* Check the second Octate */

            /* check for the filler bit at second nibble */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID) &&
                    (((octateValue >> 0x04) == 0xF) ||
                     ((octateValue >> 0x04) <= MAX_PLMN_ID)))
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
