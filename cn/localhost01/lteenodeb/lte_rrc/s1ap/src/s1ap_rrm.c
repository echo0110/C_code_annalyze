/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_rrm.c
 *
 ******************************************************************************
 *
 *  File Description : This file contains the definitions of functions handling
 *                     RRM requests.
 *
 ******************************************************************************
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "s1ap_rrm.h"
#include "s1ap_mme_fsm.h"
#include "s1ap_asn_codec.h"
#include "rrc_defines.h"
#include "s1ap_enb_mgr_fsm.h"
#include "rrc_s1apRrm_il_composer.h"
#include "rrc_s1apRrm_il_parser.h"
#include "s1ap_rrm_rim_intl.h"
#include "s1ap_lppa_asn_codec.h"
#include "rrc_cp_common.h"


 /******************************************************************************
   Private Definitions
  *****************************************************************************/

 /******************************************************************************
   Private Types
  *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

 /******************************************************************************
  * Private Constants
  *****************************************************************************/

 /******************************************************************************
  * Exported Variables
  *****************************************************************************/

 /*****************************************************************************
  * Private Variables (Must be declared static)
  *****************************************************************************/
/*SPR 19697 Fix Start*/
/*****************************************************************************
 * Function Name  : s1ap_generate_plmn_identity_for_rim_to_int 
 * Inputs         : p_plmn_identity - pointer to plmn_identity_t
 *                  p_s1ap_plmn_identity - pointer to OSOCTET 
 * Outputs        : p_plmn_identity
 * Returns        : None
 * Description    : This function generates plmn identity
 ******************************************************************************/
void s1ap_generate_plmn_identity_for_rim_to_int(
        plmn_identity_t    *p_plmn_identity,
        OSOCTET                 *p_s1ap_plmn_identity)
{
    /* TS 3GPP TS 24.008 -10.5.5.15 Routing area identification  
     MCC digit 2	MCC digit 1
     MNC digit 3	MCC digit 3
     MNC digit 2	MNC digit 1
     */

    /* MCC should be present always */
    p_plmn_identity->presence_bitmask |=
        PLMN_IDENTITY_MCC_PRESENCE_FLAG;

    /* PLMN Identity 1st octet : Pack first 2 digits of MCC */
    /*SPR 5517 Fix Start */
    p_plmn_identity->mcc[0] = 
        (p_s1ap_plmn_identity[0] & 0x0F);
    p_plmn_identity->mcc[1] =
        (p_s1ap_plmn_identity[0] & 0xF0) >> 4;

    /* PLMN Identity 2nd octet: Pack 3rd digit of MCC */
    p_plmn_identity->mcc[2] = 
	    (p_s1ap_plmn_identity[1] & 0x0F);
    /* if filler digit */
    if ((p_s1ap_plmn_identity[1] & 0xF0) == 0xF0)
    {
	    p_plmn_identity->mnc.count = 2;

	    /* PLMN Identity 3rd octet : 1st and 2nd digit of MNC */
	    p_plmn_identity->mnc.mnc[0] = 
		    (p_s1ap_plmn_identity[2] & 0x0F);
	    p_plmn_identity->mnc.mnc[1] = 
		    (p_s1ap_plmn_identity[2] & 0xF0) >> 4;
    }
    else
    {
	    /* PLMN Identity 3rd octet : Pack 1st digit of MNC */
	    p_plmn_identity->mnc.count = 3;
	    p_plmn_identity->mnc.mnc[0] = 
		    (p_s1ap_plmn_identity[2] & 0x0F);

	    /* PLMN Identity 3rd octet : 2nd and 3rd digit of MNC */
	    p_plmn_identity->mnc.mnc[1] = 
		    (p_s1ap_plmn_identity[2] & 0xF0)>>4;
	    /*SPR 5517 Fix Stop */
	    p_plmn_identity->mnc.mnc[2] = 
		    (p_s1ap_plmn_identity[1] & 0xF0) >> 4;
    }
    /*SPR 5517 Fix Stop */
}
/*SPR 19697 Fix End*/


/*****************************************************************************
  * Function Name  : s1ap_process_rrm_messages
  * Inputs         : p_api - Pointer to input API buffer
  *                  p_s1ap_gb_ctx - Pointer to S1AP global context.
  * Outputs        : None
  * Returns        : None
  * Description    : This function is invoked to handle RRM messages to S1AP.
  ******************************************************************************/
 void s1ap_process_rrm_messages(
         /* Pointer to input API buffer */
         void            *p_api,         
         /* Pointer to the S1AP global context data */
         s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    switch (rrc_get_api_id((U8*)p_api))
    {
        case S1AP_RRM_ENB_DIRECT_INFO_TRANSFER:
            process_s1ap_rrm_enb_direct_info_transfer_msg(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_ENB_CONFIG_TRANSFER:
            process_s1ap_rrm_enb_config_transfer_msg(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_PWS_RESP:
          process_s1ap_rrm_pws_resp_msg(p_api, p_s1ap_gb_ctx);
            break;
        
        case S1AP_RRM_KILL_RESP:
          process_s1ap_rrm_kill_resp_msg(p_api, p_s1ap_gb_ctx);
            break;
	case S1AP_RRM_RIM_INFO_REQ:
	  process_s1ap_rrm_rim_info_req(p_api, p_s1ap_gb_ctx);
	    break;
	case S1AP_RRM_RIM_INFO:
	  process_s1ap_rrm_rim_info();/*SPR 17777 +-*/
	    break;
	case S1AP_RRM_RIM_INFO_ACK:
	  process_s1ap_rrm_rim_info_ack(p_api, p_s1ap_gb_ctx);
	    break;
	case S1AP_RRM_RIM_INFO_ERROR:
	  process_s1ap_rrm_rim_info_error();/*SPR 17777 +-*/
	    break;
	case S1AP_RRM_RIM_INFO_APP_ERROR:
	  process_s1ap_rrm_rim_info_app_error(p_api, p_s1ap_gb_ctx);
	    break;

        case S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP:
            process_s1ap_rrm_lppa_meas_init_resp(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL:
            process_s1ap_rrm_lppa_meas_init_fail(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND:
            process_s1ap_rrm_lppa_meas_fail_ind(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_LPPA_ECID_MEAS_REPORT:
            process_s1ap_rrm_lppa_meas_report(p_api, p_s1ap_gb_ctx);
            break;

        case S1AP_RRM_LPPA_OTDOA_INFO_RESP:
            process_s1ap_rrm_lppa_otdoa_info_resp(p_api, p_s1ap_gb_ctx);
            break;
        
        case S1AP_RRM_LPPA_OTDOA_INFO_FAIL:
            process_s1ap_rrm_lppa_otdoa_info_fail(p_api, p_s1ap_gb_ctx);
            break;
        
        case S1AP_RRM_LPPA_ERROR_IND_REQ:
            process_s1ap_rrm_lppa_error_ind_req(p_api, p_s1ap_gb_ctx);
            break;

        default:
            RRC_S1AP_TRACE(RRC_WARNING,
             "s1ap_process_oam_messages: "
             "unexpected API from OAM module received");
            break;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_enb_config_transfer_msg 
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the MME eNB Config Info Transfer
 *                  msg(S1AP_RRM_ENB_CONFIG_TRANSFER msg )
 ****************************************************************************/
void process_s1ap_rrm_enb_config_transfer_msg(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_config_transfer_t enb_config_transfer;
    rrc_return_et response = RRC_FAILURE;
    S32 length_read = RRC_NULL;
    mme_context_t * p_mme_context = PNULL;
    U8 asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN]  = {0};
    U8 mme_id = 0;

    U16 asn_msg_len  = RRC_NULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&enb_config_transfer,
            RRC_NULL,
            sizeof(s1ap_rrm_enb_config_transfer_t));

    do
    {
        /* Check the state of s1ap */
        if (S1AP_W_FOR_PROV_REQ == p_s1ap_gb_ctx->s1ap_current_state)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP module is not provisioned"); 
            break;
        }

        /* Decode the received message from OAM */
        response = rrc_il_parse_s1ap_rrm_enb_config_transfer(
                &enb_config_transfer,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - 
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        /* Decode failed, break here */
        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "eNB Configuration Transfer decode failed");
            break;
        }

        RRC_S1AP_TRACE(RRC_INFO, "eNB Configuration Transfer decode"
                " success");

        if (RRC_SUCCESS == compose_asn_enb_configuration_transfer(
                    &enb_config_transfer,
                    p_s1ap_gb_ctx,
                    asn_msg_buff,
                    &asn_msg_len))
        {
             /* If decode success, send message to MME */

            mme_id = enb_config_transfer.mme_id;
            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Configuration Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Configuration Tranfer failed", mme_id);
                break;
            }

            /* SPR 2793 Start */
            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Configuration Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Configuration Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_configuration_transfer "
                    "returned failure",__FUNCTION__);
        }

    }while (0);
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_enb_direct_info_transfer_msg
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the MME ENB Direct Info Transfer
 *                  msg.
 ****************************************************************************/
void process_s1ap_rrm_enb_direct_info_transfer_msg(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_direct_info_transfer_t    enb_info_msg;
    rrc_return_et response = RRC_FAILURE;
    S32 length_read = RRC_NULL;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    mme_context_t *p_mme_context = PNULL;
    U8 mme_id = 0;

    U16 asn_msg_len  = RRC_NULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&enb_info_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_enb_direct_info_transfer_t));

    do
    {
        /* Check the state of s1ap */
        if (S1AP_W_FOR_PROV_REQ == p_s1ap_gb_ctx->s1ap_current_state)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP module is not provisioned"); 
            break;
        }

        /* Decode the received message from OAM */
        response = rrc_il_parse_s1ap_rrm_enb_direct_info_transfer(
                &enb_info_msg ,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - 
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        /* Decode failed, break here */
        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "eNB Direct Information Transfer decode failed");
            break;
        }

        RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer decode"
                " success");

        if (RRC_SUCCESS == compose_asn_enb_direct_info_transfer(
                    &enb_info_msg,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            /* If decode success, send message to MME */

            mme_id = enb_info_msg.mme_id;
            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Direct Info Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Direct Info Tranfer failed", mme_id);
                break;
            }

            /* SPR 2793 Start */
            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Direct Information Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_direct_info_transfer "
                    "returned failure",__FUNCTION__);
        }
    } while (0);

    return;
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_pws_resp_msg
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the S1AP_RRM_PWS_RESP  msg 
 ****************************************************************************/
void process_s1ap_rrm_pws_resp_msg(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_pws_response_t    s1ap_rrm_pws_resp_msg;
    S32 length_read = RRC_NULL;
    U8 mme_id = RRC_NULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&s1ap_rrm_pws_resp_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_pws_response_t));
    do
    {
      /* Precondition - p_api should point to right message */
      if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_pws_response(
            &s1ap_rrm_pws_resp_msg,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }
      if (PNULL != p_s1ap_gb_ctx->p_pws_info)
      {    
        mme_id = p_s1ap_gb_ctx->p_pws_info->mme_id;
        /* call the appropriate fsm */
        enb_mgr_fsm_write_replace_warning_process_event(
                ENB_EV_RRM_PWS_RESP,
                p_api,
                &s1ap_rrm_pws_resp_msg,
                p_s1ap_gb_ctx,
                p_s1ap_gb_ctx->mme_contexts[mme_id]);

      }
    } while (0);

    return;
}


/*****************************************************************************
 * Function Name  : process_s1ap_rrm_kill_resp_msg
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes s1ap_rrm_kill_response msg.
****************************************************************************/
void process_s1ap_rrm_kill_resp_msg(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_kill_response_t    s1ap_rrm_kill_resp_msg;
    S32 length_read = RRC_NULL;
    U8 mme_id = RRC_NULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&s1ap_rrm_kill_resp_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_kill_response_t));
    do
    {
      /* Precondition - p_api should point to right message */
      if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_kill_response(
            &s1ap_rrm_kill_resp_msg,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }
      if ( p_s1ap_gb_ctx->p_kill_data != PNULL)
      {    
        mme_id = p_s1ap_gb_ctx->p_kill_data->mme_id;
      }
      else
      {
          mme_id = 0;
      }
      /* call the appropriate fsm */
      enb_mgr_fsm_write_replace_warning_process_event(
          ENB_EV_RRM_KILL_RESP,
          p_api,
          &s1ap_rrm_kill_resp_msg,
          p_s1ap_gb_ctx,
          p_s1ap_gb_ctx->mme_contexts[mme_id]);


    } while (0);

    return;
}

/*****************************************************************************
 * Function Name  : rim_asn_decode_global_enb_id
 * Inputs         : p_elem_source - pointer to asn buffer  
 *                  p_elem_target - pointer to global eNB data 
 *                  length        - length of asn buffer 
 * Outputs        : p_elem_target 
 * Returns        : RRC_FAILURE/RRC_SUCCESS/RRC_PARTIAL_SUCCESS 
 * Description    : This function decodes the Globlal ENB Id and populates 
 *                  in to p_elem_target. 
 ****************************************************************************/
rrc_return_et rim_asn_decode_global_enb_id 
(
 U8    *p_elem_source,
 rim_global_enb_id_t   *p_elem_target,
 U16 length
)
{
    OSCTXT          asn1_ctx;
    rrc_return_et   result = RRC_FAILURE;

    s1ap_Global_ENB_ID    s1ap_global_enb_id ;

    RRC_ASSERT(PNULL != p_elem_source);
    RRC_ASSERT(PNULL != p_elem_target);

    do   
    {    
        /* ASN Context Initializtion
         * */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {    
            RRC_TRACE(RRC_WARNING, "Failed to Initialize ASN context\n");
            break;
        }    
        asn1Init_s1ap_Global_ENB_ID(&s1ap_global_enb_id);
        

        /*Set pointer of asn buffer in asn context*/
        pu_setBuffer(&asn1_ctx, p_elem_source, length, TRUE);

        if (RT_OK != asn1PD_s1ap_Global_ENB_ID(&asn1_ctx, &s1ap_global_enb_id))
        {
            RRC_TRACE(RRC_ERROR, "ASN Decoding Fail for Global ENB ID\n");
            break;
        }
        l3_memcpy_wrapper(p_elem_target->plmn.plmn,
                   s1ap_global_enb_id.pLMNidentity.data,
                   RIM_PLMN_OCTETS);


        /* SPR_7858_fix */
        if (T_s1ap_ENB_ID_macroENB_ID == s1ap_global_enb_id.eNB_ID.t){
            p_elem_target->enb_type = RIM_ENB_TYPE_MACRO_ENB;
            if (RRC_SUCCESS != rrc_cp_unpack_U32(&p_elem_target->enb_id,
                            s1ap_global_enb_id.eNB_ID.u.macroENB_ID->data, "macroENB_ID"))
            {
                RRC_S1AP_TRACE(RRC_WARNING, "Failed to build ENB ID.");
            }
        } else if (T_s1ap_ENB_ID_homeENB_ID  == s1ap_global_enb_id.eNB_ID.t){
            p_elem_target->enb_type = RIM_ENB_TYPE_HOME_ENB;
            if (RRC_SUCCESS != rrc_cp_unpack_U32(&p_elem_target->enb_id,
                            s1ap_global_enb_id.eNB_ID.u.homeENB_ID->data, "homeENB_ID"))
            {
                RRC_S1AP_TRACE(RRC_WARNING, "Failed to build ENB ID.");
            }
        } else {
            RRC_TRACE(RRC_WARNING, "Wrong Cell Type \n");
            break;
        }
        /* SPR_7858_fix */
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    return result;
}


/*****************************************************************************
 * Function Name  : rim_asn_encode_global_enb_id
 * Inputs         : p_elem_source - pointer to global eNB Id data  
 *                  p_elem_target - pointer to cell id of eNB  
 * Outputs        : p_elem_target  
 * Returns        : RRC_FAILURE/RRC_SUCCESS/RRC_PARTIAL_SUCCESS 
 * Description    : This function  ASN encoding  the Globlal ENB Id  
 ****************************************************************************/
rrc_return_et rim_asn_encode_global_enb_id 
(
 rim_global_enb_id_t   *p_elem_source,
 rim_cell_id_intl_t    *p_elem_target
 )
{
    OSCTXT          asn1_ctx;
    rrc_return_et   result = RRC_FAILURE;
    U8              msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U16 length = 0;
    
    s1ap_Global_ENB_ID    s1ap_global_enb_id ;

    RRC_ASSERT(PNULL != p_elem_source);
    RRC_ASSERT(PNULL != p_elem_target);

    memset_wrapper(msg_buf, 0, S1AP_MAX_ASN1_BUF_LEN);
    do
    {
        /* ASN Context Initializtion */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "Failed to Initialize ASN context\n");
            break;
        }

        /* Fill S1ap Global Enb Structure */
        
        asn1Init_s1ap_Global_ENB_ID(&s1ap_global_enb_id);
        s1ap_global_enb_id.pLMNidentity.numocts = RIM_PLMN_OCTETS;

        l3_memcpy_wrapper(s1ap_global_enb_id.pLMNidentity.data,
                   p_elem_source->plmn.plmn,
                   RIM_PLMN_OCTETS);

        /* SPR_7858_fix */
        /* check cell Type */
        if (RIM_ENB_TYPE_MACRO_ENB == p_elem_source->enb_type){
            /* Macro Cell */
            s1ap_global_enb_id.eNB_ID.t =  T_s1ap_ENB_ID_macroENB_ID; 
            if (PNULL == (s1ap_global_enb_id.eNB_ID.u.macroENB_ID =
                          rtxMemAllocType(&asn1_ctx, ASN1BitStr32)))
            {
                RRC_S1AP_TRACE(RRC_WARNING, "Failed to allocate Memmory to Macro Enb Id\n");
                break;
            }
            else
            {
                memset_wrapper (s1ap_global_enb_id.eNB_ID.u.macroENB_ID, 0, MAX_MACRO_CELL_ID_BITS);
                s1ap_global_enb_id.eNB_ID.u.macroENB_ID->numbits = MAX_MACRO_CELL_ID_BITS;

                if (RRC_SUCCESS != rrc_cp_pack_U32(s1ap_global_enb_id.eNB_ID.u.macroENB_ID->data,
                        &p_elem_source->enb_id, "macroENB_ID"))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "Failed to build ENB ID.");
                }
            }
        } else if (RIM_ENB_TYPE_HOME_ENB == p_elem_source->enb_type){
            /* Home ENB Cell */
            s1ap_global_enb_id.eNB_ID.t = T_s1ap_ENB_ID_homeENB_ID; 
            if (PNULL == (s1ap_global_enb_id.eNB_ID.u.homeENB_ID =
                          rtxMemAllocType(&asn1_ctx, ASN1BitStr32))){
                RRC_S1AP_TRACE(RRC_WARNING, "Failed to allocate Memmory to Home Enb Id\n");
                break;
            }
            else
            {
                memset_wrapper (s1ap_global_enb_id.eNB_ID.u.homeENB_ID, 0, MAX_HOME_CELL_ID_BITS);
                s1ap_global_enb_id.eNB_ID.u.homeENB_ID->numbits = MAX_HOME_CELL_ID_BITS;

                if (RRC_SUCCESS != rrc_cp_pack_U32(s1ap_global_enb_id.eNB_ID.u.homeENB_ID->data,
                        &p_elem_source->enb_id, "homeENB_ID"))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "Failed to build ENB ID.");
                }
            }
        }
        else {
            RRC_S1AP_TRACE(RRC_WARNING, "Wrong Cell Type\n");
            break;
        }
        /* SPR_7858_fix */

        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_s1ap_Global_ENB_ID(&asn1_ctx, &s1ap_global_enb_id))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "ASN Encoding Fail for Global ENB ID\n");
            break;
        }

        length = pe_GetMsgLen(&asn1_ctx);
        l3_memcpy_wrapper(p_elem_target->cell_identifier + RIM_PLMN_OCTETS + RIM_TAC_OCTETS, 
                   msg_buf, 
                   length);

        p_elem_target->length = length + RIM_PLMN_OCTETS + RIM_TAC_OCTETS;
        result = RRC_SUCCESS;
    } while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    return result;
}
/*SPR 19183 START*/
/* Code Deleted */
/*SPR 19183 END*/


/*****************************************************************************
 * Function Name  : compose_rim_info_req
 * Inputs         : p_src_msg     - pointer to the RIM INFORMATION req data 
 *                  p_elem_target - pointer to the RAN-INFORMATION-REQUEST
 *                                 data 
 * Outputs        : p_tgt_msg 
 * Returns        : RRC_FAILURE/RRC_SUCCESS/RRC_PARTIAL_SUCCESS 
 * Description    : This function composes the RAN-INFORMATION-REQUEST 
 *                  Info request 
 ****************************************************************************/
rrc_return_et compose_rim_info_req(
        rim_information_req_t * p_src_msg, 
        ran_information_req_rim_t * p_tgt_msg)
{
    RRC_S1AP_UT_TRACE_ENTER();
    rrc_return_et return_val = RRC_SUCCESS;

    p_tgt_msg->pdu_type = RIM_RAN_INFO_REQ_PDU;

    if (p_src_msg->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {

        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                p_src_msg->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_src_msg->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);

        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_GERAN_CELL_OCTETS;
    }
    /* SPR 17078 Fix Start */	 
    if (p_src_msg->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                p_src_msg->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_src_msg->dst_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        
        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_RNC_ID_OCTETS;

    }

    if (p_src_msg->dst_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->dst_cell.eutran_rtng_addr.enb, &p_tgt_msg->dst_cell_id);
    }
    /* SPR 17078 Fix Stop */	 
    
    if (p_src_msg->src_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                p_src_msg->src_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->src_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_src_msg->src_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
        
        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_GERAN_CELL_OCTETS;
    }
    /* SPR 17078 Fix Start */	 
    if (p_src_msg->src_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                p_src_msg->src_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->src_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_src_msg->src_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);

        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_RNC_ID_OCTETS;
    }

    if (p_src_msg->src_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                p_src_msg->src_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                p_src_msg->src_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->src_cell.eutran_rtng_addr.enb, &p_tgt_msg->src_cell_id);
    }
    /* SPR 17078 Fix Stop */	 


    p_tgt_msg->bitmask |= RIM_INFO_REQ_PRESENT;
    p_tgt_msg->rim_req.rim_app_id.rim_app_id = p_src_msg->app_idty;
    if (!(p_src_msg->app_idty)){
        RRC_S1AP_TRACE (RRC_WARNING, "Wrong APP ID \n");
        return RRC_FAILURE;
    }

    p_tgt_msg->rim_req.bitmask |= RIM_INFO_REQ_APP_ID_PRESENT;
    
    p_tgt_msg->rim_req.rim_seq_num.rim_seq_num = p_src_msg->seq_num;
    p_tgt_msg->rim_req.bitmask |= RIM_INFO_REQ_SEQ_NUM_PRESENT;

    p_tgt_msg->rim_req.rim_pdu_ind.rim_pdu_ind = p_src_msg->pdu_ind.pdu_ext;
    p_tgt_msg->rim_req.rim_pdu_ind.rim_pdu_ind <<= 1;
    
    if (RIM_NO_ACK_REQUESTED == p_src_msg->pdu_ind.ack){
        p_tgt_msg->rim_req.rim_pdu_ind.rim_pdu_ind |= RIM_NO_ACK_REQUESTED;
    } else if (RIM_NO_ACK_REQUESTED == p_src_msg->pdu_ind.ack){
        p_tgt_msg->rim_req.rim_pdu_ind.rim_pdu_ind |= RIM_ACK_REQUESTED;
    } else {
        RRC_S1AP_TRACE (RRC_WARNING, "Wrong Ack type \n");
    }
    
    p_tgt_msg->rim_req.bitmask |= RIM_INFO_REQ_PDU_IND_PRESENT;
   
    if (p_src_msg->bitmask & RIM_INFO_REQ_PROTOCOL_VER_PRESENT) {
        p_tgt_msg->rim_req.rim_protocol_ver.rim_protocol_ver = p_src_msg->ver;
    }
     
    if (p_src_msg->bitmask & RIM_INFO_REQ_APP_CONTAINER_PRESENT){

        if (p_src_msg->app_cont.bitmask & RIM_REQ_APP_CONTAINER_NACC_PRESENT)
        {
            l3_memcpy_wrapper(p_tgt_msg->rim_req.app_cont.nacc.cell_identifier,
                    p_src_msg->app_cont.nacc.cell_idty.routing_idty.plmn.plmn,
                    RIM_PLMN_OCTETS                        
                    );
            l3_memcpy_wrapper(((p_tgt_msg->rim_req.app_cont.nacc.cell_identifier + RIM_PLMN_OCTETS)),
                    p_src_msg->app_cont.nacc.cell_idty.routing_idty.lac,
                    RIM_LAC_OCTETS 
                    );
            l3_memcpy_wrapper(((p_tgt_msg->rim_req.app_cont.nacc.cell_identifier + RIM_PLMN_OCTETS+RIM_LAC_OCTETS)),
                    &p_src_msg->app_cont.nacc.cell_idty.routing_idty.rac,
                    sizeof(U8) 
                    );
            l3_memcpy_wrapper(((p_tgt_msg->rim_req.app_cont.nacc.cell_identifier + 
                         RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                     p_src_msg->app_cont.nacc.cell_idty.geran_cell_idty,
                     RIM_GERAN_CELL_OCTETS 
                     );
        p_tgt_msg->rim_req.app_cont.bitmask |= RIM_REQ_APP_CONTAINER_NACC_PRESENT;
        }
        if (p_src_msg->app_cont.bitmask & RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT)
        {
            l3_memcpy_wrapper(p_tgt_msg->rim_req.app_cont.utra_si.cell_identifier,
                    p_src_msg->app_cont.utra_si.rpt_cell_id.plmn.plmn,
                    RIM_PLMN_OCTETS                        
                    );
            l3_memcpy_wrapper(((p_tgt_msg->rim_req.app_cont.utra_si.cell_identifier + RIM_PLMN_OCTETS)),
                    &p_src_msg->app_cont.utra_si.rpt_cell_id.cell_id,
                    sizeof(U32) 
                    );
        p_tgt_msg->rim_req.app_cont.bitmask |= RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT;
        }
        if ((p_src_msg->app_cont.bitmask & RIM_REQ_APP_CONTAINER_SI3_PRESENT) ||
                (p_src_msg->app_cont.bitmask & RIM_REQ_APP_CONTAINER_MBMS_PRESENT) ||
                (p_src_msg->app_cont.bitmask & RIM_REQ_APP_CONTAINER_SON_PRESENT)) {
            RRC_S1AP_TRACE(RRC_WARNING, "Not supported RIM INFO REQ APP CONTAINER.\n");
        }
    } else if (p_src_msg->bitmask & RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT){
            RRC_S1AP_TRACE(RRC_WARNING, "Not supported RIM INFO REQ SON APP IDNTY\n");
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return return_val;
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_rim_info_req
 * Inputs         : p_api - pointer to the input API buffer  
 *                  s1ap_gb_context_t - pointer to s1ap global context 
 * Outputs        : none 
 * Returns        : none  
 * Description    : This function processes rim_information_req msg and sends 
 *                  this msg to MME 
 ****************************************************************************/

void process_s1ap_rrm_rim_info_req(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    rim_information_req_t msg;
    
    ran_information_req_rim_t temp_msg;

    S32 length_read = RRC_NULL;
    
    U32 msg_length = 0;
    /* SPR_17550_changes_start */
    U8 *p_msg = RRC_PNULL;
    /* SPR_17550_changes_start */

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&msg,
            RRC_NULL,
            sizeof(rim_information_req_t));
    memset_wrapper(&temp_msg,
            RRC_NULL,
            sizeof(ran_information_req_rim_t));
    do
    {
      /* Precondition - p_api should point to right message */

      if (RRC_SUCCESS != rrc_il_parse_rim_information_req(
            &msg,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }

      /*convert to internal structure message*/
      if (RRC_FAILURE == compose_rim_info_req(&msg, &temp_msg))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }
    
    /* Allocate buffer */
    p_msg = rrc_mem_get(sizeof (U8) * MAX_BSSGP_PDU_LEN);
    if ( p_msg == PNULL){
        return ;
    }

      /*Encode TLV message*/
    if (RRC_FAILURE == rrc_intl_compose_ran_information_req_rim(&p_msg, &temp_msg, &msg_length))
    {
        rrc_mem_free(p_msg);

        return ;
    }

      /*Send message*/
    send_rim_info_req(p_msg, msg_length, &msg, p_s1ap_gb_ctx);
    }while(0);
    /* SPR_17550_changes_start */
    if (RRC_PNULL != p_msg)
    {
        rrc_mem_free(p_msg);
	  /*Bug 4091 Fix Start*/
        p_msg = PNULL;
	  /*Bug 4091 Fix End*/
    }
    /* SPR_17550_changes_end */
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : send_rim_info_req 
 * Inputs         : p_trg_msg     - pointer to ran_information_req_rim data 
 *                  msg_length    - length of message   
 *                  message       - pointer to rim_information_req data
 *                  p_s1ap_gb_ctx - pointer to S1AP global context
 * Outputs        : None 
 * Returns        : none  
 * Description    : This function fills the RRM ENB direct info msg in to a 
 *                  msssage and sends to  MME
 ****************************************************************************/
void send_rim_info_req(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_information_req_t    *message, 
                    s1ap_gb_context_t        *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_direct_info_transfer_t    enb_info_msg;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    mme_context_t *p_mme_context = PNULL;
    U16 asn_msg_len  = RRC_NULL;
    U8 mme_id=0;
    /*SPR 19708 Fix Start*/
    U16 utran_rncid = RRC_NULL;
    /*SPR 19708 Fix End*/
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&enb_info_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_enb_direct_info_transfer_t));
    if (message->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.ci,
                (const void *)message->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
	    /*SPR 19697 Fix Start*/
	    s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.geran_cell_id.lai.plmn_identity, 
			    /*SPR 19697 Fix End*/
               message->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn);        

        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.lai.location_area_code,
                (const void *)message->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        enb_info_msg.geran_cell_id.routing_area_code = message->dst_cell.geran_rtng_addr.
            routing_idty.rac;
        
        enb_info_msg.bitmask |= GERAN_CELL_ID_PRESENT;
    }
    /* SPR 17078 Fix Start */
    if (message->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        enb_info_msg.target_rnc_id.routing_area_code = message->dst_cell.
            utran_rtng_addr.routing_idty.rac;

        l3_memcpy_wrapper((void *)enb_info_msg.target_rnc_id.lai.location_area_code,
                (const void *)message->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);
        //PLMN //
	/*SPR 19697 Fix Start*/
        s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.target_rnc_id.lai.plmn_identity, 
	/*SPR 19697 Fix End*/
               message->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn);        
        /*SPR 19708 FIX START*/
        utran_rncid = message->dst_cell.utran_rtng_addr.rnc_id[0];
        utran_rncid = utran_rncid << 8;
        utran_rncid =utran_rncid|message->dst_cell.utran_rtng_addr.rnc_id[1];
	if(utran_rncid <= RRC_12BIT_MAX_VALUE)
	{ 
		enb_info_msg.target_rnc_id.rnc_id = utran_rncid;
	}
	else
	{
		enb_info_msg.target_rnc_id.rnc_id = RRC_NULL;
		enb_info_msg.target_rnc_id.extended_rnc_id = utran_rncid;
		enb_info_msg.target_rnc_id.bitmask |= RRM_RNC_EXTENDED_RNC_ID_PRESENT;
        
	}
        /*SPR 19708 FIX END*/
        
        enb_info_msg.target_rnc_id.bitmask |= RRM_RNC_ROUTING_AREA_CODE_PRESENT;
        enb_info_msg.bitmask |= TARGET_RNC_ID_PRESENT; 
    }
    /* SPR 17078 Fix Stop */
    
    enb_info_msg.mme_id = message->mme_id;
    enb_info_msg.len_rim_info = msg_length;
    l3_memcpy_wrapper((void *)enb_info_msg.rim_info, (const void *)p_trg_msg,
           msg_length);

    do
    {

        if (RRC_SUCCESS == compose_asn_enb_direct_info_transfer(
                    &enb_info_msg,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            /* If decode success, send message to MME */

            mme_id = enb_info_msg.mme_id;
            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Direct Info Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Direct Info Tranfer failed", mme_id);
                break;
            }

            /* SPR 2793 Start */
            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Direct Information Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_direct_info_transfer "
                    "returned failure",__FUNCTION__);
        }
    } while (0);

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : process_s1ap_rrm_rim_info 
 * Inputs         : p_api - pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data 
 * Outputs        : none 
 * Returns        : none  
 * Description    : This Function will process RIM INFO Message. 
 ****************************************************************************/
void process_s1ap_rrm_rim_info(void)
    /*SPR 17777 +-*/
{
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_rim_info_error 
 * Inputs         : p_api - pointer to input API buffer  
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data 
 * Outputs        : none 
 * Returns        : none  
 * Description    : This function will process RIM INFO ERROR Message.
 ****************************************************************************/
void process_s1ap_rrm_rim_info_error(void)
    /*SPR 17777 +-*/
{
}

/*****************************************************************************
 * Function Name  : send_rim_info_ack 
 * Inputs         : p_trg_msg - pointer to the ran_information_ack_rim data   
 *                  msg_length - length of message 
 *                  message - pointer to the rim_information_ack data 
 *                  p_s1ap_gb_ctx - pointer to the S1AP global context  
 * Outputs        : None 
 * Returns        : none  
 * Description    : This function composes ENB Direct Info Transfer msg and 
 *                  sends to MME.
 ****************************************************************************/
void send_rim_info_ack(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_information_ack_t    *message, 
                    s1ap_gb_context_t        *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_direct_info_transfer_t    enb_info_msg;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    mme_context_t *p_mme_context = PNULL;
    U16 asn_msg_len  = RRC_NULL;
    U8 mme_id=0;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&enb_info_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_enb_direct_info_transfer_t));
    if (message->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.ci,
                (const void *)message->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);

	/*SPR 19697 Fix Start*/
        s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.geran_cell_id.lai.plmn_identity, 
	/*SPR 19697 Fix End*/
               message->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn);        

        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.lai.location_area_code,
                (const void *)message->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        enb_info_msg.geran_cell_id.routing_area_code = message->dst_cell.geran_rtng_addr.
            routing_idty.rac;
        
        enb_info_msg.bitmask |= GERAN_CELL_ID_PRESENT;

    }
    else if (message->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        enb_info_msg.target_rnc_id.routing_area_code = message->dst_cell.
            utran_rtng_addr.routing_idty.rac;

        l3_memcpy_wrapper((void *)enb_info_msg.target_rnc_id.lai.location_area_code,
                (const void *)message->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);
        //PLMN //
        s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.target_rnc_id.lai.plmn_identity, 
                message->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn);        
        /*coverity 81537 fix start*/
        //enb_info_msg.target_rnc_id.rnc_id = message->dst_cell.utran_rtng_addr.rnc_id[0];
        //enb_info_msg.target_rnc_id.rnc_id = enb_info_msg.target_rnc_id.rnc_id << 8;
        /*coverity 81537 fix end*/
        /*SPR 20553 FIX START */
        enb_info_msg.target_rnc_id.rnc_id = enb_info_msg.target_rnc_id.rnc_id | message->dst_cell.utran_rtng_addr.rnc_id[1];
        /*SPR 20553 FIX START */
        enb_info_msg.target_rnc_id.bitmask |= RRM_RNC_ROUTING_AREA_CODE_PRESENT;
        enb_info_msg.bitmask |= TARGET_RNC_ID_PRESENT; 
    }
    
    enb_info_msg.mme_id = message->mme_id;
    enb_info_msg.len_rim_info = msg_length;
    l3_memcpy_wrapper((void *)enb_info_msg.rim_info, (const void *)p_trg_msg,
           msg_length);

    do
    {

        if (RRC_SUCCESS == compose_asn_enb_direct_info_transfer(
                    &enb_info_msg,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            /* If decode success, send message to MME */

            mme_id = enb_info_msg.mme_id;
            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Direct Info Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Direct Info Tranfer failed", mme_id);
                break;
            }

            /* SPR 2793 Start */
            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Direct Information Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_direct_info_transfer "
                    "returned failure",__FUNCTION__);
        }
    } while (0);

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : compose_rim_info_ack 
 * Inputs         : p_src_msg - pointer to rim_information data  
 *                  p_tgt_msg - pointer to the ran_information_ack RIM data  
 * Outputs        : p_tgt_msg  
 * Returns        : RRC_SUCCESS/RRC_FALURE/RRC_PARTIAL_SUCCESS 
 * Description    : This function composes RRM RIM INFO PDU for Acklowleged 
 *                  mode.
 *****************************************************************************/
rrc_return_et compose_rim_info_ack(
        rim_information_ack_t* p_src_msg, 
        ran_information_ack_rim_t * p_tgt_msg)
{
    RRC_S1AP_UT_TRACE_ENTER();
    rrc_return_et return_val = RRC_SUCCESS;
    
    p_tgt_msg->pdu_type = RIM_RAN_INFO_ACK_PDU;

    if (p_src_msg->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_GERAN_CELL_OCTETS;
    }
    else if (p_src_msg->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->dst_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_RNC_ID_OCTETS;
    }
    else if (p_src_msg->dst_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->dst_cell.eutran_rtng_addr.enb, &p_tgt_msg->dst_cell_id);
    }
    
    if (p_src_msg->src_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->src_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_GERAN_CELL_OCTETS;
    }
    else if (p_src_msg->src_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->src_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_RNC_ID_OCTETS;

    }
    else if (p_src_msg->src_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->src_cell.eutran_rtng_addr.enb, &p_tgt_msg->src_cell_id);
    }


    p_tgt_msg->bitmask |= RIM_INFO_ACK_PRESENT;
    p_tgt_msg->rim_info.rim_app_id.rim_app_id = p_src_msg->app_idty;
    p_tgt_msg->rim_info.bitmask |= RIM_INFO_ACK_APP_ID_PRESENT;
    
    p_tgt_msg->rim_info.rim_seq_num.rim_seq_num = p_src_msg->seq_num;
    p_tgt_msg->rim_info.bitmask |= RIM_INFO_ACK_SEQ_NUM_PRESENT;

    if (p_src_msg->bitmask & RIM_INFO_ACK_PROTOCOL_VER_PRESENT) {
        p_tgt_msg->rim_info.rim_protocol_ver.rim_protocol_ver = p_src_msg->ver;
        p_tgt_msg->rim_info.bitmask |= RIM_INFO_ACK_PROTOCOL_VER_PRESENT_INTL;
    }

    RRC_S1AP_UT_TRACE_EXIT();     
    return return_val;
}


/*****************************************************************************
 * Function Name  : process_s1ap_rrm_rim_info_ack 
 * Inputs         : p_api - pointer to the input API buffer  
 *                  p_s1ap_gb_ctx - pointer to S1AP global context
 * Outputs        : none 
 * Returns        : none 
 * Description    : This function processes RRM RIM INFO PDU for Acklowleged mode
 *                  and sends this message to MME
 *****************************************************************************/

void process_s1ap_rrm_rim_info_ack(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    rim_information_ack_t  msg;
    ran_information_ack_rim_t temp_msg;

    S32 length_read = RRC_NULL;

    U32 msg_length = 0;
    U8 *p_msg;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&msg,
            RRC_NULL,
            sizeof(rim_information_ack_t));
    memset_wrapper(&temp_msg,
            RRC_NULL,
            sizeof(ran_information_ack_rim_t));
    do
    {
      /* Precondition - p_api should point to right message */
      if (RRC_SUCCESS != rrc_il_parse_rim_information_ack(
            &msg,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }

      /*Compose TLV message*/
      compose_rim_info_ack(&msg, &temp_msg);

      /*Encode TLV message*/
      //encode_rim_info_req(&temp_msg);
    
      /* Allocate buffer */
      p_msg = rrc_mem_get( sizeof(U8) * MAX_BSSGP_PDU_LEN );
      if ( p_msg == PNULL){
          return ;
       }

      if (RRC_FAILURE == rrc_compose_ran_information_ack_rim (&p_msg, &temp_msg, &msg_length))
      {
          rrc_mem_free(p_msg);
          return ;
      }

      /*Send message*/
       send_rim_info_ack(p_msg, msg_length, &msg, p_s1ap_gb_ctx);

	  /*Bug 4091 Fix Start*/
	  if (p_msg != PNULL)
      {
        rrc_mem_free(p_msg);
        p_msg = PNULL;
      }
	  /*Bug 4091 Fix End*/

      }while(0);
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : compose_rim_info_app_error 
 * Inputs         : p_src_msg - pointer to RAN-INFORMATION-ERROR RIM PDU
 *                  p_tgt_msg - pointer to RAN-INFORAMATION-APPLICATION-ERROR 
 *                              RIM PDU
 * Outputs        : p_tgt_msg 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS 
 * Description    : This function composes  RRM RIM INFO ERROR PDU 
 *****************************************************************************/
rrc_return_et compose_rim_info_app_error(
        rim_information_app_err_t* p_src_msg,
        ran_information_app_error_rim_t* p_tgt_msg)
{
    RRC_S1AP_UT_TRACE_ENTER();
    rrc_return_et return_val = RRC_SUCCESS;
    U16 length = 0;
    U8 index = 0, num_si_psi = 0, no_of_octets = 0, temp = 0;
    
    p_tgt_msg->pdu_type = RIM_RAN_INFO_APP_ERR_PDU;

    if (p_src_msg->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_GERAN_CELL_OCTETS;
    }
    else if (p_src_msg->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->dst_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->dst_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        p_tgt_msg->dst_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_RNC_ID_OCTETS;
    }
    else if (p_src_msg->dst_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->dst_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->dst_cell_id.cell_identifier,
                &p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->dst_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->dst_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->dst_cell.eutran_rtng_addr.enb, &p_tgt_msg->dst_cell_id);
    }
    
    if (p_src_msg->src_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->src_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);
        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_GERAN_CELL_OCTETS;
    }
    else if (p_src_msg->src_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_src_msg->src_cell.utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                &p_src_msg->src_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        p_tgt_msg->src_cell_id.length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_RNC_ID_OCTETS;

    }
    else if (p_src_msg->src_cell.bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_tgt_msg->src_cell_id.routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_tgt_msg->src_cell_id.cell_identifier,
                &p_src_msg->src_cell.eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_tgt_msg->src_cell_id.cell_identifier + RIM_PLMN_OCTETS)),
                &p_src_msg->src_cell.eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_src_msg->src_cell.eutran_rtng_addr.enb, &p_tgt_msg->src_cell_id);
    }
    
    p_tgt_msg->bitmask |= RIM_INFO_APP_ERROR_PRESENT;
    p_tgt_msg->rim_app_error.rim_app_id.rim_app_id = p_src_msg->app_idty;
    if (!(p_src_msg->app_idty)){
        RRC_S1AP_TRACE (RRC_WARNING, "Wrong APP IDNTY \n");
        return RRC_FAILURE;
    }
    p_tgt_msg->rim_app_error.bitmask |= RIM_INFO_APP_ERROR_APP_ID_PRESENT;
    
    p_tgt_msg->rim_app_error.rim_seq_num.rim_seq_num = p_src_msg->seq_num;
    p_tgt_msg->rim_app_error.bitmask |= RIM_INFO_APP_ERROR_SEQ_NUM_PRESENT;

    p_tgt_msg->rim_app_error.rim_pdu_ind.rim_pdu_ind = p_src_msg->pdu_ind.pdu_ext;
    p_tgt_msg->rim_app_error.rim_pdu_ind.rim_pdu_ind <<= 1;
    
    if (RIM_NO_ACK_REQUESTED == p_src_msg->pdu_ind.ack){
        p_tgt_msg->rim_app_error.rim_pdu_ind.rim_pdu_ind |= RIM_NO_ACK_REQUESTED;
    } else if (RIM_NO_ACK_REQUESTED == p_src_msg->pdu_ind.ack){
        p_tgt_msg->rim_app_error.rim_pdu_ind.rim_pdu_ind |= RIM_ACK_REQUESTED;
    } else {
        RRC_S1AP_TRACE (RRC_WARNING, "Wrong Ack type \n");
    }
    
    p_tgt_msg->rim_app_error.bitmask |= RIM_INFO_APP_ERROR_PDU_IND_PRESENT;
   
    if (p_src_msg->bitmask & RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT) {
        p_tgt_msg->rim_app_error.rim_protocol_ver.rim_protocol_ver = p_src_msg->ver;
        p_tgt_msg->rim_app_error.bitmask |= RIM_INFO_APP_ERROR_PROTOCOL_VER_PRESENT;
    }
     
    if (p_src_msg->err_cont.bitmask & RIM_APP_ERR_CONTAINER_NACC_PRESENT)
    {
        p_tgt_msg->rim_app_error.app_cont.bitmask |= RIM_APP_ERR_CONTAINER_NACC_PRESENT;
        p_tgt_msg->rim_app_error.app_cont.nacc.nacc_cause = p_src_msg->err_cont.nacc.cause;
       
        if (RIM_REQ_NACC_PDU_IN_ERROR_PRESENT == p_src_msg->err_cont.nacc.bitmask)
        {
            l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info,
                    &p_src_msg->err_cont.nacc.req_err_pdu.cell_idty.routing_idty.plmn.plmn,
                    RIM_PLMN_OCTETS);

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + RIM_PLMN_OCTETS),
                    &p_src_msg->err_cont.nacc.req_err_pdu.cell_idty.routing_idty.lac,
                    RIM_LAC_OCTETS);

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + RIM_PLMN_OCTETS + RIM_LAC_OCTETS),
                    &p_src_msg->err_cont.nacc.req_err_pdu.cell_idty.routing_idty.rac,
                    sizeof(U8));

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + 
                          RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE),
                        &p_src_msg->err_cont.nacc.req_err_pdu.cell_idty.geran_cell_idty,
                        RIM_GERAN_CELL_OCTETS);

            p_tgt_msg->rim_app_error.app_cont.nacc.counter = 
                RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + RIM_GERAN_CELL_OCTETS;
        } 
        else if (RIM_INFO_NACC_PDU_IN_ERROR_PRESENT == p_src_msg->err_cont.nacc.bitmask)
        {
            length = 0;
            l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info,
                    &p_src_msg->err_cont.nacc.info_err_pdu.cell_idty.routing_idty.plmn.plmn,
                    RIM_PLMN_OCTETS);
            length += RIM_PLMN_OCTETS;

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + RIM_PLMN_OCTETS),
                    &p_src_msg->err_cont.nacc.info_err_pdu.cell_idty.routing_idty.lac,
                    RIM_LAC_OCTETS);
            length += RIM_LAC_OCTETS;

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + RIM_PLMN_OCTETS + RIM_LAC_OCTETS),
                    &p_src_msg->err_cont.nacc.info_err_pdu.cell_idty.routing_idty.rac,
                    sizeof(U8));
            length += sizeof(U8);

            l3_memcpy_wrapper((p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + 
                          RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE),
                        &p_src_msg->err_cont.nacc.info_err_pdu.cell_idty.geran_cell_idty,
                        RIM_GERAN_CELL_OCTETS);
            length += RIM_GERAN_CELL_OCTETS;

            temp = p_src_msg->err_cont.nacc.info_err_pdu.num_si_psi;
            temp <<= 1;
            temp |= (p_src_msg->err_cont.nacc.info_err_pdu.type & 0x1);

            l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + length, &temp, sizeof(U8));

            length += sizeof (U8);

            if (RIM_SI_MSG == p_src_msg->err_cont.nacc.info_err_pdu.type)
            {
                no_of_octets = RIM_MAX_SI_PAYLOAD_INDIV;
            } 
            else if (RIM_PSI_MSG == p_src_msg->err_cont.nacc.info_err_pdu.type)
            {
                no_of_octets = RIM_MAX_SI_PSI_PAYLOAD_INDIV;
            }

            num_si_psi = p_src_msg->err_cont.nacc.info_err_pdu.num_si_psi;
            for (index = 0; index < num_si_psi; index++){
                l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.nacc.nacc_info + length,
                           p_src_msg->err_cont.nacc.info_err_pdu.si_psi_indiv[index].si_psi,
                           no_of_octets);
                length += no_of_octets;
            }
            p_tgt_msg->rim_app_error.app_cont.nacc.counter = length;
        } 
    }
    if (p_src_msg->err_cont.bitmask & RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT)
    {
        p_tgt_msg->rim_app_error.app_cont.bitmask |= RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT;
        p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_cause = p_src_msg->err_cont.utra_si.cause;
        if (RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT == p_src_msg->err_cont.utra_si.bitmask)
        {
            l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info,
                    &p_src_msg->err_cont.utra_si.req_err_pdu.rpt_cell_id.plmn.plmn,
                    RIM_PLMN_OCTETS);

            l3_memcpy_wrapper(((p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info + RIM_PLMN_OCTETS)),
                    &p_src_msg->err_cont.utra_si.req_err_pdu.rpt_cell_id.cell_id,
                    sizeof(U32));

            p_tgt_msg->rim_app_error.app_cont.utra_si.counter = RIM_PLMN_OCTETS + sizeof(U32);
        }
        else if (RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT == p_src_msg->err_cont.utra_si.bitmask)
        {
            length = 0;
            l3_memcpy_wrapper(p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info,
                    &p_src_msg->err_cont.utra_si.info_err_pdu.rpt_cell_id.plmn.plmn,
                    RIM_PLMN_OCTETS);
            length += RIM_PLMN_OCTETS;

            l3_memcpy_wrapper(((p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info + length)),
                    &p_src_msg->err_cont.utra_si.info_err_pdu.rpt_cell_id.cell_id,
                    sizeof(U32));
            length += sizeof(U32);
            
            l3_memcpy_wrapper(((p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info + length)),
                    &p_src_msg->err_cont.utra_si.info_err_pdu.len,
                    sizeof (U16));
            length += sizeof (U16);
            
            l3_memcpy_wrapper(((p_tgt_msg->rim_app_error.app_cont.utra_si.utra_si_info + length)),
                    p_src_msg->err_cont.utra_si.info_err_pdu.si,
                    p_src_msg->err_cont.utra_si.info_err_pdu.len);
            length += p_src_msg->err_cont.utra_si.info_err_pdu.len;

            p_tgt_msg->rim_app_error.app_cont.utra_si.counter = length;
        }
    }
    if ((p_src_msg->err_cont.bitmask & RIM_APP_ERR_CONTAINER_SI3_PRESENT) ||
       (p_src_msg->err_cont.bitmask & RIM_APP_ERR_CONTAINER_MBMS_PRESENT) ||
       (p_src_msg->err_cont.bitmask & RIM_APP_ERR_CONTAINER_SON_PRESENT)) {
           RRC_S1AP_TRACE(RRC_WARNING, "Not supported RIM INFO REQ APP CONTAINER.\n");
        }

    RRC_S1AP_UT_TRACE_EXIT();
    return return_val;
}


/*****************************************************************************
 * Function Name  : process_s1ap_rrm_rim_info_app_error 
 * Inputs         : p_api - pointer to input API buffer  
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : none 
 * Returns        : none 
 * Description    : This function processes  RRM RIM INFO APPLICATION ERROR 
 *                  and sends this msg to MME
 *****************************************************************************/
void process_s1ap_rrm_rim_info_app_error(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    rim_information_app_err_t msg;
    ran_information_app_error_rim_t temp_msg;

    S32 length_read = RRC_NULL;

    U32 msg_length = 0;
    U8 *p_msg;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&msg,
            RRC_NULL,
            sizeof(rim_information_app_err_t));
    memset_wrapper(&temp_msg,
            RRC_NULL,
            sizeof(ran_information_app_error_rim_t));
    do
    {
      /* Precondition - p_api should point to right message */
      if (RRC_SUCCESS != rrc_il_parse_rim_information_app_err(
            &msg,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }

      /*Compose TLV message*/
      if (RRC_FAILURE == compose_rim_info_app_error(&msg, &temp_msg))
      {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
      }

      /* Allocate buffer */
      p_msg = rrc_mem_get( sizeof(U8) * MAX_BSSGP_PDU_LEN );
      if ( p_msg == PNULL){
          return ;
      }
      

      if (RRC_FAILURE == rrc_compose_ran_information_app_error_rim (&p_msg, &temp_msg, &msg_length ))
      {
          rrc_mem_free(p_msg);
          return ;
      }

      /*Send message*/
       send_rim_info_app_error(p_msg, msg_length, &msg, p_s1ap_gb_ctx);


	  /*Bug 4091 Fix Start*/
	  if (p_msg != PNULL)
      {
        rrc_mem_free(p_msg);
        p_msg = PNULL;
      }
	  /*Bug 4091 Fix End*/

      }while(0);

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : send_rim_info_app_error 
 * Inputs         : p_trg_msg - pointer to RAN-INFORMATION-APPLICATION-ERROR
 *                              data  
 *                  msg_length - length of message
 *                  message - pointer to RIM-INFORMATION-APPLICATION ERROR data  
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : none 
 * Returns        : none 
 * Description    : This function sends  RRM RIM INFO APPLICATION ERROR to 
 *                  MME
 *****************************************************************************/
void send_rim_info_app_error(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_information_app_err_t *message, 
                    s1ap_gb_context_t         *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_direct_info_transfer_t    enb_info_msg;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    mme_context_t *p_mme_context = PNULL;
    U16 asn_msg_len  = RRC_NULL;
    U8 mme_id=0;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&enb_info_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_enb_direct_info_transfer_t));
    if (message->dst_cell.bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.ci,
                (const void *)message->dst_cell.geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);

	/*SPR 19697 Fix Start*/
        s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.geran_cell_id.lai.plmn_identity, 
	/*SPR 19697 Fix End*/
               message->dst_cell.geran_rtng_addr.routing_idty.plmn.plmn);        

        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.lai.location_area_code,
                (const void *)message->dst_cell.geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        enb_info_msg.geran_cell_id.routing_area_code = message->dst_cell.geran_rtng_addr.
            routing_idty.rac;

        enb_info_msg.bitmask |= GERAN_CELL_ID_PRESENT;
    }
    else if (message->dst_cell.bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        enb_info_msg.target_rnc_id.routing_area_code = message->dst_cell.
            utran_rtng_addr.routing_idty.rac;

        l3_memcpy_wrapper((void *)enb_info_msg.target_rnc_id.lai.location_area_code,
                (const void *)message->dst_cell.utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);
        //PLMN //
	/*SPR 19697 Fix Start*/
        s1ap_generate_plmn_identity_for_rim_to_int(&enb_info_msg.target_rnc_id.lai.plmn_identity, 
	/*SPR 19697 Fix End*/
               message->dst_cell.utran_rtng_addr.routing_idty.plmn.plmn);        
        
        l3_memcpy_wrapper((void *)&enb_info_msg.target_rnc_id.rnc_id,
                (const void *)message->dst_cell.utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        
        enb_info_msg.target_rnc_id.bitmask |= RRM_RNC_ROUTING_AREA_CODE_PRESENT;
        enb_info_msg.bitmask |= TARGET_RNC_ID_PRESENT; 
    }
    
    enb_info_msg.mme_id = message->mme_id;
    enb_info_msg.len_rim_info = msg_length;
    l3_memcpy_wrapper((void *)enb_info_msg.rim_info, (const void *)p_trg_msg,
           msg_length);

    do
    {

        if (RRC_SUCCESS == compose_asn_enb_direct_info_transfer(
                    &enb_info_msg,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            /* If decode success, send message to MME */

            mme_id = enb_info_msg.mme_id;
            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Direct Info Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Direct Info Tranfer failed", mme_id);
                break;
            }

            /* SPR 2793 Start */
            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Direct Information Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_direct_info_transfer "
                    "returned failure",__FUNCTION__);
        }
    } while (0);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_meas_init_resp
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP
 *                  msg and sends this msg to MME
 ****************************************************************************/
void process_s1ap_rrm_lppa_meas_init_resp(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    static s1ap_rrm_lppa_ecid_meas_init_resp_t  lppa_meas_init_resp;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U32 mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U32 enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_meas_init_resp, RRC_NULL,
               sizeof(s1ap_rrm_lppa_ecid_meas_init_resp_t));

    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_ecid_meas_init_resp(
                   &lppa_meas_init_resp,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }
    
    SET_UE_INDEX(lppa_meas_init_resp.ue_index);

    RRC_S1AP_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", lppa_meas_init_resp.ue_index);
    if (RRC_SUCCESS != 
          lppa_encode_E_CIDMeasurementInitiationResponse(
                             lppa_pdu_asn_msg_buff,
                             &asn_lppa_pdu_msg_len, 
                             &lppa_meas_init_resp))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_encode_E_CIDMeasurementInitiationResponse failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    enb_ue_s1ap_id = lppa_meas_init_resp.ue_index;
    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                 &p_s1ap_context, enb_ue_s1ap_id);
    if ( p_s1ap_context  != PNULL )
    {
        mme_ue_s1ap_id = p_s1ap_context->mme_ue_s1ap_id;
    }
    else
    {
        RRC_S1AP_TRACE (RRC_ERROR, "UE Context from UE ID returned NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
            s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                mme_ue_s1ap_id, enb_ue_s1ap_id, lppa_meas_init_resp.routing_id,
                asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_meas_init_resp.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkUEAssociatedLPPaTransport",lppa_meas_init_resp.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_meas_init_resp.mme_id);
        return;
    }

    if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_meas_init_resp.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_meas_init_fail
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL  
 *                  msg and sends to MME 
 ****************************************************************************/
void process_s1ap_rrm_lppa_meas_init_fail(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_lppa_ecid_meas_init_fail_t  lppa_meas_init_fail;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U32 mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U32 enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_meas_init_fail, RRC_NULL,
               sizeof(s1ap_rrm_lppa_ecid_meas_init_fail_t));
    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_ecid_meas_init_fail(
                   &lppa_meas_init_fail,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }

    SET_UE_INDEX(lppa_meas_init_fail.ue_index);

    RRC_S1AP_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", lppa_meas_init_fail.ue_index);
    if (RRC_SUCCESS != 
          lppa_encode_E_CIDMeasurementInitiationFailure(lppa_pdu_asn_msg_buff,
                             &asn_lppa_pdu_msg_len, &lppa_meas_init_fail, RRC_NULL))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_encode_E_CIDMeasurementInitiationFailure failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    enb_ue_s1ap_id = lppa_meas_init_fail.ue_index;
    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                 &p_s1ap_context, enb_ue_s1ap_id);
    if ( p_s1ap_context  != PNULL )
    {
        mme_ue_s1ap_id = p_s1ap_context->mme_ue_s1ap_id;
    }
    else
    {
        RRC_S1AP_TRACE (RRC_ERROR, "UE Context from UE ID returned NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
            s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                mme_ue_s1ap_id, enb_ue_s1ap_id, lppa_meas_init_fail.routing_id,
                asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_meas_init_fail.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkUEAssociatedLPPaTransport",lppa_meas_init_fail.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_meas_init_fail.mme_id);
        return;
    }

    if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_meas_init_fail.mme_id);
        return;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_meas_fail_ind
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND  
 *                  msg and sends this msg to MME
 ****************************************************************************/
void process_s1ap_rrm_lppa_meas_fail_ind(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_lppa_ecid_meas_fail_ind_t  lppa_meas_fail_ind;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U32 mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U32 enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_meas_fail_ind, RRC_NULL,
               sizeof(s1ap_rrm_lppa_ecid_meas_fail_ind_t));
    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_ecid_meas_fail_ind(
                   &lppa_meas_fail_ind,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }

    SET_UE_INDEX(lppa_meas_fail_ind.ue_index);

    RRC_S1AP_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", lppa_meas_fail_ind.ue_index);
    if (RRC_SUCCESS != 
          lppa_encode_E_CIDMeasurementFailureIndication(lppa_pdu_asn_msg_buff,
                             &asn_lppa_pdu_msg_len, &lppa_meas_fail_ind))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_encode_E_CIDMeasurementFailureIndication failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    enb_ue_s1ap_id = lppa_meas_fail_ind.ue_index;
    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                 &p_s1ap_context, enb_ue_s1ap_id);
    if ( p_s1ap_context  != PNULL )
    {
        mme_ue_s1ap_id = p_s1ap_context->mme_ue_s1ap_id;
    }
    else
    {
        RRC_S1AP_TRACE (RRC_ERROR, "UE Context from UE ID returned NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
            s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                mme_ue_s1ap_id, enb_ue_s1ap_id, lppa_meas_fail_ind.routing_id,
                asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_meas_fail_ind.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkUEAssociatedLPPaTransport",lppa_meas_fail_ind.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_meas_fail_ind.mme_id);
        return;
    }

    if (RRC_SUCCESS != send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_meas_fail_ind.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_meas_report
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the lppa measurement report.
 ****************************************************************************/
void process_s1ap_rrm_lppa_meas_report(
        void       	     *p_api,
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    static s1ap_rrm_lppa_ecid_meas_report_t  lppa_meas_report;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U32 mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U32 enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_meas_report, RRC_NULL,
               sizeof(s1ap_rrm_lppa_ecid_meas_report_t));
    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_ecid_meas_report(
                   &lppa_meas_report,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }

    SET_UE_INDEX(lppa_meas_report.ue_index);

    RRC_S1AP_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", lppa_meas_report.ue_index);
    if (RRC_SUCCESS !=
          lppa_encode_E_CIDMeasurementReport(lppa_pdu_asn_msg_buff,	
                             &asn_lppa_pdu_msg_len, &lppa_meas_report))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "lppa_encode_E_CIDMeasurementReport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    enb_ue_s1ap_id = lppa_meas_report.ue_index;
    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                 &p_s1ap_context, enb_ue_s1ap_id);
    if ( p_s1ap_context  != PNULL )
    {
        mme_ue_s1ap_id = p_s1ap_context->mme_ue_s1ap_id;
    }
    else
    {
        RRC_S1AP_TRACE (RRC_ERROR, "UE Context from UE ID returned NULL");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
            s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                mme_ue_s1ap_id, enb_ue_s1ap_id, lppa_meas_report.routing_id,
                asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_meas_report.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkUEAssociatedLPPaTransport",lppa_meas_report.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_meas_report.mme_id);
        return;
    }

    if (RRC_SUCCESS != send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_meas_report.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_otdoa_info_resp
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the llpa Otoda response message.
 ****************************************************************************/
void process_s1ap_rrm_lppa_otdoa_info_resp(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    static s1ap_rrm_lppa_otdoa_info_resp_t  lppa_otdoa_info_resp;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_otdoa_info_resp, RRC_NULL,
               sizeof(s1ap_rrm_lppa_otdoa_info_resp_t));

    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_otdoa_info_resp(
            &lppa_otdoa_info_resp,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }

    if(RRC_SUCCESS != 
         lppa_internal_encode_otdoa_information_resp(
                            &lppa_otdoa_info_resp,
                            lppa_pdu_asn_msg_buff,
                            &asn_lppa_pdu_msg_len))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_internal_encode_otdoa_information_resp failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
        s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
            lppa_otdoa_info_resp.routing_id,
            asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
            &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_otdoa_info_resp.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkNonUEAssociatedLPPaTransport", lppa_otdoa_info_resp.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if(MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_otdoa_info_resp.mme_id);
        return;
    }

    if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkNonUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_otdoa_info_resp.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_otdoa_info_fail
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the Otdoa Failure message.
 ****************************************************************************/
void process_s1ap_rrm_lppa_otdoa_info_fail(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_lppa_otdoa_info_fail_t  lppa_otdoa_info_fail;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_otdoa_info_fail, RRC_NULL,
               sizeof(s1ap_rrm_lppa_otdoa_info_fail_t));

    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_otdoa_info_fail(
            &lppa_otdoa_info_fail,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if(RRC_SUCCESS != 
         lppa_internal_encode_otdoa_information_fail(
                            &lppa_otdoa_info_fail,
                            lppa_pdu_asn_msg_buff,
                            &asn_lppa_pdu_msg_len))
    {
        RRC_S1AP_TRACE (RRC_ERROR, 
            "lppa_internal_encode_otdoa_information_fail failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS !=
        s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
            lppa_otdoa_info_fail.routing_id,
            asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
            &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
            "s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_otdoa_info_fail.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkNonUEAssociatedLPPaTransport", lppa_otdoa_info_fail.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_otdoa_info_fail.mme_id);
        return;
    }

    /* Send message to MME */
    if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkNonUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_otdoa_info_fail.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_lppa_error_ind_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the llpa Error Indication message
 ****************************************************************************/
void process_s1ap_rrm_lppa_error_ind_req(
        void             *p_api,
        s1ap_gb_context_t    *p_s1ap_gb_ctx)
{
    s1ap_rrm_lppa_error_ind_req_t lppa_error_ind;
    S32 length_read = RRC_NULL;
    U8  lppa_pdu_asn_msg_buff[LPPA_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_lppa_pdu_msg_len = 0;
    U16 asn_s1ap_pdu_msg_len = 0;
    U8  s1ap_pdu_asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN] = {0};
    U32 mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U32 enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t *p_s1ap_context    = PNULL;
    mme_context_t *p_mme_context = PNULL;

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&lppa_error_ind, RRC_NULL, sizeof(s1ap_rrm_lppa_error_ind_req_t));

    if (RRC_SUCCESS != rrc_il_parse_s1ap_rrm_lppa_error_ind_req(
                &lppa_error_ind,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Bad message - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(lppa_error_ind.ue_index);

    RRC_S1AP_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", lppa_error_ind.ue_index);
    if( !( (lppa_error_ind.cause.type == T_lppa_Cause_protocol) &&
           ((lppa_error_ind.cause.value == lppa_message_not_compatible_with_receiver_state) || 
             (lppa_error_ind.cause.value == lppa_semantic_error)) ) )
    {
        RRC_S1AP_TRACE (RRC_WARNING, "Invalid values received.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
      
    if(RRC_SUCCESS !=
            lppa_encode_ErrorIndication(lppa_pdu_asn_msg_buff,
                        &asn_lppa_pdu_msg_len, &lppa_error_ind))
    {
        RRC_S1AP_TRACE (RRC_ERROR,
                "lppa_encode_ErrorIndication failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    if (LPPA_ERROR_IND_UE_INDEX_PRESENT & lppa_error_ind.bitmask)
    {
        enb_ue_s1ap_id = lppa_error_ind.ue_index;
        get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                &p_s1ap_context, enb_ue_s1ap_id);
        if ( p_s1ap_context  != PNULL )
        {
            mme_ue_s1ap_id = p_s1ap_context->mme_ue_s1ap_id;
        }
        else
        {
            RRC_S1AP_TRACE (RRC_ERROR, "UE Context from UE ID returned NULL");
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        if (RRC_SUCCESS !=
                s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
                    mme_ue_s1ap_id, enb_ue_s1ap_id, lppa_error_ind.routing_id,
                    asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                    &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
        {
            RRC_S1AP_TRACE (RRC_ERROR,
                    "s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport failed.");
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }
    }
    else
    {
        if (RRC_SUCCESS !=
                s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
                    lppa_error_ind.routing_id,
                    asn_lppa_pdu_msg_len, lppa_pdu_asn_msg_buff,
                    &asn_s1ap_pdu_msg_len, s1ap_pdu_asn_msg_buff))
        {
            RRC_S1AP_TRACE (RRC_ERROR,
                    "s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport failed.");
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }
    }

    /* Send message to MME */
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[lppa_error_ind.mme_id];

    if(PNULL == p_mme_context)
    {
        /* Pointer Null */
        RRC_S1AP_TRACE(RRC_WARNING,
            "MME Context for mme_id %d is Null, Unable to send"
            " s1ap_UplinkUEAssociatedLPPaTransport",lppa_error_ind.mme_id);
        return;
    }

    /* Check if the MME is in active state */
    if (MME_ACTIVE != p_mme_context->current_mme_state)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
          "MME %d is not in active state", lppa_error_ind.mme_id);
        return;
    }

    if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        s1ap_pdu_asn_msg_buff,
                        asn_s1ap_pdu_msg_len,
                        PNULL))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "s1ap_UplinkUEAssociatedLPPaTransport Message"
                " failed to MME ID %d", lppa_error_ind.mme_id);
        return;
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*SPR 19183 START*/
/*****************************************************************************
 * Function Name  : send_rim_info_error 
 * Inputs         : p_trg_msg     - pointer to ran_information_req_rim data 
 *                  msg_length    - length of message   
 *                  p_dest_cell   - Pointer to destination cell
 *                  mme_id        - MME ID
 *                  p_s1ap_gb_ctx - pointer to S1AP global context
 * Outputs        : None 
 * Returns        : none  
 * Description    : This function fills the ENB direct info msg in to a 
 *                  msssage and sends to  MME
 ****************************************************************************/
void send_rim_info_error(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_routing_info_t       *p_dest_cell,
                    U8                       mme_id,
                    s1ap_gb_context_t        *p_s1ap_gb_ctx)
{
    s1ap_rrm_enb_direct_info_transfer_t    enb_info_msg;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN]  = {0};
    mme_context_t *p_mme_context = PNULL;
    U16 asn_msg_len  = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&enb_info_msg, 
            RRC_NULL,
            sizeof(s1ap_rrm_enb_direct_info_transfer_t));
    if (p_dest_cell->bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
    {
        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.ci,
                (const void *)p_dest_cell->geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);

        s1ap_generate_plmn_identity_to_int(&enb_info_msg.geran_cell_id.lai.plmn_identity, 
               p_dest_cell->geran_rtng_addr.routing_idty.plmn.plmn);        

        l3_memcpy_wrapper((void *)enb_info_msg.geran_cell_id.lai.location_area_code,
                (const void *)p_dest_cell->geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        enb_info_msg.geran_cell_id.routing_area_code = p_dest_cell->geran_rtng_addr.
            routing_idty.rac;
        
        enb_info_msg.bitmask |= GERAN_CELL_ID_PRESENT;
    }
    if (p_dest_cell->bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
    {
        enb_info_msg.target_rnc_id.routing_area_code = p_dest_cell->
            utran_rtng_addr.routing_idty.rac;

        l3_memcpy_wrapper((void *)enb_info_msg.target_rnc_id.lai.location_area_code,
                (const void *)p_dest_cell->utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);
        //PLMN //
        s1ap_generate_plmn_identity_to_int(&enb_info_msg.target_rnc_id.lai.plmn_identity, 
               p_dest_cell->utran_rtng_addr.routing_idty.plmn.plmn);        
        
        enb_info_msg.target_rnc_id.rnc_id = p_dest_cell->utran_rtng_addr.rnc_id[0];
        enb_info_msg.target_rnc_id.rnc_id = enb_info_msg.target_rnc_id.rnc_id << 8;
        /* CID 109070 fix start*/ 
        enb_info_msg.target_rnc_id.rnc_id = enb_info_msg.target_rnc_id.rnc_id | p_dest_cell->utran_rtng_addr.rnc_id[1];
        /* CID 109070 fix end*/ 
        
        enb_info_msg.target_rnc_id.bitmask |= RRM_RNC_ROUTING_AREA_CODE_PRESENT;
        enb_info_msg.bitmask |= TARGET_RNC_ID_PRESENT; 
    }
    
    enb_info_msg.mme_id = mme_id;
    enb_info_msg.len_rim_info = msg_length;
    l3_memcpy_wrapper((void *)enb_info_msg.rim_info, (const void *)p_trg_msg,
           msg_length);

    do
    {

        if (RRC_SUCCESS == compose_asn_enb_direct_info_transfer(
                    &enb_info_msg,
                    asn_msg_buff,
                    &asn_msg_len))
        {
            /* If decode success, send message to MME */

            /* fetch MME Context */
            p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

            if(PNULL == p_mme_context)
            {
                /* Pointer Null */
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME Context for mme_id %d is Null, Unable to"
                        " send the eNB Direct Info Transfer",mme_id);
                break;
            }

            /* Check if the MME is in active state */
            if (MME_ACTIVE != p_mme_context->current_mme_state)
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "MME %d is not in active state, "
                        "eNB Direct Info Tranfer failed", mme_id);
                break;
            }

            if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                        asn_msg_buff,
                        asn_msg_len,
                        PNULL))
            {
                RRC_S1AP_TRACE(RRC_WARNING,
                        "eNB Direct Information Transfer Message"
                        " failed to MME ID %d", mme_id);
                break;
            }

            RRC_S1AP_TRACE(RRC_INFO, "eNB Direct Information Transfer"
                    " message sent to MME %d", mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "compose_asn_enb_direct_info_transfer "
                    "returned failure",__FUNCTION__);
        }
    } while (0);

    RRC_S1AP_UT_TRACE_EXIT();
}
/*SPR 19183 END*/
