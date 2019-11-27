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
#include "s1ap_error_indication.h"
#include "s1ap_asn_codec.h"
#include "rrc_s1ap_uecc_intf.h"
#include "s1ap_logger.h"
#include "rtxContext.h"
#include "s1ap_mme.h"

#include "rrc_logging.h"
#include "lppa_asn_enc_dec_3gpp.h"
#include "s1ap_lppa_asn_codec.h"

/******************************************************************************
Private Definitions
*****************************************************************************/

/******************************************************************************
Private Types
*****************************************************************************/
#define RRC_S1AP_SET_MEMBER 1
/*****************************************************************************
* Private Function Prototypes
****************************************************************************/
void add_to_err_ind_ie_list(
        s1ap_error_ind_ie_list_t *p_ie_list,
        s1ap_Criticality iECriticality,
        s1ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et ismissing,
        rrc_bool_et bypass_ignore_check);

void  s1ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        s1ap_CriticalityDiagnostics_IE_List *p_list,
        s1ap_error_ind_ie_list_t              *p_iE_list);
 void fill_error_indication_values(rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_ProcedureCode proc_code,
        s1ap_TriggeringMessage triggering_msg,
        s1ap_Criticality proc_criticality);
 
rrc_return_et lppa_build_and_send_err_indication(
        mme_context_t           *p_mme_context,
        rrc_lppa_error_indication_t  *p_error_indication,
        U16 *p_stream_id);

void  lppa_build_criticality_diagnostics_list(
        OSCTXT                               *p_asn1_ctx,
        lppa_CriticalityDiagnostics_IE_List  *p_list,
        lppa_error_ind_ie_list_t             *p_iE_list);
 
/******************************************************************************
* Private Constants
*****************************************************************************/

/******************************************************************************
* Exported Variables
*****************************************************************************/

/*****************************************************************************
* Private Variables (Must be declared static)
*****************************************************************************/

/****************************************************************************
 * Function Name  : s1ap_build_and_send_err_indication
 * Inputs         : p_mme_context - pointer to MME context 
 *                  p_error_indication - pointer to 
 *                                        rrc_s1ap_error_indication_t
 *                  p_stream_id - pointer to stream id
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function builds and sends error indicatiion to MME
****************************************************************************/
rrc_return_et s1ap_build_and_send_err_indication
(
  mme_context_t           *p_mme_context,
  rrc_s1ap_error_indication_t *p_error_indication,
  U16 *p_stream_id
)
{
    OSCTXT asn1_ctx;
    U8 asn_buff[S1AP_MAX_ASN1_BUF_LEN];
    U16 asn_msg_len = 0;
    rrc_return_et result = RRC_FAILURE;
    LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT *p_event_s1_error_ind = PNULL;

    RRC_ASSERT(PNULL != p_mme_context);


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    memset_wrapper(asn_buff, 0, S1AP_MAX_ASN1_BUF_LEN);

    do
    {
        /* Its a Transfer Syntax Error */
        if (PNULL == p_error_indication)
        {
            RRC_S1AP_TRACE(RRC_INFO, "Encoding Transfer Syntax Error");
            result = rrc_s1ap_error_indication_tse_intrl_enc(&asn1_ctx,
                    asn_buff,
                    &asn_msg_len);
            if (RRC_FAILURE == result)
            {
                RRC_S1AP_TRACE(RRC_WARNING, "Unable to Encode Transfer Syntax Error");
            }

            break;
        }

        /* Its a Abstract Syntax Error */
        RRC_S1AP_TRACE(RRC_INFO, "Encoding Abstract Syntax Error");

        result = rrc_s1ap_error_indication_ase_intrl_enc(&asn1_ctx,
                asn_buff,
                &asn_msg_len,
                p_error_indication);

    }while (0);

    if (RRC_SUCCESS == result)
    {
            result = send_msg_to_mme(p_mme_context, asn_buff, asn_msg_len,
                     p_stream_id);

        if( RRC_SUCCESS != result)
        {
            RRC_S1AP_TRACE(RRC_ERROR,
                    "Unable to send message to MME %d",
                    p_mme_context->mme_id);
        }
        else
        {
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
                    p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = p_error_indication->cause.t;

		            /* CSR 51447 START */
                    switch(p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_s1ap_Cause_radioNetwork: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.radioNetwork;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t,
                                    p_error_indication->cause.u.radioNetwork);
                            break;

                        case T_s1ap_Cause_transport: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.transport;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t,
                                    p_error_indication->cause.u.transport);
                            break;

                        case T_s1ap_Cause_nas: 
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.nas;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t,
                                    p_error_indication->cause.u.nas);
                            break; 

                        case T_s1ap_Cause_protocol:
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.protocol;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t,
                                    p_error_indication->cause.u.protocol);
                            break; 

                        case T_s1ap_Cause_misc:
                            p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
                                p_error_indication->cause.u.misc;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    p_error_indication->cause.t,
                                    p_error_indication->cause.u.misc);
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
        RRC_S1AP_TRACE(RRC_WARNING, "Unable to encode the ERROR Indication"
                " message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
    return result;
}


/****************************************************************************
 * Function Name  : validate_ie_value
 * Inputs         : p_ie_order_map - pointer to s1ap message data 
 *                  order_index - order num 
 *                  id - id of ie
 *                  p_value - pointer to value of ie
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates the ie value of passed ie
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
rrc_return_et validate_ie_value(
        s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value)
{
    rrc_return_et result = RRC_SUCCESS;
    s1ap_SONConfigurationTransfer *p_SONConfigurationTransfer;
    U8 counter = 0;
    S16 count = 0;
    U8 map_index = 0;
    U8 offset_count = 0;
    U32 orig_index = 0;

    rrc_bool_et flag = RRC_FALSE;
    rrc_bool_et temp_flag= RRC_TRUE;

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
            }    
        }    
        /* Detect the wrong Order */
        if ((p_ie_order_map->msg_map[map_index].order_num == orig_index) &&
                (p_ie_order_map->msg_map[map_index].ie_id == id))
        {
            /* Duplicate Case */
            if (orig_index > map_index)
            {
                update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            }
        }
        else
        {
            update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            update_message_map(p_ie_order_map, OCCURANCE, map_index, id);
            flag = RRC_TRUE;
        }

        if (RRC_TRUE == flag)
        {
            /* Entry found break out of the while*/
            break;
        }

        /* If wrong order is not detected and the IE is optional, update
         * the rest of the index */ 
        if (PNULL == p_value)
        {
            update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            break;
        }

        switch(id)
        {
            case ASN1V_s1ap_id_OverloadResponse:
                {
                    /* The procedure code is different from the Map*/
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id); 
                    /* CR 776 changes */
                    if (s1ap_reject_delay_tolerant_access < 
                            *(s1ap_OverloadAction *)p_value)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Overload Action IE value",
                                *(s1ap_OverloadAction *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);

                        result = RRC_FAILURE;
                    }

                    break;
                }

            case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (MAX_MME_UE_S1AP_ID < *(s1ap_MME_UE_S1AP_ID *)p_value)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid MAX_MME_UE_S1AP_ID",
                                *(s1ap_MME_UE_S1AP_ID *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
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
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (MAX_ENB_UE_S1AP_ID < *(s1ap_ENB_UE_S1AP_ID *)p_value)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid MAX_MME_UE_S1AP_ID",
                                *(s1ap_ENB_UE_S1AP_ID *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
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
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    /* Since its a list type we'll check and update 
                     * the value while parsing the list */

                    break;
                }

            case ASN1V_s1ap_id_CriticalityDiagnostics:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    /* Since this IE is of ignore type and every element is of 
                     * Ignore TYPE not processing this message further. 
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    break;
                }

            case ASN1V_s1ap_id_Cause:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    switch (((s1ap_Cause *)p_value)->t)
                    {
                        case T_s1ap_Cause_radioNetwork:
                            {
                                if (((s1ap_Cause *)p_value)->u.radioNetwork > 
                                        s1ap_not_supported_QCI_value)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        case T_s1ap_Cause_transport:
                            {
                                if (((s1ap_Cause *)p_value)->u.transport > s1ap_unspecified_3)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        case T_s1ap_Cause_nas:
                            {
                                if (((s1ap_Cause *)p_value)->u.nas > s1ap_unspecified_4)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        case T_s1ap_Cause_protocol:
                            {
                                if (((s1ap_Cause *)p_value)->u.protocol > s1ap_unspecified_1)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        case T_s1ap_Cause_misc:
                            {
                                if (((s1ap_Cause *)p_value)->u.misc > s1ap_unknown_PLMN)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        default:
                            temp_flag = RRC_FALSE;
                    }

                    if (RRC_FALSE == temp_flag)
                    {
                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }

            case ASN1V_s1ap_id_ResetType:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    switch(((s1ap_ResetType *)p_value)->t)
                    {
                        case T_s1ap_ResetType_s1_Interface:
                            {
                                if (((s1ap_ResetType *)p_value)->u.s1_Interface > s1ap_reset_all)
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        case T_s1ap_ResetType_partOfS1_Interface:
                            {
                                /* The internal values would be decoded and chcked by 
                                 * ASN decoder from objsys */
                                if ((PNULL == ((s1ap_ResetType *)p_value)->u.partOfS1_Interface))
                                {
                                    temp_flag = RRC_FALSE;
                                }
                                break;
                            }
                        default:
                            temp_flag = RRC_FALSE;
                    }

                    if (RRC_FALSE == temp_flag)
                    {
                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }

                    break;
                }
            case ASN1V_s1ap_id_TimeToWait:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    /* Since this IE is of ignore type and every element is of 
                     * Ignore TYPE not processing this message further. 
                     * However we'll be able to detect the wrong order,
                     * and Too many occurances*/
                    if (s1ap_v60s < *(s1ap_TimeToWait*)p_value)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid TimeTo Wait ",
                                *(s1ap_TimeToWait *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    break;

                }
            case ASN1V_s1ap_id_Inter_SystemInformationTransferTypeMDT:
            case ASN1V_s1ap_id_MMEname:
            case ASN1V_s1ap_id_ServedGUMMEIs:
            case ASN1V_s1ap_id_UEIdentityIndexValue:
            case ASN1V_s1ap_id_UEPagingID:
            case ASN1V_s1ap_id_RelativeMMECapacity:
            case ASN1V_s1ap_id_TAIList:
	    case ASN1V_s1ap_id_CSG_IdList:
	    case ASN1V_s1ap_id_WarningAreaList:
            case ASN1V_s1ap_id_MessageIdentifier:
            case ASN1V_s1ap_id_SerialNumber:
            case ASN1V_s1ap_id_NumberofBroadcastRequest:
            case ASN1V_s1ap_id_WarningType: 
            case ASN1V_s1ap_id_WarningSecurityInfo:
            case ASN1V_s1ap_id_DataCodingScheme:
            case ASN1V_s1ap_id_WarningMessageContents:
            case ASN1V_s1ap_id_GUMMEIList:
            case ASN1V_s1ap_id_Routing_ID:
            case ASN1V_s1ap_id_LPPa_PDU:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                    /* No logical range defined*/

                    break;
                }

            case ASN1V_s1ap_id_pagingDRX:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (*((s1ap_PagingDRX *)p_value) > s1ap_v256)
                    {
                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    }
                    break;
                }
            case ASN1V_s1ap_id_CNDomain:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (*((s1ap_CNDomain*)p_value) > s1ap_cs)
                    {
                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    }
                    break;
                }

            case ASN1V_s1ap_id_SONConfigurationTransferMCT:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                    p_SONConfigurationTransfer = (s1ap_SONConfigurationTransfer *) p_value;
                    if (T_s1ap_SONInformation_sONInformationRequest == p_SONConfigurationTransfer->sONInformation.t)
                    {
                        if ( ENUM_NOT_VALID ==
                                p_SONConfigurationTransfer->sONInformation.u.sONInformationRequest)
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%d Invalid SON Information Request",
                                    p_SONConfigurationTransfer->sONInformation.u.sONInformationRequest);

                            update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                            result = RRC_FAILURE;


                        } 
                    }

                    else if(T_s1ap_SONInformation_sONInformationReply == p_SONConfigurationTransfer->sONInformation.t)
                    {

                        if ((ASN1V_s1ap_maxnoofeNBX2TLAs < p_SONConfigurationTransfer->sONInformation.u.sONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n) 
                                || (p_SONConfigurationTransfer->sONInformation.u.sONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n == 0)) 
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Number of eNBX2TransportLayerAddresses",  
                                    p_SONConfigurationTransfer->sONInformation.u.sONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n);
                            update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                            result = RRC_FAILURE;

                        }
                    }
                    break;
                }

          case ASN1V_s1ap_id_RepetitionPeriod:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (*((s1ap_RepetitionPeriod*)p_value) > 4095)
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Repetition Period",
                                *(s1ap_RepetitionPeriod *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    break ;
                 }

          case ASN1V_s1ap_id_ExtendedRepetitionPeriod:
                {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if ((*((s1ap_ExtendedRepetitionPeriod*)p_value) > 131071) ||
                        (*((s1ap_ExtendedRepetitionPeriod*)p_value) < 4096))
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Extended Repetition Period",
                                *(s1ap_ExtendedRepetitionPeriod *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                         
                    }
                      break ;
                 }

         case ASN1V_s1ap_id_ConcurrentWarningMessageIndicator:
                 {
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if (s1ap_true__1 < *((s1ap_ConcurrentWarningMessageIndicator*)p_value))
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Concurrent Message Indicator", 
                                *(s1ap_ConcurrentWarningMessageIndicator *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    break ;
                 }
         case ASN1V_s1ap_id_PagingPriority:
                    update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                    if ( *((s1ap_PagingPriority *)p_value) >s1ap_priolevel8 ) 
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Paging Priority", 
                                *(s1ap_PagingPriority *)p_value);

                        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                        result = RRC_FAILURE;
                    }
                    break ;
         case ASN1V_s1ap_id_TrafficLoadReductionIndication:
                    {
                        /* The procedure code is different from the Map*/
                        update_message_map(p_ie_order_map, OCCURANCE, order_index, id); 

                        if (( (*(s1ap_TrafficLoadReductionIndication *)p_value) < 1) || 
                                ((*(s1ap_TrafficLoadReductionIndication *)p_value) > 99 ))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%d Invalid TrafficLoadReductionIndication IE value",
                                    *(s1ap_TrafficLoadReductionIndication *)p_value);

                            update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);

                            result = RRC_FAILURE;
                        }

                        break;
                    }
         case ASN1V_s1ap_id_MMERelaySupportIndicator:
                    {
                        /* The procedure code is different from the Map*/
                        update_message_map(p_ie_order_map, OCCURANCE, order_index, id); 

                        if ( (*(s1ap_MMERelaySupportIndicator *)p_value) != 0 )  
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%d Invalid MMERelaySupportIndicator IE value",
                                    *(s1ap_MMERelaySupportIndicator *)p_value);

                            update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);

                            result = RRC_FAILURE;
                        }
                    }

                        break;
            default:
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%s, Unknown IE", __FUNCTION__);
                    result = RRC_FAILURE;
                }

        }
    }
    while(0);

    return result;
}

/****************************************************************************
 * Function Name  : validate_lppa_cause
 * Inputs         : p_value - pointer to value for which validation is to be 
 *                            done 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates lppa cause
 ****************************************************************************/
rrc_return_et validate_lppa_cause(lppa_Cause *p_value)
{
    rrc_return_et result = RRC_SUCCESS;
    switch(p_value->t)
    {
        case T_lppa_Cause_radioNetwork:
            if (p_value->u.radioNetwork > 
                   lppa_requested_item_temporarily_not_available)
            {
                result = RRC_FAILURE;
            }
            break;
        case T_lppa_Cause_protocol:
            if (p_value->u.protocol > 
                   lppa_abstract_syntax_error_falsely_constructed_message)
            {
                result = RRC_FAILURE;
            }
            break;
        case T_lppa_Cause_misc:
            if (p_value->u.misc > lppa_unspecified)
            {
                result = RRC_FAILURE;
            }
            break;
        default:
            result = RRC_FAILURE;
    }
    return result;
}

/****************************************************************************
 * Function Name  : lppa_validate_ie_value
 * Inputs         : p_ie_order_map - pointer to lppa_message_data_t
 *                  order_index - order num
 *                  id - id of ie
 *                  p_value - pointer to value passed for validation 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates lppa ie value (Class -2 Errors).
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
rrc_return_et lppa_validate_ie_value(
        lppa_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value)
{
    rrc_return_et result = RRC_SUCCESS;
    U8 counter = 0;
    S16 count = 0;
    U8 map_index = 0;
    U8 offset_count = 0;
    U32 orig_index = 0;

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
                for (count = (S16)(map_index - 1); count >= 0; count--)
                {
                    if ((lppa_optional == p_ie_order_map->msg_map[count].presence)
                            && (RRC_NULL == p_ie_order_map->msg_map[count].occurances))
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
                lppa_update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            }
        }
        else
        {
            lppa_update_message_map(p_ie_order_map, WRONG_ORDER, map_index, id);
            lppa_update_message_map(p_ie_order_map, OCCURANCE, map_index, id);
            flag = RRC_TRUE;
        }

        if (RRC_TRUE == flag)
        {
            /* Entry found break out of the while*/
            break;
        }

        /* If wrong order is not detected and the IE is optional, update
         * the rest of the index */ 
        if (PNULL == p_value)
        {
            lppa_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            break;
        }

        switch(id)
        {
            case ASN1V_lppa_id_E_SMLC_UE_Measurement_ID:
            case ASN1V_lppa_id_eNB_UE_Measurement_ID:
            {
                lppa_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                if ((1 > *(lppa_Measurement_ID *)p_value)  || 
                          (15 < *(lppa_Measurement_ID *)p_value))
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%d Invalid lppa_Measurement_ID",
                            *(lppa_Measurement_ID *)p_value);

                    lppa_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    result = RRC_FAILURE;
                }
                break;
            }
            case ASN1V_lppa_id_ReportCharacteristics:
            {
                lppa_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                if ((LPPA_ONDEMAND != *(lppa_ReportCharacteristics*)p_value) && 
                    (LPPA_PERIODIC != *(lppa_ReportCharacteristics*)p_value))
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%d Invalid lppa_ReportCharacteristics",
                            *(lppa_ReportCharacteristics*)p_value);
                    lppa_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    result = RRC_FAILURE;
                }
                break;
            }
            case ASN1V_lppa_id_MeasurementPeriodicity:
            {
                lppa_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);

                if (LPPA_MSMIN60 < *(lppa_MeasurementPeriodicity*)p_value)
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%d Invalid lppa_MeasurementPeriodicity",
                            *(lppa_MeasurementPeriodicity*)p_value);
                    lppa_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    result = RRC_FAILURE;
                }
                break;
            }
            case ASN1V_lppa_id_Cause:
            {
                lppa_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                if ( RRC_SUCCESS != validate_lppa_cause(((lppa_Cause *)p_value)))
                {
                     lppa_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                     result = RRC_FAILURE;
                }
            }
                break;
            case ASN1V_lppa_id_OTDOA_Information_Type_Group:
            case ASN1V_lppa_id_OTDOA_Information_Type_Item:
            case ASN1V_lppa_id_MeasurementQuantities:
            case ASN1V_lppa_id_MeasurementQuantities_Item:
            {
                lppa_update_message_map(p_ie_order_map, OCCURANCE, order_index, id);
                /* No logical range defined*/
                break;
            }

            default:
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%s, Unknown IE", __FUNCTION__);
                    result = RRC_FAILURE;
                }
        }
    }
    while(0);

    return result;
}

/****************************************************************************
 * Function Name  : update_message_map
 * Inputs         : p_ie_order_map - pointer to s1ap message data 
 *                  update_type - pointer to map_updation_const_et
 *                  order_index - order num 
 *                  id - id of ie
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function updates message map
****************************************************************************/
rrc_return_et update_message_map
(
 s1ap_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,
 U32 order_index,
 U16 id
)
{
    s1ap_message_map_t *p_msg_map = PNULL;
    U8 count = 0;
    rrc_bool_et match_found = RRC_FALSE;
    rrc_return_et result = RRC_SUCCESS;

    p_msg_map = (s1ap_message_map_t *)(p_ie_order_map->msg_map);

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
            /* Do not update the Map */
            result = RRC_FAILURE;
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
                    result=RRC_FAILURE;
                    /* Do Nothing */
                    RRC_S1AP_TRACE(RRC_INFO, "Invalid Operation Value Given");
                }
        }
    } while(0);
    return result;
}

/****************************************************************************
 * Function Name  : lppa_update_message_map
 * Inputs         : p_ie_order_map - pointer to lppa_message_data_t 
 *                  update_type - type of map_updation_const_et
 *                  order_index - order index
 *                  id - id 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function updates message mao lppa (Class -2 Errors).
****************************************************************************/
rrc_return_et lppa_update_message_map
(
 lppa_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,
 U32 order_index,
 U16 id
)
{
    s1ap_message_map_t *p_msg_map = PNULL;
    U8 count = 0;
    rrc_bool_et match_found = RRC_FALSE;
    rrc_return_et result = RRC_SUCCESS;

    p_msg_map = (s1ap_message_map_t *)(p_ie_order_map->msg_map);

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
            /* Do not update the Map */
            result = RRC_FAILURE;
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
                    result=RRC_FAILURE;
                    /* Do Nothing */
                    RRC_S1AP_TRACE(RRC_INFO, "Invalid Operation Value Given");
                }
        }
    } while(0);
    return result;
}

/****************************************************************************
 * Function Name  : add_to_err_ind_ie_list
 * Inputs         : p_ie_list - pointer to s1ap_error_ind_ie_list_t 
 *                  iECriticality  - type of s1ap_Criticality
 *                  iE_ID - s1ap protocol id
 *                  p_index - pointer to index
 *                  p_send_error_indication - pointer to s1ap_error_ind_bool_t
 *                  ismissing - value of type rrc_bool_et
 *                  bypass_ignore_check - value of type rrc_bool_et
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function adds error indication to the ie list
****************************************************************************/
void add_to_err_ind_ie_list(
        s1ap_error_ind_ie_list_t *p_ie_list,
        s1ap_Criticality iECriticality,
        s1ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        s1ap_error_ind_bool_t *p_send_error_indication,
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
            RRC_S1AP_TRACE(RRC_WARNING, "%s: Invalid Index %d", __FUNCTION__, *p_index);
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
        RRC_S1AP_TRACE(RRC_INFO, "Invalid IE encountered with Criticality Ignored");
    }
}

/****************************************************************************
 * Function Name  : parse_message_map
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_mme_context - pointer to MME context
 *                  p_msg_map - pointer to s1ap_message_data_t
 *                  p_ie_list - pointer to s1ap_error_ind_ie_list_t
 *                  p_index_to_update - pointer to index
 *                  p_send_error_indication - pointer to s1ap_error_ind_bool_t
 *                  proc_code - procedure code   
 *                  triggering_msg - msg triggered 
 *                  proc_criticality - criticality 
 *                  p_error_indication - pointer to rrc_s1ap_error_indication_t 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function parses message map.
****************************************************************************/
rrc_return_et parse_message_map(
        OSCTXT *p_asn1_ctx,
        mme_context_t *p_mme_context,
        s1ap_message_data_t *p_msg_map,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        s1ap_error_ind_bool_t *p_send_error_indication,
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
                /* This is reject case for Unknow IE in 
                 * resonse we have to termiate the procedure */
                if (RRC_TRUE == p_send_error_indication->send_err_indication_reject_ie_present)
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] Unknow IE with Reject Criticality Detected "
                            "in Response MSG, Terminate the procedure and do the local"
                            " Error Handling", __FUNCTION__);
                    return RRC_SUCCESS;
                }

                RRC_S1AP_TRACE(RRC_INFO, "%s:[EI] Parsing the Known IE in the Response Message", __FUNCTION__);
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
                            RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
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
                            RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
                                    "Detected Error Indication for Reject"
                                    " Criticatlity IE", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return RRC_SUCCESS;
                        }
                    }
                    else /* This is for the notify, where know IE we will send EI */
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
                                "Detected Error Indication for Notify"
                                " Criticatlity IE, Error Indication should be sent", 
                                __FUNCTION__, p_msg_map->msg_map[count].ie_id);
                        break;
                    }
                }

                RRC_S1AP_TRACE(RRC_INFO, "%s: Response message parse Success", __FUNCTION__);
                return RRC_FAILURE;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s:[EI] S1AP Initiating Message", __FUNCTION__);
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"[EI] Unknow IE with Notify criticality detected,"
                    " Error Indication Should be Sent");
        }
    } while(0);

    if (T_S1AP_PDU_initiatingMessage == triggering_msg)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[EI] Parsing Initiating Message");
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"[EI] Parsing Resposne Message with "
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
                RRC_S1AP_TRACE(RRC_INFO, "[EI] %d Mandatory EI/Data missing Detected",
                        p_msg_map->msg_map[count].ie_id);
                /* Mandatory Missing or Data missing */
                add_to_err_ind_ie_list(p_ie_list, 
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
                        (p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT))
                {
                    /* We'll add the IE in the list even if the criticality 
                     * is ignore when there is wrong order or too many
                     * occurances */
                    RRC_S1AP_TRACE(RRC_INFO, "[EI] IE ID %d Wrong Order/Too many occurance for "
                            "Ignore Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);

                    add_to_err_ind_ie_list(p_ie_list, 
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
                    RRC_S1AP_TRACE(RRC_INFO, "[EI] IE ID %d Wrong Value/Order/Too many occurance for "
                            "Reject/Notify Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);
                    add_to_err_ind_ie_list(p_ie_list, 
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
            RRC_S1AP_TRACE(RRC_INFO, "[EI] Building and Sending Error Indication Message");

            memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));
            /* Send the Error Indication from here */
            /* For UE ASSOCIATED MESSAGE the data will be copied
             * autmatically */

            error_indication.bitmask = p_msg_map->bitmask;
            error_indication.mme_ue_s1ap_id = p_msg_map->mme_ue_s1ap_id;
            error_indication.enb_ue_s1ap_id = p_msg_map->enb_ue_s1ap_id;

            fill_error_indication_values(&error_indication,
                    (s1ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            error_indication.bitmask |= RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            s1ap_build_criticality_diagnostics_list(p_asn1_ctx,
                    &error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            s1ap_build_and_send_err_indication(p_mme_context,
                    &error_indication,
                    PNULL);

            /* If Know IE has notify we have to continue the procedure */
            if (RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)
            {
                RRC_S1AP_TRACE(RRC_INFO, "[EI] Continue the Procedure");
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
                RRC_S1AP_TRACE(RRC_INFO, "p_error_indication Pointer Null");
                break;
            }
            RRC_S1AP_TRACE(RRC_INFO, "[EI]Build Error Indication to be sent in Response");

            p_error_indication->bitmask = p_msg_map->bitmask;
            p_error_indication->mme_ue_s1ap_id = p_msg_map->mme_ue_s1ap_id;
            p_error_indication->enb_ue_s1ap_id = p_msg_map->enb_ue_s1ap_id;

            fill_error_indication_values(p_error_indication,
                    (s1ap_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality);

            p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            p_error_indication->bitmask |= RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            s1ap_build_criticality_diagnostics_list(p_asn1_ctx,
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
 * Function Name  : fill_error_indication_values
 * Inputs         : p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  proc_code - s1ap procedure code
 *                  triggering_msg - type of s1ap_TriggeringMessage 
 *                  proc_criticality - type of s1ap_Criticality
 * Outputs        : None
 * Returns        : None
 * Description    : this function fills error indication values.
****************************************************************************/
void fill_error_indication_values(rrc_s1ap_error_indication_t *p_err_ind,
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

    p_err_ind->cause.t = T_s1ap_Cause_protocol;
    p_err_ind->cause.u.protocol = 
        s1ap_abstract_syntax_error_falsely_constructed_message;
}

/*****************************************************************************
 * Function Name  : s1ap_build_criticality_diagnostics_list 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_list - pointer to s1ap_CriticalityDiagnostics_IE_List
 *                  p_iE_list - pointer to s1ap_error_ind_ie_list_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes builds the criticality diagnostics for 
 *                  Abstract Syntax Error.
 ********************************************************************************/
void  s1ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        s1ap_CriticalityDiagnostics_IE_List *p_list,
        s1ap_error_ind_ie_list_t              *p_iE_list
        )
{

    U8 count = RRC_NULL;
    OSRTDListNode* p_node = PNULL;
    s1ap_CriticalityDiagnostics_IE_Item *p_elem = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

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
            RRC_S1AP_TRACE(RRC_ERROR,"Can't allocate p_node");
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

    RRC_S1AP_UT_TRACE_EXIT();

    return; 
}

/****************************************************************************
 * Function Name  :log_criticality_diagostics 
 * Inputs         : p_criticality_diagostics - pointer to 
 *                                             s1ap_CriticalityDiagnostics 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepare logs for CRITICALITY DIAGNOSTICS 
 *                  procedures
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
void log_criticality_diagostics(
        s1ap_CriticalityDiagnostics *p_criticality_diagostics)
{
    s1ap_CriticalityDiagnostics_IE_Item* p_item = PNULL;
    OSRTDListNode* p_node = PNULL;
    U8 count = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();


    RRC_ASSERT(PNULL!=p_criticality_diagostics);
   
    if (p_criticality_diagostics->m.procedureCodePresent)
    {
        RRC_S1AP_TRACE(RRC_BRIEF,"[CD] ProcedureCode = %d", 
                p_criticality_diagostics->procedureCode);
    }

    if (p_criticality_diagostics->m.triggeringMessagePresent)
    {
        RRC_S1AP_TRACE(RRC_BRIEF,"[CD] TriggeringMessage = %d", 
                p_criticality_diagostics->triggeringMessage);
    }

    if (p_criticality_diagostics->m.procedureCriticalityPresent)
    {
        RRC_S1AP_TRACE(RRC_BRIEF,"[CD] ProcedureCriticality = %d", 
                p_criticality_diagostics->procedureCriticality);
    }

    if (p_criticality_diagostics->m.iEsCriticalityDiagnosticsPresent)
    {
        RRC_S1AP_TRACE(RRC_BRIEF,"[CD] iEsCriticalityDiagnostics" );

        p_node = p_criticality_diagostics->iEsCriticalityDiagnostics.head;
        for (count = 0; count < p_criticality_diagostics->iEsCriticalityDiagnostics.count;
                count++)
        {
            p_item = (s1ap_CriticalityDiagnostics_IE_Item*)p_node->data;

            if (PNULL == p_item)
            {
                RRC_S1AP_TRACE(RRC_INFO, "Pointer NULL");
                break;
            }

            RRC_S1AP_TRACE(RRC_BRIEF, "------iE_Criticality %d iE_ID", p_item->iECriticality, p_item->iE_ID);

            switch (p_item->typeOfError)
            {
                case 0:
                    RRC_S1AP_TRACE(RRC_INFO, "------TypeOfError:%u not-understood", 
                            p_item->iECriticality);
                    break;
                case 1:
                    RRC_S1AP_TRACE(RRC_INFO, "------TypeOfError:%u missing", 
                            p_item->iECriticality);
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_INFO, "Unknwon Type of Error");
            }
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : log_error_indication_message 
 * Inputs         : p_err_ind - pointer to error indication data 
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares logs for error indication messages 
 *                  Class -2 Errors 
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
****************************************************************************/
void log_error_indication_message(rrc_s1ap_error_indication_t *p_err_ind)
{
    RRC_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO, "[ERROR INDICATION MESSAGE]");

    if (RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        RRC_S1AP_TRACE(RRC_INFO, "[EI] MME UE S1AP ID = %d ", p_err_ind->mme_ue_s1ap_id);
    }

    if (RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        RRC_S1AP_TRACE(RRC_INFO, "[EI] eNB UE S1AP ID = %d ", p_err_ind->enb_ue_s1ap_id);
    }

    if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT &
            p_err_ind->bitmask)
    {
        RRC_S1AP_TRACE(RRC_INFO, "[EI] Cause Type: =%d [EI] Cause Value =%d", 
                p_err_ind->cause.t, p_err_ind->cause.u.radioNetwork);
    }

    if (RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_err_ind->bitmask)
    {
        log_criticality_diagostics(&p_err_ind->criticality_diagnostics);
    }

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : lppa_parse_message_map
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  mme_context_t - pointer to MME context 
 *                  p_msg_map - pointer to lppa_message_data_t
 *                  p_ie_list - pointer to lppa_error_ind_ie_list_t
 *                  p_index_to_update - index to update 
 *                  p_send_error_indication - pointer to s1ap_error_ind_bool_t
 *                  proc_code - s1ap procedure code
 *                  triggering_msg - s1ap triggering msg
 *                  proc_criticality - s1ap procedure criticality
 *                  p_error_indication - pointer to rrc_lppa_error_indication_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function parses lppa messagw map 
****************************************************************************/
rrc_return_et lppa_parse_message_map(
        OSCTXT  *p_asn1_ctx,
        mme_context_t *p_mme_context,
        lppa_message_data_t *p_msg_map,
        lppa_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        s1ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_lppa_error_indication_t *p_error_indication)
{
    U8 count = 0;
    rrc_lppa_error_indication_t error_indication;
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
            if (T_lppa_LPPA_PDU_initiatingMessage != triggering_msg)
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] Parsing Response Message", __FUNCTION__); 
                /* This is reject case for Unknow IE in 
                 * resonse we have to termiate the procedure */
                if (RRC_TRUE == p_send_error_indication->send_err_indication_reject_ie_present)
                {
                    RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] Unknow IE with Reject Criticality Detected "
                            "in Response MSG, Terminate the procedure and do the local"
                            " Error Handling", __FUNCTION__);
                    return RRC_SUCCESS;
                }

                RRC_S1AP_TRACE(RRC_INFO, "%s:[EI] Parsing the Known IE in the Response Message", __FUNCTION__);
                for (count = 0; count < p_msg_map->max_count; count++)
                {
                    /* We are checking for Ignore IE Errors*/
                    if (p_msg_map->msg_map[count].criticality == lppa_ignore)
                    {
                        /* If there are too many occurances of Ignore IE,
                         * We have to terminate the procedure, so return Success */
                        if ((p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
                                    "Wrong Order or Too many occurance Detected"
                                    " for Ignore Criticality IE in Response MSG,"
                                    "Terminate the procedure and do the local"
                                    " Error Handling", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return RRC_SUCCESS;
                        }
                    } /* This is the check for Know Reject IE with Invalid value */
                    else if (p_msg_map->msg_map[count].criticality == lppa_reject)
                    {
                        if ((p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].invalid_value_present >= S1AP_IE_PRESENT) ||
                                (p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                                ((p_msg_map->msg_map[count].occurances == RRC_NULL) && 
                                 (p_msg_map->msg_map[count].presence == lppa_mandatory)) ||
                                (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
                                    "Detected Error Indication for Reject"
                                    " Criticatlity IE", __FUNCTION__, 
                                    p_msg_map->msg_map[count].ie_id);
                            return RRC_SUCCESS;
                        }
                    }
                    else /* This is for the notify, where know IE we will send EI */
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "%s: [EI] IE ID %d "
                                "Detected Error Indication for Notify"
                                " Criticatlity IE, Error Indication should be sent", 
                                __FUNCTION__, p_msg_map->msg_map[count].ie_id);
                        break;
                    }
                }

                RRC_S1AP_TRACE(RRC_INFO, "%s: Response message parse Success", __FUNCTION__);
                return RRC_FAILURE;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s:[EI] S1AP Initiating Message", __FUNCTION__);
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"[EI] Unknow IE with Notify criticality detected,"
                    " Error Indication Should be Sent");
        }
    } while(0);

    if (T_lppa_LPPA_PDU_initiatingMessage == triggering_msg)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[EI] Parsing Initiating Message");
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"[EI] Parsing Resposne Message with "
                "Notify IE Error present");
    }

    /* Update the list, for any error encountered */
    for (count = 0; count < p_msg_map->max_count; count++)
    {
        if ((p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].invalid_value_present >= S1AP_IE_PRESENT) ||
                (p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                ((p_msg_map->msg_map[count].occurances == RRC_NULL) && 
                 (p_msg_map->msg_map[count].presence == lppa_mandatory)) ||
                (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
        {

            if (((p_msg_map->msg_map[count].occurances == RRC_NULL) &&
                        (p_msg_map->msg_map[count].presence == lppa_mandatory))  || 
                    (p_msg_map->msg_map[count].data_missing >= S1AP_IE_PRESENT))
            {
                RRC_S1AP_TRACE(RRC_INFO, "[EI] %d Mandatory IE/Data missing Detected",
                        p_msg_map->msg_map[count].ie_id);
                /* Mandatory Missing or Data missing */
                lppa_add_to_err_ind_ie_list(p_ie_list, 
                        p_msg_map->msg_map[count].criticality,
                        p_msg_map->msg_map[count].ie_id,
                        p_index_to_update,
                        p_send_error_indication,
                        RRC_TRUE,
                        RRC_FALSE);
            }
            else
            {
                if((p_msg_map->msg_map[count].wrong_order >= S1AP_IE_PRESENT) ||
                        (p_msg_map->msg_map[count].occurances > S1AP_IE_PRESENT))
                {
                    /* We'll add the IE in the list even if the criticality 
                     * is ignore when there is wrong order or too many
                     * occurances */
                    RRC_S1AP_TRACE(RRC_INFO, "[EI] IE ID %d Wrong Order/Too many occurance for "
                            "Ignore Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);

                    lppa_add_to_err_ind_ie_list(p_ie_list, 
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
                    RRC_S1AP_TRACE(RRC_INFO, "[EI] IE ID %d Wrong Value/Order/Too many occurance for "
                            "Reject/Notify Criticality IE Detected",
                            p_msg_map->msg_map[count].ie_id);
                    lppa_add_to_err_ind_ie_list(p_ie_list, 
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
    if ((p_msg_map->msg_map[count].presence == lppa_mandatory) &&
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
        if(((p_msg_map->successful_outcome_present == RRC_TRUE) && 
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
        if((RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present) ||
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
        if(RRC_FALSE == send_cd_in_resp)
        {
            RRC_S1AP_TRACE(RRC_INFO, "[EI] Building and Sending Error Indication Message");

            memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_lppa_error_indication_t));
            /* Send the Error Indication from here */
            /* For UE ASSOCIATED MESSAGE the data will be copied
             * autmatically */

            error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
            error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
            error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT;
            error_indication.mme_ue_s1ap_id = p_mme_context->lppa_info.mme_ue_s1ap_id;
            error_indication.enb_ue_s1ap_id = p_mme_context->lppa_info.enb_ue_s1ap_id;
            error_indication.routing_id     = p_mme_context->lppa_info.routing_id;

            lppa_fill_error_indication_values(&error_indication,
                    (lppa_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality,
                    (lppa_LPPATransactionID)p_mme_context->lppa_info.lppa_trans_id);

            error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            error_indication.bitmask |= RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            lppa_build_criticality_diagnostics_list(p_asn1_ctx,
                    &error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
                    p_ie_list);

            lppa_build_and_send_err_indication(p_mme_context,
                    &error_indication, PNULL);

            /* If Know IE has notify we have to continue the procedure */
            if(RRC_TRUE == p_send_error_indication->send_err_indication_notify_ie_present)
            {
                RRC_S1AP_TRACE(RRC_INFO, "[EI] Continue the Procedure");
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
                RRC_S1AP_TRACE(RRC_INFO, "p_error_indication Pointer Null");
                break;
            }
            RRC_S1AP_TRACE(RRC_INFO, "[EI]Build Error Indication to be sent in Response");

            p_error_indication->bitmask |= RRC_LPPA_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
            p_error_indication->bitmask |= RRC_LPPA_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
            p_error_indication->bitmask |= RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT;
            p_error_indication->mme_ue_s1ap_id = p_mme_context->lppa_info.mme_ue_s1ap_id;
            p_error_indication->enb_ue_s1ap_id = p_mme_context->lppa_info.enb_ue_s1ap_id;
            p_error_indication->routing_id     = p_mme_context->lppa_info.routing_id;

            lppa_fill_error_indication_values(p_error_indication,
                    (lppa_ProcedureCode)proc_code,
                    triggering_msg,
                    proc_criticality,
                    (lppa_LPPATransactionID)p_mme_context->lppa_info.lppa_trans_id);

            p_error_indication->criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
                RRC_S1AP_SET_MEMBER;

            p_error_indication->bitmask |= RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;

            lppa_build_criticality_diagnostics_list(p_asn1_ctx,
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
 * Function Name  : lppa_fill_error_indication_values
 * Inputs         : p_err_ind - pointer to rrc_lppa_error_indication_t
 *                  proc_code - lppa ProcedureCode
 *                  triggering_msg - lppa TriggeringMessage 
 *                  proc_criticality - lppa Criticality
 *                  lppa_trans_id - lppa LPPATransactionID
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills error indication values for lppa.
****************************************************************************/
void  lppa_fill_error_indication_values(rrc_lppa_error_indication_t *p_err_ind,
        lppa_ProcedureCode proc_code,
        lppa_TriggeringMessage triggering_msg,
        lppa_Criticality proc_criticality,
        lppa_LPPATransactionID lppa_trans_id)
{
    p_err_ind->criticality_diagnostics.m.procedureCodePresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCode = proc_code;

    p_err_ind->criticality_diagnostics.m.triggeringMessagePresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.triggeringMessage = triggering_msg -1;
    p_err_ind->criticality_diagnostics.m.procedureCriticalityPresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.procedureCriticality = proc_criticality;

    p_err_ind->criticality_diagnostics.m.lppatransactionIDPresent = RRC_S1AP_SET_MEMBER;
    p_err_ind->criticality_diagnostics.lppatransactionID = lppa_trans_id;

    p_err_ind->bitmask |= RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT;
    p_err_ind->cause.t = T_lppa_Cause_protocol;
    p_err_ind->cause.u.protocol = 
        lppa_abstract_syntax_error_falsely_constructed_message;
}
/*****************************************************************************
 * Function Name  : lppa_build_criticality_diagnostics_list 
 * Inputs         : p_asn1_ctx - pointer to ASN context 
 *                  p_list - pointer to lppa_CriticalityDiagnostics_IE_List
 *                  p_iE_list - pointer to lppa_error_ind_ie_list_t
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes builds the criticality diagnostics for 
 *                  Abstract Syntax Error.
 ********************************************************************************/
void  lppa_build_criticality_diagnostics_list(
        OSCTXT                               *p_asn1_ctx,
        lppa_CriticalityDiagnostics_IE_List  *p_list,
        lppa_error_ind_ie_list_t             *p_iE_list
        )
{

    U8 count = RRC_NULL;
    OSRTDListNode* p_node = PNULL;
    lppa_CriticalityDiagnostics_IE_List_element  *p_elem = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL!=p_asn1_ctx);

    if (PNULL == p_iE_list)
    {       
        return; 
    }       

    asn1Init_lppa_CriticalityDiagnostics_IE_List(p_list);

    for(count =0; count < p_iE_list->ie_cnt; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                lppa_CriticalityDiagnostics_IE_List_element,
                &p_node,
                &p_elem);

        if (PNULL==p_node)
        {       
            RRC_S1AP_TRACE(RRC_ERROR,"Can't allocate p_node");
        }       
        else    
        {       

            asn1Init_lppa_CriticalityDiagnostics_IE_List_element(p_elem);

            p_elem->m.iE_ExtensionsPresent = 0;

            p_elem->iECriticality  = p_iE_list->iE_list[count].iECriticality;
            p_elem->iE_ID          = p_iE_list->iE_list[count].iE_ID;
            p_elem->typeOfError    = p_iE_list->iE_list[count].typeOfError;

            rtxDListAppendNode(p_list,p_node);
        }       
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return; 
}
/****************************************************************************
 * Function Name  : lppa_build_and_send_err_indication
 * Inputs         : p_mme_context - pointer to MME context
 *                  p_error_indication - pointer to rrc_lppa_error_indication_t
 *                  p_stream_id - pointer to stram id
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function builds and sends lppa erroe indication
****************************************************************************/
rrc_return_et lppa_build_and_send_err_indication
(
  mme_context_t           *p_mme_context,
  rrc_lppa_error_indication_t  *p_error_indication,
  U16 *p_stream_id
)
{
    OSCTXT  asn1_ctx;
    U8      lppa_asn_buff[S1AP_MAX_ASN1_BUF_LEN];
    U16     lppa_asn_msg_len = 0;
    U8      s1ap_asn_buff[S1AP_MAX_ASN1_BUF_LEN];
    U16     s1ap_asn_msg_len = 0;
    rrc_return_et result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_mme_context);


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    memset_wrapper(lppa_asn_buff, 0, S1AP_MAX_ASN1_BUF_LEN);

    do
    {
        /* Its a Transfer Syntax Error */
        if (PNULL == p_error_indication)
        {
            RRC_S1AP_TRACE(RRC_INFO, "Encoding Transfer Syntax Error");
            result = rrc_lppa_error_indication_tse_intrl_enc(&asn1_ctx,
                    lppa_asn_buff,
                    &lppa_asn_msg_len,
                    p_mme_context);
            break;
        }

        /* Its a Abstract Syntax Error */
        RRC_S1AP_TRACE(RRC_INFO, "Encoding Abstract Syntax Error");

        result = rrc_lppa_error_indication_ase_intrl_enc(&asn1_ctx,
                lppa_asn_buff,
                &lppa_asn_msg_len,
                p_error_indication,
                p_mme_context);

    }while (0);

    if (RRC_SUCCESS == result)
    {
        /* build s1ap_pdu based on ue_asso_type*/
        if(UE_ASSO_LPPA == p_mme_context->lppa_info.ue_asso_type)
        {
            result = s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                    p_mme_context->lppa_info.mme_ue_s1ap_id,
                    p_mme_context->lppa_info.enb_ue_s1ap_id,
                    p_mme_context->lppa_info.routing_id,
                    lppa_asn_msg_len, lppa_asn_buff, 
                    &s1ap_asn_msg_len, s1ap_asn_buff);
        }
        else if(NON_UE_ASSO_LPPA == p_mme_context->lppa_info.ue_asso_type)
        {
            result = s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
                    p_mme_context->lppa_info.routing_id,
                    lppa_asn_msg_len, lppa_asn_buff,
                    &s1ap_asn_msg_len, s1ap_asn_buff);
        }

        if (RRC_SUCCESS == result)
        {
            if(RRC_SUCCESS != send_msg_to_mme(p_mme_context, s1ap_asn_buff, 
                                     s1ap_asn_msg_len, p_stream_id))
            {
                RRC_S1AP_TRACE(RRC_ERROR,
                "Unable to send message to MME %d", p_mme_context->mme_id);
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "Unable to encode the S1AP PDU");
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Unable to encode the ERROR Indication"
                " message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
    return result;
}
/****************************************************************************
 * Function Name  : lppa_add_to_err_ind_ie_list
 * Inputs         : p_ie_list - pointer to lppa_error_ind_ie_list_t
 *                  iECriticality - lppa Criticality value
 *                  iE_ID - lppa ProtocolIE ID value
 *                  p_index - pointer to index
 *                  p_send_error_indication - pointer to s1ap_error_ind_bool_t
 *                  ismissing - boolean value
 *                  bypass_ignore_check - boolean value
 * Outputs        : None
 * Returns        : None
 * Description    : This function adds error indication to ie_list for lppa.
****************************************************************************/
void lppa_add_to_err_ind_ie_list(
        lppa_error_ind_ie_list_t  *p_ie_list,
        lppa_Criticality  iECriticality,
        lppa_ProtocolIE_ID  iE_ID,
        U16  *p_index,
        s1ap_error_ind_bool_t  *p_send_error_indication,
        rrc_bool_et  ismissing,
        rrc_bool_et  bypass_ignore_check)
{
    RRC_ASSERT(PNULL != p_ie_list);
    RRC_ASSERT(PNULL != p_index);

    /* Ignore for criticality Ignore */
    if((lppa_ignore != iECriticality) || (RRC_TRUE == bypass_ignore_check))
    {
        if(S1AP_MAX_IES <= *p_index)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: Invalid Index %d", __FUNCTION__, *p_index);
            return;
        }
        p_ie_list->ie_cnt++;
        p_ie_list->iE_list[*p_index].iECriticality = iECriticality;
        p_ie_list->iE_list[*p_index].iE_ID = iE_ID;

        /* set the value according to the added IE, to keep track
         * if there is any notify IE Added to the list we have to 
         * send the Error Indication Message to MME. */
        if (lppa_notify == iECriticality)
        {
            p_send_error_indication->send_err_indication_notify_ie_present = RRC_TRUE;
        }
        else
        {
            p_send_error_indication->send_err_indication_reject_ie_present = RRC_TRUE;
        }

        if (RRC_TRUE == ismissing)
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
        RRC_S1AP_TRACE(RRC_INFO, "Invalid IE encountered with Criticality Ignored");
    }
}

/****************************************************************************
 * Function Name  : lppa_log_criticality_diagostics 
 * Inputs         : p_criticality_diagostics - pointer to 
 *                                         lppa_CriticalityDiagnostics
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes logs for LPPA CRITICALITY DIAGNOSTICS
 *                  errrs [Class -2 Errors].
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
void lppa_log_criticality_diagostics(
        lppa_CriticalityDiagnostics *p_criticality_diagostics)
{
    lppa_CriticalityDiagnostics_IE_List_element* p_item = PNULL;
    OSRTDListNode* p_node = PNULL;
    U8 count = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO, "[LPPA CRITICALITY DIAGNOSTICS]");

    RRC_ASSERT(PNULL != p_criticality_diagostics);
   
    if (p_criticality_diagostics->m.procedureCodePresent)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[CD] ProcedureCode = %d", 
                p_criticality_diagostics->procedureCode);
    }

    if (p_criticality_diagostics->m.triggeringMessagePresent)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[CD] TriggeringMessage = %d", 
                p_criticality_diagostics->triggeringMessage);
    }

    if (p_criticality_diagostics->m.procedureCriticalityPresent)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[CD] ProcedureCriticality = %d", 
                p_criticality_diagostics->procedureCriticality);
    }
    
    if (p_criticality_diagostics->m.lppatransactionIDPresent)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[CD] lppatransactionID = %d", 
                p_criticality_diagostics->lppatransactionID);
    }

    if (p_criticality_diagostics->m.iEsCriticalityDiagnosticsPresent)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[CD] iEsCriticalityDiagnostics" );

        p_node = p_criticality_diagostics->iEsCriticalityDiagnostics.head;
        for (count = 0; count < p_criticality_diagostics->iEsCriticalityDiagnostics.count;
                count++)
        {
            p_item = (lppa_CriticalityDiagnostics_IE_List_element*)p_node->data;

            if (PNULL == p_item)
            {
                RRC_S1AP_TRACE(RRC_INFO, "Pointer NULL");
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "------iE_Criticality %d", p_item->iECriticality);
            RRC_S1AP_TRACE(RRC_INFO, "------iE_ID %d", p_item->iE_ID);

            switch (p_item->typeOfError)
            {
                case 0:
                    RRC_S1AP_TRACE(RRC_INFO, "------TypeOfError:%u lppa_not_understood", 
                            p_item->iECriticality);
                    break;
                case 1:
                    RRC_S1AP_TRACE(RRC_INFO, "------TypeOfError:%u lppa_missing", 
                            p_item->iECriticality);
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_INFO, "Unknwon Type of Error");
            }
            p_node = p_node->next;
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

