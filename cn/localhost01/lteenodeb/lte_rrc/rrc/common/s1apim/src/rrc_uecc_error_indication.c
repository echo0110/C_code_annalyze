/******************************************************************************
*  ARICENT -
*
*  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
*
******************************************************************************
*
*  $Id: s1ap_error_indication.c,v 1.6 2011/02/28 07:04:17 Jitender Kumar Exp $
*
******************************************************************************
*
*  File Description : This file contains the Error Indication related code
*
******************************************************************************/

/******************************************************************************
* Standard Library Includes
*****************************************************************************/

/******************************************************************************
* Project Includes
*****************************************************************************/
#include "rrc_uecc_error_indication.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "rrc_s1ap_uecc_intf.h"
#include "uecc_ue_ctx.h"
#include "uecc_logger.h"
#include "rtxContext.h"
#include "uecc_global_ctx.h"
#include "rrc_logging.h"
/*SPR 15896 Fix Start*/
#include "uecc_utils.h"
/*SPR 15896 Fix Stop*/
/******************************************************************************
Private Definitions
*****************************************************************************/

/******************************************************************************
Private Types
*****************************************************************************/
#define RRC_S1AP_SET_MEMBER 1
/* cr_761 macro */
#define MAX_PLMN_ID_VAL        0x09
#define EUTRAN_TRACE_ID_VAL_CHECK   3
/*****************************************************************************
* Private Function Prototypes
****************************************************************************/
void  uecc_s1ap_build_criticality_diagnostics_list(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        OSCTXT                              *p_asn1_ctx,
        s1ap_CriticalityDiagnostics_IE_List *p_list,
        s1ap_error_ind_ie_list_t              *p_iE_list);
 void uecc_fill_error_indication_values(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_ProcedureCode proc_code,
        s1ap_TriggeringMessage triggering_msg,
        s1ap_Criticality proc_criticality);
void fill_cause_for_error_indication_message(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_TriggeringMessage triggering_msg,
        U16 proc_code,
        rrc_s1ap_error_indication_t *p_error_indication);

rrc_return_et verify_erab_id(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_E_RAB_ID *p_e_RAB_ID, 
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
rrc_return_et verify_erab_qos_params(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_E_RABLevelQoSParameters *p_qos,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
rrc_return_et verify_tnl_address(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_TransportLayerAddress *p_tnl,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
rrc_return_et verify_nas_pdu(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_NAS_PDU *p_nas,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
rrc_return_et verify_cause(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_Cause *p_value,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
void log_uecc_error_indication_message(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        rrc_s1ap_error_indication_t *p_err_ind);


rrc_return_et verify_gtp_teid(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_GTP_TEID *p_gtp_teid,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);

/* cr_761 function */
rrc_return_et check_plmn_id_validity(U8 index, U8 octateValue);
rrc_return_et check_lac_validity(U8 index, U8 octateValue);
/* lipa start */
rrc_return_et verify_correlation_id(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_Correlation_ID *p_correlation_id,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id);
/* lipa end */
/******************************************************************************
* Private Constants
*****************************************************************************/
#define MAXBITRATE 10000000000ull /*To remove warning,typecasting has been done*/

/******************************************************************************
* Exported Variables
*****************************************************************************/

/*****************************************************************************
* Private Variables (Must be declared static)
*****************************************************************************/

static U8 plmn_count;
static U8 lac_count;
static U8 trace_count;


/****************************************************************************
* Function Name  : uecc_s1ap_build_and_send_err_indication
* Inputs         : uecc_gb_context_t           *p_uecc_gb_context
*                  rrc_module_id_t             dest_module_id
*                  U8                          mme_id
* Outputs        : p_buff (ASN.1 encoded message),
*                  p_buff_size (size of result buffer)
* Returns        : RRC_SUCCESS / RRC_FAILURE
* Description    : packs p_error_indication into S1AP_PDU ASN
****************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_err_indication
(
 uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
 rrc_module_id_t             dest_module_id,
 U8                          mme_id,
 rrc_s1ap_error_indication_t *p_error_indication
)
{
    OSCTXT asn1_ctx;
    U8 asn_buff[S1AP_MAX_ASN1_BUF_LEN];
    rrc_return_et result = RRC_FAILURE;
    U8 p_message[sizeof(s1ap_ue_associated_sig_msg_ind_t) +
        S1AP_MAX_ASN1_BUF_LEN] = {0};

    s1ap_ue_associated_sig_msg_req_t* p_sig_msg_req =
        (s1ap_ue_associated_sig_msg_req_t *)p_message;
    LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT *p_event_s1_error_ind = PNULL;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    memset_wrapper(asn_buff, 0, S1AP_MAX_ASN1_BUF_LEN);
    memset_wrapper(p_sig_msg_req, RRC_NULL, sizeof(s1ap_ue_associated_sig_msg_req_t));

    p_sig_msg_req->mme_id = mme_id;
    p_sig_msg_req->bitmask |= UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT;
    if (PNULL != p_error_indication)
    {
        p_sig_msg_req->mme_ue_s1ap_id = p_error_indication->mme_ue_s1ap_id;
    }

    do
    {
        /* Its a Transfer Syntax Error */
        if (PNULL == p_error_indication)
        {
            p_sig_msg_req->procedure_code = ASN1V_s1ap_id_ErrorIndication;

           RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                   p_uecc_gb_context->facility_name,
                    RRC_INFO, "Encoding Transfer Syntax Error");
            result = rrc_uecc_error_indication_tse_intrl_enc(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size);

            if (RRC_FAILURE == result)
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING, "Unable to Encode Transfer Syntax Error");
            }

            break;
        }
        else
        {

            p_sig_msg_req->procedure_code = ASN1V_s1ap_id_ErrorIndication;

            /* Its a Abstract Syntax Error */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "Encoding Abstract Syntax Error");

            result = rrc_uecc_error_indication_ase_intrl_enc(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    p_error_indication);
            if (RRC_SUCCESS == result)
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len);
                    p_event_header->event_id = LOCAL_S1_ERROR_INDICATION;
                    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

                    if ((PNULL != p_uecc_gb_context->p_p_csc_context) &&
                        (PNULL != p_uecc_gb_context->p_p_csc_context[p_sig_msg_req->cell_index])&&
                        (PNULL != p_uecc_gb_context->p_p_csc_context[p_sig_msg_req->cell_index]->
                             p_csc_init_setup_ind))
                    {
                        l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_uecc_gb_context->
                                p_p_csc_context[p_sig_msg_req->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                                cell_access_related_info.cell_Id, sizeof(U32));
                    }
                    if (p_error_indication->bitmask & RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT)
                    {
                        p_event_header->EVENT_PARAM_MMES1APID = p_error_indication->mme_ue_s1ap_id;
                    }

                    if (p_error_indication->bitmask & RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT)
                    {
                        p_event_header->EVENT_PARAM_RAC_UE_REF = p_error_indication->enb_ue_s1ap_id;
                    }

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }
        }

    } while (0);

    if (RRC_SUCCESS == result)
    {
        result = uecc_rrc_send_internal_msg(
                dest_module_id,
                S1AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(s1ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                p_message);

		RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
						 p_uecc_gb_context->facility_name,
						 RRC_INFO,
						 "s1ap msg: error indication %s.",
						 change_data_to_str(p_sig_msg_req->message_buff_size,p_sig_msg_req->message_buff_p));


        if(RRC_FAILURE == result)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                    "failure sending message.");
        }
        else
        {
            /* Generating Local Event LOCAL_EVENT_S1_ERROR_INDICATION */ 
            p_event_s1_error_ind = rrc_mem_get(sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT));
            if (PNULL != p_event_s1_error_ind)
            {
                memset_wrapper(p_event_s1_error_ind, RRC_NULL, 
                        sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT));

                p_event_s1_error_ind->header.length = sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT);
                p_event_s1_error_ind->header.event_id = LOCAL_EVENT_S1_ERROR_INDICATION;

                p_event_s1_error_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                if((PNULL != p_error_indication) && (p_error_indication->bitmask &
                            RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT))
                {
                    p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = (U32)p_error_indication->cause.t;

			        /* CSR 51447 START */
                    switch(p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_s1ap_Cause_radioNetwork: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.radioNetwork;
                            
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                NOTIFICATION, "cause type = %d\n and cause value = %d",
								(U32)p_error_indication->cause.t, p_error_indication->cause.u.radioNetwork);
                            break;

                        case T_s1ap_Cause_transport: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.transport;
                            
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                NOTIFICATION, "cause type = %d\n and cause value = %d",
								(U32)p_error_indication->cause.t, p_error_indication->cause.u.transport);
                            break;

                        case T_s1ap_Cause_nas: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.nas;
                            
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                NOTIFICATION, "cause type = %d\n and cause value = %d",
								(U32)p_error_indication->cause.t, p_error_indication->cause.u.nas);
                            break; 

                        case T_s1ap_Cause_protocol:
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.protocol;
                            
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                NOTIFICATION, "cause type = %d\n and cause value = %d",
								(U32)p_error_indication->cause.t, p_error_indication->cause.u.protocol);
                            break; 

                        case T_s1ap_Cause_misc:
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.misc;
                            
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                NOTIFICATION, "cause type = %d\n and cause value = %d",
								(U32)p_error_indication->cause.t, p_error_indication->cause.u.misc);
                            break; 
                    }
	                /* CSR 51447 STOP */
                }

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, 
                        p_event_s1_error_ind);
            }
        }
    }
    else
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Unable to encode the ERROR Indication"
                " message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_validate_ie_value
 * Inputs         : uecc_gb_context_t           *p_uecc_gb_context 
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 *                  void *p_value
 *                  uecc_ue_context_t  *p_ue_context
 * Outputs        : None
 * Returns        : RRC_FAILURE
 * Description    : Class -2 Errors 
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
rrc_return_et uecc_validate_ie_value(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id,
        void *p_value,
        uecc_ue_context_t  *p_ue_context)
{
    rrc_return_et result = RRC_SUCCESS;
    s1ap_HandoverRestrictionList *p_s1ap_HandoverRestrictionList;
    OSRTDListNode *p_node, *p_node_1 = PNULL;
    U8 counter = 0;
    U32 count = 0;
    U32 map_index = 0;
    U8 offset_count = 0;
    U32 orig_index = 0;
    s1ap_E_RABList_element*  p_erab_elem = PNULL;
    s1ap_E_RABToBeSwitchedULList_element* p_erab_switch_elem = PNULL;

    rrc_bool_et flag = RRC_FALSE;

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
                    if ((s1ap_optional == p_ie_order_map->msg_map[count].presence)
                            && (RRC_NULL == p_ie_order_map->msg_map[count].occurances))
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
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING, "[EI] Duplicate Detected");
                uecc_update_message_map(p_uecc_gb_context,
                        p_ie_order_map, UECC_WRONG_ORDER, map_index, id);
            }
        }
        else
        {
            uecc_update_message_map(p_uecc_gb_context,
                    p_ie_order_map, UECC_WRONG_ORDER, map_index, id);
            uecc_update_message_map(p_uecc_gb_context,
                    p_ie_order_map, UECC_OCCURANCE, map_index, id);
            flag = RRC_TRUE;
        }

        if (RRC_TRUE == flag)
        {
            result = RRC_FAILURE;
            /* Entry found break out of the while*/
            break;
        }

        /* If wrong order is not detected and the IE is optional, update
         * the rest of the index */ 
        if (PNULL == p_value)
        {
            uecc_update_message_map(p_uecc_gb_context,
                    p_ie_order_map, UECC_DATA_MISSING, order_index, id);
            break;
        }

        switch(id)
        {
            case ASN1V_s1ap_id_MME_UE_S1AP_ID:
            case ASN1V_s1ap_id_MME_UE_S1AP_ID_2:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (MAX_MME_UE_S1AP_ID < *(s1ap_MME_UE_S1AP_ID *)p_value)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%d Invalid MAX_MME_UE_S1AP_ID",
                                *(s1ap_MME_UE_S1AP_ID *)p_value);

                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    else
                    {
                        /* Update in the MAP */
                        /* set the bitmask */
                        p_ie_order_map->bitmask |= 
                            RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
                        p_ie_order_map->mme_ue_s1ap_id = *(s1ap_MME_UE_S1AP_ID *)p_value;
                    }

                    break;
                }

            case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (MAX_ENB_UE_S1AP_ID < *(s1ap_ENB_UE_S1AP_ID *)p_value)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%d Invalid MAX_MME_UE_S1AP_ID",
                                *(s1ap_ENB_UE_S1AP_ID *)p_value);

                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    else
                    {
                        /* Update in the Map */
                        /* set the bitmask */
                        p_ie_order_map->bitmask |= 
                            RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
                        p_ie_order_map->mme_ue_s1ap_id = *(s1ap_ENB_UE_S1AP_ID *)p_value;
                    }

                    break;
                }

            case ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionListResAck:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    /* Since its a list type we'll check and update 
                     * the value while parsing the list */

                    break;
                }

            case ASN1V_s1ap_id_CriticalityDiagnostics:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    /* Since this IE is of ignore type and every element is of 
                     * Ignore TYPE not processing this message further. 
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    break;
                }

            case ASN1V_s1ap_id_Cause:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (RRC_FAILURE == verify_cause(p_uecc_gb_context,
                                (s1ap_Cause *)p_value, p_ie_order_map,
                                order_index, id))
                    {
                        result = RRC_FAILURE;
                    }

                    break;
                }

           case ASN1V_s1ap_id_RequestType:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (((s1ap_RequestType *)p_value)->eventType > 
                            s1ap_stop_change_of_serve_cell)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid RequestType: eventType");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, 
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    if (((s1ap_RequestType *)p_value)->reportArea > s1ap_ecgi)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid RequestType: reportArea");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, 
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }

            case ASN1V_s1ap_id_HandoverRestrictionList:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    p_s1ap_HandoverRestrictionList = (s1ap_HandoverRestrictionList *) p_value;

                    if ( ENUM_NOT_VALID == p_s1ap_HandoverRestrictionList->forbiddenInterRATs)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%d Invalid forbiddenInterRATs",
                                p_s1ap_HandoverRestrictionList->forbiddenInterRATs);
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }

                    /* Other fields of Sequence need not to checked. If they are invalid
                     * then decoding of message will fail and that will be case of TSE*/
                     
                    break;
                }

           case ASN1V_s1ap_id_HandoverType:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (*(s1ap_HandoverType *)p_value > s1ap_gerantolte)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid HandoverType %d", *(s1ap_HandoverType *)p_value);
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, 
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }

           case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    /* cr_743 UMBR check  */
                    if ((((s1ap_UEAggregateMaximumBitrate *)p_value)->
                                uEaggregateMaximumBitRateDL > MAXBITRATE) || 
                            (((s1ap_UEAggregateMaximumBitrate *)p_value)->
                             uEaggregateMaximumBitRateUL > MAXBITRATE) ||
                            (( RRC_NULL == ((s1ap_UEAggregateMaximumBitrate *)p_value)->
                               uEaggregateMaximumBitRateDL) && 
                             (RRC_NULL == ((s1ap_UEAggregateMaximumBitrate *)p_value)->
                              uEaggregateMaximumBitRateUL)))
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid ASN1V_s1ap_id_uEaggregateMaximumBitrate");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }

           case ASN1V_s1ap_id_SRVCCOperationPossible:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (*((s1ap_SRVCCOperationPossible*)p_value) > 
                            s1ap_possible)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid ASN1V_s1ap_id_SRVCCOperationPossible");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }
/* cr_761 plmn and lac check */

            case ASN1V_s1ap_id_RegisteredLAI:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    //check for plmn_id
                    for(plmn_count = 0 ; plmn_count < ((s1ap_LAI *)(p_value))->
                    pLMNidentity.numocts; plmn_count++)
                    {
                        if(RRC_FAILURE == check_plmn_id_validity(plmn_count, 
                        ((s1ap_LAI *)(p_value))->pLMNidentity.data[plmn_count]))
                        {
                            uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, 
                                UECC_INVALID_VALUE,order_index, id);
                            result = RRC_FAILURE;
                        }
                    }
                    //check for lac
                    for(lac_count = 0 ; lac_count < ((s1ap_LAI *)(p_value))->
                    lAC.numocts; lac_count++)
                    {
                        if(RRC_FAILURE == check_lac_validity(lac_count,
                           ((s1ap_LAI *)(p_value))->lAC.data[lac_count]))
                        {
                            uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map,
                                UECC_INVALID_VALUE,order_index, id);
                            result = RRC_FAILURE;
                        }
                    }
                    break;
                }
           case ASN1V_s1ap_id_E_RABToBeSetupListBearerSUReq:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                         p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_E_RABToBeSetupListBearerSUReq *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABToBeSetupListBearerSUReq_element *p_elem = 
                            (s1ap_E_RABToBeSetupListBearerSUReq_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!=p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1);

                        if (ASN1V_s1ap_id_E_RABToBeSetupItemBearerSUReq != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, 
                                    "ASN1V_s1ap_id_E_RABToBeSetupItemBearerSUReq != %d",
                                    p_elem->id);

                            uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_erab_qos_params(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RABlevelQoSParameters,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_tnl_address(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->transportLayerAddress,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_nas_pdu(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->nAS_PDU,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                        /* lipa start */
                        if (p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->m.iE_ExtensionsPresent)
                        {
                            p_node_1 = (( s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions * )
                                 &(p_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->iE_Extensions))->head;
                            
                            for (; PNULL != p_node_1 ; p_node_1 = p_node_1->next)
                            {
                                s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions_element  *p_elem_1 =
                                 (s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions_element *)p_node_1->data;
        
                                RRC_ASSERT(PNULL!=p_elem_1);
                                RRC_ASSERT(PNULL!=p_elem_1->extensionValue.u._E_RABToBeSetupItemBearerSUReqExtIEs_1);

                                if (ASN1V_s1ap_id_Correlation_ID != p_elem_1->id)
                                {
                                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                      p_uecc_gb_context->facility_name,
                                      RRC_WARNING, 
                                      "ASN1V_s1ap_id_Correlation_ID != %d",
                                      p_elem_1->id);

                                    uecc_update_message_map(p_uecc_gb_context,
                                     p_ie_order_map, UECC_INVALID_VALUE,
                                     order_index, id);
                                  
                                    result = RRC_FAILURE;
                                    continue;
                               }

                               if (RRC_FAILURE == verify_correlation_id(p_uecc_gb_context,
                                   p_elem_1->extensionValue.u._E_RABToBeSetupItemBearerSUReqExtIEs_1,
                                                               p_ie_order_map, order_index, id))
                                  {
                                      result = RRC_FAILURE;
                                      continue;
                                  }
                            
                            }
 
                        }
                       /* lipa end */
                    }
                    break;
                }
           case ASN1V_s1ap_id_E_RABToBeSetupListCtxtSUReq:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_E_RABToBeSetupListCtxtSUReq *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABToBeSetupListCtxtSUReq_element *p_elem = 
                            (s1ap_E_RABToBeSetupListCtxtSUReq_element*)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!=p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1);

                        if (ASN1V_s1ap_id_E_RABToBeSetupItemCtxtSUReq!= p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, 
                                    "ASN1V_s1ap_id_E_RABToBeSetupItemCtxtSUReq != %d",
                                    p_elem->id);

                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_erab_qos_params(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RABlevelQoSParameters,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_tnl_address(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->transportLayerAddress,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if ((p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->m.nAS_PDUPresent) &&
                                (RRC_FAILURE == verify_nas_pdu(p_uecc_gb_context,
                                                               &p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->nAS_PDU,
                                                               p_ie_order_map, order_index, id)))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                        /* lipa start */
			            if (p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->m.iE_ExtensionsPresent)
                        {
                            p_node_1 = ((s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions*)
                                       &(p_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->iE_Extensions))->head;

                            for ( ; PNULL != p_node_1; p_node_1 = p_node_1->next)
                            {
                                 s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions_element *p_elem_1 = 
                                  (s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions_element*)p_node_1->data;

                                 RRC_ASSERT(PNULL!=p_elem_1);

                                 RRC_ASSERT(PNULL!=p_elem_1->extensionValue.u._E_RABToBeSetupItemCtxtSUReqExtIEs_1);

                                 if (ASN1V_s1ap_id_Correlation_ID!= p_elem_1->id)
                                 {
                                      RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                             p_uecc_gb_context->facility_name,
                                             RRC_WARNING, 
                                    "ASN1V_s1ap_id_Correlation_ID!= %d",
                                    p_elem_1->id);

                                      uecc_update_message_map(p_uecc_gb_context,
                                         p_ie_order_map, UECC_INVALID_VALUE,
                                           order_index, id);
                                          result = RRC_FAILURE;
                                         continue;
                                  }

                                  if (RRC_FAILURE == verify_correlation_id(p_uecc_gb_context,
                                   p_elem_1->extensionValue.u._E_RABToBeSetupItemCtxtSUReqExtIEs_1,
                                                               p_ie_order_map, order_index, id))
                                  {
                                      result = RRC_FAILURE;
                                      continue;
                                  }
                            }
                      }
                      /* lipa end */

                   }
                    break;
                }

           case ASN1V_s1ap_id_E_RABToBeModifiedListBearerModReq:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_E_RABToBeModifiedListBearerModReq *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABToBeModifiedListBearerModReq_element *p_elem = 
                            (s1ap_E_RABToBeModifiedListBearerModReq_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!= p_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1);

                        if (ASN1V_s1ap_id_E_RABToBeModifiedItemBearerModReq != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, 
                                    "ASN1V_s1ap_id_E_RABToBeModifiedItemBearerModReq != %d",
                                    p_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_erab_qos_params(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->e_RABLevelQoSParameters,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_nas_pdu(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->nAS_PDU,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                    }
                    break;
                }
           case ASN1V_s1ap_id_E_RABtoReleaseListHOCmd:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_E_RABList  *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABList_element *p_elem = 
                            (s1ap_E_RABList_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!=p_elem->value.u._E_RABItemIEs_1);

                        if (ASN1V_s1ap_id_E_RABItem != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, 
                                    "ASN1V_s1ap_id_E_RABItem != %d",
                                    p_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABItemIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        /* TRACE_fix */
			/*SPR 15896 Fix Start*/
                        if (PNULL != p_ue_context && p_ue_context->m.traceActivated) 
                        {
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), S1, "\n <ie name=\"E-RABID\">%d</ie>",p_elem->value.u._E_RABItemIEs_1->e_RAB_ID);
			/*SPR 15896 Fix Stop*/
                        }
                        /* TRACE_fix */
                        if (RRC_FAILURE == 
                                verify_cause(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABItemIEs_1->cause,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                    }
                    break;
                }
           case ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_E_RABSubjecttoDataForwardingList *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABSubjecttoDataForwardingList_element *p_elem = 
                            (s1ap_E_RABSubjecttoDataForwardingList_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!= p_elem->value.u._E_RABDataForwardingItemIEs_1);

			/*SPR 15896 Fix Start*/
			if (PNULL != p_ue_context && p_ue_context->m.traceActivated) 
			{
				/* SPR 5905 Start */
				P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), S1,"\n<ie name=\"E-RABID\">%d</ie>",p_elem->value.u._E_RABDataForwardingItemIEs_1->e_RAB_ID);
				/* SPR 5905 End */
			}
			/*SPR 15896 Fix Stop*/


                        if (ASN1V_s1ap_id_E_RABDataForwardingItem != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, 
                                    "ASN1V_s1ap_id_E_RABDataForwardingItem != %d",
                                    p_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABDataForwardingItemIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (p_elem->value.u._E_RABDataForwardingItemIEs_1->m.dL_transportLayerAddressPresent)
                        {
                            if (RRC_FAILURE == 
                                    verify_tnl_address(p_uecc_gb_context,
                                        &p_elem->value.u._E_RABDataForwardingItemIEs_1->dL_transportLayerAddress,
                                        p_ie_order_map, order_index, id))
                            {
                                result = RRC_FAILURE;
                                continue;
                            }
                        }

                        if (p_elem->value.u._E_RABDataForwardingItemIEs_1->m.uL_TransportLayerAddressPresent)
                        {
                            if (RRC_FAILURE == 
                                    verify_tnl_address(p_uecc_gb_context,
                                        &p_elem->value.u._E_RABDataForwardingItemIEs_1->uL_TransportLayerAddress,
                                        p_ie_order_map, order_index, id))
                            {
                                result = RRC_FAILURE;
                                continue;
                            }
                        }

                    }
                    break;
                }

           case ASN1V_s1ap_id_SecurityContext:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (7 < ((s1ap_SecurityContext  *)(p_value))->nextHopChainingCount)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid ASN1V_s1ap_id_SecurityContext");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }
           case ASN1V_s1ap_id_NASSecurityParameterstoE_UTRAN:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (PNULL == ((s1ap_NASSecurityParameterstoE_UTRAN  *)(p_value))->data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid ASN1V_s1ap_id_NASSecurityParameterstoE_UTRAN");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }
           case ASN1V_s1ap_id_Target_ToSource_TransparentContainer_Secondary:
           case ASN1V_s1ap_id_Target_ToSource_TransparentContainer:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (PNULL == ((s1ap_Target_ToSource_TransparentContainer *)(p_value))->data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "Invalid ASN1V_s1ap_id_Target_ToSource_TransparentContainer");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }
           /* cr_592 CSG status check */
           case ASN1V_s1ap_id_CSGMembershipStatus:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    
                    if (s1ap_not_member  < *(s1ap_CSGMembershipStatus *)p_value)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%d Invalid CSGMembershipStatus IE value",
                                *(s1ap_CSGMembershipStatus *)p_value);

                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);

                        result = RRC_FAILURE;
                    }
                    break;
                }

           case ASN1V_s1ap_id_CSG_Id:
           case ASN1V_s1ap_id_SecurityKey:
           case ASN1V_s1ap_id_UE_S1AP_IDs:
           case ASN1V_s1ap_id_Inter_SystemInformationTransferTypeMDT:
           case ASN1V_s1ap_id_MMEname:
           case ASN1V_s1ap_id_ServedGUMMEIs:
           case ASN1V_s1ap_id_RelativeMMECapacity:
           case ASN1V_s1ap_id_GUMMEI_ID:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    break;
                }

           case ASN1V_s1ap_id_eNB_StatusTransfer_TransparentContainer:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    p_node = ((s1ap_ENB_StatusTransfer_TransparentContainer  *)(p_value))->
                        bearers_SubjectToStatusTransferList.head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_Bearers_SubjectToStatusTransferList_element *p_elem = 
                            (s1ap_Bearers_SubjectToStatusTransferList_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!= p_elem->value.u._Bearers_SubjectToStatusTransfer_ItemIEs_1);

                        if (ASN1V_s1ap_id_Bearers_SubjectToStatusTransfer_Item != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING,"ASN1V_s1ap_id_Bearers_SubjectToStatusTransfer_Item != %d",
                                    p_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._Bearers_SubjectToStatusTransfer_ItemIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                    }

                    break;
                }
           case ASN1V_s1ap_id_NASSecurityParametersfromE_UTRAN:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (PNULL == ((s1ap_NASSecurityParametersfromE_UTRAN  *)(p_value))->data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_NASSecurityParametersfromE_UTRAN");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }

           case ASN1V_s1ap_id_UERadioCapability:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if (PNULL == ((s1ap_UERadioCapability  *)(p_value))->data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_UERadioCapability");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }
           case ASN1V_s1ap_id_Source_ToTarget_TransparentContainer:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (PNULL == ((s1ap_Source_ToTarget_TransparentContainer  *)(p_value))->data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_Source_ToTarget_TransparentContainer");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }

           case ASN1V_s1ap_id_TraceActivation:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (s1ap_maximumWithoutVendorSpecificExtension < 
                            ((s1ap_TraceActivation *)(p_value))->traceDepth)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_TraceActivation");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    if (RRC_FAILURE == 
                            verify_tnl_address(p_uecc_gb_context,
                                &(((s1ap_TraceActivation *)(p_value))->traceCollectionEntityIPAddress),
                                p_ie_order_map, order_index, id))
                    {
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                /* cr_726 check for Eutran trace id */

                    for(trace_count = 0; trace_count < EUTRAN_TRACE_ID_VAL_CHECK; 
                       trace_count++)
                    {
                        if(RRC_FAILURE == check_plmn_id_validity(trace_count,
                        ((s1ap_TraceActivation *)(p_value))->
                        e_UTRAN_Trace_ID.data[trace_count]))
                        {
                             uecc_update_message_map(p_uecc_gb_context,
                                 p_ie_order_map,
                                 UECC_INVALID_VALUE,order_index, id);
                             result = RRC_FAILURE;
                        }
                    }
                    break;
                }
           case ASN1V_s1ap_id_UESecurityCapabilities:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if(PNULL == 
                            ((s1ap_UESecurityCapabilities  *)(p_value))->encryptionAlgorithms.data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid encryptionAlgorithms");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    if (PNULL == 
                            ((s1ap_UESecurityCapabilities  *)(p_value))->integrityProtectionAlgorithms.data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid integrityProtectionAlgorithms");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }

           case ASN1V_s1ap_id_NRUESecurityCapabilities:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    if(PNULL == 
                            ((s1ap_NRUESecurityCapabilities  *)(p_value))->nRencryptionAlgorithms.data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid nRencryptionAlgorithms");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }

                    if (PNULL == 
                            ((s1ap_NRUESecurityCapabilities  *)(p_value))->nRintegrityProtectionAlgorithms.data)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid nRintegrityProtectionAlgorithms");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }		   
           case ASN1V_s1ap_id_E_RABToBeSetupListHOReq:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    p_node = ((s1ap_E_RABToBeSetupListHOReq *)(p_value))->head;

                    for ( ; PNULL != p_node; p_node = p_node->next)
                    {
                        s1ap_E_RABToBeSetupListHOReq_element *p_elem = 
                            (s1ap_E_RABToBeSetupListHOReq_element *)p_node->data;
                        RRC_ASSERT(PNULL!=p_elem);
                        RRC_ASSERT(PNULL!=p_elem->value.u._E_RABToBeSetupItemHOReqIEs_1);

                        if (ASN1V_s1ap_id_E_RABToBeSetupItemHOReq != p_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING,"ASN1V_s1ap_id_E_RABToBeSetupItemHOReq != %d",
                                    p_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE,
                                    order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemHOReqIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_erab_qos_params(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemHOReqIEs_1->e_RABlevelQosParameters,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == 
                                verify_tnl_address(p_uecc_gb_context,
                                    &p_elem->value.u._E_RABToBeSetupItemHOReqIEs_1->transportLayerAddress,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                    }

                    break;
                }
           case ASN1V_s1ap_id_NAS_PDU:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (RRC_FAILURE == verify_nas_pdu(p_uecc_gb_context,
                                (s1ap_NAS_PDU *)p_value, p_ie_order_map,
                                order_index, id))
                    {
                        result = RRC_FAILURE;
                    }
                    break;
                }
           case ASN1V_s1ap_id_SubscriberProfileIDforRFP:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (*((s1ap_SubscriberProfileIDforRFP *)p_value) > 256 )
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_SubscriberProfileIDforRFP");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }

           case ASN1V_s1ap_id_CSFallbackIndicator:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);

                    if (*((s1ap_CSFallbackIndicator *)p_value) > s1ap_cs_fallback_high_priority)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING,"Invalid ASN1V_s1ap_id_CSFallbackIndicator");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;
                }
           case ASN1V_s1ap_id_E_RABToBeReleasedList:
                {
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    
                    for (p_node = ((s1ap_E_RABList *)(p_value))->head;
                            PNULL != p_node;
                            p_node = p_node->next)
                    {
                        p_erab_elem = (s1ap_E_RABList_element*)p_node->data;
                        RRC_ASSERT(PNULL != p_erab_elem);
                        RRC_ASSERT(PNULL != p_erab_elem->value.u._E_RABItemIEs_1);
                        if (ASN1V_s1ap_id_E_RABItem != p_erab_elem->id)
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING,
                                    "ASN1V_s1ap_id_E_RABItem ! =p_erab_elem"
                                    "->id (%i)",
                                    p_erab_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,
                                    p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context,
                                    &p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_cause(p_uecc_gb_context,
                                    &p_erab_elem->value.u._E_RABItemIEs_1->cause,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                    }
                    break;
                }

           case ASN1V_s1ap_id_E_RABToBeSwitchedULList:
                {
                    uecc_update_message_map(p_uecc_gb_context, p_ie_order_map, UECC_OCCURANCE, order_index, id);
                    
                    for (p_node = ((s1ap_E_RABToBeSwitchedULList*)(p_value))->head;
                            PNULL != p_node;
                            p_node = p_node->next)
                    {
                        p_erab_switch_elem = (s1ap_E_RABToBeSwitchedULList_element*)p_node->data;
                        RRC_ASSERT(PNULL != p_erab_switch_elem);
                        RRC_ASSERT(PNULL != p_erab_switch_elem->value.u._E_RABToBeSwitchedULItemIEs_1);
                        if (ASN1V_s1ap_id_E_RABToBeSwitchedULItem!=
                                p_erab_switch_elem->id)
                        {
                            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                 p_uecc_gb_context->facility_name,RRC_WARNING,
                                    "ASN1V_s1ap_id_E_RABToBeSwitchedULItem !  =p_erab_switch_elem"
                                    "->id (%i)",
                                    p_erab_switch_elem->id);
                            uecc_update_message_map(p_uecc_gb_context,p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE == verify_erab_id(p_uecc_gb_context, &p_erab_switch_elem->value.
                        u._E_RABToBeSwitchedULItemIEs_1->e_RAB_ID,
                                p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }

                        if (RRC_FAILURE ==
                                verify_tnl_address(p_uecc_gb_context, &p_erab_switch_elem->value.
                                u._E_RABToBeSwitchedULItemIEs_1->transportLayerAddress,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                        if (RRC_FAILURE == verify_gtp_teid(p_uecc_gb_context, &p_erab_switch_elem->value.
                        u._E_RABToBeSwitchedULItemIEs_1->gTP_TEID,
                                    p_ie_order_map, order_index, id))
                        {
                            result = RRC_FAILURE;
                            continue;
                        }
                    }
                    break;
                }
/* CDMA + CSFB Changes start: */
	case ASN1V_s1ap_id_cdma2000HOStatus:
	{
		uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                 if (*((s1ap_Cdma2000HOStatus *)p_value) > s1ap_hOFailure)
                 {
                	 RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        	 p_uecc_gb_context->facility_name,
                         	RRC_WARNING,"Invalid ASN1V_s1ap_id_cdma2000HOStatus");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
			break;
	}
	case ASN1V_s1ap_id_cdma2000RATType:
	{
		uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id);
                
		if (*((s1ap_Cdma2000RATType* )p_value) > s1ap_onexRTT )
                 {
                	 RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        	 p_uecc_gb_context->facility_name,
                         	RRC_WARNING,"Invalid ASN1V_s1ap_id_cdma2000RATType");
                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE,
                                order_index, id);
                        result = RRC_FAILURE;
                    }
			break;
	}
	case ASN1V_s1ap_id_cdma2000PDU:
	{
		uecc_update_message_map(p_uecc_gb_context,
                	  p_ie_order_map, UECC_OCCURANCE, order_index, id);
		RRC_ASSERT(PNULL != ((s1ap_Cdma2000PDU*)p_value)->data);
        	
		break;      
	}
 
/* CDMA + CSFB Changes end: */
            case ASN1V_s1ap_id_ManagementBasedMDTAllowed:
                {
                    /* The procedure code is different from the Map*/
                    uecc_update_message_map(p_uecc_gb_context,
                            p_ie_order_map, UECC_OCCURANCE, order_index, id); 

                    if (s1ap_allowed < 
                            *(s1ap_ManagementBasedMDTAllowed *)p_value)
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%d Invalid ManagementBasedMDTAllowed IE value",
                                *(s1ap_ManagementBasedMDTAllowed *)p_value);

                        uecc_update_message_map(p_uecc_gb_context,
                                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);

                        result = RRC_FAILURE;
                    }

                    break;
                }
            default:
                {
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING, "%s: Unknow IE ", __FUNCTION__);
                    result = RRC_FAILURE;
                }
        }
    }
    while(0);

    return result;
}

/****************************************************************************
* Function Name  : uecc_add_to_err_ind_ie_list
* Inputs         : uecc_gb_context_t           *p_uecc_gb_context
*                  s1ap_error_ind_ie_list_t *p_ie_list
*                  s1ap_Criticality iECriticality
*                  s1ap_ProtocolIE_ID iE_ID
*                  U16 *p_index
*                  uecc_s1ap_error_ind_bool_t *p_send_error_indication
*                  rrc_bool_et ismissing
*                  rrc_bool_et bypass_ignore_check
* Outputs        : None
* Returns        : None
* Description    : This Function Sends the error indication msg if there is 
*                  notify IE added to the list 
****************************************************************************/
void uecc_add_to_err_ind_ie_list(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        s1ap_error_ind_ie_list_t *p_ie_list,
        s1ap_Criticality iECriticality,
        s1ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        uecc_s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et ismissing,
        rrc_bool_et bypass_ignore_check)
{
    RRC_ASSERT(PNULL != p_ie_list);
    RRC_ASSERT(PNULL != p_index);

    /* Ignore for criticality Ignore */
    if ((s1ap_ignore != iECriticality) || (RRC_TRUE == bypass_ignore_check))
    {
        if (S1AP_MAX_IES <=*p_index)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "%s: Invalid Index %d", __FUNCTION__, *p_index);
            return;
        }
        p_ie_list->ie_cnt++;
        p_ie_list->iE_list[*p_index].iECriticality = iECriticality;
        p_ie_list->iE_list[*p_index].iE_ID = iE_ID;

        /* set the value according to the added IE, to keep track
         * if there is any notify IE Added to the list we have to 
         * send the Error Indication Message to MME. */
        if (s1ap_notify == iECriticality)
        {
            p_send_error_indication->send_err_indication_notify_ie_present = RRC_TRUE;
        }
        else
        {
            p_send_error_indication->send_err_indication_reject_ie_present = RRC_TRUE;
        }

        if (RRC_TRUE == ismissing)
        {
            p_ie_list->iE_list[*p_index].typeOfError = UECC_ASN_MISSING;
        }

        else
        {
            p_ie_list->iE_list[*p_index].typeOfError =
                UECC_ASN_NOT_UNDERSTOOD;
        }

        *p_index = (U16)(*p_index + 1);
    }
    else
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid IE encountered with Criticality Ignored");
    }
}

/****************************************************************************
* Function Name  : uecc_update_message_map
* Inputs         : uecc_s1ap_message_map_t *p_msg_map = PNULL
*                  U8 count = 0
*                  rrc_bool_et match_found = RRC_FALSE
*                  rrc_return_et result = RRC_SUCCESS
*                  p_msg_map = p_ie_order_map->msg_map
* Outputs        : None
* Returns        : RRC_FAILURE/RRC_SUCCESS
* Description    : Class -2 Errors
****************************************************************************/
rrc_return_et uecc_update_message_map
(
 uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
 uecc_s1ap_message_data_t *p_ie_order_map,
 uecc_map_updation_const_et update_type,
 U32 order_index,
 U16 id
)
{
    uecc_s1ap_message_map_t *p_msg_map = PNULL;
    U8 count = 0;
    rrc_bool_et match_found = RRC_FALSE;
    rrc_return_et result = RRC_SUCCESS;

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
                match_found = RRC_TRUE;
                break;
            }
        }

        if (RRC_TRUE != match_found)
        {
            result = RRC_FAILURE;
            /* Do not update the Map */
            break;
        }

    switch(update_type)
    {
        case UECC_OCCURANCE:
            {
                /* Occurance can also tell the Missing parameters */
                /* Since IE Order check has passed,
                 * We need not check here */
                p_msg_map[order_index].occurances++;
                break;
            }

        case UECC_WRONG_ORDER:
            {
                p_msg_map[order_index].wrong_order++;
                break;
            }

        case UECC_INVALID_VALUE:
            {
                p_msg_map[order_index].invalid_value_present++;
                break;
            }

        case UECC_DATA_MISSING:
            {
                p_msg_map[order_index].data_missing++;
                break;
            }

            default:
                {
                    result = RRC_FAILURE;
                    /* Do Nothing */
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING, "Invalid Operation Value Given");
                }
        }
    } while(0);
    return result;
}

/****************************************************************************
* Function Name  :  uecc_parse_message_map
* Inputs         :  uecc_gb_context_t   *p_uecc_gb_context, 
                    OSCTXT *p_asn1_ctx,
                    U8 mme_id,
                    uecc_s1ap_message_data_t *p_msg_map,
                    s1ap_error_ind_ie_list_t *p_ie_list,
                    U16* p_index_to_update,
                    uecc_s1ap_error_ind_bool_t *p_send_error_indication,
                    U16 proc_code,
                    U8 triggering_msg,
                    U8 proc_criticality     
                    rrc_s1ap_error_indication_t *p_error_indication            
* Outputs        :  None
* Returns        :  RRC_SUCCESS
* Description    : This Function triggers local error handling
****************************************************************************/
rrc_return_et uecc_parse_message_map(
        uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
        OSCTXT *p_asn1_ctx,
        U8 mme_id,
        uecc_s1ap_message_data_t *p_msg_map,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        uecc_s1ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_s1ap_error_indication_t *p_error_indication)
{
    U8 count = 0;
    rrc_s1ap_error_indication_t error_indication;
    rrc_return_et result = RRC_FAILURE;
    rrc_bool_et send_cd_in_resp = RRC_FALSE;
    rrc_bool_et bypass_ignore_check = RRC_FALSE;

    RRC_ASSERT(PNULL != p_msg_map);
    RRC_ASSERT(PNULL != p_ie_list);



    do 
    {
        /* If its a response message terminate the procedure 
         * and there is no Notify IE Added to the list
         * trigger local Error Handling */
        if (RRC_FALSE == p_send_error_indication->send_err_indication_notify_ie_present) 
        {
            /* Find Errors in Successful and unsuccessful response messages*/
            if (T_S1AP_PDU_initiatingMessage != triggering_msg)
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_BRIEF, "%s: [EI] Parseing Response Message", __FUNCTION__);
                /* This is reject case for Unknow IE in 
                 * resonse we have to termiate the procedure */
                if (RRC_TRUE == p_send_error_indication->send_err_indication_reject_ie_present)
                {
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING, "%s: [EI] Unknow IE with Reject Criticality Detected "
                            "in Response MSG, Terminate the procedure and do the local"
                            " Error Handling", __FUNCTION__);
                    return RRC_SUCCESS;
                }

               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_BRIEF, "%s:[EI] Parsing the Known IE in the Response Message", __FUNCTION__);
                for (count = 0; count < p_msg_map->max_count; count++)
                {
                    /* We are checking for Ignore IE Errors*/
                    if (p_msg_map->msg_map[count].criticality == s1ap_ignore)
                    {
                        /* If there are too many occurances of Ignore IE,
                         * We have to terminate the procedure, so return Success */
                        if ((p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT))
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, "%s: [EI] IE ID %d "
                                    "Wrong Order or Too many occurance Detected"
                                    " for Ignore Criticality IE in Response MSG,"
                                    "Terminate the procedure and do the local"
                                    " Error Handling", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return RRC_SUCCESS;
                        }
                    } /* This is the check for Know Reject IE with Invalid value */
                    else if (p_msg_map->msg_map[count].criticality == s1ap_reject)
                    {
                        if ((p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].invalid_value_present >= S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                                ((p_msg_map->msg_map[count].occurances == RRC_NULL) && 
                                 (p_msg_map->msg_map[count].presence == s1ap_mandatory)) ||
                                (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
                        {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_WARNING, "%s: [EI] IE ID %d "
                                    "Detected Error Indication for Reject"
                                    " Criticatlity IE", __FUNCTION__,
                                    p_msg_map->msg_map[count].ie_id);
                            return RRC_SUCCESS;
                        }
                    }
                    else /* This is for the notify, where know IE we will send EI */
                    {
                       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_WARNING, "%s: [EI] IE ID %d "
                                "Detected Error Indication for Notify"
                                " Criticatlity IE, Error Indication should be sent", 
                                __FUNCTION__, p_msg_map->msg_map[count].ie_id);
                        break;
                    }
                }

               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_BRIEF, "%s:[EI] Response message parse Success", 
                        __FUNCTION__);
                return RRC_FAILURE;
            }
            else
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_BRIEF, "%s:[EI] S1AP Initiating Message", __FUNCTION__);
            }

        }
        else
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING,"[EI] Unknow IE with Notify criticality detected,"
                    " Error Indication Should be Sent");
        }
    }while(0);

    if (T_S1AP_PDU_initiatingMessage == triggering_msg)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_BRIEF,"[EI] Parsing Initiating Message");
    }
    else
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO,"[EI] Parsing Resposne Message with "
                "Notify IE Error present");
    }

    /* Update the list, for any error encountered */
    for (count = 0; count < p_msg_map->max_count; count++)
    {
        if ((p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].invalid_value_present >= S1AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                ((p_msg_map->msg_map[count].occurances == RRC_NULL) && 
                 (p_msg_map->msg_map[count].presence == s1ap_mandatory)) ||
                (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
        {

            if (((p_msg_map->msg_map[count].occurances == RRC_NULL) &&
                        (p_msg_map->msg_map[count].presence == s1ap_mandatory))  || 
                    (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
            {
                /* Mandatory Missing or Data missing */
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING, "[EI] %d Mandatory EI/Data missing Detected",
                        p_msg_map->msg_map[count].ie_id);

                uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                        p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        RRC_TRUE,
                        RRC_FALSE);
            }
            else
            {
                if ((p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                /* SPR 13238 Fix Start */
                        (p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT) ||
                         (p_msg_map->msg_map[count].ie_id == 
                                             ASN1V_s1ap_id_E_RABToBeReleasedList))
                /* SPR 13238 Fix End */
                {
                    /* We'll add the IE in the list even if the criticality 
                     * is ignore when there is wrong order or too many
                     * occurances */
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING, "[EI] IE ID %d Wrong Order/Too many occurance for "
                            "Ignore Criticality IE Detected", 
                            p_msg_map->msg_map[count].ie_id);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        RRC_FALSE,
                        RRC_TRUE);

                    bypass_ignore_check = RRC_TRUE;
                }
                else
                {
                    /* Invalid Value, Wrong Order, too many occurances */
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING, "[EI] IE ID %d Wrong Value/Order/Too many occurance for "
                            "Reject/Notify Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            p_ie_list, 
                            p_msg_map->msg_map[count].criticality,
                            p_msg_map->msg_map[count].ie_id,
                            p_index_to_update,
                            p_send_error_indication,
                            RRC_FALSE,
                            RRC_FALSE);
                }
            }
        } 
    }

    /* If there are no error detected for the mandatory fields
     * We can send the CD in response message */
    if ((p_msg_map->msg_map[count].presence == s1ap_mandatory) &&
            (p_msg_map->msg_map[count].occurances == S1AP_IE_PRESENT) &&
            (p_msg_map->msg_map[count].data_missing == RRC_NULL) &&
            (p_msg_map->msg_map[count].wrong_order == RRC_NULL) &&
            ((p_msg_map->unsuccessful_outcome_present == RRC_TRUE) || 
             (p_msg_map->successful_outcome_present == RRC_TRUE)))
    {
        send_cd_in_resp = RRC_TRUE;
    }
    else
    {
        /* Send the Error Indication in case if there is no Unsuccessful 
         * outcome present. If the message has Successful outcome and there
         * are notify IE with Error, We'll send the CD in ACK message */
        if (((p_msg_map->successful_outcome_present == RRC_TRUE) && 
                    (RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)) ||
                (p_msg_map->unsuccessful_outcome_present == RRC_TRUE))
        {
            /* We have to check if Info is sufficent */
            send_cd_in_resp = RRC_TRUE;
        }
        else
        {
            send_cd_in_resp = RRC_FALSE;
        }
    }

    do
    {
        if ((RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present) ||
                (RRC_TRUE == p_send_error_indication->send_err_indication_reject_ie_present) || 
                (RRC_TRUE == bypass_ignore_check))
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
        if (RRC_FALSE == send_cd_in_resp)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "[EI] Building and Sending Error Indication Message");

            memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));
            /* Send the Error Indication from here */
            /* For UE ASSOCIATED MESSAGE the data will be copied
             * autmatically */

            error_indication.bitmask = p_msg_map->bitmask;
            error_indication.mme_ue_s1ap_id = p_msg_map->mme_ue_s1ap_id;
            error_indication.enb_ue_s1ap_id = p_msg_map->enb_ue_s1ap_id;

            uecc_fill_error_indication_values(p_uecc_gb_context,
                    &error_indication,
                    (s1ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            error_indication.bitmask |= RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            uecc_s1ap_build_criticality_diagnostics_list(p_uecc_gb_context,
                    p_asn1_ctx,
                    &error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            uecc_s1ap_build_and_send_err_indication(p_uecc_gb_context,
                    RRC_S1AP_MODULE_ID,mme_id,
                    &error_indication);

            /* If Know IE has notify we have to continue the procedure */
            if (RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_BRIEF, "[EI] Continue the Procedure");
                result = RRC_FAILURE;
            }
            else
            {
                result = RRC_SUCCESS;
            }

            break;
        }
        else /* this case is for message with response messages */
        {
            if (PNULL == p_error_indication)
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_INFO, "p_error_indication Pointer Null");
                break;
            }

           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "[EI]Build Error Indication to be sent in Response");

            p_error_indication->bitmask = p_msg_map->bitmask;
            p_error_indication->mme_ue_s1ap_id = p_msg_map->mme_ue_s1ap_id;
            p_error_indication->enb_ue_s1ap_id = p_msg_map->enb_ue_s1ap_id;

            uecc_fill_error_indication_values(p_uecc_gb_context,
                    p_error_indication,
                    (s1ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            p_error_indication->bitmask |= RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            uecc_s1ap_build_criticality_diagnostics_list(p_uecc_gb_context,
                    p_asn1_ctx,
                    &p_error_indication->criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            if (RRC_TRUE ==  p_send_error_indication->send_err_indication_reject_ie_present)
            {
                result = RRC_SUCCESS;
            }
            else
            {
                result = RRC_FAILURE;
            }
            break;
        }
    }
    while(0);

    return result;
}

/****************************************************************************
* Function Name  : uecc_fill_error_indication_values
* Inputs         : uecc_gb_context_t           *p_uecc_gb_context
*                  rrc_s1ap_error_indication_t *p_err_ind
*                  s1ap_ProcedureCode proc_code
*                  s1ap_TriggeringMessage triggering_msg
*                  s1ap_Criticality proc_criticality
*                 
* Outputs        : None
* Returns        : None
* Description    : This function provides error indication values
****************************************************************************/
void uecc_fill_error_indication_values(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_ProcedureCode proc_code,
        s1ap_TriggeringMessage triggering_msg,
        s1ap_Criticality proc_criticality)
{
    p_err_ind->criticality_diagnostics.m.procedureCodePresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCode = proc_code;

    p_err_ind->criticality_diagnostics.m.triggeringMessagePresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.triggeringMessage = triggering_msg -1;
    p_err_ind->criticality_diagnostics.m.procedureCriticalityPresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCriticality = proc_criticality;

    p_err_ind->bitmask |= RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT;

    fill_cause_for_error_indication_message(p_uecc_gb_context,
            triggering_msg, proc_code, p_err_ind);
}

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_criticality_diagnostics_list 
*   INPUT        : uecc_gb_context_t           *p_uecc_gb_context
*                  OSCTXT                              *p_asn1_ctx
*                  s1ap_CriticalityDiagnostics_IE_List *p_list
*                  s1ap_error_ind_ie_list_t              *p_iE_list
    OUTPUT       : None
    RETURNS      : None
*   DESCRIPTION  : This function processes builds the criticality diagnostics for Abstract
*                  Syntax Error.
******************************************************************************/
void  uecc_s1ap_build_criticality_diagnostics_list(
        uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
        OSCTXT                              *p_asn1_ctx,
        s1ap_CriticalityDiagnostics_IE_List *p_list,
        s1ap_error_ind_ie_list_t              *p_iE_list
)
{

    U8 count = RRC_NULL;
    OSRTDListNode* p_node = PNULL;
    s1ap_CriticalityDiagnostics_IE_Item *p_elem = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_asn1_ctx);

    if (PNULL == p_iE_list)
    {
        return;
    }

    asn1Init_s1ap_CriticalityDiagnostics_IE_List(p_list);

    for(count =0; count < p_iE_list->ie_cnt;count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                s1ap_CriticalityDiagnostics_IE_Item,
                &p_node,
                &p_elem);

        if (PNULL==p_node)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,"Can't allocate p_node");
        }
        else
        {

            asn1Init_s1ap_CriticalityDiagnostics_IE_Item(p_elem);

            p_elem->m.iE_ExtensionsPresent = 0;

            p_elem->iECriticality  = p_iE_list->iE_list[count].iECriticality;
            p_elem->iE_ID          = p_iE_list->iE_list[count].iE_ID;
            p_elem->typeOfError    = p_iE_list->iE_list[count].typeOfError;

            rtxDListAppendNode(p_list,p_node);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return;
}

/****************************************************************************
* Function Name  : build_and_send_error_indication_unknown_proc
* Inputs         : uecc_gb_context_t          *p_uecc_gb_context
*                  U8                          mme_id
*                  s1ap_ProcedureCode          proc_code
*                  s1ap_Criticality            proc_criticality
*                   s1ap_TriggeringMessage     triggering_msg
* Outputs        : None
* Returns        : RRC_SUCCESS / RRC_FAILURE
* Description    : packs p_error_indication into S1AP_PDU ASN
****************************************************************************/
rrc_return_et build_and_send_error_indication_unknown_proc(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        U8 mme_id, 
        s1ap_ProcedureCode proc_code,
        s1ap_Criticality proc_criticality,
        s1ap_TriggeringMessage triggering_msg
        )
{
    rrc_s1ap_error_indication_t  error_indication;
    rrc_return_et result = RRC_SUCCESS;

    RRC_UT_TRACE_ENTER();

    memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));

    error_indication.bitmask = 0;

    uecc_fill_error_indication_values(p_uecc_gb_context,
            &error_indication,
            proc_code,
            triggering_msg,
            proc_criticality);

    error_indication.bitmask |= RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

   if (RRC_SUCCESS != uecc_s1ap_build_and_send_err_indication(p_uecc_gb_context,
               RRC_S1AP_MODULE_ID,mme_id,
                &error_indication))
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "%s : Failed", __FUNCTION__);
        result = RRC_FAILURE;
    }

    return result; 
}

/****************************************************************************
* Function Name  : fill_cause_for_error_indication_message
* Inputs         : uecc_gb_context_t          *p_uecc_gb_context
*                  s1ap_TriggeringMessage      triggering_msg
*                  U16                         procedure_code
*                  rrc_s1ap_error_indication_t *p_error_indication
*                  p_ue_context_release_complete
* Outputs        : p_buff (ASN.1 encoded message),
*                  p_buff_size (size of result buffer) 
* Returns        : RRC_SUCCESS / RRC_FAILURE
* Description    : packs p_error_indication into S1AP_PDU ASN
****************************************************************************/
void fill_cause_for_error_indication_message(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_TriggeringMessage triggering_msg,
        U16 procedure_code,
        rrc_s1ap_error_indication_t *p_error_indication)
{
    switch(triggering_msg)
    {
        case T_S1AP_PDU_initiatingMessage:
            {
                switch(procedure_code)
                {
                    case ASN1V_s1ap_id_downlinkNASTransport:
                    case ASN1V_s1ap_id_InitialContextSetup:
                    case ASN1V_s1ap_id_MMEStatusTransfer:
                    case ASN1V_s1ap_id_E_RABSetup:
                    case ASN1V_s1ap_id_E_RABModify:
                    case ASN1V_s1ap_id_UEContextModification:
                    case ASN1V_s1ap_id_E_RABRelease:
                    case ASN1V_s1ap_id_LocationReportingControl:
                        {
                            if ((RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT & 
                                        p_error_indication->bitmask) &&
                                    !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                        p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = s1ap_unknown_enb_ue_s1ap_id;
                            }
                            else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                        p_error_indication->bitmask) &&
                                    (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                     p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = s1ap_unknown_mme_ue_s1ap_id;
                            }
                            else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                        p_error_indication->bitmask) &&
                                    !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                        p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = s1ap_unknown_pair_ue_s1ap_id;
                            }
                            else 
                            {
                                p_error_indication->cause.t = T_s1ap_Cause_protocol;
                                p_error_indication->cause.u.protocol =
                                    s1ap_abstract_syntax_error_falsely_constructed_message;
                            }

                            break;
                        }

                    case ASN1V_s1ap_id_HandoverResourceAllocation:
                        {
                            if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                        p_error_indication->bitmask))
                            {
                                p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                                p_error_indication->cause.u.radioNetwork = s1ap_unknown_mme_ue_s1ap_id;
                            }

                            break;
                        }
                    case ASN1V_s1ap_id_UEContextRelease:
                    default:
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_protocol;
                            p_error_indication->cause.u.protocol =
                                s1ap_abstract_syntax_error_falsely_constructed_message;
                        }
                }

                break;
            }
        case T_S1AP_PDU_successfulOutcome:
        {
            switch (procedure_code)
            {
                case ASN1V_s1ap_id_HandoverPreparation:
                case ASN1V_s1ap_id_HandoverCancel:
                    {
                        if ((RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT & 
                                    p_error_indication->bitmask) &&
                                !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_enb_ue_s1ap_id;
                        }
                        else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                 p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_mme_ue_s1ap_id;
                        }
                        else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_pair_ue_s1ap_id;
                        }
                        else
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_protocol;
                            p_error_indication->cause.u.protocol =
                                s1ap_abstract_syntax_error_falsely_constructed_message;
                        }

                        break;
                    }
                default:
                    {
                        p_error_indication->cause.t = T_s1ap_Cause_protocol;
                        p_error_indication->cause.u.protocol =
                            s1ap_abstract_syntax_error_falsely_constructed_message;
                    }
            }

            break;
        }
        case T_S1AP_PDU_unsuccessfulOutcome:
        {
            switch (procedure_code)
            {
                case ASN1V_s1ap_id_HandoverPreparation:
                    {
                        if ((RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT & 
                                    p_error_indication->bitmask) &&
                                !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_enb_ue_s1ap_id;
                        }
                        else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                 p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_mme_ue_s1ap_id;
                        }
                        else if (!(RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask) &&
                                !(RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT &
                                    p_error_indication->bitmask))
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_radioNetwork;
                            p_error_indication->cause.u.radioNetwork = s1ap_unknown_pair_ue_s1ap_id;
                        }
                        else
                        {
                            p_error_indication->cause.t = T_s1ap_Cause_protocol;
                            p_error_indication->cause.u.protocol =
                                s1ap_abstract_syntax_error_falsely_constructed_message;
                        }

                        break;
                    }

                default:
                    {
                        p_error_indication->cause.t = T_s1ap_Cause_protocol;
                        p_error_indication->cause.u.protocol =
                            s1ap_abstract_syntax_error_falsely_constructed_message;
                    }
            }

            break;
        }
        default:
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING, "Invalid Triggering Message");
            }
    }
}

/****************************************************************************
* Function Name  : log_uecc_criticality_diagostics 
* Inputs         : uecc_gb_context_t          *p_uecc_gb_context
*                  s1ap_CriticalityDiagnostics *p_criticality_diagostics
* Outputs        : None
* Returns        : None
* Description    : Class -2 Errors 
*                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
****************************************************************************/
void log_uecc_criticality_diagostics(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_CriticalityDiagnostics *p_criticality_diagostics)
{
    s1ap_CriticalityDiagnostics_IE_Item* p_item = PNULL;
    OSRTDListNode* p_node = PNULL;
    U8 count = RRC_NULL;

    RRC_UT_TRACE_ENTER();

   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
            p_uecc_gb_context->facility_name,
            RRC_INFO, "[CRITICALITY DIAGNOSTICS]");

    RRC_ASSERT(PNULL!=p_criticality_diagostics);

    if (p_criticality_diagostics->m.procedureCodePresent)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name,
                RRC_INFO,"[CD] ProcedureCode = %d", 
                p_criticality_diagostics->procedureCode);
    }

    if (p_criticality_diagostics->m.triggeringMessagePresent)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO,"[CD] TriggeringMessage = %d", 
                p_criticality_diagostics->triggeringMessage);
    }

    if (p_criticality_diagostics->m.procedureCriticalityPresent)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO,"[CD] ProcedureCriticality = %d", 
                p_criticality_diagostics->procedureCriticality);
    }

    if (p_criticality_diagostics->m.iEsCriticalityDiagnosticsPresent)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO,"[CD] iEsCriticalityDiagnostics" );

        p_node = p_criticality_diagostics->iEsCriticalityDiagnostics.head;
        for (count = 0; count < p_criticality_diagostics->iEsCriticalityDiagnostics.count;
                count++)
        {       
            p_item = (s1ap_CriticalityDiagnostics_IE_Item*)p_node->data;

            if (PNULL == p_item) 
            {       
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_INFO, "Pointer NULL");
                break;  
            }       

           RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "------iE_Criticality %d", p_item->iECriticality);
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "------iE_ID %d", p_item->iE_ID);

            switch (p_item->typeOfError)
            {       
                case 0: 
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,  
                            p_uecc_gb_context->facility_name,
                            RRC_INFO, "------TypeOfError: not-understood",
                            p_item->iECriticality);
                    break;
                case 1:
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                            p_uecc_gb_context->facility_name,
                            RRC_INFO, "------TypeOfError: missing",
                            p_item->iECriticality);
                    break;
                default:
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_INFO, "Unknwon Type of Error");
            }
        }
    }

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : log_uecc_error_indication_message 
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  rrc_s1ap_error_indication_t *p_err_ind
 * Outputs        : None
 * Returns        : None
 * Description    : Class -2 Errors 
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
void log_uecc_error_indication_message(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        rrc_s1ap_error_indication_t *p_err_ind)
{
    RRC_UT_TRACE_ENTER();

   RRC_UECC_TRACE(p_uecc_gb_context->context_index,  
            p_uecc_gb_context->facility_name,
            RRC_INFO, "[ERROR INDICATION MESSAGE]");

    if (RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "[EI] MME UE S1AP ID = %d ", p_err_ind->mme_ue_s1ap_id);
    }

    if (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "[EI] eNB UE S1AP ID = %d ", p_err_ind->enb_ue_s1ap_id);
    }

    if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT & 
            p_err_ind->bitmask)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "[EI] Cause Type: =%d [EI] Cause Value =%d", 
                p_err_ind->cause.t, p_err_ind->cause.u.radioNetwork);
    }

    if (RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_err_ind->bitmask)
    {
        log_uecc_criticality_diagostics(
                p_uecc_gb_context,
                &p_err_ind->criticality_diagnostics);
    }

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : verify_erab_id
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_E_RAB_ID *p_e_RAB_ID
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify erab id
 ****************************************************************************/
rrc_return_et verify_erab_id(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_E_RAB_ID *p_e_RAB_ID, 
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (INVALID_ERAB_ID <= *p_e_RAB_ID)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid ERAB ID %d", *p_e_RAB_ID);
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : verify_erab_qos_params 
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_E_RAB_ID *p_e_RAB_ID
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify qos params
 ****************************************************************************/
rrc_return_et verify_erab_qos_params(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_E_RABLevelQoSParameters *p_qos,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    rrc_return_et result = RRC_SUCCESS;
    do
    {
        if (s1ap_spare == p_qos->allocationRetentionPriority.priorityLevel)
        {
            /* SPR: The values 1-15 are valid. Priority Level 0 must lead to a
             * logical error. */
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "Invalid ARP Priority Level. QCI = %u",
                    p_qos->qCI);
            result = RRC_FAILURE;
        }

        if (RRC_FAILURE == result)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "Invalid QOS Prameter");
            break;
        }


        if (s1ap_may_trigger_pre_emption < 
                p_qos->allocationRetentionPriority.pre_emptionCapability)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "Invalid QOS Prameter");
            result = RRC_FAILURE;
            break;
        }

        if (s1ap_pre_emptable < 
                p_qos->allocationRetentionPriority.pre_emptionVulnerability)
        {
           RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "Invalid QOS Prameter");
            result = RRC_FAILURE;
            break;
        }

        if (p_qos->m.gbrQosInformationPresent)
        {
            if ((MAXBITRATE < p_qos->gbrQosInformation.e_RAB_MaximumBitrateDL) ||
                    (MAXBITRATE < p_qos->gbrQosInformation.e_RAB_MaximumBitrateUL) ||
                    (MAXBITRATE < p_qos->gbrQosInformation.e_RAB_GuaranteedBitrateDL) ||
                    (MAXBITRATE < p_qos->gbrQosInformation.e_RAB_GuaranteedBitrateUL))
            {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING, "Invalid QOS Prameter");
                result = RRC_FAILURE;
                break;
            }
        }

    }while(0);

    if (RRC_FAILURE == result)
    {
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
    }

    return result;
}

/****************************************************************************
 * Function Name  : verify_tnl_address 
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_TransportLayerAddress *p_tnl
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify terminal add
 ****************************************************************************/
rrc_return_et verify_tnl_address(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_TransportLayerAddress *p_tnl,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (PNULL == p_tnl->data)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid TNL Address");
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}


/****************************************************************************
 * Function Name  : verify_nas_pdu
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_NAS_PDU *p_nas
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify nas pdu
 ****************************************************************************/
rrc_return_et verify_nas_pdu(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_NAS_PDU *p_nas,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (PNULL == p_nas->data)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid NAS PDU");
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}
/****************************************************************************
 * Function Name  : verify_gtp_teid
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_GTP_TEID *p_gtp_teid
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify gtp teid
****************************************************************************/
rrc_return_et verify_gtp_teid(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_GTP_TEID *p_gtp_teid,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if (PNULL == p_gtp_teid)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid GTP TEID");
        uecc_update_message_map(p_uecc_gb_context, p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : verify_cause 
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_Cause *p_value
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify gtp teid
 ****************************************************************************/
rrc_return_et verify_cause(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_Cause *p_value,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    rrc_return_et result = RRC_SUCCESS;
    rrc_bool_et flag = RRC_TRUE;
    switch (p_value->t)
    {
        case T_s1ap_Cause_radioNetwork:
            {
                if (p_value->u.radioNetwork > 
                        s1ap_invalid_CSG_Id)
                {
                    flag = RRC_FALSE;
                }
                break;
            }
        case T_s1ap_Cause_transport:
            {
                if (p_value->u.transport > s1ap_unspecified_3)
                {
                    flag = RRC_FALSE;
                }
                break;
            }
        case T_s1ap_Cause_nas:
            {/*CR 101 changes start*/
                if (p_value->u.nas > s1ap_csg_subscription_expiry)/*CR 101 changes stop*/
                {
                    flag = RRC_FALSE;
                }
                break;
            }
        case T_s1ap_Cause_protocol:
            {
                if (p_value->u.protocol > s1ap_unspecified_1)
                {
                    flag = RRC_FALSE;
                }
                break;
            }
        case T_s1ap_Cause_misc:
            {
                if (p_value->u.misc > s1ap_unknown_PLMN)
                {
                    flag = RRC_FALSE;
                }
                break;
            }
        default:
            flag = RRC_FALSE;
    }

    if (RRC_FALSE == flag)
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid Cause Value");
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE, order_index, id);
        result = RRC_FAILURE;
    }

    return result;
}

/* cr_761 plmn check function */

/****************************************************************************
** Function Name  : check_plmn_id_validity
** Inputs         : index
**                  octateValue
** Outputs        : None
** Returns        : rrc_return_et
** Description    : Check the validity of PLMN
*****************************************************************************/
rrc_return_et check_plmn_id_validity(U8 index, U8 octateValue)
{
    rrc_return_et result = RRC_FAILURE;

    switch (index)
    {
        case 0:
        case 2:
            /* Check the first Octate */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID_VAL) &&
                    ((octateValue >> 0x04) <= MAX_PLMN_ID_VAL))
            {
                result = RRC_SUCCESS;
            }
            break;
        case 1:
            /* Check the second Octate */

            /* check for the filler bit at second nibble */
            if (((octateValue & 0x0F) <= MAX_PLMN_ID_VAL) &&
                    (((octateValue >> 0x04) == 0xF) ||
                     ((octateValue >> 0x04) <= MAX_PLMN_ID_VAL)))
            {
                result = RRC_SUCCESS;
            }
            break;
        default:
            {
                result = RRC_FAILURE;
            }
    }
    return result;
}

/****************************************************************************
** Function Name  : check_lac_validity
** Inputs         : index
**                  octateValue
** Outputs        : None
** Returns        : rrc_return_et
** Description    : Check the validity of lAC
*****************************************************************************/
rrc_return_et check_lac_validity(U8 index, U8 octateValue)
{
    rrc_return_et result = RRC_FAILURE;

    switch (index)
    {
        case 0:
            /* Check the first Octate */
            if ((octateValue > 0x00) && (octateValue < 0xFF))
            {
                result = RRC_SUCCESS;
            }
            break;
        case 1:
            /* Check the second Octate */

            if ((octateValue > 0x00) && (octateValue < 0xFE))
            {
                result = RRC_SUCCESS;
            }
            break;
        default:
            {
                result = RRC_FAILURE;
            }
    }
    return result;
}
/* lipa start */
/****************************************************************************
 * Function Name  : verify_correlation_id
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  s1ap_Correlation_ID *p_correlation_id
 *                  uecc_s1ap_message_data_t *p_ie_order_map
 *                  U32 order_index
 *                  U8 id
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function verify correlation id 
 ****************************************************************************/
rrc_return_et verify_correlation_id(
        uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
        s1ap_Correlation_ID *p_correlation_id,
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id)
{
    if ( PNULL == p_correlation_id || RRC_NULL == p_correlation_id->numocts )
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "Invalid Correlation ID");
        uecc_update_message_map(p_uecc_gb_context,
                p_ie_order_map, UECC_INVALID_VALUE,
                order_index, id);
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}
/* lipa end */
